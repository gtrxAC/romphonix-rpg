#include "../common.h"

// _____________________________________________________________________________
//
//  Battle menu - state update function
// _____________________________________________________________________________
//
void updateBattleMenu() {
    // Update health bar animation (up to 2 steps per frame), only after the hit animation
    int animLength = shget(g.textures, MENU.attackAnim).width / 64 * 4;
    
    if (MENU.attackAnimTimer >= 0 && strlen(MENU.attackAnim) && MENU.attackAnimTimer > animLength) {
        for (int i = 0; i < 2; i++) {
            if (MENU.enemy.shownHP < ENEMYP.hp) MENU.enemy.shownHP++;
            if (MENU.enemy.shownHP > ENEMYP.hp) MENU.enemy.shownHP--;
            if (MENU.player.shownHP < PLAYERP.hp) MENU.player.shownHP++;
            if (MENU.player.shownHP > PLAYERP.hp) MENU.player.shownHP--;
        }
    }
    if (MENU.battleState == BS_STARTING) {
        MENU.enemy.shownHP = ENEMYP.hp;
        MENU.player.shownHP = PLAYERP.hp;
    }

    if (MENU.battleState == BS_WAITING || MENU.battleState == BS_WAITING_MOVE) {
        // Command menu is just a standard menu
        updateMenu();
        if (K_B_PRESS() && MENU.battleState == BS_WAITING_MOVE) {
            setBattleState(BS_WAITING);
        }   
    }
    // For any other state, the menu only updates when the confirm button is
    // pressed.
    else if ((K_A_PRESS() || K_B_PRESS())) {
        if (MENU.battleTextboxTimer < 50) {
            MENU.battleTextboxTimer = 1000;
        }
        else switch (MENU.battleState) {
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