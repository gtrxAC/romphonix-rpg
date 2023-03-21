#include "common.h"

void updateMenu(Game *g);
void drawMenu(Game *g);

// _____________________________________________________________________________
//
//  Shows a menu where the user can select from a list of choices.
//  The menu is pushed to the top of a menu stack, where previous menus are
//  stored and can be returned back to.
//
//  If canSkip is set to true, the player can close the menu by pressing the B
//  button (see keybindings in common.h). In this case, menuChoice is set to -1.
//  canSkip only applies if updateMenu is used as the update function.
// _____________________________________________________________________________
//
void pushMenu(Game *g, int numChoices, const char **choices, bool canSkip) {
	arrput(g->menus, (Menu) {0});
	MENU.canSkip = canSkip;
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
void updateMenu(Game *g) {
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
        if (MENU.nextFunc) MENU.nextFunc(g);
    }
    if (K_A_PRESS()) {
        if (MENU.nextFunc) MENU.nextFunc(g);
    }
}

// _____________________________________________________________________________
//
//  Draw menu
//  Default behavior which can be overridden by setting MENU.updateFunc
// _____________________________________________________________________________
//
void drawMenu(Game *g) {
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