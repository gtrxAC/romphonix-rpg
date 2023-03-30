// _____________________________________________________________________________
//
//  Phone actions menu
//  Works much like the phone actions menu, but with different actions. The
//  phone actions and the item actions menus use the same draw function.
// _____________________________________________________________________________
//
#include "../common.h"
#include "itemactionsmenu.h"

void scrPhoneActionsMenu(Game *g);
void updatePhoneActionsMenu(Game *g);
void checkPhoneActionsMenu(Game *g);

// _____________________________________________________________________________
//
//  Phone actions menu - init function
// _____________________________________________________________________________
//
void scrPhoneActionsMenu(Game *g) {
    pushMenu(g, 0, NULL, true);
    MENU.drawFunc = drawItemActionsMenu;
    MENU.nextFunc = checkPhoneActionsMenu;
    MENU.canSkip = true;

    arrpush(MENU.choices, "Specs");
    arrpush(MENU.choices, "Move");
}

// _____________________________________________________________________________
//
//  Phone actions menu - check user input function
// _____________________________________________________________________________
//
void checkPhoneActionsMenu(Game *g) {
    if (MENU.choice == -1) popMenu(g);
}