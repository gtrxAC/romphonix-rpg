// _____________________________________________________________________________
//
//  ROMphonix RPG - character.h
//
//  Contains the definition of the Character structure, which represents a
//  player/NPC sprite on the overworld map.
// _____________________________________________________________________________
//
#ifndef CHARACTER_H
#define CHARACTER_H

#include "common.h"

// Used for both the player (SaveData, g.s.chr) and NPCs. These are drawn in world.c.
// NPC data is stored in map metadata as a Character structure, same for the player location in the save file.
typedef struct Character {
    bool active;  // for the NPC table in the map metadata, determines if this slot is in use
    char sprite[32];
    u8 x, y;
    Direction dir;

    // Map interact script that will be executed when interacting with this character (NPC)
    void (*interactScript)();

    // _____________________________________________________________________________
    //
    //  Do not use the below fields for NPCs! They should be left undefined in
    //  map header files.
    // _____________________________________________________________________________
    //
    // movement count (how many tiles the character is about to move, used for the movement animation)
    u8 moveCount;
    
    // movement animation timer (i.e. player won't instantly move to the next tile,
    // it will slide from the prev tile to create a more natural movement)
    u8 anim;
    
    bool isPlayer;  // determines whether the movement animation can be sped up by pressing B
} Character;

#endif