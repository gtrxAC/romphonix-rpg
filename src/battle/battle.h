#ifndef BATTLE_H
#define BATTLE_H

// PLAYERP and ENEMYP contain phone properties that will (in the player's case)
// be permanently saved.
// MENU.player and MENU.enemy contain temporary properties (see phones.h
// BattlePhone struct)
#define PLAYERP (g.s.party[MENU.active])
#define ENEMYP (MENU.enemyParty[MENU.enemyActive])

// Battle state
typedef enum BattleState {
    BS_STARTING,  // battle is starting (wild X appeared or X wants to fight)
    BS_WAITING,     // Waiting for command (menu is shown)
    BS_WAITING_MOVE,  // Waiting for command (in the move selection screen)
    BS_PLAYER_TURN,
    BS_ENEMY_TURN,
    BS_AFTER_TURN,  // check status effects every turn after both players have moved
    BS_RUN,  // Run from wild battle (always succeeds)
    BS_WON,
    BS_LOST
} BattleState;

// Data of a phone while it's in battle (this gets cleared when the battle ends,
// includes stuff like stat changes)
typedef struct BattlePhone {
    int confusedTurns;
    int atkUpTurns;
    int atkUpAmount;  // can be negative if the stat was decreased
    int defUpTurns;
    int defUpAmount;
} BattlePhone;

// Sprite Y locations of assets/graphics/status.png
typedef enum BattleEffects {
    BE_CONF,
    BE_ATKUP = 13,
    BE_ATKDOWN = 26,
    BE_DEFUP = 39,
    BE_DEFDOWN = 52
} BattleEffects;

#endif