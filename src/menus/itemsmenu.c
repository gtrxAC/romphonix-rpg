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
//  Works similarly to the collection menu, but has three bag pockets which work
//  like tabs.
// _____________________________________________________________________________
//
void updateItemsMenu(Game *g) {
    if (K_B_PRESS()) popMenu(g);

    if (K_UP_PRESS() && MENU.choice > 0) {
        MENU.choice--;
        if (MENU.choice == MENU.menuScroll - 1) MENU.menuScroll--;
    }
    else if (K_DOWN_PRESS() && MENU.choice < arrlen(g->bag[MENU.bagChoice]) - 1) {
        MENU.choice++;
        if (MENU.choice == MENU.menuScroll + 12) MENU.menuScroll++;
    }
    if (K_LEFT_PRESS()) {
        MENU.bagChoice--;
        if (MENU.bagChoice < 0) MENU.bagChoice = 2;
        MENU.choice = 0;
        MENU.menuScroll = 0;
    }
    if (K_RIGHT_PRESS()) {
        MENU.bagChoice++;
        if (MENU.bagChoice > 2) MENU.bagChoice = 0;
        MENU.choice = 0;
        MENU.menuScroll = 0;
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

    // Bag sprite window (center left), bag texture and selected item sprite
    drawBox(g, 0, 20, 160, 110);
    DrawTextureRec(
        TEX(bag),
        (Rectangle) {MENU.bagChoice*96, 0, 96, 96},
        (Vector2) {32, 27}, WHITE
    );
    DrawTexture(shget(g->textures, ISPECS(CURPOCKET[MENU.choice].id).sprite), 112, 87, WHITE);
    
    // Item description window (bottom left)
    drawBox(g, 0, 130, 160, 110);

    // Item list (right)
    drawBox(g, 160, 0, 160, 240);
    for (
        int i = MENU.menuScroll;
        i < MENU.menuScroll + 12 && i < arrlen(CURPOCKET);
        i++
    ) {
        // Item name
        drawText(
            g, ISPECS(CURPOCKET[i].id).name,
            180, 6 + (i - MENU.menuScroll)*20, WHITE
        );

        // Item count
        drawTextD(
            g, TextFormat("* %d", CURPOCKET[i].count),
            290, 6 + (i - MENU.menuScroll)*20, WHITE
        );
    }

    // Item list menu indicator/arrow
    int selectorY = 6 + 20*(MENU.choice - MENU.menuScroll);

    if (MENU.menuAnimDir == DIR_UP) selectorY += MENU.menuAnim;
    else selectorY -= MENU.menuAnim;
    DrawTexture(TEX(indicator), 165, selectorY, WHITE);
}

// _____________________________________________________________________________
//
//  Items menu - check user input function
// _____________________________________________________________________________
//
void checkItemsMenu(Game *g) {
}