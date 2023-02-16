#include "common.h"

#define CONTROL_SIZE 40

void draw_controls(Game *game) {
	#ifdef PLATFORM_ANDROID
		// Up
		DrawTextureRec(
			game->textures.controls,
			(Rectangle) {0, 0, CONTROL_SIZE, CONTROL_SIZE},
			(Vector2) {CONTROL_SIZE, HEIGHT - CONTROL_SIZE*3}, Fade(WHITE, 0.5f)
		);

		// Down
		DrawTextureRec(
			game->textures.controls,
			(Rectangle) {CONTROL_SIZE, 0, CONTROL_SIZE, CONTROL_SIZE},
			(Vector2) {CONTROL_SIZE, HEIGHT - CONTROL_SIZE}, Fade(WHITE, 0.5f)
		);

		// Left
		DrawTextureRec(
			game->textures.controls,
			(Rectangle) {CONTROL_SIZE*2, 0, CONTROL_SIZE, CONTROL_SIZE},
			(Vector2) {0, HEIGHT - CONTROL_SIZE*2}, Fade(WHITE, 0.5f)
		);

		// Right
		DrawTextureRec(
			game->textures.controls,
			(Rectangle) {CONTROL_SIZE*3, 0, CONTROL_SIZE, CONTROL_SIZE},
			(Vector2) {CONTROL_SIZE*2, HEIGHT - CONTROL_SIZE*2}, Fade(WHITE, 0.5f)
		);

		// A (right action key)
		DrawTextureRec(
			game->textures.controls,
			(Rectangle) {CONTROL_SIZE*5, 0, CONTROL_SIZE, CONTROL_SIZE},
			(Vector2) {WIDTH - CONTROL_SIZE, HEIGHT - CONTROL_SIZE*2}, Fade(WHITE, 0.5f)
		);

		// B (left action key)
		DrawTextureRec(
			game->textures.controls,
			(Rectangle) {CONTROL_SIZE*4, 0, CONTROL_SIZE, CONTROL_SIZE},
			(Vector2) {WIDTH - CONTROL_SIZE*2, HEIGHT - CONTROL_SIZE*2}, Fade(WHITE, 0.5f)
		);

		// #define INPUTX (GetTouchX()/SCALE)  // GetTouchX also works for mouse, but doesn't work with SetMouseScale
		// #define INPUTY (GetTouchY()/SCALE)

		bool uppress = false;
		bool downpress = false;
		bool leftpress = false;
		bool rightpress = false;
		bool apress = false;
		bool bpress = false;
		
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			for (int i = 0; i < GetTouchPointCount(); i++) {
				Vector2 touch = GetTouchPosition(i);
				touch.x *= WIDTH;
				touch.y *= HEIGHT;

				if (CheckCollisionPointRec(touch, (Rectangle) {CONTROL_SIZE, HEIGHT - CONTROL_SIZE*3, CONTROL_SIZE, CONTROL_SIZE}))
					uppress = true;

				if (CheckCollisionPointRec(touch, (Rectangle) {CONTROL_SIZE, HEIGHT - CONTROL_SIZE, CONTROL_SIZE, CONTROL_SIZE}))
					downpress = true;

				if (CheckCollisionPointRec(touch, (Rectangle) {0, HEIGHT - CONTROL_SIZE*2, CONTROL_SIZE, CONTROL_SIZE}))
					leftpress = true;

				if (CheckCollisionPointRec(touch, (Rectangle) {CONTROL_SIZE*2, HEIGHT - CONTROL_SIZE*2, CONTROL_SIZE, CONTROL_SIZE}))
					rightpress = true;

				if (CheckCollisionPointRec(touch, (Rectangle) {WIDTH - CONTROL_SIZE, HEIGHT - CONTROL_SIZE*2, CONTROL_SIZE, CONTROL_SIZE}))
					apress = true;

				if (CheckCollisionPointRec(touch, (Rectangle) {WIDTH - CONTROL_SIZE*2, HEIGHT - CONTROL_SIZE*2, CONTROL_SIZE, CONTROL_SIZE}))
					bpress = true;
			}
		}
		
		if (uppress) game->key_up++; else game->key_up = 0;
		if (downpress) game->key_down++; else game->key_down = 0;
		if (leftpress) game->key_left++; else game->key_left = 0;
		if (rightpress) game->key_right++; else game->key_right = 0;
		if (apress) game->key_a++; else game->key_a = 0;
		if (bpress) game->key_b++; else game->key_b = 0;
	#endif
}