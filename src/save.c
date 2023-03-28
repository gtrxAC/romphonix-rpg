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