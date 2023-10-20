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
void scrSwitchPhoneMenu(BattleState nextState) {
    pushMenu(CB_CLOSE);
    setDrawFunc(drawPhonesMenu);
    setNextFunc(scrCheckSwitchPhoneMenu);
    MENU.nextBattleState = nextState;

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
        LASTMENU.active = MENU.choice;
        LASTMENU.player = (BattlePhone) {0}; // reset any stat changes
        LASTMENU.player.shownHP = g.s.party[MENU.choice].hp;

        // make the game think that the player has already moved
        // (if player uses the turn to switch phones, they dont get to attack
        // that turn)
        if (MENU.nextBattleState == BS_ENEMY_TURN) {
            LASTMENU.movedFirst = true;
        }

        BattleState nextBS = MENU.nextBattleState;
        popMenu();
        setBattleState(nextBS);
    }
}