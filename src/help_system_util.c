#include "global.h"
#include "gflib.h"
#include "event_data.h"
#include "help_system.h"
#include "strings.h"

#define ZERO 0

COMMON_DATA bool8 gHelpSystemEnabled = 0;

struct HelpSystemVideoState
{
    MainCallback savedVblankCb;
    MainCallback savedHblankCb;
    u16 savedDispCnt;
    u16 savedBg0Cnt;
    u16 savedBg0Hofs;
    u16 savedBg0Vofs;
    u16 savedBldCnt;
    u8 savedTextColor[3];
};

EWRAM_DATA u8 gDisableHelpSystemVolumeReduce = 0;
EWRAM_DATA bool8 gHelpSystemToggleWithRButtonDisabled = FALSE;

static EWRAM_DATA u8 *sMapTilesBackup = NULL;
static EWRAM_DATA struct HelpSystemVideoState sVideoState = {0};

static void DecompressAndRenderGlyph(u8 fontId, u16 glyph, struct Bitmap *srcBlit, struct Bitmap *destBlit, u8 *destBuffer, u8 x, u8 y, u8 width, u8 height);

u8 RunHelpSystemCallback(void)
{
    return 0;
}

void SaveCallbacks(void)
{
    vu16 *dma;

    sVideoState.savedVblankCb = gMain.vblankCallback;
    sVideoState.savedHblankCb = gMain.hblankCallback;
    gMain.vblankCallback = NULL;
    gMain.hblankCallback = NULL;

    dma = (void *)REG_ADDR_DMA0;
    dma[5] &= ~(DMA_START_MASK | DMA_DREQ_ON | DMA_REPEAT);
    dma[5] &= ~DMA_ENABLE;
    dma[5];
}

void SaveMapGPURegs(void)
{
    sVideoState.savedDispCnt = GetGpuReg(REG_OFFSET_DISPCNT);
    sVideoState.savedBg0Cnt = GetGpuReg(REG_OFFSET_BG0CNT);
    sVideoState.savedBg0Hofs = GetGpuReg(REG_OFFSET_BG0HOFS);
    sVideoState.savedBg0Vofs = GetGpuReg(REG_OFFSET_BG0VOFS);
    sVideoState.savedBldCnt = GetGpuReg(REG_OFFSET_BLDCNT);
}

void SaveMapTiles(void)
{
    if (sMapTilesBackup == NULL)
        sMapTilesBackup = Alloc(BG_CHAR_SIZE);

    if (sMapTilesBackup != NULL)
        RequestDma3Copy((void *)BG_CHAR_ADDR(3), sMapTilesBackup, BG_CHAR_SIZE, DMA3_16BIT);
}

void SaveMapTextColors(void)
{
    SaveTextColors(
        &sVideoState.savedTextColor[0],
        &sVideoState.savedTextColor[1],
        &sVideoState.savedTextColor[2]
    );
}

void RestoreCallbacks(void)
{
    gMain.vblankCallback = sVideoState.savedVblankCb;
    gMain.hblankCallback = sVideoState.savedHblankCb;
}

void RestoreGPURegs(void)
{
    SetGpuReg(REG_OFFSET_BLDCNT, sVideoState.savedBldCnt);
    SetGpuReg(REG_OFFSET_BG0HOFS, sVideoState.savedBg0Hofs);
    SetGpuReg(REG_OFFSET_BG0VOFS, sVideoState.savedBg0Vofs);
    SetGpuReg(REG_OFFSET_BG0CNT, sVideoState.savedBg0Cnt);
    SetGpuReg(REG_OFFSET_DISPCNT, sVideoState.savedDispCnt);
}

void RestoreMapTiles(void)
{
    if (sMapTilesBackup != NULL)
    {
        RequestDma3Copy(sMapTilesBackup, (void *)BG_CHAR_ADDR(3), BG_CHAR_SIZE, DMA3_16BIT);
        Free(sMapTilesBackup);
        sMapTilesBackup = NULL;
    }
}

void RestoreMapTextColors(void)
{
    RestoreTextColors(
        &sVideoState.savedTextColor[0],
        &sVideoState.savedTextColor[1],
        &sVideoState.savedTextColor[2]
    );
}

