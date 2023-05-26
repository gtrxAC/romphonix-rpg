// _____________________________________________________________________________
//
//  Introduction
// _____________________________________________________________________________
//
#include "common.h"

// Declarations
void scrIntroD307_2();
void scrIntroD307_3();
void scrIntroD307_4();
void scrIntroFade2();
void scrIntroRocky();
void scrIntroRocky2();
void scrIntroRocky3();
void scrIntroRocky4();
void scrIntroRocky4_1();
void scrIntroRocky5();
void scrIntroRocky6();
void scrIntroRocky7();
void scrIntroRocky7_1();
void scrIntroRocky8();
void scrIntroRocky9();
void scrIntroRocky10();
void scrIntroAppearance();
void drawIntroAppearance();
void updateIntroAppearance();
void checkIntroAppearance();
void scrIntroAppearanceConfirm();
void checkIntroAppearanceConfirm();
void scrIntroAskName();
void checkIntroConfirmName();
void scrIntroEnd2();
void scrIntroEnd3();
void scrIntroEnd4();

// _____________________________________________________________________________
//
//  Part 1 - Fade from blue to black
// _____________________________________________________________________________
//
void scrIntro() {
    g.frameCount = 0;
    g.state = ST_INTRO_FADE;
    // TODO: Music
}

void drawIntroFade() {
    ClearBackground((Color) {0, 0, 240 - 2*g.frameCount, 255});
}

void updateIntroFade() {
    if (g.frameCount > 120) {
        scrIntroD307();
    }
}

// _____________________________________________________________________________
//
//  Part 2 - Samsung D307
// _____________________________________________________________________________
//
void drawIntroD307() {
    DrawTextureRec(
        TEX(d307),
        (Rectangle) {(g.frameCount / 4 % 20)*128, 0, 128, 128},
        (Vector2) {96, 56}, WHITE
    );
}

void scrIntroD307() {
    g.state = ST_INTRO_D307;
    pushTextbox("Yes! YES! YESSSSSS!", "");
    MENU.nextFunc = scrIntroD307_2;
}

void scrIntroD307_2() {
    pushTextbox("Finally, it's mine!", "");
    MENU.nextFunc = scrIntroD307_3;
}

void scrIntroD307_3() {
    pushTextbox("The rare! The legendary! The illustrious!", "");
    MENU.nextFunc = scrIntroD307_4;
}

void scrIntroD307_4() {
    pushTextbox("The SAMSUNG SGH-D307!", "");
    MENU.nextFunc = scrIntroFade2;
}

// _____________________________________________________________________________
//
//  Part 3 - Fade from black to red gradient
// _____________________________________________________________________________
//
void scrIntroFade2() {
    g.frameCount = 0;
    g.state = ST_INTRO_FADE2;
}

void drawIntroFade2() {
    Color tintRed = {230, 41, 55, g.frameCount*2};
    Color tintWhite = {255, 255, 255, g.frameCount*2};
    DrawTexturePro(
        TEX(gradient), (Rectangle) {0, 0, 1, 64},
        (Rectangle) {0, 0, 320, 240}, (Vector2) {0, 0}, 0.0f, tintRed
    );
    DrawTexture(TEX(prof_rocky), 124, 48, tintWhite);
}

void updateIntroFade2() {
    if (g.frameCount > 120) {
        scrIntroRocky();
    }
}

// _____________________________________________________________________________
//
//  Part 4 - Professor Rocky
// _____________________________________________________________________________
//
void drawIntroRocky() {
    DrawTexturePro(
        TEX(gradient), (Rectangle) {0, 0, 1, 64},
        (Rectangle) {0, 0, 320, 240}, (Vector2) {0, 0}, 0.0f, RED
    );
    DrawTexture(TEX(prof_rocky), 124, 48, WHITE);
}

void scrIntroRocky() {
    g.state = ST_INTRO_ROCKY;
    pushTextbox("Hello there! It's so nice to meet you!", "");
    MENU.nextFunc = scrIntroRocky2;
}

