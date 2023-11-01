// _____________________________________________________________________________
//
//  battle/logic.c - turn based battle logic and implementations of skills
// _____________________________________________________________________________
//
#include "../common.h"

// _____________________________________________________________________________
//
//  Get actual stats of a phone in battle, taking stat changes and condition
//  multipliers into account (helper function)
//  These can be used outside of battles by setting bp to NULL
// _____________________________________________________________________________
//
float getAttack(Phone *ph, BattlePhone *bp) {
    float result = ph->attack;
    result *= condMultipliers[ph->screenStatus];
    if (!bp) return result;

    if (bp->atkUpAmount > 0) {
        for (int i = 0; i < bp->atkUpAmount; i++) result *= 1.2f;
    }
    else if (bp->atkUpAmount < 0) {
        bp->atkUpAmount *= -1;
        for (int i = 0; i < bp->atkUpAmount; i++) result /= 1.2f;
        bp->atkUpAmount *= -1;
    }
    return result;
}

float getDefense(Phone *ph, BattlePhone *bp) {
    float result = ph->defense;
    result *= condMultipliers[ph->boardStatus];
    if (!bp) return result;

    if (bp->defUpAmount > 0) {
        for (int i = 0; i < bp->defUpAmount; i++) result *= 1.2f;
    }
    else if (bp->defUpAmount < 0) {
        bp->defUpAmount *= -1;
        for (int i = 0; i < bp->defUpAmount; i++) result /= 1.2f;
        bp->defUpAmount *= -1;
    }
    return result;
}

float getWeight(Phone *ph) {
    float result = ph->weight;
    result *= condMultipliers[ph->coverStatus];
    return result;
}

// Returns a value 0-100
int getHitChance(BattlePhone *bp, SkillEffect effect) {
    float result = effect.chance;

    if (bp->accuracyUpAmount > 0) {
        for (int i = 0; i < bp->accuracyUpAmount; i++) result *= 1.2f;
    }
    else if (bp->accuracyUpAmount < 0) {
        bp->accuracyUpAmount *= -1;
        for (int i = 0; i < bp->accuracyUpAmount; i++) result /= 1.2f;
        bp->accuracyUpAmount *= -1;
    }
    return (int) result;
}

// _____________________________________________________________________________
//
//  Battle menu - select who moves first (helper function)
// _____________________________________________________________________________
//
int whoMovesFirst(Phone *ph1, Phone *ph2) {
    float w1 = (float) getWeight(ph1);
    float w2 = (float) getWeight(ph2);

    float ratio = w1 / w2;
    if (ratio < 1.0f) ratio = powf(ratio, 12.0f);
    else ratio = powf(ratio, 4.0f);

    float rng = (float) rand() / RAND_MAX * 2.0f;
    return rng < ratio;
}

// _____________________________________________________________________________
//
//  Battle menu - calculate damage (helper function)
// _____________________________________________________________________________
//
int getDamage(Phone *attacker, Phone *victim, BattlePhone *attackerB, BattlePhone *victimB, int damage, SkillType type) {
    // Initial calculation based on attack / defense, taking into account stat modifiers
    float result = (float) damage * getAttack(attacker, attackerB) / getDefense(victim, victimB);

    // Skill type and phone's corresponding condition for that type
    int status;
    switch (type) {
        case SKT_BATTERY: status = attacker->batteryStatus; break;
        case SKT_BOARD: status = attacker->boardStatus; break;
        case SKT_COVER: status = attacker->coverStatus; break;
        case SKT_SCREEN: status = attacker->screenStatus; break;
    }
    switch (status) {
        case COND_BROKEN: result *= 0.7f; break;
        case COND_BAD: result *= 0.9f; break;
        case COND_GOOD: result *= 1.1f; break;
    }

    schedSound("hit", 40);

    return MIN((int) result, victim->hp);
}

