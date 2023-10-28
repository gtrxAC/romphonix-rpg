// _____________________________________________________________________________
//
//  Introduction
// _____________________________________________________________________________
//
#include "common.h"

void scrIntroRocky();
void scrIntroAppearanceConfirm();
void scrIntroEnd();
void scrIntroConfirmName();
void scrIntroEnd1();
void scrIntroD307();

// _____________________________________________________________________________
//
//  Part 1 - Fade from black to red gradient
// _____________________________________________________________________________
//
void scrIntroFade() {
    setSong("assets/sounds/music/null.mid");
    g.frameCount = 0;
    g.state = ST_INTRO_FADE;
}

void drawIntroFade() {
    Color tintRed = {230, 41, 55, g.frameCount*2};
    Color tintWhite = {255, 255, 255, g.frameCount*2};
    drawTexturePro(
        "gradient", (Rectangle) {0, 0, 1, 64},
        (Rectangle) {0, 0, 320, 240}, 0.0f, tintRed
    );
    drawTexture("prof_rocky", 124, 48, tintWhite);
}

void updateIntroFade() {
    if (g.frameCount > 120) {
        scrIntroRocky();
    }
    if (g.frameCount == 60) {
        setSong("assets/sounds/music/rpxintro2.mid");
    }
}

// _____________________________________________________________________________
//
//  Part 2 - Professor Rocky
// _____________________________________________________________________________
//
void drawIntroRocky() {
    drawTexturePro(
        "gradient", (Rectangle) {0, 0, 1, 64},
        (Rectangle) {0, 0, 320, 240}, 0.0f, RED
    );
    drawTexture("prof_rocky", 124, 48, WHITE);
}

void scrIntroRocky() {
    g.state = ST_INTRO_ROCKY;
    pushTextbox("Hello there! It's so nice to meet you!", "");
    setNextFunc(scrIntroRocky2);
}

void scrIntroRocky2() {
    pushTextbox("Welcome to the WORLD OF PHONES!", "");
    setNextFunc(scrIntroRocky3);
}

void scrIntroRocky3() {
    pushTextbox(
        "My name is Professor Rocky, and I'm the",
        "authority on phones worldwide."
    );
    setNextFunc(scrIntroRocky4);
}

void scrIntroRocky4() {
    pushTextbox(
        "Big phones, small phones, smart phones, dumb",
        "phones... As a preservationist, it's my duty   \x7f"
    );
    setNextFunc(scrIntroRocky4_1);
}

void scrIntroRocky4_1() {
    pushTextbox("to preserve them all.", "");
    setNextFunc(scrIntroRocky5);
}

void scrIntroRocky5() {
    pushTextbox(
        "And as time and technology advance, the",
        "technology of the past is just left to rot."
    );
    setNextFunc(scrIntroRocky6);
}

void scrIntroRocky6() {
    pushTextbox(
        "Even though today some old technology keeps",
        "getting used, our old phones are forgotten."
    );
    setNextFunc(scrIntroRocky7);
}

void scrIntroRocky7() {
    pushTextbox(
        "These phones are more than just phones -",
        "they're valuable steps that led us to where \x7f"
    );
    setNextFunc(scrIntroRocky7_1);
}

void scrIntroRocky7_1() {
    pushTextbox(
        "we are today. Preserving them is extremely",
        "important, and that's what I do."
    );
    setNextFunc(scrIntroRocky8);
}

void scrIntroRocky8() {
    pushTextbox("That's enough about me. What about you?", "");
    setNextFunc(scrIntroAppearance);
}

// _____________________________________________________________________________
//
//  Part 3 - Ask for the player's appearance
// _____________________________________________________________________________
//
// This one is used for everything with ST_INTRO_APPEARANCE (i.e. part 3 and 4)
void drawIntroAppearanceState() {
    // Draw the gradient (same as part 4 but different color)
    drawTexturePro(
        "gradient", (Rectangle) {0, 0, 1, 64},
        (Rectangle) {0, 0, 320, 240}, 0.0f, BLUE
    );
}

