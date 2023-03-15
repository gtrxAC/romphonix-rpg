// _____________________________________________________________________________
//
//  In-game menu
//  Init/draw/check functions for the in-game menu (accessible by pressing space
//  while in the overworld). State update is handled by the standard menu
//  scripting system, no custom function is needed.
// _____________________________________________________________________________
//
#include "../common.h"

void drawInGameMenu(Game *g);
void scrInGameMenuCheck(Game *g);

// _____________________________________________________________________________
//
//  In-game menu - init function
// _____________________________________________________________________________
//
void scrInGameMenu(Game *g) {
    g->state = ST_SCRIPT;

    const char *choices[] = {
        "Collection",
        "Phones",
        "Items",
        "Player",
        "Save",
        "Options",
    };
    menu(g, 6, choices, true);
    g->nextFunc = scrInGameMenuCheck;
    g->menuDrawFunc = drawInGameMenu; 
}

// _____________________________________________________________________________
//
//  In-game menu - draw function
// _____________________________________________________________________________
//
void drawInGameMenu(Game *g) {
    drawBox(g, 0, 0, 120, 35 + 33*g->numMenuChoices);

    for (int i = 0; i < 6; i++) {
        DrawTextureRec(
            TEX(menu_icons), (Rectangle) {i*32, 0, 32, 32},
            (Vector2) {5, 5 + 33*i},
            (i == g->menuChoice ? WHITE : ColorAlpha(WHITE, 0.5f))
        );
    }

    for (int i = 0; i < g->numMenuChoices && g->menuChoices[i]; i++) {
        drawText(g, g->menuChoices[i], 42, 16 + 33*i, WHITE);
    }

    drawText(g, TextFormat("Money: $%d", g->s.money), 42, 210, WHITE);
}

// _____________________________________________________________________________
//
//  In-game menu - check user input function
// _____________________________________________________________________________
//
void scrInGameMenuCheck(Game *g) {
    switch (g->menuChoice) {
        // case 4: scrSaveMenu(g); break;
        case 0: scrCollectionMenu(g); break;
        case 1: scrPhonesMenu(g); break;
        default: endScript(g); break;
    }
}

// void scrSaveMenu(Game *g) {
//     g->state = ST_SCRIPT;
    
// }

// void drawSaveMenu(Game *g) {

// }

