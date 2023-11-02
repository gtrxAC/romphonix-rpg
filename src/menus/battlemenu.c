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
//  Note: returns true if the battle can be created, false if not (player
//  doesn't have any phones)
//  Do not edit battle variables if the function returns false - use an if
//  statement instead: if (scrBattleMenu(...)) { set battle variables... }
// _____________________________________________________________________________
//
bool scrBattleMenu(bool canRun) {
    // Choose the player's active phone (first phone that has remaining HP)
    bool found = false;
    int active;
    for (int i = 0; i < 6; i++) {
        if (g.s.party[i].active && g.s.party[i].hp > 0) {
            active = i;
            found = true;
            break;
        }
    }

    if (!found) {
        pushTextbox("You don't have any phones!", "");
        return false;
    }

    pushMenu(CB_NOTHING);
    strcpy(MENU.battleTextbox[0], "Battle doesn't have a textbox set!");
    strcpy(MENU.battleTextbox[1], "Battle doesn't have a textbox set!");
    strcpy(MENU.battleTextbox[2], "Battle doesn't have a textbox set!");

    MENU.active = -1;
    MENU.nextActive = active;  // BS_SENDING_OUT animation will set nextActive to active

    setUpdateFunc(updateBattleMenu);
    setDrawFunc(drawBattleMenu);
    setNextFunc(checkBattleMenu);

    MENU.battleState = BS_STARTING;
    addChoice("Fight");
    addChoice("Switch");
    addChoice("Items");

    MENU.canRun = canRun;
    if (canRun) addChoice("Run");
    g.state = ST_BATTLE;
    return true;
}

