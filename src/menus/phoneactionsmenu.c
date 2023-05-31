// _____________________________________________________________________________
//
//  Phone actions menu
//  Works much like the item actions menu, but with different actions. The
//  phone actions and the item actions menus use the same draw function.
// _____________________________________________________________________________
//
#include "../common.h"

// _____________________________________________________________________________
//
//  Phone actions menu - init function
// _____________________________________________________________________________
//
void scrPhoneActionsMenu() {
    pushMenu(CB_CLOSE);
    setDrawFunc(drawItemActionsMenu);
    setNextFunc(checkPhoneActionsMenu);

    addChoice("Specs");
    addChoice("Move up");
    addChoice("Move down");
}

// _____________________________________________________________________________
//
//  Phone actions menu - check user input function
// _____________________________________________________________________________
//
void checkPhoneActionsMenu() {
    switch (MENU.choice) {
        case -1: popMenu(); break;

        case 0: {
            // The phone specs menu init function takes a second argument,
            // the phone whose specs to show.
            scrPhoneSpecsMenu(&g.s.party[LASTMENU.choice]);
            break;
        }

        case 1: {
            // If not already the topmost choice, swap the places of this phone
            // and the one above it. Also switch the menu choice to keep
            // pointing at the same phone, so it can easily be moved again.
            if (LASTMENU.choice != 0) {
                Phone this = g.s.party[LASTMENU.choice];
                Phone other = g.s.party[LASTMENU.choice - 1];
                g.s.party[LASTMENU.choice - 1] = this;
                g.s.party[LASTMENU.choice] = other;
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
                if (g.s.party[i].active) phoneCount++;
            }

            if (LASTMENU.choice < phoneCount - 1) {
                Phone this = g.s.party[LASTMENU.choice];
                Phone other = g.s.party[LASTMENU.choice + 1];
                g.s.party[LASTMENU.choice + 1] = this;
                g.s.party[LASTMENU.choice] = other;
                LASTMENU.choice++;
            }
            break;
        }
    }
}