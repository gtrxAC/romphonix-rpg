#include "../common.h"

// _____________________________________________________________________________
//
//  Battle menu - draw status effects function
// _____________________________________________________________________________
//
void drawStatusEffects(Game *g, Phone phone, BattlePhone bPhone, int x) {
    int battleStatusY = 66;

    if (bPhone.confusedTurns) {
        DrawTextureRec(
            TEX(status), (Rectangle) {0, BE_CONF, 31, 13},
            (Vector2) {x, battleStatusY}, WHITE
        );
        battleStatusY += 15;
    }
    if (bPhone.atkUpTurns && bPhone.atkUpAmount > 0) {
        DrawTextureRec(
            TEX(status), (Rectangle) {0, BE_ATKUP, 31, 13},
            (Vector2) {x, battleStatusY}, WHITE
        );
        battleStatusY += 15;
    }
    if (bPhone.atkUpTurns && bPhone.atkUpAmount < 0) {
        DrawTextureRec(
            TEX(status), (Rectangle) {0, BE_ATKDOWN, 31, 13},
            (Vector2) {x, battleStatusY}, WHITE
        );
        battleStatusY += 15;
    }
    if (bPhone.defUpTurns && bPhone.defUpAmount > 0) {
        DrawTextureRec(
            TEX(status), (Rectangle) {0, BE_DEFUP, 31, 13},
            (Vector2) {x, battleStatusY}, WHITE
        );
        battleStatusY += 15;
    }
    if (bPhone.defUpTurns && bPhone.defUpAmount < 0) {
        DrawTextureRec(
            TEX(status), (Rectangle) {0, BE_DEFDOWN, 31, 13},
            (Vector2) {x, battleStatusY}, WHITE
        );
        battleStatusY += 15;
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

    // Player statuses
    drawStatusEffects(g, PLAYERP, MENU.player, 2);

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

    // Enemy statuses
    drawStatusEffects(g, ENEMYP, MENU.enemy, 287);

    // Player phone sprite
    DrawTexture(shget(g->textures, SPECS(PLAYERP.id).sprite), 48, 96, WHITE);

    // Enemy phone sprite
    DrawTexture(shget(g->textures, SPECS(ENEMYP.id).sprite), 208, 96, WHITE);

    // debug
    DrawText(TextFormat("%d", MENU.battleState), 0, 0, 10, YELLOW);
}