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

    // Load the icon for the window
    Image icon = LoadImage("assets/graphics/icon.png");
    ImageFormat(&icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    SetWindowIcon(icon);
    UnloadImage(icon);

    InitAudioDevice();
    initSynth(g);

    g->fonts.dialogue = LoadFont("assets/fonts/dialogue.png");
    g->fonts.large = LoadFont("assets/fonts/large.png");
    g->fonts.digits = LoadFont("assets/fonts/digits.png");

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
    LOAD_TEXTURE("textbox_light");
    LOAD_TEXTURE("player");
    LOAD_TEXTURE("tiles_overworld");
    LOAD_TEXTURE("menu_icons");
    LOAD_TEXTURE("rarity");
    LOAD_TEXTURE("phone_icons");
    LOAD_TEXTURE("bag");
    LOAD_TEXTURE("transition");
    LOAD_TEXTURE("flipbox");
    LOAD_TEXTURE("portraits");
    LOAD_TEXTURE("unknown_phone");
    LOAD_TEXTURE("battle_bg");

    // Sound loading/unloading works just like with textures.
    g->sounds = NULL;
    #define LOAD_SOUND(n) shput((g->sounds), n, LoadSound("assets/sounds/sfx/" n ".wav"))
    LOAD_SOUND("select");
    LOAD_SOUND("scroll");
    LOAD_SOUND("back");
    LOAD_SOUND("hit");
    LOAD_SOUND("miss");

    int unused;
    g->phoneDB = (PhoneDatabase *) LoadFileData("assets/data/phones.tfs", &unused);
    printf("PHONERPG: Loaded %d phones\n", g->phoneDB->size);

    g->itemDB = (ItemDatabase *) LoadFileData("assets/data/items.tfs", &unused);
    printf("PHONERPG: Loaded %d items\n", g->itemDB->size);

    g->skillDB = (SkillDatabase *) LoadFileData("assets/data/skills.tfs", &unused);
    printf("PHONERPG: Loaded %d skills\n", g->skillDB->size);

    // Load textures (sprites) for each phone
    for (int i = 0; i < g->phoneDB->size; i++) {
        PhoneSpecs *ph = &SPECS(i);
        shput(
            g->textures, ph->sprite,
            LoadTexture(TextFormat("assets/graphics/phones/%s.png", ph->sprite))
        );
    }

    // Load sprites for each item
    for (int i = 0; i < g->itemDB->size; i++) {
        ItemSpecs *item = &ISPECS(i);
        shput(
            g->textures, item->sprite,
            LoadTexture(TextFormat("assets/graphics/items/%s.png", item->sprite))
        );
    }

    loadMapIndex(g);
}

// _____________________________________________________________________________
//
//  Unloads any assets that init() has loaded, in reverse order.
// _____________________________________________________________________________
//
void closeGame(Game *g, int status) {
    free(g->map);
    arrfree(g->maps);

    free(g->phoneDB);

    for (int i = 0; i < shlen(g->textures); i++) {
        UnloadTexture(g->textures[i].value);
    }
    shfree(g->textures);

    for (int i = 0; i < shlen(g->sounds); i++) {
        UnloadSound(g->sounds[i].value);
    }
    shfree(g->sounds);

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
void error(Game *g, const char *message, bool exit) {
    tinyfd_messageBox("Error", message, "ok", "error", 0);
    if (exit) closeGame(g, EXIT_FAILURE);
}