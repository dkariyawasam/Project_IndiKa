#include "global.h"
#include "gflib.h"
#include "task.h"
#include "new_menu_helpers.h"
#include "m4a.h"
#include "scanline_effect.h"
#include "graphics.h"
#include "help_system.h"
#include "intro.h"
#include "load_save.h"
#include "new_game.h"
#include "random.h"
#include "save.h"
#include "main_menu.h"
#include "clear_save_data_screen.h"
#include "berry_fix_program.h"
#include "decompress.h"
#include "constants/songs.h"

enum TitleScreenScene
{
    TITLESCREENSCENE_INIT = 0,
    TITLESCREENSCENE_FLASHSPRITE,
    TITLESCREENSCENE_FADEIN,
    TITLESCREENSCENE_RUN,
    TITLESCREENSCENE_RESTART,
    TITLESCREENSCENE_CRY
};

#if   defined(FIRERED)
#define TITLE_SPECIES SPECIES_CHARIZARD
#elif defined(LEAFGREEN)
#define TITLE_SPECIES SPECIES_VENUSAUR
#endif

static EWRAM_DATA u8 sTitleScreenTimerTaskId = 0;

static void ResetGpuRegs(void);
static void CB2_TitleScreenRun(void);
static void VBlankCB(void);
static void Task_TitleScreenTimer(u8 taskId);
static void Task_TitleScreenMain(u8 taskId);
static void SetTitleScreenScene(s16 *data, u8 sceneNum);
static void SetTitleScreenScene_Init(s16 *data);
static void SetTitleScreenScene_FlashSprite(s16 *data);
static void SetTitleScreenScene_FadeIn(s16 *data);
static void SetTitleScreenScene_Run(s16 *data);
static void SetGpuRegsForTitleScreenRun(void);
static void SetTitleScreenScene_Restart(s16 *data);
static void SetTitleScreenScene_Cry(s16 *data);
static void Task_TitleScreen_SlideWin0(u8 taskId);
static void Task_TitleScreen_BlinkPressStart(u8 taskId);
static void SignalEndTitleScreenPaletteSomethingTask(void);
static void UpdateScanlineEffectRegBuffer(s16 y);
static void ScheduleStopScanlineEffect(void);
static void LoadMainTitleScreenPalsAndResetBgs(void);
static void CB2_FadeOutTransitionToSaveClearScreen(void);
static void CB2_FadeOutTransitionToBerryFix(void);
static void LoadSpriteGfxAndPals(void);
#if defined(FIRERED)
static void UpdateTitleBackdropPaletteCycle(void);
static void SpriteCallback_TitleScreenFlame(struct Sprite *sprite);
static void Task_FlameSpawner(u8 taskId);
#elif defined(LEAFGREEN)
static void SpriteCallback_TitleScreenLeaf(struct Sprite *sprite);
static void Task_LeafSpawner(u8 taskId);
#endif
static void TitleScreen_srand(u8 taskId, u8 field, u16 seed);
static u16 TitleScreen_rand(u8 taskId, u8 field);
static u32 CreateBlankSprite(void);
static void SetPalOnOrCreateBlankSprite(bool32 hasCreatedBlankSprite);
static u8 CreateSlashSprite(void);
static void DeactivateSlashSprite(u8 spriteId);
static bool32 IsSlashSpriteDeactivated(u8 spriteId);
static void SpriteCallback_Slash(struct Sprite *sprite);

static const u8 sBorderBgTiles[] = INCBIN_U8("graphics/title_screen/border_bg.4bpp.lz");

#if defined(FIRERED)
#define TITLE_BACKDROP_PAL_SLOT 4
#define TITLE_BACKDROP_NIGHT_START (15 * 60)
#define TITLE_BACKDROP_NIGHT_END   (19 * 60)
#define TITLE_BACKDROP_DAY_START   (30 * 60)
#define TITLE_BACKDROP_DAY_END     (34 * 60)

static const u16 sBackdropPal[] = INCBIN_U16("graphics/title_screen/firered/backdrop.gbapal");
static const u8 sBackdropTiles[] = INCBIN_U8("graphics/title_screen/firered/backdrop.4bpp.lz");
static const u8 sBorderBgMap[] = INCBIN_U8("graphics/title_screen/firered/backdrop.bin.lz");
static const u8 sFooterTiles[] = INCBIN_U8("graphics/title_screen/firered/footer.4bpp.lz");
static const u8 sCopyrightPressStartTiles[] = INCBIN_U8("graphics/title_screen/copyright_press_start.4bpp");
static const u8 sCopyrightPressStartTransparentTiles[] = INCBIN_U8("graphics/title_screen/copyright_press_start_transparent.4bpp");
static const u16 sGrassPal[] = INCBIN_U16("graphics/title_screen/firered/grass.gbapal");
static const u8 sGrassTiles[] = INCBIN_U8("graphics/title_screen/firered/grass.4bpp.lz");
static const u8 sGrassMap[] = INCBIN_U8("graphics/title_screen/firered/grass.bin.lz");
#elif defined(LEAFGREEN)
static const u8 sBorderBgMap[] = INCBIN_U8("graphics/title_screen/leafgreen/border_bg.bin.lz");
#endif

static const u32 sSlash_Gfx[] = INCBIN_U32("graphics/title_screen/slash.4bpp.lz");

#if defined(FIRERED)
static const u16 sSparkles_Pal[] = INCBIN_U16("graphics/intro/game_freak/sparkles.gbapal");
static const u32 sSparklesSmall_Gfx[] = INCBIN_U32("graphics/intro/game_freak/sparkles_small.4bpp.lz");
static const u32 sSparklesBig_Gfx[] = INCBIN_U32("graphics/intro/game_freak/sparkles_big.4bpp.lz");
#elif defined(LEAFGREEN)
static const u16 sLeaves_Pal[] = INCBIN_U16("graphics/title_screen/leafgreen/leaves.gbapal");
static const u32 sLeaves_Gfx[] = INCBIN_U32("graphics/title_screen/leafgreen/leaves.4bpp.lz");
static const u32 sStreak_Gfx[] = INCBIN_U32("graphics/title_screen/leafgreen/streak.4bpp.lz");
#endif

static const struct OamData sOamData_FlameOrLeaf = {
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = ST_OAM_SQUARE,
#if defined(FIRERED)
    .size = ST_OAM_SIZE_0,
#else
    .size = ST_OAM_SIZE_1,
#endif
    .tileNum = 0,
#if defined(FIRERED)
    .priority = 2,
#else
    .priority = 3,
#endif
    .paletteNum = 0
};

