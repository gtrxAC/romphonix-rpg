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
    bool shouldClose = false;
    while (!shouldClose) {
        if (WindowShouldClose()) {
            if (g->state == ST_TITLE || g->state == ST_MAINMENU) {
                shouldClose = true;
            }
            else {
                switch (tinyfd_messageBox("Close", "Do you want to save your progress?", "yesnocancel", "question", 0)) {
                    case 1: // yes
                        save(g);
                        // fall through

                    case 2: // no
                        shouldClose = true;
                        break;

                    // case 0 is cancel, we do nothing
                }
            }
        }

        checkBindings(g);

        // Update game state
        switch (g->state) {
            case ST_TITLE: updateTitle(g); break;
            case ST_MAINMENU: /*updateScript(g);*/ break;
            // case ST_TEXTBOX: updateWorld(g); /*updateScript(g);*/ break;
            case ST_INGAME: updateWorld(g); break;
        }
        // Update menu/script system
        if (arrlen(g->menus)) MENU.updateFunc(g);

        // Draw game into a render texture so we can scale it
        BeginTextureMode(g->rt);
        ClearBackground(BLACK);
        switch (g->state) {
            case ST_TITLE: drawTitle(g); break;
            case ST_MAINMENU: /*drawScript(g);*/ break;
            // case ST_TEXTBOX: drawWorld(g); /*drawScript(g);*/ break;
            case ST_INGAME: drawWorld(g); break;
        }
        // Draw menus
        for (int i = 0; i < arrlen(g->menus); i++) MENU.drawFunc(g);
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
