#include "task.h"
#include "gflib.h"
#include "menu_helpers.h"
#include "new_menu_helpers.h"
#include "scanline_effect.h"
#include "decompress.h"
#include "list_menu.h"
#include "item_menu.h"
#include "item.h"
#include "menu_indicators.h"
#include "event_object_movement.h"
#include "random.h"
#include "constants/songs.h"
#include "constants/items.h"
#include "event_data.h"
#include "load_save.h"
#include "battle_transition.h"
#include "battle_main.h"
#include "battle.h"
#include "battle_controllers.h"
#include "global.fieldmap.h"
#include "help_menu.h"
#include "overworld.h"
#include "graphics.h"
#include "fieldmap.h"
#include "strings.h"
#include "ui_hint_header.h"
#include "constants/field_effects.h"
#include "constants/event_objects.h"

struct HelpMenuCtrlBlk
{
    MainCallback callback;
    u8 mode;
    u8 whichScript;
    u8 menuMode;
    u16 scrollOffset;
    u16 selectedRow;
    u16 lessonScrollOffset;
    u16 lessonSelectedRow;
};

struct HelpMenuBuf
{
    MainCallback savedCallback;
    u16 screenTilemap[BG_SCREEN_SIZE];
    u16 buffer2[BG_SCREEN_SIZE];
    u16 buffer3[BG_SCREEN_SIZE];
    u16 titleTilemap[BG_SCREEN_SIZE];
    u8 grassAnimCounterLo;
    u8 grassAnimCounterHi;
    u8 grassAnimDisabled;
    u8 scrollIndicatorArrowPairId;
};

static EWRAM_DATA struct HelpMenuCtrlBlk sStaticResources = {0};
static EWRAM_DATA struct HelpMenuBuf * sResources = NULL;

#define HELP_MENU_STATIC_TILE 185

static void HelpMenuCmd_TransitionRenderBg2HelpMenuGraphicInitNpcPos(u8 taskId);
static void HelpMenuCmd_ClearBg2HelpMenuGraphic(u8 taskId);
static void HelpMenuCmd_NpcMoveAndSetupTextPrinter(u8 taskId);
static void HelpMenuCmd_IdleIfTextPrinterIsActive(u8 taskId);
static void HelpMenuCmd_TextPrinterSwitchStringByOptionChosen(u8 taskId);
static void HelpMenuCmd_TextPrinterSwitchStringByOptionChosen2(u8 taskId);
static void HelpMenuCmd_IdleIfTextPrinterIsActive2(u8 taskId);
static void HelpMenuCmd_EraseTextWindowIfKeyPressed(u8 taskId);
static void HelpMenuCmd_StartAnimNpcWalkIntoGrass(u8 taskId);
static void HelpMenuCmd_DudeMoveUp(u8 taskId);
static void HelpMenuCmd_DudeMoveRight(u8 taskId);
static void HelpMenuCmd_DudeTurnLeft(u8 taskId);
static void HelpMenuCmd_DudeMoveLeft(u8 taskId);
static void HelpMenuCmd_RenderAndRemoveBg1EndGraphic(u8 taskId);
static void HelpMenuCmd_TaskBattleOrFadeByOptionChosen(u8 taskId);
static void HelpMenuCallback(void);
static void HelpMenuMainCallback(void);
static void HelpMenuVblankHandler(void);
static void HelpMenuCreateAndRenderRbox(void);
static void HelpMenuInitIo(void);
static u8 HelpMenuSetupObjEventAndOam(void);
static void HelpMenuSetupPostBattleWindowAndObj(u8);
static u8 HelpMenuSetupWindow(void);
static void HelpMenuSetupScrollIndicatorArrowPair(void);
static void HelpMenuSetWindowRegs(void);
static void HelpMenuSetupBg(void);
static void HelpMenuLoadGraphic(void);
static void HelpMenuPrintControlHints(void);
static void HelpMenuPostBattleFadeControl(u8);
static void HelpMenuOptionListController(u8);
static u8 HelpMenuSetupMainWindow(void);
static u8 HelpMenuSetupLessonWindow(void);
static void HelpMenuOpenLessonSubmenu(u8 taskId, u8 lesson);
static void HelpMenuReturnToMainMenuFromSubmenu(u8 taskId);
static void HelpMenuAudioByInput(s32, bool8, struct ListMenu *);
static void HelpMenuQuitFadeControlAndTaskDel(u8 taskId);
static void HelpMenuRenderMsgAndSwitchClusterFuncs(u8 taskId);
static void HelpMenuClearBg1EndGraphicText(void);
static void HelpMenuCmd_End(u8 taskId);
static void HelpMenuSetupBagItemsByOptionChosen(void);
static void HelpMenuPrepBattle(u8 taskId);
static void HelpMenuGrassAnimationMain(u8 taskId, s16 x, s16 y, u8 subpriority, bool8 mode);
static void HelpMenuLoadBg3Map(u16 *);
static u8 HelpMenuGrassAnimationCheckIfNeedsToGenerateGrassObj(s16 x, s16 y);
static void HelpMenuGrassAnimationObjCallback(struct Sprite *sprite);
static void HelpMenuRestorePlayerPartyCallback(void);
static void HelpMenuPreBattleAnimAndSetBattleCallback(u8 taskId);
static void HelpMenuLoadMapTilesetToBuffer(const struct Tileset *ts, u8 *dstBuffer, u16 size);
static void HelpMenuPushBackNewMapPalIndexArrayEntry(const struct MapLayout *mStruct, u16 *buf1, u8 *palIndexArray, u16 mapEntry, u16 offset);
static void HelpMenuComputeMapTilesFromTilesetAndMetaTiles(const u16 *metaTilesArray, u8 *blockBuf, u8 *tileset);
static void HelpMenuComputeSingleMapTileBlockFromTilesetAndMetaTiles(u8 *blockBuf, u8 *tileset, u8 metaTile);
static u16 HelpMenuComputePalIndexArrayEntryByMetaTile(u8 *palIndexArrayBuf, u16 metaTile);
static void HelpMenuLoadMapPalette(const struct MapLayout * mStruct, const u8 *palIndexArray);

static const struct BgTemplate sBgTemplates[] = 
{
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0x000,
    },
    {
        .bg = 1,
        .charBaseIndex = 0,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0x000,
    },
    {
        .bg = 2,
        .charBaseIndex = 0,
        .mapBaseIndex = 29,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0x000,
    },
    {
        .bg = 3,
        .charBaseIndex = 2,
        .mapBaseIndex = 28,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0x000,
    },
};

static const struct WindowTemplate sWindowTemplates[] = 
{
    {
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 26,
        .height = 4,
        .paletteNum = 3,
        .baseBlock = 0x0EA,
    },
    {
        .bg = 0,
        .tilemapLeft = 4,
        .tilemapTop = 3,
        .width = 22,
        .height = 10,
        .paletteNum = 3,
        .baseBlock = 0x152,
    },
    {
        .bg = 1,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 30,
        .height = 2,
        .paletteNum = 4,
        .baseBlock = 0x22E,
    },
    DUMMY_WIN_TEMPLATE,
};

static const u8 sText_HelpMenuControlHints[] = _("{DPAD_UPDOWN}PICK {A_BUTTON}OK {B_BUTTON}BACK");

