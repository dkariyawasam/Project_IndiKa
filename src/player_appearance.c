#include "global.h"
#include "palette.h"
#include "player_appearance.h"

// Rotate hue by channel permutation, preserving the original shade components.
static void RotateAccent(u16 offset, u8 count, u8 direction)
{
    u16 colors[5];
    u8 i;
    if (direction == 0 || direction >= PLAYER_ACCENT_COUNT)
        return;
    for (i = 0; i < count; i++)
    {
        u16 color = gPlttBufferUnfaded[offset + i];
        u16 r = color & 31;
        u16 g = (color >> 5) & 31;
        u16 b = (color >> 10) & 31;
        colors[i] = direction == 1 ? RGB(g, b, r) : RGB(b, r, g);
    }
    LoadPalette(colors, offset, count * sizeof(u16));
}

void ApplyPlayerAppearancePalette(u16 offset, enum PlayerPaletteKind kind)
{
    u8 first, count, secondary;
    // Version 2 saves retain their first accent and original blue secondary.
    if (gSaveBlock2Ptr->appearanceVersion != 2
        && gSaveBlock2Ptr->appearanceVersion != PLAYER_APPEARANCE_VERSION)
        return;
    first = kind == PLAYER_PALETTE_INTRO ? 27 : kind == PLAYER_PALETTE_FRONT ? 12 : 11;
    count = kind == PLAYER_PALETTE_INTRO ? 5 : 2;
    RotateAccent(offset + first, count, gSaveBlock2Ptr->playerAccent);

    if (gSaveBlock2Ptr->appearanceVersion != PLAYER_APPEARANCE_VERSION)
        return;
    secondary = gSaveBlock2Ptr->playerSecondaryAccent;
    if (secondary == 0 || secondary >= PLAYER_ACCENT_COUNT)
        return;
    first = kind == PLAYER_PALETTE_INTRO ? 17 : 5;
    count = kind == PLAYER_PALETTE_INTRO ? 5 : kind == PLAYER_PALETTE_FRONT ? 4 : 3;
    // Secondary choices are BLUE, RED, GREEN, matching the UI frame indices.
    RotateAccent(offset + first, count, secondary == 1 ? 2 : 1);
}
