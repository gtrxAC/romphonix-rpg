#include "../common.h"

void scrInGameMenuCheck(Game *g);
void drawCollectionMenu(Game *g);
void updateCollectionMenu(Game *g);
void scrCollectionMenu(Game *g);

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

void scrInGameMenuCheck(Game *g) {
    switch (g->menuChoice) {
        // case 4: scrSaveMenu(g); break;
        case 0: scrCollectionMenu(g); break;
        default: endScript(g); break;
    }
}

// void scrSaveMenu(Game *g) {
//     g->state = ST_SCRIPT;
    
// }

// void drawSaveMenu(Game *g) {

// }

void scrCollectionMenu(Game *g) {
    g->state = ST_SCRIPT;
    g->menuUpdateFunc = updateCollectionMenu;
    g->menuDrawFunc = drawCollectionMenu;
}

void drawCollectionMenu(Game *g) {
    // "Collection" title
    drawBox(g, 0, 0, 130, 20);
    int collStrLen = measureText(g, "Collection");
    drawText(g, "Collection", 65 - collStrLen/2, 3, WHITE);

    // Phone list
    drawBox(g, 0, 20, 130, 220);
    for (int i = 0; i < 11 && i < g->phoneDB->size; i++) {
        drawText(
            g, TextFormat(
                "%s %s", g->phoneDB->phones[i].brand, g->phoneDB->phones[i].model
            ), 5, 23 + i*20, WHITE
        );
    }
}

void updateCollectionMenu(Game *g) {
    if (K_B_PRESS()) scrInGameMenu(g);

    if (K_UP_PRESS()) {

    }
    else if (K_DOWN_PRESS()) {

    }
}