void scrIntroAppearance() {
    g.state = ST_INTRO_APPEARANCE;
    pushTextbox("What do you look like?", "");
    pushMenu(CB_NOTHING);
    setUpdateFunc(updateIntroAppearance);
    setDrawFunc(drawIntroAppearance);
    MENU.drawPrevious = true;
}

void updateIntroAppearance() {
    // Similar to standard menu update function, except scrolling is done with
    // left/right instead of up/down. Fixed to 8 menu choices.
    if (K_LEFT_PRESS() && MENU.choice) MENU.choice--;

    else if (K_RIGHT_PRESS() && MENU.choice < 7) MENU.choice++;

    if (K_A_PRESS()) scrIntroAppearanceConfirm();
}

void drawIntroAppearance() {
    // Player appearance rotation
    Direction clockwise[] = {DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFT};
    Direction dir = clockwise[g.frameCount/30 % 4];

    // Draw the 8 player appearances
    for (int i = 0; i < 8; i++) {
        drawTextureRec(
            F("player%d", i),
            (Rectangle) {(g.frameCount/6 % 4)*16, dir*16, 16, 16},
            (Vector2) {36 + i*32, 104}, WHITE
        );
    }

    // Choice indicator
    drawTexture("indicator", 24 + MENU.choice*32, 107, WHITE);
}

// _____________________________________________________________________________
//
//  Part 4 - Confirm the chosen appearance
// _____________________________________________________________________________
//
void drawIntroAppearanceConfirm() {
    // When the player chooses an appearance, zoom in its large sprite and then
    // ask for confirmation
    if (MENU.timer > 30) drawTextBoxMenu();

    // Over 30 frames, transition from the original chosen sprite's size and
    // location to the large sprite's size centered
    drawTexturePro(
        F("large_player%d", LASTMENU.choice),
        (Rectangle) {0, 0, 40, 64},
        (Rectangle) {
            MAX((30.0f - MENU.timer)/30.0f, 0.0f)*(36.0f + LASTMENU.choice*32.0f) + MIN(MENU.timer/30.0f, 1.0f)*120.0f,
            MAX((30.0f - MENU.timer)/30.0f, 0.0f)*107.0f + MIN(MENU.timer/30.0f, 1.0f)*50.0f,
            MIN(16 + 2.13f*MENU.timer, 80),
            MIN(16 + 3.73f*MENU.timer, 128)
        },
        0.0f, WHITE
    );
}

void scrIntroAppearanceConfirm() {
    g.s.appearance = MENU.choice;
    pushTextboxMenu();
    setDrawFunc(drawIntroAppearanceConfirm);
    setNextFunc(checkIntroAppearanceConfirm);
    strcpy(MENU.textbox[0], "You look like this?");
    strcpy(MENU.textbox[1], "");
    addChoice("Yes");
    addChoice("No");
}

void checkIntroAppearanceConfirm() {
    int choice = MENU.choice;
    popMenu();  // confirmation menu
    if (choice == 0) {
        popMenu();  // appearance selection menu
        popMenu();  // appearance selection textbox
        scrIntroEnd();
    }
}

// _____________________________________________________________________________
//
//  Part 5 - Show the player sprite, ask for name, rest of the intro
// _____________________________________________________________________________
//
// Used by ST_INTRO_END
void drawIntroEnd() {
    // Don't use textformat here as it overwrites previous textformats used for
    // textbox text
    char sprite[16];
    sprintf(sprite, "large_player%d", g.s.appearance);
    drawTexturePro(sprite, (Rectangle) {0, 0, 40, 64}, (Rectangle) {120, 50, 80, 128}, 0.0f, WHITE);
}

void scrIntroEnd() {
    g.state = ST_INTRO_END;
    pushTextbox("I see. And what's your name?", "");
    setNextFunc(scrIntroAskName);
}

