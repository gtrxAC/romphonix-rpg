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

    // BS_SENDING_OUT state only lasts a second while the animation runs,
    // after that, go to the WAITING state (or if this state was triggered by
    // switching out phones, go to the nextBattleState)
    if (MENU.battleState == BS_SENDING_OUT && g.frameCount > 60) {
        if (MENU.movedFirst) {
            setBattleState(MENU.nextBattleState);
        }
        else {
            setBattleState(BS_WAITING);
        }
    }
    // Same for BS_RETURNING but go to the SENDING_OUT state and actually switch the phone
    if (MENU.battleState == BS_RETURNING && g.frameCount > 60) {
        setBattleState(BS_SENDING_OUT);
    }
    // Same for BS_ENEMY_SENDING_OUT (in NPC battles), go to SENDING_OUT
    if (MENU.battleState == BS_ENEMY_SENDING_OUT && g.frameCount > 60) {
        if (MENU.active == -1) {
            setBattleState(BS_SENDING_OUT);
        }
        else {
            setBattleState(BS_WAITING);
        }
    }

    if (MENU.battleState == BS_WAITING || MENU.battleState == BS_WAITING_SKILL) {
        // Command menu is just a standard menu
        updateMenu();
        if (K_B_PRESS() && MENU.battleState == BS_WAITING_SKILL) {
            setBattleState(BS_WAITING);
        }   
    }
    // For any other state, the menu only updates when the confirm button is
    // pressed. To prevent spamming the confirm button, it only works when the
    // textbox typewriter and health bar animations have finished
    else if ((K_A_PRESS() || K_B_PRESS())) {
        int textboxLen = strlen(MENU.battleTextbox[0]) + strlen(MENU.battleTextbox[1]) + strlen(MENU.battleTextbox[2]) + 20;
        if (MENU.battleTextboxTimer >= textboxLen && MENU.player.shownHP == PLAYERP.hp && MENU.enemy.shownHP == ENEMYP.hp) {
            switch (MENU.battleState) {
                case BS_STARTING: {
                    if (MENU.canRun) {
                        setBattleState(BS_SENDING_OUT);
                    }
                    else {
                        setBattleState(BS_ENEMY_SENDING_OUT);
                    }
                    break;
                }
                case BS_SENDING_OUT: case BS_ENEMY_SENDING_OUT: case BS_RETURNING: break;

                case BS_WAITING_NO_ENERGY: {
                    setBattleState(BS_WAITING_SKILL);
                    break;
                }

                case BS_PLAYER_TURN: {
                    if (PLAYERP.hp <= 0) {
                        PLAYERP.hp = 0;
                        setBattleState(BS_PLAYER_DIED);
                    }
                    else if (ENEMYP.hp <= 0) {
                        ENEMYP.hp = 0;
                        setBattleState(BS_ENEMY_DIED);
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
                        setBattleState(BS_PLAYER_DIED);
                    }
                    else if (ENEMYP.hp <= 0) {
                        ENEMYP.hp = 0;
                        setBattleState(BS_ENEMY_DIED);
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

                case BS_PLAYER_DIED: {
                    setBattleState(BS_LOST);
                    break;
                }

                case BS_ENEMY_DIED: {
                    setBattleState(BS_WON);
                    break;
                }

                case BS_RUN: case BS_WON: case BS_LOST: {
                    popMenu();
                    setSong(g.mapMeta.songName);
                    g.state = ST_INGAME;
                    break;
                } 

                default: {
                    sprintf(MENU.battleTextbox[0], "Don't know what to do next! state %d", MENU.battleState);
                    break;
                }
            }
        }
    }
}