#include "global.h"
#include "pokemon.h"
#include "palette.h"
#include "window.h"
#include "move_category_icon.h"

static const u8 sCategoryIcons[] = INCBIN_U8("graphics/interface/move_categories.4bpp");
static const u16 sCategoryPalette[] = INCBIN_U16("graphics/interface/move_categories.gbapal");

void DrawMoveCategoryIcon(u8 windowId, u16 move, u8 x, u8 y, u8 backgroundColor)
{
    u8 category = gBattleMoves[move].category;
    u16 usedColors = 1;
    u16 i;
    u8 backgroundIndex;
    u16 background = gPlttBufferUnfaded[BG_PLTT_ID(GetWindowAttribute(windowId, WINDOW_PALETTE_NUM)) + backgroundColor];

    // The blitter skips source colour 0. After switching this tile's palette,
    // untouched window pixels would otherwise become an icon colour (blue).
    // Give the panel background an unused colour in this category's palette.
    for (i = category * 128; i < (category + 1) * 128; i++)
    {
        usedColors |= 1 << (sCategoryIcons[i] & 15);
        usedColors |= 1 << (sCategoryIcons[i] >> 4);
    }
    for (backgroundIndex = 1; backgroundIndex < 16; backgroundIndex++)
        if (!(usedColors & (1 << backgroundIndex)))
            break;

    LoadPalette(sCategoryPalette, BG_PLTT_ID(9), PLTT_SIZE_4BPP);
    if (backgroundColor == 0)
        backgroundIndex = 0;
    else
        LoadPalette(&background, BG_PLTT_ID(9) + backgroundIndex, sizeof(background));
    FillWindowPixelRect(windowId, PIXEL_FILL(backgroundIndex), x, y, 16, 16);
    BlitBitmapRectToWindow(windowId, sCategoryIcons, 0, category * 16,
                          16, 48, x, y, 16, 16);
    PutWindowRectTilemapOverridePalette(windowId, x / 8, y / 8, 2, 2, 9);
}
