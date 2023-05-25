#include "common.h"

// Shorthand to access the current menu (top of the stack)
#define MENU (arrlast(g.menus))

// Shorthand to access the previous menu
#define LASTMENU (g.menus[arrlen(g.menus) - 2])

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
    CB_NEXT_MENU,  // MENU.nextFunc is run and this menu's menu choice is set to -1 (used for textboxes / NPC scripting)
    CB_CLOSE       // Menu is just closed
} CloseBehavior;

#include "battle/battle.h"

typedef struct Menu {
    unsigned int timer;
	const char **choices;  // stb_ds dynamic array
	int choice;
    CloseBehavior closeBehav;
    int scroll;  // scrolling offset for collection menu
    
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
            const char *textbox[2];
            char textboxDraw[2][64];
            unsigned int textboxTime;
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

        // Battle menu
        struct {
            char battleTextbox[3][64];
            const char *enemyName;
            int enemyActive;  // active phone index of the enemyParty
            int active;
            Phone enemyParty[6];
            BattleState battleState;
            bool canRun;  // is a wild battle?
            int movedFirst;  // did the player move first this turn?
            int playerMove;  // what move did the player choose this turn

            BattlePhone player;
            BattlePhone enemy;
        };

        // Switch phone menu (in battle)
        BattleState nextBattleState;

        // Use item menu (select which phone the item is to be used on)
        struct {
            int itemBagPocket;  // bag pocket id
            int item;  // bag item index (item slot in the user's bag, not item ID!)
        };
    };
} Menu;