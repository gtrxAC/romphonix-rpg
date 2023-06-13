
// _____________________________________________________________________________
//
//  Use item menu
//  This menu is part of the bag menus, it uses the draw function
//  of phonesmenu.c, but a custom check function to use the item on the selected
//  phone.
// _____________________________________________________________________________
//
#include "../common.h"

// _____________________________________________________________________________
//
//  Use item menu - init function
// _____________________________________________________________________________
//
void scrUseItemMenu(int bagPocket, int item) {
    pushMenu(CB_CLOSE);
    setDrawFunc(drawPhonesMenu);
    setNextFunc(scrCheckUseItemMenu);
    MENU.itemBagPocket = bagPocket;
    MENU.item = item;

    // For this menu, we create 6 "dummy" values to let the user scroll freely.
    for (int i = 0; i < 6; i++) addChoice("");
}

// _____________________________________________________________________________
//
//  Use item menu - check user input function
// _____________________________________________________________________________
//
void scrCheckUseItemMenu() {
    Phone *phone = &g.s.party[MENU.choice];
    Item *item = &g.bag[MENU.itemBagPocket][MENU.item];

    // Don't allow use on empty phone slots
    if (!phone->active) return;

    popMenu();

    switch (ISPECS(item->id).effect) {
        case IE_HEAL: {
            if (phone->hp == phone->maxHP) {
                pushTextbox(
                    F(
                        "%s %s is already at full HP!",
                        SPECS(phone->id).brand, SPECS(phone->id).model
                    ), ""
                );
                return;
            }
            else {
                int heal = MIN(ISPECS(item->id).effectParameter, phone->maxHP - phone->hp);
                phone->hp += heal;
                schedSound("heal", 10);
                pushTextbox(
                    F(
                        "Healed %s %s by %d HP.",
                        SPECS(phone->id).brand, SPECS(phone->id).model, heal
                    ), ""
                );
            }
            break;
        }

        case IE_REVIVE: {
            if (phone->hp <= 0) {
                phone->hp = phone->maxHP * ISPECS(item->id).effectParameter / 100;
                schedSound("heal", 10);
                pushTextbox(
                    F(
                        "%s %s has been revived.",
                        SPECS(phone->id).brand, SPECS(phone->id).model
                    ), ""
                );
            }
            else {
                pushTextbox("This phone is already alive!", "");
                return;
            }
            break;
        }

        case IE_REPAIR: {
            scrRepairMenu(phone, ISPECS(item->id).effectParameter);
            break;
        }

        case IE_SKILL: {
            scrTeachSkillMenu(phone, ISPECS(item->id).effectParameter);
            break;
        }
    }
    
    // Decrease the item count and remove the item's entry if count dropped to 0
    // The function would have already returned if item use failed (e.g. healing when already full hp)
    item->count--;
    if (item->count < 1) item->id = 0;
}