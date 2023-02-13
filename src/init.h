#ifndef INIT_H
#define INIT_H

#include "common.h"

void initGame(Game *g);
void closeGame(Game *g, int status);
void error(Game *g, const char *message);

#endif