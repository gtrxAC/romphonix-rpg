#include "../common.h"

// _____________________________________________________________________________
//
//  Battle menu - state update function
// _____________________________________________________________________________
//
void updateBattleMenu() {
    if (MENU.battleState == BS_WAITING || MENU.battleState == BS_WAITING_MOVE) {
        // Command menu is just a standard menu
        updateMenu();
        if (K_B_PRESS() && MENU.battleState == BS_WAITING_MOVE) {
            setBattleState(BS_WAITING);
        }   
    }
    // For any other state, the menu only updates when the confirm button is
    // pressed.
    else if (K_A_PRESS() || K_B_PRESS()) {
        switch (MENU.battleState) {
            case BS_STARTING: setBattleState(BS_WAITING); break;

            case BS_PLAYER_TURN: {
                if (PLAYERP.hp <= 0) {
                    PLAYERP.hp = 0;
                    setBattleState(BS_LOST);
                }
                else if (ENEMYP.hp <= 0) {
                    ENEMYP.hp = 0;
                    setBattleState(BS_WON);
                }
                else {
                    if (MENU.movedFirst) setBattleState(BS_ENEMY_TURN);
                    else setBattleState(BS_AFTER_TURN);
                }
                break;
            }
            
            case BS_ENEMY_TURN: {
                if (PLAYERP.hp <= 0) {
                    PLAYERP.hp = 0;
                    setBattleState(BS_LOST);
                }
                else if (ENEMYP.hp <= 0) {
                    ENEMYP.hp = 0;
                    setBattleState(BS_WON);
                }
                else {
                    if (!MENU.movedFirst) setBattleState(BS_PLAYER_TURN);
                    else setBattleState(BS_AFTER_TURN);
                }
                break;
            }

            case BS_AFTER_TURN: {
                setBattleState(BS_WAITING);
                break;
            }

            case BS_RUN: case BS_WON: case BS_LOST: {
                popMenu();
                setSong(g.mapMeta.songName);
                break;
            } 
        }
    }
}