// _____________________________________________________________________________
//
//  Phones menu
//  Shows a list of the phones the user has in their current party (which can be
//  used in battle).
//
//  The menu controls are handled by the scripting system, everything else like
//  drawing uses custom functions.
// _____________________________________________________________________________
//
#include "../common.h"

void scrPhonesMenu(Game *g);
void drawPhonesMenu(Game *g);

// _____________________________________________________________________________
//
//  Phones menu - init function
// _____________________________________________________________________________
//
void scrPhonesMenu(Game *g) {
    g->state = ST_SCRIPT;
    g->menuUpdateFunc = updatePhonesMenu;
    g->menuDrawFunc = drawPhonesMenu;
    g->menuScroll = 0;
}

// _____________________________________________________________________________
//
//  Phones menu - draw function
// _____________________________________________________________________________
//
void drawPhonesMenu(Game *g) {
}

// _____________________________________________________________________________
//
//  Phones menu - check user input function
// _____________________________________________________________________________
//
void updatePhonesMenu(Game *g) {
}