// _____________________________________________________________________________
//
//  save.c - save game loading/saving logic
// _____________________________________________________________________________
//
#include "common.h"

// _____________________________________________________________________________
//
//  Load save file
// _____________________________________________________________________________
//
void load(Game *g) {
    int unused;
    void *save = LoadFileData("save.tfs", &unused);
    memcpy(&g->s, save, sizeof(SaveData));
    
    loadMap(g, g->s.curMap);
    drawWorldRT(g);

    free(save);

    // Load bag contents (static array -> dynamic array)
    for (int b = 0; b < 3; b++) {
        for (int i = 0; i < 20; i++) {
            if (g->s.bag[b][i].count > 0) arrpush(g->bag[b], g->s.bag[b][i]);
        }
    }
}

// _____________________________________________________________________________
//
//  Load settings file
//  This is done as part of the init process, so settings are loaded before the
//  savegame.
// _____________________________________________________________________________
//
void loadSettings(Game *g) {
    if (FileExists("settings.tfs")) {
        int unused;
        void *settings = LoadFileData("settings.tfs", &unused);
        memcpy(&g->settings, settings, sizeof(Settings));
    }
    else {
        g->settings.musicVolume = 6;
        g->settings.sfxVolume = 2;
    }

    // Apply the loaded settings
    // synth.gain is in the range 0-10, default 0.2
    fluid_settings_setnum(
        g->syn.settings, "synth.gain",
        (float) g->settings.musicVolume / 20
    );
    SetMasterVolume((float) g->settings.sfxVolume / 10);
}

// _____________________________________________________________________________
//
//  Write save file
// _____________________________________________________________________________
//
void save(Game *g) {
    // Save bag contents (dynamic array -> static array)
    for (int b = 0; b < 3; b++) {
        for (int i = 0; i < arrlen(g->bag[b]); i++) {
            g->s.bag[b][i] = g->bag[b][i];
        }
    }

    SaveFileData("save.tfs", &g->s, sizeof(SaveData));
}

// _____________________________________________________________________________
//
//  Write settings file (also done when the user says "quit without saving")
// _____________________________________________________________________________
//
void saveSettings(Game *g) {
    SaveFileData("settings.tfs", &g->settings, sizeof(Settings));
}