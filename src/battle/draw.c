#include "../common.h"

// _____________________________________________________________________________
//
//  Battle menu - draw status effects function
// _____________________________________________________________________________
//
void drawStatusEffects(Phone phone, BattlePhone bPhone, int x) {
    int battleStatusY = 66;

    if (bPhone.confusedTurns) {
        drawTextureRec(
            "status", (Rectangle) {0, BE_CONF, 31, 13},
            (Vector2) {x, battleStatusY}, WHITE
        );
        battleStatusY += 15;
    }
    if (bPhone.atkUpTurns && bPhone.atkUpAmount > 0) {
        drawTextureRec(
            "status", (Rectangle) {0, BE_ATKUP, 31, 13},
            (Vector2) {x, battleStatusY}, WHITE
        );
        battleStatusY += 15;
    }
    if (bPhone.atkUpTurns && bPhone.atkUpAmount < 0) {
        drawTextureRec(
            "status", (Rectangle) {0, BE_ATKDOWN, 31, 13},
            (Vector2) {x, battleStatusY}, WHITE
        );
        battleStatusY += 15;
    }
    if (bPhone.defUpTurns && bPhone.defUpAmount > 0) {
        drawTextureRec(
            "status", (Rectangle) {0, BE_DEFUP, 31, 13},
            (Vector2) {x, battleStatusY}, WHITE
        );
        battleStatusY += 15;
    }
    if (bPhone.defUpTurns && bPhone.defUpAmount < 0) {
        drawTextureRec(
            "status", (Rectangle) {0, BE_DEFDOWN, 31, 13},
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
void drawBattleMenu() {
    drawTexture(g.mapMeta.battleBackground, 0, 0, WHITE);

    if (MENU.battleState == BS_WAITING || MENU.battleState == BS_WAITING_MOVE) {
        // Left panel (options)
        drawBox(0, 176, 120, 64);
        for (int i = 0; i < arrlen(MENU.choices); i++) {
            drawText(MENU.choices[i], 20, 180 + 14*i, WHITE);
        }
        drawTexture("indicator", 6, 180 + 14*MENU.choice, WHITE);

        // Right panel (description, just a text box)
        drawBox(120, 176, 200, 64);

        if (MENU.battleState == BS_WAITING) {
            drawText("Command?", 128, 184, WHITE);
        }
        else {
            #define SKILL (SSPECS(PLAYERP.skills[MENU.choice]))
	        drawTextRec(SKILL.description, 128, 182, 184, 64, WHITE);
            drawText(F("Type: %s", skillTypes[SKILL.type]), 128, 222, LIGHTGRAY);
        }
    }
    else {
        // Battle menu contains a one line text box (without a typewriter effect)
        drawBox(0, 176, 320, 64);

        // Typewriter effect - based on the drawTextbox code but with three lines
        int line1Len = MIN(MENU.battleTextboxTimer, 63);
        int line2Len = MIN(
            MAX(MENU.battleTextboxTimer - strlen(MENU.battleTextbox[0]) - 20, 0), 63);
        int line3Len = MIN(
            MAX(MENU.battleTextboxTimer - strlen(MENU.battleTextbox[0]) - strlen(MENU.battleTextbox[1]) - 20, 0), 63
        );

        char textboxDraw[3][64] = {0};
        strncpy(textboxDraw[0], MENU.battleTextbox[0], line1Len);
        strncpy(textboxDraw[1], MENU.battleTextbox[1], line2Len);
        strncpy(textboxDraw[2], MENU.battleTextbox[2], line3Len);
        drawText(textboxDraw[0], 8, 184, WHITE);
        drawText(textboxDraw[1], 8, 202, WHITE);
        drawText(textboxDraw[2], 8, 220, WHITE);
        MENU.battleTextboxTimer++;
    }

    // Player status bar
    drawBox(4, 4, 152, 58);
    drawText(g.s.name, 10, 10, LIGHTGRAY);

    // Note: '$' character in the digits font (drawTextD) says 'Lv.'
    drawTextD(F("$ %d", PLAYERP.level), 120, 10, WHITE);

    const char *brandModel = F(
        "%s %s", SPECS(PLAYERP.id).brand, SPECS(PLAYERP.id).model
    );
    drawText(brandModel, 10, 25, WHITE);
    drawProgressBar(MENU.player.shownHP, PLAYERP.maxHP, 10, 42, 80, GREEN);
    drawTextD(F("%d/%d", MENU.player.shownHP, PLAYERP.maxHP), 98, 42, WHITE);

    // Player statuses
    drawStatusEffects(PLAYERP, MENU.player, 2);

    // Enemy status bar
    drawBox(164, 4, 152, 58);
    drawText(MENU.enemyName, 170, 10, LIGHTGRAY);
    drawTextD(F("$ %d", ENEMYP.level), 280, 10, WHITE);

    brandModel = F("%s %s", SPECS(ENEMYP.id).brand, SPECS(ENEMYP.id).model);
    drawText(brandModel, 170, 25, WHITE);
    drawProgressBar(MENU.enemy.shownHP, ENEMYP.maxHP, 170, 42, 80, GREEN);
    drawTextD(F("%d/%d", MENU.enemy.shownHP, ENEMYP.maxHP), 258, 42, WHITE);
    drawTextD(F("$ %d", PLAYERP.level), 120, 10, WHITE);

    // Enemy statuses
    drawStatusEffects(ENEMYP, MENU.enemy, 287);

    // Phone shadows (except for cave background)
    if (strcmp(g.mapMeta.battleBackground, "battle/cave")) {
        drawTexture("shadow", 48, 150, WHITE);
        drawTexture("shadow", 208, 150, WHITE);
    }

    // Phone sprites
    drawTexture(SPECS(PLAYERP.id).sprite, 48, 96, WHITE);
    drawTexture(SPECS(ENEMYP.id).sprite, 208, 96, WHITE);

    // Attack animation (each anim frame lasts 4 frames, frames are 64×64,
    // animation timer is set by doMove function)
    int animLength = shget(g.textures, MENU.attackAnim).width / 64 * 4;

    if (MENU.attackAnimTimer >= 0 && MENU.attackAnimTimer < animLength && strlen(MENU.attackAnim)) {
        drawTextureRec(
            MENU.attackAnim, (Rectangle) {(MENU.attackAnimTimer / 4)*64, 0, 64, 64},
            (Vector2) {48 + 160*MENU.attackAnimTarget, 96}, WHITE
        );
    }
    MENU.attackAnimTimer++;
}