#include "common.h"

void drawBox(Game *g, int x, int y, int width, int height) {
    NPatchInfo nPatch = {
		(Rectangle) {0, 0, TEX(textbox).width, TEX(textbox).height},
		4, 4, 4, 4, NPATCH_NINE_PATCH
	};

    DrawTextureNPatch(
        TEX(textbox), nPatch,
        (Rectangle) {x, y, width, height},
        (Vector2) {0, 0}, 0.0f, WHITE
    );
}

void drawText(Game *g, const char *text, int x, int y, Color color) {
    DrawTextEx(g->fonts.dialogue, text, (Vector2) {x, y}, 13, 0, color);
}

void drawTextL(Game *g, const char *text, int x, int y, Color color) {
    DrawTextEx(g->fonts.large, text, (Vector2) {x, y}, 23, 1, color);
}

int measureText(Game *g, const char *text) {
    return MeasureTextEx(g->fonts.dialogue, text, 13, 0).x;
}

int measureTextL(Game *g, const char *text) {
    return MeasureTextEx(g->fonts.large, text, 23, 1).x;
}