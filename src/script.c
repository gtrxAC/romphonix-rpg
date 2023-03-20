// _____________________________________________________________________________
//
//  Functionality related to the scripting system.
//
//  The scripting system consists of a few functions that allow creating
//  high-level game logic directly from the C language.
//
//  "script" is also a game state, much like "title" or "battle", it is used
//  when textboxes or menus are shown.
//
//  Note that when beginning a script, the game state should be set to
//  ST_TEXTBOX or another script-like state for a specific menu (e.g. main menu).
// _____________________________________________________________________________
//
#include "common.h"

void updateTextbox(Game *g);
void drawTextbox(Game *g);

// _____________________________________________________________________________
//
//  Shows a text box with two lines.
// _____________________________________________________________________________
//
void textbox(Game *g, const char *line1, const char *line2) {
	// g->state = ST_TEXTBOX;
	// g->scriptType = SC_TEXTBOX;
	g->textbox[0] = line1;
	g->textbox[1] = line2;
	g->textboxTime = 0;
	MENU.updateFunc = updateTextbox;
	MENU.drawFunc = drawTextbox;
}

// _____________________________________________________________________________
//
//  Shows a menu where the user can select from a list of choices.
//  The menu is pushed to the top of a menu stack, where previous menus are
//  stored and can be returned back to.
//
//  If canSkip is set to true, the player can close the menu by pressing the B
//  button (see keybindings in common.h). In this case, menuChoice is set to -1.
//  canSkip only applies if updateMenu is used as the update function.
// _____________________________________________________________________________
//
void pushMenu(Game *g, int numChoices, const char **choices, bool canSkip) {
	// g->state = ST_TEXTBOX;
	// g->state = ST_MENU;
	arrput(g->menus, (Menu) {0});
	MENU.choice = 0;
	MENU.canSkip = canSkip;
	MENU.updateFunc = updateMenu();
	MENU.drawFunc = drawMenu();

	for (int i = 0; i < numChoices; i++) {
		arrput(MENU.choices, choices[i]);
	}
}

void pushTextbox(Game *g, )

// _____________________________________________________________________________
//
//  Removes the last menu from the menu stack.
// _____________________________________________________________________________
//
void popMenu(Game *g) {
	arrfree(MENU.choices);
	arrpop(g->menus);
}

// _____________________________________________________________________________
//
//  Ends the current script and gives back player control.
// _____________________________________________________________________________
//
// void endScript(Game *g) {
// 	g->state = ST_WORLD;
// }

// _____________________________________________________________________________
//
//  Starts a transition where the player goes to another map.
// _____________________________________________________________________________
//
void changeMap(Game *g, int map, int x, int y) {
	g->nextX = x;
	g->nextY = y;
	// g->nextMap = map;
	g->worldDrawn = false;

	// reset the frame counter so we don't have to keep track of when the transition started
	// g->frameCount = 1;
	// g->state = ST_TRANSITION;

	loadMap(g, map);
}

// _____________________________________________________________________________
//
//  Update textbox menu
// _____________________________________________________________________________
//
void updateTextbox(Game *g) {
	if (K_A_PRESS()) {
		if (g->nextFunc) g->nextFunc(g);
		else g->state = ST_INGAME;
	}
}

// _____________________________________________________________________________
//
//  Draw textbox menu
// _____________________________________________________________________________
//
void drawTextbox(Game *g) {
	int lineCount = 1;
	if (strlen(g->textbox[1])) lineCount++;

	drawBox(g, 10, 224 - 14*lineCount, 300, 16 + 14*lineCount);

	// The text uses a typewriter animation - so at the beginning only
	// part of the text is drawn, we use separate text buffers and a
	// timer to manage this.
	int line1Len = MIN(g->textboxTime, 63);
	int line2Len = MIN(g->textboxTime - strlen(g->textbox[0]), 63);
	if (line2Len == 63 && line1Len < 63) line2Len = 0;  // quick and hacky bugfix
	
	strncpy(g->textboxDraw[0], g->textbox[0], line1Len);
	strncpy(g->textboxDraw[1], g->textbox[1], line2Len);
	g->textboxDraw[0][line1Len] = 0;
	g->textboxDraw[1][line2Len] = 0;

	DrawTextEx(
		g->fonts.dialogue, g->textboxDraw[0],
		(Vector2) {18, 232 - 14*lineCount},
		13, 0, WHITE
	);
	if (lineCount == 2) DrawTextEx(
		g->fonts.dialogue, g->textboxDraw[1],
		(Vector2) {18, 218},
		13, 0, WHITE
	);

	g->textboxTime++;
}

void scrNoScript(Game *g) {
	textbox(g, "No function assigned to this script index!", "");
	g->nextFunc = endScript;
}