#ifndef TESTMAP2_H
#define TESTMAP2_H

#include "../common.h"
#include "../script.h"

void warp2script(Game *game) {
	changemap(game, 0, 10, 10);
}

const Map testmap2 = {
	"Another Test Map", 6, 6, {
		// textures of each tile (0 is top left of tileset, 255 is bottom right, assuming 16×16 tiles in tileset)
		0, 1, 0, 1, 0, 1,
        1, 0, 1, 0, 1, 0,
        0, 1, 4, 5, 0, 1,
        1, 0, 6, 7, 1, 0,
		0, 1, 0, 1, 0, 1,
        1, 0, 1, 0, 1, 8,
	}, {
		// does the player collide with each tile (scripts with collide=1 will always collide)
		0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0,
		0, 0, 1, 1, 0, 0,
		0, 0, 1, 1, 0, 0,
		0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0,
	}, {
		// see maps.h typedef struct Script
		{SA_TILE, 5, 5, false, warp2script},
		{SA_NULLTERM}
	}
};

#endif // testmap2.h