#include "common.h"

// _____________________________________________________________________________
//
//  Update title
// _____________________________________________________________________________
//
void scrMainMenu();

void updateTitle(Game *g) {
    if (
        K_UP_PRESS() || K_DOWN_PRESS() || K_LEFT_PRESS() || K_RIGHT_PRESS() ||
        K_A_PRESS() || K_B_PRESS() || K_MENU_PRESS()
    ) {
        g->state = ST_MAINMENU;
        scrMainMenu(g);
    }
}

// _____________________________________________________________________________
//
//  Draw title
//  Note: we are inside a render texture so the screen size is 320 × 240
// _____________________________________________________________________________
//
void drawTitle(Game *g) {
    DrawTexture(TEX(title), 0, 0, WHITE);
    if (g->frameCount % 60 < 30) {
        int length = MeasureTextEx(g->fonts.dialogue, "Press Any Key", 13, 0).x;
        drawText(g, "Press Any Key", 160 - length/2, 180, WHITE);
    }
}