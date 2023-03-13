// _____________________________________________________________________________
//
//  ROMphonix RPG - a phone collecting RPG
//  Based on gtrxAC's raylib RPG demo
//
//  This is the main source code file, which only contains the main function.
//  It creates the game state "g" which is passed as a pointer to every other
//  function in the game.
//
//  The main function handles calling other functions that alter the game state
//  or draw parts of the game.
//
//  Most of the important includes and definitions, including the state
//  structure, are in common.h.
// _____________________________________________________________________________
//
#define STB_DS_IMPLEMENTATION
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
    memset(g, 0, sizeof(Game));

    // Initialize the game - this loads the window and assets.
    initGame(g);

    // Set up the game variables.
    SetTargetFPS(60);
    SetExitKey(0);
    g->state = ST_TITLE;
    setSong(g, "assets/sounds/music/LG tune.mid");

    // _________________________________________________________________________
    //
    //  Main loop
    // _________________________________________________________________________
    //
    while (!WindowShouldClose()) {
        // Update game state
        switch (g->state) {
            case ST_TITLE: updateTitle(g); break;
            case ST_MAINMENU: updateScript(g); break;
            case ST_SCRIPT: updateScript(g); break;
            case ST_WORLD: updateWorld(g); break;
        }

        // Draw game into a render texture so we can scale it
        BeginTextureMode(g->rt);
        ClearBackground(BLACK);
        switch (g->state) {
            case ST_TITLE: drawTitle(g); break;
            case ST_MAINMENU: drawScript(g); break;
            case ST_SCRIPT: drawWorld(g); drawScript(g); break;
            case ST_WORLD: drawWorld(g); break;
        }
        EndTextureMode();

        // Draw render texture on screen
        // Note: Y flipped because of OpenGL coordinates
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(
            g->rt.texture,
            (Rectangle){0, 0, 320, -240},
            (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
            (Vector2){0, 0}, 0.0f, WHITE
        );
        EndDrawing();

        g->frameCount++;
    }

    // Unload assets and resources when the close button was pressed
    closeGame(g, 0);
}
