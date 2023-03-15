# ROMphonix RPG - Codebase structure and technical details

## Build system
The [gxbuild](https://github.com/gtrxAC/gxbuild) build system is used to compile this game. It consists of three unix shell scripts:
* `build.sh` compiles the game and does basic maintenance tasks, like [phone list conversion](#phones).
* `config.sh` contains configuration data, like the project name. This shouldn't be run as a script, it is loaded by the other scripts.
* `setup.sh` is used to set up the build environment. Usually this only needs to be done once after downloading the source code.

[w64devkit](https://github.com/skeeto/w64devkit/releases) is used to run unix shell scripts on Windows, so it's needed if you're on Windows. Basic build instructions are in the [README](README.md).

gxbuild also supports building for Android and Web, but these platforms have their own quirks with raylib, so they are not supported by this game for now.

## Graphics
The game is mostly powered by [raylib](https://www.raylib.com), a simple game library. It is used for window creation, graphics, input (and sound effects in the future?).

[src/util.c](src/util.c) contains a few functions to make it easier to use, providing shorthands for common tasks like drawing text with a custom font rather than raylib's default font.

raylib does not provide native style message box windows, for that TinyFileDialogs is used.

## Music
FluidSynth is used for playing music using the MIDI sequence format. A small framework is provided in [src/synth.c](src/synth.c) to allow changing songs and playing them in a (hopefully seamless) loop.

### Soundfonts
One SF2 soundfont is used for all tracks in the game (for now that is the "Nocturne" soundfont, we will make a custom one later). Soundfonts define samples and instruments used for playing MIDI files.

## Maps
Maps in ROMphonix RPG consist of two files, .H and .RPX.

.H is a C header file defining the map's metadata and script functions related to the map. This can be edited with any text editor. Headers are compiled into the game executable, unlike the actual map data.

.RPX is a custom binary format for defining the map data (background/foreground tiles, collisions, tiles containing script triggers). It can be edited using the included map editor.

## Phones
The list of phones in the game is a JSON file: [assets/data/phones.json](assets/data/phones.json). This can be edited with any text editor, but the JSON isn't actually used in the game.

### TFS file
To apply any changes done to the JSON file, a TFS file needs to be created of it. TFS is another binary format used by this game. The conversion is handled by the Python script [assets/data/convert_phones.py](assets/data/convert_phones.py), which is automatically run by the [build system](#build-system).

### Phone Sprites
Each phone has a sprite assigned to it, the name of the sprite file for each phone is in the JSON. For consistency, the file name should follow the format `Brand_Model`, where spaces and dashes are replaced by underscores.

Sprites should be 64x64 PNGs with transparency and a black border around the phone.

## Folder Structure
Click any of the links to see more information about that subject.
* `assets`: Contains files used by the game, such as images and sounds.
  * `data`: Any game data files that aren't media, e.g. [phones](#phones).
  * `fonts`: Fonts used for drawing text. These are PNG images with purple lines around each character, a format supported by raylib.
  * `graphics`: Any other images used by the game.
    * `phones`: [Phone sprites](#phone-sprites).
  * `maps`: [Map](#maps) files.
  * `sounds`: Any audio files used by the game, as well as the [soundfont](#soundfonts).
    * `music`: [Music](#music) in MIDI format.
* `gxrpg`: Source code for gtrxAC's old RPG demo, some of the game's code is copied from that. This folder is deleted when it's no longer needed.
* `include`: C headers for libraries used by the game.
* `lib`: Libraries used by the game.
* `mapeditor`: Source code for the map editor, this is compiled alongside the actual game.
* `src`: [Source code for the game](#codebase-structure).