void scrIntroRocky2() {
    pushTextbox("Welcome to the WORLD OF PHONES!", "");
    MENU.nextFunc = scrIntroRocky3;
}

void scrIntroRocky3() {
    pushTextbox(
        "My name is Professor Rocky, and I'm the",
        "authority on phones worldwide."
    );
    MENU.nextFunc = scrIntroRocky4;
}

void scrIntroRocky4() {
    pushTextbox(
        "Big phones, small phones, smart phones, dumb",
        "phones... As a preservationist, it's my duty..."
    );
    MENU.nextFunc = scrIntroRocky4_1;
}

void scrIntroRocky4_1() {
    pushTextbox("to preserve them all.", "");
    MENU.nextFunc = scrIntroRocky5;
}

void scrIntroRocky5() {
    pushTextbox(
        "And as time and technology advance, the",
        "technology of the past is just left to rot."
    );
    MENU.nextFunc = scrIntroRocky6;
}

void scrIntroRocky6() {
    pushTextbox(
        "Even though today some old technology keeps",
        "getting used, our old phones are forgotten."
    );
    MENU.nextFunc = scrIntroRocky7;
}

void scrIntroRocky7() {
    pushTextbox(
        "These phones are more than just phones -",
        "they're valuable steps that led us to where..."
    );
    MENU.nextFunc = scrIntroRocky7_1;
}

void scrIntroRocky7_1() {
    pushTextbox("we are today.", "");
    MENU.nextFunc = scrIntroRocky8;
}

void scrIntroRocky8() {
    pushTextbox(
        "The preservation of them are extremely",
        "important, and that's what I do."
    );
    MENU.nextFunc = scrIntroRocky9;
}

void scrIntroRocky9() {
    pushTextbox("That's enough about me. What about you?", "");
    MENU.nextFunc = scrIntroAppearance;
}

// _____________________________________________________________________________
//
//  Part 5 - Ask for the player's appearance
// _____________________________________________________________________________
//
// This one is used for everything with ST_INTRO_APPEARANCE (i.e. part 5 and 6)
void drawIntroAppearanceState() {
    // Draw the gradient (same as part 4 but different color)
    DrawTexturePro(
        TEX(gradient), (Rectangle) {0, 0, 1, 64},
        (Rectangle) {0, 0, 320, 240}, (Vector2) {0, 0}, 0.0f, BLUE
    );
}

void scrIntroAppearance() {
    g.state = ST_INTRO_APPEARANCE;
    pushTextbox("What do you look like?", "");
    pushMenu(0, NULL, CB_NOTHING);
    MENU.updateFunc = updateIntroAppearance;
    MENU.drawFunc = drawIntroAppearance;
}

void updateIntroAppearance() {
    // Similar to standard menu update function, except scrolling is done with
    // left/right instead of up/down. Fixed to 8 menu choices.
    if (K_LEFT_PRESS() && MENU.choice) MENU.choice--;

    else if (K_RIGHT_PRESS() && MENU.choice < 7) MENU.choice++;

    if (K_A_PRESS()) scrIntroAppearanceConfirm();
}

void drawIntroAppearance() {
    // Draw the textbox behind the menu
    Menu menu = arrpop(g.menus);
    MENU.drawFunc();
    arrpush(g.menus, menu);

    // Player appearance rotation
    Direction clockwise[] = {DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFT};
    Direction dir = clockwise[g.frameCount/30 % 4];

    // Draw the 8 player appearances
    for (int i = 0; i < 8; i++) {
        DrawTextureRec(
            shget(g.textures, TextFormat("player%d", i)),
            (Rectangle) {(g.frameCount/6 % 4)*16, dir*16, 16, 16},
            (Vector2) {36 + i*32, 104}, WHITE
        );
    }

    // Choice indicator
    DrawTexture(TEX(indicator), 24 + MENU.choice*32, 107, WHITE);
}

