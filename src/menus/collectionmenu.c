// _____________________________________________________________________________
//
//  Collection menu
//  Functions to implement the functionality of the collection menu (shows a
//  full list of phones the user has seen/owned, and a basic description and
//  other info about them).
// 
//  This menu uses fully custom functions, the menus provided by the scripting
//  system are not sufficient. A check function is not used, since the user
//  cannot select anything.
// _____________________________________________________________________________
//
#include "../common.h"

// _____________________________________________________________________________
//
//  Collection menu - init function
// _____________________________________________________________________________
//
void scrCollectionMenu() {
    pushMenu(CB_CLOSE);
    setUpdateFunc(updateCollectionMenu);
    setDrawFunc(drawCollectionMenu);
}

// _____________________________________________________________________________
//
//  Collection menu - state update function
// _____________________________________________________________________________
//
void updateCollectionMenu() {
    if (K_B_PRESS()) popMenu();

    if (K_UP_PRESS() && MENU.choice > 0) {
        MENU.choice--;
        if (MENU.choice == MENU.scroll - 1) MENU.scroll--;
    }
    else if (K_DOWN_PRESS() && MENU.choice < g.phoneDB->size - 1) {
        MENU.choice++;
        if (MENU.choice == MENU.scroll + 11) MENU.scroll++;
    }
    if (K_LEFT_PRESS()) {
        for (int i = 0; i < 11 && MENU.choice > 0; i++) {
            MENU.choice--;
            if (MENU.choice == MENU.scroll - 1) MENU.scroll--;
        }
    }
    if (K_RIGHT_PRESS()) {
        for (int i = 0; i < 11 && MENU.choice < g.phoneDB->size - 1; i++) {
            MENU.choice++;
            if (MENU.choice == MENU.scroll + 11) MENU.scroll++;
        }
    }
}

// _____________________________________________________________________________
//
//  Collection menu - draw function
// _____________________________________________________________________________
//
void drawCollectionMenu() {
    // "Collection" title
    drawBox(0, 0, 144, 20);
    int collStrLen = measureText("Collection");
    drawText("Collection", 72 - collStrLen/2, 3, WHITE);

    // Phone list
    drawBox(0, 20, 144, 220);
    for (int i = MENU.scroll; i < MENU.scroll + 11 && i < g.phoneDB->size; i++) {
        if (g.s.phonesSeen[i]) {
            Color color = GRAY;
            if (g.s.phonesCaught[i]) color = WHITE;

            drawText(
                F("%s %s", SPECS(i).brand, SPECS(i).model),
                20, 23 + (i - MENU.scroll)*20, color
            );
        }
        else {
            drawText("???", 20, 23 + (i - MENU.scroll)*20, DARKGRAY);
        }
    }

    // Phone list menu indicator (arrow)
    drawTexture("indicator", 6, 23 + 20*(MENU.choice - MENU.scroll), WHITE);

    // Phone list scrollbar
    DrawRectangle(140, 20 + 1.47f*MENU.scroll, 4, 16, ColorAlpha(WHITE, 0.3f));

    // Sprite window
    drawBox(144, 0, 176, 96);
    PhoneSpecs *selected = &SPECS(MENU.choice);

    drawTexture("shadow", 200, 70, WHITE);
    if (g.s.phonesSeen[MENU.choice]) {
        drawTexture(selected->sprite, 200, 16, WHITE);
    }
    else {
        drawTexture("unknown_phone", 200, 16, WHITE);
    }
 
    // Description
    drawBox(144, 96, 176, 144);

    if (g.s.phonesSeen[MENU.choice]) {
        drawTextRec(selected->description, 149, 101, 166, 142, WHITE);

        drawText(F("Year: %d", selected->year), 252, 220, WHITE);
        drawText("Rarity:", 149, 220, WHITE);
        drawTextureRec(
            "rarity", (Rectangle) {0, 12*selected->rarity, 64, 12},
            (Vector2) {191, 220}, WHITE
        );
    }
    else {
        drawTextRec("Not much is known about this phone yet.", 149, 101, 166, 142, WHITE);
    }
}