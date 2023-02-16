#include "common.h"
#include "maps.h"
#include "menu.h"
#include "controls.h"

bool canmove(Game *game, int dx, int dy) {
	Map *curmap = &game->maps[game->curmap];

	// Check if we're out of map bounds
	if (game->playerx + dx < 0 || game->playery + dy < 0) return false;
	if (game->playerx + dx >= curmap->width) return false;
	if (game->playery + dy >= curmap->height) return false;

	// Check if we're colliding with a solid tile
	if (curmap->collisions[(game->playery + dy)*curmap->width + game->playerx + dx]) return false;

	// Check if we're colliding with any scripts that have collide=true
	for (int i = 0; i < MAX_SCRIPTS && curmap->scripts[i].type != SA_NULLTERM; i++) {
		Script script = curmap->scripts[i];
		if (!script.collide) continue;
		if (script.x == game->playerx + dx && script.y == game->playery + dy) return false;
	}

	return true;
}

void draw_world_rt(Game *game) {
	Map *curmap = &game->maps[game->curmap];

	UnloadRenderTexture(game->world);
	game->world = LoadRenderTexture(curmap->width*TILE_WIDTH, curmap->height*TILE_HEIGHT);

	BeginTextureMode(game->world);
	for (int y = 0; y < curmap->height; y++) {
		for (int x = 0; x < curmap->width; x++) {
			DrawTextureRec(
				game->textures.tileset,
				(Rectangle) {
					curmap->tiles[y*curmap->width + x] % TILESET_WIDTH * TILE_WIDTH,
					curmap->tiles[y*curmap->width + x] / TILESET_WIDTH * TILE_HEIGHT,
					TILE_WIDTH, TILE_HEIGHT
				},
				(Vector2) {x*TILE_WIDTH, y*TILE_HEIGHT}, WHITE
			);
		}
	}
	EndTextureMode();
}

void update_world(Game *game) {
	Map *curmap = &game->maps[game->curmap];

	if (!game->worlddrawn) {
		game->worlddrawn = true;
		draw_world_rt(game);
	}

	if (game->playeranim > 0) {
		game->playeranim -= K_B() ? RUN_SPEED : WALK_SPEED;

		if (game->playeranim < 1) {
			game->playeranim = 0;
			
			switch (game->playerdir) {
				case DIR_UP: game->playery--; break;
				case DIR_DOWN: game->playery++; break;
				case DIR_LEFT: game->playerx--; break;
				case DIR_RIGHT: game->playerx++; break;
			}

			for (int i = 0; i < MAX_SCRIPTS && curmap->scripts[i].type != SA_NULLTERM; i++) {
				Script script = curmap->scripts[i];
				if (script.x == game->playerx && script.y == game->playery && script.type == SA_TILE) {
					script.func(game);
				}
			}
		}
	} else {
		if (K_UP()) {
			game->playerdir = DIR_UP;
			if (canmove(game, 0, -1)) game->playeranim = TILE_HEIGHT;
		}

		else if (K_DOWN()) {
			game->playerdir = DIR_DOWN;
			if (canmove(game, 0, 1)) game->playeranim = TILE_HEIGHT;
		}

		else if (K_LEFT()) {
			game->playerdir = DIR_LEFT;
			if (canmove(game, -1, 0)) game->playeranim = TILE_WIDTH;
		}

		else if (K_RIGHT()) {
			game->playerdir = DIR_RIGHT;
			if (canmove(game, 1, 0)) game->playeranim = TILE_WIDTH;
		}

		if (K_A_PRESS()) {
			int x = game->playerx;
			int y = game->playery;
			
			switch (game->playerdir) {
				case DIR_UP: y--; break;
				case DIR_DOWN: y++; break;
				case DIR_LEFT: x--; break;
				case DIR_RIGHT: x++; break;
			}

			for (int i = 0; i < MAX_SCRIPTS && curmap->scripts[i].type != SA_NULLTERM; i++) {
				Script script = curmap->scripts[i];
				if (script.x == x && script.y == y && script.type == SA_INTERACT) {
					script.func(game);
				}
			}
		}

		else if (K_MENU_PRESS()) ingamemenu(game);
	}
}

void draw_world(Game *game) {
	int playerdestx = game->playerx*TILE_WIDTH;
	int playerdesty = game->playery*TILE_HEIGHT;

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
		(Rectangle) {0, 0, game->maps[game->curmap].width*TILE_WIDTH, -game->maps[game->curmap].height*TILE_HEIGHT},
		(Rectangle) {
			WIDTH/2 - TILE_WIDTH/2 - playerdestx,
			HEIGHT/2 - TILE_HEIGHT/2 - playerdesty,
			game->maps[game->curmap].width*TILE_WIDTH, game->maps[game->curmap].height*TILE_HEIGHT
		},
		(Vector2) {0, 0}, 0.0f, WHITE
	);

	DrawTextureRec(
		game->textures.player,
		(Rectangle) {
			TILE_WIDTH * game->playerdir,
			(game->playeranim && game->playeranim < TILE_WIDTH/2) ? TILE_HEIGHT : 0,
			TILE_WIDTH, TILE_HEIGHT
		}, (Vector2) {WIDTH/2 - TILE_WIDTH/2, HEIGHT/2 - TILE_HEIGHT/2}, WHITE
	);
}