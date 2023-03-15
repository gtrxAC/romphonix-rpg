// _____________________________________________________________________________
//
//  Main menu logic (made up of script functions)
// _____________________________________________________________________________
//
#include "../common.h"

// Functions have to be declared before referencing them

// Note: there is a compiler flag to allow *calling* a function before declaring
// it, but this doesn't apply to *referencing* it (like in the g->nextFunc = )
void scrMainMenuCheck();

// _____________________________________________________________________________
//
void scrMainMenu(Game *g) {
    setSong(g, "assets/sounds/music/bid_right_now_p.mid");

    if (FileExists("save.tfs")) {
        const char *choices[] = {"Load Game", "New Game", "Options"};
	    menu(g, 3, choices, false);
    }
    else {
        const char *choices[] = {"New Game", "Options"};
	    menu(g, 2, choices, false);
    }

	g->nextFunc = scrMainMenuCheck;
}

void scrMainMenuCheck(Game *g) {
    int choice = g->menuChoice;
    if (g->numMenuChoices == 2) choice++;

    switch (choice) {
        case 0:
            load(g);
            g->state = ST_WORLD;
            break;

        case 1:
            // TODO: introduction thing
            g->state = ST_WORLD;
            break;

        case 2:
            // TODO: settings thing
            setSong(g, "assets/sounds/music/abangchung.mid");
            break;
    }
}