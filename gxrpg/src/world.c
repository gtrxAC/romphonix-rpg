#include "common.h"
#include "maps.h"
#include "menu.h"
#include "controls.h"

bool canMove(Game *g, int dX, int dY) {
	// Check if we're out of map bounds
	if (g->playerX + dX < 0 || g->playerY + dY < 0) return false;
	if (g->playerX + dX >= MAP_WIDTH) return false;
	if (g->playerY + dY >= MAP_HEIGHT) return false;

	// Check if we're colliding with a solid tile
	if (g->mapMeta.collisions[(g->playerY + dY)*g->map.width + g->playerX + dX]) return false;

	// Check if we're colliding with any scripts that have collide=true
	for (int i = 0; i < MAX_SCRIPTS && g->map.scripts[i].type != SA_NULLTERM; i++) {
		Script script = g->mapMeta.scripts[i];
		if (!script.collide) continue;
		if (script.x == g->playerX + dX && script.y == g->playerY + dY) return false;
	}

	return true;
}

void drawWorldRT(Game *g) {
	UnloadRenderTexture(g->world);
	g->world = LoadRenderTexture(g->mapMeta.width*16, g->mapMeta.height*16);

	BeginTextureMode(g->world);
	for (int y = 0; y < g->mapMeta.height; y++) {
		for (int x = 0; x < g->mapMeta.width; x++) {
			DrawTextureRec(
				g->textures.tileset,
				(Rectangle) {
					g->map.tiles[y*g->map.width + x] % TILESET_WIDTH * 16,
					g->map.tiles[y*g->map.width + x] / TILESET_WIDTH * 16,
					16, 16
				},
				(Vector2) {x*16, y*16}, WHITE
			);
		}
	}
	EndTextureMode();
}

void updateWorld(Game *game) {
	Map *curmap = &game->maps[game->curmap];

	if (!game->worlddrawn) {
		game->worlddrawn = true;
		drawWorldRT(game);
	}

	if (game->playeranim > 0) {
		game->playeranim -= K_B() ? RUN_SPEED : WALK_SPEED;

		if (game->playeranim < 1) {
			game->playeranim = 0;
			
			switch (game->playerdir) {
				case DIR_UP: game->playerY--; break;
				case DIR_DOWN: game->playerY++; break;
				case DIR_LEFT: game->playerX--; break;
				case DIR_RIGHT: game->playerX++; break;
			}

			for (int i = 0; i < MAX_SCRIPTS && g->map.scripts[i].type != SA_NULLTERM; i++) {
				Script script = g->map.scripts[i];
				if (script.x == game->playerX && script.y == game->playerY && script.type == SA_TILE) {
					script.func(game);
				}
			}
		}
	} else {
		if (K_UP()) {
			game->playerdir = DIR_UP;
			if (canMove(game, 0, -1)) game->playeranim = 16;
		}

		else if (K_DOWN()) {
			game->playerdir = DIR_DOWN;
			if (canMove(game, 0, 1)) game->playeranim = 16;
		}

		else if (K_LEFT()) {
			game->playerdir = DIR_LEFT;
			if (canMove(game, -1, 0)) game->playeranim = 16;
		}

		else if (K_RIGHT()) {
			game->playerdir = DIR_RIGHT;
			if (canMove(game, 1, 0)) game->playeranim = 16;
		}

		if (K_A_PRESS()) {
			int x = game->playerX;
			int y = game->playerY;
			
			switch (game->playerdir) {
				case DIR_UP: y--; break;
				case DIR_DOWN: y++; break;
				case DIR_LEFT: x--; break;
				case DIR_RIGHT: x++; break;
			}

			for (int i = 0; i < MAX_SCRIPTS && g->map.scripts[i].type != SA_NULLTERM; i++) {
				Script script = g->map.scripts[i];
				if (script.x == x && script.y == y && script.type == SA_INTERACT) {
					script.func(game);
				}
			}
		}

		else if (K_MENU_PRESS()) ingamemenu(game);
	}
}

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