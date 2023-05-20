
// _____________________________________________________________________________
//
//  Use item menu
//  This menu is part of the bag menus, it uses the draw function
//  of phonesmenu.c, but a custom check function to use the item on the selected
//  phone.
// _____________________________________________________________________________
//
#include "../common.h"

void scrUseItemMenu(Game *, int, int);
void drawPhonesMenu(Game *);
void exitUseItemMenu(Game *g);
void scrCheckUseItemMenu(Game *);

// _____________________________________________________________________________
//
//  Use item menu - init function
// _____________________________________________________________________________
//
void scrUseItemMenu(Game *g, int bagPocket, int item) {
    pushMenu(g, 6, NULL, CB_CLOSE);
    MENU.drawFunc = drawPhonesMenu;
    MENU.nextFunc = scrCheckUseItemMenu;
    MENU.itemBagPocket = bagPocket;
    MENU.item = item;

    // For this menu, we create 6 "dummy" values to let the user scroll freely.
    for (int i = 0; i < 6; i++) arrput(MENU.choices, "");
}

// _____________________________________________________________________________
//
//  Use item menu - check user input function
// _____________________________________________________________________________
//
void scrCheckUseItemMenu(Game *g) {
    Phone *phone = &g->s.party[MENU.choice];
    Item *item = &g->bag[MENU.itemBagPocket][MENU.item];

    // Don't allow use on empty phone slots
    if (!phone->active) return;

    popMenu(g);

    switch (ISPECS(item->id).effect) {
        case IE_HEAL: {
            if (phone->hp == phone->maxHP) {
                pushTextbox(
                    g, TextFormat(
                        "%s %s is already at full HP!",
                        SPECS(phone->id).brand, SPECS(phone->id).model
                    ), ""
                );
                MENU.nextFunc = popMenu;
                return;
            }
            else {
                int heal = MIN(ISPECS(item->id).effectParameter, phone->maxHP - phone->hp);
                phone->hp += heal;
                schedSound(g, SOUND(heal), 10);
                pushTextbox(
                    g, TextFormat(
                        "Healed %s %s by %d HP.",
                        SPECS(phone->id).brand, SPECS(phone->id).model, heal
                    ), ""
                );
                MENU.nextFunc = popMenu;
            }
            break;
        }

        case IE_REVIVE: {
            if (phone->hp <= 0) {
                phone->hp = phone->maxHP * ISPECS(item->id).effectParameter / 100;
                schedSound(g, SOUND(heal), 10);
                pushTextbox(
                    g, TextFormat(
                        "%s %s has been revived.",
                        SPECS(phone->id).brand, SPECS(phone->id).model
                    ), ""
                );
                MENU.nextFunc = popMenu;
            }
            else {
                pushTextbox(g, "This phone is already alive!", "");
                MENU.nextFunc = popMenu;
                return;
            }
            break;
        }

        case IE_REPAIR: {
            scrRepairMenu(g, phone, ISPECS(item->id).effectParameter);
            break;
        }
    }
    
    // Decrease the item count and remove the item's entry if count dropped to 0
    // The function would have already returned if item use failed (e.g. healing when already full hp)
    item->count--;
    if (item->count < 1) item->id = 0;
}