// _____________________________________________________________________________
//
//  ROMphonix RPG - world.c
//
//  Contains various functions for handling overworld rendering and logic,
//  including the map itself and any overworld characters.
// _____________________________________________________________________________
//
#include "common.h"

bool canMove(int dX, int dY) {
	int newX = g.s.chr.x + dX;
	int newY = g.s.chr.y + dY;

	// Check if we would move out of map bounds
	if (newX < 0 || newY < 0) return false;
	if (newX >= MAP_WIDTH || newY >= MAP_HEIGHT) return false;

	// Check if we're colliding with a solid tile
	if (MAP(newX, newY, MAP_COLLISION)) return false;

	// Check if colliding with an NPC
	for (int i = 0; i < 32; i++) {
		Character npc = g.mapMeta.npcTable[i];
		if (npc.active && npc.x == newX && npc.y == newY) return false;
	}

	return true;
}

void drawWorldRT() {
	UnloadRenderTexture(g.world);
	g.world = LoadRenderTexture(MAP_WIDTH*16, MAP_HEIGHT*16);

	BeginTextureMode(g.world);
	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			drawTextureRec(
				"tiles_overworld",
				(Rectangle) {MAP(x, y, MAP_BG_X)*16, MAP(x, y, MAP_BG_Y)*16, 16, 16},
				(Vector2) {x*16, y*16}, WHITE
			);
			drawTextureRec(
				"tiles_overworld",
				(Rectangle) {MAP(x, y, MAP_FG1_X)*16, MAP(x, y, MAP_FG1_Y)*16, 16, 16},
				(Vector2) {x*16, y*16}, WHITE
			);
			drawTextureRec(
				"tiles_overworld",
				(Rectangle) {MAP(x, y, MAP_FG2_X)*16, MAP(x, y, MAP_FG2_Y)*16, 16, 16},
				(Vector2) {x*16, y*16}, WHITE
			);
		}
	}
	EndTextureMode();
}

void updateCharacter(Character *chr) {
	// only for the player character, do a faster animation (running) if B is down
	chr->anim -= MIN(((chr->isPlayer && K_B()) ? 2 : 1), chr->anim);

	if (chr->anim == 0) {
		if (chr->moveCount != 0) {
			chr->moveCount--;
			if (chr->moveCount != 0) chr->anim = 16;
		} else {
			chr->anim = 0;
		}
		
		switch (chr->dir) {
			case DIR_UP: chr->y--; break;
			case DIR_DOWN: chr->y++; break;
			case DIR_LEFT: chr->x--; break;
			case DIR_RIGHT: chr->x++; break;
		}
		
		// only for player, check if they stepped on a map tile which has a step script
		if (chr->isPlayer && MAP(chr->x, chr->y, MAP_STEP_SCRIPT)) {
			if (g.mapMeta.stepScripts[MAP(chr->x, chr->y, MAP_STEP_SCRIPT) - 1]) {
				g.mapMeta.stepScripts[MAP(chr->x, chr->y, MAP_STEP_SCRIPT) - 1]();
			} else {
				scrNoScript();
			}
		}
	}
}

// Get the position where a character should be drawn (relative to top-left corner of map)
Vector2 getCharacterPosition(Character chr) {
	// draw location = map location * tile size
	int destX = chr.x*16;
	int destY = chr.y*16;

	// change draw location based on movement animation timer (can be in between two tiles)
	if (chr.anim) {
		switch (chr.dir) {
			case DIR_UP: destY -= 16 - chr.anim; break;
			case DIR_DOWN: destY += 16 - chr.anim; break;
			case DIR_LEFT: destX -= 16 - chr.anim; break;
			case DIR_RIGHT: destX += 16 - chr.anim; break;
		}
	}
	return (Vector2) {destX, destY};
}

void drawCharacter(Character chr) {
	// Base draw position is the center of the screen, this is the position of
	// the player, and is also used for calculating the position of NPCs
	Vector2 drawPos = {152, 112};

	if (!chr.isPlayer) {
		Vector2 playerPos = getCharacterPosition(g.s.chr);
		Vector2 npcPos = getCharacterPosition(chr);
		drawPos.x += npcPos.x - playerPos.x;
		drawPos.y += npcPos.y - playerPos.y;
	}

	drawTextureRec(
		chr.sprite,
		(Rectangle) {((16 - chr.anim)/4)*16, chr.dir*16, 16, 16},
		drawPos, WHITE
	);
}