#if defined(FIRERED)
static const union AnimCmd sSpriteAnim_Sparkle[] = {
    ANIMCMD_FRAME(0, 4),
    ANIMCMD_FRAME(1, 4),
    ANIMCMD_FRAME(2, 4),
    ANIMCMD_FRAME(3, 4),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd sSpriteAnim_BigSparkle[] = {
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(16, 8),
    ANIMCMD_FRAME(32, 8),
    ANIMCMD_FRAME(48, 8),
    ANIMCMD_END
};

static const union AnimCmd *const sSpriteAnim_FlameOrLeaf[] = {
    sSpriteAnim_Sparkle,
};

static const union AnimCmd *const sSpriteAnim_BigSparkleTable[] = {
    sSpriteAnim_BigSparkle
};

#elif defined(LEAFGREEN)
static const union AnimCmd sSpriteAnim_Leaf[] = {
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(4, 8),
    ANIMCMD_FRAME(8, 8),
    ANIMCMD_FRAME(12, 8),
    ANIMCMD_FRAME(16, 8),
    ANIMCMD_FRAME(20, 8),
    ANIMCMD_FRAME(24, 8),
    ANIMCMD_FRAME(28, 8),
    ANIMCMD_FRAME(32, 8),
    ANIMCMD_FRAME(36, 8),
    ANIMCMD_FRAME(40, 8),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd *const sSpriteAnim_FlameOrLeaf[] = {
    sSpriteAnim_Leaf
};
#endif

enum {
    TILE_TAG_FLAME_OR_LEAF,
    TILE_TAG_BLANK_OR_STREAK,
    TILE_TAG_BLANK,
    TILE_TAG_SLASH,
};

enum {
    PAL_TAG_DEFAULT,
    PAL_TAG_UNUSED,
    PAL_TAG_SLASH,
};

static const struct SpriteTemplate sSpriteTemplate_FlameOrLeaf = {
    .tileTag = TILE_TAG_FLAME_OR_LEAF,
    .paletteTag = PAL_TAG_DEFAULT,
    .oam = &sOamData_FlameOrLeaf,
    .anims = sSpriteAnim_FlameOrLeaf,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

#if defined(FIRERED)
static const struct OamData sOamData_BigSparkle = {
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = SPRITE_SHAPE(32x32),
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0
};

static const struct SpriteTemplate sSpriteTemplate_BigSparkle = {
    .tileTag = TILE_TAG_BLANK_OR_STREAK,
    .paletteTag = PAL_TAG_DEFAULT,
    .oam = &sOamData_BigSparkle,
    .anims = sSpriteAnim_BigSparkleTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

#elif defined(LEAFGREEN)
static const struct OamData sOamData_Streak = {
    .shape = SPRITE_SHAPE(32x16),
    .size = SPRITE_SIZE(32x16),
    .priority = 3
};

static const struct SpriteTemplate sSpriteTemplate_Streak = {
    .tileTag = TILE_TAG_BLANK_OR_STREAK,
    .paletteTag = PAL_TAG_DEFAULT,
    .oam = &sOamData_Streak,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};
#endif

static const struct OamData sOamData_BlankSprite = {
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = ST_OAM_V_RECTANGLE,
    .size = ST_OAM_SIZE_3,
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0
};

static const struct SpriteTemplate sSpriteTemplate_BlankSprite = {
    .tileTag = TILE_TAG_BLANK,
    .paletteTag = PAL_TAG_SLASH,
    .oam = &sOamData_BlankSprite,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

static const struct OamData sOamData_SlashSprite = {
    .objMode = ST_OAM_OBJ_WINDOW,
    .shape = ST_OAM_SQUARE,
    .size = ST_OAM_SIZE_3,
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0
};

static const struct SpriteTemplate sSlashSpriteTemplate = {
    .tileTag = TILE_TAG_SLASH,
    .paletteTag = PAL_TAG_SLASH,
    .oam = &sOamData_SlashSprite,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

static const struct BgTemplate sBgTemplates[] = {
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 1, // 8bpp
        .priority = 0,
        .baseTile = 0
    }, {
        .bg = 1,
        .charBaseIndex = 1,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0, // 4bpp
#if defined(FIRERED)
        .priority = 3,
#else
        .priority = 1,
#endif
        .baseTile = 0
    }, {
        .bg = 2,
        .charBaseIndex = 2,
        .mapBaseIndex = 29,
        .screenSize = 0,
        .paletteMode = 0, // 4bpp
        .priority = 2,
        .baseTile = 0
    }, {
        .bg = 3,
        .charBaseIndex = 1,
        .mapBaseIndex = 28,
        .screenSize = 0,
        .paletteMode = 0, // 4bpp
        .priority = 3,
        .baseTile = 0
    }
};

static void (*const sSceneFuncs[])(s16 *data) = {
    [TITLESCREENSCENE_INIT]        = SetTitleScreenScene_Init,
    [TITLESCREENSCENE_FLASHSPRITE] = SetTitleScreenScene_FlashSprite,
    [TITLESCREENSCENE_FADEIN]      = SetTitleScreenScene_FadeIn,
    [TITLESCREENSCENE_RUN]         = SetTitleScreenScene_Run,
    [TITLESCREENSCENE_RESTART]     = SetTitleScreenScene_Restart,
    [TITLESCREENSCENE_CRY]         = SetTitleScreenScene_Cry
};

#if defined(FIRERED)
static const struct CompressedSpriteSheet sSpriteSheets[] = {
    {sSparklesSmall_Gfx,             0x80,  TILE_TAG_FLAME_OR_LEAF},
    {sSparklesBig_Gfx,               0x800, TILE_TAG_BLANK_OR_STREAK},
    {gTitleScreen_BlankSprite_Tiles, 0x400, TILE_TAG_BLANK},
    {sSlash_Gfx,                     0x800, TILE_TAG_SLASH}
};

static const struct SpritePalette sSpritePals[] = {
    {sSparkles_Pal,          PAL_TAG_DEFAULT},
    {gTitleScreen_Slash_Pal, PAL_TAG_SLASH},
    {}
};

static const u8 sFlameXPositions[] = {
    4, 16, 26, 32, 48, 200, 216, 224, 232, 60, 76, 92, 108, 128, 144, 0
};

#elif defined(LEAFGREEN)
static const struct CompressedSpriteSheet sSpriteSheets[] = {
    {sLeaves_Gfx,                    0x580, TILE_TAG_FLAME_OR_LEAF},
    {sStreak_Gfx,                    0x100, TILE_TAG_BLANK_OR_STREAK},
    {gTitleScreen_BlankSprite_Tiles, 0x400, TILE_TAG_BLANK},
    {sSlash_Gfx,                     0x800, TILE_TAG_SLASH}
};

static const struct SpritePalette sSpritePals[] = {
    {sLeaves_Pal,            PAL_TAG_DEFAULT},
    {gTitleScreen_Slash_Pal, PAL_TAG_SLASH},
    {}
};

static const u16 sStreakYPositions[] = {
    40, 80, 110, 60, 90, 70, 100, 50
};
#endif

static const u32 sUnused_Tilemap1[] = INCBIN_U32("graphics/title_screen/unused1.bin.lz");
static const u32 sUnused_Tilemap2[] = INCBIN_U32("graphics/title_screen/unused2.bin.lz");
static const u32 sUnused_Tilemap3[] = INCBIN_U32("graphics/title_screen/unused3.bin.lz");
static const u32 sUnused_Tilemap4[] = INCBIN_U32("graphics/title_screen/unused4.bin.lz");
static const u32 sUnused_Tilemap5[] = INCBIN_U32("graphics/title_screen/unused5.bin.lz");
static const u32 sUnused_Tilemap6[] = INCBIN_U32("graphics/title_screen/unused6.bin.lz");

static const u32 *const sUnused_Tilemaps[] = {
    sUnused_Tilemap1,
    sUnused_Tilemap2,
    sUnused_Tilemap3,
    sUnused_Tilemap4,
    sUnused_Tilemap5,
    sUnused_Tilemap6,
};

void CB2_InitTitleScreen(void)
{
    switch (gMain.state)
    {
    default:
        gMain.state = 0;
        // fallthrough
    case 0:
        SetVBlankCallback(NULL);
        StartTimer1();
        InitHeap(gHeap, HEAP_SIZE);
        ResetTasks();
        ResetSpriteData();
        FreeAllSpritePalettes();
        ResetPaletteFade();
        ResetGpuRegs();
        DmaFill16(3, 0, (void *)VRAM, VRAM_SIZE);
        DmaFill32(3, 0, (void *)OAM, OAM_SIZE);
        DmaFill16(3, 0, (void *)PLTT, PLTT_SIZE);
        ResetBgsAndClearDma3BusyFlags(FALSE);
        InitBgsFromTemplates(0, sBgTemplates, NELEMS(sBgTemplates));
        SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
        sTitleScreenTimerTaskId = TASK_NONE;
        break;
    case 1:
        LoadPalette(gGraphics_TitleScreen_GameTitleLogoPals, BG_PLTT_ID(0), 13 * PLTT_SIZE_4BPP);
#if defined(FIRERED)
        LoadPalette(sBackdropPal, BG_PLTT_ID(4), PLTT_SIZE_4BPP);
#endif
        DecompressAndCopyTileDataToVram(0, gGraphics_TitleScreen_GameTitleLogoTiles, 0, 0, 0);
        DecompressAndCopyTileDataToVram(0, gGraphics_TitleScreen_GameTitleLogoMap, 0, 0, 1);
        LoadPalette(gGraphics_TitleScreen_BoxArtMonPals, BG_PLTT_ID(13), PLTT_SIZE_4BPP);
        DecompressAndCopyTileDataToVram(1, gGraphics_TitleScreen_BoxArtMonTiles, 0, 0, 0);
        DecompressAndCopyTileDataToVram(1, gGraphics_TitleScreen_BoxArtMonMap, 0, 0, 1);
        LoadPalette(gGraphics_TitleScreen_BackgroundPals, BG_PLTT_ID(15), PLTT_SIZE_4BPP);
        DecompressAndCopyTileDataToVram(2, gGraphics_TitleScreen_CopyrightPressStartTiles, 0, 0, 0);
        DecompressAndCopyTileDataToVram(2, gGraphics_TitleScreen_CopyrightPressStartMap, 0, 0, 1);
#if defined(FIRERED)
        DecompressAndCopyTileDataToVram(2, sGrassTiles, 0, 64, 0);
        DecompressAndCopyTileDataToVram(2, sGrassMap, 0, 0, 1);
#endif
#if defined(FIRERED)
        LoadPalette(sGrassPal, BG_PLTT_ID(14), PLTT_SIZE_4BPP);
#else
        LoadPalette(gGraphics_TitleScreen_BackgroundPals, BG_PLTT_ID(14), PLTT_SIZE_4BPP);
#endif
        DecompressAndCopyTileDataToVram(3, sBorderBgTiles, 0, 144, 0);
#if defined(FIRERED)
        DecompressAndCopyTileDataToVram(3, sFooterTiles, 0, 148, 0);
        DecompressAndCopyTileDataToVram(3, sBackdropTiles, 0, 208, 0);
#endif
        DecompressAndCopyTileDataToVram(3, sBorderBgMap, 0, 0, 1);
        LoadSpriteGfxAndPals();
        break;
    case 2:
        if (!FreeTempTileDataBuffersIfPossible())
        {
            BlendPalettes(PALETTES_BG, 16, RGB_BLACK);
            CreateTask(Task_TitleScreenMain, 4);
            sTitleScreenTimerTaskId = CreateTask(Task_TitleScreenTimer, 2);
            SetVBlankCallback(VBlankCB);
            SetMainCallback2(CB2_TitleScreenRun);
            m4aSongNumStart(MUS_TITLE);
        }
        return;
    }
    gMain.state++;
}

static void ResetGpuRegs(void)
{
    SetGpuReg(REG_OFFSET_DISPCNT,  0);
    SetGpuReg(REG_OFFSET_BLDCNT,   0);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
    SetGpuReg(REG_OFFSET_BLDY,     0);
    SetGpuReg(REG_OFFSET_BG0HOFS,  0);
    SetGpuReg(REG_OFFSET_BG0VOFS,  0);
    SetGpuReg(REG_OFFSET_BG1HOFS,  0);
    SetGpuReg(REG_OFFSET_BG1VOFS,  0);
    SetGpuReg(REG_OFFSET_BG2HOFS,  0);
    SetGpuReg(REG_OFFSET_BG2VOFS,  0);
    SetGpuReg(REG_OFFSET_BG3HOFS,  0);
    SetGpuReg(REG_OFFSET_BG3VOFS,  0);
}

static void CB2_TitleScreenRun(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
#if defined(FIRERED)
    UpdateTitleBackdropPaletteCycle();
#endif
}

static void VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
    ScanlineEffect_InitHBlankDmaTransfer();

    if (sTitleScreenTimerTaskId != TASK_NONE)
        gTasks[sTitleScreenTimerTaskId].data[0]++;
}

static void Task_TitleScreenTimer(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (data[0] >= 2700)
    {
        sTitleScreenTimerTaskId = TASK_NONE;
        DestroyTask(taskId);
    }
}

#if defined(FIRERED)
static u16 TintTitleBackdropColorNight(u16 color)
{
    u8 r = color & 0x1F;
    u8 g = (color >> 5) & 0x1F;
    u8 b = (color >> 10) & 0x1F;

    r = (r * 10) / 32;
    g = (g * 11) / 32;
    b = (b * 19) / 32;

    return RGB(r, g, b);
}

static u16 BlendTitleBackdropColorTowardNight(u16 dayColor, u8 blend, u8 maxBlend)
{
    u16 nightColor = TintTitleBackdropColorNight(dayColor);
    u8 dayR = dayColor & 0x1F;
    u8 dayG = (dayColor >> 5) & 0x1F;
    u8 dayB = (dayColor >> 10) & 0x1F;
    u8 nightR = nightColor & 0x1F;
    u8 nightG = (nightColor >> 5) & 0x1F;
    u8 nightB = (nightColor >> 10) & 0x1F;
    u8 r = dayR + (((s8)nightR - (s8)dayR) * blend) / maxBlend;
    u8 g = dayG + (((s8)nightG - (s8)dayG) * blend) / maxBlend;
    u8 b = dayB + (((s8)nightB - (s8)dayB) * blend) / maxBlend;

    return RGB(r, g, b);
}

static void LoadTitleBackdropPaletteBlend(u8 blend)
{
    u32 i;
    u16 offset = BG_PLTT_ID(TITLE_BACKDROP_PAL_SLOT);

    for (i = 0; i < PLTT_SIZE_4BPP / sizeof(u16); i++)
    {
        u16 color = BlendTitleBackdropColorTowardNight(sBackdropPal[i], blend, 16);
        gPlttBufferUnfaded[offset + i] = color;
        gPlttBufferFaded[offset + i] = color;
    }
}

static void UpdateTitleBackdropPaletteCycle(void)
{
    s16 timer;
    u8 blend;

    if (sTitleScreenTimerTaskId == TASK_NONE)
        return;

    timer = gTasks[sTitleScreenTimerTaskId].data[0];
    if (timer < TITLE_BACKDROP_NIGHT_START)
        return;

    if (timer < TITLE_BACKDROP_NIGHT_END)
    {
        blend = ((timer - TITLE_BACKDROP_NIGHT_START) * 16) / (TITLE_BACKDROP_NIGHT_END - TITLE_BACKDROP_NIGHT_START);
    }
    else if (timer < TITLE_BACKDROP_DAY_START)
    {
        blend = 16;
    }
    else if (timer < TITLE_BACKDROP_DAY_END)
    {
        blend = 16 - (((timer - TITLE_BACKDROP_DAY_START) * 16) / (TITLE_BACKDROP_DAY_END - TITLE_BACKDROP_DAY_START));
    }
    else
    {
        blend = 0;
    }

    LoadTitleBackdropPaletteBlend(blend);
}
#endif

// task data for Task_TitleScreenMain and the scenes
#define tSceneNum              data[0]
#define tState                 data[1]
#define tHasCreatedBlankSprite data[5]
#define tSlashSpriteId         data[6]

static void Task_TitleScreenMain(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (JOY_NEW(A_BUTTON | B_BUTTON | START_BUTTON)
        && tSceneNum != TITLESCREENSCENE_RUN
        && tSceneNum != TITLESCREENSCENE_RESTART
        && tSceneNum != TITLESCREENSCENE_CRY)
    {
        ScheduleStopScanlineEffect();
        LoadMainTitleScreenPalsAndResetBgs();
        SetPalOnOrCreateBlankSprite(tHasCreatedBlankSprite);
        SetTitleScreenScene(data, TITLESCREENSCENE_RUN);
    }
    else
        sSceneFuncs[tSceneNum](data);
}

static void SetTitleScreenScene(s16 *data, u8 sceneNum)
{
    tState = 0;
    tSceneNum = sceneNum;
}

static void SetTitleScreenScene_Init(s16 *data)
{
    struct ScanlineEffectParams params;

    HideBg(0);
    ShowBg(1);
    ShowBg(2);
    ShowBg(3);

    params.dmaDest = (volatile void *)REG_ADDR_BLDY;
    params.dmaControl = SCANLINE_EFFECT_DMACNT_16BIT;
    params.initState = 1;
    params.unused9 = 0;

    CpuFill16(0, gScanlineEffectRegBuffers[0], 0x140);
    CpuFill16(0, gScanlineEffectRegBuffers[1], 0x140);

    ScanlineEffect_SetParams(params);

    SetTitleScreenScene(data, TITLESCREENSCENE_FLASHSPRITE);
}

static void SetTitleScreenScene_FlashSprite(s16 *data)
{
    switch (tState)
    {
    case 0:
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_LIGHTEN);
        SetGpuReg(REG_OFFSET_BLDY, 0);
        data[2] = 128;
        UpdateScanlineEffectRegBuffer(data[2]);
        tState++;
        break;
    case 1:
        data[2] -= 4;
        UpdateScanlineEffectRegBuffer(data[2]);
        if (data[2] < 0)
        {
            gScanlineEffect.state = 3;
            tState++;
        }
        break;
    case 2:
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDY, 0);
        SetTitleScreenScene(data, TITLESCREENSCENE_FADEIN);
    }
}

static void SetTitleScreenScene_FadeIn(s16 *data)
{
    switch (tState)
    {
    case 0:
        data[2] = 0;
        tState++;
        break;
    case 1:
        data[2]++;
        if (data[2] > 10)
        {
            TintPalette_GrayScale2(&gPlttBufferUnfaded[BG_PLTT_ID(13)], 16);
            BeginNormalPaletteFade(1 << 13, 9, 16, 0, RGB_BLACK);
            tState++;
        }
        break;
    case 2:
        if (!gPaletteFade.active)
        {
            data[2] = 0;
            tState++;
        }
        break;
    case 3:
        data[2]++;
        if (data[2] > 36)
        {
            ChangeBgX(2, 0, 0);
            BlendPalettesGradually((1 << 14) | (1 << 15), -4, 16, 0, RGB_BLACK, 3, 1);
            BlendPalettesGradually(1 << 13, -4, 1, 16, RGB(30, 30, 31), 0, 0);
            data[2] = 0;
            tState++;
        }
        break;
    case 4:
        if (!IsBlendPalettesGraduallyTaskActive(0) && !IsBlendPalettesGraduallyTaskActive(1))
        {
            BlendPalettesGradually(1 << 13, -4, 15, 0, RGB(30, 30, 31), 0, 0);
            tState++;
        }
        break;
    case 5:
        data[2]++;
        if (data[2] > 20)
        {
            data[2] = 0;
            BlendPalettesGradually(1 << 13, -4, 1, 16, RGB(30, 30, 31), 0, 0);
            tState++;
        }
        break;
    case 6:
        if (!IsBlendPalettesGraduallyTaskActive(0))
        {
            BlendPalettesGradually(1 << 13, -4, 15, 0, RGB(30, 30, 31), 0, 0);
            tState++;
        }
        break;
    case 7:
        data[2]++;
        if (data[2] > 20)
        {
            data[2] = 0;
            BlendPalettesGradually(1 << 13, -3, 0, 16, RGB(30, 30, 31), 0, 0);
            tState++;
        }
        break;
    case 8:
        if (!IsBlendPalettesGraduallyTaskActive(0))
        {
            u32 palettes;
            tHasCreatedBlankSprite = TRUE;
            palettes = (PALETTES_BG & ~(1 << 13) & ~(1 << 14) & ~(1 << 15)) | (0x10000 << CreateBlankSprite());
            BlendPalettes(palettes, 16, RGB(30, 30, 31));
            BeginNormalPaletteFade(palettes, 1, 16, 0, RGB(30, 30, 31));
            ShowBg(0);
            CpuCopy16(gGraphics_TitleScreen_BoxArtMonPals, &gPlttBufferUnfaded[BG_PLTT_ID(13)], PLTT_SIZE_4BPP);
            BlendPalettesGradually(1 << 13, 1, 15, 0, RGB(30, 30, 31), 0, 0);
            tState++;
        }
        break;
    case 9:
        if (!IsBlendPalettesGraduallyTaskActive(0) && !gPaletteFade.active)
        {
            SetTitleScreenScene(data, TITLESCREENSCENE_RUN);
        }
        break;
    }
}

#define KEYSTROKE_DELSAVE (B_BUTTON | SELECT_BUTTON | DPAD_UP)
#define KEYSTROKE_BERRY_FIX (B_BUTTON | SELECT_BUTTON)

static void SetTitleScreenScene_Run(s16 *data)
{
    switch (tState)
    {
    case 0:
        SetHelpContext(HELPCONTEXT_TITLE_SCREEN);
        CreateTask(Task_TitleScreen_BlinkPressStart, 0);
#if defined(FIRERED)
        CreateTask(Task_FlameSpawner, 5);
#elif defined(LEAFGREEN)
        CreateTask(Task_LeafSpawner, 5);
#endif
        SetGpuRegsForTitleScreenRun();
        tSlashSpriteId = CreateSlashSprite();
        HelpSystem_Enable();
        tState++;
        // fallthrough
    case 1:
        if (JOY_HELD(KEYSTROKE_DELSAVE) == KEYSTROKE_DELSAVE)
        {
            DeactivateSlashSprite(tSlashSpriteId);
            DestroyTask(FindTaskIdByFunc(Task_TitleScreenMain));
            SetMainCallback2(CB2_FadeOutTransitionToSaveClearScreen);
        }
#if REVISION >= 0xA
        // Berry fix trigger has been removed.
#else
        else if (JOY_HELD(KEYSTROKE_BERRY_FIX) == KEYSTROKE_BERRY_FIX)
        {
            DeactivateSlashSprite(tSlashSpriteId);
            DestroyTask(FindTaskIdByFunc(Task_TitleScreenMain));
            SetMainCallback2(CB2_FadeOutTransitionToBerryFix);
        }
#endif
        else if (JOY_NEW(A_BUTTON | START_BUTTON))
        {
            SetTitleScreenScene(data, TITLESCREENSCENE_CRY);
        }
        else if (!FuncIsActiveTask(Task_TitleScreenTimer))
        {
            SetTitleScreenScene(data, TITLESCREENSCENE_RESTART);
        }
        break;
    }
}

static void SetGpuRegsForTitleScreenRun(void)
{
    SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_OBJWIN_ON);
    SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG_ALL | WINOUT_WIN01_OBJ | WINOUT_WINOBJ_ALL);
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_LIGHTEN);
    SetGpuReg(REG_OFFSET_BLDY, 13);
}

