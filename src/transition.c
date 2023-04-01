#include "common.h"

#define TRANSITION_TIME 30       // in frames, for inwards and outwards transition each, so actual time is ×2

void updateTransition(Game *g) {
	if (g->frameCount > TRANSITION_TIME*2) {
		g->state = ST_INGAME;
	} else if (g->frameCount == TRANSITION_TIME) {
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
	float transitionTimer = (float) g->frameCount/TRANSITION_TIME;

	if (g->frameCount > TRANSITION_TIME) {
		transitionTimer = 1 - (float) (g->frameCount - 30)/TRANSITION_TIME;
	}
	
	int x = transitionTimer*160;
	int y = x - 40;
	int size = 320 - transitionTimer*320;

	DrawTexturePro(
		TEX(transition), 
		(Rectangle) {0, 0, 320, 320},
		(Rectangle) {x, y, size, size},
		(Vector2) {0, 0}, 0.0f, WHITE
	);

	DrawRectangle(0, 0, 320, y, BLACK); // top
	DrawRectangle(0, y + size, 320, 160, BLACK); // bottom
	DrawRectangle(0, 0, x, 320, BLACK); // left
	DrawRectangle(x + size, 0, 160, 320, BLACK); // right
	
	// {
	// 	// DrawTexturePro(
    //     //     TEX(transition),
	// 	// 	(Rectangle) {
	// 	// 		TRANSITION_CIRCLE_X/TRANSITION_TIME*(TRANSITION_TIME - g->frameCount),
	// 	// 		TRANSITION_CIRCLE_Y/TRANSITION_TIME*(TRANSITION_TIME - g->frameCount),
	// 	// 		320 + TRANSITION_CIRCLE_X/TRANSITION_TIME*g->frameCount*2,
	// 	// 		240 + TRANSITION_CIRCLE_Y/TRANSITION_TIME*g->frameCount*2,
	// 	// 	},
	// 	// 	(Rectangle) {0, 0, 320, 240}, (Vector2) {0, 0}, 0.0f, WHITE
	// 	// );


	// } else {
	// 	// DrawTexturePro(
	// 	// 	TEX(transition),
	// 	// 	(Rectangle) {
	// 	// 		TRANSITION_CIRCLE_X/TRANSITION_TIME*(g->frameCount - 30),
	// 	// 		TRANSITION_CIRCLE_Y/TRANSITION_TIME*(g->frameCount - 30),
	// 	// 		320 + TRANSITION_CIRCLE_X/TRANSITION_TIME*(TRANSITION_TIME - (g->frameCount - 30))*2,
	// 	// 		240 + TRANSITION_CIRCLE_Y/TRANSITION_TIME*(TRANSITION_TIME - (g->frameCount - 30))*2,
	// 	// 	},
	// 	// 	(Rectangle) {0, 0, 320, 240}, (Vector2) {0, 0}, 0.0f, WHITE
	// 	// );
	// 	DrawTexturePro(
	// 		TEX(transition), 
	// 		(Rectangle) {0, 0, 320, 320},
	// 		(Rectangle) {
	// 			*160,
	// 			(g->frameCount/TRANSITION_TIME)*160 - 40,
	// 			320 - (g->frameCount/TRANSITION_TIME)*320,
	// 			320 - (g->frameCount/TRANSITION_TIME)*320,
	// 		},
	// 		(Vector2) {0, 0}, 0.0f, WHITE
	// 	);
	// }
}