// _____________________________________________________________________________
//
//  Main menu logic (made up of script functions)
// _____________________________________________________________________________
//
#include "common.h"

// Functions have to be declared before referencing them

// Note: there is a compiler flag to allow *calling* a function before declaring
// it, but this doesn't apply to *referencing* it (like in the g->nextFunc = )
void scrMainMenuCheck();
void scrMainMenuDemo();

// _____________________________________________________________________________
//
void scrMainMenu(Game *g) {
    setSong(g, "assets/sounds/music/bid_right_now_p.mid");

	const char *choices[] = {"Change Song", "Change song 2", "Textbox demo"};
	menu(g, 3, choices);
	g->nextFunc = scrMainMenuCheck;
}

void scrMainMenuCheck(Game *g) {
    switch (g->menuChoice) {
        case 0:
            // note: meme song, will not be included in final game
            setSong(g, "assets/sounds/music/abangchung.mid");
            break;

        case 1:
            setSong(g, "assets/sounds/music/bid_right_now_p.mid");
            break;

        case 2:
	        g->nextFunc = scrMainMenuDemo;
            break;
    }
}

void scrMainMenuDemo(Game *g) {
    textbox(g, "This is an example of text boxes", "Lorem ipsum dolor sit amet...");
	g->nextFunc = scrMainMenu;
}