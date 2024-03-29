// _____________________________________________________________________________
//
//  ROMphonix RPG - menu.h
//
//  Contains the structure of the menus. The menus in this game work like a
//  stack - when a new menu is opened, it is pushed to the top of the stack.
//  When it is popped (removed from the stack), the previous menu shows again.
//
//  Each menu uses the same Menu structure, where data specific to certain menus
//  (e.g. battle menu, bag menu) is inside an union. The union members use 
//  overlapping memory, letting us have custom fields for each menu type without 
//  wasting memory for fields that are not used for that menu type.
// _____________________________________________________________________________
//
#include "common.h"

// Shorthand to access the current menu (top of the stack)
#define MENU (arrlast(g.menus))

// Shorthand to access the previous menu
#define LASTMENU (g.menus[arrlen(g.menus) - 2])

// Set functions for menus. This also declares the functions so we don't have to use headers.
#define setUpdateFunc(fn) { void fn(); MENU.updateFunc = (fn); }
#define setDrawFunc(fn) { void fn(); MENU.drawFunc = (fn); }
#define setNextFunc(fn) { void fn(); MENU.nextFunc = (fn); }

typedef enum PlayerMenuState {
    PMS_FRONT,
    PMS_ANIM_FTB,  // front to back
    PMS_BACK,
    PMS_ANIM_BTF   // back to front
} PlayerMenuState;

// What happens when the menu is closed with the B button?
// This only applies if a custom update function is not used
typedef enum CloseBehavior {
    CB_NOTHING,    // Nothing (menu can't be closed)
    CB_CONTINUE,   // MENU.nextFunc is run and this menu's menu choice is set to -1 (used for textboxes / NPC scripting)
    CB_CLOSE       // Menu is just closed
} CloseBehavior;

#include "battle/battle.h"

typedef struct Menu {
    unsigned int timer;
	char **choices;  // stb_ds dynamic array, heap allocated copied strings
	int choice;
    CloseBehavior closeBehav;
    int scroll;  // scrolling offset for collection menu
    bool drawPrevious;  // should previous menus (lower in the menu stack) be drawn behind?
    
    // For menus or textboxes that use custom update/draw/check functions.
    // - Update functions check the user's keyboard input and act accordingly (e.g. move cursor up/down)
    // - Draw functions draw the menu.
    // - Check functions (aka next function) checks what the user has selected in the menu and acts accordingly
	void (*updateFunc)();
	void (*drawFunc)();
	void (*nextFunc)();
    
    // Fields specific to certain menus
    union {
        // Textboxes
        struct {
            char textbox[2][64];
            int textboxTime;
        };
        // Bag (items) menu
        struct {
            int bagChoice;
            bool selected;
        };
        // Player info menu
        PlayerMenuState pms;

        // Phone specs menus
        Phone *phone;

        // Repair menu
        struct {
            Phone *repairPhone;
            int repairAmount;
        };

        // Teach skill menu
        struct {
            Phone *teachPhone;
            int teachSkillID;
        };

        // Battle menu
        struct {
            char battleTextbox[3][64];
            int battleTextboxTimer;

            const char *enemyName;
            const char *enemySprite;
            const char *enemyDefeatMsg[2];  // Message that the enemy says when they are defeated (2 lines)
            int enemyReward;  // Money received from defeating the NPC

            int enemyActive;  // active phone index of the enemyParty
            int active;
            int nextActive;  // next active phone (when switching phones), old one needs to be kept for the duration of the BS_RETURNING animation
            Phone enemyParty[6];
            BattleState battleState;
            bool canRun;  // is a wild battle?
            int movedFirst;  // did the player move first this turn?
            int playerMove;  // what move did the player choose this turn

            // Attack animation
            char attackAnim[64];
            int attackAnimTarget;  // 0 = player, 1 = enemy
            int attackAnimTimer;

            BattlePhone player;
            BattlePhone enemy;
            
            // Which battle state to switch to after the player switches their phone
            // (switch animation is played first)
            BattleState nextBattleState;
        };

        // Use item menu (select which phone the item is to be used on)
        struct {
            int itemBagPocket;  // bag pocket id
            int item;  // bag item index (item slot in the user's bag, not item ID!)
        };
    };
} Menu;