// _____________________________________________________________________________
//
//  Battle menu - do move (helper function)
// _____________________________________________________________________________
//
void doMove(Phone *attacker, Phone *victim, BattlePhone *attackerB, BattlePhone *victimB, SkillSpecs skill) {
    if (attackerB->confusedTurns && GetRandomValue(0, 100) < 40) {
        strcpy(MENU.battleTextbox[1], "But it failed due to confusion!");
        schedSound("miss", 40);
    }
    else {
        for (int i = 0; i < 2; i++) {
            // Chance to miss
            // If the first/primary effect of the skill misses, the secondary
            // effect is not even processed (returns from function)
            if (GetRandomValue(1, 100) > getHitChance(attackerB, skill.effects[i])) {
                if (i == 0) {
                    strcpy(MENU.battleTextbox[1], "But it missed!");
                    schedSound("miss", 40);
                }
                return;
            }

            // Set up attack animation
            // Animation can be cancelled by setting MENU.attackAnim[0] = 0;
            strcpy(MENU.attackAnim, skill.animation);
            MENU.attackAnimTarget = skill.animationTarget;
            if (attacker == &ENEMYP) MENU.attackAnimTarget = !MENU.attackAnimTarget;
            MENU.attackAnimTimer = -40;  // once it reaches 0, anim will play

            switch (skill.effects[i].effect) {
                case SE_NONE: {
                    if (i == 0) strcpy(MENU.battleTextbox[1], "No effect!");
                    break;
                }

                case SE_DAMAGE: {
                    int damage = getDamage(
                        attacker, victim, attackerB, victimB,
                        skill.effects[i].parameter, skill.type
                    );
                    victim->hp -= damage;
                    sprintf(
                        MENU.battleTextbox[i + 1],
                        "%s %s took %d damage!",
                        SPECS(victim->id).brand, SPECS(victim->id).model, damage
                    );
                    break;
                }

                case SE_SELF_DAMAGE: {
                    int damage = getDamage(
                        attacker, attacker, attackerB, attackerB,
                        skill.effects[i].parameter, skill.type
                    );
                    attacker->hp -= damage;
                    sprintf(
                        MENU.battleTextbox[i + 1],
                        "%s %s dealt %d damage to itself!",
                        SPECS(attacker->id).brand, SPECS(attacker->id).model, damage
                    );
                    break;
                }

                case SE_HEAL: {
                    int amount = skill.effects[i].parameter;
                    attacker->hp = MIN(attacker->hp + amount, attacker->maxHP);
                    sprintf(
                        MENU.battleTextbox[i + 1],
                        "%s %s healed by %d HP!",
                        SPECS(attacker->id).brand, SPECS(attacker->id).model, amount
                    );
                    break;
                }
                
                case SE_DRAIN: {
                    int damage = getDamage(
                        attacker, victim, attackerB, victimB,
                        skill.effects[i].parameter, skill.type
                    );
                    victim->hp -= damage;
                    attacker->hp = MIN(attacker->hp + damage, attacker->maxHP);
                    sprintf(
                        MENU.battleTextbox[i + 1],
                        "Absorbed %d HP from %s %s!",
                        damage, SPECS(victim->id).brand, SPECS(victim->id).model
                    );
                    break;
                }

                case SE_CONFUSE: {
                    if (victimB->confusedTurns) {
                        sprintf(
                            MENU.battleTextbox[i + 1],
                            "%s %s is already confused!",
                            SPECS(victim->id).brand, SPECS(victim->id).model
                        );
                        MENU.attackAnim[0] = 0;
                    }
                    else {
                        victimB->confusedTurns = GetRandomValue(3, 6);
                        sprintf(
                            MENU.battleTextbox[i + 1],
                            "%s %s is now confused!",
                            SPECS(victim->id).brand, SPECS(victim->id).model
                        );
                        // TODO: confuse sound
                    }
                    break;
                }

                case SE_ATK_UP: {
                    if (attackerB->atkUpAmount < 3) {
                        attackerB->atkUpAmount++;
                        attackerB->atkUpTurns = skill.effects[i].parameter + 1;
                        sprintf(
                            MENU.battleTextbox[i + 1],
                            "%s %s has boosted its attack!",
                            SPECS(attacker->id).brand, SPECS(attacker->id).model
                        );
                        schedSound("stat_up", 40 + i*20);
                    }
                    else {
                        sprintf(
                            MENU.battleTextbox[i + 1],
                            "But its attack is already maxed out..."
                        );
                        MENU.attackAnim[0] = 0;
                    }
                    break;
                }

                case SE_DEF_UP: {
                    if (attackerB->defUpAmount < 3) {
                        attackerB->defUpAmount++;
                        attackerB->defUpTurns = skill.effects[i].parameter + 1;
                        sprintf(
                            MENU.battleTextbox[i + 1],
                            "%s %s has boosted its defense!",
                            SPECS(attacker->id).brand, SPECS(attacker->id).model
                        );
                        schedSound("stat_up", 40 + i*20);
                    }
                    else {
                        sprintf(
                            MENU.battleTextbox[i + 1],
                            "But its defense is already maxed out..."
                        );
                        MENU.attackAnim[0] = 0;
                    }
                    break;
                }

                case SE_ACCURACY_UP: {
                    if (attackerB->accuracyUpAmount < 3) {
                        attackerB->accuracyUpAmount += 1;
                        attackerB->accuracyUpTurns = skill.effects[i].parameter + 1;
                        sprintf(
                            MENU.battleTextbox[i + 1],
                            "%s %s has boosted its accuracy!",
                            SPECS(attacker->id).brand, SPECS(attacker->id).model
                        );
                        schedSound("stat_up", 40 + i*20);
                    } else {
                        sprintf(
                            MENU.battleTextbox[i + 1],
                            "But its accuracy is already maxed out..."
                        );
                        MENU.attackAnim[0] = 0;
                    }
                    break;
                }
                
                case SE_ACCURACY_DOWN: {
                    if (victimB->accuracyUpAmount < 3) {
                        victimB->accuracyUpAmount -= 1;
                        victimB->accuracyUpTurns = skill.effects[i].parameter + 1;
                        sprintf(
                            MENU.battleTextbox[i + 1],
                            "%s %s's accuracy fell!",
                            SPECS(victim->id).brand, SPECS(victim->id).model
                        );
                        schedSound("stat_down", 40 + i*20);
                    } else {
                        sprintf(
                            MENU.battleTextbox[i + 1],
                            "But its accuracy can't go any lower..."
                        );
                        MENU.attackAnim[0] = 0;
                    }
                    break;
                }

                case SE_FLEX_RIP: {
                    int damage = getDamage(
                        attacker, victim, attackerB, victimB,
                        skill.effects[i].parameter, skill.type
                    );
                    if (
                        SPECS(victim->id).icon == IC_SLIDER ||
                        SPECS(victim->id).icon == IC_FLIP ||
                        SPECS(victim->id).icon == IC_COMMUNICATOR
                    ) {
                        damage *= 2;
                    }
                    if (damage > victim->hp) damage = victim->hp;
                    victim->hp -= damage;
                    sprintf(
                        MENU.battleTextbox[i + 1],
                        "%s %s took %d damage!",
                        SPECS(victim->id).brand, SPECS(victim->id).model, damage
                    );
                    break;
                }

                case SE_STOMP: {
                    int damage = getDamage(
                        attacker, victim, attackerB, victimB,
                        skill.effects[i].parameter, skill.type
                    );
                    damage = (float) damage * ((float) attacker->weight / (float) victim->weight);
                    if (damage > victim->hp) damage = victim->hp;
                    victim->hp -= damage;
                    sprintf(
                        MENU.battleTextbox[i + 1],
                        "%s %s took %d damage!",
                        SPECS(victim->id).brand, SPECS(victim->id).model, damage
                    );
                    break;
                }

                default: {
                    strcpy(MENU.battleTextbox[i + 1], "This skill effect is not implemented!!");
                    break;
                }
            }
        }
        
        if (PLAYERP.hp < 0) PLAYERP.hp = 0;
        if (ENEMYP.hp < 0) ENEMYP.hp = 0;
    }
}