// _____________________________________________________________________________
//
//  Part 6 - Confirm the chosen appearance
// _____________________________________________________________________________
//
void drawIntroAppearanceConfirm() {
    // When the player chooses an appearance, zoom in its large sprite and then
    // ask for confirmation
    if (MENU.timer > 30) drawTextBoxMenu();

    // Over 30 frames, transition from the original chosen sprite's size and
    // location to the large sprite's size centered
    DrawTexturePro(
        shget(g.textures, TextFormat("large_player%d", LASTMENU.choice)),
        (Rectangle) {0, 0, 60, 100},
        (Rectangle) {
            MAX((30.0f - MENU.timer)/30.0f, 0.0f)*(36.0f + LASTMENU.choice*32.0f) + MIN(MENU.timer/30.0f, 1.0f)*130.0f,
            MAX((30.0f - MENU.timer)/30.0f, 0.0f)*107.0f + MIN(MENU.timer/30.0f, 1.0f)*65.0f,
            MIN(16 + 1.46f*MENU.timer, 60),
            MIN(16 + 2.8f*MENU.timer, 100)
        },
        (Vector2) {0, 0}, 0.0f, WHITE
    );
}

void scrIntroAppearanceConfirm() {
    g.s.appearance = MENU.choice;
    scrTextBoxMenu();
    MENU.drawFunc = drawIntroAppearanceConfirm;
    MENU.nextFunc = checkIntroAppearanceConfirm;
    MENU.textbox[0] = "You look like this?";
    MENU.textbox[1] = "";
    arrpush(MENU.choices, "Yes");
    arrpush(MENU.choices, "No");
}

void checkIntroAppearanceConfirm() {
    int choice = MENU.choice;
    popMenu();
    if (choice == 0) {
        scrIntroEnd();
    }
}

// _____________________________________________________________________________
//
//  Part 7 - Show the player sprite, rest of the intro
// _____________________________________________________________________________
//
// Used by ST_INTRO_END
void drawIntroEnd() {
    DrawTexture(
        shget(g.textures, TextFormat("large_player%d", g.s.appearance)),
        130, 65, WHITE
    );
}

void scrIntroEnd() {
    g.state = ST_INTRO_END;
    pushTextbox("I see. And what's your name?", "");
    MENU.nextFunc = scrIntroAskName;
}

void scrIntroAskName() {
    char *defaultName = getenv("USER");  // Linux
    if (!defaultName) defaultName = getenv("USERNAME");  // Windows
    if (!defaultName) defaultName = " ";
    char *name;

    nameEntry:
        name = tinyfd_inputBox("Name", "What is your name?", defaultName);
        if (!name) return;

        if (strlen(name) > 15) {
            tinyfd_messageBox("Error", "Name must be 1-15 characters!", "ok", "error", 0);
            goto nameEntry;
        }

    strcpy(g.s.name, name);
    scrIntroConfirmName();
}

void scrIntroConfirmName() {
    scrTextBoxMenu();
    MENU.nextFunc = checkIntroConfirmName;
    MENU.textbox[0] = TextFormat("%s? That's your name?", g.s.name);
    MENU.textbox[1] = "";
    arrpush(MENU.choices, "Yes");
    arrpush(MENU.choices, "No");
}

void checkIntroConfirmName() {
    int choice = MENU.choice;
    popMenu();
    if (choice == 0) {
        scrIntroEnd1();
    }
}

void scrIntroEnd1() {
    pushTextbox(
        TextFormat("Nice to meet you, %s!", g.s.name),
        "Welcome to the team."
    );
    MENU.nextFunc = scrIntroEnd2;
}

void scrIntroEnd2() {
    pushTextbox("Are you ready to embark on a quest", "of epic proportions?");
    MENU.nextFunc = scrIntroEnd3;
}

void scrIntroEnd3() {
    pushTextbox("Have fun out there! And be sure to drop", "by my lab later.");
    MENU.nextFunc = scrIntroEnd4;
}

void scrIntroEnd4() {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(g.s.startDate, sizeof(g.s.startDate), "%Y-%m-%d", tm);

    g.s.id = GetRandomValue(10000, 99999);

    g.s.curMap = 0;
    loadMap(0);
    g.state = ST_INGAME;
    setSong(g.mapMeta.songName);
    popMenu();
}