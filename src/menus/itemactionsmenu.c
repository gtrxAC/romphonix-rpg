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
}

// _____________________________________________________________________________
//
//  Item actions menu - draw function
// _____________________________________________________________________________
//
void drawItemActionsMenu(Game *g) {
    // Draw the previous (items) menu first
    Menu menu = arrpop(g->menus);
    drawItemsMenu(g);
    arrput(g->menus, menu);

    // Darken it slightly by drawing a transparent black rectangle
    DrawRectangle(0, 0, 320, 240, ColorAlpha(BLACK, 0.2f));

    drawText(g, "a", 0, 0, WHITE);
}

// _____________________________________________________________________________
//
//  Item actions menu - check user input function
// _____________________________________________________________________________
//
void checkItemActionsMenu(Game *g) {
}