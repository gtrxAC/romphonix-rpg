// _____________________________________________________________________________
//
//  Text box menu
//  This menu is simply a combination of a standard text box (script.c) and a
//  selection menu (menu.c).
// _____________________________________________________________________________
//
#include "../common.h"

void scrTextBoxMenu(Game *g);
void drawTextBoxMenu(Game *g);
void updateMenu(Game *g);

// _____________________________________________________________________________
//
//  Text box menu - init function
// _____________________________________________________________________________
//
void scrTextBoxMenu(Game *g) {
    pushMenu(g, 0, NULL, CB_CLOSE);
    MENU.updateFunc = updateMenu;
    MENU.drawFunc = drawTextBoxMenu;
}

// _____________________________________________________________________________
//
//  Text box menu - draw function
// _____________________________________________________________________________
//
void drawTextBoxMenu(Game *g) {
    drawTextbox(g);
    drawMenu(g);
}