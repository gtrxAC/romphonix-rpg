#ifndef SKILLS_H
#define SKILLS_H

#include "common.h"

// Should match the structure and values generated by assets/data/json2tfs.py

typedef enum SkillType {
    SKT_SCREEN,
    SKT_BOARD,
    SKT_COVER,
    SKT_BATTERY
} SkillType;

static const char *skillTypes[4] = {
    "Screen", "Board", "Cover", "Battery"
};

typedef enum SkillEffect {
    SE_NONE,  // Does nothing (e.g. the secondary effect if the skill only has one effect)
    SE_DAMAGE,  // Does X damage to the enemy (X being the effect parameter)
    SE_DRAIN,  // Drains X health from the enemy and gives it to the user
    SE_CONFUSE,  // Confuses the enemy with a X% chance
    SE_ATK_UP,  // Increases the user's attack by X levels
    SE_DEF_UP  // Increases the user's defense by X levels
} SkillEffect;

typedef struct SkillSpecs {
    char name[16];
    char description[128];
    SkillType type;

    struct {
        SkillEffect effect;
        int parameter;
        int chance;
    } effects[2];

    char learnByPhones[256];
    char animation[64];
} SkillSpecs;

typedef struct SkillDatabase {
    u32 size;
    SkillSpecs skills[];
} SkillDatabase;

#endif