void scrIntroAskName() {
    char *defaultName = getenv("USER");  // Linux
    if (!defaultName) defaultName = getenv("USERNAME");  // Windows
    if (!defaultName) defaultName = "Player";
    char *name;

    nameEntry:
        name = tinyfd_inputBox("Name", "What is your name?", defaultName);
        if (!name || !strlen(name)) name = defaultName;

        if (strlen(name) > 15) {
            tinyfd_messageBox("Error", "Name must be 1-15 characters!", "ok", "error", 0);
            goto nameEntry;
        }

    strcpy(g.s.name, name);
    scrIntroConfirmName();
}

void scrIntroConfirmName() {
    pushTextboxMenu();
    setNextFunc(checkIntroConfirmName);
    strcpy(MENU.textbox[0], F("%s? That's your name?", g.s.name));
    strcpy(MENU.textbox[1], "");
    addChoice("Yes");
    addChoice("No");
}

void checkIntroConfirmName() {
    int choice = MENU.choice;
    popMenu();
    if (choice == 0) {
        scrIntroEnd1();
    } else {
        scrIntroAskName();
    }
}

void scrIntroEnd1() {
    pushTextbox(
        F("Nice to meet you, %s!", g.s.name),
        "Welcome to the team."
    );
    setNextFunc(scrIntroEnd2);
}

void scrIntroEnd2() {
    pushTextbox("Are you ready to embark on a quest", "of epic proportions?");
    setNextFunc(scrIntroEnd3);
}

void scrIntroEnd3() {
    pushTextbox("Have fun out there! And be sure to drop by", "my lab later.");
    setNextFunc(scrIntroFade2);
}

// _____________________________________________________________________________
//
//  Part 6 - Fade from blue to black, and a 1 sec timer
// _____________________________________________________________________________
//
void scrIntroFade2() {
    g.frameCount = 0;
    g.state = ST_INTRO_FADE2;
    setSong("assets/sounds/music/null.mid");
}

void drawIntroFade2() {
    if (g.frameCount < 60) {
        int blue = MAX(240 - 4*g.frameCount, 0);
        ClearBackground((Color) {0, 0, blue, 255});
    }
}

void updateIntroFade2() {
    if (g.frameCount > 120) {
        scrIntroD307();
    }
}

// _____________________________________________________________________________
//
//  Part 7 - Samsung D307
// _____________________________________________________________________________
//
void drawIntroD307() {
    drawTextureRec(
        "d307",
        (Rectangle) {(g.frameCount / 6 % 20)*128, 0, 128, 128},
        (Vector2) {96, 56}, WHITE
    );
}

void scrIntroD307() {
    g.state = ST_INTRO_D307;
    setSong("assets/sounds/music/rpxintro1.mid");
    pushTextbox("Yes! YES! YESSSSSS!", "");
    setNextFunc(scrIntroD307_2);
}

void scrIntroD307_2() {
    pushTextbox("Finally, it's mine!", "");
    setNextFunc(scrIntroD307_3);
}

void scrIntroD307_3() {
    pushTextbox("The rare! The legendary! The illustrious!", "");
    setNextFunc(scrIntroD307_4);
}

void scrIntroD307_4() {
    pushTextbox("The SAMSUNG SGH-D307!", "");
    setNextFunc(scrIntroD307_5);
}

void scrIntroD307_5() {
    pushTextbox("Come to me!", "");
    setNextFunc(scrIntroD307_6);
    // TODO: continue the cutscene
}

void scrIntroD307_6() {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(g.s.startDate, sizeof(g.s.startDate), "%Y-%m-%d", tm);

    g.s.id = GetRandomValue(10000, 99999);

    sprintf(g.s.chr.sprite, "player%d", g.s.appearance);
    g.s.chr.isPlayer = true;

    g.s.curMap = 0;
    loadMap(0);
    g.state = ST_INGAME;
    setSong(g.mapMeta.songName);
}