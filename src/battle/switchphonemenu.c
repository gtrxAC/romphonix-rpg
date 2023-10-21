// _____________________________________________________________________________
//
//  Switch phone menu
//  This menu is part of the battle system, it uses the draw function
//  of phonesmenu.c, but a custom check function to switch the selected phone
//  into battle.
// _____________________________________________________________________________
//
#include "../common.h"

// _____________________________________________________________________________
//
//  Switch phone menu - init function
// _____________________________________________________________________________
//
void scrSwitchPhoneMenu() {
    pushMenu(CB_CLOSE);
    setDrawFunc(drawPhonesMenu);
    setNextFunc(scrCheckSwitchPhoneMenu);

    // For this menu, we create 6 "dummy" values to let the user scroll freely.
    for (int i = 0; i < 6; i++) addChoice("");
}

// _____________________________________________________________________________
//
//  Switch phone menu - check user input function
// _____________________________________________________________________________
//
void scrCheckSwitchPhoneMenu() {
    if (g.s.party[MENU.choice].active && g.s.party[MENU.choice].hp) {
        LASTMENU.nextActive = MENU.choice;

        // make the game think that the player has already moved
        // (if player uses the turn to switch phones, they dont get to attack
        // that turn)
        popMenu();
        if (MENU.nextBattleState == BS_ENEMY_TURN) {
            MENU.movedFirst = true;
            setBattleState(BS_RETURNING);
        }
        else {
            setBattleState(BS_SENDING_OUT);
        }
    }
}