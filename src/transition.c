#include "common.h"

#define TRANSITION_WIDTH 1536
#define TRANSITION_HEIGHT 1536
#define TRANSITION_CIRCLE_X 672
#define TRANSITION_CIRCLE_Y 672
#define TRANSITION_TIME 30       // in frames, for inwards and outwards transition each, so actual time is ×2

void updateTransition(Game *g) {
	if (g->frameCount > TRANSITION_TIME*2) {
		g->state = ST_INGAME;
	} else if (g->frameCount > TRANSITION_TIME) {
		g->s.curMap = g->nextMap;
		g->s.playerX = g->nextX;
		g->s.playerY = g->nextY;

        loadMap(g, g->s.curMap);

		if (!g->worldDrawn) {
			g->worldDrawn = true;
			drawWorldRT(g);
		}
	}
}

void drawTransition(Game *g) {
	if (g->frameCount < TRANSITION_TIME) {
		DrawTexturePro(
            TEX(transition),
			(Rectangle) {
				TRANSITION_CIRCLE_X/TRANSITION_TIME*(TRANSITION_TIME - g->frameCount),
				TRANSITION_CIRCLE_Y/TRANSITION_TIME*(TRANSITION_TIME - g->frameCount),
				320 + TRANSITION_CIRCLE_X/TRANSITION_TIME*g->frameCount*2,
				240 + TRANSITION_CIRCLE_Y/TRANSITION_TIME*g->frameCount*2,
			},
			(Rectangle) {0, 0, 320, 240}, (Vector2) {0, 0}, 0.0f, WHITE
		);
	} else {
		DrawTexturePro(
			TEX(transition),
			(Rectangle) {
				TRANSITION_CIRCLE_X/TRANSITION_TIME*(g->frameCount - 30),
				TRANSITION_CIRCLE_Y/TRANSITION_TIME*(g->frameCount - 30),
				320 + TRANSITION_CIRCLE_X/TRANSITION_TIME*(TRANSITION_TIME - (g->frameCount - 30))*2,
				240 + TRANSITION_CIRCLE_Y/TRANSITION_TIME*(TRANSITION_TIME - (g->frameCount - 30))*2,
			},
			(Rectangle) {0, 0, 320, 240}, (Vector2) {0, 0}, 0.0f, WHITE
		);
	}
}