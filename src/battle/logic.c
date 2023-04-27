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
int getDamage(Game *g, Phone *attacker, Phone *victim, BattlePhone *attackerB, BattlePhone *victimB, SkillSpecs skill) {
    int result = skill.effects[0].effect * victim->defense / attacker->attack;

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
    switch (skill.type) {
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

    return result;
}

// _____________________________________________________________________________
//
//  Battle menu - do move (helper function)
// _____________________________________________________________________________
//
void doMove(Game *g, Phone *attacker, Phone *victim, BattlePhone *attackerB, BattlePhone *victimB, SkillSpecs skill) {
    if (attackerB->confusedTurns && GetRandomValue(0, 100) < 40) {
        strcpy(MENU.battleTextbox[1], "But it failed due to confusion!");
    }
    else for (int i = 0; i < 2; i++) {
        switch (skill.effects[i].effect) {
            case SE_NONE: {
                if (i == 0) strcpy(MENU.battleTextbox[1], "No effect!");
                break;
            }

            case SE_DRAIN: {
                int damage = getDamage(g, attacker, victim, attackerB, victimB, skill);
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
                int damage = getDamage(g, attacker, victim, attackerB, victimB, skill);
                victim->hp -= damage;
                sprintf(
                    MENU.battleTextbox[i + 1],
                    "%s %s took %d damage!",
                    SPECS(victim->id).brand, SPECS(victim->id).model, damage
                );
                break;
            }

            case SE_CONFUSE: {
                if (GetRandomValue(0, 100) < skill.effects[i].parameter) {
                    if (victimB->confusedTurns) {
                        sprintf(
                            MENU.battleTextbox[i + 1],
                            "%s %s is already confused!",
                            SPECS(victim->id).brand, SPECS(victim->id).model
                        );
                    }
                    else {
                        victimB->confusedTurns = GetRandomValue(3, 6);
                        sprintf(
                            MENU.battleTextbox[i + 1],
                            "%s %s is now confused!",
                            SPECS(victim->id).brand, SPECS(victim->id).model
                        );
                    }
                }
                break;
            }

            case SE_ATK_UP: {
                attackerB->atkUpAmount++;
                attackerB->atkUpTurns = 6;
                sprintf(
                    MENU.battleTextbox[i + 1],
                    "%s %s has boosted their attack!",
                    SPECS(attacker->id).brand, SPECS(attacker->id).model
                );
                break;
            }

            case SE_DEF_UP: {
                attackerB->defUpAmount++;
                attackerB->defUpTurns = 6;
                sprintf(
                    MENU.battleTextbox[i + 1],
                    "%s %s has boosted their defense!",
                    SPECS(attacker->id).brand, SPECS(attacker->id).model
                );
                break;
            }
        }
    }
    
    if (PLAYERP.hp < 0) PLAYERP.hp = 0;
    if (ENEMYP.hp < 0) ENEMYP.hp = 0;
}