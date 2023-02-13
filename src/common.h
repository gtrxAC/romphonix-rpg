#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>

#include "raylib.h"
#include "stb_ds.h"
#include "tinyfiledialogs.h"

typedef enum State {
    ST_TITLE,
    ST_MAINMENU,
    ST_WORLD,
    ST_BATTLE
} State;

typedef struct Game {
    State state;

    RenderTexture rt;
} Game;

#endif