void HelpSystemRenderText(u8 fontId, u8 * dest, const u8 * src, u8 x, u8 y, u8 width, u8 height)
{
    struct Bitmap srcBlit;
    struct Bitmap destBlit;
    u8 orig_x = x;
    u8 i = 0;
    s32 clearPixels = 0;

    while (1)
    {
        u16 curChar = *src;
        src++;
        switch (curChar)
        {
        case EOS:
            return;
        case CHAR_NEWLINE:
            x = orig_x;
            y += gGlyphInfo.height + 1;
            break;
        case PLACEHOLDER_BEGIN:
            curChar = *src;
            src++;
            if (curChar == PLACEHOLDER_ID_PLAYER)
            {
                for (i = 0; i < 10; i++)
                {
                    if (gSaveBlock2Ptr->playerName[i] == EOS)
                        break;
                    DecompressAndRenderGlyph(fontId, gSaveBlock2Ptr->playerName[i], &srcBlit, &destBlit, dest, x, y, width, height);
                    if (fontId == FONT_SMALL)
                        x += gGlyphInfo.width;
                    else
                        x += gGlyphInfo.width + ZERO;
                }
            }
            else if (curChar == PLACEHOLDER_ID_STRING_VAR_1)
            {
                for (i = 0; ; i++)
                {
                    const u8 *str = FlagGet(FLAG_SYS_NOT_SOMEONES_PC) == TRUE ? gString_Bill : gString_Someone;

                    if (str[i] == EOS)
                        break;
                    DecompressAndRenderGlyph(fontId, str[i], &srcBlit, &destBlit, dest, x, y, width, height);
                    if (fontId == FONT_SMALL)
                        x += gGlyphInfo.width;
                    else
                        x += gGlyphInfo.width + ZERO;
                }
            }
            break;
        case CHAR_PROMPT_SCROLL:
        case CHAR_PROMPT_CLEAR:
            x = orig_x;
            y += gGlyphInfo.height + 1;
            break;
        case EXT_CTRL_CODE_BEGIN:
            curChar = *src;
            src++;
            switch (curChar)
            {
            case EXT_CTRL_CODE_COLOR_HIGHLIGHT_SHADOW:
                src++;
                // fallthrough
            case EXT_CTRL_CODE_PLAY_BGM:
            case EXT_CTRL_CODE_PLAY_SE:
                src++;
                // fallthrough
            case EXT_CTRL_CODE_COLOR:
            case EXT_CTRL_CODE_HIGHLIGHT:
            case EXT_CTRL_CODE_SHADOW:
            case EXT_CTRL_CODE_PALETTE:
            case EXT_CTRL_CODE_FONT:
            case EXT_CTRL_CODE_PAUSE:
            case EXT_CTRL_CODE_ESCAPE:
            case EXT_CTRL_CODE_SHIFT_RIGHT:
            case EXT_CTRL_CODE_SHIFT_DOWN:
                src++;
                // fallthrough
            case EXT_CTRL_CODE_RESET_FONT:
            case EXT_CTRL_CODE_PAUSE_UNTIL_PRESS:
            case EXT_CTRL_CODE_WAIT_SE:
            case EXT_CTRL_CODE_FILL_WINDOW:
                break;
            case EXT_CTRL_CODE_CLEAR:
            case EXT_CTRL_CODE_SKIP:
                src++;
                break;
            case EXT_CTRL_CODE_CLEAR_TO:
                clearPixels = *src + orig_x - x;

                if (clearPixels > 0)
                {
                    destBlit.pixels = dest;
                    destBlit.width = width * 8;
                    destBlit.height = height * 8;
                    FillBitmapRect4Bit(&destBlit, x, y, clearPixels, GetFontAttribute(fontId, FONTATTR_MAX_LETTER_HEIGHT), 0);
                    x += clearPixels;
                }
                src++;
                break;
            case EXT_CTRL_CODE_MIN_LETTER_SPACING:
                src++;
                break;
            case EXT_CTRL_CODE_JPN:
            case EXT_CTRL_CODE_ENG:
                break;
            }
            break;
        case CHAR_KEYPAD_ICON:
            curChar = *src;
            src++;
            srcBlit.pixels = (u8 *)&gKeypadIconTiles[0x20 * GetKeypadIconTileOffset(curChar)];
            srcBlit.width = 0x80;
            srcBlit.height = 0x80;
            destBlit.pixels = dest;
            destBlit.width = width * 8;
            destBlit.height = height * 8;
            BlitBitmapRect4Bit(&srcBlit, &destBlit, 0, 0, x, y, GetKeypadIconWidth(curChar), GetKeypadIconHeight(curChar), 0);
            x += GetKeypadIconWidth(curChar);
            break;
        case CHAR_EXTRA_SYMBOL:
            curChar = *src + 0x100;
            src++;
            // fallthrough
        default:
            if (curChar == CHAR_SPACE)
            {
                if (fontId == FONT_SMALL)
                    x += 5;
                else
                    x += 4;
            }
            else
            {
                DecompressAndRenderGlyph(fontId, curChar, &srcBlit, &destBlit, dest, x, y, width, height);
                if (fontId == FONT_SMALL)
                    x += gGlyphInfo.width;
                else
                    x += gGlyphInfo.width + ZERO;
            }
            break;
        }
    }
}

static void DecompressAndRenderGlyph(u8 fontId, u16 glyph, struct Bitmap *srcBlit, struct Bitmap *destBlit, u8 *destBuffer, u8 x, u8 y, u8 width, u8 height)
{
    if (fontId == FONT_SMALL)
        DecompressGlyph_Small(glyph, FALSE);
    else if (fontId == FONT_NORMAL)
        DecompressGlyph_Female(glyph, FALSE);
    else
        DecompressGlyph_Normal(glyph, FALSE);
    srcBlit->pixels = gGlyphInfo.pixels;
    srcBlit->width = 16;
    srcBlit->height = 16;
    destBlit->pixels = destBuffer;
    destBlit->width = width * 8;
    destBlit->height = height * 8;
    BlitBitmapRect4Bit(srcBlit, destBlit, 0, 0, x, y, gGlyphInfo.width, gGlyphInfo.height, 0);
}