static const struct ListMenuItem sListMenuItems[] = 
{
    {
        .label = gHelpMenuString_CatchPkmn,
        .index = HELPSCR_CATCHING
    },
    {
        .label = gHelpMenuString_TeachBattle,
        .index = HELPSCR_BATTLE
    },
    {
        .label = gHelpMenuString_Training,
        .index = HELPSCR_TRAINING
    },
    {
        .label = gHelpMenuString_StatusProblems,
        .index = HELPSCR_STATUS
    },
    {
        .label = gHelpMenuString_TypeMatchups,
        .index = HELPSCR_MATCHUPS
    },
};

static const struct ListMenuItem sListMenuItems_Lesson[] =
{
    {
        .label = gHelpMenuString_Demonstration,
        .index = 0
    },
    {
        .label = gHelpMenuString_Theory,
        .index = 1
    },
};

static const struct ListMenuTemplate sListMenuTemplate = 
{
    .items = sListMenuItems,
    .moveCursorFunc = NULL,
    .itemPrintFunc = NULL,
    .totalItems = NELEMS(sListMenuItems),
    .maxShowed = NELEMS(sListMenuItems),
    .windowId = 0,
    .header_X = 0,
    .item_X = 8,
    .cursor_X = 0,
    .upText_Y = 6,
    .cursorPal = 0x1,
    .fillValue = 0x0,
    .cursorShadowPal = 0x2,
    .lettersSpacing = 0x0,
    .itemVerticalPadding = 0x0,
    .scrollMultiple = 0x1,
    .fontId = FONT_NORMAL,
    .cursorKind = 0x0,
};

static const u8 sWhereToReturnToFromBattle[] = 
{
    12,
    12,
    12,
    12,
     9,
     9
};

static void (* const sBattleScript[])(u8) = 
{
    HelpMenuCmd_TransitionRenderBg2HelpMenuGraphicInitNpcPos,
    HelpMenuCmd_ClearBg2HelpMenuGraphic,
    HelpMenuCmd_NpcMoveAndSetupTextPrinter,
    HelpMenuCmd_IdleIfTextPrinterIsActive,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_TextPrinterSwitchStringByOptionChosen,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_EraseTextWindowIfKeyPressed,
    HelpMenuCmd_StartAnimNpcWalkIntoGrass,
    HelpMenuCmd_DudeMoveUp,
    HelpMenuCmd_DudeMoveRight,
    HelpMenuCmd_TaskBattleOrFadeByOptionChosen,
    HelpMenuCmd_TextPrinterSwitchStringByOptionChosen2,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_EraseTextWindowIfKeyPressed,
    HelpMenuCmd_DudeTurnLeft,
    HelpMenuCmd_DudeMoveLeft,
    HelpMenuCmd_RenderAndRemoveBg1EndGraphic,
    HelpMenuCmd_End,
};

static void (* const sStatusScript[])(u8) = 
{
    HelpMenuCmd_TransitionRenderBg2HelpMenuGraphicInitNpcPos,
    HelpMenuCmd_ClearBg2HelpMenuGraphic,
    HelpMenuCmd_NpcMoveAndSetupTextPrinter,
    HelpMenuCmd_IdleIfTextPrinterIsActive,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_TextPrinterSwitchStringByOptionChosen,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_EraseTextWindowIfKeyPressed,
    HelpMenuCmd_StartAnimNpcWalkIntoGrass,
    HelpMenuCmd_DudeMoveUp,
    HelpMenuCmd_DudeMoveRight,
    HelpMenuCmd_TaskBattleOrFadeByOptionChosen,
    HelpMenuCmd_TextPrinterSwitchStringByOptionChosen2,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_EraseTextWindowIfKeyPressed,
    HelpMenuCmd_DudeTurnLeft,
    HelpMenuCmd_DudeMoveLeft,
    HelpMenuCmd_RenderAndRemoveBg1EndGraphic,
    HelpMenuCmd_End,
};

static void (* const sMatchupsScript[])(u8) = 
{
    HelpMenuCmd_TransitionRenderBg2HelpMenuGraphicInitNpcPos,
    HelpMenuCmd_ClearBg2HelpMenuGraphic,
    HelpMenuCmd_NpcMoveAndSetupTextPrinter,
    HelpMenuCmd_IdleIfTextPrinterIsActive,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_TextPrinterSwitchStringByOptionChosen,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_EraseTextWindowIfKeyPressed,
    HelpMenuCmd_StartAnimNpcWalkIntoGrass,
    HelpMenuCmd_DudeMoveUp,
    HelpMenuCmd_DudeMoveRight,
    HelpMenuCmd_TaskBattleOrFadeByOptionChosen,
    HelpMenuCmd_TextPrinterSwitchStringByOptionChosen2,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_EraseTextWindowIfKeyPressed,
    HelpMenuCmd_DudeTurnLeft,
    HelpMenuCmd_DudeMoveLeft,
    HelpMenuCmd_RenderAndRemoveBg1EndGraphic,
    HelpMenuCmd_End,
};

static void (* const sCatchingScript[])(u8) = 
{
    HelpMenuCmd_TransitionRenderBg2HelpMenuGraphicInitNpcPos,
    HelpMenuCmd_ClearBg2HelpMenuGraphic,
    HelpMenuCmd_NpcMoveAndSetupTextPrinter,
    HelpMenuCmd_IdleIfTextPrinterIsActive,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_TextPrinterSwitchStringByOptionChosen,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_EraseTextWindowIfKeyPressed,
    HelpMenuCmd_StartAnimNpcWalkIntoGrass,
    HelpMenuCmd_DudeMoveUp,
    HelpMenuCmd_DudeMoveRight,
    HelpMenuCmd_TaskBattleOrFadeByOptionChosen,
    HelpMenuCmd_TextPrinterSwitchStringByOptionChosen2,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_EraseTextWindowIfKeyPressed,
    HelpMenuCmd_DudeTurnLeft,
    HelpMenuCmd_DudeMoveLeft,
    HelpMenuCmd_RenderAndRemoveBg1EndGraphic,
    HelpMenuCmd_End,
};

static void (* const sTMsScript[])(u8) = 
{
    HelpMenuCmd_TransitionRenderBg2HelpMenuGraphicInitNpcPos,
    HelpMenuCmd_ClearBg2HelpMenuGraphic,
    HelpMenuCmd_NpcMoveAndSetupTextPrinter,
    HelpMenuCmd_IdleIfTextPrinterIsActive,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_TextPrinterSwitchStringByOptionChosen,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_EraseTextWindowIfKeyPressed,
    HelpMenuCmd_TaskBattleOrFadeByOptionChosen,
    HelpMenuCmd_TextPrinterSwitchStringByOptionChosen2,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_EraseTextWindowIfKeyPressed,
    HelpMenuCmd_DudeTurnLeft,
    HelpMenuCmd_DudeMoveLeft,
    HelpMenuCmd_RenderAndRemoveBg1EndGraphic,
    HelpMenuCmd_End,
};

static void (* const sMatchupsTheoryScript[])(u8) =
{
    HelpMenuCmd_TransitionRenderBg2HelpMenuGraphicInitNpcPos,
    HelpMenuCmd_ClearBg2HelpMenuGraphic,
    HelpMenuCmd_NpcMoveAndSetupTextPrinter,
    HelpMenuCmd_IdleIfTextPrinterIsActive,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_TextPrinterSwitchStringByOptionChosen,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_EraseTextWindowIfKeyPressed,
    HelpMenuCmd_TextPrinterSwitchStringByOptionChosen2,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_EraseTextWindowIfKeyPressed,
    HelpMenuCmd_DudeTurnLeft,
    HelpMenuCmd_DudeMoveLeft,
    HelpMenuCmd_RenderAndRemoveBg1EndGraphic,
    HelpMenuCmd_End,
};