static void SetTitleScreenScene_Restart(s16 *data)
{
    switch (tState)
    {
    case 0:
        DeactivateSlashSprite(tSlashSpriteId);
        tState++;
        break;
    case 1:
        if (!gPaletteFade.active && !IsSlashSpriteDeactivated(tSlashSpriteId))
        {
            FadeOutMapMusic(10);
            BeginNormalPaletteFade(PALETTES_ALL, 3, 0, 0x10, RGB_BLACK);
            SignalEndTitleScreenPaletteSomethingTask();
            data[1]++;
        }
        break;
    case 2:
        if (IsNotWaitingForBGMStop() && !gPaletteFade.active)
        {
            DestroyTask(FindTaskIdByFunc(Task_TitleScreen_BlinkPressStart));
            data[2] = 0;
            tState++;
        }
        break;
    case 3:
        data[2]++;
        if (data[2] >= 20)
        {
            DestroyTask(FindTaskIdByFunc(Task_TitleScreen_BlinkPressStart));
            tState++;
        }
        break;
    case 4:
        HelpSystem_Disable();
        DestroyTask(FindTaskIdByFunc(Task_TitleScreenMain));
        SetMainCallback2(CB2_InitCopyrightScreenAfterTitleScreen);
        break;
    }
}

static void SetTitleScreenScene_Cry(s16 *data)
{
    switch (tState)
    {
    case 0:
        if (!gPaletteFade.active)
        {
            PlayCry_Normal(TITLE_SPECIES, 0);
            DeactivateSlashSprite(tSlashSpriteId);
            data[2] = 0;
            tState++;
        }
        break;
    case 1:
        if (data[2] < 90)
            data[2]++;
        else if (!IsSlashSpriteDeactivated(tSlashSpriteId))
        {
            BeginNormalPaletteFade((PALETTES_ALL & ~(1 << 0x1C) & ~(1 << 0x1D) & ~(1 << 0x1E) & ~(1 << 0x1F)), 0, 0, 16, RGB_WHITE);
            SignalEndTitleScreenPaletteSomethingTask();
            FadeOutBGM(4);
            tState++;
        }
        break;
    case 2:
        if (!gPaletteFade.active)
        {
            SeedRngAndSetTrainerId();
            SetSaveBlocksPointers();
            ResetMenuAndMonGlobals();
            Save_ResetSaveCounters();
            LoadGameSave(SAVE_NORMAL);
            if (gSaveFileStatus == SAVE_STATUS_EMPTY || gSaveFileStatus == SAVE_STATUS_INVALID)
                Sav2_ClearSetDefault();
            SetPokemonCryStereo(gSaveBlock2Ptr->optionsSound);
            InitHeap(gHeap, HEAP_SIZE);
            SetMainCallback2(CB2_InitMainMenu);
            DestroyTask(FindTaskIdByFunc(Task_TitleScreenMain));
        }
        break;
    }
}

