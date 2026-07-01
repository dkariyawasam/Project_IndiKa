#include "global.h"
#include "gflib.h"
#include "decompress.h"
#include "scanline_effect.h"
#include "overworld.h"
#include "link.h"
#include "pokedex.h"
#include "item_menu.h"
#include "party_menu.h"
#include "save.h"
#include "link_rfu.h"
#include "help_message.h"
#include "event_data.h"
#include "fieldmap.h"
#include "safari_zone.h"
#include "start_menu.h"
#include "menu.h"
#include "load_save.h"
#include "strings.h"
#include "menu_helpers.h"
#include "text_window.h"
#include "field_fadetransition.h"
#include "field_player_avatar.h"
#include "new_menu_helpers.h"
#include "event_object_movement.h"
#include "event_object_lock.h"
#include "script.h"
#include "quest_log.h"
#include "new_game.h"
#include "event_scripts.h"
#include "field_weather.h"
#include "field_specials.h"
#include "pokedex_screen.h"
#include "trainer_card.h"
#include "option_menu.h"
#include "save_menu_util.h"
#include "help_system.h"
#include "constants/songs.h"
#include "constants/field_weather.h"
#include "sloopsvc.h"
#include "quests.h"

enum StartMenuOption
{
    STARTMENU_POKEDEX = 0,
    STARTMENU_POKEMON,
    STARTMENU_BAG,
    STARTMENU_QUEST,
    STARTMENU_PLAYER,
    STARTMENU_SAVE,
    STARTMENU_OPTION,
    STARTMENU_EXIT,
    STARTMENU_RETIRE,
    STARTMENU_PLAYER2,
    MAX_STARTMENU_ITEMS
};

enum SaveCBReturn
{
    SAVECB_RETURN_CONTINUE = 0,
    SAVECB_RETURN_OKAY,
    SAVECB_RETURN_CANCEL,
    SAVECB_RETURN_ERROR
};

static EWRAM_DATA bool8 (*sStartMenuCallback)(void) = NULL;
static EWRAM_DATA u8 sStartMenuCursorPos = 0;
static EWRAM_DATA u8 sNumStartMenuItems = 0;
static EWRAM_DATA u8 sStartMenuOrder[MAX_STARTMENU_ITEMS] = {};
static EWRAM_DATA s8 sDrawStartMenuState[2] = {};
static EWRAM_DATA u8 sRadialStartMenuWindowIds[8] = {};
static EWRAM_DATA u8 sRadialStartMenuSlotToItem[8] = {};
static EWRAM_DATA u8 sRadialStartMenuSlotToOrderIndex[8] = {};
static EWRAM_DATA u8 sRadialStartMenuButtonSpriteIds[8] = {};
static EWRAM_DATA u8 sRadialStartMenuSpriteIds[8] = {};
static EWRAM_DATA u8 sRadialStartMenuCursorSlot = 0;
static EWRAM_DATA bool8 sRadialStartMenuSpritesLoaded = FALSE;
static EWRAM_DATA u8 sSafariZoneStatsWindowId = 0;
static ALIGNED(4) EWRAM_DATA u8 sSaveStatsWindowId = 0;

static u8 (*sSaveDialogCB)(void);
static u8 sSaveDialogDelay;
static bool8 sSaveDialogIsPrinting;

static const u8 gText_MenuQuest[] = _("LOGBOOK");
static const u8 sStartMenuDesc_Quest[] = _("Review your FIELD AIDE research,\nrumours, and active assignments.");

static bool8 StartMenuQuestCallback(void);
static void SetUpStartMenu_Link(void);
static void SetUpStartMenu_UnionRoom(void);
static void SetUpStartMenu_SafariZone(void);
static void SetUpStartMenu_NormalField(void);
static bool8 StartCB_HandleInput(void);
static void StartMenu_FadeScreenIfLeavingOverworld(void);
static bool8 StartMenuPokedexSanityCheck(void);
static bool8 StartMenuPokedexCallback(void);
static bool8 StartMenuPokemonCallback(void);
static bool8 StartMenuBagCallback(void);
static bool8 StartMenuPlayerCallback(void);
static bool8 StartMenuSaveCallback(void);
static bool8 StartMenuOptionCallback(void);
static bool8 StartMenuExitCallback(void);
static bool8 StartMenuSafariZoneRetireCallback(void);
static bool8 StartMenuLinkPlayerCallback(void);
static bool8 StartCB_Save1(void);
static bool8 StartCB_Save2(void);
static void StartMenu_PrepareForSave(void);
static void CreateRadialStartMenu(void);
static void DestroyRadialStartMenuWindows(bool8 copyToVram);
static void DrawRadialStartMenu(void);
static void CreateRadialStartMenuSprites(void);
static void DestroyRadialStartMenuSprites(void);
static void UpdateRadialStartMenuSpriteStates(void);
static void UpdateRadialStartMenuSelection(u8 newSlot);
static u8 FindNextRadialStartMenuSlot(s8 step);
static u8 FindRadialStartMenuSlotInDirection(s8 dx, s8 dy);
static u8 GetRadialStartMenuWindowLeft(u8 slot);
static u8 GetRadialStartMenuWindowTop(u8 slot);
static s16 GetRadialStartMenuSpriteX(u8 slot);
static s16 GetRadialStartMenuSpriteY(u8 slot);
static void RefreshStartMenuHelpText(void);
static u8 RunSaveDialogCB(void);
static void task50_save_game(u8 taskId);
static u8 SaveDialogCB_PrintAskSaveText(void);
static u8 SaveDialogCB_AskSavePrintYesNoMenu(void);
static u8 SaveDialogCB_AskSaveHandleInput(void);
static u8 SaveDialogCB_PrintAskOverwriteText(void);
static u8 SaveDialogCB_AskOverwritePrintYesNoMenu(void);
static u8 SaveDialogCB_AskReplacePreviousFilePrintYesNoMenu(void);
static u8 SaveDialogCB_AskOverwriteOrReplacePreviousFileHandleInput(void);
static u8 SaveDialogCB_PrintSavingDontTurnOffPower(void);
static u8 SaveDialogCB_DoSave(void);
static u8 SaveDialogCB_PrintSaveResult(void);
static u8 SaveDialogCB_WaitPrintSuccessAndPlaySE(void);
static u8 SaveDialogCB_ReturnSuccess(void);
static u8 SaveDialogCB_WaitPrintErrorAndPlaySE(void);
static u8 SaveDialogCB_ReturnError(void);
static void CB2_WhileSavingAfterLinkBattle(void);
static void task50_after_link_battle_save(u8 taskId);
static void PrintSaveStats(void);
static void CloseSaveStatsWindow(void);
static void CloseStartMenu(void);

static const struct MenuAction sStartMenuActionTable[] = {
    [STARTMENU_POKEDEX] = { gText_MenuPokedex, {.u8_void = StartMenuPokedexCallback} },
    [STARTMENU_POKEMON] = { gText_MenuPokemon, {.u8_void = StartMenuPokemonCallback} },
    [STARTMENU_BAG]     = { gText_MenuBag,     {.u8_void = StartMenuBagCallback} },
    [STARTMENU_QUEST]   = { gText_MenuQuest,   {.u8_void = StartMenuQuestCallback} },
    [STARTMENU_PLAYER]  = { gText_MenuPlayer,  {.u8_void = StartMenuPlayerCallback} },
    [STARTMENU_SAVE]    = { gText_MenuSave,    {.u8_void = StartMenuSaveCallback} },
    [STARTMENU_OPTION]  = { gText_MenuOption,  {.u8_void = StartMenuOptionCallback} },
    [STARTMENU_EXIT]    = { gText_MenuExit,    {.u8_void = StartMenuExitCallback} },
    [STARTMENU_RETIRE]  = { gText_MenuRetire,  {.u8_void = StartMenuSafariZoneRetireCallback} },
    [STARTMENU_PLAYER2] = { gText_MenuPlayer,  {.u8_void = StartMenuLinkPlayerCallback} }
};

static const struct WindowTemplate sSafariZoneStatsWindowTemplate = {
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 1,
    .width = 10,
    .height = 4,
    .paletteNum = 15,
    .baseBlock = 0x008
};

static const u8 *const sStartMenuDescPointers[] = {
    gStartMenuDesc_Pokedex,
    gStartMenuDesc_Pokemon,
    gStartMenuDesc_Bag,
    sStartMenuDesc_Quest,
    gStartMenuDesc_Player,
    gStartMenuDesc_Save,
    gStartMenuDesc_Option,
    gStartMenuDesc_Exit,
    gStartMenuDesc_Retire,
    gStartMenuDesc_Player
};

static const struct BgTemplate sBGTemplates_AfterLinkSaveMessage[] = {
    {
        .bg = 0,
        .charBaseIndex = 2,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0x000
    }
};

