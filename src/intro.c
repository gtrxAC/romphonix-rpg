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
    Color tint = {255, 255, 255, g.frameCount*2};
    DrawTexturePro(
        TEX(intro_gradient), (Rectangle) {0, 0, 1, 64},
        (Rectangle) {0, 0, 320, 240}, (Vector2) {0, 0}, 0.0f, tint
    );
    DrawTexture(TEX(prof_rocky), 124, 48, tint);
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
        TEX(intro_gradient), (Rectangle) {0, 0, 1, 64},
        (Rectangle) {0, 0, 320, 240}, (Vector2) {0, 0}, 0.0f, WHITE
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
        "phones... As a preservationist, it's my..."
    );
    MENU.nextFunc = scrIntroRocky4_1;
}

void scrIntroRocky4_1() {
    pushTextbox("...duty to preserve them all.", "");
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
        "they're valuable steps that led us to..."
    );
    MENU.nextFunc = scrIntroRocky7_1;
}

void scrIntroRocky7_1() {
    pushTextbox("...where we are today.", "");
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
}