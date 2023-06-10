// _____________________________________________________________________________
//
//  Item actions menu
//  Functions to implement the item action menu (use, toss, give, etc.). This is
//  a sub-menu of the items/bag menu. This menu accesses data of the previous
//  menu in the stack to know which item is selected and what actions to show.
//
//  This menu acts like any normal selection menu, so no custom update function
//  is needed. A custom draw function is used to show the options in the bottom
//  right corner, and to draw the bag menu behind it.
// _____________________________________________________________________________
//
#include "../common.h"

// _____________________________________________________________________________
//
//  Item actions menu - init function
// _____________________________________________________________________________
//
void scrItemActionsMenu() {
    pushMenu(CB_CLOSE);
    setDrawFunc(drawItemActionsMenu);
    setNextFunc(checkItemActionsMenu);
    MENU.drawPrevious = true;

    addChoice("Use");
    addChoice("Give");
    addChoice("Toss");
}

// _____________________________________________________________________________
//
//  Item actions menu - draw function
//  Also used for the phone actions menu.
// _____________________________________________________________________________
//
void drawItemActionsMenu() {
    // Darken the previous menu by drawing a black checkerboard over it
    for (int y = 0; y < 240; y += 80) {
        for (int x = 0; x < 320; x += 80) {
            drawTexture("checkerboard", x, y, BLACK);
            drawTexture("checkerboard", x + 1, y, ColorAlpha(BLACK, 0.5f));
        }
    }

    // Code based on standard drawMenu function, but draws on the bottom right
    // instead of the top left.

    // Get length of the longest menu choice
    int longest = 0;
    for (int i = 0; i < arrlen(MENU.choices); i++) {
        int length = measureText(MENU.choices[i]);
        if (length > longest) longest = length;
    }

    int baseX = 290 - longest;
    int baseY = 224 - 14*arrlen(MENU.choices);

    drawBox(baseX, baseY, 30 + longest, 16 + 14*arrlen(MENU.choices));

    for (int i = 0; i < arrlen(MENU.choices) && MENU.choices[i]; i++) {
        DrawTextEx(
            g.fonts.dialogue, MENU.choices[i],
            (Vector2) {baseX + 22, baseY + 8 + 14*i},
            13, 0, WHITE
        );
    }

    drawTexture("indicator", baseX + 8, baseY + 8 + 14*MENU.choice, WHITE);
}

// _____________________________________________________________________________
//
//  Item actions menu - check user input function
// _____________________________________________________________________________
//
void checkItemActionsMenu() {
    int choice = MENU.choice;
    popMenu();
    // Now MENU refers to the items menu

    switch (choice) {
        // Use
        case 0: {
            switch (ISPECS(g.bag[MENU.bagChoice][MENU.choice].id).effect) {
                // Effects which are applied to a phone
                case IE_HEAL:
                case IE_REVIVE:
                case IE_UPGRADE:
                case IE_REPAIR: {
                    scrUseItemMenu(MENU.bagChoice, MENU.choice);
                    break;
                }

                default: {
                    pushTextbox("This item cannot be used (yet).", "");
                    break;
                }
            }
        }

        // Give (to a phone)
        case 1: {
        }

        // Toss (destroy)
        case 2: {

        }
    }
}