static const struct WindowTemplate sWindowTemplates_AfterLinkSaveMessage[] = {
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 26,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 0x198
    }, DUMMY_WIN_TEMPLATE
};

static const struct WindowTemplate sSaveStatsWindowTemplate = {
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 1,
    .width = 14,
    .height = 9,
    .paletteNum = 13,
    .baseBlock = 0x008
};

static ALIGNED(2) const u8 sTextColor_StatName[] = { 1, 2, 3 };
static ALIGNED(2) const u8 sTextColor_StatValue[] = { 1, 4, 5 };
static ALIGNED(2) const u8 sTextColor_LocationHeader[] = { 1, 6, 7 };
static ALIGNED(2) const u8 sTextColor_RadialMenuNormal[] = { TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY };
static ALIGNED(2) const u8 sTextColor_RadialMenuSelected[] = { TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY };

static const s8 sRadialStartMenuWindowXOffsets[] = { -40, 32, -16, -16, -40, -64, -64, -72 };
static const s8 sRadialStartMenuWindowYOffsets[] = { -19, -47, -7, 17, 29, 17, -7, -47 };
static const s8 sRadialStartMenuSlotXs[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
static const s8 sRadialStartMenuSlotYs[] = { -1, -1, 0, 1, 1, 1, 0, -1 };

#define TAG_START_MENU_BUTTON_ICON  0x1250
#define TAG_START_MENU_POKEMON_ICON 0x1251
#define TAG_START_MENU_SETTINGS_ICON 0x1252
#define TAG_START_MENU_LOGBOOK_ICON 0x1253
#define TAG_START_MENU_POKEDEX_ICON 0x1254
#define TAG_START_MENU_CARD_ICON    0x1255
#define TAG_START_MENU_BAG_ICON     0x1256

static const u32 sRadialStartMenuButtonIconGfx[] = INCBIN_U32("graphics/start_menu/button.4bpp.lz");
static const u32 sRadialStartMenuButtonIconPal[] = INCBIN_U32("graphics/start_menu/button.gbapal.lz");
static const u32 sRadialStartMenuPokemonIconGfx[] = INCBIN_U32("graphics/start_menu/pokemon.4bpp.lz");
static const u32 sRadialStartMenuSettingsIconGfx[] = INCBIN_U32("graphics/start_menu/settings.4bpp.lz");
static const u32 sRadialStartMenuSettingsIconPal[] = INCBIN_U32("graphics/start_menu/settings.gbapal.lz");
static const u32 sRadialStartMenuLogbookIconGfx[] = INCBIN_U32("graphics/start_menu/logbook.4bpp.lz");
static const u32 sRadialStartMenuPokedexIconGfx[] = INCBIN_U32("graphics/start_menu/pokedex.4bpp.lz");
static const u32 sRadialStartMenuCardIconGfx[] = INCBIN_U32("graphics/start_menu/card.4bpp.lz");
static const u32 sRadialStartMenuBagIconGfx[] = INCBIN_U32("graphics/start_menu/bag.4bpp.lz");

static const struct OamData sOamData_RadialStartMenuIcon = {
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .shape = SPRITE_SHAPE(32x32),
    .size = SPRITE_SIZE(32x32),
    .priority = 0
};

static const union AnimCmd sAnim_RadialStartMenuIcon[] = {
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd sAnim_RadialStartMenuIconPressed[] = {
    ANIMCMD_FRAME(16, 0),
    ANIMCMD_END
};

static const union AnimCmd *const sAnims_RadialStartMenuIcon[] = {
    sAnim_RadialStartMenuIcon,
    sAnim_RadialStartMenuIconPressed
};

static const union AffineAnimCmd sAffineAnim_RadialStartMenuIconNormal[] = {
    AFFINEANIMCMD_FRAME(0x100, 0x100, 0, 0),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd sAffineAnim_RadialStartMenuIconSelected[] = {
    AFFINEANIMCMD_FRAME(0x100, 0x100, 0, 0),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd *const sAffineAnims_RadialStartMenuIcon[] = {
    sAffineAnim_RadialStartMenuIconNormal,
    sAffineAnim_RadialStartMenuIconSelected
};

static const struct CompressedSpriteSheet sSpriteSheet_RadialStartMenuButtonIcon = {
    sRadialStartMenuButtonIconGfx, 32 * 32 / 2, TAG_START_MENU_BUTTON_ICON
};

static const struct CompressedSpritePalette sSpritePalette_RadialStartMenuButtonIcon = {
    sRadialStartMenuButtonIconPal, TAG_START_MENU_BUTTON_ICON
};

static const struct SpriteTemplate sSpriteTemplate_RadialStartMenuButtonIcon = {
    .tileTag = TAG_START_MENU_BUTTON_ICON,
    .paletteTag = TAG_START_MENU_SETTINGS_ICON,
    .oam = &sOamData_RadialStartMenuIcon,
    .anims = sAnims_RadialStartMenuIcon,
    .images = NULL,
    .affineAnims = sAffineAnims_RadialStartMenuIcon,
    .callback = SpriteCallbackDummy
};

static const struct CompressedSpriteSheet sSpriteSheet_RadialStartMenuPokemonIcon = {
    sRadialStartMenuPokemonIconGfx, 32 * 64 / 2, TAG_START_MENU_POKEMON_ICON
};

static const struct SpriteTemplate sSpriteTemplate_RadialStartMenuPokemonIcon = {
    .tileTag = TAG_START_MENU_POKEMON_ICON,
    .paletteTag = TAG_START_MENU_SETTINGS_ICON,
    .oam = &sOamData_RadialStartMenuIcon,
    .anims = sAnims_RadialStartMenuIcon,
    .images = NULL,
    .affineAnims = sAffineAnims_RadialStartMenuIcon,
    .callback = SpriteCallbackDummy
};

static const struct CompressedSpriteSheet sSpriteSheet_RadialStartMenuSettingsIcon = {
    sRadialStartMenuSettingsIconGfx, 32 * 64 / 2, TAG_START_MENU_SETTINGS_ICON
};

static const struct CompressedSpritePalette sSpritePalette_RadialStartMenuSettingsIcon = {
    sRadialStartMenuSettingsIconPal, TAG_START_MENU_SETTINGS_ICON
};

static const struct SpriteTemplate sSpriteTemplate_RadialStartMenuSettingsIcon = {
    .tileTag = TAG_START_MENU_SETTINGS_ICON,
    .paletteTag = TAG_START_MENU_SETTINGS_ICON,
    .oam = &sOamData_RadialStartMenuIcon,
    .anims = sAnims_RadialStartMenuIcon,
    .images = NULL,
    .affineAnims = sAffineAnims_RadialStartMenuIcon,
    .callback = SpriteCallbackDummy
};

static const struct CompressedSpriteSheet sSpriteSheet_RadialStartMenuLogbookIcon = {
    sRadialStartMenuLogbookIconGfx, 32 * 64 / 2, TAG_START_MENU_LOGBOOK_ICON
};

static const struct SpriteTemplate sSpriteTemplate_RadialStartMenuLogbookIcon = {
    .tileTag = TAG_START_MENU_LOGBOOK_ICON,
    .paletteTag = TAG_START_MENU_SETTINGS_ICON,
    .oam = &sOamData_RadialStartMenuIcon,
    .anims = sAnims_RadialStartMenuIcon,
    .images = NULL,
    .affineAnims = sAffineAnims_RadialStartMenuIcon,
    .callback = SpriteCallbackDummy
};

static const struct CompressedSpriteSheet sSpriteSheet_RadialStartMenuPokedexIcon = {
    sRadialStartMenuPokedexIconGfx, 32 * 64 / 2, TAG_START_MENU_POKEDEX_ICON
};

static const struct SpriteTemplate sSpriteTemplate_RadialStartMenuPokedexIcon = {
    .tileTag = TAG_START_MENU_POKEDEX_ICON,
    .paletteTag = TAG_START_MENU_SETTINGS_ICON,
    .oam = &sOamData_RadialStartMenuIcon,
    .anims = sAnims_RadialStartMenuIcon,
    .images = NULL,
    .affineAnims = sAffineAnims_RadialStartMenuIcon,
    .callback = SpriteCallbackDummy
};

static const struct CompressedSpriteSheet sSpriteSheet_RadialStartMenuCardIcon = {
    sRadialStartMenuCardIconGfx, 32 * 64 / 2, TAG_START_MENU_CARD_ICON
};

static const struct SpriteTemplate sSpriteTemplate_RadialStartMenuCardIcon = {
    .tileTag = TAG_START_MENU_CARD_ICON,
    .paletteTag = TAG_START_MENU_SETTINGS_ICON,
    .oam = &sOamData_RadialStartMenuIcon,
    .anims = sAnims_RadialStartMenuIcon,
    .images = NULL,
    .affineAnims = sAffineAnims_RadialStartMenuIcon,
    .callback = SpriteCallbackDummy
};

static const struct CompressedSpriteSheet sSpriteSheet_RadialStartMenuBagIcon = {
    sRadialStartMenuBagIconGfx, 32 * 64 / 2, TAG_START_MENU_BAG_ICON
};

static const struct SpriteTemplate sSpriteTemplate_RadialStartMenuBagIcon = {
    .tileTag = TAG_START_MENU_BAG_ICON,
    .paletteTag = TAG_START_MENU_SETTINGS_ICON,
    .oam = &sOamData_RadialStartMenuIcon,
    .anims = sAnims_RadialStartMenuIcon,
    .images = NULL,
    .affineAnims = sAffineAnims_RadialStartMenuIcon,
    .callback = SpriteCallbackDummy
};

// Unused
static void SetHasPokedexAndPokemon(void)
{
    FlagSet(FLAG_SYS_POKEDEX_GET);
    FlagSet(FLAG_SYS_POKEMON_GET);
}

static void SetUpStartMenu(void)
{
    sNumStartMenuItems = 0;
    if (IsUpdateLinkStateCBActive() == TRUE)
        SetUpStartMenu_Link();
    else if (InUnionRoom() == TRUE)
        SetUpStartMenu_UnionRoom();
    else if (GetSafariZoneFlag() == TRUE)
        SetUpStartMenu_SafariZone();
    else
        SetUpStartMenu_NormalField();
}

static void AppendToStartMenuItems(u8 newEntry)
{
    AppendToList(sStartMenuOrder, &sNumStartMenuItems, newEntry);
}

static void SetUpStartMenu_NormalField(void)
{
    if (FlagGet(FLAG_SYS_POKEDEX_GET) == TRUE)
        AppendToStartMenuItems(STARTMENU_POKEDEX);
    if (FlagGet(FLAG_SYS_POKEMON_GET) == TRUE)
        AppendToStartMenuItems(STARTMENU_POKEMON);
    AppendToStartMenuItems(STARTMENU_BAG);
    if (FlagGet(FLAG_SYS_QUEST_MENU_GET))
        AppendToStartMenuItems(STARTMENU_QUEST);
    AppendToStartMenuItems(STARTMENU_PLAYER);
    AppendToStartMenuItems(STARTMENU_SAVE);
    AppendToStartMenuItems(STARTMENU_OPTION);
}

static void SetUpStartMenu_SafariZone(void)
{
    AppendToStartMenuItems(STARTMENU_RETIRE);
    AppendToStartMenuItems(STARTMENU_POKEDEX);
    AppendToStartMenuItems(STARTMENU_POKEMON);
    AppendToStartMenuItems(STARTMENU_BAG);
    AppendToStartMenuItems(STARTMENU_PLAYER);
    AppendToStartMenuItems(STARTMENU_OPTION);
}

static void SetUpStartMenu_Link(void)
{
    AppendToStartMenuItems(STARTMENU_POKEMON);
    AppendToStartMenuItems(STARTMENU_BAG);
    AppendToStartMenuItems(STARTMENU_PLAYER2);
    AppendToStartMenuItems(STARTMENU_OPTION);
}

static void SetUpStartMenu_UnionRoom(void)
{
    AppendToStartMenuItems(STARTMENU_POKEMON);
    AppendToStartMenuItems(STARTMENU_BAG);
    AppendToStartMenuItems(STARTMENU_PLAYER);
    AppendToStartMenuItems(STARTMENU_OPTION);
}

static void DrawSafariZoneStatsWindow(void)
{
    sSafariZoneStatsWindowId = AddWindow(&sSafariZoneStatsWindowTemplate);
    PutWindowTilemap(sSafariZoneStatsWindowId);
    DrawStdWindowFrame(sSafariZoneStatsWindowId, FALSE);
    ConvertIntToDecimalStringN(gStringVar1, gSafariZoneStepCounter, STR_CONV_MODE_RIGHT_ALIGN, 3);
    ConvertIntToDecimalStringN(gStringVar2, 600, STR_CONV_MODE_RIGHT_ALIGN, 3);
    ConvertIntToDecimalStringN(gStringVar3, gNumSafariBalls, STR_CONV_MODE_RIGHT_ALIGN, 2);
    StringExpandPlaceholders(gStringVar4, gText_MenuSafariStats);
    AddTextPrinterParameterized(sSafariZoneStatsWindowId, FONT_NORMAL, gStringVar4, 4, 3, 0xFF, NULL);
    CopyWindowToVram(sSafariZoneStatsWindowId, COPYWIN_GFX);
}

static void DestroySafariZoneStatsWindow(void)
{
    if (GetSafariZoneFlag())
    {
        ClearStdWindowAndFrameToTransparent(sSafariZoneStatsWindowId, FALSE);
        CopyWindowToVram(sSafariZoneStatsWindowId, COPYWIN_GFX);
        RemoveWindow(sSafariZoneStatsWindowId);
    }
}

static s8 PrintStartMenuItems(s8 *cursor_p, u8 nitems)
{
    s16 i = *cursor_p;
    do
    {
        if (sStartMenuOrder[i] == STARTMENU_PLAYER || sStartMenuOrder[i] == STARTMENU_PLAYER2)
        {
            Menu_PrintFormatIntlPlayerName(GetStartMenuWindowId(), sStartMenuActionTable[sStartMenuOrder[i]].text, 8, i * 15);
        }
        else
        {
            StringExpandPlaceholders(gStringVar4, sStartMenuActionTable[sStartMenuOrder[i]].text);
            AddTextPrinterParameterized(GetStartMenuWindowId(), FONT_NORMAL, gStringVar4, 8, i * 15, 0xFF, NULL);
        }
        i++;
        if (i >= sNumStartMenuItems)
        {
            *cursor_p = i;
            return TRUE;
        }
    } while (--nitems);
    *cursor_p = i;
    return FALSE;
}

static u8 GetRadialStartMenuPreferredSlot(u8 menuItem)
{
    switch (menuItem)
    {
    case STARTMENU_POKEDEX:
        return 0;
    case STARTMENU_POKEMON:
        return 3;
    case STARTMENU_BAG:
        return 2;
    case STARTMENU_SAVE:
        return 7;
    case STARTMENU_OPTION:
        return 4;
    case STARTMENU_QUEST:
        return 5;
    case STARTMENU_PLAYER:
    case STARTMENU_PLAYER2:
        return 6;
    case STARTMENU_EXIT:
        return 1;
    case STARTMENU_RETIRE:
    default:
        return 0;
    }
}

static void InitRadialStartMenuSlots(void)
{
    u8 i;

    for (i = 0; i < NELEMS(sRadialStartMenuWindowIds); i++)
    {
        sRadialStartMenuWindowIds[i] = WINDOW_NONE;
        sRadialStartMenuSlotToItem[i] = 0xFF;
        sRadialStartMenuSlotToOrderIndex[i] = 0xFF;
        sRadialStartMenuButtonSpriteIds[i] = MAX_SPRITES;
        sRadialStartMenuSpriteIds[i] = MAX_SPRITES;
    }
}

static void PopulateRadialStartMenuSlots(void)
{
    u8 i;
    u8 slot;
    u8 preferredSlot;

    InitRadialStartMenuSlots();
    for (i = 0; i < sNumStartMenuItems; i++)
    {
        preferredSlot = GetRadialStartMenuPreferredSlot(sStartMenuOrder[i]);
        for (slot = preferredSlot; sRadialStartMenuSlotToItem[slot] != 0xFF; slot = (slot + 1) & 7)
            ;
        sRadialStartMenuSlotToItem[slot] = sStartMenuOrder[i];
        sRadialStartMenuSlotToOrderIndex[slot] = i;
    }
}

static u8 GetFirstPopulatedRadialStartMenuSlot(void)
{
    u8 i;

    for (i = 0; i < NELEMS(sRadialStartMenuSlotToItem); i++)
    {
        if (sRadialStartMenuSlotToItem[i] != 0xFF)
            return i;
    }
    return 0;
}

static void SelectInitialRadialStartMenuSlot(void)
{
    u8 i;

    for (i = 0; i < NELEMS(sRadialStartMenuSlotToOrderIndex); i++)
    {
        if (sRadialStartMenuSlotToOrderIndex[i] == sStartMenuCursorPos)
        {
            sRadialStartMenuCursorSlot = i;
            return;
        }
    }
    sRadialStartMenuCursorSlot = GetFirstPopulatedRadialStartMenuSlot();
    sStartMenuCursorPos = sRadialStartMenuSlotToOrderIndex[sRadialStartMenuCursorSlot];
}

static void PrintRadialStartMenuItem(u8 slot)
{
    u8 windowId = sRadialStartMenuWindowIds[slot];
    u8 menuItem = sRadialStartMenuSlotToItem[slot];
    const u8 *colors;
    const u8 *text;
    s32 width;
    s32 x;

    if (windowId == WINDOW_NONE)
        return;

    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
    DrawStdWindowFrame(windowId, FALSE);

    colors = (slot == sRadialStartMenuCursorSlot) ? sTextColor_RadialMenuSelected : sTextColor_RadialMenuNormal;
    StringExpandPlaceholders(gStringVar4, sStartMenuActionTable[menuItem].text);
    text = gStringVar4;

    width = GetStringWidth(FONT_NORMAL, text, 0);
    x = (80 - width) / 2;
    if (x < 0)
        x = 0;
    AddTextPrinterParameterized3(windowId, FONT_NORMAL, x, 6, colors, 0xFF, text);
    CopyWindowToVram(windowId, COPYWIN_FULL);
}

static void DrawRadialStartMenu(void)
{
    u8 i;

    for (i = 0; i < NELEMS(sRadialStartMenuWindowIds); i++)
    {
        if (sRadialStartMenuWindowIds[i] != WINDOW_NONE)
            PrintRadialStartMenuItem(i);
    }
}

static u8 GetClampedRadialStartMenuTileCoord(s16 pixelCoord, u8 maxTile)
{
    s16 tileCoord = pixelCoord / 8;

    if (tileCoord < 0)
        return 0;
    if (tileCoord > maxTile)
        return maxTile;
    return tileCoord;
}

static u8 GetRadialStartMenuWindowLeft(u8 slot)
{
    u8 menuItem = sRadialStartMenuSlotToItem[slot];

    if (menuItem == STARTMENU_SAVE)
        return 0;
    if (menuItem == STARTMENU_EXIT)
        return 20;
    return GetClampedRadialStartMenuTileCoord(DISPLAY_WIDTH / 2 + sRadialStartMenuWindowXOffsets[slot], 20);
}

static u8 GetRadialStartMenuWindowTop(u8 slot)
{
    u8 menuItem = sRadialStartMenuSlotToItem[slot];

    if (menuItem == STARTMENU_SAVE || menuItem == STARTMENU_EXIT)
        return 0;
    return GetClampedRadialStartMenuTileCoord(DISPLAY_HEIGHT / 2 + sRadialStartMenuWindowYOffsets[slot], 17);
}

static s16 GetRadialStartMenuSpriteX(u8 slot)
{
    return DISPLAY_WIDTH / 2 + sRadialStartMenuWindowXOffsets[slot] + 40;
}

static s16 GetRadialStartMenuSpriteY(u8 slot)
{
    return DISPLAY_HEIGHT / 2 + sRadialStartMenuWindowYOffsets[slot] - 8;
}

static void CreateRadialStartMenu(void)
{
    u8 i;

    PopulateRadialStartMenuSlots();
    SelectInitialRadialStartMenuSlot();

    for (i = 0; i < NELEMS(sRadialStartMenuWindowIds); i++)
    {
        if (sRadialStartMenuSlotToItem[i] == STARTMENU_SAVE
         || sRadialStartMenuSlotToItem[i] == STARTMENU_EXIT)
        {
            struct WindowTemplate template = {
                .bg = 0,
                .tilemapLeft = GetRadialStartMenuWindowLeft(i),
                .tilemapTop = GetRadialStartMenuWindowTop(i),
                .width = 10,
                .height = 3,
                .paletteNum = 15,
                .baseBlock = 0x200 + (i * 0x20)
            };
            sRadialStartMenuWindowIds[i] = AddWindow(&template);
            PutWindowTilemap(sRadialStartMenuWindowIds[i]);
        }
    }
    DrawRadialStartMenu();
    CreateRadialStartMenuSprites();
}

static void DestroyRadialStartMenuWindows(bool8 copyToVram)
{
    u8 i;

    DestroyRadialStartMenuSprites();

    for (i = 0; i < NELEMS(sRadialStartMenuWindowIds); i++)
    {
        if (sRadialStartMenuWindowIds[i] != WINDOW_NONE)
        {
            ClearStdWindowAndFrameToTransparent(sRadialStartMenuWindowIds[i], copyToVram);
            RemoveWindow(sRadialStartMenuWindowIds[i]);
            sRadialStartMenuWindowIds[i] = WINDOW_NONE;
        }
    }
}

static void CreateRadialStartMenuSprites(void)
{
    u8 i;
    bool8 needButtonIcon = FALSE;
    bool8 needPokedexIcon = FALSE;
    bool8 needPokemonIcon = FALSE;
    bool8 needSettingsIcon = FALSE;
    bool8 needLogbookIcon = FALSE;
    bool8 needCardIcon = FALSE;
    bool8 needBagIcon = FALSE;

    if (sRadialStartMenuSpritesLoaded)
        return;

    for (i = 0; i < NELEMS(sRadialStartMenuSpriteIds); i++)
    {
        switch (sRadialStartMenuSlotToItem[i])
        {
        case STARTMENU_POKEDEX:
            needPokedexIcon = TRUE;
            break;
        case STARTMENU_POKEMON:
            needPokemonIcon = TRUE;
            break;
        case STARTMENU_BAG:
            needBagIcon = TRUE;
            break;
        case STARTMENU_OPTION:
            needSettingsIcon = TRUE;
            break;
        case STARTMENU_QUEST:
            needLogbookIcon = TRUE;
            break;
        case STARTMENU_PLAYER:
        case STARTMENU_PLAYER2:
            needCardIcon = TRUE;
            break;
        case STARTMENU_EXIT:
        case STARTMENU_SAVE:
        case 0xFF:
            break;
        default:
            needButtonIcon = TRUE;
            break;
        }
    }

    if (needPokedexIcon)
    {
        LoadCompressedSpriteSheet(&sSpriteSheet_RadialStartMenuPokedexIcon);
    }
    if (needLogbookIcon)
    {
        LoadCompressedSpriteSheet(&sSpriteSheet_RadialStartMenuLogbookIcon);
    }
    if (needSettingsIcon || needPokedexIcon || needPokemonIcon || needLogbookIcon || needCardIcon || needBagIcon || needButtonIcon)
    {
        LoadCompressedSpritePalette(&sSpritePalette_RadialStartMenuSettingsIcon);
    }
    if (needSettingsIcon)
    {
        LoadCompressedSpriteSheet(&sSpriteSheet_RadialStartMenuSettingsIcon);
    }
    if (needCardIcon)
    {
        LoadCompressedSpriteSheet(&sSpriteSheet_RadialStartMenuCardIcon);
    }
    if (needBagIcon)
    {
        LoadCompressedSpriteSheet(&sSpriteSheet_RadialStartMenuBagIcon);
    }
    if (needPokemonIcon)
    {
        LoadCompressedSpriteSheet(&sSpriteSheet_RadialStartMenuPokemonIcon);
    }
    if (needButtonIcon)
    {
        LoadCompressedSpriteSheet(&sSpriteSheet_RadialStartMenuButtonIcon);
    }
    sRadialStartMenuSpritesLoaded = TRUE;

    for (i = 0; i < NELEMS(sRadialStartMenuSpriteIds); i++)
    {
        if (sRadialStartMenuSlotToItem[i] != 0xFF
         && sRadialStartMenuSlotToItem[i] != STARTMENU_EXIT
         && sRadialStartMenuSlotToItem[i] != STARTMENU_SAVE
         && sRadialStartMenuSlotToItem[i] != STARTMENU_POKEMON
         && sRadialStartMenuSlotToItem[i] != STARTMENU_BAG
         && sRadialStartMenuSlotToItem[i] != STARTMENU_OPTION
         && sRadialStartMenuSlotToItem[i] != STARTMENU_QUEST
         && sRadialStartMenuSlotToItem[i] != STARTMENU_POKEDEX
         && sRadialStartMenuSlotToItem[i] != STARTMENU_PLAYER
         && sRadialStartMenuSlotToItem[i] != STARTMENU_PLAYER2)
        {
            s16 x = GetRadialStartMenuSpriteX(i);
            s16 y = GetRadialStartMenuSpriteY(i);

            sRadialStartMenuButtonSpriteIds[i] = CreateSprite(&sSpriteTemplate_RadialStartMenuButtonIcon, x, y, 1);
        }
        if (sRadialStartMenuSlotToItem[i] == STARTMENU_BAG)
        {
            s16 x = GetRadialStartMenuSpriteX(i);
            s16 y = GetRadialStartMenuSpriteY(i);

            sRadialStartMenuSpriteIds[i] = CreateSprite(&sSpriteTemplate_RadialStartMenuBagIcon, x, y, 0);
        }
        if (sRadialStartMenuSlotToItem[i] == STARTMENU_POKEDEX)
        {
            s16 x = GetRadialStartMenuSpriteX(i);
            s16 y = GetRadialStartMenuSpriteY(i);

            sRadialStartMenuSpriteIds[i] = CreateSprite(&sSpriteTemplate_RadialStartMenuPokedexIcon, x, y, 0);
        }
        if (sRadialStartMenuSlotToItem[i] == STARTMENU_POKEMON)
        {
            s16 x = GetRadialStartMenuSpriteX(i);
            s16 y = GetRadialStartMenuSpriteY(i);

            sRadialStartMenuSpriteIds[i] = CreateSprite(&sSpriteTemplate_RadialStartMenuPokemonIcon, x, y, 0);
        }
        if (sRadialStartMenuSlotToItem[i] == STARTMENU_OPTION)
        {
            s16 x = GetRadialStartMenuSpriteX(i);
            s16 y = GetRadialStartMenuSpriteY(i);

            sRadialStartMenuSpriteIds[i] = CreateSprite(&sSpriteTemplate_RadialStartMenuSettingsIcon, x, y, 0);
        }
        if (sRadialStartMenuSlotToItem[i] == STARTMENU_QUEST)
        {
            s16 x = GetRadialStartMenuSpriteX(i);
            s16 y = GetRadialStartMenuSpriteY(i);

            sRadialStartMenuSpriteIds[i] = CreateSprite(&sSpriteTemplate_RadialStartMenuLogbookIcon, x, y, 0);
        }
        if (sRadialStartMenuSlotToItem[i] == STARTMENU_PLAYER
         || sRadialStartMenuSlotToItem[i] == STARTMENU_PLAYER2)
        {
            s16 x = GetRadialStartMenuSpriteX(i);
            s16 y = GetRadialStartMenuSpriteY(i);

            sRadialStartMenuSpriteIds[i] = CreateSprite(&sSpriteTemplate_RadialStartMenuCardIcon, x, y, 0);
        }
    }
    UpdateRadialStartMenuSpriteStates();
}

static void DestroyRadialStartMenuSprites(void)
{
    u8 i;

    if (!sRadialStartMenuSpritesLoaded)
        return;

    for (i = 0; i < NELEMS(sRadialStartMenuSpriteIds); i++)
    {
        if (sRadialStartMenuButtonSpriteIds[i] != MAX_SPRITES)
        {
            DestroySprite(&gSprites[sRadialStartMenuButtonSpriteIds[i]]);
            sRadialStartMenuButtonSpriteIds[i] = MAX_SPRITES;
        }
        if (sRadialStartMenuSpriteIds[i] != MAX_SPRITES)
        {
            DestroySprite(&gSprites[sRadialStartMenuSpriteIds[i]]);
            sRadialStartMenuSpriteIds[i] = MAX_SPRITES;
        }
    }
    FreeSpriteTilesByTag(TAG_START_MENU_BUTTON_ICON);
    FreeSpritePaletteByTag(TAG_START_MENU_BUTTON_ICON);
    FreeSpriteTilesByTag(TAG_START_MENU_POKEMON_ICON);
    FreeSpriteTilesByTag(TAG_START_MENU_SETTINGS_ICON);
    FreeSpritePaletteByTag(TAG_START_MENU_SETTINGS_ICON);
    FreeSpriteTilesByTag(TAG_START_MENU_LOGBOOK_ICON);
    FreeSpriteTilesByTag(TAG_START_MENU_POKEDEX_ICON);
    FreeSpriteTilesByTag(TAG_START_MENU_CARD_ICON);
    FreeSpriteTilesByTag(TAG_START_MENU_BAG_ICON);
    sRadialStartMenuSpritesLoaded = FALSE;
}

static void UpdateRadialStartMenuSpriteStates(void)
{
    u8 i;

    for (i = 0; i < NELEMS(sRadialStartMenuSpriteIds); i++)
    {
        if (sRadialStartMenuButtonSpriteIds[i] != MAX_SPRITES)
            StartSpriteAffineAnim(&gSprites[sRadialStartMenuButtonSpriteIds[i]], i == sRadialStartMenuCursorSlot ? 1 : 0);
        if (sRadialStartMenuSpriteIds[i] != MAX_SPRITES)
        {
            StartSpriteAnim(&gSprites[sRadialStartMenuSpriteIds[i]], i == sRadialStartMenuCursorSlot ? 1 : 0);
            StartSpriteAffineAnim(&gSprites[sRadialStartMenuSpriteIds[i]], i == sRadialStartMenuCursorSlot ? 1 : 0);
        }
    }
}

static void RefreshStartMenuHelpText(void)
{
    DrawHelpMessageWindowWithText(sStartMenuDescPointers[sStartMenuOrder[sStartMenuCursorPos]]);
}

static void UpdateRadialStartMenuSelection(u8 newSlot)
{
    u8 oldSlot;

    if (newSlot == sRadialStartMenuCursorSlot || sRadialStartMenuSlotToItem[newSlot] == 0xFF)
        return;

    oldSlot = sRadialStartMenuCursorSlot;
    sRadialStartMenuCursorSlot = newSlot;
    sStartMenuCursorPos = sRadialStartMenuSlotToOrderIndex[newSlot];
    PrintRadialStartMenuItem(oldSlot);
    PrintRadialStartMenuItem(newSlot);
    UpdateRadialStartMenuSpriteStates();
    RefreshStartMenuHelpText();
}

static u8 FindNextRadialStartMenuSlot(s8 step)
{
    u8 i;
    u8 slot = sRadialStartMenuCursorSlot;

    for (i = 0; i < NELEMS(sRadialStartMenuSlotToItem); i++)
    {
        slot = (slot + step) & 7;
        if (sRadialStartMenuSlotToItem[slot] != 0xFF)
            return slot;
    }
    return sRadialStartMenuCursorSlot;
}

static u8 FindRadialStartMenuSlotByItem(u8 menuItem)
{
    u8 i;

    for (i = 0; i < NELEMS(sRadialStartMenuSlotToItem); i++)
    {
        if (sRadialStartMenuSlotToItem[i] == menuItem)
            return i;
    }
    return 0xFF;
}

static u8 FindRadialStartMenuSlotInDirection(s8 dx, s8 dy)
{
    u8 i;
    u8 bestSlot = 0xFF;
    u8 targetSlot;
    u8 currentItem = sRadialStartMenuSlotToItem[sRadialStartMenuCursorSlot];
    s16 bestScore = -32768;
    s16 fromX = sRadialStartMenuSlotXs[sRadialStartMenuCursorSlot];
    s16 fromY = sRadialStartMenuSlotYs[sRadialStartMenuCursorSlot];

    if ((currentItem == STARTMENU_POKEDEX && dx == 0 && dy < 0)
     || (currentItem == STARTMENU_OPTION && dx == 0 && dy > 0)
     || (currentItem == STARTMENU_QUEST && dx < 0 && dy == 0)
     || (currentItem == STARTMENU_QUEST && dx == 0 && dy > 0)
     || (currentItem == STARTMENU_POKEMON && dx > 0 && dy == 0)
     || (currentItem == STARTMENU_POKEMON && dx == 0 && dy > 0)
     || ((currentItem == STARTMENU_PLAYER || currentItem == STARTMENU_PLAYER2) && dx < 0 && dy == 0)
     || ((currentItem == STARTMENU_PLAYER || currentItem == STARTMENU_PLAYER2) && dx == 0 && dy < 0)
     || (currentItem == STARTMENU_BAG && dx == 0 && dy < 0)
     || (currentItem == STARTMENU_BAG && dx > 0 && dy == 0))
        return sRadialStartMenuCursorSlot;

    if (currentItem == STARTMENU_POKEDEX)
    {
        if (dx < 0 && dy == 0)
        {
            targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_PLAYER);
            if (targetSlot == 0xFF)
                targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_PLAYER2);
        }
        else if (dx > 0 && dy == 0)
            targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_BAG);
        else
            targetSlot = 0xFF;

        if (targetSlot != 0xFF)
            return targetSlot;
    }
    if ((currentItem == STARTMENU_PLAYER || currentItem == STARTMENU_PLAYER2) && dx > 0 && dy == 0)
    {
        targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_POKEDEX);
        if (targetSlot != 0xFF)
            return targetSlot;
    }
    if (currentItem == STARTMENU_BAG && dx < 0 && dy == 0)
    {
        targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_POKEDEX);
        if (targetSlot != 0xFF)
            return targetSlot;
    }

    if (currentItem == STARTMENU_OPTION)
    {
        if (dx < 0 && dy == 0)
            targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_QUEST);
        else if (dx > 0 && dy == 0)
            targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_POKEMON);
        else
            targetSlot = 0xFF;

        if (targetSlot != 0xFF)
            return targetSlot;
    }
    if (currentItem == STARTMENU_POKEMON)
    {
        if (dx == 0 && dy < 0)
            targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_BAG);
        else if (dx < 0 && dy == 0)
            targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_OPTION);
        else
            targetSlot = 0xFF;

        if (targetSlot != 0xFF)
            return targetSlot;
    }
    if (currentItem == STARTMENU_QUEST)
    {
        if (dx == 0 && dy < 0)
        {
            targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_PLAYER);
            if (targetSlot == 0xFF)
                targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_PLAYER2);
        }
        else if (dx > 0 && dy == 0)
            targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_OPTION);
        else
            targetSlot = 0xFF;

        if (targetSlot != 0xFF)
            return targetSlot;
    }

    for (i = 0; i < NELEMS(sRadialStartMenuSlotToItem); i++)
    {
        s16 relX;
        s16 relY;
        s16 projection;
        s16 perpendicular;
        s16 score;

        if (i == sRadialStartMenuCursorSlot || sRadialStartMenuSlotToItem[i] == 0xFF)
            continue;

        relX = sRadialStartMenuSlotXs[i] - fromX;
        relY = sRadialStartMenuSlotYs[i] - fromY;
        projection = relX * dx + relY * dy;
        if (projection <= 0)
            continue;

        perpendicular = relX * dy - relY * dx;
        if (perpendicular < 0)
            perpendicular = -perpendicular;
        score = projection * 4 - perpendicular;
        if (score > bestScore)
        {
            bestScore = score;
            bestSlot = i;
        }
    }

    if (bestSlot == 0xFF)
        return dx + dy > 0 ? FindNextRadialStartMenuSlot(1) : FindNextRadialStartMenuSlot(-1);
    return bestSlot;
}

