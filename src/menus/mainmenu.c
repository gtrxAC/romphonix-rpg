// _____________________________________________________________________________
//
//  Main menu logic (made up of script functions)
// _____________________________________________________________________________
//
#include "../common.h"

// Functions have to be declared before referencing them

// Note: there is a compiler flag to allow *calling* a function before declaring
// it, but this doesn't apply to *referencing* it (like in the MENU.nextFunc = )
void scrMainMenuCheck(Game *g);

// _____________________________________________________________________________
//
void scrMainMenu(Game *g) {
    setSong(g, "assets/sounds/music/bid_right_now_p.mid");

    if (FileExists("save.tfs")) {
        const char *choices[] = {"Load Game", "New Game", "Options"};
	    pushMenu(g, 3, choices, false);
    }
    else {
        const char *choices[] = {"New Game", "Options"};
	    pushMenu(g, 2, choices, false);
    }

	MENU.nextFunc = scrMainMenuCheck;
}

void scrMainMenuCheck(Game *g) {
    int choice = MENU.choice;
    if (arrlen(MENU.choices) == 2) choice++;

    switch (choice) {
        case 0:
            load(g);
            g->state = ST_INGAME;
            popMenu(g);
            break;

        case 1: {
            // TODO: introduction thing
            char *defaultName = getenv("USER");
            if (!defaultName) defaultName = " ";
            char *name;

            nameEntry:
                name = tinyfd_inputBox("Name", "What is your name?", defaultName);
                if (!name || strlen(name) > 15) {
                    tinyfd_messageBox("Error", "Name must be 1-15 characters!", "ok", "error", 0);
                    goto nameEntry;
                }

            strcpy(g->s.name, name);

            time_t t = time(NULL);
            struct tm *tm = localtime(&t);
            strftime(g->s.startDate, sizeof(g->s.startDate), "%Y-%m-%d", tm);

            g->s.id = GetRandomValue(10000, 99999);

            g->state = ST_INGAME;
            popMenu(g);
            break;
        }

        case 2:
            // TODO: settings thing
            setSong(g, "assets/sounds/music/abangchung.mid");
            break;
    }
}