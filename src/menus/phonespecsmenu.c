// _____________________________________________________________________________
//
//  Phone specs menu
// _____________________________________________________________________________
//
#include "../common.h"

void scrPhoneSpecsMenu(Phone *phone);
void updatePhoneSpecsMenu();
void drawPhoneSpecsMenu();

// _____________________________________________________________________________
//
//  Phone specs menu - init function
//  The init function takes a second argument, the phone whose specs to show.
// _____________________________________________________________________________
//
void scrPhoneSpecsMenu(Phone *phone) {
    pushMenu(0, NULL, CB_CLOSE);
    MENU.updateFunc = updatePhoneSpecsMenu;
    MENU.drawFunc = drawPhoneSpecsMenu;
	MENU.phone = phone;

	for (int i = 0; i < 4; i++) {
		arrpush(MENU.choices, g.skillDB->skills[MENU.phone->skills[i]].name);
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
    DrawTexture(shget(g.textures, PHONESPECS.sprite), 4, 4, WHITE);

	// Top right box (name, battery, level, exp)
	drawBox(72, 0, 248, 72);
    drawText(TextFormat("%s %s", PHONESPECS.brand, PHONESPECS.model), 80, 10, WHITE);
    drawTextD(TextFormat("$ %d", PHONE->level), 285, 9, WHITE);
    drawText(TextFormat("Battery: %d%%", (int) ((float) PHONE->hp/PHONE->maxHP)*100), 80, 29, WHITE);
    drawText(TextFormat("EXP: %d/%d", PHONE->exp, PHONE->maxExp), 80, 48, WHITE);

	// Middle box (stats and status)
	drawBox(0, 72, 320, 80);
    drawText(TextFormat("HP: %d", PHONE->maxHP), 6, 82, WHITE);
    drawText(TextFormat("AT: %d", PHONE->attack), 6, 98, WHITE);
    drawText(TextFormat("DF: %d", PHONE->defense), 6, 114, WHITE);
    drawText(TextFormat("WT: %d", PHONE->weight), 6, 130, WHITE);

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
	drawBox(0, 152, 160, 88);
	for (int i = 0; i < arrlen(MENU.choices); i++) {
		drawText(MENU.choices[i], 22, 162 + 18*i, WHITE);
	}
    DrawTexture(TEX(indicator), 7, 162 + 18*MENU.choice, WHITE);

	// Bottom right box (skill info)
	#define SKILL (g.skillDB->skills[PHONE->skills[MENU.choice]])
	drawBox(160, 152, 160, 88);
	drawTextRec(SKILL.description, 164, 156, 152, 80, WHITE);

	switch (SKILL.effects[0].effect) {
		case SE_DAMAGE:
		case SE_DRAIN:
			drawText(TextFormat("Damage: %d", SKILL.effects[0].parameter), 165, 205, WHITE);
			break;

		case SE_CONFUSE:
			drawText(TextFormat("Chance: %d", SKILL.effects[0].parameter), 165, 205, WHITE);
			break;
	}
	drawText(TextFormat("Type: %s", skillTypes[SKILL.type]), 165, 220, WHITE);
}