// _____________________________________________________________________________
//
//  This can be used as a template for creating a map header file.
// _____________________________________________________________________________
//
// Change this to your map's name.
#ifndef DEMO_H
#define DEMO_H

#include "common.h"
// _____________________________________________________________________________
//
//  Scripts
//  Put your script functions here, prefix them with with scr and the map name
// _____________________________________________________________________________
//
// Step scripts
void scrDemoDoor(Game *g) {

}
void scrDemoGrass(Game *g) {

}

// Interact scripts
void scrDemoSign(Game *g) {
    
}

void scrDemoMailbox(Game *g) {
    
}

void scrDemoClock(Game *g) {
    
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

    // List of step script functions (executed when player steps on a tile)
    // Each tile has a step/interact script value from 0-255, 0 means no script
    // These can be edited in the map editor by pressing 4 or 5.
    // The first one in this list is executed for any tile that has a value of 1
    {

    },

    // Same but for interact scripts (executed when player interacts with the tile)
    {

    }
}

#endif