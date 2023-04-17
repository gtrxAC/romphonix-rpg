// _____________________________________________________________________________
//
//  Battle menu
//  Functions to implement the functionality of the battle menu. In a battle,
//  the player fights with their phone(s) against an NPC's phone(s) or a wild
//  phone.
//  
//  The player and NPC take turns attacking the other side's phone, they can
//  also switch to a different phone or use an item from their bag.
//  In a wild battle, the player can also "Run" to end the battle.
// _____________________________________________________________________________
//
#include "../common.h"

void scrBattleMenu(Game *g);
void updateBattleMenu(Game *g);
void drawBattleMenu(Game *g);
void checkBattleMenu(Game *g);

// _____________________________________________________________________________
//
//  Battle menu - init function
// _____________________________________________________________________________
//
void scrBattleMenu(Game *g) {
    pushMenu(g, 0, NULL, CB_CLOSE);
    strcpy(MENU.battleTextbox, "Testing");
    MENU.updateFunc = updateBattleMenu;
    MENU.drawFunc = drawBattleMenu;
    MENU.nextFunc = checkBattleMenu;
}

// _____________________________________________________________________________
//
//  Battle menu - do move (helper function)
// _____________________________________________________________________________
//
void doMove(Game *g, Phone *attacker, Phone *victim, SkillSpecs skill) {
    for (int i = 0; i < 2; i++) {
        switch (skill.effects[i].effect) {
            case SE_NONE: break;

            case SE_DAMAGE: {

            }
        }
    }
}

// _____________________________________________________________________________
//
//  Battle menu - state update function
// _____________________________________________________________________________
//
void updateBattleMenu(Game *g) {
    if (MENU.battleState == BS_WAITING) {
        // Command menu is just a standard menu
        updateMenu(g);
    }
    // For any other state, the menu only updates when the confirm button is
    // pressed.
    else if (K_A_PRESS() || K_B_PRESS()) {
        // if (BS_)
    }
}

// _____________________________________________________________________________
//
//  Battle menu - draw function
// _____________________________________________________________________________
//
void drawBattleMenu(Game *g) {
    Phone *playerPh = &g->s.party[MENU.active];
    Phone *enemyPh = &MENU.enemyParty[MENU.enemyActive];

    DrawTexture(TEX(battle_bg), 0, 0, WHITE);

    // Battle menu contains a one line text box (without a typewriter effect)
    drawBox(g, 10, 210, 300, 30);
    drawText(g, MENU.battleTextbox, 18, 218, WHITE);

    // Player status bar
    drawBox(g, 4, 4, 152, 58);
    drawText(g, g->s.name, 10, 10, LIGHTGRAY);

    // Note: '$' character in the digits font (drawTextD) says 'Lv.'
    drawTextD(g, TextFormat("$ %d", playerPh->level), 120, 10, WHITE);

    const char *brandModel = TextFormat(
        "%s %s", SPECS(playerPh->id).brand, SPECS(playerPh->id).model
    );
    drawText(g, brandModel, 10, 25, WHITE);
    drawProgressBar(g, playerPh->hp, playerPh->maxHP, 10, 42, 80, GREEN);
    drawTextD(g, TextFormat("%d/%d", playerPh->hp, playerPh->maxHP), 98, 42, WHITE);

    // Enemy status bar
    drawBox(g, 164, 4, 152, 58);
    drawText(g, MENU.enemyName, 170, 10, LIGHTGRAY);
    drawTextD(g, TextFormat("$ %d", enemyPh->level), 280, 10, WHITE);

    brandModel = TextFormat(
        "%s %s", SPECS(enemyPh->id).brand, SPECS(enemyPh->id).model
    );
    drawText(g, brandModel, 170, 25, WHITE);
    drawProgressBar(g, enemyPh->hp, enemyPh->maxHP, 170, 42, 80, GREEN);
    drawTextD(g, TextFormat("%d/%d", enemyPh->hp, enemyPh->maxHP), 258, 42, WHITE);
    drawTextD(g, TextFormat("$ %d", playerPh->level), 120, 10, WHITE);

    // Player phone sprite
    DrawTexture(shget(g->textures, SPECS(playerPh->id).sprite), 48, 96, WHITE);

    // Enemy phone sprite
    DrawTexture(shget(g->textures, SPECS(enemyPh->id).sprite), 240, 96, WHITE);
}

// _____________________________________________________________________________
//
//  Battle menu - check user input function
// _____________________________________________________________________________
//
void checkBattleMenu(Game *g) {

}