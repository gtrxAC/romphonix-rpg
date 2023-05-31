// _____________________________________________________________________________
//
//  Main menu logic (made up of script functions)
// _____________________________________________________________________________
//
#include "../common.h"

// _____________________________________________________________________________
//
void scrMainMenu() {
    setSong("assets/sounds/music/bid_right_now_p.mid");

    pushMenu(CB_NOTHING);
    if (FileExists("save.tfs")) addChoice("Load Game");
    addChoice("New Game");
    addChoice("Options");
	setNextFunc(scrMainMenuCheck);
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