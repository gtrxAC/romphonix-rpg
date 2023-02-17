#include "common.h"

// _____________________________________________________________________________
//
//  Loads textures and any graphics/audio related stuff.
//  Note: raylib is used for SFX and fluidsynth for music
// _____________________________________________________________________________
//
void initGame(Game *g) {
    // Create the window
    InitWindow(960, 720, "ROMphonix RPG");
    g->rt = LoadRenderTexture(320, 240);

    InitAudioDevice();
    initSynth(g);

    g->fonts.dialogue = LoadFont("assets/fonts/dialogue.png");
    g->fonts.large = LoadFont("assets/fonts/large.png");

    // Textures are loaded with the LOAD_TEXTURE macro, defined here.
    // They load from the assets/graphics folder, with file extension PNG which
    // does not need to be specified when using the macro.

    // The loaded textures are then used with the TEX macro.
    // For example, a texture loaded with LOAD_TEXTURE("title") is used in
    // raylib functions with TEX(title) - notice lack of quotation marks.
    g->textures = NULL;
    #define LOAD_TEXTURE(n) shput((g->textures), n, LoadTexture("assets/graphics/" n ".png"))
    LOAD_TEXTURE("title");
    LOAD_TEXTURE("indicator");
    LOAD_TEXTURE("textbox");
    LOAD_TEXTURE("player");
    LOAD_TEXTURE("tiles_overworld");

    loadMapIndex(g);
    loadMap(g, 0);
}

// _____________________________________________________________________________
//
//  Unloads any assets that init() has loaded, in reverse order.
// _____________________________________________________________________________
//
void closeGame(Game *g, int status) {
    free(g->map);
    arrfree(g->maps);

    for (int i = 0; i < shlen(g->textures); i++) {
        UnloadTexture(g->textures[i].value);
    }
    shfree(g->textures);

    UnloadFont(g->fonts.dialogue);
    UnloadFont(g->fonts.large);

    closeSynth(g);
    CloseAudioDevice();

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