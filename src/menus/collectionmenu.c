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

void scrCollectionMenu(Game *g);
void updateCollectionMenu(Game *g);
void drawCollectionMenu(Game *g);

// _____________________________________________________________________________
//
//  Collection menu - init function
// _____________________________________________________________________________
//
void scrCollectionMenu(Game *g) {
    g->state = ST_SCRIPT;
    g->menuUpdateFunc = updateCollectionMenu;
    g->menuDrawFunc = drawCollectionMenu;
    g->menuScroll = 0;
}

// _____________________________________________________________________________
//
//  Collection menu - state update function
// _____________________________________________________________________________
//
void updateCollectionMenu(Game *g) {
    if (K_B_PRESS()) scrInGameMenu(g);

    if (K_UP_PRESS() && g->menuChoice > 0) {
        g->menuChoice--;
        if (g->menuChoice == g->menuScroll - 1) g->menuScroll--;
    }
    else if (K_DOWN_PRESS() && g->menuChoice < g->phoneDB->size - 1) {
        g->menuChoice++;
        if (g->menuChoice == g->menuScroll + 11) g->menuScroll++;
    }
    if (K_LEFT_PRESS()) {
        for (int i = 0; i < 11 && g->menuChoice > 0; i++) {
            g->menuChoice--;
            if (g->menuChoice == g->menuScroll - 1) g->menuScroll--;
        }
    }
    if (K_RIGHT_PRESS()) {
        for (int i = 0; i < 11 && g->menuChoice < g->phoneDB->size - 1; i++) {
            g->menuChoice++;
            if (g->menuChoice == g->menuScroll + 11) g->menuScroll++;
        }
    }
}

// _____________________________________________________________________________
//
//  Collection menu - draw function
// _____________________________________________________________________________
//
void drawCollectionMenu(Game *g) {
    // "Collection" title
    drawBox(g, 0, 0, 144, 20);
    int collStrLen = measureText(g, "Collection");
    drawText(g, "Collection", 72 - collStrLen/2, 3, WHITE);

    // Phone list
    drawBox(g, 0, 20, 144, 220);
    for (int i = g->menuScroll; i < g->menuScroll + 11 && i < g->phoneDB->size; i++) {
        drawText(
            g, TextFormat(
                "%s %s", SPECS(i).brand, SPECS(i).model
            ), 20, 23 + (i - g->menuScroll)*20, WHITE
        );
    }

    // Phone list menu indicator (arrow)
    int selectorY = 23 + 20*(g->menuChoice - g->menuScroll);

    if (g->menuAnimDir == DIR_UP) selectorY += g->menuAnim;
    else selectorY -= g->menuAnim;
    DrawTexture(TEX(indicator), 6, selectorY, WHITE);

    // Phone list scrollbar
    DrawRectangle(140, 20 + 1.47f*g->menuScroll, 4, 16, ColorAlpha(WHITE, 0.3f));

    // Sprite window
    PhoneSpecs *selected = &SPECS(g->menuChoice);
    drawBox(g, 144, 0, 176, 96);
    DrawTexture(shget(g->textures, SPECS(g->menuChoice).sprite), 200, 16, WHITE);
 
    // Description
    drawBox(g, 144, 96, 176, 144);
    drawTextRec(g, selected->description, 148, 100, 168, 144, WHITE);

    drawText(g, TextFormat("Year: %d", selected->year), 252, 220, WHITE);
    drawText(g, "Rarity:", 148, 220, WHITE);
    DrawTextureRec(TEX(rarity), (Rectangle) {0, 12*selected->rarity, 64, 12}, (Vector2) {190, 220}, WHITE);
}