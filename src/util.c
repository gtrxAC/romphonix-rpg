// _____________________________________________________________________________
//
//  util.c - common drawing utilities for ROMphonix RPG / raylib
// _____________________________________________________________________________
//
#include "common.h"

// _____________________________________________________________________________
//
//  Box drawing (blue and light blue)
// _____________________________________________________________________________
//
void drawBoxCommon(Texture texture, int x, int y, int width, int height) {
    NPatchInfo nPatch = {
        (Rectangle) {0, 0, texture.width, texture.height},
        4, 4, 4, 4, NPATCH_NINE_PATCH
    };

    DrawTextureNPatch(
        texture, nPatch, (Rectangle) {x, y, width, height},
        (Vector2) {0, 0}, 0.0f, WHITE
    );
}

void drawBox(int x, int y, int width, int height) {
    drawBoxCommon(shget(g.textures, "textbox"), x, y, width, height);
}

void drawBoxL(int x, int y, int width, int height) {
    drawBoxCommon(shget(g.textures, "textbox_light"), x, y, width, height);
}

// _____________________________________________________________________________
//
//  Text drawing wrappers
// _____________________________________________________________________________
//
void drawText(const char *text, int x, int y, Color color) {
    DrawTextEx(g.fonts.dialogue, text, (Vector2) {x, y}, 13, 0, color);
}

void drawTextL(const char *text, int x, int y, Color color) {
    DrawTextEx(g.fonts.large, text, (Vector2) {x, y}, 23, 1, color);
}

void drawTextT(const char *text, int x, int y, Color color) {
    DrawTextEx(g.fonts.thin, text, (Vector2) {x, y}, 23, 1, color);
}

void drawTextD(const char *text, int x, int y, Color color) {
    // Draw an outline around the text
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            DrawTextEx(g.fonts.digits, text, (Vector2) {x + dx, y + dy}, 13, 0, BLACK);
        }
    }
    DrawTextEx(g.fonts.digits, text, (Vector2) {x, y}, 13, 0, color);
}

int measureText(const char *text) {
    return MeasureTextEx(g.fonts.dialogue, text, 13, 0).x;
}

int measureTextL(const char *text) {
    return MeasureTextEx(g.fonts.large, text, 23, 1).x;
}

// _____________________________________________________________________________
//
//  Texture drawing wrappers
// _____________________________________________________________________________
//
void drawTexturePro(const char *name, Rectangle source, Rectangle dest, float rotation, Color tint) {
    if (shgeti(g.textures, name) >= 0) {
        DrawTexturePro(shget(g.textures, name), source, dest, (Vector2) {0, 0}, rotation, tint);
    }
    else {
        drawText(F("Missing\ntexture\n'%s'", name), dest.x, dest.y, RED);
    }
}

void drawTextureRec(const char *name, Rectangle source, Vector2 position, Color tint) {
    drawTexturePro(
        name, source,
        (Rectangle) {position.x, position.y, source.width, source.height},
        0.0f, tint
    );
}

void drawTexture(const char *name, int x, int y, Color tint) {
    if (shgeti(g.textures, name) >= 0) {
        DrawTexture(shget(g.textures, name), x, y, tint);
    }
    else {
        drawText(F("Missing\ntexture\n'%s'", name), x, y, RED);
    }
}

