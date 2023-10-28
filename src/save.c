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
void load() {
    int unused;
    void *save = LoadFileData("save.tfs", &unused);
    memcpy(&g.s, save, sizeof(SaveData));
    
    loadMap(g.s.curMap);
    drawWorldRT();

    free(save);

    // Load bag contents (static array -> dynamic array)
    for (int b = 0; b < 3; b++) {
        for (int i = 0; i < 20; i++) {
            if (g.s.bag[b][i].count > 0) arrpush(g.bag[b], g.s.bag[b][i]);
        }
    }

    g.mapMeta.npcTable[0] = (Character) {
        true,
        "player3",
        0, 0,
        10,
        DIR_RIGHT,
        16,
        false,
        4
    };

    g.mapMeta.npcTable[1] = (Character) {
        true,
        "player4",
        0, 4,
        0,
        DIR_DOWN,
        0,
        false,
        5
    };
}

// _____________________________________________________________________________
//
//  Load settings file
//  This is done as part of the init process, so settings are loaded before the
//  savegame.
// _____________________________________________________________________________
//
void loadSettings() {
    if (FileExists("settings.tfs")) {
        int unused;
        void *settings = LoadFileData("settings.tfs", &unused);
        memcpy(&g.settings, settings, sizeof(Settings));
    }
    else {
        g.settings.musicVolume = 10;
        g.settings.sfxVolume = 3;
    }

    // Apply the loaded settings
    // synth.gain is in the range 0-10, default 0.2
    #ifndef NO_SYNTH
        fluid_settings_setnum(
            g.syn.settings, "synth.gain",
            (float) g.settings.musicVolume / 40
        );
    #endif
    SetMasterVolume((float) g.settings.sfxVolume / 20);
}

// _____________________________________________________________________________
//
//  Write save file
// _____________________________________________________________________________
//
void save() {
    // Save bag contents (dynamic array -> static array)
    for (int b = 0; b < 3; b++) {
        for (int i = 0; i < arrlen(g.bag[b]); i++) {
            g.s.bag[b][i] = g.bag[b][i];
        }
    }

    SaveFileData("save.tfs", &g.s, sizeof(SaveData));
}

// _____________________________________________________________________________
//
//  Write settings file (also done when the user says "quit without saving")
// _____________________________________________________________________________
//
void saveSettings() {
    SaveFileData("settings.tfs", &g.settings, sizeof(Settings));
}