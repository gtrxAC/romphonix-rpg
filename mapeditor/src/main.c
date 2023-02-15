#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "raylib.h"
#include "tinyfiledialogs.h"

Camera2D camera;
char *map;
char *workDir;
int curTileX, curTileY;
RenderTexture mapRT;
Texture tileset;
bool grid = true;
bool mapLoaded = false;

enum {
    MODE_BG,
    MODE_FG,
    MODE_COLLISION,
    MODE_SCRIPT
} mode = MODE_BG;

#define MAP_WIDTH map[0]
#define MAP_HEIGHT map[1]
#define TILE_LENGTH 6

#define MAP(x, y, val) map[2 + TILE_LENGTH*((y)*MAP_WIDTH + (x)) + (val)]

const char *modes[4] = {
    "[1: BG]  2: FG   3: collision   4: script",
    " 1: BG  [2: FG]  3: collision   4: script",
    " 1: BG   2: FG  [3: collision]  4: script",
    " 1: BG   2: FG   3: collision  [4: script]"
};

void newMap() {
    char *input = tinyfd_inputBox("Width", "Map width?", "64");
    if (!input) return;
    int width = atoi(input);

    input = tinyfd_inputBox("Height", "Map height?", "64");
    if (!input) return;
    int height = atoi(input);

    map = calloc(2 + width*height*TILE_LENGTH, 1);
    MAP_WIDTH = width;
    MAP_HEIGHT = height;
    mapLoaded = true;
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "ROMphonix RPG Map Editor");
    SetTargetFPS(120);
    SetExitKey(0);

    tileset = LoadTexture("assets/graphics/tiles_overworld.png");

    workDir = calloc(strlen(GetWorkingDirectory()) + 32, 1);
    strcpy(workDir, GetWorkingDirectory());
    strcat(workDir, "/assets/maps/");

    camera = (Camera2D) {(Vector2) {0, 0}, (Vector2) {0, 0}, 0.0f, 1.0f};

    while (!WindowShouldClose()) {
        // Update
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && mapLoaded) {
            Vector2 pos = GetScreenToWorld2D(GetMousePosition(), camera);
            pos.x /= 16;
            pos.y /= 16;

            if (pos.x >= 0 && pos.y >= 0 && pos.x < MAP_WIDTH && pos.y < MAP_HEIGHT) {
                switch (mode) {
                    case MODE_BG:
                        MAP((int) pos.x, (int) pos.y, 0) = curTileX;
                        MAP((int) pos.x, (int) pos.y, 1) = curTileY;
                        break;

                    case MODE_FG:
                        MAP((int) pos.x, (int) pos.y, 2) = curTileX;
                        MAP((int) pos.x, (int) pos.y, 3) = curTileY;
                        break;

                    // collision and script use IsMouseButtonPressed instead
                }
            }
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            // pressed on tileset - select tile
            if (GetMouseX() < tileset.width && GetMouseY() < tileset.height) {
                curTileX = GetMouseX() / 16;
                curTileY = GetMouseY() / 16;
            }
            else {
                Vector2 pos = GetScreenToWorld2D(GetMousePosition(), camera);
                pos.x /= 16;
                pos.y /= 16;

                switch (mode) {
                    // bg and fg use IsMouseButtonDown instead
                    case MODE_COLLISION:
                        MAP((int) pos.x, (int) pos.y, 4) = !MAP((int) pos.x, (int) pos.y, 4);
                        break;

                    case MODE_SCRIPT:
                        if (MAP((int) pos.x, (int) pos.y, 5)) {
                            MAP((int) pos.x, (int) pos.y, 5) = 0;
                        }
                        else {
                            char *input = tinyfd_inputBox("Script", "What script ID should this tile have?", " ");
                            if (!input) break;
                            MAP((int) pos.x, (int) pos.y, 5) = atoi(input);
                        }
                        break;
                }
            }
        }

        // scroll zooming
        float prevZoom = camera.zoom;

        if (GetMouseWheelMove() < 0) {
            camera.zoom -= 0.5f;
            if (camera.zoom < 0.5f) camera.zoom = 0.5f;
        }
        if (GetMouseWheelMove() > 0) {
            SetTextureFilter(tileset, TEXTURE_FILTER_POINT);
            camera.zoom += 0.5f;
            if (camera.zoom > 10.0f) camera.zoom = 10.0f;
        }
        if (GetMouseWheelMove() != 0.0f) {
            SetTextureFilter(tileset, (floor(camera.zoom) == camera.zoom) ? TEXTURE_FILTER_POINT : TEXTURE_FILTER_BILINEAR);
            camera.offset.x *= (camera.zoom / prevZoom);
            camera.offset.y *= (camera.zoom / prevZoom);
        }

        // panning
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) || IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            camera.offset.x += GetMouseDelta().x;
            camera.offset.y += GetMouseDelta().y;
        }

        // key bindings
        if (IsKeyPressed(KEY_N)) newMap();
        if (IsKeyPressed(KEY_G)) grid = !grid;

        if (IsKeyPressed(KEY_O)) {
            char *file = tinyfd_openFileDialog("Open map", workDir, 0, NULL, NULL, 0);
            if (file) {
                if (map) free(map);
                int unused;
                map = LoadFileData(file, &unused);
                mapLoaded = true;
            }
        }

        if (IsKeyPressed(KEY_S)) {
            char *file = tinyfd_saveFileDialog("Save map", workDir, 0, NULL, NULL);
            if (file) SaveFileData(file, map, 2 + MAP_WIDTH*MAP_HEIGHT*TILE_LENGTH);
        }

        if (IsKeyPressed(KEY_F)) {
            if (tinyfd_messageBox("Fill", "Are you sure you want to fill the map with the currently selected tile? All changes will be lost.", "yesno", "question", 0)) {
                for (int y = 0; y < MAP_HEIGHT; y++) {
                    for (int x = 0; x < MAP_WIDTH; x++) {
                        MAP(x, y, 0) = curTileX;
                        MAP(x, y, 1) = curTileY;
                        MAP(x, y, 2) = 0;
                        MAP(x, y, 3) = 0;
                        MAP(x, y, 4) = 0;
                        MAP(x, y, 5) = 0;
                    }
                }
            }
        }

        if (IsKeyPressed(KEY_ONE)) mode = MODE_BG;
        if (IsKeyPressed(KEY_TWO)) mode = MODE_FG;
        if (IsKeyPressed(KEY_THREE)) mode = MODE_COLLISION;
        if (IsKeyPressed(KEY_FOUR)) mode = MODE_SCRIPT;

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        if (mapLoaded) {
            BeginMode2D(camera);
            for (int y = 0; y < MAP_HEIGHT; y++) {
                for (int x = 0; x < MAP_WIDTH; x++) {
                    switch (mode) {
                        case MODE_BG:
                            DrawTextureRec(
                                tileset, (Rectangle) {MAP(x, y, 0)*16, MAP(x, y, 1)*16, 16, 16},
                                (Vector2) {x*16, y*16}, WHITE
                            );
                            DrawTextureRec(
                                tileset, (Rectangle) {MAP(x, y, 2)*16, MAP(x, y, 3)*16, 16, 16},
                                (Vector2) {x*16, y*16}, ColorAlpha(WHITE, 0.5f)
                            );
                            break;

                        case MODE_FG:
                            DrawTextureRec(
                                tileset, (Rectangle) {MAP(x, y, 0)*16, MAP(x, y, 1)*16, 16, 16},
                                (Vector2) {x*16, y*16}, ColorAlpha(WHITE, 0.5f)
                            );
                            DrawTextureRec(
                                tileset, (Rectangle) {MAP(x, y, 2)*16, MAP(x, y, 3)*16, 16, 16},
                                (Vector2) {x*16, y*16}, WHITE
                            );
                            break;

                        case MODE_COLLISION:
                            DrawTextureRec(
                                tileset, (Rectangle) {MAP(x, y, 0)*16, MAP(x, y, 1)*16, 16, 16},
                                (Vector2) {x*16, y*16}, WHITE
                            );
                            DrawTextureRec(
                                tileset, (Rectangle) {MAP(x, y, 2)*16, MAP(x, y, 3)*16, 16, 16},
                                (Vector2) {x*16, y*16}, WHITE
                            );
                            if (MAP(x, y, 4)) DrawRectangle(x*16, y*16, 16, 16, ColorAlpha(RED, 0.5f));
                            break;

                        case MODE_SCRIPT:
                            DrawTextureRec(
                                tileset, (Rectangle) {MAP(x, y, 0)*16, MAP(x, y, 1)*16, 16, 16},
                                (Vector2) {x*16, y*16}, WHITE
                            );
                            DrawTextureRec(
                                tileset, (Rectangle) {MAP(x, y, 2)*16, MAP(x, y, 3)*16, 16, 16},
                                (Vector2) {x*16, y*16}, WHITE
                            );
                            if (MAP(x, y, 5)) {
                                DrawRectangle(x*16, y*16, 16, 16, ColorAlpha(YELLOW, 0.5f));
                                DrawText(TextFormat("%d", MAP(x, y, 5)), x*16, y*16, 10, BLACK);
                            }
                            break;
                    }
                }
            }
            if (grid) {
                for (int x = 0; x <= MAP_WIDTH*16; x += 16) {
                    DrawLine(x, 0, x, MAP_HEIGHT*16, WHITE);
                }
                for (int y = 0; y <= MAP_HEIGHT*16; y += 16) {
                    DrawLine(0, y, MAP_WIDTH*16, y, WHITE);
                }
            }
            EndMode2D();
        }
        else {
            DrawText("RPXMAP - ROMphonix RPG Map Editor", 256, 0, 20, WHITE);
            DrawText("Press N for new map, O to open, S to save (make sure to save often!)", 256, 20, 20, WHITE);
            DrawText("Left click to place tiles or select from the tileset", 256, 50, 20, WHITE);
            DrawText("Right click drag to move around the map", 256, 70, 20, WHITE);
            DrawText("Scroll wheel to zoom", 256, 90, 20, WHITE);
            DrawText("G to toggle grid, F to fill map with selected tile", 256, 110, 20, WHITE);
            DrawText("Be careful when editing - there is no undo!", 256, 140, 20, WHITE);
        }

        // UI
        DrawText(modes[mode], 0, GetScreenHeight() - 20, 20, WHITE);
        DrawTexture(tileset, 0, 0, ColorAlpha(WHITE, 0.7f));
        DrawRectangleLines(curTileX*16, curTileY*16, 16, 16, RED);
        EndDrawing();
    }

    UnloadTexture(tileset);
    free(map);
    free(workDir);
    CloseWindow();
    return EXIT_SUCCESS;
}
