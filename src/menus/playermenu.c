// _____________________________________________________________________________
//
//  Player info menu
//  Functions to implement the player info card menu. The player info menu uses
//  four PMS (Player Menu States), these are front, back, animation front to
//  back, and animation back to front.
// _____________________________________________________________________________
//
#include "../common.h"

// _____________________________________________________________________________
//
//  Player info menu - init function
// _____________________________________________________________________________
//
void scrPlayerMenu() {
    pushMenu(CB_CLOSE);
    setUpdateFunc(updatePlayerMenu);
    setDrawFunc(drawPlayerMenu);
}

// _____________________________________________________________________________
//
//  Player info menu - state update function
// _____________________________________________________________________________
//
void updatePlayerMenu() {
    if (K_B_PRESS()) popMenu();

    if (K_A_PRESS()) {
        if (MENU.pms == PMS_FRONT) {
            MENU.pms = PMS_ANIM_FTB;
            g.frameCount = 0;
        }
        else if (MENU.pms == PMS_BACK) {
            MENU.pms = PMS_ANIM_BTF;
            g.frameCount = 0;
        }
    }

    if (MENU.pms == PMS_ANIM_FTB) {
        if (g.frameCount >= 30) MENU.pms = PMS_BACK;
    }
    else if (MENU.pms == PMS_ANIM_BTF) {
        if (g.frameCount >= 30) MENU.pms = PMS_FRONT;
    }
}

// _____________________________________________________________________________
//
//  Player info menu - draw function
// _____________________________________________________________________________
//
void drawPlayerMenu() {
    switch (MENU.pms) {
        case PMS_FRONT: {
            drawBox(48, 64, 224, 112);
            drawTexture(F("large_player%d", g.s.appearance), 64, 70, WHITE);
            drawTextL(g.s.name, 119, 68, WHITE);
            drawText(F("ID No. %d", g.s.id), 120, 90, WHITE);

            int collection = 0;
            for (int i = 0; i < g.phoneDB->size; i++) {
                if (g.s.phonesCaught[i]) collection++;
            }

            drawText(F("Collection: %d/%d", collection, g.phoneDB->size), 120, 104, WHITE);
            drawText(F("Date: %s", g.s.startDate), 120, 118, WHITE);
            drawText(F("Press L to flip!", g.s.startDate), 108, 154, GRAY);
            break;
        }

        case PMS_BACK: {
            drawBox(48, 64, 224, 112);
            break;
        }

        case PMS_ANIM_BTF:
        case PMS_ANIM_FTB: {
            drawTextureRec(
                "flipbox",
                (Rectangle) {(g.frameCount/6)*326, 0, 326, 326},
                (Vector2) {-3, -43}, WHITE
            );
            break;
        }
    }
}