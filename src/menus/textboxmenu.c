// _____________________________________________________________________________
//
//  Text box menu
//  This menu is simply a combination of a standard text box (script.c) and a
//  selection menu (menu.c).
// _____________________________________________________________________________
//
#include "../common.h"

// _____________________________________________________________________________
//
//  Text box menu - init function
// _____________________________________________________________________________
//
void pushTextboxMenu() {
    pushMenu(CB_CONTINUE);
    setUpdateFunc(updateMenu);
    setDrawFunc(drawTextBoxMenu);
}

// _____________________________________________________________________________
//
//  Text box menu - draw function
// _____________________________________________________________________________
//
void drawTextBoxMenu() {
    drawTextbox();
    drawMenu();
}