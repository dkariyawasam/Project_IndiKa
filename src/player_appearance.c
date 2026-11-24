#include "global.h"
#include "palette.h"
#include "player_appearance.h"

// Extend the original red ramp only for sprites with extra trouser shades.
static u16 ExtendShade(u16 color, bool8 highlight)
{
    u16 r = color & 31;
    u16 g = (color >> 5) & 31;
    u16 b = (color >> 10) & 31;
    if (highlight)
        return RGB(r + (31 - r) / 3, g + (31 - g) / 3, b + (31 - b) / 3);
    return RGB(r / 2, g / 2, b / 2);
}

static void ApplyAccent(u16 offset, u8 count, u8 accent, const u8 *shades, u16 ramps[][5])
{
    u16 colors[5];
    u8 i;

    if (accent >= PLAYER_ACCENT_COUNT)
        accent = 0;
    for (i = 0; i < count; i++)
        colors[i] = ramps[accent][shades[i]];
    LoadPalette(colors, offset, count * sizeof(u16));
}

void ApplyPlayerAppearancePalette(u16 offset, enum PlayerPaletteKind kind)
{
    static const u8 introShades[] = {0, 1, 2, 3, 4};
    static const u8 primaryShades[] = {1, 2};
    static const u8 overworldPrimaryShades[] = {0, 2};
    static const u8 secondaryShades[] = {1, 2, 0};
    static const u8 frontSecondaryShades[] = {0, 1, 2, 3};
    // Secondary menu order is BLUE, RED, GREEN; the shared ramps are RED, BLUE, GREEN.
    static const u8 secondaryAccents[] = {1, 0, 2};
    u8 secondary = 0;
    u8 i;
    u16 ramps[PLAYER_ACCENT_COUNT][5];

    if (gSaveBlock2Ptr->appearanceVersion != 2
        && gSaveBlock2Ptr->appearanceVersion != PLAYER_APPEARANCE_VERSION)
        return;
    // Older appearance saves retain blue as their secondary selection.
    if (gSaveBlock2Ptr->appearanceVersion == PLAYER_APPEARANCE_VERSION
        && gSaveBlock2Ptr->playerSecondaryAccent < PLAYER_ACCENT_COUNT)
        secondary = gSaveBlock2Ptr->playerSecondaryAccent;

    // Callers load the original sprite palette before applying appearance.
    // Capture both accents before writing either so matching selections share
    // the original cap/vest red or jeans blue for this particular sprite.
    if (kind == PLAYER_PALETTE_INTRO)
    {
        for (i = 0; i < 5; i++)
        {
            ramps[0][i] = gPlttBufferUnfaded[offset + 27 + i];
            ramps[1][i] = gPlttBufferUnfaded[offset + 17 + i];
        }
    }
    else
    {
        u8 primary = kind == PLAYER_PALETTE_FRONT ? 12 : 11;
        ramps[0][1] = gPlttBufferUnfaded[offset + primary];
        ramps[0][2] = gPlttBufferUnfaded[offset + primary + 1];
        ramps[0][0] = ExtendShade(ramps[0][1], TRUE);
        ramps[0][3] = ExtendShade(ramps[0][2], FALSE);
        ramps[0][4] = ExtendShade(ramps[0][3], FALSE);
        if (kind == PLAYER_PALETTE_FRONT)
            for (i = 0; i < 4; i++)
                ramps[1][i] = gPlttBufferUnfaded[offset + 5 + i];
        else
        {
            ramps[1][0] = gPlttBufferUnfaded[offset + 7];
            ramps[1][1] = gPlttBufferUnfaded[offset + 5];
            ramps[1][2] = gPlttBufferUnfaded[offset + 6];
            ramps[1][3] = ExtendShade(ramps[1][2], FALSE);
        }
        ramps[1][4] = ExtendShade(ramps[1][3], FALSE);
        if (kind == PLAYER_PALETTE_OVERWORLD)
        {
            // The overworld jeans use slot 7 as their visible light fabric and
            // slot 6 as shadow. Slot 5 is an intermediate accent, not the base.
            // Keep the original cap red at the same light-fabric shade.
            u16 light = gPlttBufferUnfaded[offset + primary];
            u16 dark = ramps[0][2];
            ramps[0][0] = light;
            ramps[0][1] = RGB(((light & 31) + (dark & 31)) / 2,
                              (((light >> 5) & 31) + ((dark >> 5) & 31)) / 2,
                              (((light >> 10) & 31) + ((dark >> 10) & 31)) / 2);
        }
    }
    // Derive matching green shades from red; soften overworld and Style 2 portrait greens.
    for (i = 0; i < 5; i++)
    {
        u16 color = ramps[0][i];
        u16 r = (color >> 10) & 31;
        u16 g = color & 31;
        u16 b = (color >> 5) & 31;
        if (kind == PLAYER_PALETTE_OVERWORLD
            || (kind == PLAYER_PALETTE_FRONT && gSaveBlock2Ptr->playerGender == FEMALE))
        {
            // Blend one third toward neutral, then reduce brightness slightly.
            // Deriving every shade also preserves the lighter reflection palette.
            u16 neutral = (r + g + b) / 3;
            r = (2 * r + neutral) * 7 / 24;
            g = (2 * g + neutral) * 7 / 24;
            b = (2 * b + neutral) * 7 / 24;
            // Warm the muted green toward olive without increasing its peak.
            r += (g - r) / 4;
            b = b * 7 / 8;
        }
        ramps[2][i] = RGB(r, g, b);
    }

    if (kind == PLAYER_PALETTE_INTRO)
    {
        ApplyAccent(offset + 27, 5, gSaveBlock2Ptr->playerAccent, introShades, ramps);
        ApplyAccent(offset + 17, 5, secondaryAccents[secondary], introShades, ramps);
    }
    else
    {
        ApplyAccent(offset + (kind == PLAYER_PALETTE_FRONT ? 12 : 11),
                    2, gSaveBlock2Ptr->playerAccent,
                    kind == PLAYER_PALETTE_OVERWORLD ? overworldPrimaryShades : primaryShades, ramps);
        if (kind == PLAYER_PALETTE_FRONT)
            ApplyAccent(offset + 5, 4, secondaryAccents[secondary], frontSecondaryShades, ramps);
        else
            ApplyAccent(offset + 5, 3, secondaryAccents[secondary], secondaryShades, ramps);
    }
}
