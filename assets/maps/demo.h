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
void scrDemoDoor(Game *g) {
    PlaySound(SOUND(door));
    changeMap(g, 1, 5, 8);
}

void scrDemoGrass(Game *g) {

}

// Interact scripts
void scrDemoSign(Game *g) {
    pushTextbox(g, "This is a sign!", "");
    MENU.nextFunc = popMenu;
}

void scrDemoMailbox(Game *g) {
    pushTextbox(g, "You have no mail.", "");
    MENU.nextFunc = popMenu;
}

void scrDemoClock(Game *g) {
    pushTextbox(g, "The time is... I don't know, I don't care", "enough to check.");
    MENU.nextFunc = popMenu;
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
    "assets/sounds/music/shakey_snakey.mid",

    // List of step script functions (executed when player steps on a tile)
    // Each tile has a step/interact script value from 0-255, 0 means no script
    // These can be edited in the map editor by pressing 5 or 6.
    // The first one in this list is executed for any tile that has a value of 1
    {
        scrDemoDoor,
        scrDemoGrass
    },

    // Same but for interact scripts (executed when player interacts with the tile)
    {
        scrDemoSign,
        scrDemoMailbox,
        scrDemoClock
    }
};

#endif