#ifndef PHONES_H
#define PHONES_H

#include "common.h"

// _____________________________________________________________________________
//
//  Phone structure
//  Note: PhoneSpecs is the constant data in the phone database, Phone is a
//  phone that the player owns in their party or PC.
// _____________________________________________________________________________
//
// Icon indexing is based on assets/graphics/phone_icons.png, top to bottom
typedef enum Icon {
    IC_CANDYBAR,
    IC_NEW_CANDYBAR,
    IC_SLIDER,
    IC_FLIP,
    IC_FLIP_COVER,
    IC_QWERTY,
    IC_GX55X,  // Gxphone (up for deletion)
    IC_3650,  // Nokia
    IC_N_GAGE,  // Nokia
    IC_P900,  // SE
    IC_COMMUNICATOR,  // Nokia
    IC_XELIBRI_8,  // Siemens
    IC_77XX,  // Nokia
    IC_SX1  // Siemens
} Icon;

typedef struct PhoneSpecs {
    // Make sure none of the descriptions exceed the max length (should be well
    // below that, 100 is a good maximum)
    char brand[16];
    char model[32];
    char description[128];
    
    u16 year;
    bool fictional;

    u16 battery;
    u16 attack, defense, weight;
    u8 rarity;  // 0 (common) to 5 (rare)
    float brokenChance; // 0 to 1
    u8 baseExp;
    Icon icon;
} PhoneSpecs;

typedef struct Phone {
    unsigned short id;  // ID of phone from database
    char nickname[16];

    u16 hp, maxHP, energy;
    u8 level, exp, maxExp, baseExp;
    u16 attack, defense, weight;
    u16 skills[4];
    u8 screenStatus, boardStatus, coverStatus, batteryStatus;
} Phone;

// _____________________________________________________________________________
//
//  Phone database
// _____________________________________________________________________________
//

#endif