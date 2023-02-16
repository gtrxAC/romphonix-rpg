#ifndef CONTROLS_H
#define CONTROLS_H

#include "common.h"

void draw_controls(Game *game);

#ifdef PLATFORM_ANDROID
	#define K_UP() game->key_up
	#define K_DOWN() game->key_down
	#define K_LEFT() game->key_left
	#define K_RIGHT() game->key_right
	#define K_A() game->key_a
	#define K_B() game->key_b
	#define K_MENU() IsKeyDown(KEY_BACK)

	#define K_UP_PRESS() (game->key_up == 1)
	#define K_DOWN_PRESS() (game->key_down == 1)
	#define K_LEFT_PRESS() (game->key_left == 1)
	#define K_RIGHT_PRESS() (game->key_right == 1)
	#define K_A_PRESS() (game->key_a == 1)
	#define K_B_PRESS() (game->key_b == 1)
	#define K_MENU_PRESS() IsKeyPressed(KEY_BACK)
#else
	#define K_UP() (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
	#define K_DOWN() (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
	#define K_LEFT() (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
	#define K_RIGHT() (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
	#define K_A() IsKeyDown(KEY_L)
	#define K_B() IsKeyDown(KEY_K)
	#define K_MENU() IsKeyDown(KEY_ESCAPE)

	#define K_UP_PRESS() (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
	#define K_DOWN_PRESS() (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
	#define K_LEFT_PRESS() (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
	#define K_RIGHT_PRESS() (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
	#define K_A_PRESS() IsKeyPressed(KEY_L)
	#define K_B_PRESS() IsKeyPressed(KEY_K)
	#define K_MENU_PRESS() IsKeyPressed(KEY_ESCAPE)
#endif

#endif // controls.h