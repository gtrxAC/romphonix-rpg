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
}

// _____________________________________________________________________________
//
//  Write save file
// _____________________________________________________________________________
//
void save(Game *g) {
    WriteFileData("save.tfs", &g->s, sizeof(SaveData));
}