static void (* const sBattleTheoryScript[])(u8) =
{
    HelpMenuCmd_TransitionRenderBg2HelpMenuGraphicInitNpcPos,
    HelpMenuCmd_ClearBg2HelpMenuGraphic,
    HelpMenuCmd_NpcMoveAndSetupTextPrinter,
    HelpMenuCmd_IdleIfTextPrinterIsActive,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_TextPrinterSwitchStringByOptionChosen,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_EraseTextWindowIfKeyPressed,
    HelpMenuCmd_TextPrinterSwitchStringByOptionChosen2,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_EraseTextWindowIfKeyPressed,
    HelpMenuCmd_DudeTurnLeft,
    HelpMenuCmd_DudeMoveLeft,
    HelpMenuCmd_RenderAndRemoveBg1EndGraphic,
    HelpMenuCmd_End,
};

static void (* const sCatchingTheoryScript[])(u8) =
{
    HelpMenuCmd_TransitionRenderBg2HelpMenuGraphicInitNpcPos,
    HelpMenuCmd_ClearBg2HelpMenuGraphic,
    HelpMenuCmd_NpcMoveAndSetupTextPrinter,
    HelpMenuCmd_IdleIfTextPrinterIsActive,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_TextPrinterSwitchStringByOptionChosen,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_EraseTextWindowIfKeyPressed,
    HelpMenuCmd_TextPrinterSwitchStringByOptionChosen2,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_EraseTextWindowIfKeyPressed,
    HelpMenuCmd_DudeTurnLeft,
    HelpMenuCmd_DudeMoveLeft,
    HelpMenuCmd_RenderAndRemoveBg1EndGraphic,
    HelpMenuCmd_End,
};

static void (* const sStatusTheoryScript[])(u8) =
{
    HelpMenuCmd_TransitionRenderBg2HelpMenuGraphicInitNpcPos,
    HelpMenuCmd_ClearBg2HelpMenuGraphic,
    HelpMenuCmd_NpcMoveAndSetupTextPrinter,
    HelpMenuCmd_IdleIfTextPrinterIsActive,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_TextPrinterSwitchStringByOptionChosen,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_EraseTextWindowIfKeyPressed,
    HelpMenuCmd_TextPrinterSwitchStringByOptionChosen2,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_EraseTextWindowIfKeyPressed,
    HelpMenuCmd_DudeTurnLeft,
    HelpMenuCmd_DudeMoveLeft,
    HelpMenuCmd_RenderAndRemoveBg1EndGraphic,
    HelpMenuCmd_End,
};

static void (* const sTrainingScript[])(u8) =
{
    HelpMenuCmd_TransitionRenderBg2HelpMenuGraphicInitNpcPos,
    HelpMenuCmd_ClearBg2HelpMenuGraphic,
    HelpMenuCmd_NpcMoveAndSetupTextPrinter,
    HelpMenuCmd_IdleIfTextPrinterIsActive,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_TextPrinterSwitchStringByOptionChosen,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_EraseTextWindowIfKeyPressed,
    HelpMenuCmd_TextPrinterSwitchStringByOptionChosen2,
    HelpMenuCmd_IdleIfTextPrinterIsActive2,
    HelpMenuCmd_EraseTextWindowIfKeyPressed,
    HelpMenuCmd_DudeTurnLeft,
    HelpMenuCmd_DudeMoveLeft,
    HelpMenuCmd_RenderAndRemoveBg1EndGraphic,
    HelpMenuCmd_End,
};

static void HelpMenuCallback(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void HelpMenuVblankHandler(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void InitHelpMenuController(u8 mode, MainCallback cb)
{
    sStaticResources.mode = mode;
    sStaticResources.callback = cb;
    if (mode == 0)
    {
        sStaticResources.scrollOffset = 0;
        sStaticResources.selectedRow = 0;
        sStaticResources.lessonScrollOffset = 0;
        sStaticResources.lessonSelectedRow = 0;
        sStaticResources.menuMode = 0;
        sStaticResources.whichScript = HELPSCR_BATTLE;
    }
    if (mode == 1)
    {
        sStaticResources.mode = 0;
    }
    SetMainCallback2(HelpMenuMainCallback);
}

void CB2_ReturnToHelpMenu(void)
{
    if (sStaticResources.mode == 1)
        InitHelpMenuController(1, sStaticResources.callback);
    else
        InitHelpMenuController(2, sStaticResources.callback);
}

void SetHelpMenuControllerModeToResume(void)
{
    sStaticResources.mode = 1;
}

static void HelpMenuMainCallback(void)
{
    u8 taskId;
    struct Task *taskAddr;

    switch (gMain.state)
    {
    case 0:
        sResources = AllocZeroed(sizeof(struct HelpMenuBuf));
        sResources->savedCallback = NULL;
        sResources->grassAnimDisabled = 0;
        sResources->scrollIndicatorArrowPairId = 0xFF;
        SetVBlankHBlankCallbacksToNull();
        ClearScheduledBgCopiesToVram();
        ScanlineEffect_Stop();
        FreeAllSpritePalettes();
        ResetPaletteFade();
        ResetSpriteData();
        ResetTasks();
        HelpMenuSetupBg();
        HelpMenuLoadGraphic();
        ++gMain.state;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() == TRUE)
            return;
        HelpMenuCreateAndRenderRbox();
        HelpMenuInitIo();
        if (sStaticResources.mode == 2)
        {
            taskId = CreateTask(HelpMenuPostBattleFadeControl, 0);
            gTasks[taskId].data[1] = HelpMenuSetupObjEventAndOam();
            HelpMenuSetupPostBattleWindowAndObj(taskId);
        }
        else
        {
            taskId = CreateTask(HelpMenuOptionListController, 0);
            gTasks[taskId].data[0] = HelpMenuSetupWindow();
            gTasks[taskId].data[1] = HelpMenuSetupObjEventAndOam();
            HelpMenuSetupScrollIndicatorArrowPair();
            PlayNewMapMusic(MUS_HELP_MENU_MENU);
            HelpMenuSetWindowRegs();
        }
        ScheduleBgCopyTilemapToVram(0);
        ScheduleBgCopyTilemapToVram(1);
        ScheduleBgCopyTilemapToVram(2);
        ScheduleBgCopyTilemapToVram(3);
        BlendPalettes(PALETTES_ALL, 0x10, 0);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0x10, 0, 0);
        SetVBlankCallback(HelpMenuVblankHandler);
        SetMainCallback2(HelpMenuCallback);
        break;
    }
}

static void HelpMenuSetupBg(void)
{
    ResetAllBgsCoordinatesAndBgCntRegs();
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sBgTemplates, 4);
    SetBgTilemapBuffer(1, sResources->screenTilemap);
    SetBgTilemapBuffer(2, sResources->buffer2);
    SetBgTilemapBuffer(3, sResources->buffer3);
    SetGpuReg(REG_OFFSET_DISPCNT, 0x3040);
    ShowBg(0);
    ShowBg(1);
    ShowBg(2);
    ShowBg(3);
    ChangeBgX(3, 0x1000, 2);
    ChangeBgY(3, 0x2800, 1);
    sResources->grassAnimCounterLo = 0;
    sResources->grassAnimCounterHi = 3;
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
}

