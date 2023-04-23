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

// PLAYERP and ENEMYP contain phone properties that will (in the player's case)
// be permanently saved.
// MENU.player and MENU.enemy contain temporary properties (see phones.h
// BattlePhone struct)
#define PLAYERP (g->s.party[MENU.active])
#define ENEMYP (MENU.enemyParty[MENU.enemyActive])

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
void doMove(Game *g, Phone *attacker, Phone *victim, BattlePhone *attackerB, BattlePhone *victimB, SkillSpecs skill) {
    for (int i = 0; i < 2; i++) {
        switch (skill.effects[i].effect) {
            case SE_NONE: {
                if (i == 0) strcpy(MENU.battleTextbox[1], "No effect!");
                break;
            }

            case SE_DRAIN: {
                int damage = (float) skill.effects[i].parameter * attacker->attack / victim->defense;
                victim->hp -= damage;
                attacker->hp = MIN(attacker->hp + damage, 100);
                sprintf(
                    MENU.battleTextbox[i + 1],
                    "Absorbed %d HP from %s %s!",
                    damage, SPECS(victim->id).brand, SPECS(victim->id).model
                );
                break;
            }

            case SE_DAMAGE: {
                int damage = (float) skill.effects[i].parameter * attacker->attack / victim->defense;
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
                    victimB->confused = true;
                    sprintf(
                        MENU.battleTextbox[i + 1],
                        "%s %s is now confused!",
                        SPECS(victim->id).brand, SPECS(victim->id).model
                    );
                }
                break;
            }

            case SE_ATK_UP: {
                attackerB->atkUpTurns = 6;
                sprintf(
                    MENU.battleTextbox[i + 1],
                    "%s %s has boosted their attack!",
                    SPECS(attacker->id).brand, SPECS(attacker->id).model
                );
                break;
            }

            case SE_DEF_UP: {
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
}

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

            if (MENU.player.atkUpTurns) {
                MENU.player.atkUpTurns--;
                if (!MENU.player.atkUpTurns) {
                    sprintf(
                        MENU.battleTextbox[0],
                        "%s %s's attack boost wore off...",
                        SPECS(PLAYERP.id).brand, SPECS(PLAYERP.id).model
                    );
                    textboxSlot++;
                }
            }
            if (MENU.enemy.atkUpTurns) {
                MENU.enemy.atkUpTurns--;
                if (!MENU.enemy.atkUpTurns) {
                    sprintf(
                        MENU.battleTextbox[textboxSlot],
                        "%s %s's attack boost wore off...",
                        SPECS(ENEMYP.id).brand, SPECS(ENEMYP.id).model
                    );
                    textboxSlot++;
                }
            }
            if (MENU.player.defUpTurns) {
                MENU.player.defUpTurns--;
                if (!MENU.player.defUpTurns) {
                    sprintf(
                        MENU.battleTextbox[textboxSlot],
                        "%s %s's defense boost wore off...",
                        SPECS(PLAYERP.id).brand, SPECS(PLAYERP.id).model
                    );
                    textboxSlot++;
                }
            }
            if (MENU.enemy.defUpTurns) {
                MENU.enemy.defUpTurns--;
                if (!MENU.enemy.defUpTurns && textboxSlot < 3) {
                    sprintf(
                        MENU.battleTextbox[textboxSlot],
                        "%s %s's defense boost wore off...",
                        SPECS(ENEMYP.id).brand, SPECS(ENEMYP.id).model
                    );
                    textboxSlot++;
                }
            }

            // if no status effects were processed (nothing to show in textbox),
            // just skip to the next turn
            if (!textboxSlot) setBattleState(g, BS_WAITING);
            break;
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
            setBattleState(g, BS_WAITING);
        }   
    }
    // For any other state, the menu only updates when the confirm button is
    // pressed.
    else if (K_A_PRESS() || K_B_PRESS()) {
        switch (MENU.battleState) {
            case BS_STARTING: setBattleState(g, BS_WAITING); break;

            case BS_PLAYER_TURN: {
                if (MENU.movedFirst) setBattleState(g, BS_ENEMY_TURN);
                else setBattleState(g, BS_AFTER_TURN);
                break;
            }
            
            case BS_ENEMY_TURN: {
                if (!MENU.movedFirst) setBattleState(g, BS_PLAYER_TURN);
                else setBattleState(g, BS_AFTER_TURN);
                break;
            }

            case BS_AFTER_TURN: {
                setBattleState(g, BS_WAITING);
                break;
            }

            case BS_RUN: popMenu(g); break;
        }
    }
}

