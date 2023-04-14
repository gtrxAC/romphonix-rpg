#include "common.h"

// _____________________________________________________________________________
//
// Initialize the FluidSynth MIDI synth.
// Much of this is copied from https://www.fluidsynth.org/api/example_8c-example.html
// _____________________________________________________________________________
//
void initSynth(Game *g) {
    g->syn.settings = new_fluid_settings();
    if (!g->syn.settings) error(g, "Failed to create FluidSynth settings");

    g->syn.synth = new_fluid_synth(g->syn.settings);
    if (!g->syn.synth) error(g, "Failed to create FluidSynth synth");

    g->syn.sfont_id = fluid_synth_sfload(g->syn.synth, "assets/sounds/soundfont.sf2", 1);
    if (g->syn.sfont_id == FLUID_FAILED) error(g, "Failed to load soundfont. Make sure you are running romphonix.bat, not the exe in the windows folder.");

    g->syn.driver = new_fluid_audio_driver(g->syn.settings, g->syn.synth);
    if (!g->syn.driver) error(g, "Failed to create FluidSynth audio driver");
}

// _____________________________________________________________________________
//
//  Clean up resources used by the synth
// _____________________________________________________________________________
//
void closeSynth(Game *g) {
    fluid_player_stop(g->syn.player);
    delete_fluid_player(g->syn.player);
    delete_fluid_audio_driver(g->syn.driver);
    delete_fluid_synth(g->syn.synth);
    delete_fluid_settings(g->syn.settings);
}

// _____________________________________________________________________________
//
//  Change the song played by the synth
// _____________________________________________________________________________
//
void setSong(Game *g, const char *path) {
    // Remove old player and synth (unless there's a better way to clear the "playlist"?)
    if (g->syn.player) {
        fluid_player_stop(g->syn.player);
        delete_fluid_player(g->syn.player);
        delete_fluid_synth(g->syn.synth);

        g->syn.synth = new_fluid_synth(g->syn.settings);
        if (!g->syn.synth) error(g, "Failed to create FluidSynth synth");

        g->syn.sfont_id = fluid_synth_sfload(g->syn.synth, "assets/sounds/soundfont.sf2", 1);
        if (g->syn.sfont_id == FLUID_FAILED) error(g, "Failed to load soundfont");
    }

    // Create a new player that plays the specified file in an infinite loop
    g->syn.player = new_fluid_player(g->syn.synth);
    fluid_player_add(g->syn.player, path);
    fluid_player_set_loop(g->syn.player, -1);
    fluid_player_play(g->syn.player);
}