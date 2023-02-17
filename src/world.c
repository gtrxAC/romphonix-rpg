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
				(Rectangle) {MAP(x, y, MAP_BG_X)*16, MAP(x, y, MAP_BG_Y)*16, 16, 16},
				(Vector2) {x*16, y*16}, WHITE
			);
			DrawTextureRec(
				TEX(tiles_overworld),
				(Rectangle) {MAP(x, y, MAP_FG_X)*16, MAP(x, y, MAP_FG_Y)*16, 16, 16},
				(Vector2) {x*16, y*16}, WHITE
			);
		}
	}
	EndTextureMode();
}

void updateWorld(Game *g) {
	if (!g->worldDrawn) {
		g->worldDrawn = true;
		drawWorldRT(g);
	}

	if (g->playerAnim > 0) {
		g->playerAnim -= K_B() ? 4 : 2;

		if (g->playerAnim < 1) {
			g->playerAnim = 0;
			
			switch (g->playerDir) {
				case DIR_UP: g->playerY--; break;
				case DIR_DOWN: g->playerY++; break;
				case DIR_LEFT: g->playerX--; break;
				case DIR_RIGHT: g->playerX++; break;
			}
			
			if (MAP(g->playerX, g->playerY, MAP_STEP_SCRIPT)) {
				g->mapMeta.stepScripts[MAP(g->playerX, g->playerY, MAP_STEP_SCRIPT)](g);
			}
		}
	} else {
		if (K_UP()) {
			g->playerDir = DIR_UP;
			if (canMove(g, 0, -1)) g->playerAnim = 16;
		}
		else if (K_DOWN()) {
			g->playerDir = DIR_DOWN;
			if (canMove(g, 0, 1)) g->playerAnim = 16;
		}
		else if (K_LEFT()) {
			g->playerDir = DIR_LEFT;
			if (canMove(g, -1, 0)) g->playerAnim = 16;
		}
		else if (K_RIGHT()) {
			g->playerDir = DIR_RIGHT;
			if (canMove(g, 1, 0)) g->playerAnim = 16;
		}

		// Handle interact scripts
		if (K_A_PRESS()) {
			int x = g->playerX;
			int y = g->playerY;
			
			switch (g->playerDir) {
				case DIR_UP: y--; break;
				case DIR_DOWN: y++; break;
				case DIR_LEFT: x--; break;
				case DIR_RIGHT: x++; break;
			}

			if (MAP(x, y, MAP_INTERACT_SCRIPT)) {
				g->mapMeta.interactScripts[MAP(x, y, MAP_INTERACT_SCRIPT)](g);
			}
		}

		// else if (K_MENU_PRESS()) scrInGameMenu(g);
	}
}

// This draws the render texture pre-rendered with drawWorldRT, as well as any sprites
void drawWorld(Game *g) {
	int playerDestX = g->playerX*16;
	int playerDestY = g->playerY*16;

	if (g->playerAnim) {
		switch (g->playerDir) {
			case DIR_UP: playerDestY -= 16 - g->playerAnim; break;
			case DIR_DOWN: playerDestY += 16 - g->playerAnim; break;
			case DIR_LEFT: playerDestX -= 16 - g->playerAnim; break;
			case DIR_RIGHT: playerDestX += 16 - g->playerAnim; break;
		}
	}

	DrawTexturePro(
		g->world.texture,
		(Rectangle) {0, 0, MAP_WIDTH*16, -MAP_HEIGHT*16},
		(Rectangle) {
			152 - playerDestX, 112 - playerDestY,
			MAP_WIDTH*16, MAP_HEIGHT*16
		},
		(Vector2) {0, 0}, 0.0f, WHITE
	);

	// (g->playerAnim && g->playerAnim < 16/2) ? 16 : 0,
	DrawTextureRec(
		TEX("player"),
		(Rectangle) {16*g->playerDir, g->playerAnim%4, 16, 16},
		(Vector2) {152, 112}, WHITE
	);
}