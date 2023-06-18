# ROMphonix RPG
A phone collecting RPG! (in early development)

* [Discord](https://discord.gg/qQ2yCmynCK)
* [Development Wiki](https://github.com/gtrxAC/romphonix-rpg/wiki)
* [Project wall](https://github.com/users/gtrxAC/projects/1)

<div style="display: flex; gap: 1em; margin-bottom: 1em;">
    <img src="assets/screenshots/intro.png" width="320px" height="240px">
    <img src="assets/screenshots/battle.png" width="320px" height="240px">
</div>
<div style="display: flex; gap: 1em; margin-bottom: 1em;">
    <img src="assets/screenshots/collection.png" width="320px" height="240px">
    <img src="assets/screenshots/intro2.png" width="320px" height="240px">
</div>

## Download
* [GitHub (need account)](https://github.com/gtrxAC/romphonix-rpg/releases)
* [nightly.link (no account)](https://nightly.link/gtrxAC/romphonix-rpg/workflows/main/main)

## Building
1. If you're on Windows, download [w64devkit](https://github.com/skeeto/w64devkit/releases). Make sure you get a release zip, not the source code. Extract the archive somewhere and run `w64devkit.exe`. On Linux, just open a terminal.
2. Follow the below instructions for the platform you want to build for.

### Desktop
1. Run `make setup` to set up the project.
2. Run `make` to compile the project.

### Web
Web (WASM) is not properly supported yet. Dependencies like FluidSynth and tinyfiledialogs will have to be recompiled or substituted for Web. The build system has untested support for Web with the emscripten toolkit.
1. Run `make setup PLATFORM=Web` to set up the project. You will need about 1 GB of free space.
2. Run `make PLATFORM=Web` to compile the project.

### Compiling for Windows from Linux
1. Install `mingw-w64` using your package manager.
2. Run `make setup PLATFORM=Windows_NT` to set up the project.
3. Run `make PLATFORM=Windows_NT` to compile the project.
