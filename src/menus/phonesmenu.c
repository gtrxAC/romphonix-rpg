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

void scrPhonesMenu();
void drawPhonesMenu();
void scrCheckPhonesMenu();

// _____________________________________________________________________________
//
//  Phones menu - init function
// _____________________________________________________________________________
//
void scrPhonesMenu() {
    pushMenu(6, NULL, CB_CLOSE);
    MENU.drawFunc = drawPhonesMenu;
    MENU.nextFunc = scrCheckPhonesMenu;

    // For this menu, we create 6 "dummy" values to let the user scroll freely.
    for (int i = 0; i < 6; i++) arrput(MENU.choices, "");
}

// _____________________________________________________________________________
//
//  Phones menu - draw function
// _____________________________________________________________________________
//
void drawPhonesMenu() {
    for (int i = 0; i < 6; i++) {
        Phone *ph = &g.s.party[i];
        PhoneSpecs *spec = &SPECS(ph->id);

        // Box
        drawBox(0, i*40, 320, 40);

        // Menu indicator (arrow)
        if (MENU.choice == i) {
            DrawTexture(TEX(indicator), 6, 14 + i*40, WHITE);
        }

        if (!g.s.party[i].active) continue;

        // Sprite
        DrawTextureRec(
            TEX(phone_icons),
            (Rectangle) {(g.frameCount % 40 < 20) * 32, 32*spec->icon, 32, 32},
            (Vector2) {MENU.choice == i ? 24 : 16, 4 + i*40}, WHITE
        );

        // Text
        // Note: '$' character in the digits font (drawTextD) says 'Lv.'
        drawText(TextFormat("%s %s", spec->brand, spec->model), 64, 5 + i*40, WHITE);
        drawTextD(TextFormat("$ %d", ph->level), 285, 5 + i*40, WHITE);

        // Health bar
        drawProgressBar(ph->hp, ph->maxHP, 64, i*40 + 21, 100, GREEN);
        drawTextD(TextFormat("%d/%d", ph->hp, ph->maxHP), 172, i*40 + 20, WHITE);
    }
}

// _____________________________________________________________________________
//
//  Phones menu - check user input function
// _____________________________________________________________________________
//
void scrCheckPhonesMenu() {
    if (MENU.choice == -1) {
        popMenu();
    }
    else {
        // If this slot isn't empty, show the actions menu
        if (g.s.party[MENU.choice].active) {
            scrPhoneActionsMenu();
        }
    }
}