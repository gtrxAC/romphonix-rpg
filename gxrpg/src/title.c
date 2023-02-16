#include "common.h"
#include "controls.h"

void update_title(Game *game) {
	if (GetKeyPressed() || K_A() || K_B() || K_UP() || K_DOWN() || K_LEFT() || K_RIGHT())
		game->state = ST_MAINMENU;
}

void draw_title(Game *game) {
	Color fade;
	Texture texture = game->textures.title1;

	if (game->framecount < 30) {
		// Fade black -> title1 (copyright)
		fade = Fade(WHITE, (float) game->framecount/30);
	} else if (game->framecount < 150) {
		// title1
		fade = WHITE;
	} else if (game->framecount < 180) {
		// Fade title1 -> black
		fade = Fade(WHITE, 1.0f - ((float) game->framecount - 150)/30);
	} else if (game->framecount < 240) {
		// Fade black -> title2 (logo)
		texture = game->textures.title2;
		fade = Fade(WHITE, ((float) game->framecount - 180)/60);
	} else {
		// title2
		texture = game->textures.title2;
		fade = WHITE;
	}

	DrawTexture(texture, WIDTH/2 - texture.width/2, 0, fade);
}