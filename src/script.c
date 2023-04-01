// _____________________________________________________________________________
//
//  Functionality related to the scripting system.
//
//  See also: menu.c
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
void pushTextbox(Game *g, const char *line1, const char *line2) {
	arrput(g->menus, (Menu) {0});
	MENU.updateFunc = updateTextbox;
	MENU.drawFunc = drawTextbox;

	// arrput(MENU.choices, line1);
	// arrput(MENU.choices, line2);
	MENU.textbox[0] = line1;
	MENU.textbox[1] = line2;
}

// _____________________________________________________________________________
//
//  Removes the last menu or text box from the menu stack.
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
	g->nextMap = map;
	g->worldDrawn = false;

	// reset the frame counter so we don't have to keep track of when the transition started
	g->frameCount = 1;
	g->state = ST_TRANSITION;
}

// _____________________________________________________________________________
//
//  Update textbox menu
// _____________________________________________________________________________
//
void updateTextbox(Game *g) {
	if (K_A_PRESS()) {
		if (MENU.nextFunc) MENU.nextFunc(g);
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
	if (strlen(MENU.textbox[1])) lineCount++;

	drawBox(g, 10, 224 - 14*lineCount, 300, 16 + 14*lineCount);

	// The text uses a typewriter animation - so at the beginning only
	// part of the text is drawn, we use separate text buffers and a
	// timer to manage this.
	int line1Len = MIN(MENU.textboxTime, 63);
	int line2Len = MIN(MENU.textboxTime - strlen(MENU.textbox[0]), 63);
	if (line2Len == 63 && line1Len < 63) line2Len = 0;  // quick and hacky bugfix
	
	strncpy(MENU.textboxDraw[0], MENU.textbox[0], line1Len);
	strncpy(MENU.textboxDraw[1], MENU.textbox[1], line2Len);
	MENU.textboxDraw[0][line1Len] = 0;
	MENU.textboxDraw[1][line2Len] = 0;

	DrawTextEx(
		g->fonts.dialogue, MENU.textboxDraw[0],
		(Vector2) {18, 232 - 14*lineCount},
		13, 0, WHITE
	);
	if (lineCount == 2) DrawTextEx(
		g->fonts.dialogue, MENU.textboxDraw[1],
		(Vector2) {18, 218},
		13, 0, WHITE
	);

	MENU.textboxTime++;
}

void scrNoScript(Game *g) {
	pushTextbox(g, "No function assigned to this script index!", "");
	MENU.nextFunc = popMenu;
}