static void HelpMenuLoadGraphic(void)
{
    u16 src = RGB_BLACK;
    ResetTempTileDataBuffers();
    DecompressAndCopyTileDataToVram(1, gHelpMenu_Gfx, 0, 0, 0);
    LZDecompressWram(gHelpMenuScreen_Tilemap, sResources->screenTilemap);
    LZDecompressWram(gHelpMenuTitle_Tilemap, sResources->titleTilemap);
    LoadCompressedPalette(gHelpMenu_Pal, BG_PLTT_ID(0), 4 * PLTT_SIZE_4BPP);
    LoadPalette(gUiHintHeaderPalette, BG_PLTT_ID(4), PLTT_SIZE_4BPP);
    LoadPalette(&src, BG_PLTT_ID(0), sizeof(src));
    LoadSpritePalette(&gSpritePalette_GeneralFieldEffect1);
    HelpMenuLoadBg3Map(sResources->buffer3);
}

static void HelpMenuCreateAndRenderRbox(void)
{
    InitWindows(sWindowTemplates);
    DeactivateAllTextPrinters();
    FillWindowPixelBuffer(0, 0xCC);
    PutWindowTilemap(0);
    PutWindowTilemap(1);
    HelpMenuPrintControlHints();
    CopyWindowToVram(0, COPYWIN_GFX);
}

static void HelpMenuPrintControlHints(void)
{
    DrawUiHintHeader(2, sText_HelpMenuControlHints, 8, 10, 0, TRUE);
}

static u8 HelpMenuSetupWindow(void)
{
    if (sStaticResources.menuMode != 0)
        return HelpMenuSetupLessonWindow();
    return HelpMenuSetupMainWindow();
}

static u8 HelpMenuSetupMainWindow(void)
{
    gMultiuseListMenuTemplate = sListMenuTemplate;
    gMultiuseListMenuTemplate.windowId = 1;
    gMultiuseListMenuTemplate.moveCursorFunc = HelpMenuAudioByInput;
    return ListMenuInit(
               &gMultiuseListMenuTemplate,
               sStaticResources.scrollOffset,
               sStaticResources.selectedRow
    );
}

static u8 HelpMenuSetupLessonWindow(void)
{
    if (sStaticResources.lessonSelectedRow >= NELEMS(sListMenuItems_Lesson))
        sStaticResources.lessonSelectedRow = 0;

    gMultiuseListMenuTemplate = sListMenuTemplate;
    gMultiuseListMenuTemplate.windowId = 1;
    gMultiuseListMenuTemplate.moveCursorFunc = HelpMenuAudioByInput;
    gMultiuseListMenuTemplate.items = sListMenuItems_Lesson;
    gMultiuseListMenuTemplate.totalItems = NELEMS(sListMenuItems_Lesson);
    gMultiuseListMenuTemplate.maxShowed = NELEMS(sListMenuItems_Lesson);
    gMultiuseListMenuTemplate.upText_Y = (gMultiuseListMenuTemplate.upText_Y + 8) & 0xF;
    return ListMenuInit(
               &gMultiuseListMenuTemplate,
               sStaticResources.lessonScrollOffset,
               sStaticResources.lessonSelectedRow
    );
}

static void HelpMenuSetupScrollIndicatorArrowPair(void)
{
    sResources->scrollIndicatorArrowPairId = 0xFF;
}

static void HelpMenuRemoveScrollIndicatorArrowPair(void)
{
    if (sResources->scrollIndicatorArrowPairId != 0xFF)
    {
        RemoveScrollIndicatorArrowPair(sResources->scrollIndicatorArrowPairId);
        sResources->scrollIndicatorArrowPairId = 0xFF;
    }
}

static void HelpMenuAudioByInput(s32 notUsed, bool8 play, struct ListMenu *notUsedAlt)
{
    if (play != TRUE)
        PlaySE(SE_SELECT);
}

static void HelpMenuInitIo(void)
{
    SetGpuReg(REG_OFFSET_WININ, 0x3F);
    SetGpuReg(REG_OFFSET_WINOUT, 0x1F);
    SetGpuReg(REG_OFFSET_BLDCNT, 0xCC);
    SetGpuReg(REG_OFFSET_BLDY, 0x5);
}

static u8 HelpMenuSetupObjEventAndOam(void)
{
    u8 objId = CreateObjectGraphicsSprite(OBJ_EVENT_GFX_HELP_MENU_HOST, SpriteCallbackDummy, 0, 0, 8);
    gSprites[objId].oam.priority = 2;
    gSprites[objId].invisible = 1;
    return objId;
}

static void HelpMenuSetSpriteCoordsAndSwitchFrame(u8 objId, u16 x, u16 y, u8 frame)
{
    gSprites[objId].x2 = x;
    gSprites[objId].y2 = y;
    gSprites[objId].invisible = 0;
    StartSpriteAnim(&gSprites[objId], frame);
}

static void HelpMenuSetWindowRegs(void)
{
    SetGpuReg(REG_OFFSET_WIN0V, 0x1C64);
    SetGpuReg(REG_OFFSET_WIN0H, 0x1CD4);
}

static void HelpMenuClearWindowRegs(void)
{
    SetGpuReg(REG_OFFSET_WIN0V, 0x0);
    SetGpuReg(REG_OFFSET_WIN0H, 0x0);
}

static void HelpMenuBg2AnimController(void)
{
    u16 * tilemapBuffer = GetBgTilemapBuffer(2);
    u8 i, j;
    for (i = 3; i < 13; i++)
    {
        for (j = 2; j < 28; j++)
        {
            tilemapBuffer[32 * i + j] = ((Random() & 3) << 10) + (3 << 12) + HELP_MENU_STATIC_TILE;
        }
    }
    ScheduleBgCopyTilemapToVram(2);
}

static void HelpMenuSetupPostBattleWindowAndObj(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    struct Sprite *objAddr = &gSprites[data[1]];

    ClearWindowTilemap(1);
    HelpMenuClearWindowRegs();
    switch (sStaticResources.whichScript)
    {
    case HELPSCR_BATTLE:
    case HELPSCR_STATUS:
    case HELPSCR_MATCHUPS:
    case HELPSCR_CATCHING:
        HelpMenuSetSpriteCoordsAndSwitchFrame(data[1], 0x78, 0x38, 0);
        ChangeBgX(3, 0x3000, 1);
        ChangeBgY(3, 0x3000, 2);
        sResources->grassAnimCounterLo += 3;
        sResources->grassAnimCounterHi -= 3;
        break;
    case HELPSCR_TMS:
        HelpMenuSetSpriteCoordsAndSwitchFrame(data[1], 0x78, 0x38, 0);
        break;
    }

    data[4] = 0;
    data[5] = 0;
    HelpMenuGrassAnimationMain(taskId, objAddr->x2, objAddr->y2, 0, 1);
}

static void HelpMenuInitTextPrinter(const u8 *text)
{
    gTextFlags.autoScroll = 0;
    AddTextPrinterParameterized2(0, FONT_NORMAL, text, GetTextSpeedSetting(), 0, 1, 0xC, 3);
}

