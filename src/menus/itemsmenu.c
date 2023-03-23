// _____________________________________________________________________________
//
//  Items menu
//  Functions to implement the functionality of the items menu (shows a
//  list of items the user has, grouped into a few categories)
// 
//  This menu uses fully custom functions, the menus provided by the scripting
//  system are not sufficient.
// _____________________________________________________________________________
//
#include "../common.h"

void scrItemsMenu(Game *g);
void updateItemsMenu(Game *g);
void drawItemsMenu(Game *g);
void checkItemsMenu(Game *g);

// _____________________________________________________________________________
//
//  Items menu - init function
// _____________________________________________________________________________
//
void scrItemsMenu(Game *g) {
    pushMenu(g, 0, NULL, true);
    MENU.updateFunc = updateItemsMenu;
    MENU.drawFunc = drawItemsMenu;
    MENU.nextFunc = checkItemsMenu;
}

// _____________________________________________________________________________
//
//  Items menu - state update function
// _____________________________________________________________________________
//
void updateItemsMenu(Game *g) {
    if (K_B_PRESS()) popMenu(g);

    if (K_UP_PRESS() && MENU.choice > 0) {
        MENU.choice--;
        if (MENU.choice == MENU.menuScroll - 1) MENU.menuScroll--;
    }
    else if (K_DOWN_PRESS() && MENU.choice < g->phoneDB->size - 1) {
        MENU.choice++;
        if (MENU.choice == MENU.menuScroll + 11) MENU.menuScroll++;
    }
    if (K_LEFT_PRESS()) {
    }
    if (K_RIGHT_PRESS()) {
    }
}

// _____________________________________________________________________________
//
//  Items menu - draw function
// _____________________________________________________________________________
//
void drawItemsMenu(Game *g) {
    // "Items" title (top left)
    drawBox(g, 0, 0, 160, 20);
    int textWidth = measureText(g, "Items");
    drawText(g, "Items", 80 - textWidth/2, 3, WHITE);

    // Bag sprite window (center left)
    drawBox(g, 0, 20, 160, 110);
    
    // Item description window (bottom left)
    drawBox(g, 0, 20, 160, 110);

    // Item list (right)
    drawBox(g, 160, 0, 160, 240);
}

// _____________________________________________________________________________
//
//  Items menu - check user input function
// _____________________________________________________________________________
//
void checkItemsMenu(Game *g) {
}