static s8 DoDrawStartMenu(void)
{
    switch (sDrawStartMenuState[0])
    {
    case 0:
        MapNamePopupWindowIdSetDummy();
        sDrawStartMenuState[0]++;
        break;
    case 1:
        SetUpStartMenu();
        sDrawStartMenuState[0]++;
        break;
    case 2:
        LoadStdWindowFrameGfx();
        CreateRadialStartMenu();
        sDrawStartMenuState[0]++;
        break;
    case 3:
        if (GetSafariZoneFlag())
            DrawSafariZoneStatsWindow();
        sDrawStartMenuState[0]++;
        break;
    case 4:
        sDrawStartMenuState[0]++;
        break;
    case 5:
        RefreshStartMenuHelpText();
        return TRUE;
    }
    return FALSE;
}

static void DrawStartMenuInOneGo(void)
{
    sDrawStartMenuState[0] = 0;
    sDrawStartMenuState[1] = 0;
    while (!DoDrawStartMenu())
        ;
}

static void task50_startmenu(u8 taskId)
{
    if (DoDrawStartMenu() == TRUE)
        SwitchTaskToFollowupFunc(taskId);
}

static void OpenStartMenuWithFollowupFunc(TaskFunc func)
{
    u8 taskId;
    sDrawStartMenuState[0] = 0;
    sDrawStartMenuState[1] = 0;
    taskId = CreateTask(task50_startmenu, 80);
    SetTaskFuncWithFollowupFunc(taskId, task50_startmenu, func);
}

