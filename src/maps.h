#ifndef MAPS_H
#define MAPS_H

#include "common.h"

// Other map attributes like width, height, and data are in the game state,
// loaded when the map is actually in use.
// Amount of step and interact scripts can be increased up to 256, but this
// means more memory is used by each map.
typedef struct Map {
    const char *name;
    const char *fileName;
    const char *songName;
    const char *battleBackground;
    void (*stepScripts[64])();
    void (*interactScripts[64])();
    char encounterChance;
    int encounterTable[16][2];
    Character npcTable[32];
} Map;

#endif