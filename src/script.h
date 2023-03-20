#ifndef SCRIPT_H
#define SCRIPT_H

#include "common.h"

// void pushTextbox(Game *g, const char *line1, const char *line2);
// void pushMenu(Game *g, int numChoices, const char **choices, bool canSkip);
// void endScript(Game *g);
// void changeMap(Game *g, int map, int x, int y);
void popMenu(Game *g);

#endif