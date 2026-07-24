#include "global.h"
#include "gflib.h"
#include "decompress.h"
#include "scanline_effect.h"
#include "overworld.h"
#include "link.h"
#include "pokedex.h"
#include "item.h"
#include "item_menu.h"
#include "party_menu.h"
#include "region_map.h"
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
#include "palette.h"
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
#include "help_menu.h"
#include "constants/songs.h"
#include "constants/field_weather.h"
#include "constants/items.h"
#include "sloopsvc.h"
#include "quests.h"
#include "ui_hint_header.h"

enum StartMenuOption
{
    STARTMENU_POKEDEX = 0,
    STARTMENU_POKEMON,
    STARTMENU_BAG,
    STARTMENU_QUEST,
    STARTMENU_PLAYER,
    STARTMENU_SAVE,
    STARTMENU_SETTINGS,
    STARTMENU_OPTION,
    STARTMENU_EXIT,
    STARTMENU_RETIRE,
    STARTMENU_PLAYER2,
    STARTMENU_MAP,
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
static EWRAM_DATA u8 sRadialStartMenuSpriteIds[8] = {};
static EWRAM_DATA u8 sRadialStartMenuBackdropSpriteId = MAX_SPRITES;
static EWRAM_DATA u8 sRadialStartMenuCursorSlot = 0;
static EWRAM_DATA bool8 sRadialStartMenuSpritesLoaded = FALSE;
static EWRAM_DATA bool8 sRadialStartMenuIconsVisible = FALSE;
static EWRAM_DATA u8 sStartMenuReturnItem = 0xFF;
static EWRAM_DATA u8 sStartMenuHeaderWindowId = WINDOW_NONE;
static EWRAM_DATA u8 sStartMenuCenterLabelWindowId = WINDOW_NONE;
static EWRAM_DATA u16 sStartMenuHeaderBgBackup[30 * 2] = {};
static EWRAM_DATA u16 sStartMenuCenterLabelBgBackup[9 * 2] = {};
static EWRAM_DATA u16 sRadialStartMenuObjPalBackupUnfaded[2 * 16] = {};
static EWRAM_DATA u16 sRadialStartMenuObjPalBackupFaded[2 * 16] = {};
static EWRAM_DATA bool8 sRadialStartMenuObjPalBackupValid = FALSE;
static EWRAM_DATA u8 sSafariZoneStatsWindowId = 0;
static ALIGNED(4) EWRAM_DATA u8 sSaveStatsWindowId = WINDOW_NONE;

static u8 (*sSaveDialogCB)(void);
static u8 sSaveDialogDelay;
static bool8 sSaveDialogIsPrinting;

static const u8 gText_MenuQuest[] = _("LOGBOOK");
static const u8 sStartMenuDesc_Quest[] = _("Review your FIELD AIDE research,\nrumours, and active assignments.");
static const u8 sText_StartMenuHeaderHints[] = _("{L_BUTTON}SAVE {DPAD_ANY}PICK {A_BUTTON}OK {B_BUTTON}BACK");
static const u8 sText_MenuCard[] = _("CARD");
static const u8 sText_MenuMap[] = _("MAP");
static const u8 sStartMenuDesc_Map[] = _("View the KANTO region map.");

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
static bool8 StartMenuMapCallback(void);
static bool8 StartMenuPlayerCallback(void);
static bool8 StartMenuSaveCallback(void);
static bool8 StartMenuSettingsCallback(void);
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
static void CreateStartMenuHeaderWindow(void);
static void DestroyStartMenuHeaderWindow(bool8 copyToVram);
static void PrintStartMenuHeaderWindow(void);
static void CreateStartMenuCenterLabelWindow(void);
static void DestroyStartMenuCenterLabelWindow(bool8 copyToVram);
static void PrintStartMenuCenterLabel(void);
static const u8 *GetStartMenuCenterLabelText(u8 menuItem);
static bool8 StartMenuHasItem(u8 menuItem);
static void SaveStartMenuBg0TilemapRect(u16 *dest, u8 left, u8 top, u8 width, u8 height);
static void RestoreStartMenuBg0TilemapRect(const u16 *src, u8 left, u8 top, u8 width, u8 height, bool8 copyToVram);
static void BackupRadialStartMenuObjPalettes(void);
static void LoadRadialStartMenuObjPalettes(void);
static void RestoreRadialStartMenuObjPalettes(void);
static void CreateRadialStartMenuSprites(void);
static void DestroyRadialStartMenuSprites(void);
static void UpdateRadialStartMenuSpriteStates(void);
static void SetRadialStartMenuIconsVisible(bool8 visible);
static u8 GetRadialStartMenuIconIndex(u8 item);
static void SpriteCB_RadialStartMenuBackdrop(struct Sprite *sprite);
static void SpriteCB_RadialStartMenuIcon(struct Sprite *sprite);
static void UpdateRadialStartMenuSelection(u8 newSlot);
static u8 FindNextRadialStartMenuSlot(s8 step);
static u8 FindRadialStartMenuSlotInDirection(s8 dx, s8 dy);
static u8 GetRadialStartMenuWindowLeft(u8 slot);
static u8 GetRadialStartMenuWindowTop(u8 slot);
static u8 GetRadialStartMenuWindowWidth(u8 slot);
static s16 GetRadialStartMenuSpriteX(u8 slot);
static s16 GetRadialStartMenuSpriteY(u8 slot);
static void SetStartMenuReturnItem(u8 menuItem);
static bool8 IsSelectableRadialStartMenuSlot(u8 slot);
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
    [STARTMENU_SETTINGS] = { gText_MenuSettings, {.u8_void = StartMenuSettingsCallback} },
    [STARTMENU_OPTION]  = { gText_MenuOption,  {.u8_void = StartMenuOptionCallback} },
    [STARTMENU_EXIT]    = { gText_MenuExit,    {.u8_void = StartMenuExitCallback} },
    [STARTMENU_RETIRE]  = { gText_MenuRetire,  {.u8_void = StartMenuSafariZoneRetireCallback} },
    [STARTMENU_PLAYER2] = { gText_MenuPlayer,  {.u8_void = StartMenuLinkPlayerCallback} },
    [STARTMENU_MAP]     = { sText_MenuMap,     {.u8_void = StartMenuMapCallback} }
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
    gStartMenuDesc_Settings,
    gStartMenuDesc_Option,
    gStartMenuDesc_Exit,
    gStartMenuDesc_Retire,
    gStartMenuDesc_Player,
    sStartMenuDesc_Map
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

#define START_MENU_LABEL_PALETTE_NUM 15
#define START_MENU_LABEL_FILL_COLOR TEXT_COLOR_DARK_GRAY
#define START_MENU_CENTER_LABEL_INSET 3
#define START_MENU_CENTER_LABEL_WIDTH 58
#define START_MENU_CENTER_LABEL_TOP 3
#define START_MENU_CENTER_LABEL_HEIGHT 10
#define START_MENU_SELECTED_ICON_BOB_INTERVAL 8
#define START_MENU_ICON_OBJ_PAL_SLOT 15
#define START_MENU_OBJ_PAL_BACKUP_START START_MENU_ICON_OBJ_PAL_SLOT
#define START_MENU_OBJ_PAL_BACKUP_COUNT 1
#define START_MENU_HEADER_LEFT 0
#define START_MENU_HEADER_TOP 0
#define START_MENU_HEADER_WIDTH 30
#define START_MENU_HEADER_HEIGHT 2
#define START_MENU_RADIAL_Y_OFFSET (-8)
#define START_MENU_RADIAL_WINDOW_Y_OFFSET (START_MENU_RADIAL_Y_OFFSET / 8)
#define START_MENU_BACKDROP_START_Y (DISPLAY_HEIGHT / 2)
#define START_MENU_BACKDROP_FINAL_Y (START_MENU_BACKDROP_START_Y + START_MENU_RADIAL_Y_OFFSET)
#define START_MENU_BACKDROP_OPEN_FRAMES 6
#define START_MENU_CENTER_LABEL_WINDOW_LEFT 11
#define START_MENU_CENTER_LABEL_WINDOW_TOP (9 + START_MENU_RADIAL_WINDOW_Y_OFFSET)
#define START_MENU_CENTER_LABEL_WINDOW_WIDTH 8
#define START_MENU_CENTER_LABEL_WINDOW_HEIGHT 2

static const s8 sRadialStartMenuWindowXOffsets[] = { -40, -16, -16, -16, -40, -64, -64, -64 };
static const s8 sRadialStartMenuWindowYOffsets[] = { -20, -10, -6, 20, 32, 20, -6, -10 };
static const s8 sRadialStartMenuSlotXs[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
static const s8 sRadialStartMenuSlotYs[] = { -1, -1, 0, 1, 1, 1, 0, -1 };

#define TAG_START_MENU_ICONS        0x1250
#define TAG_START_MENU_BACKDROP     0x1257

#define START_MENU_ICON_FRAME_TILES      16
#define START_MENU_ICON_FRAMES_PER_ENTRY 2

enum
{
    RADIAL_START_MENU_ICON_POKEDEX,
    RADIAL_START_MENU_ICON_POKEMON,
    RADIAL_START_MENU_ICON_BAG,
    RADIAL_START_MENU_ICON_LOGBOOK,
    RADIAL_START_MENU_ICON_CARD,
    RADIAL_START_MENU_ICON_HELP,
    RADIAL_START_MENU_ICON_MAP,
    RADIAL_START_MENU_ICON_SETTINGS,
    RADIAL_START_MENU_ICON_COUNT,
    RADIAL_START_MENU_ICON_NONE = 0xFF
};

static const u32 sRadialStartMenuBackdropGfx[] = INCBIN_U32("graphics/start_menu/backdrop.4bpp.lz");
static const u32 sRadialStartMenuIconsGfx[] = INCBIN_U32("graphics/start_menu/icons.4bpp.lz");
static const u32 sRadialStartMenuSettingsIconPal[] = INCBIN_U32("graphics/start_menu/settings.gbapal.lz");

static const struct OamData sOamData_RadialStartMenuIcon = {
    .affineMode = ST_OAM_AFFINE_OFF,
    .shape = SPRITE_SHAPE(32x32),
    .size = SPRITE_SIZE(32x32),
    .priority = 0
};

static const struct OamData sOamData_RadialStartMenuBackdrop = {
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .shape = SPRITE_SHAPE(64x64),
    .size = SPRITE_SIZE(64x64),
    .priority = 1
};

static const union AnimCmd sAnim_RadialStartMenuStatic[] = {
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd *const sAnims_RadialStartMenuStatic[] = {
    sAnim_RadialStartMenuStatic
};

#define RADIAL_START_MENU_ICON_FRAME(icon, state) ((icon) * START_MENU_ICON_FRAMES_PER_ENTRY * START_MENU_ICON_FRAME_TILES + (state) * START_MENU_ICON_FRAME_TILES)
#define RADIAL_START_MENU_ICON_ANIM(name, icon, state) \
static const union AnimCmd name[] = { \
    ANIMCMD_FRAME(RADIAL_START_MENU_ICON_FRAME(icon, state), 0), \
    ANIMCMD_END \
}

RADIAL_START_MENU_ICON_ANIM(sAnim_RadialStartMenuPokedexIcon, RADIAL_START_MENU_ICON_POKEDEX, 0);
RADIAL_START_MENU_ICON_ANIM(sAnim_RadialStartMenuPokedexIconPressed, RADIAL_START_MENU_ICON_POKEDEX, 1);
RADIAL_START_MENU_ICON_ANIM(sAnim_RadialStartMenuPokemonIcon, RADIAL_START_MENU_ICON_POKEMON, 0);
RADIAL_START_MENU_ICON_ANIM(sAnim_RadialStartMenuPokemonIconPressed, RADIAL_START_MENU_ICON_POKEMON, 1);
RADIAL_START_MENU_ICON_ANIM(sAnim_RadialStartMenuBagIcon, RADIAL_START_MENU_ICON_BAG, 0);
RADIAL_START_MENU_ICON_ANIM(sAnim_RadialStartMenuBagIconPressed, RADIAL_START_MENU_ICON_BAG, 1);
RADIAL_START_MENU_ICON_ANIM(sAnim_RadialStartMenuLogbookIcon, RADIAL_START_MENU_ICON_LOGBOOK, 0);
RADIAL_START_MENU_ICON_ANIM(sAnim_RadialStartMenuLogbookIconPressed, RADIAL_START_MENU_ICON_LOGBOOK, 1);
RADIAL_START_MENU_ICON_ANIM(sAnim_RadialStartMenuCardIcon, RADIAL_START_MENU_ICON_CARD, 0);
RADIAL_START_MENU_ICON_ANIM(sAnim_RadialStartMenuCardIconPressed, RADIAL_START_MENU_ICON_CARD, 1);
RADIAL_START_MENU_ICON_ANIM(sAnim_RadialStartMenuHelpIcon, RADIAL_START_MENU_ICON_HELP, 0);
RADIAL_START_MENU_ICON_ANIM(sAnim_RadialStartMenuHelpIconPressed, RADIAL_START_MENU_ICON_HELP, 1);
RADIAL_START_MENU_ICON_ANIM(sAnim_RadialStartMenuMapIcon, RADIAL_START_MENU_ICON_MAP, 0);
RADIAL_START_MENU_ICON_ANIM(sAnim_RadialStartMenuMapIconPressed, RADIAL_START_MENU_ICON_MAP, 1);
RADIAL_START_MENU_ICON_ANIM(sAnim_RadialStartMenuSettingsIcon, RADIAL_START_MENU_ICON_SETTINGS, 0);
RADIAL_START_MENU_ICON_ANIM(sAnim_RadialStartMenuSettingsIconPressed, RADIAL_START_MENU_ICON_SETTINGS, 1);

static const union AnimCmd *const sAnims_RadialStartMenuIcon[] = {
    sAnim_RadialStartMenuPokedexIcon,
    sAnim_RadialStartMenuPokedexIconPressed,
    sAnim_RadialStartMenuPokemonIcon,
    sAnim_RadialStartMenuPokemonIconPressed,
    sAnim_RadialStartMenuBagIcon,
    sAnim_RadialStartMenuBagIconPressed,
    sAnim_RadialStartMenuLogbookIcon,
    sAnim_RadialStartMenuLogbookIconPressed,
    sAnim_RadialStartMenuCardIcon,
    sAnim_RadialStartMenuCardIconPressed,
    sAnim_RadialStartMenuHelpIcon,
    sAnim_RadialStartMenuHelpIconPressed,
    sAnim_RadialStartMenuMapIcon,
    sAnim_RadialStartMenuMapIconPressed,
    sAnim_RadialStartMenuSettingsIcon,
    sAnim_RadialStartMenuSettingsIconPressed
};

#undef RADIAL_START_MENU_ICON_ANIM
#undef RADIAL_START_MENU_ICON_FRAME

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

static const union AffineAnimCmd sAffineAnim_RadialStartMenuBackdropOpen[] = {
    AFFINEANIMCMD_FRAME(0x10, 0x10, 0, 0),
    AFFINEANIMCMD_FRAME(0x50, 0x50, 0, 3),
    AFFINEANIMCMD_FRAME(0x100, 0x100, 0, 0),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd *const sAffineAnims_RadialStartMenuBackdrop[] = {
    sAffineAnim_RadialStartMenuBackdropOpen
};

static const struct CompressedSpriteSheet sSpriteSheet_RadialStartMenuBackdrop = {
    sRadialStartMenuBackdropGfx, 64 * 64 / 2, TAG_START_MENU_BACKDROP
};

static const struct SpriteTemplate sSpriteTemplate_RadialStartMenuBackdrop = {
    .tileTag = TAG_START_MENU_BACKDROP,
    .paletteTag = TAG_NONE,
    .oam = &sOamData_RadialStartMenuBackdrop,
    .anims = sAnims_RadialStartMenuStatic,
    .images = NULL,
    .affineAnims = sAffineAnims_RadialStartMenuBackdrop,
    .callback = SpriteCB_RadialStartMenuBackdrop
};

static const struct CompressedSpriteSheet sSpriteSheet_RadialStartMenuIcons = {
    sRadialStartMenuIconsGfx, 32 * 32 * RADIAL_START_MENU_ICON_COUNT * START_MENU_ICON_FRAMES_PER_ENTRY / 2, TAG_START_MENU_ICONS
};

static const struct SpriteTemplate sSpriteTemplate_RadialStartMenuIcon = {
    .tileTag = TAG_START_MENU_ICONS,
    .paletteTag = TAG_NONE,
    .oam = &sOamData_RadialStartMenuIcon,
    .anims = sAnims_RadialStartMenuIcon,
    .images = NULL,
    .affineAnims = sAffineAnims_RadialStartMenuIcon,
    .callback = SpriteCB_RadialStartMenuIcon
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
    AppendToStartMenuItems(STARTMENU_MAP);
    AppendToStartMenuItems(STARTMENU_BAG);
    if (FlagGet(FLAG_SYS_LOGBOOK_MENU_GET))
        AppendToStartMenuItems(STARTMENU_QUEST);
    AppendToStartMenuItems(STARTMENU_PLAYER);
    AppendToStartMenuItems(STARTMENU_SAVE);
    AppendToStartMenuItems(STARTMENU_SETTINGS);
    AppendToStartMenuItems(STARTMENU_OPTION);
}

static void SetUpStartMenu_SafariZone(void)
{
    AppendToStartMenuItems(STARTMENU_RETIRE);
    AppendToStartMenuItems(STARTMENU_POKEDEX);
    AppendToStartMenuItems(STARTMENU_POKEMON);
    AppendToStartMenuItems(STARTMENU_BAG);
    AppendToStartMenuItems(STARTMENU_PLAYER);
    AppendToStartMenuItems(STARTMENU_SETTINGS);
    AppendToStartMenuItems(STARTMENU_OPTION);
}

static void SetUpStartMenu_Link(void)
{
    AppendToStartMenuItems(STARTMENU_POKEMON);
    AppendToStartMenuItems(STARTMENU_BAG);
    AppendToStartMenuItems(STARTMENU_PLAYER2);
    AppendToStartMenuItems(STARTMENU_SETTINGS);
    AppendToStartMenuItems(STARTMENU_OPTION);
}

static void SetUpStartMenu_UnionRoom(void)
{
    AppendToStartMenuItems(STARTMENU_POKEMON);
    AppendToStartMenuItems(STARTMENU_BAG);
    AppendToStartMenuItems(STARTMENU_PLAYER);
    AppendToStartMenuItems(STARTMENU_SETTINGS);
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
        return 7;
    case STARTMENU_BAG:
        return 1;
    case STARTMENU_PLAYER:
    case STARTMENU_PLAYER2:
        return 5;
    case STARTMENU_MAP:
        return 6;
    case STARTMENU_SETTINGS:
        return 2;
    case STARTMENU_OPTION:
        return 4;
    case STARTMENU_QUEST:
        return 3;
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
        if (sStartMenuOrder[i] == STARTMENU_SAVE)
            continue;
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
        if (IsSelectableRadialStartMenuSlot(i))
            return i;
    }
    return 0;
}

static void SelectInitialRadialStartMenuSlot(void)
{
    u8 i;

    if (sStartMenuReturnItem != 0xFF)
    {
        for (i = 0; i < NELEMS(sRadialStartMenuSlotToItem); i++)
        {
            if (sRadialStartMenuSlotToItem[i] == sStartMenuReturnItem && IsSelectableRadialStartMenuSlot(i))
            {
                sRadialStartMenuCursorSlot = i;
                sStartMenuCursorPos = sRadialStartMenuSlotToOrderIndex[i];
                sStartMenuReturnItem = 0xFF;
                return;
            }
        }
        sStartMenuReturnItem = 0xFF;
    }

    for (i = 0; i < NELEMS(sRadialStartMenuSlotToOrderIndex); i++)
    {
        if (sRadialStartMenuSlotToOrderIndex[i] == sStartMenuCursorPos && IsSelectableRadialStartMenuSlot(i))
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
    u8 fontId;
    s32 windowWidth;
    s32 width;
    s32 x;

    if (windowId == WINDOW_NONE)
        return;

    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
    DrawStdWindowFrame(windowId, FALSE);

    colors = (slot == sRadialStartMenuCursorSlot && IsSelectableRadialStartMenuSlot(slot)) ? sTextColor_RadialMenuSelected : sTextColor_RadialMenuNormal;
    StringExpandPlaceholders(gStringVar4, sStartMenuActionTable[menuItem].text);
    text = gStringVar4;

    fontId = (menuItem == STARTMENU_SAVE || menuItem == STARTMENU_SETTINGS) ? FONT_SMALL : FONT_NORMAL;
    windowWidth = GetWindowAttribute(windowId, WINDOW_WIDTH) * 8;
    width = GetStringWidth(fontId, text, 0);
    x = (windowWidth - width) / 2;
    if (x < 0)
        x = 0;
    AddTextPrinterParameterized3(windowId, fontId, x, 6, colors, 0xFF, text);
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
    PrintStartMenuCenterLabel();
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
    if (sRadialStartMenuSlotToItem[slot] == STARTMENU_EXIT)
        return 30 - GetRadialStartMenuWindowWidth(slot);
    return GetClampedRadialStartMenuTileCoord(DISPLAY_WIDTH / 2 + sRadialStartMenuWindowXOffsets[slot], 20);
}

static u8 GetRadialStartMenuWindowTop(u8 slot)
{
    if (sRadialStartMenuSlotToItem[slot] == STARTMENU_EXIT)
        return 0;
    return GetClampedRadialStartMenuTileCoord(DISPLAY_HEIGHT / 2 + START_MENU_RADIAL_Y_OFFSET + sRadialStartMenuWindowYOffsets[slot], 17);
}

static u8 GetRadialStartMenuWindowWidth(u8 slot)
{
    return 10;
}

static s16 GetRadialStartMenuSpriteX(u8 slot)
{
    s16 x = DISPLAY_WIDTH / 2 + sRadialStartMenuWindowXOffsets[slot] + 40;

    if (sRadialStartMenuSlotToItem[slot] == STARTMENU_MAP)
        x -= 10;
    else if (sRadialStartMenuSlotToItem[slot] == STARTMENU_SETTINGS)
        x += 10;

    return x;
}

static s16 GetRadialStartMenuSpriteY(u8 slot)
{
    u8 menuItem = sRadialStartMenuSlotToItem[slot];
    s16 y = DISPLAY_HEIGHT / 2 + START_MENU_RADIAL_Y_OFFSET + sRadialStartMenuWindowYOffsets[slot] - 8;

    if (menuItem == STARTMENU_POKEDEX
     || menuItem == STARTMENU_POKEMON
     || menuItem == STARTMENU_BAG)
        y -= 7;
    else if (menuItem == STARTMENU_PLAYER2
          || menuItem == STARTMENU_PLAYER
          || menuItem == STARTMENU_OPTION
          || menuItem == STARTMENU_QUEST)
        y += 7;

    if (menuItem == STARTMENU_MAP || menuItem == STARTMENU_SETTINGS)
        y += 12;

    return y;
}

static void SetStartMenuReturnItem(u8 menuItem)
{
    sStartMenuReturnItem = menuItem;
}

static bool8 IsSelectableRadialStartMenuSlot(u8 slot)
{
    u8 menuItem = sRadialStartMenuSlotToItem[slot];

    return menuItem != 0xFF;
}

static void CreateRadialStartMenu(void)
{
    u8 i;

    ShowBg(0);
    PopulateRadialStartMenuSlots();
    SelectInitialRadialStartMenuSlot();

    for (i = 0; i < NELEMS(sRadialStartMenuWindowIds); i++)
    {
        if (sRadialStartMenuSlotToItem[i] == STARTMENU_EXIT)
        {
            struct WindowTemplate template = {
                .bg = 0,
                .tilemapLeft = GetRadialStartMenuWindowLeft(i),
                .tilemapTop = GetRadialStartMenuWindowTop(i),
                .width = GetRadialStartMenuWindowWidth(i),
                .height = 3,
                .paletteNum = 15,
                .baseBlock = 0x200 + (i * 0x20)
            };
            sRadialStartMenuWindowIds[i] = AddWindow(&template);
            PutWindowTilemap(sRadialStartMenuWindowIds[i]);
        }
    }
    CreateStartMenuHeaderWindow();
    CreateStartMenuCenterLabelWindow();
    DrawRadialStartMenu();
    CreateRadialStartMenuSprites();
}

static void DestroyRadialStartMenuWindows(bool8 copyToVram)
{
    u8 i;

    DestroyRadialStartMenuSprites();
    DestroyStartMenuHeaderWindow(copyToVram);
    DestroyStartMenuCenterLabelWindow(copyToVram);

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

static void CreateStartMenuHeaderWindow(void)
{
    struct WindowTemplate template = {
        .bg = 0,
        .tilemapLeft = START_MENU_HEADER_LEFT,
        .tilemapTop = START_MENU_HEADER_TOP,
        .width = START_MENU_HEADER_WIDTH,
        .height = START_MENU_HEADER_HEIGHT,
        .paletteNum = START_MENU_LABEL_PALETTE_NUM,
        .baseBlock = 0x280
    };

    sStartMenuHeaderWindowId = AddWindow(&template);
    SaveStartMenuBg0TilemapRect(sStartMenuHeaderBgBackup, template.tilemapLeft, template.tilemapTop, template.width, template.height);
    LoadPalette(GetTextWindowPalette(2), BG_PLTT_ID(START_MENU_LABEL_PALETTE_NUM), PLTT_SIZE_4BPP);
    PutWindowTilemap(sStartMenuHeaderWindowId);
    PrintStartMenuHeaderWindow();
}

static void DestroyStartMenuHeaderWindow(bool8 copyToVram)
{
    if (sStartMenuHeaderWindowId != WINDOW_NONE)
    {
        RestoreStartMenuBg0TilemapRect(sStartMenuHeaderBgBackup, START_MENU_HEADER_LEFT, START_MENU_HEADER_TOP, START_MENU_HEADER_WIDTH, START_MENU_HEADER_HEIGHT, copyToVram);
        RemoveWindow(sStartMenuHeaderWindowId);
        sStartMenuHeaderWindowId = WINDOW_NONE;
    }
}

static void PrintStartMenuHeaderWindow(void)
{
    DrawUiHintHeader(sStartMenuHeaderWindowId, sText_StartMenuHeaderHints, 15, 0, 0, FALSE);
}

static void CreateStartMenuCenterLabelWindow(void)
{
    struct WindowTemplate template = {
        .bg = 0,
        .tilemapLeft = START_MENU_CENTER_LABEL_WINDOW_LEFT,
        .tilemapTop = START_MENU_CENTER_LABEL_WINDOW_TOP,
        .width = START_MENU_CENTER_LABEL_WINDOW_WIDTH,
        .height = START_MENU_CENTER_LABEL_WINDOW_HEIGHT,
        .paletteNum = START_MENU_LABEL_PALETTE_NUM,
        .baseBlock = 0x260
    };

    sStartMenuCenterLabelWindowId = AddWindow(&template);
    SaveStartMenuBg0TilemapRect(sStartMenuCenterLabelBgBackup, template.tilemapLeft, template.tilemapTop, template.width, template.height);
    PutWindowTilemap(sStartMenuCenterLabelWindowId);
    CopyWindowToVram(sStartMenuCenterLabelWindowId, COPYWIN_MAP);
}

static void DestroyStartMenuCenterLabelWindow(bool8 copyToVram)
{
    if (sStartMenuCenterLabelWindowId != WINDOW_NONE)
    {
        RestoreStartMenuBg0TilemapRect(sStartMenuCenterLabelBgBackup,
                                       START_MENU_CENTER_LABEL_WINDOW_LEFT,
                                       START_MENU_CENTER_LABEL_WINDOW_TOP,
                                       START_MENU_CENTER_LABEL_WINDOW_WIDTH,
                                       START_MENU_CENTER_LABEL_WINDOW_HEIGHT,
                                       copyToVram);
        RemoveWindow(sStartMenuCenterLabelWindowId);
        sStartMenuCenterLabelWindowId = WINDOW_NONE;
    }
}

static void PrintStartMenuCenterLabel(void)
{
    const u8 *text;
    s32 windowWidth;
    s32 width;
    s32 x;

    if (sStartMenuCenterLabelWindowId == WINDOW_NONE)
        return;

    text = GetStartMenuCenterLabelText(sRadialStartMenuSlotToItem[sRadialStartMenuCursorSlot]);
    windowWidth = START_MENU_CENTER_LABEL_WIDTH;
    width = GetStringWidth(FONT_NORMAL, text, 0);
    x = START_MENU_CENTER_LABEL_INSET + (windowWidth - width) / 2;
    if (x < 0)
        x = START_MENU_CENTER_LABEL_INSET;

    FillWindowPixelBuffer(sStartMenuCenterLabelWindowId, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
    FillWindowPixelRect(sStartMenuCenterLabelWindowId,
                        PIXEL_FILL(START_MENU_LABEL_FILL_COLOR),
                        START_MENU_CENTER_LABEL_INSET,
                        START_MENU_CENTER_LABEL_TOP,
                        START_MENU_CENTER_LABEL_WIDTH,
                        START_MENU_CENTER_LABEL_HEIGHT);
    AddTextPrinterParameterized3(sStartMenuCenterLabelWindowId, FONT_NORMAL, x, 1, sTextColor_RadialMenuSelected, 0xFF, text);
    CopyWindowToVram(sStartMenuCenterLabelWindowId, COPYWIN_GFX);
}

static const u8 *GetStartMenuCenterLabelText(u8 menuItem)
{
    if (menuItem == STARTMENU_PLAYER || menuItem == STARTMENU_PLAYER2)
        return sText_MenuCard;
    if (menuItem == STARTMENU_SETTINGS)
        return gText_MenuSettings;
    return sStartMenuActionTable[menuItem].text;
}

static bool8 StartMenuHasItem(u8 menuItem)
{
    u8 i;

    for (i = 0; i < sNumStartMenuItems; i++)
    {
        if (sStartMenuOrder[i] == menuItem)
            return TRUE;
    }
    return FALSE;
}

static void SaveStartMenuBg0TilemapRect(u16 *dest, u8 left, u8 top, u8 width, u8 height)
{
    u8 i;
    u8 j;
    const u16 *src = GetBgTilemapBuffer(0);

    if (src == NULL)
        return;

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
            dest[i * width + j] = src[(top + i) * 32 + left + j];
    }
}

static void RestoreStartMenuBg0TilemapRect(const u16 *src, u8 left, u8 top, u8 width, u8 height, bool8 copyToVram)
{
    u8 i;
    u8 j;
    u16 *dest = GetBgTilemapBuffer(0);

    if (dest == NULL)
        return;

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
            dest[(top + i) * 32 + left + j] = src[i * width + j];
    }

    if (copyToVram == TRUE)
        CopyBgTilemapBufferToVram(0);
}

static void BackupRadialStartMenuObjPalettes(void)
{
    CpuCopy16(&gPlttBufferUnfaded[OBJ_PLTT_ID(START_MENU_OBJ_PAL_BACKUP_START)],
              sRadialStartMenuObjPalBackupUnfaded,
              sizeof(sRadialStartMenuObjPalBackupUnfaded));
    CpuCopy16(&gPlttBufferFaded[OBJ_PLTT_ID(START_MENU_OBJ_PAL_BACKUP_START)],
              sRadialStartMenuObjPalBackupFaded,
              sizeof(sRadialStartMenuObjPalBackupFaded));
    sRadialStartMenuObjPalBackupValid = TRUE;
}

static void LoadRadialStartMenuObjPalettes(void)
{
    LZ77UnCompWram(sRadialStartMenuSettingsIconPal, gDecompressionBuffer);
    LoadPalette(gDecompressionBuffer, OBJ_PLTT_ID(START_MENU_ICON_OBJ_PAL_SLOT), PLTT_SIZE_4BPP);
}

static void RestoreRadialStartMenuObjPalettes(void)
{
    if (!sRadialStartMenuObjPalBackupValid)
        return;

    CpuCopy16(sRadialStartMenuObjPalBackupUnfaded,
              &gPlttBufferUnfaded[OBJ_PLTT_ID(START_MENU_OBJ_PAL_BACKUP_START)],
              sizeof(sRadialStartMenuObjPalBackupUnfaded));
    CpuCopy16(sRadialStartMenuObjPalBackupFaded,
              &gPlttBufferFaded[OBJ_PLTT_ID(START_MENU_OBJ_PAL_BACKUP_START)],
              sizeof(sRadialStartMenuObjPalBackupFaded));
    sRadialStartMenuObjPalBackupValid = FALSE;
}

static void CreateRadialStartMenuSprites(void)
{
    u8 i;

    if (sRadialStartMenuSpritesLoaded)
        return;

    BackupRadialStartMenuObjPalettes();
    LoadRadialStartMenuObjPalettes();

    LoadCompressedSpriteSheet(&sSpriteSheet_RadialStartMenuBackdrop);
    LoadCompressedSpriteSheet(&sSpriteSheet_RadialStartMenuIcons);

    sRadialStartMenuSpritesLoaded = TRUE;
    sRadialStartMenuIconsVisible = FALSE;
    sRadialStartMenuBackdropSpriteId = CreateSprite(&sSpriteTemplate_RadialStartMenuBackdrop, DISPLAY_WIDTH / 2, START_MENU_BACKDROP_START_Y, 4);
    if (sRadialStartMenuBackdropSpriteId != MAX_SPRITES)
    {
        gSprites[sRadialStartMenuBackdropSpriteId].oam.paletteNum = START_MENU_ICON_OBJ_PAL_SLOT;
        gSprites[sRadialStartMenuBackdropSpriteId].data[0] = 0;
    }

    for (i = 0; i < NELEMS(sRadialStartMenuSpriteIds); i++)
    {
        u8 iconIndex = GetRadialStartMenuIconIndex(sRadialStartMenuSlotToItem[i]);

        if (iconIndex != RADIAL_START_MENU_ICON_NONE)
        {
            s16 x = GetRadialStartMenuSpriteX(i);
            s16 y = GetRadialStartMenuSpriteY(i);

            sRadialStartMenuSpriteIds[i] = CreateSprite(&sSpriteTemplate_RadialStartMenuIcon, x, y, 0);
            if (sRadialStartMenuSpriteIds[i] != MAX_SPRITES)
            {
                gSprites[sRadialStartMenuSpriteIds[i]].oam.paletteNum = START_MENU_ICON_OBJ_PAL_SLOT;
                gSprites[sRadialStartMenuSpriteIds[i]].data[0] = i;
                gSprites[sRadialStartMenuSpriteIds[i]].data[3] = iconIndex;
            }
        }
    }
    UpdateRadialStartMenuSpriteStates();
    SetRadialStartMenuIconsVisible(sRadialStartMenuBackdropSpriteId == MAX_SPRITES);
}

static void DestroyRadialStartMenuSprites(void)
{
    u8 i;

    if (!sRadialStartMenuSpritesLoaded)
        return;

    if (sRadialStartMenuBackdropSpriteId != MAX_SPRITES)
    {
        FreeSpriteOamMatrix(&gSprites[sRadialStartMenuBackdropSpriteId]);
        DestroySprite(&gSprites[sRadialStartMenuBackdropSpriteId]);
        sRadialStartMenuBackdropSpriteId = MAX_SPRITES;
    }

    for (i = 0; i < NELEMS(sRadialStartMenuSpriteIds); i++)
    {
        if (sRadialStartMenuSpriteIds[i] != MAX_SPRITES)
        {
            DestroySprite(&gSprites[sRadialStartMenuSpriteIds[i]]);
            sRadialStartMenuSpriteIds[i] = MAX_SPRITES;
        }
    }
    FreeSpriteTilesByTag(TAG_START_MENU_BACKDROP);
    FreeSpriteTilesByTag(TAG_START_MENU_ICONS);
    RestoreRadialStartMenuObjPalettes();
    sRadialStartMenuSpritesLoaded = FALSE;
    sRadialStartMenuIconsVisible = FALSE;
}

static void UpdateRadialStartMenuSpriteStates(void)
{
    u8 i;

    for (i = 0; i < NELEMS(sRadialStartMenuSpriteIds); i++)
    {
        if (sRadialStartMenuSpriteIds[i] != MAX_SPRITES)
        {
            u8 iconIndex = gSprites[sRadialStartMenuSpriteIds[i]].data[3];

            StartSpriteAnim(&gSprites[sRadialStartMenuSpriteIds[i]], iconIndex * START_MENU_ICON_FRAMES_PER_ENTRY + (i == sRadialStartMenuCursorSlot ? 1 : 0));
        }
    }
}

static void SetRadialStartMenuIconsVisible(bool8 visible)
{
    u8 i;

    sRadialStartMenuIconsVisible = visible;

    for (i = 0; i < NELEMS(sRadialStartMenuSpriteIds); i++)
    {
        if (sRadialStartMenuSpriteIds[i] != MAX_SPRITES)
            gSprites[sRadialStartMenuSpriteIds[i]].invisible = !visible;
    }
}

static u8 GetRadialStartMenuIconIndex(u8 item)
{
    switch (item)
    {
    case STARTMENU_POKEDEX:
        return RADIAL_START_MENU_ICON_POKEDEX;
    case STARTMENU_POKEMON:
        return RADIAL_START_MENU_ICON_POKEMON;
    case STARTMENU_BAG:
        return RADIAL_START_MENU_ICON_BAG;
    case STARTMENU_QUEST:
        return RADIAL_START_MENU_ICON_LOGBOOK;
    case STARTMENU_PLAYER:
    case STARTMENU_PLAYER2:
        return RADIAL_START_MENU_ICON_CARD;
    case STARTMENU_MAP:
        return RADIAL_START_MENU_ICON_MAP;
    case STARTMENU_OPTION:
        return RADIAL_START_MENU_ICON_HELP;
    case STARTMENU_SETTINGS:
        return RADIAL_START_MENU_ICON_SETTINGS;
    case STARTMENU_EXIT:
    case STARTMENU_SAVE:
    case 0xFF:
    default:
        return RADIAL_START_MENU_ICON_NONE;
    }
}

static void SpriteCB_RadialStartMenuBackdrop(struct Sprite *sprite)
{
    if (sprite->data[0] < START_MENU_BACKDROP_OPEN_FRAMES)
    {
        sprite->data[0]++;
        sprite->y = START_MENU_BACKDROP_START_Y
                  + ((START_MENU_BACKDROP_FINAL_Y - START_MENU_BACKDROP_START_Y) * sprite->data[0]) / START_MENU_BACKDROP_OPEN_FRAMES;
    }
    else
    {
        sprite->y = START_MENU_BACKDROP_FINAL_Y;
    }

    if (!sRadialStartMenuIconsVisible && sprite->affineAnimEnded)
        SetRadialStartMenuIconsVisible(TRUE);
}

static void SpriteCB_RadialStartMenuIcon(struct Sprite *sprite)
{
    if (sprite->data[0] != sRadialStartMenuCursorSlot)
    {
        sprite->y2 = 0;
        sprite->data[1] = 0;
        sprite->data[2] = 0;
        return;
    }

    if (sprite->data[2] == 0)
    {
        sprite->y2 = -1;
        sprite->data[2] = 1;
        return;
    }

    if (++sprite->data[1] >= START_MENU_SELECTED_ICON_BOB_INTERVAL)
    {
        sprite->data[1] = 0;
        sprite->y2 = -sprite->y2;
    }
}

static void RefreshStartMenuHelpText(void)
{
    DrawHelpMessageWindowWithText(sStartMenuDescPointers[sStartMenuOrder[sStartMenuCursorPos]]);
}

static void UpdateRadialStartMenuSelection(u8 newSlot)
{
    u8 oldSlot;

    if (newSlot == sRadialStartMenuCursorSlot || !IsSelectableRadialStartMenuSlot(newSlot))
        return;

    oldSlot = sRadialStartMenuCursorSlot;
    sRadialStartMenuCursorSlot = newSlot;
    sStartMenuCursorPos = sRadialStartMenuSlotToOrderIndex[newSlot];
    PrintRadialStartMenuItem(oldSlot);
    PrintRadialStartMenuItem(newSlot);
    PrintStartMenuCenterLabel();
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
        if (IsSelectableRadialStartMenuSlot(slot))
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
     || (currentItem == STARTMENU_POKEMON && dx < 0 && dy == 0)
     || (currentItem == STARTMENU_POKEMON && dx == 0 && dy < 0)
     || (currentItem == STARTMENU_MAP && dx < 0 && dy == 0)
     || (currentItem == STARTMENU_BAG && dx > 0 && dy == 0)
     || (currentItem == STARTMENU_BAG && dx == 0 && dy < 0)
     || (currentItem == STARTMENU_SETTINGS && dx > 0 && dy == 0)
     || ((currentItem == STARTMENU_PLAYER || currentItem == STARTMENU_PLAYER2) && dx < 0 && dy == 0)
     || ((currentItem == STARTMENU_PLAYER || currentItem == STARTMENU_PLAYER2) && dx == 0 && dy > 0)
     || (currentItem == STARTMENU_QUEST && dx > 0 && dy == 0)
     || (currentItem == STARTMENU_QUEST && dx == 0 && dy > 0))
        return sRadialStartMenuCursorSlot;

    if (currentItem == STARTMENU_POKEDEX)
    {
        if (dx < 0 && dy == 0)
            targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_POKEMON);
        else if (dx > 0 && dy == 0)
            targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_BAG);
        else if (dx == 0 && dy > 0)
            targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_OPTION);
        else
            targetSlot = 0xFF;

        if (targetSlot != 0xFF)
            return targetSlot;
    }
    if (currentItem == STARTMENU_OPTION)
    {
        if (dx < 0 && dy == 0)
        {
            targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_PLAYER);
            if (targetSlot == 0xFF)
                targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_PLAYER2);
        }
        else if (dx > 0 && dy == 0)
            targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_QUEST);
        else if (dx == 0 && dy < 0)
            targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_POKEDEX);
        else
            targetSlot = 0xFF;

        if (targetSlot != 0xFF)
            return targetSlot;
    }
    if (currentItem == STARTMENU_POKEMON)
    {
        if (dx > 0 && dy == 0)
            targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_POKEDEX);
        else if (dx == 0 && dy > 0)
        {
            targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_MAP);
            if (targetSlot == 0xFF)
            {
                targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_PLAYER);
                if (targetSlot == 0xFF)
                    targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_PLAYER2);
            }
        }
        else
            targetSlot = 0xFF;

        if (targetSlot != 0xFF)
            return targetSlot;
    }
    if (currentItem == STARTMENU_MAP)
    {
        if (dx == 0 && dy < 0)
            targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_POKEMON);
        else if (dx == 0 && dy > 0)
        {
            targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_PLAYER);
            if (targetSlot == 0xFF)
                targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_PLAYER2);
        }
        else if (dx > 0 && dy == 0)
            targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_SETTINGS);
        else
            targetSlot = 0xFF;

        if (targetSlot != 0xFF)
            return targetSlot;
    }
    if (currentItem == STARTMENU_BAG)
    {
        if (dx < 0 && dy == 0)
            targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_POKEDEX);
        else if (dx == 0 && dy > 0)
        {
            targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_SETTINGS);
            if (targetSlot == 0xFF)
                targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_QUEST);
        }
        else
            targetSlot = 0xFF;

        if (targetSlot != 0xFF)
            return targetSlot;
    }
    if (currentItem == STARTMENU_SETTINGS)
    {
        if (dx == 0 && dy < 0)
            targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_BAG);
        else if (dx == 0 && dy > 0)
            targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_QUEST);
        else if (dx < 0 && dy == 0)
            targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_MAP);
        else
            targetSlot = 0xFF;

        if (targetSlot != 0xFF)
            return targetSlot;
    }
    if (currentItem == STARTMENU_QUEST)
    {
        if (dx < 0 && dy == 0)
            targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_OPTION);
        else if (dx == 0 && dy < 0)
        {
            targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_SETTINGS);
            if (targetSlot == 0xFF)
                targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_BAG);
        }
        else
            targetSlot = 0xFF;

        if (targetSlot != 0xFF)
            return targetSlot;
    }
    if (currentItem == STARTMENU_PLAYER || currentItem == STARTMENU_PLAYER2)
    {
        if (dx == 0 && dy < 0)
        {
            targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_MAP);
            if (targetSlot == 0xFF)
                targetSlot = FindRadialStartMenuSlotByItem(STARTMENU_POKEMON);
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

        if (i == sRadialStartMenuCursorSlot || !IsSelectableRadialStartMenuSlot(i))
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
    if (JOY_NEW(L_BUTTON))
    {
        if (StartMenuHasItem(STARTMENU_SAVE))
        {
            PlaySE(SE_SELECT);
            sStartMenuCallback = sStartMenuActionTable[STARTMENU_SAVE].func.u8_void;
            StartMenu_FadeScreenIfLeavingOverworld();
            return FALSE;
        }
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

static bool8 StartMenuMapCallback(void)
{
    if (!gPaletteFade.active)
    {
        PlayRainStoppingSoundEffect();
        DestroySafariZoneStatsWindow();
        DestroyRadialStartMenuWindows(FALSE);
        CleanupOverworldWindowsAndTilemaps();
        SetStartMenuReturnItem(STARTMENU_MAP);
        InitRegionMapWithExitCB(REGIONMAP_TYPE_NORMAL, CB2_ReturnToFieldWithOpenMenu);
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

static bool8 StartMenuSettingsCallback(void)
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

static bool8 StartMenuOptionCallback(void)
{
    if (!gPaletteFade.active)
    {
        PlayRainStoppingSoundEffect();
        DestroySafariZoneStatsWindow();
        DestroyRadialStartMenuWindows(FALSE);
        CleanupOverworldWindowsAndTilemaps();
        SetStartMenuReturnItem(STARTMENU_OPTION);
        InitHelpMenuController(0, CB2_ReturnToFieldWithOpenMenu);
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
        return TRUE;
    case SAVECB_RETURN_CANCEL:
        ClearDialogWindowAndFrameToTransparent(0, FALSE);
        DrawStartMenuInOneGo();
        sStartMenuCallback = StartCB_HandleInput;
        break;
    case SAVECB_RETURN_ERROR:
        ClearDialogWindowAndFrameToTransparent(0, TRUE);
        ClearPlayerHeldMovementAndUnfreezeObjectEvents();
        UnlockPlayerFieldControls();
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
        if (gDifferentSaveFile == TRUE)
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
    if (sSaveStatsWindowId != WINDOW_NONE)
    {
        ClearStdWindowAndFrame(sSaveStatsWindowId, FALSE);
        RemoveWindow(sSaveStatsWindowId);
        sSaveStatsWindowId = WINDOW_NONE;
    }
}

static void CloseStartMenu(void)
{
    PlaySE(SE_SELECT);
    DestroyRadialStartMenuWindows(TRUE);
    EraseFieldMessageBox(TRUE);
    HideBg(0);
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
        LogbookMenu_Init(0, CB2_ReturnToFieldWithOpenMenu);
        return TRUE;
    }
    return FALSE;
}
