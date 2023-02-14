// _____________________________________________________________________________
//
//  Functionality related to the scripting system.
//
//  The scripting system consists of a few functions that allow creating
//  high-level game logic directly from the C language.
//
//  "script" is also a game state, much like "title" or "battle", it is used
//  when textboxes or menus are shown.
// _____________________________________________________________________________
//
#include "common.h"
// #include "world.h"

// _____________________________________________________________________________
//
//  Shows a text box with two lines.
// _____________________________________________________________________________
//
void textbox(Game *g, const char *line1, const char *line2) {
	g->state = ST_SCRIPT;
	g->scriptType = SC_TEXTBOX;
	g->textbox[0] = line1;
	g->textbox[1] = line2;
	g->textboxTime = g->frameCount;
}

// _____________________________________________________________________________
//
//  Shows a menu where the user can select from up to 8 choices.
// _____________________________________________________________________________
//
void menu(Game *g, int numChoices, const char **choices) {
	g->state = ST_SCRIPT;
	g->scriptType = SC_MENU;
	g->menuChoice = 0;
	g->numMenuChoices = numChoices;

	for (int i = 0; i < numChoices; i++) {
		g->menuChoices[i] = choices[i];
	}
}

// _____________________________________________________________________________
//
//  Ends the current script and gives back player control.
// _____________________________________________________________________________
//
void endScript(Game *g) {
	g->state = ST_WORLD;
}

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
//  Update "script" game state (textboxes and menus)
// _____________________________________________________________________________
//
void updateScript(Game *g) {
	if (!g->worldDrawn) {
		g->worldDrawn = true;
		// drawWorldRT(g);
	}

	switch (g->scriptType) {
		case SC_MENU:
			if (K_UP_PRESS() && g->menuChoice) {
				g->menuAnim = 0;
				g->menuAnimDir = DIR_UP;
				g->scriptType = SC_MENU_ANIM;
			}
			else if (K_DOWN_PRESS() && g->menuChoice < g->numMenuChoices - 1) {
				g->menuAnim = 0;
				g->menuAnimDir = DIR_DOWN;
				g->scriptType = SC_MENU_ANIM;
			}
			// fall through ("A" button to confirm choice)

		case SC_TEXTBOX:
			if (K_A_PRESS()) {
				if (g->nextFunc) g->nextFunc(g);
				else g->state = ST_WORLD;
			}
			break;

		case SC_MENU_ANIM:
			g->menuAnim += 2;
			if (g->menuAnim > 14) {
				if (g->menuAnimDir == DIR_UP) g->menuChoice--;
				else g->menuChoice++;
				g->scriptType = SC_MENU;
			}
			break;
	}
}

// _____________________________________________________________________________
//
//  Draw "script" game state (textboxes and menus)
// _____________________________________________________________________________
//
void drawScript(Game *g) {
	// note: drawWorld is run before this in the main loop, except in the main menu

	switch (g->scriptType) {
		case SC_TEXTBOX: {
			int lineCount = 1;
			if (strlen(g->textbox[1])) lineCount++;

            drawBox(g, 10, 224 - 14*lineCount, 300, 16 + 14*lineCount);

			DrawTextEx(
				g->fonts.dialogue, g->textbox[0],
				(Vector2) {18, 232 - 14*lineCount},
				14, 1, WHITE
			);
			if (lineCount == 2) DrawTextEx(
				g->fonts.dialogue, g->textbox[1],
				(Vector2) {18, 218},
				14, 1, WHITE
			);
			break;
		}

		case SC_MENU:
		case SC_MENU_ANIM: {
			// Get length of the longest menu choice
			int longest = 0;
			for (int i = 0; i < g->numMenuChoices; i++) {
				Vector2 length = MeasureTextEx(g->fonts.dialogue, g->menuChoices[i], 14, 1);
				if (length.x > longest) longest = length.x;
			}

            drawBox(g, 0, 0, 30 + longest, 16 + 14*g->numMenuChoices);

			for (int i = 0; i < g->numMenuChoices && g->menuChoices[i]; i++) {
				DrawTextEx(
					g->fonts.dialogue, g->menuChoices[i],
					(Vector2) {22, 8 + 14*i},
					14, 1, WHITE
				);
			}

			int selectorY = 8 + 14*g->menuChoice;

			if (g->scriptType == SC_MENU_ANIM) {
				if (g->menuAnimDir == DIR_UP) selectorY -= g->menuAnim;
				else selectorY += g->menuAnim;
			}
			DrawTexture(TEX(indicator), 8, selectorY, WHITE);
		}
	}
}