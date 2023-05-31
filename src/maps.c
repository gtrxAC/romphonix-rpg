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

// When creating a new map:
// 1. Include your map header files here
#include "../assets/maps/demo.h"
#include "../assets/maps/demoroom.h"

// 2. Add them to the index here
// The second argument of arrput should be the name of the map metadata
// structure in the map header file (it should be unique)
void loadMapIndex() {
    arrput(g.maps, mapDemo);
    arrput(g.maps, mapDemoRoom);
}

// Don't touch this
void loadMap(int index) {
    int unused;
    g.mapMeta = g.maps[index];

    if (g.map) UnloadFileData(g.map);
    g.map = LoadFileData(g.mapMeta.fileName, &unused);
}