// _____________________________________________________________________________
//
//  Item actions menu
//  Functions to implement the item action menu (use, toss, give, etc.). This is
//  a sub-menu of the items/bag menu. This menu accesses data of the previous
//  menu in the stack to know which item is selected and what actions to show.
//
//  This menu acts like any normal selection menu, so no custom update function
//  is needed. A custom draw function is used to show the options in the bottom
//  right corner, and to draw the bag menu behind it.
// _____________________________________________________________________________
//
#include "../common.h"

void scrItemActionsMenu(Game *g);
void updateItemActionsMenu(Game *g);
void drawItemActionsMenu(Game *g);
void checkItemActionsMenu(Game *g);

// _____________________________________________________________________________
//
//  Item actions menu - init function
// _____________________________________________________________________________
//
void scrItemActionsMenu(Game *g) {
    pushMenu(g, 0, NULL, true);
    // MENU.updateFunc = updateItemActionsMenu;
    MENU.drawFunc = drawItemActionsMenu;
    MENU.nextFunc = checkItemActionsMenu;
    MENU.canSkip = true;

    arrpush(MENU.choices, "Use");
    arrpush(MENU.choices, "Give");
    arrpush(MENU.choices, "Toss");
}

// _____________________________________________________________________________
//
//  Item actions menu - draw function
//  Also used for the phone actions menu.
// _____________________________________________________________________________
//
void drawItemActionsMenu(Game *g) {
    // Draw the previous (items) menu first
    // We can't use LASTMENU macro here because the draw function expects the
    // items menu to be at the top, so we need to temporarily pop the actions
    // menu. A bit inefficient, but it works.
    Menu menu = arrpop(g->menus);
    MENU.drawFunc(g);
    arrput(g->menus, menu);

    // Darken it slightly by drawing a transparent black rectangle
    DrawRectangle(0, 0, 320, 240, ColorAlpha(BLACK, 0.3f));

    // Code based on standard drawMenu function, but draws on the bottom right
    // instead of the top left.

    // Get length of the longest menu choice
    int longest = 0;
    for (int i = 0; i < arrlen(MENU.choices); i++) {
        int length = measureText(g, MENU.choices[i]);
        if (length > longest) longest = length;
    }

    int baseX = 290 - longest;
    int baseY = 224 - 14*arrlen(MENU.choices);

    drawBox(g, baseX, baseY, 30 + longest, 16 + 14*arrlen(MENU.choices));

    for (int i = 0; i < arrlen(MENU.choices) && MENU.choices[i]; i++) {
        DrawTextEx(
            g->fonts.dialogue, MENU.choices[i],
            (Vector2) {baseX + 22, baseY + 8 + 14*i},
            13, 0, WHITE
        );
    }

    DrawTexture(TEX(indicator), baseX + 8, baseY + 8 + 14*MENU.choice, WHITE);
}

// _____________________________________________________________________________
//
//  Item actions menu - check user input function
// _____________________________________________________________________________
//
void checkItemActionsMenu(Game *g) {
    switch (MENU.choice) {
        case -1: popMenu(g); break;

        case 0: {

        }

        case 1: {

        }

        case 2: {

        }
    }
}