#include "common.h"
#include "maps/testmap.h"
#include "maps/testmap2.h"

void loadmaps(Game *game) {
	game->maps[0] = testmap;
	game->maps[1] = testmap2;
}