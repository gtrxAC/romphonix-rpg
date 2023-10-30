// _____________________________________________________________________________
//
//  This can be used as a template for creating a map header file.
// _____________________________________________________________________________
//
// Change this to your map's name.
#ifndef DEMO_H
#define DEMO_H

#include "../src/common.h"

// _____________________________________________________________________________
//
//  Scripts
//  Put your script functions here, prefix them with with scr and the map name
// _____________________________________________________________________________
//
// If scripts are referenced using MENU.nextFunc, they have to be declared first

// Step scripts
void scrDemoDoor() {
    playSound("door");
    changeMap(1, 5, 8);
}

// Interact scripts
void scrDemoSign() {
    pushTextbox("This is a sign!", "");
}

void scrDemoMailbox() {
    pushTextbox("You have no mail.", "");
}

void scrDemoClock() {
    pushTextbox("The time is... I don't know, I don't care", "enough to check.");
}

void scrDemoNpc(Character *npc) {
    pushTextbox(F("Hi %s!", g.s.name), "I'm a generic NPC!");
}

void scrDemoNpc2(Character *npc) {
    pushTextboxMenu();
    strcpy(MENU.textbox[0], "You wanted to battle, right?");
    addChoice("Yes");
    addChoice("No");
    MENU.nextFunc = scrDemoNpc3;
}

void scrDemoNpc3(Character *npc) {
    int choice = MENU.choice;
    popMenu();
    if (choice == 0 && scrBattleMenu(false)) {
        setSong("assets/sounds/music/jht9392remix.mid");
        int id = 50;
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
        MENU.enemyActive = 0;
        MENU.enemyParty[0] = phone1;
        MENU.enemyName = "Random NPC";
        MENU.enemySprite = "large_player4";
        MENU.enemyReward = 1000;
        MENU.enemyDefeatMsg[0] = "This is line 1 of my defeat message.";
        MENU.enemyDefeatMsg[1] = "This is line 2 of my defeat message.";
        strcpy(
            MENU.battleTextbox[0],
            "Random NPC challenged you to a battle!"
        );
        MENU.battleTextbox[1][0] = '\0';
        MENU.battleTextbox[2][0] = '\0';
    } else {
        pushTextbox("ok", "");
    }
}

void scrDemoNpcUpdateTest(Character *npc) {
    Direction dir = (Direction) GetRandomValue(0, 3);
    moveCharacter(npc, dir, 1);
}

// _____________________________________________________________________________
//
//  Map metadata
// _____________________________________________________________________________
//
Map mapDemo = {
    // Name of the map, is shown to the user in some situations.
    "Demo",

    // Path to the map, should include the full path looking from the root folder
    "assets/maps/demo.rpx",

    // Path to the song used by the map, also looking from the root
    "assets/sounds/music/sneakmission.mid",

    // The battle background image used in this map. (assets/graphics/)
    "battle/field",

    // List of step script functions (executed when player steps on a tile)
    // Each tile has a step/interact script value from 0-255, 0 means no script
    // These can be edited in the map editor by pressing 5 or 6.
    // The first one in this list is executed for any tile that has a value of 1
    {
        scrWildEncounter,
        scrDemoDoor,
    },

    // Same but for interact scripts (executed when player interacts with the tile)
    // This list doesn't need to include NPC interact scripts, those are defined for
    // each NPC below (in the NPC table)
    {
        scrDemoSign,
        scrDemoMailbox,
        scrDemoClock
    },

    // Chance to encounter a wild phone when scrWildEncounter is run in this
    // map. This is X out of 255.
    25,

    // Wild encounter table used by this map. The table should have up to 16
    // pairs of values. The first value of a pair is the phone ID and the second
    // value is the weight value of how often that phone should appear compared
    // to other phones.
    {
        {1, 1}, {2, 3}
    },

    // NPC table used by this map. See character.h for the structure.
    // 'active' should always be true.
    {
        {
            /* active */ true,
            /* sprite */ "player3",
            /* location */ 0, 0,
            /* direction */ DIR_RIGHT,
            /* updateScript */ NULL,
            /* interactScript */ scrDemoNpc
        },
        {
            /* active */ true,
            /* sprite */ "player4",
            /* location */ 0, 4,
            /* direction */ DIR_DOWN,
            /* updateScript */ scrDemoNpcUpdateTest,
            /* interactScript */ scrDemoNpc2
        }
    }
};

#endif