// _____________________________________________________________________________
//
//  DrawTextRec was removed from raylib in 4.0, we need to re-implement it
//  https://github.com/raysan5/raylib/blob/master/examples/text/text_rectangle_bounds.c
// _____________________________________________________________________________
//
// Draw text using font inside rectangle limits with support for text selection
static void DrawTextBoxedSelectable(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint, int selectStart, int selectLength, Color selectTint, Color selectBackTint)
{
    int length = TextLength(text);  // Total length in bytes of the text, scanned by codepoints in loop

    float textOffsetY = 0;          // Offset between lines (on line break '\n')
    float textOffsetX = 0.0f;       // Offset X to next character to draw

    float scaleFactor = fontSize/(float)font.baseSize;     // Character rectangle scaling factor

    // Word/character wrapping mechanism variables
    enum { MEASURE_STATE = 0, DRAW_STATE = 1 };
    int state = wordWrap? MEASURE_STATE : DRAW_STATE;

    int startLine = -1;         // Index where to begin drawing (where a line begins)
    int endLine = -1;           // Index where to stop drawing (where a line ends)
    int lastk = -1;             // Holds last value of the character position

    for (int i = 0, k = 0; i < length; i++, k++)
    {
        // Get next codepoint from byte string and glyph index in font
        int codepointByteCount = 0;
        int codepoint = GetCodepoint(&text[i], &codepointByteCount);
        int index = GetGlyphIndex(font, codepoint);

        // NOTE: Normally we exit the decoding sequence as soon as a bad byte is found (and return 0x3f)
        // but we need to draw all of the bad bytes using the '?' symbol moving one byte
        if (codepoint == 0x3f) codepointByteCount = 1;
        i += (codepointByteCount - 1);

        float glyphWidth = 0;
        if (codepoint != '\n')
        {
            glyphWidth = (font.glyphs[index].advanceX == 0) ? font.recs[index].width*scaleFactor : font.glyphs[index].advanceX*scaleFactor;

            if (i + 1 < length) glyphWidth = glyphWidth + spacing;
        }

        // NOTE: When wordWrap is ON we first measure how much of the text we can draw before going outside of the rec container
        // We store this info in startLine and endLine, then we change states, draw the text between those two variables
        // and change states again and again recursively until the end of the text (or until we get outside of the container).
        // When wordWrap is OFF we don't need the measure state so we go to the drawing state immediately
        // and begin drawing on the next line before we can get outside the container.
        if (state == MEASURE_STATE)
        {
            // TODO: There are multiple types of spaces in UNICODE, maybe it's a good idea to add support for more
            // Ref: http://jkorpela.fi/chars/spaces.html
            if ((codepoint == ' ') || (codepoint == '\t') || (codepoint == '\n')) endLine = i;

            if ((textOffsetX + glyphWidth) > rec.width)
            {
                endLine = (endLine < 1)? i : endLine;
                if (i == endLine) endLine -= codepointByteCount;
                if ((startLine + codepointByteCount) == endLine) endLine = (i - codepointByteCount);

                state = !state;
            }
            else if ((i + 1) == length)
            {
                endLine = i;
                state = !state;
            }
            else if (codepoint == '\n') state = !state;

            if (state == DRAW_STATE)
            {
                textOffsetX = 0;
                i = startLine;
                glyphWidth = 0;

                // Save character position when we switch states
                int tmp = lastk;
                lastk = k - 1;
                k = tmp;
            }
        }
        else
        {
            if (codepoint == '\n')
            {
                if (!wordWrap)
                {
                    textOffsetY += (font.baseSize + font.baseSize/2)*scaleFactor;
                    textOffsetX = 0;
                }
            }
            else
            {
                if (!wordWrap && ((textOffsetX + glyphWidth) > rec.width))
                {
                    textOffsetY += (font.baseSize + font.baseSize/2)*scaleFactor;
                    textOffsetX = 0;
                }

                // When text overflows rectangle height limit, just stop drawing
                if ((textOffsetY + font.baseSize*scaleFactor) > rec.height) break;

                // Draw selection background
                bool isGlyphSelected = false;
                if ((selectStart >= 0) && (k >= selectStart) && (k < (selectStart + selectLength)))
                {
                    DrawRectangleRec((Rectangle){ rec.x + textOffsetX - 1, rec.y + textOffsetY, glyphWidth, (float)font.baseSize*scaleFactor }, selectBackTint);
                    isGlyphSelected = true;
                }

                // Draw current character glyph
                if ((codepoint != ' ') && (codepoint != '\t'))
                {
                    DrawTextCodepoint(font, codepoint, (Vector2){ rec.x + textOffsetX, rec.y + textOffsetY }, fontSize, isGlyphSelected? selectTint : tint);
                }
            }

            if (wordWrap && (i == endLine))
            {
                textOffsetY += (font.baseSize + font.baseSize/2)*scaleFactor;
                textOffsetX = 0;
                startLine = endLine;
                endLine = -1;
                glyphWidth = 0;
                selectStart += lastk - k;
                k = lastk;

                state = !state;
            }
        }

        // Slight modification: fix empty space at the start of a line, caused by word wrap
        if (textOffsetX || codepoint != ' ') textOffsetX += glyphWidth;
    }
}

static void DrawTextRec(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint)
{
    DrawTextBoxedSelectable(font, text, rec, fontSize, spacing, wordWrap, tint, 0, 0, WHITE, WHITE);
}

void drawTextRec(const char *text, int x, int y, int width, int height, Color color) {
    DrawTextRec(g.fonts.thin, text, (Rectangle) {x, y, width, height}, 13, 0, true, color);
}

// _____________________________________________________________________________
//
//  Sound wrapper and scheduling system
//  Allows playing one sound shortly after another by using a timer. Only
//  one scheduled sound can exist at a time.
// _____________________________________________________________________________
//
void playSound(const char *name) {
    PlaySound(shget(g.sounds, name));
}

void stopSound(const char *name) {
    StopSound(shget(g.sounds, name));
}

void schedSound(const char *name, int frames) {
    g.schedSound = shget(g.sounds, name);
    g.schedSoundTimer = frames;
}

void updateSchedSound() {
    if (g.schedSoundTimer) {
        g.schedSoundTimer--;
        if (!g.schedSoundTimer) {
            PlaySound(g.schedSound);
        }
    }
}

// _____________________________________________________________________________
//
//  Miscellaneous
// _____________________________________________________________________________
//
void drawProgressBar(int value, int max, int x, int y, int width, Color color) {
    DrawRectangle(x, y, width + 4, 12, BLACK);
    DrawRectangle(x + 2, y + 2, ((float) value / max) * width, 8, color);
}