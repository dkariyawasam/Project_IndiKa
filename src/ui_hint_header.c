#include "global.h"
#include "gflib.h"
#include "menu.h"
#include "strings.h"
#include "ui_hint_header.h"
#include "text_window.h"

const u16 gUiHintHeaderPalette[16] =
{
    RGB(0, 15, 25),  RGB_WHITE,      RGB(12, 12, 12), RGB(26, 26, 25),
    RGB(28, 1, 1),   RGB(31, 23, 14), RGB(4, 19, 1),   RGB(18, 30, 18),
    RGB(0, 15, 25),  RGB(20, 24, 30), RGB(13, 25, 24), RGB_BLACK,
    RGB_BLACK,       RGB(31, 19, 18), RGB_WHITE,       RGB(23, 11, 10),
};

const u8 gUiHintHeaderTextColors[3] = {0, 1, 2};

// Accent shades: highlight, bevel, dark edge, mid edge, main background.
static const u16 sRedUiShades[5] = {RGB(27, 18, 19), RGB(25, 13, 14), RGB(14, 4, 5), RGB(18, 6, 7), RGB(23, 9, 10)};
static const u16 sGreenUiShades[5] = {RGB(21, 27, 18), RGB(17, 25, 13), RGB(7, 14, 4), RGB(10, 18, 6), RGB(13, 23, 9)};

static const u16 *GetUiThemeShades(u8 frameType)
{
    if (frameType == 1)
        return sRedUiShades;
    if (frameType == 2)
        return sGreenUiShades;
    return NULL;
}

u16 GetUiHintHeaderColor(void)
{
    const u16 *shades = GetUiThemeShades(gSaveBlock2Ptr->optionsWindowFrameType);
    return shades != NULL ? shades[4] : RGB(0, 15, 25);
}

void ApplyUiHintHeaderTheme(u16 paletteEntry)
{
    const u16 *shades = GetUiThemeShades(gSaveBlock2Ptr->optionsWindowFrameType);
    if (shades != NULL)
        LoadPalette(&shades[4], paletteEntry, sizeof(u16));
}

void ApplyUiDetailsFooterTheme(u16 paletteOffset)
{
    const u16 *shades = GetUiThemeShades(gSaveBlock2Ptr->optionsWindowFrameType);
    if (shades != NULL)
        LoadPalette(shades, paletteOffset + 11, 5 * sizeof(u16));
}

// Prepare all shades before loading to prevent a flash of the blue palette.
void LoadUiHintHelpPaletteForFrame(u16 paletteOffset, u8 frameType)
{
    u16 palette[16];
    const u16 *shades = GetUiThemeShades(frameType);

    CpuCopy16(GetTextWindowPalette(2), palette, sizeof(palette));
    if (shades != NULL)
        CpuCopy16(shades, &palette[11], 5 * sizeof(u16));
    LoadPalette(palette, paletteOffset, sizeof(palette));
}

void LoadUiHintHelpPalette(u16 paletteOffset)
{
    LoadUiHintHelpPaletteForFrame(paletteOffset, gSaveBlock2Ptr->optionsWindowFrameType);
}

void DrawUiHintHeader(u8 windowId, const u8 *text, u8 fillColor, u8 lineColor, u8 textY, bool8 putTilemap)
{
    u8 x = 236 - GetStringWidth(FONT_SMALL, text, 0);
    u8 colors[3] = {fillColor, 1, 2};

    // Transparent Trainer Card text sits above a separately themed strip.
    if (textY == 0)
        ApplyUiHintHeaderTheme(BG_PLTT_ID(GetWindowAttribute(windowId, WINDOW_PALETTE_NUM)) + fillColor);

    FillWindowPixelBuffer(windowId, PIXEL_FILL(fillColor));
    AddTextPrinterParameterized4(windowId, FONT_SMALL, x, textY, 0, 0, colors, 0, text);
    if (textY == 0)
        ScrollWindow(windowId, 0, 1, PIXEL_FILL(fillColor));
    FillWindowPixelRect(windowId, PIXEL_FILL(lineColor), 0, 15, 240, 1);
    if (putTilemap)
    {
        PutWindowTilemap(windowId);
        CopyWindowToVram(windowId, COPYWIN_FULL);
    }
    else
    {
        CopyWindowToVram(windowId, COPYWIN_GFX);
    }
}

// Identical rounded 6x8 indicators, with 12-pixel spacing, on both screens.
void DrawUiPageNotches(u8 windowId, u8 x, u8 count, u8 selected, u8 background)
{
    u8 i;
    u16 color = RGB(20, 24, 30);
    if (gSaveBlock2Ptr->optionsWindowFrameType == 1)
        color = RGB(30, 21, 20);
    else if (gSaveBlock2Ptr->optionsWindowFrameType == 2)
        color = RGB(22, 30, 20);
    LoadPalette(&color, BG_PLTT_ID(GetWindowAttribute(windowId, WINDOW_PALETTE_NUM)) + 9, sizeof(color));
    for (i = 0; i < count; i++)
    {
        u8 shade = i == selected ? 1 : 9;
        u8 left = x + i * 12;
        FillWindowPixelRect(windowId, PIXEL_FILL(background), left, 3, 8, 8);
        FillWindowPixelRect(windowId, PIXEL_FILL(shade), left + 2, 3, 4, 8);
        FillWindowPixelRect(windowId, PIXEL_FILL(shade), left + 1, 4, 6, 6);
    }
}
