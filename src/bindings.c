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

void checkBindings() {
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
            if (!g.s.party[i].active) {
                g.s.party[i].active = true;
                g.s.party[i].id = id;
                g.s.phonesSeen[id] = true;
                g.s.phonesCaught[id] = true;

                g.s.party[i].hp = 100;
                g.s.party[i].maxHP = 100; // still not sure about this
                g.s.party[i].attack = SPECS(id).attack;
                g.s.party[i].defense = SPECS(id).defense;
                g.s.party[i].weight = SPECS(id).weight;
                
                g.s.party[i].level = 40;
                g.s.party[i].baseExp = SPECS(id).baseExp;

                g.s.party[i].screenStatus = GetRandomValue(1, 3);
                g.s.party[i].boardStatus = GetRandomValue(1, 3);
                g.s.party[i].coverStatus = GetRandomValue(1, 3);
                g.s.party[i].batteryStatus = GetRandomValue(1, 3);

                if (GetRandomValue(1, 256) < SPECS(id).brokenChance) {
                    switch (GetRandomValue(0, 3)) {
                        case 0: g.s.party[i].screenStatus = COND_BROKEN; break;
                        case 1: g.s.party[i].boardStatus = COND_BROKEN; break;
                        case 2: g.s.party[i].coverStatus = COND_BROKEN; break;
                        case 3: g.s.party[i].batteryStatus = COND_BROKEN; break;
                    }
                }

                g.s.party[i].skills[0] = 5;
                g.s.party[i].skills[1] = 10;
                g.s.party[i].skills[2] = 11;
                g.s.party[i].skills[3] = 12;

                got = true;
                return;
            }
        }

        if (!got) tinyfd_messageBox("Error", "Your party is full!", "ok", "error", 0);
    }

    // I: give item
    if (IsKeyPressed(KEY_I)) {
        char *input = tinyfd_inputBox("Give item", "What item ID do you want to get? (check assets/data/items.json, the first one is number 0, second is 1, and so on)", " ");
        if (!input) return;
        int id = atoi(input);
        ItemSpecs *item = &ISPECS(id);

        #define POCKET (g.bag[item->pocket])

        // Search if there is already a stack with this item
        for (int i = 0; i < arrlen(POCKET); i++) {
            if (POCKET[i].id == id) {
                POCKET[i].count += 100;
                return;   
            }
        }

        if (arrlen(POCKET) < 20) {
            arrput(POCKET, ((Item) {id, 100}));
        } else {
            tinyfd_messageBox("Error", "The bag pocket is full!", "ok", "error", 0);
        }
    }

    // B: random wild battle
    if (IsKeyPressed(KEY_B)) {
        if (scrBattleMenu(true)) {
            setSong("assets/sounds/music/jht9392remix.mid");
            int id = randomPhone();
            Phone phone = {
                true, id,
                100, 100, 100,
                g.s.party[0].level + GetRandomValue(-5, 5), 0, 0, SPECS(id).baseExp,
                SPECS(id).attack, SPECS(id).defense, SPECS(id).weight,
                {
                    GetRandomValue(1, g.skillDB->size - 1), GetRandomValue(1, g.skillDB->size - 1), 
                    GetRandomValue(1, g.skillDB->size - 1), GetRandomValue(1, g.skillDB->size - 1), 
                },
                GetRandomValue(0, 3), GetRandomValue(0, 3),
                GetRandomValue(0, 3), GetRandomValue(0, 3)
            };
            MENU.enemyActive = 0;
            MENU.enemyParty[0] = phone;
            MENU.enemyName = "Enemy";
            MENU.enemyDefeatMsg[0] = "you won, congrats i guess???";
            strcpy(
                MENU.battleTextbox[0],
                F("A wild %s %s appeared!", SPECS(id).brand, SPECS(id).model)
            );
            MENU.battleTextbox[1][0] = '\0';
            MENU.battleTextbox[2][0] = '\0';
        }
    }

    // N: NPC battle
    if (IsKeyPressed(KEY_N)) {
        if (scrBattleMenu(false)) {
            setSong("assets/sounds/music/jht9392remix.mid");
            int id = randomPhone();
            Phone phone1 = {
                true, id,
                100, 100, 100,
                g.s.party[0].level + GetRandomValue(-5, 5), 0, 0, SPECS(id).baseExp,
                SPECS(id).attack, SPECS(id).defense, SPECS(id).weight,
                {
                    GetRandomValue(1, g.skillDB->size - 1), GetRandomValue(1, g.skillDB->size - 1), 
                    GetRandomValue(1, g.skillDB->size - 1), GetRandomValue(1, g.skillDB->size - 1), 
                },
                GetRandomValue(0, 3), GetRandomValue(0, 3),
                GetRandomValue(0, 3), GetRandomValue(0, 3)
            };
            Phone phone2 = {
                true, id + 1,
                100, 100, 100,
                g.s.party[0].level + GetRandomValue(-5, 5), 0, 0, SPECS(id).baseExp,
                SPECS(id).attack, SPECS(id).defense, SPECS(id).weight,
                {
                    GetRandomValue(1, g.skillDB->size - 1), GetRandomValue(1, g.skillDB->size - 1), 
                    GetRandomValue(1, g.skillDB->size - 1), GetRandomValue(1, g.skillDB->size - 1), 
                },
                GetRandomValue(0, 3), GetRandomValue(0, 3),
                GetRandomValue(0, 3), GetRandomValue(0, 3)
            };
            MENU.enemyActive = 0;
            MENU.enemyParty[0] = phone1;
            MENU.enemyParty[1] = phone2;
            MENU.enemyName = "NAME";
            MENU.enemySprite = "large_player4";
            MENU.enemyReward = 100;
            MENU.enemyDefeatMsg[0] = "This is line 1 of my defeat message.";
            MENU.enemyDefeatMsg[1] = "This is line 2 of my defeat message.";
            strcpy(
                MENU.battleTextbox[0],
                "NAME has challenged you to a battle!"
            );
            MENU.battleTextbox[1][0] = '\0';
            MENU.battleTextbox[2][0] = '\0';
        }
    }

    // T: reload a texture (useful for testing art w/o restarting game)
    if (IsKeyPressed(KEY_T)) {
        char *name = tinyfd_inputBox("Texture reload", "What texture name do you want to reload? (see LOAD_TEXTUREs in init.c)", " ");
        if (!name) return;

        UnloadTexture(shget(g.textures, name));
        shput(g.textures, name, LoadTexture(F("assets/graphics/%s.png", name)));
    }
}