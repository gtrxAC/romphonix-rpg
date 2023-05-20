// _____________________________________________________________________________
//
//  Settings menu
//  Allows the player to change basic game options, like sound volume. The menu
//  items cannot be selected, instead the values are changed by scrolling left
//  and right.
// _____________________________________________________________________________
//
#include "../common.h"

void scrSettingsMenu(Game *g);
void updateSettingsMenu(Game *g);
void drawSettingsMenu(Game *g);

// _____________________________________________________________________________
//
//  Settings menu - init function
// _____________________________________________________________________________
//
void scrSettingsMenu(Game *g) {
    pushMenu(g, 0, NULL, CB_CLOSE);
    MENU.updateFunc = updateSettingsMenu;
    MENU.drawFunc = drawSettingsMenu;
    arrpush(MENU.choices, "Music volume");
    arrpush(MENU.choices, "Sound effect volume");
}

// _____________________________________________________________________________
//
//  Settings menu - state update function
// _____________________________________________________________________________
//
void updateSettingsMenu(Game *g) {
    // The scrolling behavior is based on the basic menu. Selecting items
    // doesn't do anything because a check function isn't set.
    updateMenu(g);

    int delta = 0;
    if (K_LEFT_PRESS()) delta = -1;
    else if (K_RIGHT_PRESS()) delta = 1;

    if (delta) switch (MENU.choice) {
        case 0: {
            if (
                (delta == -1 && g->settings.musicVolume > 0) ||
                (delta == 1 && g->settings.musicVolume < 10)
            ) {
                g->settings.musicVolume += delta;
            }
            fluid_settings_setnum(
                g->syn.settings, "synth.gain",
                (float) g->settings.musicVolume / 20
            );
            break;
        }

        case 1: {
            if (
                (delta == -1 && g->settings.sfxVolume > 0) ||
                (delta == 1 && g->settings.sfxVolume < 10)
            ) {
                g->settings.sfxVolume += delta;
            }
            SetMasterVolume((float) g->settings.sfxVolume / 10);
            StopSound(SOUND(scroll));
            schedSound(g, SOUND(select), 2);
            break;
        }
    }
}

// _____________________________________________________________________________
//
//  Settings menu - draw function
// _____________________________________________________________________________
//
void drawSettingsMenu(Game *g) {
    // Top box - settings list
    drawBox(g, 0, 0, 320, 210);

    // Draw choices and indicator (copied from basic menu in menu.c)
    for (int i = 0; i < arrlen(MENU.choices) && MENU.choices[i]; i++) {
        DrawTextEx(
            g->fonts.dialogue, MENU.choices[i],
            (Vector2) {22, 8 + 14*i},
            13, 0, WHITE
        );
    }
    DrawTexture(TEX(indicator), 8, 8 + 14*MENU.choice, WHITE);

    // Draw values (right aligned)
    const char *volStr = TextFormat("%d / 10", g->settings.musicVolume);
    int width = measureText(g, volStr);
    drawText(g, volStr, 312 - width, 8, WHITE);

    volStr = TextFormat("%d / 10", g->settings.sfxVolume);
    width = measureText(g, volStr);
    drawText(g, volStr, 312 - width, 22, WHITE);

    // Bottom box - help text
    drawBox(g, 0, 210, 320, 30);
    width = measureText(g, "Use left and right arrows to change value");
    drawText(g, "Use left and right arrows to change value", 160 - width/2, 218, WHITE);
}