static bool8 FieldCB2_DrawStartMenu(void)
{
    if (!DoDrawStartMenu())
        return FALSE;
    FadeTransition_FadeInOnReturnToStartMenu();
    return TRUE;
}

void SetUpReturnToStartMenu(void)
{
    sDrawStartMenuState[0] = 0;
    sDrawStartMenuState[1] = 0;
    gFieldCallback2 = FieldCB2_DrawStartMenu;
}

void Task_StartMenuHandleInput(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    switch (data[0])
    {
    case 0:
        if (InUnionRoom() == TRUE)
            SetUsingUnionRoomStartMenu();
        sStartMenuCallback = StartCB_HandleInput;
        data[0]++;
        break;
    case 1:
        if (sStartMenuCallback() == TRUE)
            DestroyTask(taskId);
        break;
    }
}

void ShowStartMenu(void)
{
    if (!IsUpdateLinkStateCBActive())
    {
        FreezeObjectEvents();
        HandleEnforcedLookDirectionOnPlayerStopMoving();
        StopPlayerAvatar();
    }
    OpenStartMenuWithFollowupFunc(Task_StartMenuHandleInput);
    LockPlayerFieldControls();
}

static bool8 StartCB_HandleInput(void)
{
    if (JOY_NEW(DPAD_UP))
    {
        PlaySE(SE_SELECT);
        UpdateRadialStartMenuSelection(FindRadialStartMenuSlotInDirection(0, -1));
    }
    if (JOY_NEW(DPAD_DOWN))
    {
        PlaySE(SE_SELECT);
        UpdateRadialStartMenuSelection(FindRadialStartMenuSlotInDirection(0, 1));
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        PlaySE(SE_SELECT);
        UpdateRadialStartMenuSelection(FindRadialStartMenuSlotInDirection(-1, 0));
    }
    if (JOY_NEW(DPAD_RIGHT))
    {
        PlaySE(SE_SELECT);
        UpdateRadialStartMenuSelection(FindRadialStartMenuSlotInDirection(1, 0));
    }
    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        if (!StartMenuPokedexSanityCheck())
            return FALSE;
        sStartMenuCallback = sStartMenuActionTable[sStartMenuOrder[sStartMenuCursorPos]].func.u8_void;
        StartMenu_FadeScreenIfLeavingOverworld();
        return FALSE;
    }
    if (JOY_NEW(B_BUTTON | START_BUTTON))
    {
        DestroySafariZoneStatsWindow();
        DestroyHelpMessageWindow_();
        CloseStartMenu();
        return TRUE;
    }
    return FALSE;
}

