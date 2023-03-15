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
            (Rectangle) {(g->frameCount % 60 < 30) * 32, 0, 32, 32},
            (Vector2) {g->menuChoice == i ? 28 : 16, 4 + i*40}, WHITE
        );

        // Text
        Phone *ph = &g->s.party[i];
        PhoneSpecs *spec = &g->phoneDB->phones[ph->id];
        drawText(g, TextFormat("%s %s", spec->brand, spec->model), 64, 4 + i*40, WHITE);
    }
}

// _____________________________________________________________________________
//
//  Phones menu - check user input function
// _____________________________________________________________________________
//
void scrCheckPhonesMenu(Game *g) {
}