// _____________________________________________________________________________
//
//  Battle menu - draw function
// _____________________________________________________________________________
//
void drawBattleMenu(Game *g) {
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
            #define SKILL (SSPECS(PLAYERP.skills[MENU.choice]))
	        drawTextRec(g, SKILL.description, 128, 182, 184, 64, WHITE);
            drawText(g, TextFormat("Type: %s", skillTypes[SKILL.type]), 128, 222, LIGHTGRAY);
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
    drawTextD(g, TextFormat("$ %d", PLAYERP.level), 120, 10, WHITE);

    const char *brandModel = TextFormat(
        "%s %s", SPECS(PLAYERP.id).brand, SPECS(PLAYERP.id).model
    );
    drawText(g, brandModel, 10, 25, WHITE);
    drawProgressBar(g, PLAYERP.hp, PLAYERP.maxHP, 10, 42, 80, GREEN);
    drawTextD(g, TextFormat("%d/%d", PLAYERP.hp, PLAYERP.maxHP), 98, 42, WHITE);

    // Player confused status (text with black outline)
    if (MENU.player.confused) {
        for (int x = -1; x < 2; x++) {
            for (int y = -1; y < 2; y++) {
                drawText(g, "Confused", 5 + x, 65 + y, BLACK);
            }
        }
        drawText(g, "Confused", 5, 65, YELLOW);
    }

    // Enemy status bar
    drawBox(g, 164, 4, 152, 58);
    drawText(g, MENU.enemyName, 170, 10, LIGHTGRAY);
    drawTextD(g, TextFormat("$ %d", ENEMYP.level), 280, 10, WHITE);

    brandModel = TextFormat(
        "%s %s", SPECS(ENEMYP.id).brand, SPECS(ENEMYP.id).model
    );
    drawText(g, brandModel, 170, 25, WHITE);
    drawProgressBar(g, ENEMYP.hp, ENEMYP.maxHP, 170, 42, 80, GREEN);
    drawTextD(g, TextFormat("%d/%d", ENEMYP.hp, ENEMYP.maxHP), 258, 42, WHITE);
    drawTextD(g, TextFormat("$ %d", PLAYERP.level), 120, 10, WHITE);

    // Enemy confused status (text with black outline)
    if (MENU.enemy.confused) {
        for (int x = -1; x < 2; x++) {
            for (int y = -1; y < 2; y++) {
                drawText(g, "Confused", 259 + x, 65 + y, BLACK);
            }
        }
        drawText(g, "Confused", 259, 65, YELLOW);
    }

    // Player phone sprite
    DrawTexture(shget(g->textures, SPECS(PLAYERP.id).sprite), 48, 96, WHITE);

    // Enemy phone sprite
    DrawTexture(shget(g->textures, SPECS(ENEMYP.id).sprite), 208, 96, WHITE);

    // debug
    DrawText(TextFormat("%d", MENU.battleState), 0, 0, 10, YELLOW);
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
                // Choose which side moves first, based on the weights of the phone and a bit of random chance
                MENU.movedFirst = whoMovesFirst(ENEMYP.weight, PLAYERP.weight);
                MENU.playerMove = MENU.choice;
                if (MENU.movedFirst) setBattleState(g, BS_PLAYER_TURN);
                else setBattleState(g, BS_ENEMY_TURN);
            }
            break;
        }
    }
}