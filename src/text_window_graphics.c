#include "global.h"
#include "text_window_graphics.h"

const u16 gSignpostWindow_Gfx[] = INCBIN_U16("graphics/text_window/signpost.4bpp");

static const u16 sUserFrame_Empty1[16] = {0};
static const u16 sUserFrame_Type3_Gfx[] = INCBIN_U16("graphics/text_window/type3.4bpp");
static const u16 sUserFrame_Type4_Gfx[] = INCBIN_U16("graphics/text_window/type4.4bpp");
static const u16 sUserFrame_Type5_Gfx[] = INCBIN_U16("graphics/text_window/type5.4bpp");
static const u16 sUserFrame_Empty2[16] = {0};
static const u16 sUserFrame_Empty3[16] = {0};

static const u16 sUserFrame_Type3_Pal[] = INCBIN_U16("graphics/text_window/type3.gbapal");
static const u16 sUserFrame_Type4_Pal[] = INCBIN_U16("graphics/text_window/type4.gbapal");
static const u16 sUserFrame_Type5_Pal[] = INCBIN_U16("graphics/text_window/type5.gbapal");

const u16 gStdTextWindow_Gfx[] = INCBIN_U16("graphics/text_window/std.4bpp");
const u16 gQuestLogWindow_Gfx[] = INCBIN_U16("graphics/text_window/quest_log.4bpp");

const u16 gTextWindowPalettes[][16] = {
    INCBIN_U16("graphics/text_window/stdpal_0.gbapal"),
    INCBIN_U16("graphics/text_window/stdpal_1.gbapal"),
    INCBIN_U16("graphics/text_window/stdpal_2.gbapal"),
    INCBIN_U16("graphics/text_window/stdpal_3.gbapal"),
    INCBIN_U16("graphics/text_window/stdpal_4.gbapal")
};

const struct TextWindowGraphics gUserFrames[USER_WINDOW_FRAME_COUNT] = {
    {sUserFrame_Type5_Gfx,  sUserFrame_Type5_Pal},
    {sUserFrame_Type3_Gfx,  sUserFrame_Type3_Pal},
    {sUserFrame_Type4_Gfx,  sUserFrame_Type4_Pal},
};

const struct TextWindowGraphics *GetUserWindowGraphics(u8 idx)
{
    if (idx >= ARRAY_COUNT(gUserFrames))
        return &gUserFrames[0];
    else
        return &gUserFrames[idx];
}