static void StartMenu_FadeScreenIfLeavingOverworld(void)
{
    if (sStartMenuCallback != StartMenuSaveCallback
     && sStartMenuCallback != StartMenuExitCallback
     && sStartMenuCallback != StartMenuSafariZoneRetireCallback)
    {
        StopPokemonLeagueLightingEffectTask();
        FadeScreen(FADE_TO_BLACK, 0);
    }
}

static bool8 StartMenuPokedexSanityCheck(void)
{
    if (sStartMenuActionTable[sStartMenuOrder[sStartMenuCursorPos]].func.u8_void == StartMenuPokedexCallback && GetNationalPokedexCount(0) == 0)
        return FALSE;
    return TRUE;
}

static bool8 StartMenuPokedexCallback(void)
{
    if (!gPaletteFade.active)
    {
        IncrementGameStat(GAME_STAT_CHECKED_POKEDEX);
        PlayRainStoppingSoundEffect();
        DestroySafariZoneStatsWindow();
        DestroyRadialStartMenuWindows(FALSE);
        CleanupOverworldWindowsAndTilemaps();
        SetMainCallback2(CB2_OpenPokedexFromStartMenu);
        return TRUE;
    }
    return FALSE;
}

static bool8 StartMenuPokemonCallback(void)
{
    if (!gPaletteFade.active)
    {
        PlayRainStoppingSoundEffect();
        DestroySafariZoneStatsWindow();
        DestroyRadialStartMenuWindows(FALSE);
        CleanupOverworldWindowsAndTilemaps();
        SetMainCallback2(CB2_PartyMenuFromStartMenu);
        return TRUE;
    }
    return FALSE;
}