static void HelpMenuFree(void)
{
    Free(sResources);
    sResources = NULL;
    FreeAllWindowBuffers();
}

static void HelpMenuQuitBeginFade(u8 taskId)
{
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, 0);
    gTasks[taskId].func = HelpMenuQuitFadeControlAndTaskDel;
}

static void HelpMenuQuitFadeControlAndTaskDel(u8 taskId)
{
    if (!(gPaletteFade.active))
    {
        if (sResources->savedCallback != NULL)
        {
            SetMainCallback2(sResources->savedCallback);
        }
        else
        {
            Overworld_PlaySpecialMapMusic();
            SetMainCallback2(sStaticResources.callback);
        }
        HelpMenuFree();
        DestroyTask(taskId);
    }
}

static void HelpMenuOptionListController(u8 taskId)
{
    s16 * data = gTasks[taskId].data;
    s32 input;

    HelpMenuBg2AnimController();
    if (!gPaletteFade.active)
    {
        input = ListMenu_ProcessInput(data[0]);
        if (sStaticResources.menuMode != 0)
            ListMenuGetScrollAndRow(data[0], &sStaticResources.lessonScrollOffset, &sStaticResources.lessonSelectedRow);
        else
            ListMenuGetScrollAndRow(data[0], &sStaticResources.scrollOffset, &sStaticResources.selectedRow);
        if ((JOY_NEW(SELECT_BUTTON) && sStaticResources.callback != CB2_BagMenuFromStartMenu))
        {
            PlaySE(SE_SELECT);
            HelpMenuQuitBeginFade(taskId);
        }
        else
        {
            switch (input)
            {
            case -1:
                break;
            case -2:
                PlaySE(SE_SELECT);
                if (sStaticResources.menuMode != 0)
                    HelpMenuReturnToMainMenuFromSubmenu(taskId);
                else
                    HelpMenuQuitBeginFade(taskId);
                break;
            default:
                PlaySE(SE_SELECT);
                if (sStaticResources.menuMode != 0)
                {
                    switch (sStaticResources.lessonSelectedRow)
                    {
                    case 0:
                        if (sStaticResources.menuMode == 1)
                            input = HELPSCR_BATTLE;
                        else if (sStaticResources.menuMode == 2)
                            input = HELPSCR_MATCHUPS;
                        else if (sStaticResources.menuMode == 3)
                            input = HELPSCR_CATCHING;
                        else
                            input = HELPSCR_STATUS;
                        break;
                    case 1:
                        if (sStaticResources.menuMode == 1)
                            input = HELPSCR_BATTLE_THEORY;
                        else if (sStaticResources.menuMode == 2)
                            input = HELPSCR_MATCHUPS_THEORY;
                        else if (sStaticResources.menuMode == 3)
                            input = HELPSCR_CATCHING_THEORY;
                        else
                            input = HELPSCR_STATUS_THEORY;
                        break;
                    default:
                        HelpMenuReturnToMainMenuFromSubmenu(taskId);
                        return;
                    }
                }
                if (sStaticResources.menuMode == 0 && input == HELPSCR_BATTLE)
                {
                    HelpMenuOpenLessonSubmenu(taskId, 1);
                    break;
                }
                if (sStaticResources.menuMode == 0 && input == HELPSCR_STATUS)
                {
                    HelpMenuOpenLessonSubmenu(taskId, 4);
                    break;
                }
                if (sStaticResources.menuMode == 0 && input == HELPSCR_MATCHUPS)
                {
                    HelpMenuOpenLessonSubmenu(taskId, 2);
                    break;
                }
                if (sStaticResources.menuMode == 0 && input == HELPSCR_CATCHING)
                {
                    HelpMenuOpenLessonSubmenu(taskId, 3);
                    break;
                }
                sStaticResources.whichScript = input;
                if (sStaticResources.menuMode != 0)
                    DestroyListMenuTask(data[0], &sStaticResources.lessonScrollOffset, &sStaticResources.lessonSelectedRow);
                else
                    DestroyListMenuTask(data[0], &sStaticResources.scrollOffset, &sStaticResources.selectedRow);
                HelpMenuClearWindowRegs();
                ClearWindowTilemap(1);
                ScheduleBgCopyTilemapToVram(0);
                HelpMenuRemoveScrollIndicatorArrowPair();
                data[3] = 0;
                data[2] = 0;
                gTasks[taskId].func = HelpMenuRenderMsgAndSwitchClusterFuncs;
                break;
            }
        }
    }
}

static void HelpMenuOpenLessonSubmenu(u8 taskId, u8 lesson)
{
    s16 *data = gTasks[taskId].data;

    DestroyListMenuTask(data[0], &sStaticResources.scrollOffset, &sStaticResources.selectedRow);
    HelpMenuRemoveScrollIndicatorArrowPair();
    FillWindowPixelBuffer(1, 0);
    sStaticResources.menuMode = lesson;
    sStaticResources.lessonScrollOffset = 0;
    sStaticResources.lessonSelectedRow = 0;
    data[0] = HelpMenuSetupWindow();
    PutWindowTilemap(1);
    ScheduleBgCopyTilemapToVram(0);
}

static void HelpMenuReturnToMainMenuFromSubmenu(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    DestroyListMenuTask(data[0], &sStaticResources.lessonScrollOffset, &sStaticResources.lessonSelectedRow);
    FillWindowPixelBuffer(1, 0);
    sStaticResources.menuMode = 0;
    data[0] = HelpMenuSetupWindow();
    PutWindowTilemap(1);
    HelpMenuSetupScrollIndicatorArrowPair();
    ScheduleBgCopyTilemapToVram(0);
}

static void HelpMenuCmd_TransitionRenderBg2HelpMenuGraphicInitNpcPos(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    HelpMenuBg2AnimController();
    if (++data[2] > 63)
    {
        CopyToBgTilemapBufferRect_ChangePalette(2, sResources->titleTilemap, 0, 0, 0x20, 0x20, 0x11);
        HelpMenuSetSpriteCoordsAndSwitchFrame(data[1], 8, 0x38, 7);
        ScheduleBgCopyTilemapToVram(2);
        data[2] = 0;
        ++data[3];
        PlayNewMapMusic(MUS_FOLLOW_ME);
    }
}

static void HelpMenuCmd_ClearBg2HelpMenuGraphic(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (++data[2] == 134)
    {
        FillBgTilemapBufferRect_Palette0(2, 0, 2, 1, 0x1A, 0xC);
        ScheduleBgCopyTilemapToVram(2);
        data[2] = 0;
        ++data[3];
    }
}

static void HelpMenuCmd_NpcMoveAndSetupTextPrinter(u8 taskId)
{
    s16 * data = gTasks[taskId].data;
    struct Sprite *spriteAddr = &gSprites[data[1]];
    if (data[2] != 35)
        ++data[2];
    else {
        if (spriteAddr->x2 == 0x78)
        {
            StartSpriteAnim(&gSprites[data[1]], 0);
            HelpMenuInitTextPrinter(gHelpMenuText_PokedudeSaysHello);
            data[2] = 0;
            ++data[3];
        }
        else
            ++spriteAddr->x2;
    }   
}

static void HelpMenuCmd_IdleIfTextPrinterIsActive(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (!RunTextPrinters_CheckActive(0))
        ++data[3];
}

