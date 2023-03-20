#include "common.h"
#include "world.h"

#define TRANSITION_WIDTH 1536
#define TRANSITION_HEIGHT 1536
#define TRANSITION_CIRCLE_X 672
#define TRANSITION_CIRCLE_Y 672
#define TRANSITION_TIME 30       // in frames, for inwards and outwards transition each, so actual time is ×2

void update_transition(Game *game) {
	if (game->framecount > TRANSITION_TIME*2) {
		game->state = ST_INGAME;
	} else if (game->framecount > TRANSITION_TIME) {
		game->curmap = game->nextmap;
		game->playerx = game->nextx;
		game->playery = game->nexty;

		if (!game->worlddrawn) {
			game->worlddrawn = true;
			draw_world_rt(game);
		}
	}
}

void draw_transition(Game *game) {
	if (game->framecount < TRANSITION_TIME) {
		DrawTexturePro(
			game->textures.transition,
			(Rectangle) {
				TRANSITION_CIRCLE_X/TRANSITION_TIME*(TRANSITION_TIME - game->framecount),
				TRANSITION_CIRCLE_Y/TRANSITION_TIME*(TRANSITION_TIME - game->framecount),
				WIDTH + TRANSITION_CIRCLE_X/TRANSITION_TIME*game->framecount*2,
				HEIGHT + TRANSITION_CIRCLE_Y/TRANSITION_TIME*game->framecount*2,
			},
			(Rectangle) {0, 0, WIDTH, HEIGHT}, (Vector2) {0, 0}, 0.0f, WHITE
		);
	} else {
		DrawTexturePro(
			game->textures.transition,
			(Rectangle) {
				TRANSITION_CIRCLE_X/TRANSITION_TIME*(game->framecount - 30),
				TRANSITION_CIRCLE_Y/TRANSITION_TIME*(game->framecount - 30),
				WIDTH + TRANSITION_CIRCLE_X/TRANSITION_TIME*(TRANSITION_TIME - (game->framecount - 30))*2,
				HEIGHT + TRANSITION_CIRCLE_Y/TRANSITION_TIME*(TRANSITION_TIME - (game->framecount - 30))*2,
			},
			(Rectangle) {0, 0, WIDTH, HEIGHT}, (Vector2) {0, 0}, 0.0f, WHITE
		);
	}
}