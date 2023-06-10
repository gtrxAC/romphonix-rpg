#include "../common.h"

// _____________________________________________________________________________
//
//  Battle menu - select who moves first (helper function)
// _____________________________________________________________________________
//
int whoMovesFirst(int weight_1, int weight_2) {
    float w1 = (float) weight_1;
    float w2 = (float) weight_2;

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
    float result = (float) damage * victim->defense / attacker->attack;

    // Stat changes
    if (attackerB->atkUpAmount > 0) {
        for (int i = 0; i < attackerB->atkUpAmount; i++) {
            result *= 1.2f;
        }
    }
    else if (attackerB->atkUpAmount < 0) {
        attackerB->atkUpAmount *= -1;
        for (int i = 0; i < attackerB->atkUpAmount; i++) {
            result /= 1.2f;
        }
        attackerB->atkUpAmount *= -1;
    }

    if (victimB->defUpAmount > 0) {
        for (int i = 0; i < victimB->defUpAmount; i++) {
            result /= 1.2f;
        }
    }
    else if (victimB->defUpAmount < 0) {
        victimB->defUpAmount *= -1;
        for (int i = 0; i < victimB->defUpAmount; i++) {
            result *= 1.2f;
        }
        victimB->defUpAmount *= -1;
    }

    // Skill type and phone condition
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
            if (GetRandomValue(1, 100) > skill.effects[i].chance) {
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
            }
        }
        
        if (PLAYERP.hp < 0) PLAYERP.hp = 0;
        if (ENEMYP.hp < 0) ENEMYP.hp = 0;
    }
}