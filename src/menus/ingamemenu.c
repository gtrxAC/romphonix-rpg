// _____________________________________________________________________________
//
//  In-game menu
//  Init/draw/check functions for the in-game menu (accessible by pressing space
//  while in the overworld). State update is handled by the standard menu
//  system, no custom function is needed.
// _____________________________________________________________________________
//
#include "../common.h"

void drawInGameMenu();
void scrInGameMenuCheck();

// _____________________________________________________________________________
//
//  In-game menu - init function
// _____________________________________________________________________________
//
void scrInGameMenu() {
    const char *choices[] = {
        "Collection",
        "Phones",
        "Items",
        g.s.name,
        "Options",
    };
    pushMenu(5, choices, CB_CLOSE);
    MENU.drawFunc = drawInGameMenu;
    MENU.nextFunc = scrInGameMenuCheck;
    PlaySound(SOUND(hit));
}

// _____________________________________________________________________________
//
//  In-game menu - draw function
// _____________________________________________________________________________
//
void drawInGameMenu() {
    drawBox(0, 0, 130, 35 + 33*arrlen(MENU.choices));

    for (int i = 0; i < 5; i++) {
        DrawTextureRec(
            TEX(menu_icons), (Rectangle) {i*32, 0, 32, 32},
            (Vector2) {5, 5 + 33*i},
            (i == MENU.choice ? WHITE : ColorAlpha(WHITE, 0.5f))
        );
    }

    for (int i = 0; i < arrlen(MENU.choices) && MENU.choices[i]; i++) {
        drawText(
            g, MENU.choices[i], 42, 16 + 33*i,
            (i == MENU.choice ? WHITE : LIGHTGRAY)
        );
    }

    drawText(TextFormat("Money: $%d", g.s.money), 10, 178, WHITE);
}

// _____________________________________________________________________________
//
//  In-game menu - check user input function
// _____________________________________________________________________________
//
void scrInGameMenuCheck() {
    switch (MENU.choice) {
        case 0: scrCollectionMenu(); break;
        case 1: scrPhonesMenu(); break;
        case 2: scrItemsMenu(); break;
        case 3: scrPlayerMenu(); break;
        case 4: scrSettingsMenu(); break;
        default: popMenu(); break;
    }
}