static void HelpMenuRenderMsgAndSwitchClusterFuncs(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (JOY_NEW(B_BUTTON))
    {
        sResources->grassAnimDisabled = 1;
        HelpMenuSetSpriteCoordsAndSwitchFrame(data[1], 0, 0, 0);
        FillWindowPixelBuffer(0, 0xCC);
        CopyWindowToVram(0, COPYWIN_GFX);
        HelpMenuClearBg1EndGraphicText();
        data[2] = 0;
        data[3] = 0;
        gTasks[taskId].func = HelpMenuCmd_End;
    }
    else
    {
        static void (* const * const array[])(u8) =
        {
            sBattleScript,
            sStatusScript,
            sMatchupsScript,
            sCatchingScript,
            sTMsScript,
            sMatchupsTheoryScript,
            sBattleTheoryScript,
            sCatchingTheoryScript,
            sStatusTheoryScript,
            sTrainingScript,
        };
        void (*const *cluster)(u8) = array[sStaticResources.whichScript];
        cluster[data[3]](taskId);
    }
}

static void HelpMenuCmd_TextPrinterSwitchStringByOptionChosen(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    static const u8 *const texts[] = {
        gHelpMenuText_BattleScript1,
        gHelpMenuText_StatusScript1,
        gHelpMenuText_MatchupsScript1,
        gHelpMenuText_CatchingScript1,
        gHelpMenuText_TMsScript1,
        gHelpMenuText_MatchupsTheoryScript1,
        gHelpMenuText_BattleTheoryScript1,
        gHelpMenuText_CatchingTheoryScript1,
        gHelpMenuText_StatusTheoryScript1,
        gHelpMenuText_TrainingScript1,
    };
    HelpMenuInitTextPrinter(texts[sStaticResources.whichScript]);
    ++data[3];
}

static void HelpMenuCmd_TextPrinterSwitchStringByOptionChosen2(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    static const u8 *const texts[] =
    {
        gHelpMenuText_BattleScript2,
        gHelpMenuText_StatusScript2,
        gHelpMenuText_MatchupsScript2,
        gHelpMenuText_CatchingScript2,
        gHelpMenuText_TMsScript2,
        gHelpMenuText_MatchupsTheoryScript2,
        gHelpMenuText_BattleTheoryScript2,
        gHelpMenuText_CatchingTheoryScript2,
        gHelpMenuText_StatusTheoryScript2,
        gHelpMenuText_TrainingScript2,
    };
    HelpMenuInitTextPrinter(texts[sStaticResources.whichScript]);
    ++data[3];
}

static const u16 sBg1EndGraphic[] = 
{
    0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8,
    0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8,
};

static const struct Subsprite sSubspriteArray[] = 
{
    {
        .x = -0x8,
        .y = -0x8,
        .shape = 0x1,
        .size = 0x0,
        .tileOffset = 0x0,
        .priority = 0x3,
    },
    {
        .x = -0x8,
        .y = 0x0,
        .shape = 0x1,
        .size = 0x0,
        .tileOffset = 0x2,
        .priority = 0x2,
    },
};

static const struct SubspriteTable sSubspriteTableArray[] = 
{
    {
        .subspriteCount = 0,
        .subsprites = NULL,
    },
    {
        .subspriteCount = 2,
        .subsprites = sSubspriteArray,
    },
};

static const u8 sGrassAnimArray[] = 
{
    0, 0, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 0, 0,
};

static void HelpMenuCmd_IdleIfTextPrinterIsActive2(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (!RunTextPrinters_CheckActive(0))
        ++data[3];
}

static void HelpMenuCmd_EraseTextWindowIfKeyPressed(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (JOY_NEW(A_BUTTON | B_BUTTON))
    {
        FillWindowPixelBuffer(0, 0xCC);
        CopyWindowToVram(0, COPYWIN_GFX);
        ++data[3];
    }
}

static void HelpMenuCmd_StartAnimNpcWalkIntoGrass(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    StartSpriteAnim(&gSprites[data[1]], 5);
    data[2] = 0;
    data[4] = 0;
    data[5] = 1;
    ++data[3];
}

static void HelpMenuCmd_DudeMoveUp(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    struct Sprite *obj = &gSprites[data[1]];
    ChangeBgY(3, 0x100, 2);
    if (!(++data[2] & 0xF))
    {
        --sResources->grassAnimCounterHi;
        HelpMenuGrassAnimationMain(taskId, obj->x2, obj->y2, 0, 0);
    }
    if (data[2] == 48)
    {
        data[2] = 0;
        data[4] = -1;
        data[5] = 0;
        StartSpriteAnim(obj, 7);
        ++data[3];
    }
}

static void HelpMenuCmd_DudeMoveRight(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    struct Sprite *obj = &gSprites[data[1]];
    ChangeBgX(3, 0x100, 1);
    if (!(++data[2] & 0xF))
        ++sResources->grassAnimCounterLo;
    if (!((data[2] + 8) & 0xF))
        HelpMenuGrassAnimationMain(taskId, obj->x2 + 8, obj->y2, 0, 0);
    if (data[2] == 0x30)
    {
        data[2] = 0;
        data[4] = 0;
        data[5] = 0;
        StartSpriteAnim(obj, 3);
        ++data[3];
    }
}

static void HelpMenuCmd_DudeTurnLeft(u8 taskId)
{

    s16 *data = gTasks[taskId].data;
    struct Sprite *objAddr = &gSprites[data[1]];
    StartSpriteAnim(objAddr, 6);
    ++data[3];
    data[4] = 0;
    data[5] = 0;
    HelpMenuGrassAnimationMain(taskId, objAddr->x2, objAddr->y2, 0, 0);
}

static void HelpMenuCmd_DudeMoveLeft(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    struct Sprite *objAddr = &gSprites[data[1]];

    if (!(objAddr->x2 & 0xF))
        HelpMenuGrassAnimationMain(taskId, objAddr->x2 - 8, objAddr->y2, 0, 0);
    if (objAddr->x2 == 8)
        ++data[3];
    else
        --objAddr->x2;
}

static void HelpMenuCmd_RenderAndRemoveBg1EndGraphic(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (!data[2])
    {
        CopyToBgTilemapBufferRect_ChangePalette(1, sBg1EndGraphic, 20, 10, 8, 2, 0x11);
        ScheduleBgCopyTilemapToVram(1);
    }
    if (++data[2] > 126)
    {
        HelpMenuClearBg1EndGraphicText();
        data[2] = 0;
        ++data[3];
    }
}

static void HelpMenuClearBg1EndGraphicText(void)
{
    FillBgTilemapBufferRect_Palette0(1, 0, 20, 10, 8, 2);
    ScheduleBgCopyTilemapToVram(1);
}

static void HelpMenuCmd_End(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (data[2] == 0)
        PlayNewMapMusic(MUS_HELP_MENU_MENU);
    HelpMenuBg2AnimController();
    if (++data[2] > 63)
    {
        data[2] = 0;
        data[3] = 0;
        data[0] = HelpMenuSetupWindow();
        gTasks[taskId].func = HelpMenuOptionListController;
        PutWindowTilemap(0);
        HelpMenuSetupScrollIndicatorArrowPair();
        HelpMenuSetWindowRegs();
        ScheduleBgCopyTilemapToVram(0);
        ChangeBgX(3, 0x0, 0);
        ChangeBgY(3, 0x0, 0);
        ChangeBgX(3, 0x1000, 2);
        ChangeBgY(3, 0x2800, 1);
        sResources->grassAnimCounterLo = 0;
        sResources->grassAnimCounterHi = 3;
        sResources->grassAnimDisabled = 0;
    }
}

