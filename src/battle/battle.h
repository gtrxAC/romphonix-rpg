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
    BS_ENEMY_SENDING_OUT, // Enemy (NPC) sending out a phone (animation)
    BS_SENDING_OUT,  // Player is sending out a phone (short animation)
    BS_RETURNING, // Player is returning their phone (switching to another one)
    BS_WAITING,     // Waiting for command (menu is shown)
    BS_WAITING_SKILL,  // Waiting for command (in the skill selection screen)
    BS_WAITING_NO_ENERGY,  // Same as above, but a move was chosen that the phone doesn't have enough energy for
    BS_PLAYER_TURN,
    BS_ENEMY_TURN,
    BS_AFTER_TURN,  // check status effects every turn after both players have moved
    BS_PLAYER_DIED,
    BS_ENEMY_DIED,
    BS_RUN,  // Run from wild battle (always succeeds)
    BS_WON,
    BS_LOST  // If player has phones remaining, show a menu for choosing the next one. Otherwise the battle is lost.
} BattleState;

// Data of a phone while it's in battle (this gets cleared when the battle ends,
// includes stuff like stat changes)
typedef struct BattlePhone {
    int confusedTurns;
    int atkUpTurns;
    int atkUpAmount;  // can be negative if the stat was decreased
    int defUpTurns;
    int defUpAmount;
    int accuracyUpTurns;
    int accuracyUpAmount;
    int shownHP;
} BattlePhone;

// Sprite Y locations of assets/graphics/status.png
typedef enum BattleEffects {
    BE_CONF = 0,
    BE_ATKUP = 13,
    BE_ATKDOWN = 26,
    BE_DEFUP = 39,
    BE_DEFDOWN = 52,
    BE_ACCUP = 65,
    BE_ACCDOWN = 78
} BattleEffects;

#endif