#define STBDS_IMPLEMENTATION
#define TSF_IMPLEMENTATION
#include "common.h"

// _____________________________________________________________________________
//
//  Calls initialization tasks and runs the main loop.
// _____________________________________________________________________________
//
int main() {
    // Create game state structure
    Game *g = malloc(sizeof(Game));
    if (!g) error(g, "Failed to allocate game state");

    // Initialize the game - this loads the window and assets.
    initGame(g);

    // Set up the game variables.
    SetTargetFPS(60);
    g->state = ST_TITLE;

    // _________________________________________________________________________
    //
    //  Main loop
    // _________________________________________________________________________
    //
    while (!WindowShouldClose()) {
        // Update game state
        switch (g->state) {
            case ST_TITLE: updateTitle(g); break;
        }

        // Draw game into a render texture so we can scale it
        BeginTextureMode(g->rt);
        ClearBackground(BLACK);
        switch (g->state) {
            case ST_TITLE: drawTitle(g); break;
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