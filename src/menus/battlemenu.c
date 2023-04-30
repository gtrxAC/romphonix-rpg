// _____________________________________________________________________________
//
//  Battle menu
//  Functions to implement the functionality of the battle menu. In a battle,
//  the player fights with their phone(s) against an NPC's phone(s) or a wild
//  phone.
//  
//  The player and NPC take turns attacking the other side's phone, they can
//  also switch to a different phone or use an item from their bag.
//  In a wild battle, the player can also "Run" to end the battle.
// _____________________________________________________________________________
//
#include "../common.h"

// _____________________________________________________________________________
//
//  Battle menu - init function
// _____________________________________________________________________________
//
void scrBattleMenu(Game *g, bool canRun) {
    pushMenu(g, 0, NULL, CB_NOTHING);
    strcpy(MENU.battleTextbox[0], "Battle doesn't have a textbox set!");
    strcpy(MENU.battleTextbox[1], "Battle doesn't have a textbox set!");
    strcpy(MENU.battleTextbox[2], "Battle doesn't have a textbox set!");

    MENU.updateFunc = updateBattleMenu;
    MENU.drawFunc = drawBattleMenu;
    MENU.nextFunc = checkBattleMenu;

    MENU.battleState = BS_STARTING;
    arrpush(MENU.choices, "Fight");
    arrpush(MENU.choices, "Switch");
    arrpush(MENU.choices, "Items");

    MENU.canRun = canRun;
    if (canRun) arrpush(MENU.choices, "Run");
}

// _____________________________________________________________________________
//
//  Battle menu - update status effect timers (helper function)
// _____________________________________________________________________________
//
void updateStatusTimers(Game *g, int *textboxSlot, Phone *phone, BattlePhone *bPhone) {
    if (bPhone->atkUpTurns > 0) {
        bPhone->atkUpTurns--;
        if (!bPhone->atkUpTurns) {
            sprintf(
                MENU.battleTextbox[*textboxSlot],
                "%s %s's attack boost wore off...",
                SPECS(phone->id).brand, SPECS(phone->id).model
            );
            *textboxSlot++;
        }
    }
    if (bPhone->defUpTurns > 0) {
        bPhone->defUpTurns--;
        if (!bPhone->defUpTurns) {
            sprintf(
                MENU.battleTextbox[*textboxSlot],
                "%s %s's defense boost wore off...",
                SPECS(phone->id).brand, SPECS(phone->id).model
            );
            *textboxSlot++;
        }
    }
    if (bPhone->confusedTurns > 0) {
        bPhone->confusedTurns--;
        if (!bPhone->confusedTurns && *textboxSlot < 3) {
            sprintf(
                MENU.battleTextbox[*textboxSlot],
                "%s %s is no longer confused.",
                SPECS(phone->id).brand, SPECS(phone->id).model
            );
            *textboxSlot++;
        }
    }
}

// _____________________________________________________________________________
//
//  Battle menu - set state (helper function)
// _____________________________________________________________________________
//
void setBattleState(Game *g, BattleState bs) {
    MENU.battleState = bs;

    switch (bs) {
        case BS_WAITING: {
            MENU.choice = 0;
            arrfree(MENU.choices);
            arrpush(MENU.choices, "Fight");
            arrpush(MENU.choices, "Switch");
            arrpush(MENU.choices, "Items");
            if (MENU.canRun) arrpush(MENU.choices, "Run");
            break;
        }

        case BS_WAITING_MOVE: {
            MENU.battleState = BS_WAITING_MOVE;
            MENU.choice = 0;
            arrfree(MENU.choices);
            for (int i = 0; i < 4; i++) {
                arrpush(MENU.choices, SSPECS(PLAYERP.skills[i]).name);
            }
            break;
        }

        case BS_ENEMY_TURN: {
            // for now, enemy always does random skill
            SkillSpecs skill = SSPECS(ENEMYP.skills[GetRandomValue(0, 3)]);
            sprintf(
                MENU.battleTextbox[0], "%s %s uses %s...",
                SPECS(ENEMYP.id).brand, SPECS(ENEMYP.id).model, skill.name
            );
            MENU.battleTextbox[1][0] = '\0';
            MENU.battleTextbox[2][0] = '\0';
            doMove(g, &ENEMYP, &PLAYERP, &MENU.enemy, &MENU.player, skill);
            break;
        }

        case BS_PLAYER_TURN: {
            SkillSpecs skill = SSPECS(PLAYERP.skills[MENU.playerMove]);
            sprintf(
                MENU.battleTextbox[0], "%s %s uses %s...",
                SPECS(PLAYERP.id).brand, SPECS(PLAYERP.id).model, skill.name
            );
            MENU.battleTextbox[1][0] = '\0';
            MENU.battleTextbox[2][0] = '\0';
            doMove(g, &PLAYERP, &ENEMYP, &MENU.player, &MENU.enemy, skill);
            break;
        }

        case BS_AFTER_TURN: {
            for (int i = 0; i < 3; i++) MENU.battleTextbox[i][0] = '\0';
            int textboxSlot = 0;

            updateStatusTimers(g, &textboxSlot, &PLAYERP, &MENU.player);
            updateStatusTimers(g, &textboxSlot, &ENEMYP, &MENU.enemy);

            // if no status effects were processed (nothing to show in textbox),
            // just skip to the next turn
            if (!textboxSlot) setBattleState(g, BS_WAITING);
            break;
        }

        case BS_WON: {
            setSong(g, "assets/sounds/music/thtune.mid");
            strcpy(MENU.battleTextbox[0], "you won, congrats i guess???");
            strcpy(MENU.battleTextbox[1], "");
            strcpy(MENU.battleTextbox[2], "");
            break;
        }

        case BS_LOST: {
            strcpy(MENU.battleTextbox[0], "LOL you lose the battle and cannot switch");
            strcpy(MENU.battleTextbox[1], "phones yet...");
            strcpy(MENU.battleTextbox[2], "");
            break;
        }
    }
}

// _____________________________________________________________________________
//
//  Battle menu - check user input function
// _____________________________________________________________________________
//
void checkBattleMenu(Game *g) {
    switch (MENU.battleState) {
        case BS_WAITING: {
            switch (MENU.choice) {
                case -1: break;

                case 0: { // Fight
                    setBattleState(g, BS_WAITING_MOVE);
                    break;
                }

                case 1: { // Switch

                    break;
                }
                
                case 2: { // Items

                    break;
                }
                
                case 3: { // Run (only visible for wild battles)
                    MENU.battleState = BS_RUN;
                    strcpy(MENU.battleTextbox[0], "Got away safely!");
                    MENU.battleTextbox[1][0] = '\0';
                    MENU.battleTextbox[2][0] = '\0';
                    break;
                }
            }
            break;
        }

        case BS_WAITING_MOVE: {
            if (MENU.choice == -1) {
                setBattleState(g, BS_WAITING); // go back to the start
            }
            else {
                // Choose which side moves first, based on the weights of the
                // phones and a bit of random chance
                MENU.movedFirst = whoMovesFirst(ENEMYP.weight, PLAYERP.weight);
                MENU.playerMove = MENU.choice;
                if (MENU.movedFirst) setBattleState(g, BS_PLAYER_TURN);
                else setBattleState(g, BS_ENEMY_TURN);
            }
            break;
        }
    }
}