#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h> // needed for tinyFD on windows
#include <stdint.h>
#include <time.h>
#include <math.h>

#include "raylib.h"
#include "stb_ds.h"
#include "tinyfiledialogs.h"
#include "fluidsynth.h"

typedef struct Game Game;

// _____________________________________________________________________________
//
//  Controller macros
//  The game controls are based on 7 buttons: up/down/left/right, A (select),
//  B (back) and menu.
// _____________________________________________________________________________
//
#define K_UP() (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
#define K_DOWN() (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
#define K_LEFT() (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
#define K_RIGHT() (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
#define K_A() (IsKeyDown(KEY_L) || IsKeyDown(KEY_ENTER))
#define K_B() (IsKeyDown(KEY_K) || IsKeyDown(KEY_ESCAPE))
#define K_MENU() IsKeyDown(KEY_SPACE)

#define K_UP_PRESS() (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
#define K_DOWN_PRESS() (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
#define K_LEFT_PRESS() (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
#define K_RIGHT_PRESS() (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
#define K_A_PRESS() (IsKeyPressed(KEY_L) || IsKeyPressed(KEY_ENTER))
#define K_B_PRESS() (IsKeyPressed(KEY_K) || IsKeyPressed(KEY_ESCAPE))
#define K_MENU_PRESS() IsKeyPressed(KEY_SPACE)

// _____________________________________________________________________________
//
//  Other helper macros
// _____________________________________________________________________________
//
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

#define MAP_WIDTH (g->map[0])
#define MAP_HEIGHT (g->map[1])

// Gets a byte of a map tile (val takes a MapByte defined below)
#define MAP(x, y, val) (g->map[2 + 13*((y)*MAP_WIDTH + (x)) + (val)])

// Gets specs for a phone by ID (phone data base constant data, not the phones
// that the player owns, so this doesnt include learned moves, remaining HP, etc)
#define SPECS(id) (g->phoneDB->phones[id])

// Gets specs for an item or skill (database constant data)
#define ISPECS(id) (g->itemDB->items[id])
#define SSPECS(id) (g->skillDB->skills[id])

// Math stuff
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#undef GRAY
#define GRAY (Color) { 255, 255, 255, 224 }
#undef LIGHTGRAY
#define LIGHTGRAY (Color) { 255, 255, 255, 160 }
#undef GREEN
#define GREEN (Color) {16, 255, 96, 255}

// _____________________________________________________________________________
//
//  Enumerations and Structures
// _____________________________________________________________________________
//
enum MapByte {
    MAP_BG_X,
    MAP_BG_Y,
    MAP_FG1_X,
    MAP_FG1_Y,
    MAP_FG2_X,
    MAP_FG2_Y,
    MAP_COLLISION,
    MAP_STEP_SCRIPT,
    MAP_INTERACT_SCRIPT,
    MAP_UNUSED1,
    MAP_UNUSED2,
    MAP_UNUSED3,
    MAP_UNUSED4,
};

typedef enum State {
    ST_TITLE,
    ST_MAINMENU,  // nothing is updated/drawn except for menus
    ST_INGAME,
    ST_TRANSITION,  // transition from one map to another
    ST_BATTLE
} State;

typedef enum Direction {
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT
} Direction;

typedef struct Synth {
    fluid_settings_t *settings;
    fluid_synth_t *synth;
    fluid_audio_driver_t *driver;
    fluid_player_t *player;
    int sfont_id;
    bool loaded;
} Synth;

typedef enum ScriptType {
    SC_TEXTBOX,
    SC_MENU
} ScriptType;

// Other map attributes like width, height, and data are in the game state,
// loaded when the map is actually in use.
// Amount of step and interact scripts can be increased up to 256, but this
// means more memory is used by each map.
typedef struct Map {
    const char *name;
    const char *fileName;
    const char *songName;
    void (*stepScripts[64])(Game *);
    void (*interactScripts[64])(Game *);
} Map;

#include "phones.h"

// _____________________________________________________________________________
//
//  Save data structure
// _____________________________________________________________________________
//
#include "items.h"
#include "skills.h"

typedef struct SaveData {
    // Player location
    int curMap, playerX, playerY, playerDir;

    // Owned phones
    bool phonesSeen[256];
    bool phonesCaught[256];
    Phone party[6];
    Phone pc[10][5][5];  // these values may be tweaked later, but 5 × 5 is a good grid size

    // Items are saved into a normal array, but loaded to a stb_ds dyn array
    Item bag[3][20];

    char name[16];
    char startDate[11]; // YYYY-MM-DD
    int id;
    unsigned int money;
} SaveData;

// _____________________________________________________________________________
//
//  Menu structure
// _____________________________________________________________________________
//
// Shorthand to access the current menu (top of the stack)
#define MENU (arrlast(g->menus))

// Shorthand to access the previous menu
#define LASTMENU (g->menus[arrlen(g->menus) - 2])

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
	void (*updateFunc)(Game *);
	void (*drawFunc)(Game *);
	void (*nextFunc)(Game *);
    
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

// _____________________________________________________________________________
//
//  Game state structure
// _____________________________________________________________________________
//
typedef struct Game {
    bool shouldClose;
    
    // Internal counter that ticks every frame and can be reset by certain actions.
    unsigned int frameCount;

    // Current game "state", basically which screen the player is on
    State state;

    // Rendering/audio stuff
    RenderTexture rt;
	RenderTexture world;
    bool worldDrawn;

    Synth syn;
    bool audioStarted;

    // Map stuff
    Map *maps;  // metadata for all maps
    Map mapMeta;  // metadata for loaded map
    char *map;  // actual data (one map fully loaded at a time)
    
    // Player location (values which aren't written in the save file)
    // nextMap/X/Y are the location where the player moves after a map change transition
	int nextMap;
	int nextX, nextY;
	int playerAnim;

    PhoneDatabase *phoneDB;
    ItemDatabase *itemDB;
    SkillDatabase *skillDB;

    SaveData s;
    Item *bag[3];  // 3 stb_ds dyn arrays

    Menu *menus; // stb_ds dynamic array (menu stack)

    // _________________________________________________________________________
    //
    //  Assets
    //  Note: music is not included here as it's managed by fluidsynth
    // _________________________________________________________________________
    //
    // Gets a texture
    #define TEX(t) (shget(g->textures, #t))

    // Hash table of texture name -> texture struct
    // Allows quick creation of textures without having hundreds of variables
    struct { char *key; Texture value; } *textures;

    struct {
        Font dialogue;
        Font large;
        Font digits;
    } fonts;

    // Gets a sound
    #define SOUND(t) (shget(g->sounds, #t))

    struct { char *key; Sound value; } *sounds;

    // _________________________________________________________________________
    //
    //  Scripting related variables
    // _________________________________________________________________________
    //
    // ScriptType scriptType;
} Game;

#include "script.h"

#endif