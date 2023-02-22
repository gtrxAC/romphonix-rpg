#include "../common.h"

void scrInGameMenuCheck(Game *g);

void scrInGameMenu(Game *g) {
    g->state = ST_SCRIPT;

    const char *choices[] = {
        "Collection",
        "Phones",
        "Items",
        "Player",
        "Save",
        "Options",
        "Money: ???"
    };
    menu(g, 7, choices, true);
    g->nextFunc = scrInGameMenuCheck;
}

void scrInGameMenuCheck(Game *g) {
    switch (g->menuChoice) {
        case 4: scrSaveMenu(g); break;
        case 6: break;
    }
}