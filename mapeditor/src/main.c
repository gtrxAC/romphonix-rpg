#include "raylib.h"
#include "tinyfiledialogs.h"

Camera2D camera;
char *map;
int curTileX, curTileY;
RenderTexture mapRT;
Texture tileset;
bool grid = true;

#define MAP_WIDTH map[0]
#define MAP_HEIGHT map[1]

#define MAP(x, y, val) map[2 + 4*((y)*MAP_WIDTH + (x)) + (val)]

int main() {
    map = (char []) {0, 0};

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "ROMphonix RPG Map Editor");

    tileset = LoadTexture("assets/graphics/tiles_overworld.png");

    camera = (Camera2D) {(Vector2) {0, 0}, (Vector2) {0, 0}, 0.0f, 1.0f};

    while (!WindowShouldClose()) {
        // Update
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // pressed on tileset - select tile
            if (GetMouseX() < tileset.width && GetMouseY() < tileset.height) {
                curTileX = GetMouseX() / 16;
                curTileY = GetMouseY() / 16;
            }

            // pressed on map - write tile to map
            else {
                Vector2 pos = GetWorldToScreen2D(GetMousePosition(), camera);
                MAP((int) pos.x / 16, (int) pos.y / 16, 0) = curTileX;
                MAP((int) pos.x / 16, (int) pos.y / 16, 1) = curTileY;
            }
        }

        // scroll zooming
        if (GetMouseWheelMove() < 0) {
            camera.zoom -= 0.2f;
            if (camera.zoom < 0.2f) camera.zoom = 0.2f;
        }
        if (GetMouseWheelMove() > 0) {
            camera.zoom += 0.2f;
            if (camera.zoom > 10.0f) camera.zoom = 10.0f;
        }

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);
        if (grid) {
            for (int x = 0; x < MAP_WIDTH*16; x += 16) {
                DrawLine(x, 0, x, MAP_HEIGHT*16, WHITE);
            }
            for (int y = 0; y < MAP_HEIGHT*16; y += 16) {
                DrawLine(0, y, MAP_WIDTH*16, 0, WHITE);
            }
        }

        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                DrawTextureRec(
                    tileset, (Rectangle) {MAP(x, y, 0), MAP(x, y, 1), 16, 16},
                    (Vector2) {x*16, y*16}, WHITE
                );
            }
        }
        EndMode2D();

        // UI
        DrawTexture(tileset, 0, 0, ColorAlpha(WHITE, 0.8f));
        DrawRectangleLines(curTileX*16, curTileY*16, 16, 16, RED);
        EndDrawing();
    }
}