static bool8 StartMenuBagCallback(void)
{
    if (!gPaletteFade.active)
    {
        PlayRainStoppingSoundEffect();
        DestroySafariZoneStatsWindow();
        DestroyRadialStartMenuWindows(FALSE);
        CleanupOverworldWindowsAndTilemaps();
        SetMainCallback2(CB2_BagMenuFromStartMenu);
        return TRUE;
    }
    return FALSE;
}

static bool8 StartMenuPlayerCallback(void)
{
    if (!gPaletteFade.active)
    {
        PlayRainStoppingSoundEffect();
        DestroySafariZoneStatsWindow();
        DestroyRadialStartMenuWindows(FALSE);
        CleanupOverworldWindowsAndTilemaps();
        ShowPlayerTrainerCard(CB2_ReturnToFieldWithOpenMenu);
        return TRUE;
    }
    return FALSE;
}

static bool8 StartMenuSaveCallback(void)
{
    sStartMenuCallback = StartCB_Save1;
    return FALSE;
}

static bool8 StartMenuOptionCallback(void)
{
    if (!gPaletteFade.active)
    {
        PlayRainStoppingSoundEffect();
        DestroySafariZoneStatsWindow();
        DestroyRadialStartMenuWindows(FALSE);
        CleanupOverworldWindowsAndTilemaps();
        SetMainCallback2(CB2_OptionsMenuFromStartMenu);
        gMain.savedCallback = CB2_ReturnToFieldWithOpenMenu;
        return TRUE;
    }
    return FALSE;
}

