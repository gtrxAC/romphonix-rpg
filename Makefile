# ______________________________________________________________________________
#
#  ROMphonix RPG - Makefile
#  This file compiles the ROMphonix RPG game and all components needed by it.
#  Compiled object files go in build/ and the final executable goes in bin/.
#
#  There are a few ways to run this makefile:
#
#  - make:         Builds the game.
#  - make DEBUG=1: Builds a debug version. Run 'make clean' first.
#  - make run:     Builds and runs the game.
#
#  - make clean:   Deletes all previously created files.
#  - make package: Creates a distributable package of the game in release/.
#
#  You can also specify the target platform:
#
#  - make PLATFORM=Windows_NT: Build for Windows using MinGW toolkit.
#  - make PLATFORM=Linux:      Build for Linux using gcc.
#  - make PLATFORM=Web:        Build for Web (WASM) using emscripten.
# ______________________________________________________________________________
#
TARGET = bin/romphonix
SRC = $(wildcard src/*.c) $(wildcard src/*/*.c)

# Choose host platform as the target platform if not otherwise specified
ifeq ($(OS),Windows_NT)
	PLATFORM ?= $(OS)
else
	PLATFORM ?= $(shell uname)
endif

BUILD = build/$(PLATFORM)
LDLIBS += -lraylib
PYTHON = python3

# Platform specific compiler flags
ifeq ($(PLATFORM),Linux)
	CC = gcc
	LDLIBS += -lGL -lm -lpthread -ldl -lrt -lX11 -lfluidsynth
	MAKE_FLAGS += PLATFORM=PLATFORM_DESKTOP
endif

ifeq ($(PLATFORM),Windows_NT)
	CC = x86_64-w64-mingw32-gcc
	LDLIBS += -lopengl32 -lgdi32 -lwinmm
	LDLIBS += -lfluidsynth.dll -lcomdlg32 -lole32
	LDFLAGS += -Wl,--subsystem,windows -static
	TARGET := $(TARGET).exe
	MAKE_FLAGS += PLATFORM=PLATFORM_DESKTOP CC=x86_64-w64-mingw32-gcc \
		AR=x86_64-w64-mingw32-ar OS=Windows_NT
endif

ifeq ($(PLATFORM),Darwin)
	CC = clang
	LDLIBS += -framework CoreVideo -framework IOKit -framework Cocoa
	LDLIBS += -framework GLUT -framework OpenGL
	CFLAGS += -DNO_SYNTH
endif

ifeq ($(PLATFORM),Web)
	CC = emcc
	CFLAGS += -s ASYNCIFY -s USE_GLFW=3 -s TOTAL_MEMORY=67108864
	CFLAGS += -s FORCE_FILESYSTEM=1 --shell-file src/shell.html
	CFLAGS += --preload-file assets
	TARGET := $(TARGET).html
	MAKE_FLAGS += PLATFORM=PLATFORM_WEB
endif

ifeq ($(CC),)
	echo Unsupported platform $(PLATFORM)
endif

# Compiler flags for all platforms
CFLAGS += -Iinclude
LDFLAGS += -Llib/$(PLATFORM)

# Compiler flags for release and debug modes
ifneq ($(DEBUG),)
	CFLAGS += -DDEBUG -O0 -g -Wall -Wextra -Wpedantic
else
	CFLAGS += -Os -flto -s
endif

# ______________________________________________________________________________
#
#  Compilation targets (default)
# ______________________________________________________________________________
#
all: romphonix rpxmap

run: all
ifeq ($(PLATFORM),Web)
	emrun $(TARGET)
else
ifneq ($(DEBUG),)
	gdb $(TARGET)
else
	LD_LIBRARY_PATH=bin ./$(TARGET)
endif
endif

romphonix: platform_tasks directories json2tfs src/functions.h $(TARGET)

platform_tasks:
ifeq ($(PLATFORM),Web)
	source emsdk/emsdk_env.sh
endif
ifeq ($(PLATFORM),Linux)
	cp lib/Linux/libfluidsynth.so bin/libfluidsynth.so
	cp lib/Linux/libfluidsynth.so bin/libfluidsynth.so.3
endif

directories:
# We use $(shell find) instead of $(wildcard) because we only want to find
# folders, but the latter also gives us files on Windows
	mkdir -p $(patsubst src/%/,$(BUILD)/%/,$(shell find src/*/ -type d)) $(BUILD)/mapeditor

json2tfs: $(wildcard assets/data/*.json)
	$(PYTHON) scripts/json2tfs.py

src/functions.h: $(SRC)
	$(PYTHON) scripts/funcmap.py

$(BUILD)/%.o: src/%.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(TARGET): $(patsubst src/%.c,$(BUILD)/%.o,$(SRC))
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD)/mapeditor/%.o: mapeditor/%.c
	$(CC) -c $(CFLAGS) $^ -o $@

rpxmap: $(patsubst %.c,$(BUILD)/%.o,$(wildcard mapeditor/*.c))
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

# ______________________________________________________________________________
#
#  Setup (separate target)
# ______________________________________________________________________________
#
HAVE_APT = $(shell command -v apt)
HAVE_DNF = $(shell command -v dnf)
HAVE_GIT = $(shell command -v git)
MACOSX_DEPLOYMENT_TARGET = 10.9

.PHONY: setup
setup: setup_dirs setup_deps setup_emsdk setup_raylib

setup_dirs:
	mkdir -p include src assets lib/$(PLATFORM)

setup_deps:
ifeq ($(PLATFORM),Linux)
ifneq ($(HAVE_APT),)
	sudo apt install build-essential git libasound2-dev mesa-common-dev \
		libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev \
		libglu1-mesa-dev
endif
ifneq ($(HAVE_DNF),)
	sudo dnf install alsa-lib-devel mesa-libGL-devel libX11-devel \
		libXrandr-devel libXi-devel libXcursor-devel libXinerama-devel
endif
endif
ifeq ($(PLATFORM),Darwin)
	-xcode-select --install
endif

emsdk:
ifeq ($(PLATFORM),Web)
ifneq ($(HAVE_GIT),)
	git clone https://github.com/emscripten-core/emsdk --depth 1
else
	@echo "emsdk directory not found, see https://emscripten.org/docs/getting_started/downloads.html or install git"
endif
endif

setup_emsdk: emsdk
ifeq ($(PLATFORM),Web)
	cd emsdk && ./emsdk install latest && ./emsdk activate latest
endif

raylib/:
ifneq ($(HAVE_GIT),)
	git clone https://github.com/raysan5/raylib --depth 1
else
	@echo "raylib directory not found, download it from https://github.com/raysan5/raylib or install git"
endif

update_raylib:
ifneq ($(HAVE_GIT),)
	cd raylib && git pull
endif

setup_raylib: raylib/ update_raylib
	rm -f raylib/src/*.o
	cd raylib/src && make $(MAKE_FLAGS)
	mv raylib/src/libraylib.a lib/$(PLATFORM)/
	cp raylib/src/raylib.h include/

# ______________________________________________________________________________
#
#  Other targets
# ______________________________________________________________________________
#
package: romphonix
	rm -rf release
	mkdir release

	cp -r assets release/
	cp -r bin release/
	cp romphonix_linux.sh release/
	cp romphonix_win.bat release/
	cp README.md release/

	cd release/assets && rm -f data/*.json maps/*.h

clean:
	rm -rf assets/data/*.tfs src/functions.h
	rm -rf build/
	rm -rf bin/romphonix*
	rm -rf mapeditor/*.o
	cd raylib/src && make clean