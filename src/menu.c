#include "common.h"

void updateMenu(Game *g) {
    if (MENU.menuDrawFunc) {
        MENU.menuDrawFunc(g);
        return;
    }

    if (K_UP_PRESS() && MENU.menuChoice) {
        MENU.menuChoice--;
        MENU.menuAnim = 16;
        MENU.menuAnimDir = DIR_UP;
    }
    else if (K_DOWN_PRESS() && MENU.menuChoice < MENU.numMenuChoices - 1) {
        MENU.menuChoice++;
        MENU.menuAnim = 16;
        MENU.menuAnimDir = DIR_DOWN;
    }
    if (MENU.menuAnim > 0) MENU.menuAnim -= 2;

    if (MENU.canSkipMenu && K_B_PRESS()) {
        MENU.menuChoice = -1;
        if (g->nextFunc) g->nextFunc(g);
        else g->state = ST_WORLD;
    }
    if (K_A_PRESS()) {
        if (g->nextFunc) g->nextFunc(g);
        else g->state = ST_WORLD;
    }
}

void drawMenu(Game *g) {
    if (MENU.menuDrawFunc) {
        MENU.menuDrawFunc(g);
        return;
    }

    // Get length of the longest menu choice
    int longest = 0;
    for (int i = 0; i < MENU.numMenuChoices; i++) {
        int length = measureText(g, MENU.menuChoices[i]);
        if (length > longest) longest = length;
    }

    drawBox(g, 0, 0, 30 + longest, 16 + 14*MENU.numMenuChoices);

    for (int i = 0; i < MENU.numMenuChoices && MENU.menuChoices[i]; i++) {
        DrawTextEx(
            g->fonts.dialogue, MENU.menuChoices[i],
            (Vector2) {22, 8 + 14*i},
            13, 0, WHITE
        );
    }

    int selectorY = 8 + 14*MENU.menuChoice;

    if (MENU.menuAnimDir == DIR_UP) selectorY += MENU.menuAnim;
    else selectorY -= MENU.menuAnim;
    DrawTexture(TEX(indicator), 8, selectorY, WHITE);
}