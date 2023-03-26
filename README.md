# ROMphonix RPG
A phone collecting RPG! (in development)

[Codebase structure](STRUCTURE.md)

<hr>

>## **How to help?**
>This game is still in early development, so **your help is appreciated**.
>
>If you want to help, here are some easy ways:
>* [Submit phones](https://forms.gle/pNYc8qudVosWcPSf6) to be featured in the game (submissions closing soon)
>* Draw [sprites](assets/graphics/phones/) for the [phones](assets/data/phones.json).
>* Compose [music](assets/sounds/music/) for the game.
>* Join our [Discord](https://discord.gg/qQ2yCmynCK) to discuss the game's development (ROMphonix RPG #discussion channel)
>
>If you do any of these, your name will be featured in the game's credits!

<hr>

## Download
* [GitHub (need account)](https://github.com/gtrxAC/romphonix-rpg/releases)
* [nightly.link (no account)](https://nightly.link/gtrxAC/romphonix-rpg/workflows/main/main)

## Building
1. If you're on Windows, download [w64devkit](https://github.com/skeeto/w64devkit/releases). Make sure you get a release zip, not the source code. Extract the archive somewhere and run `w64devkit.exe`. On Linux, just open a terminal.
2. Follow the below instructions for the platform you want to build for.

### Desktop
2. Run `./setup.sh` to set up the project.
3. Run `./build.sh` to compile the project.

### Web
1. Run `TARGET=Web ./setup.sh` to set up the project. You will need about 1 GB of free space.
2. Run `TARGET=Web ./build.sh` to compile the project.

### Compiling for Windows from Linux
1. Install `mingw-w64` using your package manager.
2. Run `TARGET=Windows_NT ./setup.sh` to set up the project.
3. Run `TARGET=Windows_NT ./build.sh` to compile the project.
