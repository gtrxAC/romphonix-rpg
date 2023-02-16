#include "common.h"
// #include "menu.h"

bool canMove(Game *g, int dX, int dY) {
	int newX = g->playerX + dX;
	int newY = g->playerY + dY;

	// Check if we're out of map bounds
	if (newX < 0 || newY < 0) return false;
	if (newX >= MAP_WIDTH || newY >= MAP_HEIGHT) return false;

	// Check if we're colliding with a solid tile
	if (MAP(newX, newY, MAP_COLLISION)) return false;

	return true;
}

void drawWorldRT(Game *g) {
	UnloadRenderTexture(g->world);
	g->world = LoadRenderTexture(MAP_WIDTH*16, MAP_HEIGHT*16);

	BeginTextureMode(g->world);
	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			DrawTextureRec(
				TEX(tiles_overworld),
				(Rectangle) {MAP(x, y, MAP_BG_X), MAP(x, y, MAP_BG_Y), 16, 16},
				(Vector2) {x*16, y*16}, WHITE
			);
			DrawTextureRec(
				TEX(tiles_overworld),
				(Rectangle) {MAP(x, y, MAP_FG_X), MAP(x, y, MAP_FG_Y), 16, 16},
				(Vector2) {x*16, y*16}, WHITE
			);
		}
	}
	EndTextureMode();
}

void updateWorld(Game *g) {
	Map *curmap = &g->maps[g->curmap];

	if (!g->worlddrawn) {
		g->worlddrawn = true;
		drawWorldRT(g);
	}

	if (g->playeranim > 0) {
		g->playeranim -= K_B() ? RUN_SPEED : WALK_SPEED;

		if (g->playeranim < 1) {
			g->playeranim = 0;
			
			switch (g->playerdir) {
				case DIR_UP: g->playerY--; break;
				case DIR_DOWN: g->playerY++; break;
				case DIR_LEFT: g->playerX--; break;
				case DIR_RIGHT: g->playerX++; break;
			}

			for (int i = 0; i < MAX_SCRIPTS && g->map.scripts[i].type != SA_NULLTERM; i++) {
				Script script = g->map.scripts[i];
				if (script.x == g->playerX && script.y == g->playerY && script.type == SA_TILE) {
					script.func(g);
				}
			}
		}
	} else {
		if (K_UP()) {
			g->playerdir = DIR_UP;
			if (canMove(g, 0, -1)) g->playeranim = 16;
		}

		else if (K_DOWN()) {
			g->playerdir = DIR_DOWN;
			if (canMove(g, 0, 1)) g->playeranim = 16;
		}

		else if (K_LEFT()) {
			g->playerdir = DIR_LEFT;
			if (canMove(g, -1, 0)) g->playeranim = 16;
		}

		else if (K_RIGHT()) {
			g->playerdir = DIR_RIGHT;
			if (canMove(g, 1, 0)) g->playeranim = 16;
		}

		if (K_A_PRESS()) {
			int x = g->playerX;
			int y = g->playerY;
			
			switch (g->playerdir) {
				case DIR_UP: y--; break;
				case DIR_DOWN: y++; break;
				case DIR_LEFT: x--; break;
				case DIR_RIGHT: x++; break;
			}

			for (int i = 0; i < MAX_SCRIPTS && g->map.scripts[i].type != SA_NULLTERM; i++) {
				Script script = g->map.scripts[i];
				if (script.x == x && script.y == y && script.type == SA_INTERACT) {
					script.func(g);
				}
			}
		}

		else if (K_MENU_PRESS()) ingamemenu(g);
	}
}

// This draws the render texture pre-rendered with drawWorldRT, as well as any sprites
void drawWorld(Game *game) {
	int playerdestx = game->playerX*16;
	int playerdesty = game->playerY*16;

	if (game->playeranim) {
		switch (game->playerdir) {
			case DIR_UP: playerdesty -= 12 - game->playeranim; break;
			case DIR_DOWN: playerdesty += 12 - game->playeranim; break;
			case DIR_LEFT: playerdestx -= 12 - game->playeranim; break;
			case DIR_RIGHT: playerdestx += 12 - game->playeranim; break;
		}
	}

	DrawTexturePro(
		game->world.texture,
		(Rectangle) {0, 0, game->maps[game->curmap].width*16, -game->maps[game->curmap].height*16},
		(Rectangle) {
			WIDTH/2 - 16/2 - playerdestx,
			HEIGHT/2 - 16/2 - playerdesty,
			game->maps[game->curmap].width*16, game->maps[game->curmap].height*16
		},
		(Vector2) {0, 0}, 0.0f, WHITE
	);

	DrawTextureRec(
		game->textures.player,
		(Rectangle) {
			16 * game->playerdir,
			(game->playeranim && game->playeranim < 16/2) ? 16 : 0,
			16, 16
		}, (Vector2) {WIDTH/2 - 16/2, HEIGHT/2 - 16/2}, WHITE
	);
}