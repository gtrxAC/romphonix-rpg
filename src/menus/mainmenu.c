// _____________________________________________________________________________
//
//  Main menu logic (made up of script functions)
// _____________________________________________________________________________
//
#include "../common.h"

// Functions have to be declared before referencing them

// Note: there is a compiler flag to allow *calling* a function before declaring
// it, but this doesn't apply to *referencing* it (like in the MENU.nextFunc = )
void scrMainMenuCheck();

// _____________________________________________________________________________
//
void scrMainMenu() {
    setSong("assets/sounds/music/bid_right_now_p.mid");

    if (FileExists("save.tfs")) {
        const char *choices[] = {"Load Game", "New Game", "Options"};
	    pushMenu(3, choices, CB_NOTHING);
    }
    else {
        const char *choices[] = {"New Game", "Options"};
	    pushMenu(2, choices, CB_NOTHING);
    }

	MENU.nextFunc = scrMainMenuCheck;
}

void scrMainMenuCheck() {
    int choice = MENU.choice;
    if (arrlen(MENU.choices) == 2) choice++;

    switch (choice) {
        case 0:
            load();
            g.state = ST_INGAME;
            setSong(g.mapMeta.songName);
            popMenu();
            break;

        case 1: {
            popMenu();
            scrIntroFade();
            break;
        }

        case 2:
            scrSettingsMenu();
            break;
    }
}