#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "config.h"

struct Game;
typedef struct Game Game;

typedef enum Direction {
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT
} Direction;

// Map scripts (NPCs, warps, etc)
typedef struct Script {
	enum {            // script activation type
		SA_NULLTERM,  // null terminator
		SA_TILE,      // standing on tile will activate, make sure it's not an
		              // infinite activation loop by using a flag variable or
					  // moving the player away from the tile
		SA_INTERACT   // interacting with tile will activate (e.g. npc)
	} type;
	int x, y;              // position of script tile
	bool collide;          // does the player collide with this tile?
	void (*func)(Game *);  // function to execute when activating (function can
	                       // set game->nextfunc to do multiple textboxes)
} Script;

typedef struct Map {
	const char *name;
	int width, height;
	int tiles[MAX_WORLD_WIDTH*MAX_WORLD_HEIGHT];
	int collisions[MAX_WORLD_WIDTH*MAX_WORLD_HEIGHT];
	Script scripts[MAX_SCRIPTS];
} Map;

struct Game {
	enum {
		ST_TITLE,
		ST_MAINMENU,   // same as script but map and player aren't shown in the background
		ST_INGAME,
		ST_TEXTBOX,
		ST_TRANSITION  // transition from one map to another
	} state;

	struct {
		Texture title1, title2;
		Texture tileset;
		Texture player;
		Texture textbox, menu;
		NPatchInfo textboxn, menun;
		Font font;
		Texture indicator;
		Texture transition;

		#ifdef PLATFORM_ANDROID
			Texture controls;
		#endif
	} textures;

	unsigned int framecount;  // transitions will reset the framecount
	int shake;
	
	Map maps[MAP_COUNT];
	bool worlddrawn;
	RenderTexture world;

	// nextmap/x/y are applied after map transition ends
	int curmap, nextmap;
	int playerX, playerY, nextx, nexty;
	int playeranim;
	Direction playerdir;

	// Touch screen control state (each value is the amount of frames that key has been down)
	#ifdef PLATFORM_ANDROID
		unsigned int key_up;
		unsigned int key_down;
		unsigned int key_left;
		unsigned int key_right;
		unsigned int key_a, key_b;
	#endif

	// Next script function in queue to execute, can be set by script functions
	// to chain multiple actions and textboxes together
	void (*nextfunc)(Game *);

	enum {
		SC_TEXTBOX,
		SC_MENU,
		SC_MENU_ANIM
	} scripttype;

	const char *textbox[2];
	unsigned int textboxtime;

	const char *menuchoices[MAX_MENUCHOICES];
	int menuchoice;
	int nummenuchoices;
	int menuanim;
	Direction menuanimdir;
};

#endif // common.h