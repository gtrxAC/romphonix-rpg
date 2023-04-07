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
#include "itemactionsmenu.h"

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
    pushMenu(g, 0, NULL, CB_CLOSE);
    MENU.updateFunc = updateItemsMenu;
    MENU.drawFunc = drawItemsMenu;
}

// _____________________________________________________________________________
//
//  Items menu - state update function
//  Works similarly to the collection menu, but has three bag pockets which work
//  like tabs.
// _____________________________________________________________________________
//
void updateItemsMenu(Game *g) {
    #define ITEMCOUNT (arrlen(g->bag[MENU.bagChoice]))
    
    if (K_B_PRESS()) popMenu(g);

    if (K_UP_PRESS() && MENU.choice > 0) {
        MENU.choice--;
        if (MENU.choice == MENU.scroll - 1) MENU.scroll--;
    }
    else if (K_DOWN_PRESS() && MENU.choice < ITEMCOUNT - 1) {
        MENU.choice++;
        if (MENU.choice == MENU.scroll + 12) MENU.scroll++;
    }
    if (K_LEFT_PRESS()) {
        MENU.bagChoice--;
        if (MENU.bagChoice < 0) MENU.bagChoice = 2;
        MENU.choice = 0;
        MENU.scroll = 0;
    }
    if (K_RIGHT_PRESS()) {
        MENU.bagChoice++;
        if (MENU.bagChoice > 2) MENU.bagChoice = 0;
        MENU.choice = 0;
        MENU.scroll = 0;
    }
    if (K_A_PRESS() && ITEMCOUNT) {
        scrItemActionsMenu(g);
    }
}

// _____________________________________________________________________________
//
//  Items menu - draw function
// _____________________________________________________________________________
//
void drawItemsMenu(Game *g) {
    #define CURPOCKET (g->bag[MENU.bagChoice])

    // "Items" title (top left)
    drawBox(g, 0, 0, 160, 20);
    int textWidth = measureText(g, "Items");
    drawText(g, "Items", 80 - textWidth/2, 3, WHITE);

    // Bag sprite window (center left), bag texture
    drawBox(g, 0, 20, 160, 110);
    DrawTextureRec(
        TEX(bag),
        (Rectangle) {MENU.bagChoice*96, 0, 96, 96},
        (Vector2) {32, 27}, WHITE
    );

    // Selected item sprite (below the bag sprite)
    if (arrlen(CURPOCKET)) {
        DrawTexture(shget(g->textures, ISPECS(CURPOCKET[MENU.choice].id).sprite), 112, 87, WHITE);
    }
    
    // Item description window (bottom left)
    drawBox(g, 0, 130, 160, 110);
    if (arrlen(CURPOCKET)) {
        drawTextRec(g, ISPECS(CURPOCKET[MENU.choice].id).description, 4, 134, 152, 102, WHITE);
    }

    // Item list (right)
    drawBox(g, 160, 0, 160, 240);
    for (
        int i = MENU.scroll;
        i < MENU.scroll + 12 && i < arrlen(CURPOCKET);
        i++
    ) {
        // Item name
        drawText(
            g, ISPECS(CURPOCKET[i].id).name,
            179, 6 + (i - MENU.scroll)*20, WHITE
        );

        // Item count
        drawTextD(
            g, TextFormat("* %d", CURPOCKET[i].count),
            287, 6 + (i - MENU.scroll)*20, WHITE
        );
    }

    // Item list menu indicator/arrow
    if (arrlen(CURPOCKET)) {
        DrawTexture(TEX(indicator), 165, 6 + 20*(MENU.choice - MENU.scroll), WHITE);
    } else {
        // Centered "No items" text if pocket is empty
        int textLen = measureText(g, "You have no items.");
        drawText(g, "You have no items.", 240 - textLen/2, 114, WHITE);
    }
}