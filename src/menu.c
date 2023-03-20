#include "common.h"

void updateMenu(Game *g) {
    if (MENU.drawFunc) {
        MENU.drawFunc(g);
        return;
    }

    if (K_UP_PRESS() && MENU.choice) {
        MENU.choice--;
        MENU.menuAnim = 16;
        MENU.menuAnimDir = DIR_UP;
    }
    else if (K_DOWN_PRESS() && MENU.choice < arrlen(MENU.choices) - 1) {
        MENU.choice++;
        MENU.menuAnim = 16;
        MENU.menuAnimDir = DIR_DOWN;
    }
    if (MENU.menuAnim > 0) MENU.menuAnim -= 2;

    if (MENU.canSkip && K_B_PRESS()) {
        MENU.choice = -1;
        if (g->nextFunc) g->nextFunc(g);
        else g->state = ST_INGAME;
    }
    if (K_A_PRESS()) {
        if (g->nextFunc) g->nextFunc(g);
        else g->state = ST_INGAME;
    }
}

void drawMenu(Game *g) {
    if (MENU.drawFunc) {
        MENU.drawFunc(g);
        return;
    }

    // Get length of the longest menu choice
    int longest = 0;
    for (int i = 0; i < arrlen(MENU.choices); i++) {
        int length = measureText(g, MENU.choices[i]);
        if (length > longest) longest = length;
    }

    drawBox(g, 0, 0, 30 + longest, 16 + 14*arrlen(MENU.choices));

    for (int i = 0; i < arrlen(MENU.choices) && MENU.choices[i]; i++) {
        DrawTextEx(
            g->fonts.dialogue, MENU.choices[i],
            (Vector2) {22, 8 + 14*i},
            13, 0, WHITE
        );
    }

    int selectorY = 8 + 14*MENU.choice;

    if (MENU.menuAnimDir == DIR_UP) selectorY += MENU.menuAnim;
    else selectorY -= MENU.menuAnim;
    DrawTexture(TEX(indicator), 8, selectorY, WHITE);
}