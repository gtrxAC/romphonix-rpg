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
    }
};

#endif