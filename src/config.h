#ifndef CONFIG_H
#define CONFIG_H
// _____________________________________________________________________________
//
//  Configuration macros
//  Originated from gtrxAC's raylib RPG demo
//  This is only temporary, will be removed once everything from the demo
//  is added!
// _____________________________________________________________________________
//

// _____________________________________________________________________________
//
//  Graphics
// _____________________________________________________________________________
//

// Screen size
// Game must be full screen on Android
#ifdef PLATFORM_ANDROID
	#define HEIGHT 180
	#define WIDTH (HEIGHT*((float) GetScreenWidth()/GetScreenHeight()))
	#define SCALE (GetScreenHeight()/HEIGHT)
#else
	#define WIDTH 320
	#define HEIGHT 240
	#define SCALE 3
#endif

// Tile/tileset size
// TILE_WIDTH != TILE_HEIGHT is experimental
// Tileset size is measured in tiles, not pixels
#define TILE_WIDTH 12
#define TILE_HEIGHT 12
#define TILESET_WIDTH 16
#define TILESET_HEIGHT 16

// Font size
#define FONT_SIZE 14
#define FONT_SPACING 0

// Textbox properties
#define TEXTBOX_WIDTH 300
#define TEXTBOX_LINE_SPACING 0
#define TEXTBOX_FONT_COLOR (Color) {24, 16, 20, 255}
#define TEXTBOX_BORDER_X 8
#define TEXTBOX_BORDER_Y 8

// Menu properties
// MENU_INDICATOR_SPACE is how much empty space to give for the indicator on the left side
#define MENU_LINE_SPACING 0
#define MENU_FONT_COLOR (Color) {24, 16, 20, 255}
#define MENU_INDICATOR_SPACE 14
#define MENU_BORDER_X 8
#define MENU_BORDER_Y 8

// _____________________________________________________________________________
//
//  Gameplay
// _____________________________________________________________________________
//

// Maximum values
// MAP_COUNT should be the amount of maps loaded in maps.c -> loadmaps()
// MAX_SCRIPTS is how many scripts a single map can have
#define MAP_COUNT 2
#define MAX_WORLD_WIDTH 128
#define MAX_WORLD_HEIGHT 128
#define MAX_SCRIPTS 128
#define MAX_MENUCHOICES 10

// Animation speeds
// MENU_ANIM_SPEED is how many pixels the menu indicator moves per frame, should be divisible by FONT_SIZE
// WALK/RUN_SPEED should be divisible by TILE_WIDTH and TILE_HEIGHT
#define MENU_ANIM_SPEED 2
#define WALK_SPEED 1
#define RUN_SPEED 3

// Player starting position
// Used when a new save file is created
#define STARTING_MAP 0
#define STARTING_X 10
#define STARTING_Y 10

#endif // config.h