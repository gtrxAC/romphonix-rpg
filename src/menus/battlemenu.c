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

void scrBattleMenu(Game *g, bool canRun);
void updateBattleMenu(Game *g);
void drawBattleMenu(Game *g);
void checkBattleMenu(Game *g);

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
//  Battle menu - do move (helper function)
// _____________________________________________________________________________
//
void doMove(Game *g, Phone *attacker, Phone *victim, SkillSpecs skill) {
    for (int i = 0; i < 2; i++) {
        switch (skill.effects[i].effect) {
            case SE_NONE: break;

            case SE_DAMAGE: {
                int damage = (float) skill.effects[i].parameter * attacker->attack / victim->defense;
                victim->hp -= damage;
                strcat(
                    MENU.battleTextbox[1],
                    TextFormat(
                        " %s %s took %d damage!", SPECS(victim->id).brand,
                        SPECS(victim->id).model, damage
                    )
                );
            }

            case SE_CONFUSE: {
                if (GetRandomValue(0, 100) < skill.effects[i].parameter) {
                    victim->confused = true;
                    // strcat(MENU.battleTextbox, TextFormat(" !", damage));
                }
            }
        }
    }
}

// _____________________________________________________________________________
//
//  Battle menu - state update function
// _____________________________________________________________________________
//
void updateBattleMenu(Game *g) {
    if (MENU.battleState == BS_WAITING || MENU.battleState == BS_WAITING_MOVE) {
        // Command menu is just a standard menu
        updateMenu(g);
        if (K_B_PRESS() && MENU.battleState == BS_WAITING_MOVE) {
            MENU.battleState = BS_WAITING;
            MENU.choice = 0;
            arrfree(MENU.choices);
            arrpush(MENU.choices, "Fight");
            arrpush(MENU.choices, "Switch");
            arrpush(MENU.choices, "Items");
            if (MENU.canRun) arrpush(MENU.choices, "Run");
        }   
    }
    // For any other state, the menu only updates when the confirm button is
    // pressed.
    else if (K_A_PRESS() || K_B_PRESS()) {
        switch (MENU.battleState) {
            case BS_STARTING: MENU.battleState = BS_WAITING; break;
        }
    }
}

// _____________________________________________________________________________
//
//  Battle menu - draw function
// _____________________________________________________________________________
//
void drawBattleMenu(Game *g) {
    Phone *playerPh = &g->s.party[MENU.active];
    Phone *enemyPh = &MENU.enemyParty[MENU.enemyActive];

    DrawTexture(TEX(battle_bg), 0, 0, WHITE);

    if (MENU.battleState == BS_WAITING || MENU.battleState == BS_WAITING_MOVE) {
        // Left panel (options)
        drawBox(g, 0, 176, 120, 64);
        for (int i = 0; i < arrlen(MENU.choices); i++) {
            drawText(g, MENU.choices[i], 20, 180 + 14*i, WHITE);
        }
        DrawTexture(TEX(indicator), 6, 180 + 14*MENU.choice, WHITE);

        // Right panel (description, just a text box)
        drawBox(g, 120, 176, 200, 64);

        if (MENU.battleState == BS_WAITING) {
            drawText(g, "Command?", 128, 184, WHITE);
        }
        else {
	        drawTextRec(
                g, SSPECS(playerPh->skills[MENU.choice]).description,
                128, 182, 184, 64, WHITE
            );
            drawText(g, TextFormat("Type: %s"))
        }
    }
    else {
        // Battle menu contains a one line text box (without a typewriter effect)
        drawBox(g, 10, 176, 300, 64);
        drawText(g, MENU.battleTextbox[0], 18, 184, WHITE);
        drawText(g, MENU.battleTextbox[1], 18, 202, WHITE);
        drawText(g, MENU.battleTextbox[2], 18, 220, WHITE);
    }

    // Player status bar
    drawBox(g, 4, 4, 152, 58);
    drawText(g, g->s.name, 10, 10, LIGHTGRAY);

    // Note: '$' character in the digits font (drawTextD) says 'Lv.'
    drawTextD(g, TextFormat("$ %d", playerPh->level), 120, 10, WHITE);

    const char *brandModel = TextFormat(
        "%s %s", SPECS(playerPh->id).brand, SPECS(playerPh->id).model
    );
    drawText(g, brandModel, 10, 25, WHITE);
    drawProgressBar(g, playerPh->hp, playerPh->maxHP, 10, 42, 80, GREEN);
    drawTextD(g, TextFormat("%d/%d", playerPh->hp, playerPh->maxHP), 98, 42, WHITE);

    // Enemy status bar
    drawBox(g, 164, 4, 152, 58);
    drawText(g, MENU.enemyName, 170, 10, LIGHTGRAY);
    drawTextD(g, TextFormat("$ %d", enemyPh->level), 280, 10, WHITE);

    brandModel = TextFormat(
        "%s %s", SPECS(enemyPh->id).brand, SPECS(enemyPh->id).model
    );
    drawText(g, brandModel, 170, 25, WHITE);
    drawProgressBar(g, enemyPh->hp, enemyPh->maxHP, 170, 42, 80, GREEN);
    drawTextD(g, TextFormat("%d/%d", enemyPh->hp, enemyPh->maxHP), 258, 42, WHITE);
    drawTextD(g, TextFormat("$ %d", playerPh->level), 120, 10, WHITE);

    // Player phone sprite
    DrawTexture(shget(g->textures, SPECS(playerPh->id).sprite), 48, 96, WHITE);

    // Enemy phone sprite
    DrawTexture(shget(g->textures, SPECS(enemyPh->id).sprite), 240, 96, WHITE);

    // debug
    DrawText(TextFormat("%d", MENU.battleState), 0, 0, 10, YELLOW);
}

// _____________________________________________________________________________
//
//  Battle menu - check user input function
// _____________________________________________________________________________
//
void checkBattleMenu(Game *g) {
    Phone *playerPh = &g->s.party[MENU.active];
    Phone *enemyPh = &MENU.enemyParty[MENU.enemyActive];

    switch (MENU.battleState) {
        case BS_WAITING: {
            switch (MENU.choice) {
                case -1: break;

                case 0: { // Fight
                    MENU.battleState = BS_WAITING_MOVE;
                    MENU.choice = 0;
                    arrfree(MENU.choices);
                    for (int i = 0; i < 4; i++) {
                        arrpush(MENU.choices, SSPECS(playerPh->skills[0]).name);
                    }
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
                MENU.battleState = BS_WAITING; // go back to the start
            }
            else {
                // Choose which side moves first, based on the weights of the phone and a bit of random chance

            }
            break;
        }
    }
}