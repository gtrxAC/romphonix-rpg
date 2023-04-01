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
    arrpush(MENU.choices, "Move up");
    arrpush(MENU.choices, "Move down");
}

// _____________________________________________________________________________
//
//  Phone actions menu - check user input function
// _____________________________________________________________________________
//
void checkPhoneActionsMenu(Game *g) {
    if (MENU.choice == -1) popMenu(g);
    switch (MENU.choice) {
        case -1: popMenu(g); break;

        case 1: {
            // If not already the topmost choice, swap the places of this phone
            // and the one above it. Also switch the menu choice to keep
            // pointing at the same phone, so it can easily be moved again.
            if (LASTMENU.choice != 0) {
                Phone this = g->s.party[LASTMENU.choice];
                Phone other = g->s.party[LASTMENU.choice - 1];
                g->s.party[LASTMENU.choice - 1] = this;
                g->s.party[LASTMENU.choice] = other;
                LASTMENU.choice--;
            }
            break;
        }

        case 2: {
            // Same as above, but other way around.
            // First we need to count the amount of phones in the party to make
            // sure we have space to move to (no blank spots in between).
            int phoneCount = 0;
            for (int i = 0; i < 6; i++) {
                if (g->s.party[i].active) phoneCount++;
            }

            if (LASTMENU.choice < phoneCount - 1) {
                Phone this = g->s.party[LASTMENU.choice];
                Phone other = g->s.party[LASTMENU.choice + 1];
                g->s.party[LASTMENU.choice + 1] = this;
                g->s.party[LASTMENU.choice] = other;
                LASTMENU.choice++;
            }
            break;
        }
    }
}