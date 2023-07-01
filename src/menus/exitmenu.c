// _____________________________________________________________________________
//
//  Exit menu
//  This menu asks the user what they want to do when the window close button is
//  pressed. It is a text box menu with three options - continue playing, save
//  and close, and close without saving.
// _____________________________________________________________________________
//
#include "../common.h"

// _____________________________________________________________________________
//
//  Text box menu - init function
// _____________________________________________________________________________
//
void scrExitMenu() {
    pushTextboxMenu();
    MENU.drawPrevious = true;
    strcpy(MENU.textbox[0], "What do you want to do?");
    strcpy(MENU.textbox[1], "");
    addChoice("Continue");
    addChoice("Save and Close");
    addChoice("Close without saving");
    setNextFunc(checkExitMenu);
}

void drawExitMenu() {
    // Darken the previous menu by drawing a black checkerboard over it
    for (int y = 0; y < 240; y += 80) {
        for (int x = 0; x < 320; x += 80) {
            drawTexture("checkerboard", x, y, BLACK);
            drawTexture("checkerboard", x + 1, y, ColorAlpha(BLACK, 0.5f));
        }
    }

    // Standard behavior of text box menu
    drawTextbox();
    drawMenu();
}

// _____________________________________________________________________________
//
//  Text box menu - check user input function
// _____________________________________________________________________________
//
void checkExitMenu() {
    switch (MENU.choice) {
        case -1:
        case 0:
            popMenu();
            break;

        case 1:
            save();
            // fall through

        case 2:
            // always save the settings anyway
            saveSettings();
            g.shouldClose = true;
            break;
    }
}