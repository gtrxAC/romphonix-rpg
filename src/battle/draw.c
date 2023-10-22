#include "../common.h"

// _____________________________________________________________________________
//
//  Battle menu - draw status effects function
// _____________________________________________________________________________
//
void drawStatusEffects(Phone phone, BattlePhone bPhone, int x) {
    int battleStatusY = 66;

    #define DRAW_STATUS(y) { \
        drawTextureRec( \
            "status", (Rectangle) {0, y, 31, 13}, \
            (Vector2) {x, battleStatusY}, WHITE \
        ); \
        battleStatusY += 15; \
    }

    if (bPhone.confusedTurns) DRAW_STATUS(BE_CONF);
    if (bPhone.atkUpTurns && bPhone.atkUpAmount > 0) DRAW_STATUS(BE_ATKUP);
    if (bPhone.atkUpTurns && bPhone.atkUpAmount < 0) DRAW_STATUS(BE_ATKDOWN);
    if (bPhone.defUpTurns && bPhone.defUpAmount > 0) DRAW_STATUS(BE_DEFUP);
    if (bPhone.defUpTurns && bPhone.defUpAmount < 0) DRAW_STATUS(BE_DEFDOWN);
    if (bPhone.accuracyUpTurns && bPhone.accuracyUpAmount > 0) DRAW_STATUS(BE_ACCUP);
    if (bPhone.accuracyUpTurns && bPhone.accuracyUpAmount < 0) DRAW_STATUS(BE_ACCDOWN);
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
        // Battle menu contains a three line text box
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

        // Show an arrow icon at the bottom right when the typewriter anim has finished
        int textboxLen = strlen(MENU.battleTextbox[0]) + strlen(MENU.battleTextbox[1]) + strlen(MENU.battleTextbox[2]) + 20;
        if (MENU.battleTextboxTimer >= textboxLen) drawText("\x7f", 304, 224, WHITE);

        MENU.battleTextboxTimer++;
    }

    // Player status bar (don't show before phone has been sent out)
    if (MENU.battleState != BS_STARTING && MENU.battleState != BS_SENDING_OUT && MENU.battleState != BS_LOST && MENU.battleState != BS_PLAYER_DIED && MENU.active != -1) {
        drawBox(4, 4, 152, 58);
        drawText(g.s.name, 10, 10, LIGHTGRAY);
        drawTextD(F("$ %d", PLAYERP.level), 120, 10, WHITE);

        // Note: '$' character in the digits font (drawTextD) says 'Lv.'
        drawTextD(F("$ %d", PLAYERP.level), 120, 10, WHITE);

        drawText(F("%s %s", SPECS(PLAYERP.id).brand, SPECS(PLAYERP.id).model), 10, 25, WHITE);
        drawProgressBar(MENU.player.shownHP, PLAYERP.maxHP, 10, 42, 80, GREEN);
        drawTextD(F("%d/%d", MENU.player.shownHP, PLAYERP.maxHP), 98, 42, WHITE);

        // Player statuses
        drawStatusEffects(PLAYERP, MENU.player, 2);
    }

    // Enemy status bar
    if ((MENU.battleState != BS_STARTING || MENU.canRun) && MENU.battleState != BS_ENEMY_SENDING_OUT && MENU.battleState != BS_ENEMY_DIED && MENU.battleState != BS_WON) {
        drawBox(164, 4, 152, 58);
        drawText(MENU.enemyName, 170, 10, LIGHTGRAY);
        drawTextD(F("$ %d", ENEMYP.level), 280, 10, WHITE);

        drawText(F("%s %s", SPECS(ENEMYP.id).brand, SPECS(ENEMYP.id).model), 170, 25, WHITE);
        drawProgressBar(MENU.enemy.shownHP, ENEMYP.maxHP, 170, 42, 80, GREEN);
        drawTextD(F("%d/%d", MENU.enemy.shownHP, ENEMYP.maxHP), 258, 42, WHITE);

        // Enemy statuses
        drawStatusEffects(ENEMYP, MENU.enemy, 287);
    }

    // Phone shadows (except for cave background)
    if (strcmp(g.mapMeta.battleBackground, "battle/cave")) {
        // Player phone's shadow will expand when sending out, and retract when returning a phone or the phone dies
        switch (MENU.battleState) {
            case BS_STARTING: break;  // don't draw shadow, phone not sent out yet
            case BS_LOST: break;  // Don't draw shadow, phone is dead

            case BS_SENDING_OUT: {
                float animTime = MIN((float) g.frameCount / 60.0f, 1.0f);
                drawTexturePro( 
                    "shadow", (Rectangle) {0, 0, 64, 20},
                    (Rectangle) {80.0f - 32.0f*animTime, 160.0f - 10.0f*animTime, 64.0f*animTime, 20.0f*animTime},
                    0.0f, WHITE
                );
                break;
            }
            
            case BS_RETURNING: case BS_PLAYER_DIED: {
                float animTime = MAX(1.0f - (float) g.frameCount / 60.0f, 0.0f);
                drawTexturePro(
                    "shadow", (Rectangle) {0, 0, 64, 20},
                    (Rectangle) {80.0f - 32.0f*animTime, 160.0f - 10.0f*animTime, 64.0f*animTime, 20.0f*animTime},
                    0.0f, WHITE
                );
                break;
            }

            case BS_ENEMY_SENDING_OUT: {
                // If enemy is sending out their first phone, don't draw the player phone shadow yet.
                // Otherwise draw it like normal (using fallthrough).
                if (MENU.active == -1) break;
            }

            default:
                drawTexture("shadow", 48, 150, WHITE);
                break;
        }

        // Enemy shadow will expand when sending out, retract when phone dies
        switch (MENU.battleState) {
            case BS_WON: break;  // don't draw

            case BS_ENEMY_SENDING_OUT: {
                float animTime = MIN((float) g.frameCount / 60.0f, 1.0f);
                drawTexturePro( 
                    "shadow", (Rectangle) {0, 0, 64, 20},
                    (Rectangle) {240.0f - 32.0f*animTime, 160.0f - 10.0f*animTime, 64.0f*animTime, 20.0f*animTime},
                    0.0f, WHITE
                );
                break;
            }

            case BS_ENEMY_DIED: {
                float animTime = MAX(1.0f - (float) g.frameCount / 60.0f, 0.0f);
                drawTexturePro(
                    "shadow", (Rectangle) {0, 0, 64, 20},
                    (Rectangle) {240.0f - 32.0f*animTime, 160.0f - 10.0f*animTime, 64.0f*animTime, 20.0f*animTime},
                    0.0f, WHITE
                );
                break;
            }

            case BS_STARTING: {
                // In wild battles, draw like normal (using fallthrough).
                // In NPC battles, don't draw as the phone isn't sent out yet.
                if (!MENU.canRun) break;
            }

            default: 
                drawTexture("shadow", 208, 150, WHITE);
                break;
        }
    }

    // Player phone sprite
    switch (MENU.battleState) {
        case BS_STARTING: break; // Don't draw player phone, it hasn't been sent out yet
        case BS_LOST: break; // Don't draw, it's already dead

        case BS_PLAYER_DIED: {
            float animTime = MIN((float) g.frameCount / 30.0f, 1.0f);
            drawTexturePro(
                SPECS(PLAYERP.id).sprite, (Rectangle) {0, 0, 64, 64},
                (Rectangle) {48.0f - 16.0f*animTime, 96.0f + 64.0f*animTime, 64.0f + 32.0f*animTime, 64.0f - 64.0f*animTime},
                0.0f, WHITE
            );
            break;
        }

        case BS_SENDING_OUT: {
            // First 15 frames: phone not shown
            // Middle 30 frames: animation
            // Last 15 frames: phone shown like normal
            if (g.frameCount > 15 && g.frameCount < 45) {
                float animTime = MIN((float) (g.frameCount - 15) / 30.0f, 1.0f);
                drawTexturePro(
                    SPECS(PLAYERP.id).sprite, (Rectangle) {0, 0, 64, 64},
                    (Rectangle) {48.0f*animTime, 64.0f + 32.0f*animTime, 8.0f + 56.0f*animTime, 8.0f + 56.0f*animTime},
                    0.0f, WHITE
                );
            }
            else if (g.frameCount >= 45) {
                drawTexture(SPECS(PLAYERP.id).sprite, 48, 96, WHITE);
            }
            break;
        }

        case BS_RETURNING: {
            // Same as SENDING_OUT but backwards
            if (g.frameCount > 15 && g.frameCount < 45) {
                float animTime = MAX(1.0f - (float) (g.frameCount - 15) / 30.0f, 0.0f);
                drawTexturePro(
                    SPECS(PLAYERP.id).sprite, (Rectangle) {0, 0, 64, 64},
                    (Rectangle) {48.0f*animTime, 64.0f + 32.0f*animTime, 8.0f + 56.0f*animTime, 8.0f + 56.0f*animTime},
                    0.0f, WHITE
                );
            }
            else if (g.frameCount <= 15) {
                drawTexture(SPECS(PLAYERP.id).sprite, 48, 96, WHITE);
            }
            break;
        }

        case BS_ENEMY_SENDING_OUT: {
            // If enemy is sending out their first phone, don't draw the player phone yet.
            // Otherwise draw it like normal (using fallthrough).
            if (MENU.active == -1) break;
        }

        default:
            drawTexture(SPECS(PLAYERP.id).sprite, 48, 96, WHITE);
            break;
    }

    // Enemy phone sprite
    switch (MENU.battleState) {
        case BS_WON: break; // don't draw, it's dead

        case BS_STARTING: {
            // Wild battle: draw phone like normal. NPC battle: draw npc sprite.
            if (MENU.canRun) {
                drawTexture(SPECS(ENEMYP.id).sprite, 208, 96, WHITE);
            }
            else {
                drawTexture(MENU.enemySprite, 220, 96, WHITE);
            }
            break;
        }

        case BS_ENEMY_SENDING_OUT: {
            // (NPC battles only) animation (lasts 60 frames)
            float animTime = MIN((float) g.frameCount / 60.0f, 1.0f);

            // Draw the NPC only for their first phone in battle
            if (MENU.active == -1) {
                float npcAnimTime = MIN(animTime * 2.0f, 1.0f);  // first 30 frames
                drawTexturePro(
                    MENU.enemySprite, (Rectangle) {0, 0, 40, 64},
                    (Rectangle) {
                        220.0f + 120.0f*npcAnimTime, 96.0f - 20.0f*npcAnimTime,
                        40.0f - 10.0f*npcAnimTime, 64.0f - 16.0f*npcAnimTime
                    },
                    0.0f, WHITE
                );
            }
            float phoneAnimTime = MIN(MAX((animTime - 0.5f) * 2.0f, 0.0f), 1.0f);  // last 30 frames
            drawTexturePro(
                SPECS(ENEMYP.id).sprite, (Rectangle) {0, 0, 64, 64},
                (Rectangle) {
                    320.0f - 112.0f*phoneAnimTime, 76.0f + 20.0f*phoneAnimTime,
                    64.0f*phoneAnimTime, 64.0f*phoneAnimTime
                },
                0.0f, WHITE
            );
            break;
        }

        case BS_ENEMY_DIED: {
            // Death animation
            float animTime = MIN((float) g.frameCount / 30.0f, 1.0f);
            drawTexturePro(
                SPECS(ENEMYP.id).sprite, (Rectangle) {0, 0, 64, 64},
                (Rectangle) {208.0f - 16.0f*animTime, 96.0f + 64.0f*animTime, 64.0f + 32.0f*animTime, 64.0f - 64.0f*animTime},
                0.0f, WHITE
            );
            break;
        }

        default:
            drawTexture(SPECS(ENEMYP.id).sprite, 208, 96, WHITE);
            break;
    }

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