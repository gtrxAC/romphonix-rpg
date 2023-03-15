// _____________________________________________________________________________
//
//  Phones menu
//  Shows a list of the phones the user has in their current party (which can be
//  used in battle).
//
//  The menu controls are handled by the scripting system, everything else like
//  drawing uses custom functions.
// _____________________________________________________________________________
//
#include "../common.h"

void scrPhonesMenu(Game *g);
void drawPhonesMenu(Game *g);
void scrCheckPhonesMenu(Game *g);

// _____________________________________________________________________________
//
//  Phones menu - init function
// _____________________________________________________________________________
//
void scrPhonesMenu(Game *g) {
    g->state = ST_SCRIPT;
    // g->menuUpdateFunc = updatePhonesMenu;
    g->menuDrawFunc = drawPhonesMenu;
    g->menuScroll = 0;
}

// _____________________________________________________________________________
//
//  Phones menu - draw function
// _____________________________________________________________________________
//
void drawPhonesMenu(Game *g) {
    for (int i = 0; i < 6; i++) {
        Phone *ph = &g->s.party[i];
        PhoneSpecs *spec = &SPECS(ph->id);

        // Box
        drawBox(g, 0, i*40, 320, 40);

        // Menu indicator (arrow)
        if (g->menuChoice == i) {
            DrawTexture(TEX(indicator), 6, 14 + i*40, WHITE);
        }

        if (!g->s.party[i].active) continue;

        // Sprite
        DrawTextureRec(
            TEX(phone_icons),
            (Rectangle) {(g->frameCount % 40 < 20) * 32, 32*spec->icon, 32, 32},
            (Vector2) {g->menuChoice == i ? 24 : 16, 4 + i*40}, WHITE
        );

        // Text
        drawText(g, TextFormat("%s %s", spec->brand, spec->model), 64, 5 + i*40, WHITE);

        // Health bar
        DrawRectangle(64, i*40 + 21, 102, 12, BLACK);
        DrawRectangle(66, i*40 + 23, ((float) ph->hp / ph->maxHP) * 100, 8, BLUE);
        drawTextD(g, TextFormat("%d/%d", ph->hp, ph->maxHP), 172, i*40 + 20, WHITE);
    }
}

// _____________________________________________________________________________
//
//  Phones menu - check user input function
// _____________________________________________________________________________
//
void scrCheckPhonesMenu(Game *g) {
}