// _____________________________________________________________________________
//
//  Player info menu
//  Functions to implement the player info card menu. The player info menu uses
//  four PMS (Player Menu States), these are front, back, animation front to
//  back, and animation back to front.
// _____________________________________________________________________________
//
#include "../common.h"

void scrPlayerMenu(Game *g);
void updatePlayerMenu(Game *g);
void drawPlayerMenu(Game *g);
void checkPlayerMenu(Game *g);

// _____________________________________________________________________________
//
//  Player info menu - init function
// _____________________________________________________________________________
//
void scrPlayerMenu(Game *g) {
    pushMenu(g, 0, NULL, CB_CLOSE);
    MENU.updateFunc = updatePlayerMenu;
    MENU.drawFunc = drawPlayerMenu;
}

// _____________________________________________________________________________
//
//  Player info menu - state update function
// _____________________________________________________________________________
//
void updatePlayerMenu(Game *g) {
    if (K_B_PRESS()) popMenu(g);

    if (K_A_PRESS()) {
        if (MENU.pms == PMS_FRONT) {
            MENU.pms = PMS_ANIM_FTB;
            g->frameCount = 0;
        }
        else if (MENU.pms == PMS_BACK) {
            MENU.pms = PMS_ANIM_BTF;
            g->frameCount = 0;
        }
    }

    if (MENU.pms == PMS_ANIM_FTB) {
        if (g->frameCount >= 30) MENU.pms = PMS_BACK;
    }
    else if (MENU.pms == PMS_ANIM_BTF) {
        if (g->frameCount >= 30) MENU.pms = PMS_FRONT;
    }
}

// _____________________________________________________________________________
//
//  Player info menu - draw function
// _____________________________________________________________________________
//
void drawPlayerMenu(Game *g) {
    switch (MENU.pms) {
        case PMS_FRONT: {
            drawBox(g, 48, 64, 224, 112);
            DrawTexture(TEX(portraits), 52, 68, WHITE);
            drawTextL(g, g->s.name, 119, 68, WHITE);
            drawText(g, TextFormat("ID No. %d", g->s.id), 120, 90, WHITE);

            int collection = 0;
            for (int i = 0; i < g->phoneDB->size; i++) {
                if (g->s.phonesCaught[i]) collection++;
            }

            drawText(g, TextFormat("Collection: %d/%d", collection, g->phoneDB->size), 120, 104, WHITE);
            drawText(g, TextFormat("Date: %s", g->s.startDate), 120, 118, WHITE);
            drawText(g, TextFormat("Press L to flip!", g->s.startDate), 108, 154, GRAY);
            break;
        }

        case PMS_BACK: {
            drawBox(g, 48, 64, 224, 112);
            break;
        }

        case PMS_ANIM_BTF:
        case PMS_ANIM_FTB: {
            DrawTextureRec(
                TEX(flipbox),
                (Rectangle) {(g->frameCount/6)*326, 0, 326, 326},
                (Vector2) {-3, -43}, WHITE
            );
            break;
        }
    }
}