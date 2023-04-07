// _____________________________________________________________________________
//
//  Phone specs menu
// _____________________________________________________________________________
//
#include "../common.h"

void scrPhoneSpecsMenu(Game *g, Phone *phone);
void updatePhoneSpecsMenu(Game *g);
void drawPhoneSpecsMenu(Game *g);

// _____________________________________________________________________________
//
//  Phone specs menu - init function
//  The init function takes a second argument, the phone whose specs to show.
// _____________________________________________________________________________
//
void scrPhoneSpecsMenu(Game *g, Phone *phone) {
    pushMenu(g, 0, NULL, CB_CLOSE);
    MENU.updateFunc = updatePhoneSpecsMenu;
    MENU.drawFunc = drawPhoneSpecsMenu;
	MENU.phone = phone;

	for (int i = 0; i < 4; i++) {
		arrpush(MENU.choices, g->skillDB->skills[MENU.phone->skills[i]].name);
	}
}

// _____________________________________________________________________________
//
//  Phone specs menu - update function
// _____________________________________________________________________________
//
void updatePhoneSpecsMenu(Game *g) {
    if (K_UP_PRESS() && MENU.choice) MENU.choice--;

    else if (K_DOWN_PRESS() && MENU.choice < arrlen(MENU.choices) - 1) {
        MENU.choice++;
    }

    if (K_B_PRESS()) popMenu(g);

    if (K_A_PRESS()) {
		scrCollectionMenu(g);
		MENU.choice = LASTMENU.phone->id;
		MENU.scroll = LASTMENU.phone->id;
    }
}

// _____________________________________________________________________________
//
//  Phone specs menu - draw function
// _____________________________________________________________________________
//
void drawPhoneSpecsMenu(Game *g) {
    #define PHONE (MENU.phone)
    #define PHONESPECS (SPECS(PHONE->id))

	// Sprite window
    drawBoxL(g, 0, 0, 72, 72);
    DrawTexture(shget(g->textures, PHONESPECS.sprite), 4, 4, WHITE);

	// Top right box (name, battery, level, exp)
	drawBox(g, 72, 0, 248, 72);
    drawText(g, TextFormat("%s %s", PHONESPECS.brand, PHONESPECS.model), 80, 10, WHITE);
    drawTextD(g, TextFormat("$ %d", PHONE->level), 285, 9, WHITE);
    drawText(g, TextFormat("Battery: %d%%", (int) ((float) PHONE->hp/PHONE->maxHP)*100), 80, 29, WHITE);
    drawText(g, TextFormat("EXP: %d/%d", PHONE->exp, PHONE->maxExp), 80, 48, WHITE);

	// Middle box (stats and status)
	drawBox(g, 0, 72, 320, 80);
    drawText(g, TextFormat("HP: %d", PHONE->maxHP), 6, 82, WHITE);
    drawText(g, TextFormat("AT: %d", PHONE->attack), 6, 98, WHITE);
    drawText(g, TextFormat("DF: %d", PHONE->defense), 6, 114, WHITE);
    drawText(g, TextFormat("WT: %d", PHONE->weight), 6, 130, WHITE);

	if (PHONE->batteryStatus != COND_OK) {
		drawText(
			g, TextFormat("x %.1f", condMultipliers[PHONE->batteryStatus]),
			65, 82, (condMultipliers[PHONE->batteryStatus]) > 1.0f ? GREEN : RED
		);
	}
	drawText(
		g, TextFormat("Battery Status:  %s", condToString[PHONE->batteryStatus]),
		120, 82, WHITE
	);

	if (PHONE->screenStatus != COND_OK) {
		drawText(
			g, TextFormat("x %.1f", condMultipliers[PHONE->screenStatus]),
			65, 98, (condMultipliers[PHONE->screenStatus]) > 1.0f ? GREEN : RED
		);
	}
	drawText(
		g, TextFormat("Screen Status:  %s", condToString[PHONE->screenStatus]),
		120, 98, WHITE
	);

	if (PHONE->boardStatus != COND_OK) {
		drawText(
			g, TextFormat("x %.1f", condMultipliers[PHONE->boardStatus]),
			65, 114, (condMultipliers[PHONE->boardStatus]) > 1.0f ? GREEN : RED
		);
	}
	drawText(
		g, TextFormat("Board Status:  %s", condToString[PHONE->boardStatus]),
		120, 114, WHITE
	);

	if (PHONE->coverStatus != COND_OK) {
		drawText(
			g, TextFormat("x %.1f", weightCondMultipliers[PHONE->coverStatus]),
			65, 130, (weightCondMultipliers[PHONE->coverStatus]) > 1.0f ? RED : GREEN
		);
	}
	drawText(
		g, TextFormat("Cover Status:  %s", condToString[PHONE->coverStatus]),
		120, 130, WHITE
	);

	// Bottom left box (skill list)
	// This list can be scrolled through and the skill's info shows on the right
	drawBox(g, 0, 152, 160, 88);
	for (int i = 0; i < arrlen(MENU.choices); i++) {
		drawText(g, MENU.choices[i], 22, 162 + 18*i, WHITE);
	}
    DrawTexture(TEX(indicator), 7, 162 + 18*MENU.choice, WHITE);

	// Bottom right box (skill info)
	#define SKILL (g->skillDB->skills[PHONE->skills[MENU.choice]])
	drawBox(g, 160, 152, 160, 88);
	drawTextRec(g, SKILL.description, 164, 156, 152, 80, WHITE);

	switch (SKILL.effects[0].effect) {
		case SE_DAMAGE:
		case SE_DRAIN:
			drawText(g, TextFormat("Damage: %d", SKILL.effects[0].parameter), 165, 205, WHITE);
			break;

		case SE_CONFUSE:
			drawText(g, TextFormat("Chance: %d", SKILL.effects[0].parameter), 165, 205, WHITE);
			break;
	}
	drawText(g, TextFormat("Type: %s", skillTypes[SKILL.type]), 165, 220, WHITE);
}