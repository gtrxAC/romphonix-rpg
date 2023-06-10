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

// _____________________________________________________________________________
//
//  Items menu - init function
// _____________________________________________________________________________
//
void scrItemsMenu() {
    pushMenu(CB_CLOSE);
    setUpdateFunc(updateItemsMenu);
    setDrawFunc(drawItemsMenu);
    MENU.bagChoice = 1;  // start in the middle pocket (items)
}

// _____________________________________________________________________________
//
//  Items menu - state update function
//  Works similarly to the collection menu, but has three bag pockets which work
//  like tabs.
// _____________________________________________________________________________
//
void updateItemsMenu() {
    #define ITEMCOUNT (arrlen(g.bag[MENU.bagChoice]))
    
    if (K_B_PRESS()) popMenu();

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
        scrItemActionsMenu();
    }
}

// _____________________________________________________________________________
//
//  Items menu - draw function
// _____________________________________________________________________________
//
const char *pocketNames[3] = {
    "Bag - Memory Cards",
    "Bag - Items",
    "Bag - Healing"
};

void drawItemsMenu() {
    #define CURPOCKET (g.bag[MENU.bagChoice])

    // "Items" title (top left)
    drawBox(0, 0, 140, 20);
    int textWidth = measureText(pocketNames[MENU.bagChoice]);
    drawText(pocketNames[MENU.bagChoice], 70 - textWidth/2, 3, WHITE);

    // Bag sprite window (center left), bag texture
    drawBox(0, 20, 140, 110);
    drawTextureRec(
        "bag",
        (Rectangle) {MENU.bagChoice*96, 0, 96, 96},
        (Vector2) {22, 27}, WHITE
    );

    // Selected item sprite (below the bag sprite)
    if (arrlen(CURPOCKET)) {
        drawTexture(ISPECS(CURPOCKET[MENU.choice].id).sprite, 102, 87, WHITE);
    }
    
    // Item description window (bottom left)
    drawBox(0, 130, 140, 110);
    if (arrlen(CURPOCKET)) {
        drawTextRec(ISPECS(CURPOCKET[MENU.choice].id).description, 5, 134, 130, 102, WHITE);
    }

    // Item list (right)
    drawBox(140, 0, 180, 240);
    for (
        int i = MENU.scroll;
        i < MENU.scroll + 12 && i < arrlen(CURPOCKET);
        i++
    ) {
        // Item name
        drawText(
            ISPECS(CURPOCKET[i].id).name,
            160, 6 + (i - MENU.scroll)*20, WHITE
        );

        // Item count
        drawTextD(
            F("* %d", CURPOCKET[i].count),
            284, 6 + (i - MENU.scroll)*20, WHITE
        );
    }

    // Item list menu indicator/arrow
    if (arrlen(CURPOCKET)) {
        drawTexture("indicator", 146, 6 + 20*(MENU.choice - MENU.scroll), WHITE);
    }
    else {
        // Centered "No items" text if pocket is empty
        int textLen = measureText("This pocket is empty.");
        drawText("This pocket is empty.", 230 - textLen/2, 114, WHITE);
    }
}