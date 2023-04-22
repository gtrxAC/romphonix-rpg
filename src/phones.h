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

typedef enum Condition {
    COND_BROKEN,
    COND_BAD,
    COND_OK,
    COND_GOOD
} Condition;

static const char *condToString[4] = {
    "Broken", "Bad", "OK", "Good"
};
static const float condMultipliers[4] = {
    0.5f, 0.8f, 1.0f, 1.2f
};
static const float weightCondMultipliers[4] = {
    // Weight is counted in the opposite way, lower is better,
    // so worse condition = higher weight = worse
    1.5f, 1.2f, 1.0f, 0.8f
};

typedef struct PhoneSpecs {
    // Make sure none of the descriptions exceed the max length (should be well
    // below that, 100 is a good maximum)
    char brand[16];
    char model[16];
    char sprite[32];
    char description[128];
    u32 year;

    u32 battery;
    u32 attack, defense, weight;
    u32 rarity;  // 0 (common) to 5 (rare)
    u32 brokenChance; // how many phones out of 256 are broken (previously one out of this many phones were broken)
    u32 baseExp;
    Icon icon;
} PhoneSpecs;

typedef struct Phone {
    bool active;  // is this phone slot in use?
    u16 id;  // ID of phone from database

    u16 hp, maxHP, energy;
    u8 level, exp, maxExp, baseExp;
    u16 attack, defense, weight;
    u16 skills[4];
    Condition screenStatus, boardStatus, coverStatus, batteryStatus;

    bool confused;
} Phone;

// Data of a phone while it's in battle (this gets cleared when the battle ends,
// includes stuff like stat changes)
typedef struct BattlePhone {

} BattlePhone;

typedef struct PhoneDatabase {
    u32 size;
    PhoneSpecs phones[];
} PhoneDatabase;

#endif