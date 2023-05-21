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

Game g;

// _____________________________________________________________________________
//
//  Calls initialization tasks and runs the main loop.
// _____________________________________________________________________________
//
int main() {
    // Create game state structure
    memset(&g, 0, sizeof(Game));

    // Initialize the game - this loads the window and assets.
    initGame();

    // Set up the game variables.
    SetTargetFPS(60);
    SetExitKey(0);
    g.state = ST_TITLE;
    setSong("assets/sounds/music/abangchung.mid");

    loadSettings();

    // _________________________________________________________________________
    //
    //  Main loop
    // _________________________________________________________________________
    //
    while (!g.shouldClose) {
        // If the close button is pressed, show a menu asking what to do, if it
        // isn't already being shown. In the title or title menu, close without
        // asking.
        if (WindowShouldClose()) {
            if (g.state == ST_TITLE || g.state == ST_MAINMENU) {
                g.shouldClose = true;
            }
            else if (!arrlen(g.menus) || !MENU.textbox[0] || strcmp(MENU.textbox[0], "What do you want to do?")) {
                scrExitMenu();
            }
        }

        // Check global keybindings
        checkBindings();

        // Update menu/script system (only update the topmost/current menu)
        if (arrlen(g.menus)) {
            // Menu sound effects are handled globally here, because they are
            // the same for all menus, so they won't have to be implemented
            // separately for each menu
            if (K_UP_PRESS() || K_DOWN_PRESS() || K_LEFT_PRESS() || K_RIGHT_PRESS()) {
                PlaySound(SOUND(scroll));
            }
            if (K_A_PRESS()) PlaySound(SOUND(select));
            if (K_B_PRESS()) PlaySound(SOUND(back));

            MENU.updateFunc();
            MENU.timer++;
        }
        else {
            // Update game world or title state (only if no menus are open)
            switch (g.state) {
                case ST_TITLE: updateTitle(); break;
                case ST_INGAME: if (!arrlen(g.menus)) updateWorld(); break;
                case ST_TRANSITION: updateTransition(); break;
            }
        }

        // Draw game into a render texture so we can scale it
        BeginTextureMode(g.rt);
        ClearBackground(BLACK);
        switch (g.state) {
            case ST_TITLE: drawTitle(); break;
            case ST_INGAME: drawWorld(); break;
            case ST_TRANSITION: drawWorld(); drawTransition(); break;
        }
        // Draw menus
        for (int i = 0; i < arrlen(g.menus); i++) MENU.drawFunc();
        EndTextureMode();

        // Draw render texture on screen
        // Note: Y flipped because of OpenGL coordinates
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(
            g.rt.texture,
            (Rectangle){0, 0, 320, -240},
            (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
            (Vector2){0, 0}, 0.0f, WHITE
        );
        EndDrawing();

        updateSchedSound();
        g.frameCount++;
    }

    // Unload assets and resources when the close button was pressed
    closeGame(0);
}
