// _____________________________________________________________________________
//
//  Functionality related to the scripting system.
//
//  See also: menu.c
// _____________________________________________________________________________
//
#include "common.h"

void updateTextbox();
void drawTextbox();

// _____________________________________________________________________________
//
//  Shows a text box with two lines.
// _____________________________________________________________________________
//
void pushTextbox(const char *line1, const char *line2) {
	arrput(g.menus, (Menu) {0});
	setUpdateFunc(updateTextbox);
	setDrawFunc(drawTextbox);

	strcpy(MENU.textbox[0], line1);
	strcpy(MENU.textbox[1], line2);
}

// _____________________________________________________________________________
//
//  Removes the last menu or text box from the menu stack.
// _____________________________________________________________________________
//
void popMenu() {
	for (int i = 0; i < arrlen(MENU.choices); i++) {
		free(MENU.choices[i]);
	}
	arrfree(MENU.choices);
	arrpop(g.menus);
}

// _____________________________________________________________________________
//
//  Starts a transition where the player goes to another map.
// _____________________________________________________________________________
//
void changeMap(int map, int x, int y) {
	g.nextX = x;
	g.nextY = y;
	g.nextMap = map;
	g.worldDrawn = false;

	// reset the frame counter so we don't have to keep track of when the transition started
	g.frameCount = 1;
	g.state = ST_TRANSITION;
}

// _____________________________________________________________________________
//
//  Draws a random phone from the phone pool (lower rarity phones are drawn more
//  often).
// _____________________________________________________________________________
//
int randomPhone() {
	int *pool = NULL;  // stb_ds dynarray
	const int phonesPerRarity[6] = {24, 16, 8, 4, 2, 1};

	for (int i = 0; i < g.phoneDB->size; i++) {
		for (int n = 0; n < phonesPerRarity[SPECS(i).rarity]; n++) {
			arrpush(pool, i);
		}
	}

	int result = pool[GetRandomValue(0, arrlen(pool) - 1)];
	arrfree(pool);
	return result;
}

// _____________________________________________________________________________
//
//  Update textbox menu
// _____________________________________________________________________________
//
void updateTextbox() {
	if (K_A_PRESS()) {
		if (MENU.textboxTime < strlen(MENU.textbox[0]) + strlen(MENU.textbox[1])) {
			MENU.textboxTime = 128;
		} else {
			void (*func)() = MENU.nextFunc;
			popMenu();
			if (func) func();
			else g.state = ST_INGAME;
		}
	}
}

// _____________________________________________________________________________
//
//  Draw textbox menu
// _____________________________________________________________________________
//
void drawTextbox() {
	int lineCount = 1;
	if (strlen(MENU.textbox[1])) lineCount++;

	drawBox(10, 224 - 14*lineCount, 300, 16 + 14*lineCount);

	// The text uses a typewriter animation - so at the beginning only
	// part of the text is drawn, we use separate text buffers and a
	// timer to manage this.
	int line1Len = MIN(MENU.textboxTime, 63);
	int line2Len = MIN(MAX(MENU.textboxTime - strlen(MENU.textbox[0]), 0), 63);

	char textboxDraw[2][64] = {0};

	strncpy(textboxDraw[0], MENU.textbox[0], line1Len);
	drawText(textboxDraw[0], 18, 232 - 14*lineCount, WHITE);

	if (lineCount > 1) {
		strncpy(textboxDraw[1], MENU.textbox[1], line2Len);
		drawText(textboxDraw[1], 18, 218, WHITE);
	}

	MENU.textboxTime++;
}

void scrNoScript() {
	pushTextbox("No function assigned to this script index!", "");
}