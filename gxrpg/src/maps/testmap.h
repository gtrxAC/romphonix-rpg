#ifndef TESTMAP_H
#define TESTMAP_H

#include "../common.h"
#include "../script.h"
#include "tileset.h"

char testscriptbuf[32] = {0};

void testscript6(Game *game) {
	const char *choices[] = {
		"First option", "Second option", "Long option", 
		"Longer option", "Longest option", "ok"
	};
	menu(game, 6, choices);
	game->nextfunc = endscript;
}

void testscript5(Game *game) {
	textbox(game, "Even more options!", "");
	game->nextfunc = testscript6;
}

void testscript3(Game *game) {
	sprintf(testscriptbuf, "You chose option %d", game->menuchoice + 1);
	textbox(game, testscriptbuf, "");
	game->nextfunc = endscript;
}

void testscript2(Game *game) {
	const char *choices[] = {"First", "Second", "Third"};
	menu(game, 3, choices);
	game->nextfunc = testscript3;
}

void testscript1(Game *game) {
	textbox(game, "Here is another one!", "We even have menus!");
	game->nextfunc = testscript2;
}

void testscript(Game *game) {
	textbox(game, "This is an example", "textbox");
	game->nextfunc = testscript1;
}

void warpscript(Game *game) {
	changemap(game, 1, 0, 0);
}


const Map testmap = {
	"Test Map", 32, 32, {
		// textures of each tile (0 is top left of tileset, 255 is bottom right, assuming 16×16 tiles in tileset)
		W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,T0,T1,T0,T1,T0,T1,T0,T1,T0,T1,T0,T1,T0,T1,T0,T1,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,T2,T3,T2,T3,T2,T3,T2,T3,T2,T3,T2,T3,T2,T3,T2,T3,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,T0,T1,G1,G1,G0,G0,G0,G1,G1,T0,T1,G0,G0,G1,T0,T1,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,T2,T3,G0,G0,G2,G0,G0,G1,G0,T2,T3,G0,G0,G0,T2,T3,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,T0,T1,T0,T1,T0,T1,G0,G0,G0,T0,T1,G1,G0,TS,T0,T1,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,T2,T3,T2,T3,T2,T3,G1,G0,G0,T2,T3,G0,G0,G1,T2,T3,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,T0,T1,G1,G1,G1,G0,G0,G1,G0,T0,T1,G1,G1,TS,T0,T1,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,T2,T3,G1,G0,G0,G0,G1,G0,G0,T2,T3,G0,G1,G0,T2,T3,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,T0,T1,G0,G0,G0,G1,G0,G0,G0,G0,G1,G0,G0,G0,T0,T1,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,T2,T3,G0,G1,G1,G0,G0,G0,G1,G0,G0,G0,G0,DR,T2,T3,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,T0,T1,G0,G0,G0,G1,G0,G1,G0,G0,G0,G1,G1,G0,T0,T1,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,T2,T3,G1,G0,G1,G0,G0,G0,G0,G0,G0,G0,G0,G0,T2,T3,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,G0,G0,G0,G2,G0,G1,G0,G0,G1,G1,G0,G0,G0,G1,T0,T1,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,G2,G1,G0,G1,G0,G0,G0,G2,G1,G0,G0,G1,G1,G0,T2,T3,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,T0,T1,T0,T1,T0,T1,G0,G0,G1,G2,T0,T1,T0,T1,T0,T1,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,T2,T3,T2,T3,T2,T3,G1,G0,G0,G1,T2,T3,T2,T3,T2,T3,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,W1,W1,r0,r1,W1,W1,W1,W1,W1,W1,r0,r1,W1,W1,W1,W1,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,R2,R3,W0,W0,W0,W0,W0,W0,R2,R3,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,W0,R0,R1,W0,W0,W0,W0,W0,W0,W0,W0,R0,R1,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,W0,R2,R3,W0,W0,W0,W0,W0,W0,W0,W0,R2,R3,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,R0,R1,W0,W0,W0,W0,W0,W0,W0,R0,R1,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,R2,R3,W0,W0,W0,W0,W0,W0,W0,R2,R3,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,R0,R1,R0,R1,W0,W0,W0,W0,W0,W0,R0,R1,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,
		W0,W0,W0,W0,W0,W0,W0,W0,R2,R3,R2,R3,W0,W0,W0,W0,W0,W0,R2,R3,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0,W0
	}, {
		// does the player collide with each tile (scripts with collide=1 will always collide)
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	}, {
		// see common.h typedef struct Script
		{SA_INTERACT, 21, 12, true, testscript},
		{SA_INTERACT, 21, 14, true, testscript5},
		{SA_TILE, 21, 17, false, warpscript},
		{SA_NULLTERM}
	}
};

#endif // testmap.h