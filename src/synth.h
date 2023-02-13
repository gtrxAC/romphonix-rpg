#ifndef SYNTH_H
#define SYNTH_H

#include "common.h"

void initSynth(Game *g);
void closeSynth(Game *g);
void setSong(Game *g, const char *path);

#endif