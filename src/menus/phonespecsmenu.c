// _____________________________________________________________________________
//
//  Phone specs menu
// _____________________________________________________________________________
//
#include "../common.h"

// _____________________________________________________________________________
//
//  Phone specs menu - init function
//  The init function takes a second argument, the phone whose specs to show.
// _____________________________________________________________________________
//
void scrPhoneSpecsMenu(Phone *phone) {
    pushMenu(CB_CLOSE);
    setUpdateFunc(updatePhoneSpecsMenu);
    setDrawFunc(drawPhoneSpecsMenu);
	MENU.phone = phone;

	for (int i = 0; i < 4; i++) {
		addChoice(g.skillDB->skills[MENU.phone->skills[i]].name);
	}
}

// _____________________________________________________________________________
//
//  Phone specs menu - update function
// _____________________________________________________________________________
//
void updatePhoneSpecsMenu() {
    if (K_UP_PRESS() && MENU.choice) MENU.choice--;

    else if (K_DOWN_PRESS() && MENU.choice < arrlen(MENU.choices) - 1) {
        MENU.choice++;
    }

    if (K_B_PRESS()) popMenu();

    if (K_A_PRESS()) {
		scrCollectionMenu();
		MENU.choice = LASTMENU.phone->id;
		MENU.scroll = LASTMENU.phone->id;
    }
}

// _____________________________________________________________________________
//
//  Phone specs menu - draw function
// _____________________________________________________________________________
//
void drawPhoneSpecsMenu() {
    #define PHONE (MENU.phone)
    #define PHONESPECS (SPECS(PHONE->id))

	// Sprite window
    drawBoxL(0, 0, 72, 72);
    drawTexture(PHONESPECS.sprite, 4, 4, WHITE);

	// Top right box (name, battery, level, exp)
	drawBox(72, 0, 248, 72);
    drawTextL(F("%s %s", PHONESPECS.brand, PHONESPECS.model), 80, 11, WHITE);
    drawTextD(F("$ %d", PHONE->level), 285, 8, WHITE);
    drawText(F("Battery: %d%%", (int) ((float) PHONE->hp/PHONE->maxHP)*100), 80, 37, WHITE);
    drawText(F("EXP: %d/%d", PHONE->exp, PHONE->maxExp), 80, 52, WHITE);

	// Middle box (stats and status)
	drawBox(0, 72, 320, 80);
    drawText(F("HP: %d", PHONE->maxHP), 6, 82, WHITE);
    drawText(F("AT: %d", PHONE->attack), 6, 98, WHITE);
    drawText(F("DF: %d", PHONE->defense), 6, 114, WHITE);
    drawText(F("WT: %d", PHONE->weight), 6, 130, WHITE);

	if (PHONE->batteryStatus != COND_OK) {
		drawText(
			F("x %.1f", condMultipliers[PHONE->batteryStatus]),
			65, 82, (condMultipliers[PHONE->batteryStatus]) > 1.0f ? GREEN : RED
		);
	}
	drawText(
		F("Battery Status:  %s", condToString[PHONE->batteryStatus]),
		120, 82, WHITE
	);

	if (PHONE->screenStatus != COND_OK) {
		drawText(
			F("x %.1f", condMultipliers[PHONE->screenStatus]),
			65, 98, (condMultipliers[PHONE->screenStatus]) > 1.0f ? GREEN : RED
		);
	}
	drawText(
		F("Screen Status:  %s", condToString[PHONE->screenStatus]),
		120, 98, WHITE
	);

	if (PHONE->boardStatus != COND_OK) {
		drawText(
			F("x %.1f", condMultipliers[PHONE->boardStatus]),
			65, 114, (condMultipliers[PHONE->boardStatus]) > 1.0f ? GREEN : RED
		);
	}
	drawText(
		F("Board Status:  %s", condToString[PHONE->boardStatus]),
		120, 114, WHITE
	);

	if (PHONE->coverStatus != COND_OK) {
		drawText(
			F("x %.1f", weightCondMultipliers[PHONE->coverStatus]),
			65, 130, (weightCondMultipliers[PHONE->coverStatus]) > 1.0f ? RED : GREEN
		);
	}
	drawText(
		F("Cover Status:  %s", condToString[PHONE->coverStatus]),
		120, 130, WHITE
	);

	// Bottom left box (skill list)
	// This list can be scrolled through and the skill's info shows on the right
	drawBox(0, 152, 140, 88);
	for (int i = 0; i < arrlen(MENU.choices); i++) {
		drawText(MENU.choices[i], 22, 162 + 18*i, WHITE);
	}
    drawTexture("indicator", 7, 162 + 18*MENU.choice, WHITE);

	// Bottom right box (skill info)
	#define SKILL (g.skillDB->skills[PHONE->skills[MENU.choice]])
	drawBox(140, 152, 180, 88);
	drawTextRec(SKILL.description, 145, 156, 152, 80, WHITE);

	switch (SKILL.effects[0].effect) {
		case SE_DAMAGE:
		case SE_DRAIN:
			drawText(F("Damage: %d", SKILL.effects[0].parameter), 145, 205, WHITE);
			break;

		case SE_CONFUSE:
			drawText(F("Chance: %d", SKILL.effects[0].parameter), 145, 205, WHITE);
			break;
	}
	drawText(F("Type: %s", skillTypes[SKILL.type]), 145, 220, WHITE);
}