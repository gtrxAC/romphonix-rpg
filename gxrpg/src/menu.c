#include "common.h"
#include "script.h"

// Built-in menus (main menu, in-game menu, saving/loading)
// textbox, menu, endscript, etc. are in script.c

bool save(Game *game) {
	if (!SaveStorageValue(0, 1)) return false;
	SaveStorageValue(1, game->curmap);
	SaveStorageValue(2, game->playerx);
	SaveStorageValue(3, game->playery);
	SaveStorageValue(4, game->playerdir);
	return true;
}

bool load(Game *game) {
	if (LoadStorageValue(0) != 1) return false;
	game->curmap = LoadStorageValue(1);
	game->playerx = LoadStorageValue(2);
	game->playery = LoadStorageValue(3);
	game->playerdir = LoadStorageValue(4);
	game->worlddrawn = false;

	if (game->playerx < 0 || game->playery < 0 || game->playerdir > DIR_RIGHT) return false;
	return true;
}

void mainmenu2(Game *game);

void mainmenu(Game *game) {
	const char *choices[] = {"Load Game", "New Game"};
	menu(game, 2, choices);
	game->state = ST_MAINMENU;
	game->nextfunc = mainmenu2;
}

void mainmenu2(Game *game) {
	enum {LOAD, NEW};

	switch (game->menuchoice) {
		case LOAD:
			if (!FileExists("storage.data")) {
				textbox(game, "Save file not found.", "");
				game->state = ST_MAINMENU;
				game->nextfunc = mainmenu;
				break;
			}
			if (!load(game)) {
				textbox(game, "Invalid save file.", "");
				game->state = ST_MAINMENU;
				game->nextfunc = mainmenu;
				break;
			}
			endscript(game);
			break;

		case NEW:
			game->curmap = STARTING_MAP;
			game->playerx = STARTING_X;
			game->playery = STARTING_Y;
			endscript(game);
			break;
	}
}

void ingamemenu2(Game *game);

void ingamemenu(Game *game) {
	const char *choices[] = {"Save", "Exit"};
	menu(game, 2, choices);
	game->nextfunc = ingamemenu2;
}

void ingamemenu2(Game *game) {
	enum {SAVE, EXIT};

	switch (game->menuchoice) {
		case SAVE:
			if (save(game)) textbox(game, "Saved!", "");
			else textbox(game, "Failed to save.", "");
			game->nextfunc = endscript;
			break;

		case EXIT:
			endscript(game);
			break;
	}
}