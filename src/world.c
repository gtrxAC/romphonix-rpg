#include "common.h"
// #include "menu.h"

bool canMove(Game *g, int dX, int dY) {
	int newX = g->s.playerX + dX;
	int newY = g->s.playerY + dY;

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
		g->playerAnim -= K_B() ? 2 : 1;

		if (g->playerAnim < 1) {
			g->playerAnim = 0;
			
			switch (g->s.playerDir) {
				case DIR_UP: g->s.playerY--; break;
				case DIR_DOWN: g->s.playerY++; break;
				case DIR_LEFT: g->s.playerX--; break;
				case DIR_RIGHT: g->s.playerX++; break;
			}
			
			if (MAP(g->s.playerX, g->s.playerY, MAP_STEP_SCRIPT)) {
				if (g->mapMeta.stepScripts[MAP(g->s.playerX, g->s.playerY, MAP_STEP_SCRIPT)]) {
					g->mapMeta.stepScripts[MAP(g->s.playerX, g->s.playerY, MAP_STEP_SCRIPT)](g);
				} else {
					scrNoScript(g);
				}
			}
		}
	} else {
		if (K_UP()) {
			g->s.playerDir = DIR_UP;
			if (canMove(g, 0, -1)) g->playerAnim = 16;
		}
		else if (K_DOWN()) {
			g->s.playerDir = DIR_DOWN;
			if (canMove(g, 0, 1)) g->playerAnim = 16;
		}
		else if (K_LEFT()) {
			g->s.playerDir = DIR_LEFT;
			if (canMove(g, -1, 0)) g->playerAnim = 16;
		}
		else if (K_RIGHT()) {
			g->s.playerDir = DIR_RIGHT;
			if (canMove(g, 1, 0)) g->playerAnim = 16;
		}

		// Handle interact scripts
		if (K_A_PRESS()) {
			int x = g->s.playerX;
			int y = g->s.playerY;
			
			switch (g->s.playerDir) {
				case DIR_UP: y--; break;
				case DIR_DOWN: y++; break;
				case DIR_LEFT: x--; break;
				case DIR_RIGHT: x++; break;
			}

			if (MAP(x, y, MAP_INTERACT_SCRIPT)) {
				if (g->mapMeta.interactScripts[MAP(x, y, MAP_INTERACT_SCRIPT) - 1]) {
					g->mapMeta.interactScripts[MAP(x, y, MAP_INTERACT_SCRIPT) - 1](g);
				} else {
					scrNoScript(g);
				}
			}
		}

		else if (K_MENU_PRESS()) scrInGameMenu(g);
	}
}

// This draws the render texture pre-rendered with drawWorldRT, as well as any sprites
void drawWorld(Game *g) {
	int playerDestX = g->s.playerX*16;
	int playerDestY = g->s.playerY*16;

	if (g->playerAnim) {
		switch (g->s.playerDir) {
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

	DrawTextureRec(
		TEX(player),
		(Rectangle) {((16 - g->playerAnim)/4)*16, 16*g->s.playerDir, 16, 16},
		(Vector2) {152, 112}, WHITE
	);
}