static void HelpMenuCmd_TaskBattleOrFadeByOptionChosen(u8 taskId)
{
    switch (sStaticResources.whichScript)
    {
    case HELPSCR_BATTLE:
    case HELPSCR_STATUS:
    case HELPSCR_MATCHUPS:
    case HELPSCR_CATCHING:
        HelpMenuPrepBattle(taskId);
        break;
    case HELPSCR_TMS:
        sResources->savedCallback = HelpMenuSetupBagItemsByOptionChosen;
        HelpMenuQuitBeginFade(taskId);
        break;
    }
}

static void HelpMenuSetupBagItemsByOptionChosen(void)
{
    InitPokedudeBag(ITEMMENULOCATION_HELPSCR_TMS);
}

static void HelpMenuPostBattleFadeControl(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (!(gPaletteFade.active))
    {
        data[3] = sWhereToReturnToFromBattle[sStaticResources.whichScript];
        gTasks[taskId].func = HelpMenuRenderMsgAndSwitchClusterFuncs;
    }
}

static void HelpMenuGrassAnimationMain(u8 taskId, s16 x, s16 y, u8 subpriority, bool8 mode)
{
    struct Sprite *obj;
    u8 spriteId;

    if (sResources->grassAnimDisabled != 1 && HelpMenuGrassAnimationCheckIfNeedsToGenerateGrassObj(x - 0x10, y))
    {
        spriteId = CreateSprite(gFieldEffectObjectTemplatePointers[FLDEFFOBJ_TALL_GRASS], 0, 0, subpriority);
        obj = &gSprites[spriteId];
        obj->x2 = x;
        obj->y2 = y + 8;
        obj->callback = HelpMenuGrassAnimationObjCallback;
        obj->data[0] = taskId;
        if (mode == 1)
        {
            SeekSpriteAnim(obj, 4);
            obj->oam.priority = 2;
        }
        else
        {
            SetSubspriteTables(obj, sSubspriteTableArray);
            obj->subspriteTableNum = 0;
            obj->subspriteMode = 1;
        }
    }
}

static void HelpMenuGrassAnimationObjCallback(struct Sprite *sprite)
{
    s16 diff1, diff2;
    s16 *data = gTasks[sprite->data[0]].data;
    struct Sprite *objAddr = &gSprites[data[1]];

    if (sResources->grassAnimDisabled == 1)
        DestroySprite(sprite);
    else
    {
        if (sprite->animCmdIndex == 0)
            sprite->subspriteTableNum = 1;
        else
            sprite->subspriteTableNum = 0;
        sprite->x2 += data[4];
        sprite->y2 += data[5];
        if (sprite->animEnded)
        {
            sprite->subpriority = 0;
            diff1 = sprite->x2 - objAddr->x2;
            diff2 = sprite->y2 - objAddr->y2;
            if (diff1 <= -16 || diff1 >= 16 || diff2 <= -16 || diff2 >= 24)
                DestroySprite(sprite);
        }
    }
}

static u8 HelpMenuGrassAnimationCheckIfNeedsToGenerateGrassObj(s16 x, s16 y)
{
    const u8 * arr;
    struct HelpMenuBuf *ptr;
    int high, low;
    if ((x < 0) || (y < 0))
        return 0;
    arr = sGrassAnimArray;
    high = ((y >> 4) + sResources->grassAnimCounterHi) << 4;
    low = ((x >> 4) + sResources->grassAnimCounterLo);
    return arr[high+low];
}

static void HelpMenuPrepBattle(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    HelpMenuFree();
    gSpecialVar_0x8004 = sStaticResources.whichScript;
    gMain.savedCallback = HelpMenuRestorePlayerPartyCallback;
    SavePlayerParty();
    InitPokedudePartyAndOpponent();
    PlayMapChosenOrBattleBGM(MUS_DUMMY);
    if (sStaticResources.whichScript == HELPSCR_BATTLE)
        data[6] = B_TRANSITION_WHITE_BARS_FADE;
    else
        data[6] = B_TRANSITION_SLICE;
    data[7] = 0;
    gTasks[taskId].func = HelpMenuPreBattleAnimAndSetBattleCallback;
}

static void HelpMenuPreBattleAnimAndSetBattleCallback(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    switch (data[7])
    {
    case 0:
        BattleTransition_StartOnField(data[6]);
        ++data[7];
        break;
    case 1:
        if (IsBattleTransitionDone())
        {
            SetMainCallback2(CB2_InitBattle);
            DestroyTask(taskId);
        }
        break;
    }
}

static void HelpMenuRestorePlayerPartyCallback(void)
{
    LoadPlayerParty();
    if (gBattleOutcome == B_OUTCOME_DREW)
        SetHelpMenuControllerModeToResume();
    else
        PlayNewMapMusic(MUS_FOLLOW_ME);
    CB2_ReturnToHelpMenu();
}

static void HelpMenuLoadBg3Map(u16 *buffer)
{
    u16 * bgTilesBuffer;
    u8 * mapTilesRowBuffer;
    u16 i, j, k;
    u16 currentBlockIdx;
    void *tilesetsBuffer;
    void *palIndicesBuffer;
    u16 numMapTilesRows = 0;
    const struct MapLayout *layout = &Route1_Layout;
    u16 * blockIndicesBuffer = AllocZeroed(0x800);
    tilesetsBuffer = AllocZeroed(NUM_TILES_TOTAL * TILE_SIZE_4BPP);
    palIndicesBuffer = Alloc(16);
    memset(palIndicesBuffer, 0xFF, 16);

    HelpMenuLoadMapTilesetToBuffer(layout->primaryTileset, tilesetsBuffer, NUM_TILES_IN_PRIMARY);
    HelpMenuLoadMapTilesetToBuffer(layout->secondaryTileset, tilesetsBuffer + NUM_TILES_IN_PRIMARY * TILE_SIZE_4BPP, NUM_TILES_TOTAL - NUM_TILES_IN_PRIMARY);

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 16; j++)
        {
            currentBlockIdx = layout->map[8 + (i + 6) * layout->width + j] & 0x3FF;
            for (k = 0; k < (i << 4) + j; k++)
            {
                if (blockIndicesBuffer[k] == 0)
                    break;
                if (blockIndicesBuffer[k] == currentBlockIdx)
                    break;
            }
            if (blockIndicesBuffer[k] == 0)
            {
                blockIndicesBuffer[k] = currentBlockIdx;
                numMapTilesRows++;
            }
            HelpMenuPushBackNewMapPalIndexArrayEntry(layout, &buffer[64 * i + 2 * j], palIndicesBuffer, currentBlockIdx, k);
        }
    }

    bgTilesBuffer = Alloc(numMapTilesRows * 0x80);
    mapTilesRowBuffer = Alloc(0x80);
    for (i = 0; i < numMapTilesRows; i++)
    {
        memset(mapTilesRowBuffer, 0, 0x80);
        if (blockIndicesBuffer[i] < NUM_METATILES_IN_PRIMARY)
            HelpMenuComputeMapTilesFromTilesetAndMetaTiles((const void *)layout->primaryTileset->metatiles + blockIndicesBuffer[i] * 16, mapTilesRowBuffer, tilesetsBuffer);
        else
            HelpMenuComputeMapTilesFromTilesetAndMetaTiles((const void *)layout->secondaryTileset->metatiles + (blockIndicesBuffer[i] - NUM_METATILES_IN_PRIMARY) * 16, mapTilesRowBuffer, tilesetsBuffer);
        CpuFastCopy(mapTilesRowBuffer, bgTilesBuffer + i * 0x40, 0x80);
    }

    LoadBgTiles(3, bgTilesBuffer, numMapTilesRows * 0x80, 0);
    HelpMenuLoadMapPalette(layout, palIndicesBuffer);

    Free(mapTilesRowBuffer);
    Free(bgTilesBuffer);
    Free(palIndicesBuffer);
    Free(tilesetsBuffer);
    Free(blockIndicesBuffer);
}