#undef tSceneNum
#undef tState
#undef tHasCreatedBlankSprite
#undef tSlashSpriteId

static void Task_TitleScreen_SlideWin0(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    switch (data[0])
    {
    case 0:
        SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON);
        SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_ALL);
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR);
        SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(0, DISPLAY_HEIGHT));
        SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(0, 0));
        BlendPalettes(1 << 0xE, 0, RGB_BLACK);
        data[0]++;
        break;
    case 1:
        data[1] += 24 << 4;
        data[2] = data[1] >> 4;
        if (data[2] >= DISPLAY_WIDTH)
        {
            data[2] = DISPLAY_WIDTH;
            data[0]++;
        }
        SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(0, data[2]));
        break;
    case 2:
        data[3]++;
        if (data[3] >= 10)
        {
            data[3] = 0;
            data[0]++;
        }
        break;
    case 3:
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR);
        SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(DISPLAY_WIDTH, DISPLAY_WIDTH));
        ChangeBgX(2, -0xF000, 0);
        BlendPalettes(1 << 0xF, 0, RGB_BLACK);
        data[1] = 10 * 24 << 4;
        data[0]++;
        break;
    case 4:
        data[1] -= 24 << 4;
        data[2] = data[1] >> 4;
        if (data[2] <= 0)
        {
            data[2] = 0;
            data[0]++;
        }
        ChangeBgX(2, -data[2] << 8, 0);
        SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(data[2], DISPLAY_WIDTH));
        break;
    case 5:
        ClearGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON);
        DestroyTask(taskId);
        break;
    }
}

