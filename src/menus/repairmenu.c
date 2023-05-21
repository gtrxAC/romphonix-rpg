// _____________________________________________________________________________
//
//  Repair menu
//  Lets the user choose which condition type of a phone to improve
//  (when using the Screwdriver item).
//  The selected phone is passed through from the use item menu.
// _____________________________________________________________________________
//
#include "../common.h"

void scrRepairMenu(Phone *phone, int amount);
void scrCheckRepairMenu();
void drawRepairMenu();

// _____________________________________________________________________________
//
//  Repair menu - init function
// _____________________________________________________________________________
//
void scrRepairMenu(Phone *phone, int amount) {
    scrTextBoxMenu();
    MENU.drawFunc = drawRepairMenu;
    MENU.nextFunc = scrCheckRepairMenu;
    MENU.repairPhone = phone;
    MENU.repairAmount = amount;
    
    if (phone->batteryStatus < COND_GOOD) arrpush(
        MENU.choices,
        TextFormat(
            "Battery (HP): %s -> %s",
            condToString[phone->batteryStatus],
            condToString[MIN(phone->batteryStatus + amount, COND_GOOD)]
        )
    );
    if (phone->screenStatus < COND_GOOD) arrpush(
        MENU.choices,
        TextFormat(
            "Screen (AT): %s -> %s",
            condToString[phone->screenStatus],
            condToString[MIN(phone->screenStatus + amount, COND_GOOD)]
        )
    );
    if (phone->boardStatus < COND_GOOD) arrpush(
        MENU.choices,
        TextFormat(
            "Board (DF): %s -> %s",
            condToString[phone->boardStatus],
            condToString[MIN(phone->boardStatus + amount, COND_GOOD)]
        )
    );
    if (phone->coverStatus < COND_GOOD) arrpush(
        MENU.choices,
        TextFormat(
            "Cover (WT): %s -> %s",
            condToString[phone->coverStatus],
            condToString[MIN(phone->coverStatus + amount, COND_GOOD)]
        )
    );
}

// _____________________________________________________________________________
//
//  Repair menu - draw function
// _____________________________________________________________________________
//
void drawRepairMenu() {
    // This is just a text box (but only one static line and without typewriter effect)
	drawBox(10, 210, 300, 30);
	DrawTextEx(
		g.fonts.dialogue, "Which part's condition do you want to improve?",
		(Vector2) {18, 218}, 13, 0, WHITE
	);

    // Draw a standard selection menu
    drawMenu();
}

// _____________________________________________________________________________
//
//  Repair menu - check user input function
// _____________________________________________________________________________
//
void scrCheckRepairMenu() {
    if (MENU.choice == -1) {
        popMenu();
        return;
    }

    else {
        if (strncmp(MENU.choices[MENU.choice], "Battery", 7)) {
            MENU.repairPhone->batteryStatus = MIN(
                MENU.repairPhone->batteryStatus + MENU.repairAmount, COND_GOOD
            );
        }
        if (strncmp(MENU.choices[MENU.choice], "Screen", 6)) {
            MENU.repairPhone->screenStatus = MIN(
                MENU.repairPhone->screenStatus + MENU.repairAmount, COND_GOOD
            );
        }
        if (strncmp(MENU.choices[MENU.choice], "Board", 5)) {
            MENU.repairPhone->boardStatus = MIN(
                MENU.repairPhone->boardStatus + MENU.repairAmount, COND_GOOD
            );
        }
        else {
            MENU.repairPhone->coverStatus = MIN(
                MENU.repairPhone->coverStatus + MENU.repairAmount, COND_GOOD
            );
        }

        popMenu(); // close this menu
        popMenu(); // close use item menu
        popMenu(); // close item actions menu

        // excessive spaces to create a little bit of a pause
        pushTextbox(
            g, "Condition improved!                    ",
            "Oh, and your screwdriver broke..."
        );
        MENU.nextFunc = popMenu;
    }
}