void moveCharacter(Character *chr, Direction dir, int tiles) {
	g.s.chr.dir = dir;
	int dx = 0, dy = 0;
	switch (dir) {
		case DIR_UP: dy = -1; break;
		case DIR_DOWN: dy = 1; break;
		case DIR_LEFT: dx = -1; break;
		case DIR_RIGHT: dx = 1; break;
	}
	if (canMove(dx, dy)) {
		g.s.chr.anim = 16;
		g.s.chr.moveCount = tiles;
	}
}

void updateWorld() {
	if (!g.worldDrawn) {
		g.worldDrawn = true;
		drawWorldRT();
	}

	// Update player (movement animation and input)
	if (g.s.chr.anim != 0) {
		updateCharacter(&g.s.chr);
	} else {
		if (K_UP()) moveCharacter(&g.s.chr, DIR_UP, 1);
		else if (K_DOWN()) moveCharacter(&g.s.chr, DIR_DOWN, 1);
		else if (K_LEFT()) moveCharacter(&g.s.chr, DIR_LEFT, 1);
		else if (K_RIGHT()) moveCharacter(&g.s.chr, DIR_RIGHT, 1);

		// Handle interact scripts (when pressing A, check which tile the player
		// is pointing at, if there is an interact script on that tile, run it)
		if (K_A_PRESS()) {
			int x = g.s.chr.x;
			int y = g.s.chr.y;
			
			switch (g.s.chr.dir) {
				case DIR_UP: y--; break;
				case DIR_DOWN: y++; break;
				case DIR_LEFT: x--; break;
				case DIR_RIGHT: x++; break;
			}

			// Interact script on map tile
			if (MAP(x, y, MAP_INTERACT_SCRIPT)) {
				if (g.mapMeta.interactScripts[MAP(x, y, MAP_INTERACT_SCRIPT) - 1]) {
					g.mapMeta.interactScripts[MAP(x, y, MAP_INTERACT_SCRIPT) - 1]();
				} else {
					scrNoScript();
				}
			}

			// Interact script on NPC who is standing on that tile
			for (int i = 0; i < 32; i++) {
				Character *npc = &g.mapMeta.npcTable[i];
				if (npc->active && npc->x == x && npc->y == y && npc->interactScript) {
					if (g.mapMeta.interactScripts[npc->interactScript - 1]) {
						// Make the NPC look at the player
						Direction opposite;
						switch (g.s.chr.dir) {
							case DIR_UP: opposite = DIR_DOWN; break;
							case DIR_DOWN: opposite = DIR_UP; break;
							case DIR_LEFT: opposite = DIR_RIGHT; break;
							case DIR_RIGHT: opposite = DIR_LEFT; break;
						}
						npc->dir = opposite;

						// Run the script
						g.mapMeta.interactScripts[npc->interactScript - 1]();
					} else {
						scrNoScript();
					}
				}
			}
		}

		else if (K_MENU_PRESS()) scrInGameMenu();
	}

	// Update NPCs (only movement animation)
	for (int i = 0; i < 32; i++) {
		Character *npc = &g.mapMeta.npcTable[i];
		if (npc->active && npc->anim != 0) updateCharacter(npc);
	}
}

// This draws the render texture pre-rendered with drawWorldRT, as well as any sprites
void drawWorld() {
	Vector2 playerDest = getCharacterPosition(g.s.chr);
	DrawTexturePro(
		g.world.texture,
		(Rectangle) {0, 0, MAP_WIDTH*16, -MAP_HEIGHT*16},
		(Rectangle) {
			152 - playerDest.x, 112 - playerDest.y,
			MAP_WIDTH*16, MAP_HEIGHT*16
		},
		(Vector2) {0, 0}, 0.0f, WHITE
	);

	// Draw player and NPCs
	drawCharacter(g.s.chr);
	for (int i = 0; i < 32; i++) {
		Character npc = g.mapMeta.npcTable[i];
		if (npc.active) drawCharacter(npc);
	}
}