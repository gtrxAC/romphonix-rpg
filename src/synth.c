#include "common.h"

// _____________________________________________________________________________
//
// Initialize the FluidSynth MIDI synth.
// Much of this is copied from https://www.fluidsynth.org/api/example_8c-example.html
// _____________________________________________________________________________
//
void initSynth() {
    g.syn.settings = new_fluid_settings();
    if (!g.syn.settings) {
        error("Failed to create FluidSynth settings", false);
        return;
    }

    fluid_settings_setint(g.syn.settings, "synth.threadsafe-api", 0);
    fluid_settings_setint(g.syn.settings, "player.reset-synth", 0);

    g.syn.synth = new_fluid_synth(g.syn.settings);
    if (!g.syn.synth) {
        error("Failed to create FluidSynth synth", false);
        return;
    }

    g.syn.sfont_id = fluid_synth_sfload(g.syn.synth, "assets/sounds/soundfont.sf2", 1);
    if (g.syn.sfont_id == FLUID_FAILED) {
        error("Failed to load soundfont. Make sure you are running romphonix_win.bat or romphonix_linux.sh, not the files in the bin folder.", false);
        return;
    }

    g.syn.driver = new_fluid_audio_driver(g.syn.settings, g.syn.synth);
    if (!g.syn.driver) {
        error("Failed to create FluidSynth audio driver", false);
        return;
    }

    g.syn.loaded = true;
}

// _____________________________________________________________________________
//
//  Clean up resources used by the synth
// _____________________________________________________________________________
//
void closeSynth() {
    if (g.syn.player) {
        fluid_player_stop(g.syn.player);
        delete_fluid_player(g.syn.player);
    }

    if (g.syn.driver) delete_fluid_audio_driver(g.syn.driver);
    if (g.syn.sfont_id) fluid_synth_sfunload(g.syn.synth, g.syn.sfont_id, 1);
    if (g.syn.synth) delete_fluid_synth(g.syn.synth);
    if (g.syn.settings) delete_fluid_settings(g.syn.settings);
}

// _____________________________________________________________________________
//
//  Change the song played by the synth
// _____________________________________________________________________________
//
void setSong(const char *path) {
    // Ignore music changes if the synth failed to open
    if (!g.syn.loaded) return;

    // Delete old player
    // https://github.com/aburch/simutrans/blob/master/src/simutrans/music/fluidsynth.cc
    if (g.syn.player) {
        fluid_player_stop(g.syn.player);
        fluid_player_join(g.syn.player);
        fluid_synth_all_notes_off(g.syn.synth, -1);
        delete_fluid_player(g.syn.player);
    }

    fluid_synth_system_reset(g.syn.synth);

    // Create a new player that plays the specified file in an infinite loop
    g.syn.player = new_fluid_player(g.syn.synth);
    fluid_player_add(g.syn.player, path);
    fluid_player_set_loop(g.syn.player, -1);
    fluid_player_play(g.syn.player);
}