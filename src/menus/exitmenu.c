// _____________________________________________________________________________
//
//  Exit menu
//  This menu asks the user what they want to do when the window close button is
//  pressed. It is a text box menu with three options - continue playing, save
//  and close, and close without saving.
// _____________________________________________________________________________
//
#include "../common.h"

void scrExitMenu(Game *g);
void checkExitMenu(Game *g);

// _____________________________________________________________________________
//
//  Text box menu - init function
// _____________________________________________________________________________
//
void scrExitMenu(Game *g) {
    scrTextBoxMenu(g);
    MENU.textbox[0] = "What do you want to do?";
    MENU.textbox[1] = "";
    arrpush(MENU.choices, "Continue");
    arrpush(MENU.choices, "Save and Close");
    arrpush(MENU.choices, "Close without saving");
    MENU.nextFunc = checkExitMenu;
}

// _____________________________________________________________________________
//
//  Text box menu - check user input function
// _____________________________________________________________________________
//
void checkExitMenu(Game *g) {
    switch (MENU.choice) {
        case -1:
        case 0:
            popMenu(g);
            break;

        case 1:
            save(g);
            // fall through

        case 2:
            // always save the settings anyway
            saveSettings(g);
            g->shouldClose = true;
            break;
    }
}
