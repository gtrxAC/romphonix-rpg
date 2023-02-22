#ifndef SCRIPT_H
#define SCRIPT_H

#include "common.h"

void textbox(Game *g, const char *line1, const char *line2);
void menu(Game *g, int numChoices, const char **choices, bool canSkip);
void endScript(Game *g);
void changeMap(Game *g, int map, int x, int y);

#endif