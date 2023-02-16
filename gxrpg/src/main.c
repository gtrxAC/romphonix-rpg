#include "common.h"
#include "title.h"
#include "world.h"
#include "script.h"
#include "menu.h"
#include "transition.h"
#include "maps.h"
#include "controls.h"

Game game = {0};
RenderTexture rt;

int main(int argc, char **argv) {
	#ifdef PLATFORM_ANDROID
		InitWindow(0, 0, "");
	#else
		InitWindow(WIDTH*SCALE, HEIGHT*SCALE, "game");
	#endif

	InitAudioDevice();

	SetTargetFPS(60);
	SetExitKey(0);

	rt = LoadRenderTexture(WIDTH, HEIGHT);

	#ifndef PLATFORM_ANDROID
		ChangeDirectory("assets");
	#endif

	game.textures.title1 = LoadTexture("title1.png");
	game.textures.title2 = LoadTexture("title2.png");
	game.textures.tileset = LoadTexture("tileset.png");
	game.textures.player = LoadTexture("player.png");
	game.textures.textbox = LoadTexture("textbox.png");
	game.textures.menu = LoadTexture("menu.png");
	game.textures.font = LoadFontEx("font.ttf", FONT_SIZE, NULL, 256);
	game.textures.indicator = LoadTexture("indicator.png");
	game.textures.transition = LoadTexture("transition.png");

	game.textures.textboxn = (NPatchInfo) {
		(Rectangle) {0, 0, game.textures.textbox.width, game.textures.textbox.height},
		TEXTBOX_BORDER_X, TEXTBOX_BORDER_Y, TEXTBOX_BORDER_X, TEXTBOX_BORDER_Y, NPATCH_NINE_PATCH
	};

	game.textures.menun = (NPatchInfo) {
		(Rectangle) {0, 0, game.textures.menu.width, game.textures.textbox.height},
		MENU_BORDER_X, MENU_BORDER_Y, MENU_BORDER_X, MENU_BORDER_Y, NPATCH_NINE_PATCH
	};

	#ifdef PLATFORM_ANDROID
		game.textures.controls = LoadTexture("controls.png");
	#endif

	#ifndef PLATFORM_ANDROID
		ChangeDirectory("..");
	#endif

	loadmaps(&game);

	mainmenu(&game);
	game.state = ST_TITLE;

	while (!WindowShouldClose()) {
		// Update
		switch (game.state) {
			case ST_TITLE: update_title(&game); break;
			case ST_WORLD: update_world(&game); break;
			case ST_MAINMENU: case ST_SCRIPT: update_script(&game); break;
			case ST_TRANSITION: update_transition(&game); break;
		}

		// Draw game to render texture
		BeginTextureMode(rt);
		ClearBackground(BLACK);
		switch (game.state) {
			case ST_TITLE: draw_title(&game); break;
			case ST_SCRIPT: draw_world(&game); // fall through
			case ST_MAINMENU: draw_script(&game); break;
			case ST_WORLD: draw_world(&game); break;
			case ST_TRANSITION: draw_world(&game); draw_transition(&game); break;
		}
		draw_controls(&game);
		EndTextureMode();

		// Draw render texture to screen
		BeginDrawing();
		ClearBackground(BLACK);
		DrawTexturePro(
			rt.texture,
			(Rectangle) {0, 0, WIDTH, -HEIGHT},
			(Rectangle) {0, 0, WIDTH*SCALE, HEIGHT*SCALE},
			(Vector2) {
				GetRandomValue(-game.shake, game.shake),
				GetRandomValue(-game.shake, game.shake)
			}, 0.0f, WHITE
		);
		EndDrawing();

		if (game.shake) game.shake--;
		game.framecount++;
	}

	CloseWindow();
	CloseAudioDevice();
	return 0;
}