static void Task_TitleScreen_BlinkPressStart(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (data[15] && gPaletteFade.active)
        data[14] = 1;
    if (data[14] && !gPaletteFade.active)
        DestroyTask(taskId);
    else
    {
        if (!data[1])
            data[2] = 60;
        else
            data[2] = 30;
        data[0]++;
        if (data[0] >= data[2])
        {
            data[0] = 0;
            data[1] ^= 1;
            if (data[1])
            {
                LoadBgTiles(2, sCopyrightPressStartTransparentTiles, sizeof(sCopyrightPressStartTransparentTiles), 0);
            }
            else
            {
                LoadBgTiles(2, sCopyrightPressStartTiles, sizeof(sCopyrightPressStartTiles), 0);
            }
            if (data[14])
            {
                BlendPalettes(0x00008000, gPaletteFade.y, gPaletteFade.blendColor);
            }
        }
    }
}

static void SignalEndTitleScreenPaletteSomethingTask(void)
{
    u8 taskId = FindTaskIdByFunc(Task_TitleScreen_BlinkPressStart);
    gTasks[taskId].data[15] = TRUE;
}

static void UpdateScanlineEffectRegBuffer(s16 y)
{
    s32 i;

    if (y >= 0)
        gScanlineEffectRegBuffers[gScanlineEffect.srcBuffer][y] = 16;

    for (i = 0; i < 16; i++)
    {
        if (y + i >= 0)
            gScanlineEffectRegBuffers[gScanlineEffect.srcBuffer][y + i] = 15 - i;

        if (y - i >= 0)
            gScanlineEffectRegBuffers[gScanlineEffect.srcBuffer][y - i] = 15 - i;

    }

    for (i = y + 16; i < 160; i++)
    {
        if (i >= 0)
            gScanlineEffectRegBuffers[gScanlineEffect.srcBuffer][i] = 0;
    }

    for (i = y - 16; i >= 0; i--)
    {
        if (i >= 0)
            gScanlineEffectRegBuffers[gScanlineEffect.srcBuffer][i] = 0;
    }
}