// _____________________________________________________________________________
//
//  Battle menu - update status effect timers (helper function)
// _____________________________________________________________________________
//
void updateStatusTimers(int *textboxSlot, Phone *phone, BattlePhone *bPhone) {
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
void setBattleState(BattleState bs) {
    MENU.battleTextboxTimer = 0;
    MENU.battleState = bs;

    switch (bs) {
        case BS_WAITING: {
            MENU.choice = 0;
            arrfree(MENU.choices);
            addChoice("Fight");
            addChoice("Switch");
            addChoice("Items");
            if (MENU.canRun) addChoice("Run");
            break;
        }

        case BS_ENEMY_SENDING_OUT: {
            sprintf(
                MENU.battleTextbox[0], "%s sent out %s %s!",
                MENU.enemyName, SPECS(ENEMYP.id).brand, SPECS(ENEMYP.id).model
            );
            strcpy(MENU.battleTextbox[1], "");
            strcpy(MENU.battleTextbox[2], "");
            g.frameCount = 0;  // Start animation
            break;
        }

        case BS_SENDING_OUT: {
            MENU.active = MENU.nextActive;
            MENU.player = (BattlePhone) {0}; // reset any stat changes
            MENU.player.shownHP = PLAYERP.hp;

            sprintf(
                MENU.battleTextbox[0], "Go, %s %s!",
                SPECS(PLAYERP.id).brand, SPECS(PLAYERP.id).model
            );
            strcpy(MENU.battleTextbox[1], "");
            strcpy(MENU.battleTextbox[2], "");
            g.frameCount = 0;  // Start animation
            break;
        }

        case BS_RETURNING: {
            sprintf(
                MENU.battleTextbox[0], "Come back, %s %s!",
                SPECS(PLAYERP.id).brand, SPECS(PLAYERP.id).model
            );
            strcpy(MENU.battleTextbox[1], "");
            strcpy(MENU.battleTextbox[2], "");
            g.frameCount = 0;  // Start animation
            break;
        }

        case BS_WAITING_SKILL: {
            MENU.choice = 0;
            arrfree(MENU.choices);
            for (int i = 0; i < 4; i++) {
                addChoice(SSPECS(PLAYERP.skills[i]).name);
            }
            break;
        }

        case BS_WAITING_NO_ENERGY: {
            strcpy(MENU.battleTextbox[0], "Not enough energy to use this skill!");
            strcpy(MENU.battleTextbox[0], "");
            strcpy(MENU.battleTextbox[0], "");
            break;
        }

        case BS_ENEMY_TURN: {
            // for now, enemy always does random skill
            SkillSpecs skill = SSPECS(ENEMYP.skills[GetRandomValue(0, 3)]);
            sprintf(
                MENU.battleTextbox[0], "Enemy %s %s uses %s...",
                SPECS(ENEMYP.id).brand, SPECS(ENEMYP.id).model, skill.name
            );
            MENU.battleTextbox[1][0] = '\0';
            MENU.battleTextbox[2][0] = '\0';
            doMove(&ENEMYP, &PLAYERP, &MENU.enemy, &MENU.player, skill);
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
            doMove(&PLAYERP, &ENEMYP, &MENU.player, &MENU.enemy, skill);
            break;
        }

        case BS_AFTER_TURN: {
            for (int i = 0; i < 3; i++) MENU.battleTextbox[i][0] = '\0';
            int textboxSlot = 0;

            updateStatusTimers(&textboxSlot, &PLAYERP, &MENU.player);
            updateStatusTimers(&textboxSlot, &ENEMYP, &MENU.enemy);

            // if no status effects were processed (nothing to show in textbox),
            // just skip to the next turn
            if (!textboxSlot) setBattleState(BS_WAITING);
            break;
        }

        case BS_PLAYER_DIED: {
            g.frameCount = 0; // start death animation
            sprintf(
                MENU.battleTextbox[0], "Your %s %s went down!",
                SPECS(PLAYERP.id).brand, SPECS(PLAYERP.id).model
            );
            strcpy(MENU.battleTextbox[1], "");
            strcpy(MENU.battleTextbox[2], "");
            break;
        }

        case BS_ENEMY_DIED: {
            g.frameCount = 0; // start death animation
            sprintf(
                MENU.battleTextbox[0], "Enemy %s %s went down!",
                SPECS(ENEMYP.id).brand, SPECS(ENEMYP.id).model
            );
            strcpy(MENU.battleTextbox[1], "idk something about XP");
            strcpy(MENU.battleTextbox[2], "");
            break;
        }

        case BS_WON: {
            // Choose the next phone in line to send out
            bool havePhones = false;
            for (int i = 0; i < 6; i++) {
                if (MENU.enemyParty[i].active && MENU.enemyParty[i].hp > 0) {
                    MENU.enemyActive = i;
                    setBattleState(BS_ENEMY_SENDING_OUT);
                    havePhones = true;
                    break;
                }
            }

            if (!havePhones) {
                setSong("assets/sounds/music/thtune.mid");
                int rewardMsgLine = 0;
                for (int i = 0; i < 2; i++) {
                    if (MENU.enemyDefeatMsg[i] && strlen(MENU.enemyDefeatMsg[i])) {
                        strcpy(MENU.battleTextbox[i], MENU.enemyDefeatMsg[i]);
                        rewardMsgLine++;
                    }
                    else {
                        strcpy(MENU.battleTextbox[i], "");
                    }
                }
                if (MENU.enemyReward > 0) {
                    sprintf(
                        MENU.battleTextbox[rewardMsgLine], "%s gave you $%d as a reward!",
                        MENU.enemyName, MENU.enemyReward
                    );
                    g.s.money += MENU.enemyReward;
                }
            }
            break;
        }

        case BS_LOST: {
            bool havePhones = false;
            for (int i = 0; i < 6; i++) {
                if (g.s.party[i].active && g.s.party[i].hp) {
                    havePhones = true;
                    break;
                }
            }
            if (havePhones) {
                MENU.nextBattleState = BS_WAITING;
                scrSwitchPhoneMenu();
            }
            else {
                strcpy(MENU.battleTextbox[0], "LOL you lose the battle and ran out");
                strcpy(MENU.battleTextbox[1], "of phones...");
                strcpy(MENU.battleTextbox[2], "");
            }
            break;
        }
    }
}

// _____________________________________________________________________________
//
//  Battle menu - check user input function
// _____________________________________________________________________________
//
void checkBattleMenu() {
    switch (MENU.battleState) {
        case BS_WAITING: {
            switch (MENU.choice) {
                case -1: break;

                case 0: { // Fight
                    setBattleState(BS_WAITING_SKILL);
                    break;
                }

                case 1: { // Switch
                    MENU.nextBattleState = BS_ENEMY_TURN;
                    scrSwitchPhoneMenu();
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

        case BS_WAITING_SKILL: {
            if (MENU.choice == -1) {
                setBattleState(BS_WAITING); // go back to the start
            }
            else {
                if (SSPECS(PLAYERP.skills[MENU.choice]).energy > PLAYERP.energy) {
                    setBattleState(BS_WAITING_NO_ENERGY);
                    break;
                }
                // Choose which side moves first, based on the weights of the
                // phones and a bit of random chance
                MENU.movedFirst = whoMovesFirst(&ENEMYP, &PLAYERP);
                MENU.playerMove = MENU.choice;
                if (MENU.movedFirst) setBattleState(BS_PLAYER_TURN);
                else setBattleState(BS_ENEMY_TURN);
            }
            break;
        }
    }
}