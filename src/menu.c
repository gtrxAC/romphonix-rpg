#include "common.h"

void updateMenu();
void drawMenu();

// _____________________________________________________________________________
//
//  Shows a menu where the user can select from a list of choices.
//  The menu is pushed to the top of a menu stack, where previous menus are
//  stored and can be returned back to.
// _____________________________________________________________________________
//
void pushMenu(int numChoices, const char **choices, CloseBehavior closeBehav) {
	arrput(g.menus, (Menu) {0});
	MENU.closeBehav = closeBehav;
	MENU.updateFunc = updateMenu;
	MENU.drawFunc = drawMenu;

    if (choices != NULL) {
        for (int i = 0; i < numChoices; i++) {
            arrput(MENU.choices, choices[i]);
        }
    }

    MENU.nextFunc = NULL;
}

// _____________________________________________________________________________
//
//  Update menu
//  Default behavior which can be overridden by setting MENU.updateFunc
// _____________________________________________________________________________
//
void updateMenu() {
    if (K_UP_PRESS() && MENU.choice) MENU.choice--;

    else if (K_DOWN_PRESS() && MENU.choice < arrlen(MENU.choices) - 1) {
        MENU.choice++;
    }

    if (K_B_PRESS()) {
        switch (MENU.closeBehav) {
            case CB_NOTHING: break;

            case CB_NEXT_MENU: {
                MENU.choice = -1;
                if (MENU.nextFunc) MENU.nextFunc();
                break;
            }

            case CB_CLOSE:
                popMenu();
                break;
        }
    }
    if (K_A_PRESS()) {
        if (MENU.nextFunc) MENU.nextFunc();
    }
}

// _____________________________________________________________________________
//
//  Draw menu
//  Default behavior which can be overridden by setting MENU.drawFunc
// _____________________________________________________________________________
//
void drawMenu() {
    // Get length of the longest menu choice
    int longest = 0;
    for (int i = 0; i < arrlen(MENU.choices); i++) {
        int length = measureText(MENU.choices[i]);
        if (length > longest) longest = length;
    }

    drawBox(0, 0, 30 + longest, 16 + 14*arrlen(MENU.choices));

    for (int i = 0; i < arrlen(MENU.choices) && MENU.choices[i]; i++) {
        DrawTextEx(
            g.fonts.dialogue, MENU.choices[i],
            (Vector2) {22, 8 + 14*i},
            13, 0, WHITE
        );
    }

    DrawTexture(TEX(indicator), 8, 8 + 14*MENU.choice, WHITE);
}