static void ScheduleStopScanlineEffect(void)
{
    if (gScanlineEffect.state)
        gScanlineEffect.state = 3;
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BLDY, 0);
}

static void LoadMainTitleScreenPalsAndResetBgs(void)
{
    u8 taskId;

    taskId = FindTaskIdByFunc(Task_TitleScreen_SlideWin0);
    if (taskId != TASK_NONE)
        DestroyTask(taskId);

    DestroyBlendPalettesGraduallyTask();
    ResetPaletteFadeControl();
    LoadPalette(gGraphics_TitleScreen_GameTitleLogoPals, BG_PLTT_ID(0), 13 * PLTT_SIZE_4BPP);
#if defined(FIRERED)
    LoadPalette(sBackdropPal, BG_PLTT_ID(4), PLTT_SIZE_4BPP);
#endif
    LoadPalette(gGraphics_TitleScreen_BoxArtMonPals, BG_PLTT_ID(13), PLTT_SIZE_4BPP);
    LoadPalette(gGraphics_TitleScreen_BackgroundPals, BG_PLTT_ID(15), PLTT_SIZE_4BPP);
#if defined(FIRERED)
    LoadPalette(sGrassPal, BG_PLTT_ID(14), PLTT_SIZE_4BPP);
#else
    LoadPalette(gGraphics_TitleScreen_BackgroundPals, BG_PLTT_ID(14), PLTT_SIZE_4BPP);
#endif
    ResetBgPositions();
    ClearGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_WIN1_ON | DISPCNT_OBJWIN_ON);
    ShowBg(1);
    ShowBg(2);
    ShowBg(0);
    ShowBg(3);
}

