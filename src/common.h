#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h> // needed for tinyFD on windows
#include <stdint.h>

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

// Gets a texture
#define TEX(t) (shget(g->textures, #t))

#define MAP_WIDTH (g->map[0])
#define MAP_HEIGHT (g->map[1])

// Gets a byte of a map tile (val takes a MapByte defined below)
#define MAP(x, y, val) (g->map[2 + 7*((y)*MAP_WIDTH + (x)) + (val)])

// Gets specs for a phone by ID (phone data base constant data, not the phones
// that the player owns, so this doesnt include learned moves, remaining HP, etc)
#define SPECS(id) (g->phoneDB->phones[id])

// Math stuff
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// _____________________________________________________________________________
//
//  Enumerations and Structures
// _____________________________________________________________________________
//
enum MapByte {
    MAP_BG_X,
    MAP_BG_Y,
    MAP_FG_X,
    MAP_FG_Y,
    MAP_COLLISION,
    MAP_STEP_SCRIPT,
    MAP_INTERACT_SCRIPT
};

typedef enum State {
    ST_TITLE,
    ST_MAINMENU,  // same as script but map and player aren't shown in the background
    ST_WORLD,
    ST_SCRIPT,
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
    void (*stepScripts[64])(Game *);
    void (*interactScripts[64])(Game *);
} Map;

#include "phones.h"

// _____________________________________________________________________________
//
//  Save data structure
// _____________________________________________________________________________
//
typedef struct SaveData {
    // Player location
    int curMap, playerX, playerY, playerDir;

    // Owned phones
    bool phonesSeen[256];
    bool phonesCaught[256];
    Phone party[6];
    Phone pc[10][5][5];  // these values may be tweaked later, but 5 × 5 is a good grid size

    unsigned int money;
} SaveData;

// _____________________________________________________________________________
//
//  Game state structure
// _____________________________________________________________________________
//
typedef struct Game {
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

    SaveData s;

    // _________________________________________________________________________
    //
    //  Assets
    //  Note: music is not included here as it's managed by fluidsynth
    // _________________________________________________________________________
    //
    // Hash table of texture name -> texture struct
    // Allows quick creation of textures without having hundreds of variables
    struct { char *key; Texture value; } *textures;

    struct {
        Font dialogue;
        Font large;
    } fonts;

    // _________________________________________________________________________
    //
    //  Scripting related variables
    // _________________________________________________________________________
    //
    ScriptType scriptType;

	// Next script function in queue to execute, can be set by script functions
	// to chain multiple actions and textboxes together
	void (*nextFunc)(Game *);

    // menuchoice can be read by a script function to see which option the user selected
	const char *menuChoices[8];
	int menuChoice;
	int numMenuChoices;
	int menuAnim;
	Direction menuAnimDir;
    bool canSkipMenu;
    int menuScroll;  // scrolling offset for collection menu

    const char *textbox[2];
    char textboxDraw[2][64];
	unsigned int textboxTime;

    // for menus or textboxes that use custom update/draw (usually only draw) functions
	void (*menuUpdateFunc)(Game *);
	void (*menuDrawFunc)(Game *);
} Game;

#include "script.h"

#endif