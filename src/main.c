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
    if (!g) error(g, "Failed to allocate game state", true);
    memset(g, 0, sizeof(Game));

    // Initialize the game - this loads the window and assets.
    initGame(g);

    // Set up the game variables.
    SetTargetFPS(60);
    SetExitKey(0);
    g->state = ST_TITLE;
    setSong(g, "assets/sounds/music/abangchung.mid");

    loadSettings(g);

    // _________________________________________________________________________
    //
    //  Main loop
    // _________________________________________________________________________
    //
    while (!g->shouldClose) {
        // If the close button is pressed, show a menu asking what to do, if it
        // isn't already being shown. In the title or title menu, close without
        // asking.
        if (WindowShouldClose()) {
            if (g->state == ST_TITLE || g->state == ST_MAINMENU) {
                g->shouldClose = true;
            }
            else if (!arrlen(g->menus) || !MENU.textbox[0] || strcmp(MENU.textbox[0], "What do you want to do?")) {
                scrExitMenu(g);
            }
        }

        // Check global keybindings
        checkBindings(g);

        // Update menu/script system (only update the topmost/current menu)
        if (arrlen(g->menus)) {
            // Menu sound effects are handled globally here, because they are
            // the same for all menus, so they won't have to be implemented
            // separately for each menu
            if (K_UP_PRESS() || K_DOWN_PRESS() || K_LEFT_PRESS() || K_RIGHT_PRESS()) {
                PlaySound(SOUND(scroll));
            }
            if (K_A_PRESS()) PlaySound(SOUND(select));
            if (K_B_PRESS()) PlaySound(SOUND(back));

            MENU.updateFunc(g);
            MENU.timer++;
        }
        else {
            // Update game world or title state (only if no menus are open)
            switch (g->state) {
                case ST_TITLE: updateTitle(g); break;
                case ST_INGAME: if (!arrlen(g->menus)) updateWorld(g); break;
                case ST_TRANSITION: updateTransition(g); break;
            }
        }

        // Draw game into a render texture so we can scale it
        BeginTextureMode(g->rt);
        ClearBackground(BLACK);
        switch (g->state) {
            case ST_TITLE: drawTitle(g); break;
            case ST_INGAME: drawWorld(g); break;
            case ST_TRANSITION: drawWorld(g); drawTransition(g); break;
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

        updateSchedSound(g);
        g->frameCount++;
    }

    // Unload assets and resources when the close button was pressed
    closeGame(g, 0);
}