static bool8 StartMenuExitCallback(void)
{
    DestroySafariZoneStatsWindow();
    DestroyHelpMessageWindow_();
    CloseStartMenu();
    return TRUE;
}

static bool8 StartMenuSafariZoneRetireCallback(void)
{
    DestroySafariZoneStatsWindow();
    DestroyHelpMessageWindow_();
    CloseStartMenu();
    SafariZoneRetirePrompt();
    return TRUE;
}


static bool8 StartMenuLinkPlayerCallback(void)
{
    if (!gPaletteFade.active)
    {
        PlayRainStoppingSoundEffect();
        DestroyRadialStartMenuWindows(FALSE);
        CleanupOverworldWindowsAndTilemaps();
        ShowTrainerCardInLink(gLocalLinkPlayerId, CB2_ReturnToFieldWithOpenMenu);
        return TRUE;
    }
    return FALSE;
}

static bool8 StartCB_Save1(void)
{
    BackupHelpContext();
    SetHelpContext(HELPCONTEXT_SAVE);
    StartMenu_PrepareForSave();
    sStartMenuCallback = StartCB_Save2;
    return FALSE;
}

static bool8 StartCB_Save2(void)
{
    switch (RunSaveDialogCB())
    {
    case SAVECB_RETURN_CONTINUE:
        break;
    case SAVECB_RETURN_OKAY:
        ClearDialogWindowAndFrameToTransparent(0, TRUE);
        ClearPlayerHeldMovementAndUnfreezeObjectEvents();
        UnlockPlayerFieldControls();
        RestoreHelpContext();
        return TRUE;
    case SAVECB_RETURN_CANCEL:
        ClearDialogWindowAndFrameToTransparent(0, FALSE);
        DrawStartMenuInOneGo();
        RestoreHelpContext();
        sStartMenuCallback = StartCB_HandleInput;
        break;
    case SAVECB_RETURN_ERROR:
        ClearDialogWindowAndFrameToTransparent(0, TRUE);
        ClearPlayerHeldMovementAndUnfreezeObjectEvents();
        UnlockPlayerFieldControls();
        RestoreHelpContext();
        return TRUE;
    }
    return FALSE;
}

static void StartMenu_PrepareForSave(void)
{
    SaveMapView();
    sSaveDialogCB = SaveDialogCB_PrintAskSaveText;
    sSaveDialogIsPrinting = FALSE;
}

static u8 RunSaveDialogCB(void)
{
    if (RunTextPrinters_CheckPrinter0Active() == TRUE)
        return 0;
    sSaveDialogIsPrinting = FALSE;
    return sSaveDialogCB();
}

void Field_AskSaveTheGame(void)
{
    BackupHelpContext();
    SetHelpContext(HELPCONTEXT_SAVE);
    StartMenu_PrepareForSave();
    CreateTask(task50_save_game, 80);
}

static void PrintSaveTextWithFollowupFunc(const u8 *str, bool8 (*saveDialogCB)(void))
{
    StringExpandPlaceholders(gStringVar4, str);
    LoadMessageBoxAndFrameGfx(0, TRUE);
    AddTextPrinterForMessage(TRUE);
    sSaveDialogIsPrinting = TRUE;
    sSaveDialogCB = saveDialogCB;
}

static void task50_save_game(u8 taskId)
{
    switch (RunSaveDialogCB())
    {
    case 0:
        return;
    case 2:
    case 3:
        gSpecialVar_Result = FALSE;
        break;
    case 1:
        gSpecialVar_Result = TRUE;
        break;
    }
    DestroyTask(taskId);
    ScriptContext_Enable();
    RestoreHelpContext();
}

static void CloseSaveMessageWindow(void)
{
    ClearDialogWindowAndFrame(0, TRUE);
}

static void CloseSaveStatsWindow_(void)
{
    CloseSaveStatsWindow();
}

static void SetSaveDialogDelayTo60Frames(void)
{
    sSaveDialogDelay = 60;
}

