#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>

#include "raylib.h"
#include "stb_ds.h"
#include "tinyfiledialogs.h"
#include "fluidsynth.h"

typedef enum State {
    ST_TITLE,
    ST_MAINMENU,
    ST_WORLD,
    ST_BATTLE
} State;

typedef struct Synth {
    fluid_settings_t *settings;
    fluid_synth_t *synth;
    fluid_audio_driver_t *driver;
    fluid_player_t *player;
    int sfont_id;
} Synth;

typedef struct Game {
    State state;

    RenderTexture rt;

    Synth syn;
} Game;

#include "synth.h"
#include "init.h"

#include "title.h"

#endif