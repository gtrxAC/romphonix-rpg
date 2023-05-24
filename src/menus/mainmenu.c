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
            // TODO: introduction thing
            char *defaultName = getenv("USER");  // Linux
            if (!defaultName) defaultName = getenv("USERNAME");  // Windows
            if (!defaultName) defaultName = " ";
            char *name;

            nameEntry:
                name = tinyfd_inputBox("Name", "What is your name?", defaultName);
                if (!name) return;

                if (strlen(name) > 15) {
                    tinyfd_messageBox("Error", "Name must be 1-15 characters!", "ok", "error", 0);
                    goto nameEntry;
                }

            strcpy(g.s.name, name);

            time_t t = time(NULL);
            struct tm *tm = localtime(&t);
            strftime(g.s.startDate, sizeof(g.s.startDate), "%Y-%m-%d", tm);

            g.s.id = GetRandomValue(10000, 99999);

            g.s.curMap = 0;
            loadMap(0);
            g.state = ST_INGAME;
            setSong(g.mapMeta.songName);
            popMenu();

            scrIntro();
            break;
        }

        case 2:
            scrSettingsMenu();
            break;
    }
}