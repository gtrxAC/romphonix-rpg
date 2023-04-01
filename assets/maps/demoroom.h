// _____________________________________________________________________________
//
//  This can be used as a template for creating a map header file.
// _____________________________________________________________________________
//
// Change this to your map's name.
#ifndef DEMOROOM_H
#define DEMOROOM_H

#include "../src/common.h"

// _____________________________________________________________________________
//
//  Scripts
//  Put your script functions here, prefix them with with scr and the map name
// _____________________________________________________________________________
//
// If scripts are referenced using MENU.nextFunc, they have to be declared first

// Step scripts
void scrDemoRoomDoor(Game *g) {
    changeMap(g, 0, 9, 8);
}

// Interact scripts

// _____________________________________________________________________________
//
//  Map metadata
// _____________________________________________________________________________
//
Map mapDemoRoom = {
    // Name of the map, is shown to the user in some situations.
    "Demo Room",

    // Path to the map, should include the full path looking from the root folder
    "assets/maps/demoroom.rpx",

    // List of step script functions (executed when player steps on a tile)
    // Each tile has a step/interact script value from 0-255, 0 means no script
    // These can be edited in the map editor by pressing 5 or 6.
    // The first one in this list is executed for any tile that has a value of 1
    {
        scrDemoRoomDoor
    },

    // Same but for interact scripts (executed when player interacts with the tile)
    {
    }
};

#endif