static void CB2_FadeOutTransitionToSaveClearScreen(void)
{
    if (!UpdatePaletteFade())
        SetMainCallback2(CB2_SaveClearScreen_Init);
}

static void CB2_FadeOutTransitionToBerryFix(void)
{
    if (!UpdatePaletteFade())
    {
        m4aMPlayAllStop();
        SetMainCallback2(CB2_InitBerryFixProgram);
    }
}

static void LoadSpriteGfxAndPals(void)
{
    s32 i;

    for (i = 0; i < NELEMS(sSpriteSheets); i++)
        LoadCompressedSpriteSheet(&sSpriteSheets[i]);
    LoadSpritePalettes(sSpritePals);
}

#if defined(FIRERED)

#define sPosX      data[0]
#define sSpeedX    data[1]
#define sPosY      data[2]
#define sSpeedY    data[3]
#define sFlickerStart data[4]
#define sDestroyTime  data[5]
#define sCreateBigSparkle data[6]
#define sTimer     data[7]

static void SpriteCallback_TitleScreenBigSparkle(struct Sprite *sprite)
{
    if (sprite->animEnded)
        DestroySprite(sprite);
}

static void SpriteCallback_TitleScreenFlame(struct Sprite *sprite)
{
    s16 *data = sprite->data;
    sPosX += sSpeedX;
    sprite->x = sPosX >> 5;
    if (sprite->x < -8 || sprite->x > DISPLAY_WIDTH + 8)
    {
        DestroySprite(sprite);
        return;
    }
    sPosY += sSpeedY;
    sprite->y = sPosY >> 5;
    if (sprite->y < -8 || sprite->y > DISPLAY_HEIGHT + 8)
    {
        DestroySprite(sprite);
        return;
    }
    sTimer++;
    if (sTimer > sFlickerStart)
    {
        sprite->invisible = !sprite->invisible;
        if (sTimer > sDestroyTime)
        {
            if (sCreateBigSparkle)
            {
                u8 spriteId = CreateSprite(&sSpriteTemplate_BigSparkle, sprite->x, sprite->y, 0);
                if (spriteId != MAX_SPRITES)
                    gSprites[spriteId].callback = SpriteCallback_TitleScreenBigSparkle;
            }
            DestroySprite(sprite);
            return;
        }
    }
}

static bool32 CreateSparkleSprite(s32 x, s32 y, s32 xspeed, s32 yspeed, s32 lifetime, bool32 createBigSparkle)
{
    u8 spriteId;
    spriteId = CreateSprite(&sSpriteTemplate_FlameOrLeaf, x, y, 0);

    if (spriteId != MAX_SPRITES)
    {
        gSprites[spriteId].sPosX = x * 32;
        gSprites[spriteId].sSpeedX = xspeed;
        gSprites[spriteId].sPosY = y * 32;
        gSprites[spriteId].sSpeedY = yspeed;
        gSprites[spriteId].sFlickerStart = lifetime - 30;
        gSprites[spriteId].sDestroyTime = lifetime;
        gSprites[spriteId].sCreateBigSparkle = createBigSparkle;
        gSprites[spriteId].sTimer = 0;
        gSprites[spriteId].callback = SpriteCallback_TitleScreenFlame;
        return TRUE;
    }
    return FALSE;
}

#undef sPosX
#undef sSpeedX
#undef sPosY
#undef sSpeedY
#undef sFlickerStart
#undef sDestroyTime
#undef sCreateBigSparkle
#undef sTimer

#define tState       data[0]
#define tTimer       data[1]
#define tDelay       data[2]
#define tOff_Seed       3   // data[3] and data[4]
#define tOffsetX     data[5]

static void Task_FlameSpawner(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    s32 x, y, xspeed, yspeed, lifetime;
    bool32 createBigSparkle;
    s32 i;

    switch (tState)
    {
    case 0:
        TitleScreen_srand(taskId, 3, 30840);
        tState++;
        break;
    case 1:
        tTimer++;
        if (tTimer >= tDelay)
        {
            tTimer = 0;
            TitleScreen_rand(taskId, 3);
            tDelay = 48;
            xspeed = (TitleScreen_rand(taskId, 3) % 8) + 2;
            if (TitleScreen_rand(taskId, 3) & 1)
                xspeed = -xspeed;
            yspeed = -((TitleScreen_rand(taskId, 3) % 6) + 2);
            if (TitleScreen_rand(taskId, 3) & 3)
                lifetime = (TitleScreen_rand(taskId, 3) % 81) + 210;
            else
                lifetime = (TitleScreen_rand(taskId, 3) % 101) + 340;
            createBigSparkle = (TitleScreen_rand(taskId, 3) & 7) == 0;
            y = (TitleScreen_rand(taskId, 3) % 48) + 88;
            x = TitleScreen_rand(taskId, 3) % DISPLAY_WIDTH;
            CreateSparkleSprite(
                x,
                y,
                xspeed,
                yspeed,
                lifetime,
                createBigSparkle
            );
            for (i = 0; i < 15; i += 10)
            {
                CreateSparkleSprite(
                    tOffsetX + sFlameXPositions[i],
                    y,
                    xspeed,
                    yspeed,
                    lifetime,
                    createBigSparkle
                );
                xspeed = (TitleScreen_rand(taskId, 3) % 8) + 2;
                if (TitleScreen_rand(taskId, 3) & 1)
                    xspeed = -xspeed;
                yspeed = -((TitleScreen_rand(taskId, 3) % 6) + 2);
                if (TitleScreen_rand(taskId, 3) & 3)
                    lifetime = (TitleScreen_rand(taskId, 3) % 81) + 210;
                else
                    lifetime = (TitleScreen_rand(taskId, 3) % 101) + 340;
                createBigSparkle = (TitleScreen_rand(taskId, 3) & 7) == 0;
            }
            tOffsetX++;
            if (tOffsetX > 3)
                tOffsetX = 0;
        }
    }
}

#undef tState
#undef tTimer
#undef tDelay
#undef tOff_Seed
#undef tOffsetX

#elif defined(LEAFGREEN)

#define sPosX        data[0]
#define sSpeedX      data[1]
#define sPosY        data[2]
#define sSpeedY      data[3]

