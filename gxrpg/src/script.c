#include "common.h"
#include "world.h"
#include "controls.h"

void pushTextbox(Game *game, const char *line1, const char *line2) {
	game->state = ST_TEXTBOX;
	game->scripttype = SC_TEXTBOX;
	game->textbox[0] = line1;
	game->textbox[1] = line2;
	game->textboxtime = game->framecount;
}

void menu(Game *game, int numchoices, const char **choices) {
	game->state = ST_TEXTBOX;
	game->scripttype = SC_MENU;
	game->menuchoice = 0;
	game->nummenuchoices = numchoices;

	for (int i = 0; i < numchoices; i++) {
		game->menuchoices[i] = choices[i];
	}
}

void endscript(Game *game) {
	game->state = ST_INGAME;
}

void changemap(Game *game, int map, int x, int y) {
	game->nextx = x;
	game->nexty = y;
	game->nextmap = map;
	game->worlddrawn = false;

	// reset the frame counter so we don't have to keep track of when the transition started
	game->framecount = 1;
	game->state = ST_TRANSITION;
}

void update_script(Game *game) {
	if (!game->worlddrawn) {
		game->worlddrawn = true;
		drawWorldRT(game);
	}

	switch (game->scripttype) {
		case SC_MENU:
			if (K_UP_PRESS() && game->menuchoice) {
				game->menuanim = 0;
				game->menuanimdir = DIR_UP;
				game->scripttype = SC_MENU_ANIM;
			}
			else if (K_DOWN_PRESS() && game->menuchoice < game->nummenuchoices - 1) {
				game->menuanim = 0;
				game->menuanimdir = DIR_DOWN;
				game->scripttype = SC_MENU_ANIM;
			}
			// fall through

		case SC_TEXTBOX:
			if (K_A_PRESS()) {
				if (game->nextfunc) game->nextfunc(game);
				else game->state = ST_INGAME;
			}
			break;

		case SC_MENU_ANIM:
			game->menuanim += MENU_ANIM_SPEED;
			if (game->menuanim > FONT_SIZE) {
				if (game->menuanimdir == DIR_UP) game->menuchoice--;
				else game->menuchoice++;
				game->scripttype = SC_MENU;
			}
			break;
	}
}

void draw_script(Game *game) {
	// note: draw_world is run before this in the main loop, except in the main menu

	switch (game->scripttype) {
		case SC_TEXTBOX: {
			int linecount = 1;
			if (strlen(game->textbox[1])) linecount++;

			int offset = WIDTH/2 - TEXTBOX_WIDTH/2;

			DrawTextureNPatch(
				game->textures.textbox, game->textures.textboxn,
				(Rectangle) {
					offset, HEIGHT - TEXTBOX_BORDER_Y*2 - FONT_SIZE*linecount,
					TEXTBOX_WIDTH, TEXTBOX_BORDER_Y*2 + FONT_SIZE*linecount
				},
				(Vector2) {0, 0}, 0.0f, WHITE
			);

			DrawTextEx(
				game->textures.font, game->textbox[0],
				(Vector2) {
					offset + TEXTBOX_BORDER_X,
					HEIGHT - TEXTBOX_BORDER_Y*2 - FONT_SIZE*linecount + TEXTBOX_BORDER_Y
				},
				FONT_SIZE, FONT_SPACING, TEXTBOX_FONT_COLOR
			);
			if (linecount == 2) DrawTextEx(
				game->textures.font, game->textbox[1],
				(Vector2) {
					offset + TEXTBOX_BORDER_X,
					HEIGHT - TEXTBOX_BORDER_Y - FONT_SIZE + TEXTBOX_LINE_SPACING
				},
				FONT_SIZE, FONT_SPACING, TEXTBOX_FONT_COLOR
			);
			break;
		}

		case SC_MENU:
		case SC_MENU_ANIM: {
			// Get length of the longest menu choice
			int longest = 0;
			for (int i = 0; i < game->nummenuchoices; i++) {
				Vector2 length = MeasureTextEx(game->textures.font, game->menuchoices[i], FONT_SIZE, FONT_SPACING);
				if (length.x > longest) longest = length.x;
			}

			DrawTextureNPatch(
				game->textures.menu, game->textures.menun,
				(Rectangle) {
					0, 0, MENU_BORDER_X*2 + longest + MENU_INDICATOR_SPACE,
					MENU_BORDER_Y*2 + FONT_SIZE*game->nummenuchoices
				},
				(Vector2) {0, 0}, 0.0f, WHITE
			);

			for (int i = 0; i < game->nummenuchoices && game->menuchoices[i]; i++) {
				DrawTextEx(
					game->textures.font, game->menuchoices[i],
					(Vector2) {
						MENU_BORDER_X + MENU_INDICATOR_SPACE,
						MENU_BORDER_Y + (FONT_SIZE + MENU_LINE_SPACING)*i
					},
					FONT_SIZE, FONT_SPACING, MENU_FONT_COLOR
				);
			}

			int selectory = MENU_BORDER_Y + (FONT_SIZE + MENU_LINE_SPACING)*game->menuchoice;

			if (game->scripttype == SC_MENU_ANIM) {
				if (game->menuanimdir == DIR_UP) selectory -= game->menuanim;
				else selectory += game->menuanim;
			}
			
			DrawTexture(game->textures.indicator, 8, selectory, WHITE);
		}
	}
}