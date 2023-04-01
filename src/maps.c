// _____________________________________________________________________________
//
//  Map index
//  This function maps map metadata to an array. Note that the map metadata is
//  hardcoded into the game, but the actual data is loaded from a file.
//  Maps are contained in assets/maps - .h is metadata and .rpx is data.
//
//  changeMap is in script.c.
// _____________________________________________________________________________
//
#include "common.h"

// Include your map header files here
#include "../assets/maps/demo.h"
#include "../assets/maps/demoroom.h"

// And add them to the index here
void loadMapIndex(Game *g) {
    arrput(g->maps, mapDemo);
    arrput(g->maps, mapDemoRoom);
}

void loadMap(Game *g, int index) {
    int unused;
    g->mapMeta = g->maps[index];

    if (g->map) UnloadFileData(g->map);
    g->map = LoadFileData(g->mapMeta.fileName, &unused);
}