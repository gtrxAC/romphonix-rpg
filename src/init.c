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

    initSynth(g);
    setSong(g, "assets/sounds/music/LG tune.mid");
}

// _____________________________________________________________________________
//
//  Unloads any assets that init() has loaded, in reverse order.
// _____________________________________________________________________________
//
void closeGame(Game *g, int status) {
    closeSynth(g);

    UnloadRenderTexture(g->rt);
    CloseWindow();

    free(g);
    exit(status);
}

// _____________________________________________________________________________
//
//  Exits with an error.
// _____________________________________________________________________________
//
void error(Game *g, const char *message) {
    tinyfd_messageBox("Error", message, "ok", "error", 0);
    closeGame(g, EXIT_FAILURE);
}