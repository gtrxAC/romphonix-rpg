#define STBDS_IMPLEMENTATION
#define TSF_IMPLEMENTATION
#include "common.h"

// _____________________________________________________________________________
//
//  Loads textures and any graphics/audio related stuff.
// _____________________________________________________________________________
//
void initGame(Game *g) {
    // Create the window
    InitWindow(960, 720, "ROMphonix RPG");
    g->rt = LoadRenderTexture(320, 240);
}

// _____________________________________________________________________________
//
//  Unloads any assets that init() has loaded, in reverse order.
// _____________________________________________________________________________
//
void closeGame(Game *g, int status) {
    UnloadRenderTexture(g->rt);
    CloseWindow();

    free(g);
    exit(status);
}

// _____________________________________________________________________________
//
//  Calls initialization tasks and runs the main loop.
// _____________________________________________________________________________
//
int main() {
    // Create game state structure
    Game *g = malloc(sizeof(Game));
    if (!g) {
        tinyfd_messageBox("Error", "Failed to allocate game state", "ok", "error", 0);
        closeGame(g, 1);
    }

    // Initialize the game - this loads the window and assets.
    initGame(g);

    // _________________________________________________________________________
    //
    //  Main loop
    // _________________________________________________________________________
    //
    while (!WindowShouldClose()) {
        // Update game state
        switch (g->state) {
            
        }

        // Draw game into a render texture so we can scale it
        BeginTextureMode(g->rt);
        switch (g->state) {

        }
        EndTextureMode();

        // Draw render texture on screen
        // Note: Y flipped because of OpenGL coordinates
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(
            g->rt.texture,
            (Rectangle){0, 0, 320, -240},
            (Rectangle){0, 0, 960, 720},
            (Vector2){0, 0}, 0.0f, WHITE
        );
        EndDrawing();
    }

    // Unload assets and resources when the close button was pressed
    closeGame(g, 0);
}