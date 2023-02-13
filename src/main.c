#define STBDS_IMPLEMENTATION
#include "common.h"

// _____________________________________________________________________________
//
//  Loads textures and any graphics/audio related stuff.
//
void init() {
    // Create the game state
    State *s = malloc(sizeof(State));
    if (!s) {
        tinyfd_messageBox("Error", "Failed to allocate game state", "ok", "error", 0);
        unload();
        return 1;
    }

    // Create the window
    InitWindow(960, 720, "ROMphonix RPG");
    s->rt = LoadRenderTexture(320, 240);
}

// _____________________________________________________________________________
//
//  Unloads any assets that init() has loaded - in reverse order of course.
//
void cleanup() {
    UnloadRenderTexture(s->rt);
    CloseWindow();

    free(s);
}

// _____________________________________________________________________________
//
//  Calls initialization tasks and runs the main loop.
//
int main() {
    atexit(cleanup);
    init();

    // Main loop
    while (!WindowShouldClose()) {
        // Update game state

        // Draw game into a render texture so we can scale it
        BeginTextureMode(s->rt);

        EndTextureMode();

        // Draw render texture on screen
        // Note: Y flipped because of OpenGL coordinates
        BeginDrawing();
        ClearBackground(BLACK);

        DrawTexturePro(
            s->rt.texture,
            (Rectangle){0, 0, 320, -240},
            (Rectangle){0, 0, 960, 720},
            (Vector2){0, 0}, 0.0f, WHITE
        );
    }

    exit(EXIT_SUCCESS);
}