static bool8 SaveDialog_Wait60FramesOrAButtonHeld(void)
{
    sSaveDialogDelay--;
    if (JOY_HELD(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        return TRUE;
    }
    else if (sSaveDialogDelay == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static bool8 SaveDialog_Wait60FramesThenCheckAButtonHeld(void)
{
    if (sSaveDialogDelay == 0)
    {
        if (JOY_HELD(A_BUTTON))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        sSaveDialogDelay--;
        return FALSE;
    }
}

static u8 SaveDialogCB_PrintAskSaveText(void)
{
    DestroyRadialStartMenuWindows(FALSE);
    DestroyHelpMessageWindow(0);
    PrintSaveStats();
    PrintSaveTextWithFollowupFunc(gText_WouldYouLikeToSaveTheGame, SaveDialogCB_AskSavePrintYesNoMenu);
    return SAVECB_RETURN_CONTINUE;
}

static u8 SaveDialogCB_AskSavePrintYesNoMenu(void)
{
    DisplayYesNoMenuDefaultYes();
    sSaveDialogCB = SaveDialogCB_AskSaveHandleInput;
    return SAVECB_RETURN_CONTINUE;
}

static u8 SaveDialogCB_AskSaveHandleInput(void)
{
    switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
    case 0:
        if ((gSaveFileStatus != SAVE_STATUS_EMPTY && gSaveFileStatus != SAVE_STATUS_INVALID) || !gDifferentSaveFile)
            sSaveDialogCB = SaveDialogCB_PrintAskOverwriteText;
        else
            sSaveDialogCB = SaveDialogCB_PrintSavingDontTurnOffPower;
        break;
    case 1:
    case -1:
        CloseSaveStatsWindow_();
        CloseSaveMessageWindow();
        return SAVECB_RETURN_CANCEL;
    }
    return SAVECB_RETURN_CONTINUE;
}

static u8 SaveDialogCB_PrintAskOverwriteText(void)
{
    if (gDifferentSaveFile == TRUE)
        PrintSaveTextWithFollowupFunc(gText_DifferentGameFile, SaveDialogCB_AskReplacePreviousFilePrintYesNoMenu);
    else
        PrintSaveTextWithFollowupFunc(gText_AlreadySaveFile_WouldLikeToOverwrite, SaveDialogCB_AskOverwritePrintYesNoMenu);
    return SAVECB_RETURN_CONTINUE;
}

static u8 SaveDialogCB_AskOverwritePrintYesNoMenu(void)
{
    DisplayYesNoMenuDefaultYes();
    sSaveDialogCB = SaveDialogCB_AskOverwriteOrReplacePreviousFileHandleInput;
    return SAVECB_RETURN_CONTINUE;
}

static u8 SaveDialogCB_AskReplacePreviousFilePrintYesNoMenu(void)
{
    DisplayYesNoMenuDefaultNo();
    sSaveDialogCB = SaveDialogCB_AskOverwriteOrReplacePreviousFileHandleInput;
    return SAVECB_RETURN_CONTINUE;
}

static u8 SaveDialogCB_AskOverwriteOrReplacePreviousFileHandleInput(void)
{
    switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
    case 0:
        sSaveDialogCB = SaveDialogCB_PrintSavingDontTurnOffPower;
        break;
    case 1:
    case -1:
        CloseSaveStatsWindow_();
        CloseSaveMessageWindow();
        return SAVECB_RETURN_CANCEL;
    }
    return SAVECB_RETURN_CONTINUE;
}

static u8 SaveDialogCB_PrintSavingDontTurnOffPower(void)
{
    SaveQuestLogData();
    PrintSaveTextWithFollowupFunc(gText_SavingDontTurnOffThePower, SaveDialogCB_DoSave);
    return SAVECB_RETURN_CONTINUE;
}

static u8 SaveDialogCB_DoSave(void)
{
    IncrementGameStat(GAME_STAT_SAVED_GAME);
    if (gDifferentSaveFile == TRUE)
    {
        TrySavingData(SAVE_OVERWRITE_DIFFERENT_FILE);
        gDifferentSaveFile = FALSE;
    }
    else
    {
        TrySavingData(SAVE_NORMAL);
    }
    sSaveDialogCB = SaveDialogCB_PrintSaveResult;
    return SAVECB_RETURN_CONTINUE;
}

static u8 SaveDialogCB_PrintSaveResult(void)
{
    if (gSaveAttemptStatus == SAVE_STATUS_OK)
        PrintSaveTextWithFollowupFunc(gText_PlayerSavedTheGame, SaveDialogCB_WaitPrintSuccessAndPlaySE);
    else
        PrintSaveTextWithFollowupFunc(gText_SaveError_PleaseExchangeBackupMemory, SaveDialogCB_WaitPrintErrorAndPlaySE);
    SetSaveDialogDelayTo60Frames();
    return SAVECB_RETURN_CONTINUE;
}

static u8 SaveDialogCB_WaitPrintSuccessAndPlaySE(void)
{
    if (!RunTextPrinters_CheckPrinter0Active())
    {
        PlaySE(SE_SAVE);
        sSaveDialogCB = SaveDialogCB_ReturnSuccess;
    }
    return SAVECB_RETURN_CONTINUE;
}

static u8 SaveDialogCB_ReturnSuccess(void)
{
    if (!IsSEPlaying() && SaveDialog_Wait60FramesOrAButtonHeld())
    {
        CloseSaveStatsWindow_();
        return SAVECB_RETURN_OKAY;
    }
    return SAVECB_RETURN_CONTINUE;
}

static u8 SaveDialogCB_WaitPrintErrorAndPlaySE(void)
{
    if (!RunTextPrinters_CheckPrinter0Active())
    {
        PlaySE(SE_BOO);
        sSaveDialogCB = SaveDialogCB_ReturnError;
    }
    return SAVECB_RETURN_CONTINUE;
}

static u8 SaveDialogCB_ReturnError(void)
{
    if (!SaveDialog_Wait60FramesThenCheckAButtonHeld())
        return SAVECB_RETURN_CONTINUE;
    CloseSaveStatsWindow_();
    return SAVECB_RETURN_ERROR;
}

static void VBlankCB_WhileSavingAfterLinkBattle(void)
{
    TransferPlttBuffer();
}

bool32 DoSetUpSaveAfterLinkBattle(u8 *state)
{
    switch (*state)
    {
    case 0:
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        SetVBlankCallback(NULL);
        ScanlineEffect_Stop();
        DmaFill16Defvars(3, 0, (void *)PLTT, PLTT_SIZE);
        DmaFillLarge16(3, 0, (void *)VRAM, VRAM_SIZE, 0x1000);
        break;
    case 1:
        ResetSpriteData();
        ResetTasks();
        ResetPaletteFade();
        ScanlineEffect_Clear();
        break;
    case 2:
        ResetBgsAndClearDma3BusyFlags(FALSE);
        InitBgsFromTemplates(0, sBGTemplates_AfterLinkSaveMessage, NELEMS(sBGTemplates_AfterLinkSaveMessage));
        InitWindows(sWindowTemplates_AfterLinkSaveMessage);
        LoadStdWindowGfx(0, 0x008, BG_PLTT_ID(15));
        break;
    case 3:
        ShowBg(0);
        BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
        SetVBlankCallback(VBlankCB_WhileSavingAfterLinkBattle);
        EnableInterrupts(INTR_FLAG_VBLANK);
        break;
    case 4:
        return TRUE;
    }
    (*state)++;
    return FALSE;
}

void CB2_SetUpSaveAfterLinkBattle(void)
{
    if (DoSetUpSaveAfterLinkBattle(&gMain.state))
    {
        CreateTask(task50_after_link_battle_save, 80);
        SetMainCallback2(CB2_WhileSavingAfterLinkBattle);
    }
}

static void CB2_WhileSavingAfterLinkBattle(void)
{
    RunTasks();
    UpdatePaletteFade();
}

static void task50_after_link_battle_save(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (!gPaletteFade.active)
    {
        switch (data[0])
        {
        case 0:
            FillWindowPixelBuffer(0, PIXEL_FILL(1));
            AddTextPrinterParameterized2(0, FONT_NORMAL, gText_SavingDontTurnOffThePower2, 0xFF, NULL, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY);
            DrawTextBorderOuter(0, 0x008, 15);
            PutWindowTilemap(0);
            CopyWindowToVram(0, COPYWIN_FULL);
            BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
            if (gWirelessCommType != 0 && InUnionRoom())
                data[0] = 5;
            else
                data[0] = 1;
            break;
        case 1:
            SetContinueGameWarpStatusToDynamicWarp();
            WriteSaveBlock2();
            data[0] = 2;
            break;
        case 2:
            if (WriteSaveBlock1Sector())
            {
                ClearContinueGameWarpStatus2();
#if REVISION >= 0xA
                svc_FinishSave();
#endif
                data[0] = 3;
            }
            break;
        case 3:
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            data[0] = 4;
            break;
        case 4:
            FreeAllWindowBuffers();
            SetMainCallback2(gMain.savedCallback);
            DestroyTask(taskId);
            break;
        case 5:
            CreateTask(Task_LinkFullSave, 5);
            data[0] = 6;
            break;
        case 6:
            if (!FuncIsActiveTask(Task_LinkFullSave))
                data[0] = 3;
            break;
        }
    }
}

static void PrintSaveStats(void)
{
    u8 y;
    u8 x;
    sSaveStatsWindowId = AddWindow(&sSaveStatsWindowTemplate);
    LoadStdWindowGfx(sSaveStatsWindowId, 0x21D, BG_PLTT_ID(13));
    DrawStdFrameWithCustomTileAndPalette(sSaveStatsWindowId, FALSE, 0x21D, 13);
    SaveStatToString(SAVE_STAT_LOCATION, gStringVar4, 8);
    x = (u32)(112 - GetStringWidth(FONT_NORMAL, gStringVar4, -1)) / 2;
    AddTextPrinterParameterized3(sSaveStatsWindowId, FONT_NORMAL, x, 0, sTextColor_LocationHeader, -1, gStringVar4);
    x = (u32)(112 - GetStringWidth(FONT_NORMAL, gStringVar4, -1)) / 2;
    AddTextPrinterParameterized3(sSaveStatsWindowId, FONT_SMALL, 2, 14, sTextColor_StatName, -1, gSaveStatName_Player);
    SaveStatToString(SAVE_STAT_NAME, gStringVar4, 2);
    Menu_PrintFormatIntlPlayerName(sSaveStatsWindowId, gStringVar4, 60, 14);
    AddTextPrinterParameterized3(sSaveStatsWindowId, FONT_SMALL, 2, 28, sTextColor_StatName, -1, gSaveStatName_Badges);
    SaveStatToString(SAVE_STAT_BADGES, gStringVar4, 2);
    AddTextPrinterParameterized3(sSaveStatsWindowId, FONT_SMALL, 60, 28, sTextColor_StatValue, -1, gStringVar4);
    y = 42;
    if (FlagGet(FLAG_SYS_POKEDEX_GET) == TRUE)
    {
        AddTextPrinterParameterized3(sSaveStatsWindowId, FONT_SMALL, 2, 42, sTextColor_StatName, -1, gSaveStatName_Pokedex);
        SaveStatToString(SAVE_STAT_POKEDEX, gStringVar4, 2);
        AddTextPrinterParameterized3(sSaveStatsWindowId, FONT_SMALL, 60, 42, sTextColor_StatValue, -1, gStringVar4);
        y = 56;
    }
    AddTextPrinterParameterized3(sSaveStatsWindowId, FONT_SMALL, 2, y, sTextColor_StatName, -1, gSaveStatName_Time);
    SaveStatToString(SAVE_STAT_TIME, gStringVar4, 2);
    AddTextPrinterParameterized3(sSaveStatsWindowId, FONT_SMALL, 60, y, sTextColor_StatValue, -1, gStringVar4);
    CopyWindowToVram(sSaveStatsWindowId, COPYWIN_GFX);
}

static void CloseSaveStatsWindow(void)
{
    ClearStdWindowAndFrame(sSaveStatsWindowId, FALSE);
    RemoveWindow(sSaveStatsWindowId);
}

static void CloseStartMenu(void)
{
    PlaySE(SE_SELECT);
    DestroyRadialStartMenuWindows(TRUE);
    ClearPlayerHeldMovementAndUnfreezeObjectEvents();
    UnlockPlayerFieldControls();
}

void AppendToList(u8 *list, u8 *cursor, u8 newEntry)
{
    list[*cursor] = newEntry;
    (*cursor)++;
}

static bool8 StartMenuQuestCallback(void)
{
    if (!gPaletteFade.active)
    {
        PlayRainStoppingSoundEffect();
        DestroySafariZoneStatsWindow();
        DestroyRadialStartMenuWindows(FALSE);
        CleanupOverworldWindowsAndTilemaps();
        QuestMenu_Init(0, CB2_ReturnToFieldWithOpenMenu);
        return TRUE;
    }
    return FALSE;
}