static void SpriteCallback_TitleScreenLeaf(struct Sprite *sprite)
{
    s16 *data = sprite->data;
    sprite->sPosX -= sSpeedX;
    sprite->x = sprite->sPosX >> 4;
    if (sprite->x < -8)
    {
        DestroySprite(sprite);
        return;
    }
    sPosY += sSpeedY;
    sprite->y = sPosY >> 4;
    if (sprite->y < 16 || sprite->y > 200)
    {
        DestroySprite(sprite);
        return;
    }
    if (!data[5])
    { // meaningless, since data[5] and data[6] are never used outside this block
        s32 r2;
        s32 r1;
        data[6]++;
        r2 = sSpeedX * data[6];
        r1 = sSpeedY * data[6];
        r2 = (r2 * r2) >> 4;
        r1 = (r1 * r1) >> 4;
        if (r2 + r1 >= 81 << 4)
            data[5] = TRUE;
    }
}

static void CreateLeafSprite(s32 y, s32 xspeed, s32 yspeed)
{
    u8 spriteId = CreateSprite(&sSpriteTemplate_FlameOrLeaf, DISPLAY_WIDTH, y, 0);
    if (spriteId != MAX_SPRITES)
    {
        gSprites[spriteId].sPosX = DISPLAY_WIDTH * 16;
        gSprites[spriteId].sSpeedX = xspeed;
        gSprites[spriteId].sPosY = y * 16;
        gSprites[spriteId].sSpeedY = yspeed;
        gSprites[spriteId].callback = SpriteCallback_TitleScreenLeaf;
    }
}

#undef sPosX
#undef sSpeedX
#undef sPosY
#undef sSpeedY

static void SpriteCallback_Streak(struct Sprite *sprite)
{
    sprite->x -= 7;
    if (sprite->x < -16)
    {
        sprite->x = DISPLAY_WIDTH + 16;
        sprite->data[7]++;
        if (sprite->data[7] >= ARRAY_COUNT(sStreakYPositions))
            sprite->data[7] = 0;
        sprite->y = sStreakYPositions[sprite->data[7]];
    }
}

static void CreateStreakSprites(void)
{
    int i;
    u8 spriteId;
    for (i = 0; i < 4; i++)
    {
        spriteId = CreateSprite(&sSpriteTemplate_Streak, DISPLAY_WIDTH + 16 + 40 * i, sStreakYPositions[i], 0xFF);
        if (spriteId != MAX_SPRITES)
        {
            gSprites[spriteId].data[7] = i;
            gSprites[spriteId].callback = SpriteCallback_Streak;
        }
    }
}

#define tState       data[0]
#define tTimer       data[1]
#define tDelay       data[2]
#define tOff_Seed       3   // data[3] and data[4]

static void Task_LeafSpawner(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    s32 rval;
    s32 xspeed;
    s32 yspeed;
    s32 y;

    switch (tState)
    {
    case 0:
        CreateStreakSprites();
        TitleScreen_srand(taskId, tOff_Seed, 30840);
        tState++;
        break;
    case 1:
        tTimer++;
        if (tTimer >= tDelay)
        {
            tTimer = 0;
            tDelay = (TitleScreen_rand(taskId, tOff_Seed) % 6) + 6;
            rval = TitleScreen_rand(taskId, tOff_Seed) % 30;
            xspeed = 16;
            if (rval >= 6)
            {
                xspeed = 48;
                if (rval < 12)
                    xspeed = 24;
            }
            yspeed = (TitleScreen_rand(taskId, tOff_Seed) % 4) - 2;
            y = (TitleScreen_rand(taskId, tOff_Seed) % 88) + 32;
            CreateLeafSprite(y, xspeed, yspeed);
        }
        break;
    }
}

#undef tState
#undef tData1
#undef tData2
#undef tData3And4

#endif //FRLG

static void TitleScreen_srand(u8 taskId, u8 field, u16 seed)
{
    SetWordTaskArg(taskId, field, seed);
}

static u16 TitleScreen_rand(u8 taskId, u8 field)
{
    u32 rngval;

    rngval = GetWordTaskArg(taskId, field);
    rngval = ISO_RANDOMIZE1(rngval);
    SetWordTaskArg(taskId, field, rngval);
    return rngval >> 16;
}

static u32 CreateBlankSprite(void)
{
    CreateSprite(&sSpriteTemplate_BlankSprite, 24, 144, 0);
    return IndexOfSpritePaletteTag(PAL_TAG_SLASH);
}

static void SetPalOnOrCreateBlankSprite(bool32 hasCreatedBlankSprite)
{
    u32 palIdx;

    if (hasCreatedBlankSprite)
    {
        palIdx = IndexOfSpritePaletteTag(PAL_TAG_SLASH);
        LoadPalette(gTitleScreen_Slash_Pal, OBJ_PLTT_ID(palIdx), PLTT_SIZE_4BPP);
    }
    else
        CreateBlankSprite();
}

#define sState       data[0]
#define sTimer       data[1]
#define sDeactivate  data[2]

static u8 CreateSlashSprite(void)
{
    u8 spriteId = CreateSprite(&sSlashSpriteTemplate, -32, 27, 1);
    if (spriteId != MAX_SPRITES)
    {
        gSprites[spriteId].callback = SpriteCallback_Slash;
        gSprites[spriteId].sTimer = 540;
    }
    return spriteId;
}

static void DeactivateSlashSprite(u8 spriteId)
{
    if (spriteId != MAX_SPRITES)
        gSprites[spriteId].sDeactivate = TRUE;
}

static bool32 IsSlashSpriteDeactivated(u8 spriteId)
{
    if (spriteId != MAX_SPRITES)
        return gSprites[spriteId].sState ^ 2 ? TRUE : FALSE;
    else
        return FALSE;
}

static void SpriteCallback_Slash(struct Sprite *sprite)
{
    switch (sprite->sState)
    {
    case 0:
        if (sprite->sDeactivate)
        {
            sprite->invisible = TRUE;
            sprite->sState = 2;
        }
        sprite->sTimer--;
        if (sprite->sTimer == 0)
        {
            sprite->invisible = FALSE;
            sprite->sState = 1;
        }
        break;
    case 1:
        sprite->x += 9;
        if (sprite->x == 67)
            sprite->y -= 7;

        if (sprite->x == 148)
            sprite->y += 7;

        if (sprite->x > DISPLAY_WIDTH + 32)
        {
            sprite->invisible = TRUE;
            if (sprite->sDeactivate)
                sprite->sState = 2;
            else
            {
                sprite->x = -32;
                sprite->sTimer = 540;
                sprite->sState = 0;
            }
        }
        break;
    case 2:
        break;
    }
}

#undef sState
#undef sTimer
#undef sDeactivate
