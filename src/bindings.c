// _____________________________________________________________________________
//
//  Bindings
//  Checks key bindings every frame and reacts to them. These bindings are
//  global, so they can be run at any point in the game (title, overworld,
//  battle, etc).
//
//  These are implemented as a single function that is run as part of the main
//  loop (see main.c).
// _____________________________________________________________________________
//
#include "common.h"

void checkBindings(Game *g) {
    // Screen scaling with page up/down
    int scale = GetScreenWidth() / 320;
    if (IsKeyPressed(KEY_PAGE_UP) && scale < 6) {
        SetWindowSize(320*(scale + 1), 240*(scale + 1));
    }
    if (IsKeyPressed(KEY_PAGE_DOWN) && scale > 1) {
        SetWindowSize(320*(scale - 1), 240*(scale - 1));
    }

    // G: Give phone
    if (IsKeyPressed(KEY_G)) {
        char *input = tinyfd_inputBox("Give phone", "What phone ID do you want to get? (check assets/data/phones.json, the first one is number 0, second is 1, and so on)", " ");
        if (!input) return;
        int id = atoi(input);

        bool got = false;
        for (int i = 0; i < 6; i++) {
            if (!g->s.party[i].active) {
                g->s.party[i].active = true;
                g->s.party[i].id = id;

                g->s.party[i].hp = 50;
                g->s.party[i].maxHP = 100; // still not sure about this
                g->s.party[i].attack = SPECS(id).attack;
                g->s.party[i].defense = SPECS(id).defense;
                g->s.party[i].weight = SPECS(id).weight;
                
                g->s.party[i].level = 40;
                g->s.party[i].baseExp = SPECS(id).baseExp;

                g->s.party[i].screenStatus = GetRandomValue(1, 3);
                g->s.party[i].boardStatus = GetRandomValue(1, 3);
                g->s.party[i].coverStatus = GetRandomValue(1, 3);
                g->s.party[i].batteryStatus = GetRandomValue(1, 3);

                if (GetRandomValue(1, 256) < SPECS(id).brokenChance) {
                    switch (GetRandomValue(0, 3)) {
                        case 0: g->s.party[i].screenStatus = COND_BROKEN; break;
                        case 1: g->s.party[i].boardStatus = COND_BROKEN; break;
                        case 2: g->s.party[i].coverStatus = COND_BROKEN; break;
                        case 3: g->s.party[i].batteryStatus = COND_BROKEN; break;
                    }
                }

                got = true;
                return;
            }
        }

        if (!got) tinyfd_messageBox("Error", "Your party is full!", "ok", "error", 0);
    }
}