#ifndef SCRIPT_H
#define SCRIPT_H

#include "common.h"

void textbox(Game *game, const char *line1, const char *line2);
void menu(Game *game, int numchoices, const char **choices);
void endscript(Game *game);
void changemap(Game *game, int map, int x, int y);
void update_script(Game *game);
void draw_script(Game *game);

#endif // script.h