static void HelpMenuLoadMapTilesetToBuffer(const struct Tileset *ts, u8 *dstBuffer, u16 size)
{
    if (ts)
    {
        if (!ts->isCompressed)
            CpuFastCopy(ts->tiles, dstBuffer, 0x20 * size);
        else
            LZDecompressWram(ts->tiles, dstBuffer);
    }
}

static void HelpMenuPushBackNewMapPalIndexArrayEntry(const struct MapLayout *mStruct, u16 *buf1, u8 *palIndexArray, u16 mapEntry, u16 offset)
{
    const u16 * metaTileEntryAddr;
    if (mapEntry < NUM_METATILES_IN_PRIMARY)
        metaTileEntryAddr = &mStruct->primaryTileset->metatiles[8 * mapEntry];
    else
        metaTileEntryAddr = &mStruct->secondaryTileset->metatiles[8 * (mapEntry - NUM_METATILES_IN_PRIMARY)];
    buf1[0] = (HelpMenuComputePalIndexArrayEntryByMetaTile(palIndexArray, metaTileEntryAddr[0]) << 12) + 4 * offset;
    buf1[1] = (HelpMenuComputePalIndexArrayEntryByMetaTile(palIndexArray, metaTileEntryAddr[1]) << 12) + 4 * offset + 1;
    buf1[32] = (HelpMenuComputePalIndexArrayEntryByMetaTile(palIndexArray, metaTileEntryAddr[2]) << 12) + 4 * offset + 2;
    buf1[33] = (HelpMenuComputePalIndexArrayEntryByMetaTile(palIndexArray, metaTileEntryAddr[3]) << 12) + 4 * offset + 3;
}

static void HelpMenuComputeMapTilesFromTilesetAndMetaTiles(const u16 *metaTilesArray, u8 *blockBuf, u8 *tileset)
{
    HelpMenuComputeSingleMapTileBlockFromTilesetAndMetaTiles(blockBuf, &tileset[0x20 * (*metaTilesArray & 0x3FF)], (*metaTilesArray >> 10) & 3);
    HelpMenuComputeSingleMapTileBlockFromTilesetAndMetaTiles(blockBuf, &tileset[0x20 * (metaTilesArray[4] & 0x3FF)], (metaTilesArray[4] >> 10) & 3);
    HelpMenuComputeSingleMapTileBlockFromTilesetAndMetaTiles(blockBuf + 0x20, &tileset[0x20 * (metaTilesArray[1] & 0x3FF)], (metaTilesArray[1] >> 10) & 3);
    HelpMenuComputeSingleMapTileBlockFromTilesetAndMetaTiles(blockBuf + 0x20, &tileset[0x20 * (metaTilesArray[5] & 0x3FF)], (metaTilesArray[5] >> 10) & 3);
    HelpMenuComputeSingleMapTileBlockFromTilesetAndMetaTiles(blockBuf + 0x40, &tileset[0x20 * (metaTilesArray[2] & 0x3FF)], (metaTilesArray[2] >> 10) & 3);
    HelpMenuComputeSingleMapTileBlockFromTilesetAndMetaTiles(blockBuf + 0x40, &tileset[0x20 * (metaTilesArray[6] & 0x3FF)], (metaTilesArray[6] >> 10) & 3);
    blockBuf += 0x60;
    HelpMenuComputeSingleMapTileBlockFromTilesetAndMetaTiles(blockBuf, &tileset[0x20 * (metaTilesArray[3] & 0x3FF)], (metaTilesArray[3] >> 10) & 3);
    HelpMenuComputeSingleMapTileBlockFromTilesetAndMetaTiles(blockBuf, &tileset[0x20 * (metaTilesArray[7] & 0x3FF)], (metaTilesArray[7] >> 10) & 3);
}

static void HelpMenuComputeSingleMapTileBlockFromTilesetAndMetaTiles(u8 *blockBuf, u8 *tileset, u8 metaTile)
{
    u8 i, j;
    u8 * buffer = AllocZeroed(0x20);
    u8 * src = AllocZeroed(0x20);
    CpuFastCopy(tileset, buffer, 0x20);
    if (metaTile & 1)
    {
        for (i = 0; i < 8; ++i)
        {
            for (j = 0; j < 4; ++j)
            {
                u32 offset = j - 3;
                u8 value = buffer[(i << 2) - offset];
                src[(i << 2) + j] = ((value & 0xF) << 4) + ((value & 0xF0) >> 4);
            }
        }
        CpuFastCopy(src, buffer, 0x20);
    }
    if (metaTile & 2)
    {
        for (i = 0; i < 8; ++i)
            memcpy(&src[4 * i], &buffer[4 * (7 - i)], 4);
        CpuFastCopy(src, buffer, 0x20);
    }
    for (i = 0; i < 32; ++i)
    {
        if (buffer[i] & 0xF0)
            blockBuf[i] = (blockBuf[i] & 0xF) + (buffer[i] & 0xF0);
        if (buffer[i] & 0xF)
            blockBuf[i] = (blockBuf[i] & 0xF0) + (buffer[i] & 0xF);
    }
    Free(src);
    Free(buffer);
}

static u16 HelpMenuComputePalIndexArrayEntryByMetaTile(u8 *palIndexArrayBuf, u16 metaTile)
{
    u16 i;
    int firstEntry;
    int temp;
    u32 pal = metaTile >> 12;
    i = 0;
    firstEntry = *palIndexArrayBuf;
    if (firstEntry != pal)
    {
        if (firstEntry == 0xFF)
        {
            *palIndexArrayBuf = pal;
        }
        else
        {
            while (++i < 16)
            {
                temp = palIndexArrayBuf[i];
                if (temp == pal)
                    break;
                if (temp == 0xFF)
                {
                    palIndexArrayBuf[i] = pal;
                    break;
                }
            }
        }
    }
    return (0xF - i);
}

static void HelpMenuLoadMapPalette(const struct MapLayout * mStruct, const u8 * palIndexArray)
{
    u8 i;
    const struct Tileset * ts;
    const u16 * dest;

    for (i = 0; i < 16; i++)
    {
        if (palIndexArray[i] == 0xFF)
            break;
        if (palIndexArray[i] >= NUM_PALS_IN_PRIMARY)
            dest = mStruct->secondaryTileset->palettes[palIndexArray[i]];
        else
            dest = mStruct->primaryTileset->palettes[palIndexArray[i]];
        LoadPalette(dest, BG_PLTT_ID(15 - i), PLTT_SIZE_4BPP);
    }
}
