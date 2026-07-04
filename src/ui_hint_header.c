#include "global.h"
#include "gflib.h"
#include "menu.h"
#include "strings.h"
#include "ui_hint_header.h"

const u16 gUiHintHeaderPalette[16] =
{
    RGB(0, 15, 25),  RGB_WHITE,      RGB(12, 12, 12), RGB(26, 26, 25),
    RGB(28, 1, 1),   RGB(31, 23, 14), RGB(4, 19, 1),   RGB(18, 30, 18),
    RGB(0, 15, 25),  RGB(20, 24, 30), RGB(13, 25, 24), RGB_BLACK,
    RGB_BLACK,       RGB(31, 19, 18), RGB_WHITE,       RGB(23, 11, 10),
};

const u8 gUiHintHeaderTextColors[3] = {0, 1, 2};

void DrawUiHintHeader(u8 windowId, const u8 *text, u8 fillColor, u8 lineColor, u8 textY, bool8 putTilemap)
{
    u8 x = 236 - GetStringWidth(FONT_SMALL, text, 0);

    FillWindowPixelBuffer(windowId, PIXEL_FILL(fillColor));
    AddTextPrinterParameterized4(windowId, FONT_SMALL, x, textY, 0, 0, gUiHintHeaderTextColors, 0, text);
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
