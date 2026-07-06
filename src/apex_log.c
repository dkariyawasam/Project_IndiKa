#include "global.h"
#include "gflib.h"
#include "constants/songs.h"
#include "event_data.h"
#include "event_scripts.h"
#include "graphics.h"
#include "battle.h"
#include "battle_setup.h"
#include "menu.h"
#include "trainer_pokemon_sprites.h"
#include "scanline_effect.h"
#include "new_menu_helpers.h"
#include "item_menu.h"
#include "list_menu.h"
#include "task.h"
#include "event_object_movement.h"
#include "menu_indicators.h"
#include "text_window.h"
#include "apex_log.h"
#include "quests.h"
#include "strings.h"
#include "constants/event_objects.h"
#include "constants/flags.h"
#include "constants/quests.h"

#define SPRITETAG_SELECTOR_CURSOR 1000
#define SPRITETAG_QUESTION_MARK 1001
#define SPRITETAG_SPINNING_POKEBALL 1002
#define SPRITETAG_SCROLL_INDICATORS 1004
#define SPRITETAG_DAISY 1006 // TODO: Investigate, seems to be used for other NPCs (e.g. Fan Club Chairman)
#define SPRITETAG_FUJI 1007
#define SPRITETAG_OAK 1008
#define SPRITETAG_BILL 1009
#define SPRITETAG_APEX_SILHOUETTE 1010

#define ApexLog_NONTRAINER_START 0xFE00
#define ApexLog_ICON_X_LEFT 0x72
#define ApexLog_ICON_Y_TOP 0x2F
#define ApexLog_ICON_X_SPACING 47
#define ApexLog_ICON_Y_SPACING 27
#define ApexLog_APEX_DOSSIER_ICON_X_LEFT 73
#define ApexLog_APEX_DOSSIER_ICON_Y_TOP ApexLog_ICON_Y_TOP
#define ApexLog_APEX_DOSSIER_ICONDESC_LEFT 10
#define ApexLog_APEX_DOSSIER_INFOBOX_LEFT 9

struct ApexLogData
{
    MainCallback savedCallback;
    u16 listMenuTopIdx;
    u8 scrollIndicatorPairTaskId;
    u8 personHasUnlockedPanels:1;
    u8 inPickMode:1;
    u8 numUnlockedPersons:6;
    u8 listMenuTaskId;
    u8 listMenuCurIdx;
    u8 listMenuTopIdx2;
    u8 listMenuDrawnSelIdx;
    u8 unlockedPersons[NUM_APEX_LOG_PERSONS + 1];
    u8 spriteIds[6];
    u8 viewingFlavorText:1;
    u8 unk_23_1:1; // unused
    u8 pickModeOverCancel:1;
    u8 isApexDossier:1;
    u8 apexSubquest:4;
};

struct ApexRumorDossierEntry
{
    const u8 *name;
    const u8 *rumors[3];
    const u8 *locations[3];
    const u8 *sources[3];
    u16 graphicsIds[3];
    u16 apexGraphicsId;
    u16 apexInteractedFlag;
};

static EWRAM_DATA u16 * sBg3TilemapBuffer = NULL;
static EWRAM_DATA u16 * sBg1TilemapBuffer = NULL;
static EWRAM_DATA u16 * sBg2TilemapBuffer = NULL;
static EWRAM_DATA struct ApexLogData * sApexLogData = NULL;
static EWRAM_DATA struct ListMenuItem * sListMenuItems = NULL;
static EWRAM_DATA s32 sLastMenuIdx = 0;

COMMON_DATA struct ListMenuTemplate gApexLog_ListMenuTemplate = {0};
COMMON_DATA u8 gIconDescriptionBoxIsOpen = 0;

static void MainCB2_LoadApexLog(void);
static void LoadUISpriteSheetsAndPalettes(void);
static void Task_WaitFadeOnInit(u8 taskId);
static void Task_TopMenuHandleInput(u8 taskId);
static bool8 TryExitPickMode(u8 taskId);
static void MessageBoxPrintEmptyText(void);
static void Task_EnterPickMode(u8 taskId);
static void Task_ExitPickMode(u8 taskId);
static void Task_FlavorTextDisplayHandleInput(u8 taskId);
static void ApexLog_EnterFlavorTextSelection(u8 taskId, u8 initialSlot, bool8 playSound);
static void ApexLog_MoveSelectorCursor(u8 taskId, s8 dx, s8 dy);
static void GetPickModeText(void);
static void PrintSelectedNameInBrightGreen(u8 taskId);
static void WipeMsgBoxAndTransfer(void);
static void Setup_DrawMsgAndListBoxes(void);
static void ApexLog_PutWindowTilemapAndCopyWindowToVramMode3(u8 windowId);
static bool8 SetMessageSelectorIconObjMode(u8 taskId, u8 objMode);
static void Task_StartToCloseApexLog(u8 taskId);
static void Task_DestroyAssetsAndCloseApexLog(u8 taskId);
static void ApexLog_DestroyWindow(u8 windowId);
static void PrintUIHelp(u8 state);
static bool8 CreateAllFlavorTextIcons(u8 who);
static void FCSetup_ClearVideoRegisters(void);
static void FCSetup_ResetTasksAndSpriteResources(void);
static void FCSetup_TurnOnDisplay(void);
static void FCSetup_ResetBGCoords(void);
static bool8 HasUnlockedAllFlavorTextsForCurrentPerson(void);
static void FreeSelectionCursorSpriteResources(void);
static u8 CreateFlavorTextIconSelectorCursorSprite(s16 where);
static s16 ApexLog_GetFlavorTextIconX(u8 slot);
static s16 ApexLog_GetFlavorTextIconY(u8 slot);
static void SpriteCB_DestroyFlavorTextIconSelectorCursor(struct Sprite *sprite);
static void FreeQuestionMarkSpriteResources(void);
static u8 PlaceQuestionMarkTile(u8 x, u8 y);
static void FreeSpinningPokeballSpriteResources(void);
static u8 CreateSpinningPokeballSprite(void);
static void SpriteCB_DestroySpinningPokeball(struct Sprite *sprite);
static void FreeNonTrainerPicTiles(void);
static u8 CreatePersonPicSprite(u8 fcPersonIdx);
static void DestroyPersonPicSprite(u8 taskId, u16 who);
static void UpdateIconDescriptionBox(u8 whichText);
static void UpdateIconDescriptionBoxOff(void);
static void ApexLog_CreateListMenu(void);
static void SpriteCB_FCSpinningPokeball(struct Sprite *sprite);
static void InitListMenuTemplate(void);
static void ApexLog_MoveCursorFunc(s32 itemIndex, bool8 onInit, struct ListMenu * list);
static void Task_SwitchToPickMode(u8 taskId);
static void PrintCancelDescription(void);
static void ApexLog_DoMoveCursor(s32 itemIndex, bool8 onInit);
static u8 ApexLog_PopulateListMenu(void);
static bool8 ApexLog_HasApexDossierRumor(u8 rumor);
static bool8 ApexLog_HasEncounteredApexDossierMon(void);
static bool8 ApexLog_IsApexDossierMonSlot(u8 slot);
static bool8 ApexLog_IsApexDossierWitnessSlot(u8 slot);
static u8 ApexLog_GetApexDossierRumorForSlot(u8 slot);
static void SetApexDossierSilhouettePalette(u8 spriteId);
static void ApexLog_MoveApexDossierCursor(u8 taskId, u8 newSlot);
static void ApexLog_PutWindowTilemapAndCopyWindowToVramMode3_2(u8 windowId);
static void ApexLog_CreateScrollIndicatorArrowPair(void);
static void FreeListMenuSelectorArrowPairResources(void);
static u16 ApexLogGetCursorY(void);
static void HandleFlavorTextModeSwitch(bool8 state);
static void Task_FCOpenOrCloseInfoBox(u8 taskId);
static void UpdateInfoBoxTilemap(u8 bg, s16 state);
static void PlaceListMenuCursor(bool8 isActive);

static const u16 sApexLogTilemap[] = INCBIN_U16("graphics/apex_log/tilemap1.bin");
static const u8 sQuestionMarkSpriteGfx[] = INCBIN_U8("graphics/apex_log/question_mark.4bpp");
static const u8 sSpinningPokeballSpriteGfx[] = INCBIN_U8("graphics/apex_log/spinning_pokeball.4bpp");
static const u16 sSpinningPokeballSpritePalette[] = INCBIN_U16("graphics/apex_log/spinning_pokeball.gbapal");
static const u8 sSelectorCursorSpriteGfx[] = INCBIN_U8("graphics/apex_log/cursor.4bpp");
static const u16 sSelectorCursorSpritePalette[] = INCBIN_U16("graphics/apex_log/cursor.gbapal");
static const u8 sFujiSpriteGfx[] = INCBIN_U8("graphics/apex_log/mr_fuji.4bpp");
static const u16 sFujiSpritePalette[] = INCBIN_U16("graphics/apex_log/mr_fuji.gbapal");
static const u8 sBillSpriteGfx[] = INCBIN_U8("graphics/apex_log/bill.4bpp");
static const u16 sBillSpritePalette[] = INCBIN_U16("graphics/apex_log/bill.gbapal");
static const u8 sDaisySpriteGfx[] = INCBIN_U8("graphics/apex_log/daisy.4bpp");
static const u16 sDaisySpritePalette[] = INCBIN_U16("graphics/apex_log/daisy.gbapal");
static const u8 sOakSpriteGfx[] = INCBIN_U8("graphics/apex_log/prof_oak.4bpp");
static const u16 sOakSpritePalette[] = INCBIN_U16("graphics/apex_log/prof_oak.gbapal");
static const u16 sUnkPalette[] = INCBIN_U16("graphics/apex_log/unk.gbapal"); // unused?
static const u16 sSilhouettePalette[] = INCBIN_U16("graphics/apex_log/silhouette.gbapal");

static const u8 sTextColor_White[3]  = {0, 1, 2};
static const u8 sTextColor_DkGrey[3] = {0, 2, 3};
static const u8 sTextColor_Green[3]  = {0, 6, 7};

static const u8 sApexDossierName_Tangrowth[] = _("TANGROWTH");
static const u8 sApexDossierName_Zapdos[] = _("ZAPDOS");
static const u8 sApexDossierName_Articuno[] = _("ARTICUNO");
static const u8 sApexDossierName_Mewtwo[] = _("MEWTWO");
static const u8 sApexDossierName_Osscythe[] = _("OSSCYTHE");
static const u8 sApexDossierName_Moltres[] = _("MOLTRES");
static const u8 sApexDossierName_MimeSr[] = _("MIME SR.");
static const u8 sApexDossierName_Annihilape[] = _("ANNIHILAPE");

static const u8 sApexRumorUnknown[] = _("No rumour recorded yet.");
static const u8 sApexMonUnknown[] = _("The APEX POKEMON has not been\nencountered yet.");
static const u8 sApexMonRecorded[] = _("The APEX POKEMON has been\nrecorded.");
static const u8 sApexDossierText_UI[] = _("{DPAD_ANY}PICK {B_BUTTON}CANCEL");
static const u8 sApexRumorSourceUnknown[] = _("Unrecorded");
static const u8 sApexRumorLocationUnknown[] = _("????");
static const u8 sApexLoc_Apex[] = _("APEX");
static const u8 sApexSrc_FieldRecord[] = _("Field record");
static const u8 sApexLoc_Viridian[] = _("VIRIDIAN");
static const u8 sApexLoc_ForestGate[] = _("FOREST GATE");
static const u8 sApexLoc_ViridianForest[] = _("VIRIDIAN FOREST");
static const u8 sApexLoc_PowerPlant[] = _("POWER PLANT");
static const u8 sApexLoc_Route10[] = _("ROUTE 10");
static const u8 sApexLoc_Route20[] = _("ROUTE 20");
static const u8 sApexLoc_Seafoam[] = _("SEAFOAM");
static const u8 sApexLoc_Unknown[] = _("UNKNOWN");
static const u8 sApexLoc_Saffron[] = _("SAFFRON");
static const u8 sApexLoc_Lavender[] = _("LAVENDER");
static const u8 sApexLoc_PokemonTower[] = _("POKEMON TOWER");
static const u8 sApexLoc_Cinnabar[] = _("CINNABAR");
static const u8 sApexLoc_Volcano[] = _("VOLCANO");
static const u8 sApexLoc_DiglettCave[] = _("DIGLETT CAVE");
static const u8 sApexLoc_Route11[] = _("ROUTE 11");
static const u8 sApexLoc_MtMoon[] = _("MT. MOON");
static const u8 sApexLoc_Cerulean[] = _("CERULEAN");
static const u8 sApexLoc_Route4[] = _("ROUTE 4");

static const u8 sApexSrc_OldTale[] = _("Old tale");
static const u8 sApexSrc_ScaredLocal[] = _("Scared local");
static const u8 sApexSrc_FlowerWatcher[] = _("Flower watcher");
static const u8 sApexSrc_Engineer[] = _("Engineer");
static const u8 sApexSrc_Rocker[] = _("Rocker");
static const u8 sApexSrc_Picnicker[] = _("Picnicker");
static const u8 sApexSrc_Swimmer[] = _("Swimmer");
static const u8 sApexSrc_Trainer[] = _("Trainer");
static const u8 sApexSrc_Explorer[] = _("Explorer");
static const u8 sApexSrc_NoWitness[] = _("No witness");
static const u8 sApexSrc_Rocket[] = _("Rocket");
static const u8 sApexSrc_TownLocal[] = _("Town local");
static const u8 sApexSrc_Mourner[] = _("Mourner");
static const u8 sApexSrc_Scientist[] = _("Scientist");
static const u8 sApexSrc_Hiker[] = _("Hiker");
static const u8 sApexSrc_Witness[] = _("Witness");
static const u8 sApexSrc_CaveVisitor[] = _("Cave visitor");
static const u8 sApexSrc_Pokemaniac[] = _("Pokemaniac");
static const u8 sApexSrc_HouseResident[] = _("House resident");

static const u8 sApexRumor_Tangrowth0[] = _("My father once heard the growls\nof a shadowy figure deep in\nVIRIDIAN FOREST.");
static const u8 sApexRumor_Tangrowth1[] = _("Legends say VIRIDIAN FOREST\ncomes alive and ensnares the\nwicked.");
static const u8 sApexRumor_Tangrowth2[] = _("The flowers grow more vibrant\nnear the centre of VIRIDIAN\nFOREST.");
static const u8 sApexRumor_Zapdos0[] = _("They say power gathers at the\nPLANT without training, without\nlimit.");
static const u8 sApexRumor_Zapdos1[] = _("The air near the POWER PLANT\ncrackles like a storm that never\nruns dry.");
static const u8 sApexRumor_Zapdos2[] = _("A flash crossed the sky, and the\nwhole route shook with thunder.");
static const u8 sApexRumor_Articuno0[] = _("SEAFOAM changed all at once.\nA volcanic place became cold as\nice.");
static const u8 sApexRumor_Articuno1[] = _("I heard SEAFOAM fractured when\nthe spirit of ice settled in the\ncaves.");
static const u8 sApexRumor_Articuno2[] = _("This whole area used to be lava.\nThen everything changed in a\nsingle burst.");
static const u8 sApexRumor_Mewtwo0[] = _("No witness statement has been\nrecorded.");
static const u8 sApexRumor_Mewtwo1[] = _("No witness statement has been\nrecorded.");
static const u8 sApexRumor_Mewtwo2[] = _("No witness statement has been\nrecorded.");
static const u8 sApexRumor_Osscythe0[] = _("A ROCKET took a CUBONE from\nits mother. The poor thing only\ncries.");
static const u8 sApexRumor_Osscythe1[] = _("Atop POKEMON TOWER, a vengeful\nspirit howls over a tragic loss.");
static const u8 sApexRumor_Osscythe2[] = _("Families come here to mourn.\nSome losses refuse to stay quiet.");
static const u8 sApexRumor_Moltres0[] = _("CINNABAR keeps rebuilding.\nSomething beneath the volcano\nkeeps giving it life.");
static const u8 sApexRumor_Moltres1[] = _("When old rock breaks, new land\nis born. That is the volcano's\npower.");
static const u8 sApexRumor_Moltres2[] = _("A firebird's cry was heard where\nthe volcano breathes hottest.");
static const u8 sApexRumor_MimeSr0[] = _("MR. MIME have taken home in\nDIGLETT CAVE. It leads all the\nway to PEWTER.");
static const u8 sApexRumor_MimeSr1[] = _("All the MR. MIME disappeared\ninto DIGLETT CAVE and raised\nbarriers.");
static const u8 sApexRumor_MimeSr2[] = _("DIGLETT CAVE leads to VERMILION.\nWatch out for invisible walls!");
static const u8 sApexRumor_Annihilape0[] = _("There's something angry and\nprimal deep within MT. MOON.");
static const u8 sApexRumor_Annihilape1[] = _("An enormous POKEMON smashed\nthrough the wall and rampaged\nwest.");
static const u8 sApexRumor_Annihilape2[] = _("A hulking PRIMEAPE took refuge\ninside MT. MOON, totally out of\ncontrol!");

static const struct ApexRumorDossierEntry sApexRumorDossierEntries[QUEST_3_SUB_COUNT] =
{
    [SUB_QUEST_APEX_TANGROWTH] =
    {
        sApexDossierName_Tangrowth,
        {sApexRumor_Tangrowth0, sApexRumor_Tangrowth1, sApexRumor_Tangrowth2},
        {sApexLoc_Viridian, sApexLoc_ForestGate, sApexLoc_ViridianForest},
        {sApexSrc_OldTale, sApexSrc_ScaredLocal, sApexSrc_FlowerWatcher},
        {OBJ_EVENT_GFX_WOMAN_3, OBJ_EVENT_GFX_WOMAN_2, OBJ_EVENT_GFX_AROMA_LADY},
        OBJ_EVENT_GFX_TANGROWTH,
        FLAG_INTERACTED_APEX_TANGROWTH
    },
    [SUB_QUEST_APEX_ZAPDOS] =
    {
        sApexDossierName_Zapdos,
        {sApexRumor_Zapdos0, sApexRumor_Zapdos1, sApexRumor_Zapdos2},
        {sApexLoc_PowerPlant, sApexLoc_Route10, sApexLoc_Route10},
        {sApexSrc_Engineer, sApexSrc_Rocker, sApexSrc_Picnicker},
        {OBJ_EVENT_GFX_SWIMMER_M_LAND, OBJ_EVENT_GFX_ENGINEER, OBJ_EVENT_GFX_ENGINEER},
        OBJ_EVENT_GFX_ZAPDOS,
        FLAG_INTERACTED_APEX_ZAPDOS
    },
    [SUB_QUEST_APEX_ARTICUNO] =
    {
        sApexDossierName_Articuno,
        {sApexRumor_Articuno0, sApexRumor_Articuno1, sApexRumor_Articuno2},
        {sApexLoc_Route20, sApexLoc_Seafoam, sApexLoc_Seafoam},
        {sApexSrc_Swimmer, sApexSrc_Trainer, sApexSrc_Explorer},
        {OBJ_EVENT_GFX_SCOUT_M, OBJ_EVENT_GFX_SAILOR, OBJ_EVENT_GFX_OLD_MAN_1},
        OBJ_EVENT_GFX_ARTICUNO,
        FLAG_INTERACTED_APEX_ARTICUNO
    },
    [SUB_QUEST_APEX_MEWTWO] =
    {
        sApexDossierName_Mewtwo,
        {sApexRumor_Mewtwo0, sApexRumor_Mewtwo1, sApexRumor_Mewtwo2},
        {sApexLoc_Unknown, sApexLoc_Unknown, sApexLoc_Unknown},
        {sApexSrc_NoWitness, sApexSrc_NoWitness, sApexSrc_NoWitness},
        {OBJ_EVENT_GFX_CLIPBOARD, OBJ_EVENT_GFX_CLIPBOARD, OBJ_EVENT_GFX_CLIPBOARD},
        OBJ_EVENT_GFX_MEWTWO,
        FLAG_INTERACTED_APEX_MEWTWO
    },
    [SUB_QUEST_APEX_OSSCYTHE] =
    {
        sApexDossierName_Osscythe,
        {sApexRumor_Osscythe0, sApexRumor_Osscythe1, sApexRumor_Osscythe2},
        {sApexLoc_Saffron, sApexLoc_Lavender, sApexLoc_PokemonTower},
        {sApexSrc_Rocket, sApexSrc_TownLocal, sApexSrc_Mourner},
        {OBJ_EVENT_GFX_ROCKET_M, OBJ_EVENT_GFX_BOY, OBJ_EVENT_GFX_CHANNELER},
        OBJ_EVENT_GFX_OSSCYTHE,
        FLAG_INTERACTED_APEX_OSSCYTHE
    },
    [SUB_QUEST_APEX_MOLTRES] =
    {
        sApexDossierName_Moltres,
        {sApexRumor_Moltres0, sApexRumor_Moltres1, sApexRumor_Moltres2},
        {sApexLoc_Cinnabar, sApexLoc_Volcano, sApexLoc_Volcano},
        {sApexSrc_Scientist, sApexSrc_Hiker, sApexSrc_Witness},
        {OBJ_EVENT_GFX_OLD_MAN_1, OBJ_EVENT_GFX_SCIENTIST, OBJ_EVENT_GFX_KINDLER},
        OBJ_EVENT_GFX_MOLTRES,
        FLAG_INTERACTED_APEX_MOLTRES
    },
    [SUB_QUEST_APEX_MIME_SR] =
    {
        sApexDossierName_MimeSr,
        {sApexRumor_MimeSr0, sApexRumor_MimeSr1, sApexRumor_MimeSr2},
        {sApexLoc_DiglettCave, sApexLoc_Route11, sApexLoc_DiglettCave},
        {sApexSrc_CaveVisitor, sApexSrc_Pokemaniac, sApexSrc_CaveVisitor},
        {OBJ_EVENT_GFX_OLD_MAN_1, OBJ_EVENT_GFX_POKE_MANIAC, OBJ_EVENT_GFX_HIKER},
        OBJ_EVENT_GFX_MIME_SR,
        FLAG_INTERACTED_APEX_MIME_SR
    },
    [SUB_QUEST_APEX_ANNIHILAPE] =
    {
        sApexDossierName_Annihilape,
        {sApexRumor_Annihilape0, sApexRumor_Annihilape1, sApexRumor_Annihilape2},
        {sApexLoc_MtMoon, sApexLoc_Cerulean, sApexLoc_Route4},
        {sApexSrc_CaveVisitor, sApexSrc_HouseResident, sApexSrc_Hiker},
        {OBJ_EVENT_GFX_BALDING_MAN, OBJ_EVENT_GFX_HIKER, OBJ_EVENT_GFX_WOMAN_1},
        OBJ_EVENT_GFX_ANNIHILAPE,
        FLAG_INTERACTED_APEX_ANNIHILAPE
    },
};

#define APEX_LOG_PROF_OAK  (ApexLog_NONTRAINER_START + 0)
#define APEX_LOG_DAISY_OAK (ApexLog_NONTRAINER_START + 1)
#define APEX_LOG_BILL      (ApexLog_NONTRAINER_START + 2)
#define APEX_LOG_MR_FUJI   (ApexLog_NONTRAINER_START + 3)

static const u16 sTrainerIdxs[] = {
    [APEX_LOG_PERSON_OAK]      = APEX_LOG_PROF_OAK,
    [APEX_LOG_PERSON_DAISY]    = APEX_LOG_DAISY_OAK,
    [APEX_LOG_PERSON_BROCK]    = TRAINER_LEADER_BROCK_1,
    [APEX_LOG_PERSON_MISTY]    = TRAINER_LEADER_MISTY,
    [APEX_LOG_PERSON_LTSURGE]  = TRAINER_LEADER_LT_SURGE,
    [APEX_LOG_PERSON_ERIKA]    = TRAINER_LEADER_ERIKA,
    [APEX_LOG_PERSON_KOGA]     = TRAINER_LEADER_KOGA,
    [APEX_LOG_PERSON_SABRINA]  = TRAINER_LEADER_SABRINA,
    [APEX_LOG_PERSON_BLAINE]   = TRAINER_LEADER_BLAINE,
    [APEX_LOG_PERSON_LORELEI]  = TRAINER_ELITE_FOUR_LORELEI,
    [APEX_LOG_PERSON_BRUNO]    = TRAINER_ELITE_FOUR_BRUNO,
    [APEX_LOG_PERSON_AGATHA]   = TRAINER_ELITE_FOUR_AGATHA,
    [APEX_LOG_PERSON_LANCE]    = TRAINER_ELITE_FOUR_LANCE,
    [APEX_LOG_PERSON_BILL]     = APEX_LOG_BILL,
    [APEX_LOG_PERSON_MRFUJI]   = APEX_LOG_MR_FUJI,
    [APEX_LOG_PERSON_GIOVANNI] = TRAINER_LEADER_GIOVANNI
};

static const u8 *const sNonTrainerNamePointers[] = {
    gApexLogOakName,
    gApexLogDaisyName,
    gApexLogBillName,
    gApexLogMrFujiName
};

static const u8 sApexLogTrainerPicIdxs[] = {
    [APEX_LOG_PERSON_OAK]      = TRAINER_PIC_CAMPER,
    [APEX_LOG_PERSON_DAISY]    = TRAINER_PIC_LASS,
    [APEX_LOG_PERSON_BROCK]    = TRAINER_PIC_LEADER_BROCK,
    [APEX_LOG_PERSON_MISTY]    = TRAINER_PIC_LEADER_MISTY,
    [APEX_LOG_PERSON_LTSURGE]  = TRAINER_PIC_LEADER_LT_SURGE,
    [APEX_LOG_PERSON_ERIKA]    = TRAINER_PIC_LEADER_ERIKA,
    [APEX_LOG_PERSON_KOGA]     = TRAINER_PIC_LEADER_KOGA,
    [APEX_LOG_PERSON_SABRINA]  = TRAINER_PIC_LEADER_SABRINA,
    [APEX_LOG_PERSON_BLAINE]   = TRAINER_PIC_LEADER_BLAINE,
    [APEX_LOG_PERSON_LORELEI]  = TRAINER_PIC_ELITE_FOUR_LORELEI,
    [APEX_LOG_PERSON_BRUNO]    = TRAINER_PIC_ELITE_FOUR_BRUNO,
    [APEX_LOG_PERSON_AGATHA]   = TRAINER_PIC_ELITE_FOUR_AGATHA,
    [APEX_LOG_PERSON_LANCE]    = TRAINER_PIC_ELITE_FOUR_LANCE,
    [APEX_LOG_PERSON_BILL]     = TRAINER_PIC_PSYCHIC_M,
    [APEX_LOG_PERSON_MRFUJI]   = TRAINER_PIC_GENTLEMAN,
    [APEX_LOG_PERSON_GIOVANNI] = TRAINER_PIC_LEADER_GIOVANNI,
};

static const u8 sApexLogTrainerGenders_Unused[] = {
    [APEX_LOG_PERSON_OAK]      = MALE,
    [APEX_LOG_PERSON_DAISY]    = FEMALE,
    [APEX_LOG_PERSON_BROCK]    = MALE,
    [APEX_LOG_PERSON_MISTY]    = FEMALE,
    [APEX_LOG_PERSON_LTSURGE]  = MALE,
    [APEX_LOG_PERSON_ERIKA]    = FEMALE,
    [APEX_LOG_PERSON_KOGA]     = MALE,
    [APEX_LOG_PERSON_SABRINA]  = FEMALE,
    [APEX_LOG_PERSON_BLAINE]   = MALE,
    [APEX_LOG_PERSON_LORELEI]  = FEMALE,
    [APEX_LOG_PERSON_BRUNO]    = MALE,
    [APEX_LOG_PERSON_AGATHA]   = FEMALE,
    [APEX_LOG_PERSON_LANCE]    = MALE,
    [APEX_LOG_PERSON_BILL]     = MALE,
    [APEX_LOG_PERSON_MRFUJI]   = MALE,
    [APEX_LOG_PERSON_GIOVANNI] = MALE,
};

static const u8 *const sApexLogNameAndQuotesPointers[2 * NUM_APEX_LOG_PERSONS] =
{
    gApexLogPersonName_ProfOak,
    gApexLogPersonName_Daisy,
    gApexLogPersonName_Brock,
    gApexLogPersonName_Misty,
    gApexLogPersonName_LtSurge,
    gApexLogPersonName_Erika,
    gApexLogPersonName_Koga,
    gApexLogPersonName_Sabrina,
    gApexLogPersonName_Blaine,
    gApexLogPersonName_Lorelei,
    gApexLogPersonName_Bruno,
    gApexLogPersonName_Agatha,
    gApexLogPersonName_Lance,
    gApexLogPersonName_Bill,
    gApexLogPersonName_MrFuji,
    gApexLogPersonName_Giovanni,

    gApexLogPersonQuote_ProfOak,
    gApexLogPersonQuote_Daisy,
    gApexLogPersonQuote_Brock,
    gApexLogPersonQuote_Misty,
    gApexLogPersonQuote_LtSurge,
    gApexLogPersonQuote_Erika,
    gApexLogPersonQuote_Koga,
    gApexLogPersonQuote_Sabrina,
    gApexLogPersonQuote_Blaine,
    gApexLogPersonQuote_Lorelei,
    gApexLogPersonQuote_Bruno,
    gApexLogPersonQuote_Agatha,
    gApexLogPersonQuote_Lance,
    gApexLogPersonQuote_Bill,
    gApexLogPersonQuote_MrFuji,
    gApexLogPersonQuote_Giovanni
};

static const u8 *const sApexLogFlavorTextPointers[] = {
    gApexLogFlavorText_ProfOak0, gApexLogFlavorText_ProfOak1, gApexLogFlavorText_ProfOak2, gApexLogFlavorText_ProfOak3, gApexLogFlavorText_ProfOak4, gApexLogFlavorText_ProfOak5,
    gApexLogFlavorText_Daisy0, gApexLogFlavorText_Daisy1, gApexLogFlavorText_Daisy2, gApexLogFlavorText_Daisy3, gApexLogFlavorText_Daisy4, gApexLogFlavorText_Daisy5,
    gApexLogFlavorText_Brock0, gApexLogFlavorText_Brock1, gApexLogFlavorText_Brock2, gApexLogFlavorText_Brock3, gApexLogFlavorText_Brock4, gApexLogFlavorText_Brock5,
    gApexLogFlavorText_Misty0, gApexLogFlavorText_Misty1, gApexLogFlavorText_Misty2, gApexLogFlavorText_Misty3, gApexLogFlavorText_Misty4, gApexLogFlavorText_Misty5,
    gApexLogFlavorText_LtSurge0, gApexLogFlavorText_LtSurge1, gApexLogFlavorText_LtSurge2, gApexLogFlavorText_LtSurge3, gApexLogFlavorText_LtSurge4, gApexLogFlavorText_LtSurge5,
    gApexLogFlavorText_Erika0, gApexLogFlavorText_Erika1, gApexLogFlavorText_Erika2, gApexLogFlavorText_Erika3, gApexLogFlavorText_Erika4, gApexLogFlavorText_Erika5,
    gApexLogFlavorText_Koga0, gApexLogFlavorText_Koga1, gApexLogFlavorText_Koga2, gApexLogFlavorText_Koga3, gApexLogFlavorText_Koga4, gApexLogFlavorText_Koga5,
    gApexLogFlavorText_Sabrina0, gApexLogFlavorText_Sabrina1, gApexLogFlavorText_Sabrina2, gApexLogFlavorText_Sabrina3, gApexLogFlavorText_Sabrina4, gApexLogFlavorText_Sabrina5,
    gApexLogFlavorText_Blaine0, gApexLogFlavorText_Blaine1, gApexLogFlavorText_Blaine2, gApexLogFlavorText_Blaine3, gApexLogFlavorText_Blaine4, gApexLogFlavorText_Blaine5,
    gApexLogFlavorText_Lorelei0, gApexLogFlavorText_Lorelei1, gApexLogFlavorText_Lorelei2, gApexLogFlavorText_Lorelei3, gApexLogFlavorText_Lorelei4, gApexLogFlavorText_Lorelei5,
    gApexLogFlavorText_Bruno0, gApexLogFlavorText_Bruno1, gApexLogFlavorText_Bruno2, gApexLogFlavorText_Bruno3, gApexLogFlavorText_Bruno4, gApexLogFlavorText_Bruno5,
    gApexLogFlavorText_Agatha0, gApexLogFlavorText_Agatha1, gApexLogFlavorText_Agatha2, gApexLogFlavorText_Agatha3, gApexLogFlavorText_Agatha4, gApexLogFlavorText_Agatha5,
    gApexLogFlavorText_Lance0, gApexLogFlavorText_Lance1, gApexLogFlavorText_Lance2, gApexLogFlavorText_Lance3, gApexLogFlavorText_Lance4, gApexLogFlavorText_Lance5,
    gApexLogFlavorText_Bill0, gApexLogFlavorText_Bill1, gApexLogFlavorText_Bill2, gApexLogFlavorText_Bill3, gApexLogFlavorText_Bill4, gApexLogFlavorText_Bill5,
    gApexLogFlavorText_MrFuji0, gApexLogFlavorText_MrFuji1, gApexLogFlavorText_MrFuji2, gApexLogFlavorText_MrFuji3, gApexLogFlavorText_MrFuji4, gApexLogFlavorText_MrFuji5,
    gApexLogFlavorText_Giovanni0, gApexLogFlavorText_Giovanni1, gApexLogFlavorText_Giovanni2, gApexLogFlavorText_Giovanni3, gApexLogFlavorText_Giovanni4, gApexLogFlavorText_Giovanni5
};

static const u8 sApexLogArrayNpcGraphicsIds[] = {
    // OAK
    OBJ_EVENT_GFX_SIGN,
    OBJ_EVENT_GFX_PROF_OAK,
    OBJ_EVENT_GFX_WORKER_F,
    OBJ_EVENT_GFX_CLIPBOARD,
    OBJ_EVENT_GFX_AGATHA,
    OBJ_EVENT_GFX_SCIENTIST,
    // DAISY
    OBJ_EVENT_GFX_SCIENTIST,
    OBJ_EVENT_GFX_WORKER_F,
    OBJ_EVENT_GFX_GENTLEMAN,
    OBJ_EVENT_GFX_CLIPBOARD,
    OBJ_EVENT_GFX_OLD_WOMAN,
    OBJ_EVENT_GFX_CLIPBOARD,
    // BROCK
    OBJ_EVENT_GFX_GYM_SIGN,
    OBJ_EVENT_GFX_BROCK,
    OBJ_EVENT_GFX_FAT_MAN,
    OBJ_EVENT_GFX_BOY,
    OBJ_EVENT_GFX_BALDING_MAN,
    OBJ_EVENT_GFX_CLIPBOARD,
    // MISTY
    OBJ_EVENT_GFX_GYM_SIGN,
    OBJ_EVENT_GFX_MISTY,
    OBJ_EVENT_GFX_SWIMMER_M_WATER,
    OBJ_EVENT_GFX_SCOUT_M,
    OBJ_EVENT_GFX_BEAUTY,
    OBJ_EVENT_GFX_CLIPBOARD,
    // LTSURGE
    OBJ_EVENT_GFX_GYM_SIGN,
    OBJ_EVENT_GFX_LT_SURGE,
    OBJ_EVENT_GFX_GENTLEMAN,
    OBJ_EVENT_GFX_GENTLEMAN,
    OBJ_EVENT_GFX_SAILOR,
    OBJ_EVENT_GFX_CLIPBOARD,
    // ERIKA
    OBJ_EVENT_GFX_GYM_SIGN,
    OBJ_EVENT_GFX_ERIKA,
    OBJ_EVENT_GFX_LASS,
    OBJ_EVENT_GFX_BEAUTY,
    OBJ_EVENT_GFX_ERIKA,
    OBJ_EVENT_GFX_CLIPBOARD,
    // KOGA
    OBJ_EVENT_GFX_GYM_SIGN,
    OBJ_EVENT_GFX_KOGA,
    OBJ_EVENT_GFX_ROCKER,
    OBJ_EVENT_GFX_LASS,
    OBJ_EVENT_GFX_CLIPBOARD,
    OBJ_EVENT_GFX_BALDING_MAN,
    // SABRINA
    OBJ_EVENT_GFX_GYM_SIGN,
    OBJ_EVENT_GFX_MAN,
    OBJ_EVENT_GFX_SABRINA,
    OBJ_EVENT_GFX_SABRINA,
    OBJ_EVENT_GFX_CLIPBOARD,
    OBJ_EVENT_GFX_ACE_TRAINER_M,
    // BLAINE
    OBJ_EVENT_GFX_GYM_SIGN,
    OBJ_EVENT_GFX_BLAINE,
    OBJ_EVENT_GFX_SCIENTIST,
    OBJ_EVENT_GFX_WOMAN_2,
    OBJ_EVENT_GFX_CLIPBOARD,
    OBJ_EVENT_GFX_CLIPBOARD,
    // LORELEI
    OBJ_EVENT_GFX_LORELEI,
    OBJ_EVENT_GFX_LORELEI,
    OBJ_EVENT_GFX_OLD_MAN_1,
    OBJ_EVENT_GFX_CLIPBOARD,
    OBJ_EVENT_GFX_LITTLE_GIRL,
    OBJ_EVENT_GFX_OLD_WOMAN,
    // BRUNO
    OBJ_EVENT_GFX_BRUNO,
    OBJ_EVENT_GFX_BRUNO,
    OBJ_EVENT_GFX_CLIPBOARD,
    OBJ_EVENT_GFX_BLACK_BELT,
    OBJ_EVENT_GFX_BEAUTY,
    OBJ_EVENT_GFX_BLACK_BELT,
    // AGATHA
    OBJ_EVENT_GFX_AGATHA,
    OBJ_EVENT_GFX_BLACK_BELT,
    OBJ_EVENT_GFX_BLACK_BELT,
    OBJ_EVENT_GFX_CLIPBOARD,
    OBJ_EVENT_GFX_AGATHA,
    OBJ_EVENT_GFX_OLD_WOMAN,
    // LANCE
    OBJ_EVENT_GFX_LANCE,
    OBJ_EVENT_GFX_LANCE,
    OBJ_EVENT_GFX_CRUSH_GIRL,
    OBJ_EVENT_GFX_WOMAN_1,
    OBJ_EVENT_GFX_CLIPBOARD,
    OBJ_EVENT_GFX_ACE_TRAINER_M,
    // BILL
    OBJ_EVENT_GFX_BLUE,
    OBJ_EVENT_GFX_YOUNGSTER,
    OBJ_EVENT_GFX_OLD_MAN_1,
    OBJ_EVENT_GFX_SCIENTIST,
    OBJ_EVENT_GFX_GENTLEMAN,
    OBJ_EVENT_GFX_MAN,
    // MRFUJI
    OBJ_EVENT_GFX_LITTLE_GIRL,
    OBJ_EVENT_GFX_ROCKET_M,
    OBJ_EVENT_GFX_CLIPBOARD,
    OBJ_EVENT_GFX_BALDING_MAN,
    OBJ_EVENT_GFX_CLIPBOARD,
    OBJ_EVENT_GFX_CLIPBOARD,
    // GIOVANNI
    OBJ_EVENT_GFX_GIOVANNI,
    OBJ_EVENT_GFX_SCIENTIST,
    OBJ_EVENT_GFX_SCIENTIST,
    OBJ_EVENT_GFX_GIOVANNI,
    OBJ_EVENT_GFX_GYM_GUY,
    OBJ_EVENT_GFX_SCIENTIST
};

static const u8 *const sFlavorTextOriginLocationTexts[] = {
    gApexLogFlavorTextOriginLocation_ProfOak0, gApexLogFlavorTextOriginLocation_ProfOak1, gApexLogFlavorTextOriginLocation_ProfOak2, gApexLogFlavorTextOriginLocation_ProfOak3, gApexLogFlavorTextOriginLocation_ProfOak4, gApexLogFlavorTextOriginLocation_ProfOak5,
    gApexLogFlavorTextOriginLocation_Daisy0, gApexLogFlavorTextOriginLocation_Daisy1, gApexLogFlavorTextOriginLocation_Daisy2, gApexLogFlavorTextOriginLocation_Daisy3, gApexLogFlavorTextOriginLocation_Daisy4, gApexLogFlavorTextOriginLocation_Daisy5,
    gApexLogFlavorTextOriginLocation_Brock0, gApexLogFlavorTextOriginLocation_Brock1, gApexLogFlavorTextOriginLocation_Brock2, gApexLogFlavorTextOriginLocation_Brock3, gApexLogFlavorTextOriginLocation_Brock4, gApexLogFlavorTextOriginLocation_Brock5,
    gApexLogFlavorTextOriginLocation_Misty0, gApexLogFlavorTextOriginLocation_Misty1, gApexLogFlavorTextOriginLocation_Misty2, gApexLogFlavorTextOriginLocation_Misty3, gApexLogFlavorTextOriginLocation_Misty4, gApexLogFlavorTextOriginLocation_Misty5,
    gApexLogFlavorTextOriginLocation_LtSurge0, gApexLogFlavorTextOriginLocation_LtSurge1, gApexLogFlavorTextOriginLocation_LtSurge2, gApexLogFlavorTextOriginLocation_LtSurge3, gApexLogFlavorTextOriginLocation_LtSurge4, gApexLogFlavorTextOriginLocation_LtSurge5,
    gApexLogFlavorTextOriginLocation_Erika0, gApexLogFlavorTextOriginLocation_Erika1, gApexLogFlavorTextOriginLocation_Erika2, gApexLogFlavorTextOriginLocation_Erika3, gApexLogFlavorTextOriginLocation_Erika4, gApexLogFlavorTextOriginLocation_Erika5,
    gApexLogFlavorTextOriginLocation_Koga0, gApexLogFlavorTextOriginLocation_Koga1, gApexLogFlavorTextOriginLocation_Koga2, gApexLogFlavorTextOriginLocation_Koga3, gApexLogFlavorTextOriginLocation_Koga4, gApexLogFlavorTextOriginLocation_Koga5,
    gApexLogFlavorTextOriginLocation_Sabrina0, gApexLogFlavorTextOriginLocation_Sabrina1, gApexLogFlavorTextOriginLocation_Sabrina2, gApexLogFlavorTextOriginLocation_Sabrina3, gApexLogFlavorTextOriginLocation_Sabrina4, gApexLogFlavorTextOriginLocation_Sabrina5,
    gApexLogFlavorTextOriginLocation_Blaine0, gApexLogFlavorTextOriginLocation_Blaine1, gApexLogFlavorTextOriginLocation_Blaine2, gApexLogFlavorTextOriginLocation_Blaine3, gApexLogFlavorTextOriginLocation_Blaine4, gApexLogFlavorTextOriginLocation_Blaine5,
    gApexLogFlavorTextOriginLocation_Lorelei0, gApexLogFlavorTextOriginLocation_Lorelei1, gApexLogFlavorTextOriginLocation_Lorelei2, gApexLogFlavorTextOriginLocation_Lorelei3, gApexLogFlavorTextOriginLocation_Lorelei4, gApexLogFlavorTextOriginLocation_Lorelei5,
    gApexLogFlavorTextOriginLocation_Bruno0, gApexLogFlavorTextOriginLocation_Bruno1, gApexLogFlavorTextOriginLocation_Bruno2, gApexLogFlavorTextOriginLocation_Bruno3, gApexLogFlavorTextOriginLocation_Bruno4, gApexLogFlavorTextOriginLocation_Bruno5,
    gApexLogFlavorTextOriginLocation_Agatha0, gApexLogFlavorTextOriginLocation_Agatha1, gApexLogFlavorTextOriginLocation_Agatha2, gApexLogFlavorTextOriginLocation_Agatha3, gApexLogFlavorTextOriginLocation_Agatha4, gApexLogFlavorTextOriginLocation_Agatha5,
    gApexLogFlavorTextOriginLocation_Lance0, gApexLogFlavorTextOriginLocation_Lance1, gApexLogFlavorTextOriginLocation_Lance2, gApexLogFlavorTextOriginLocation_Lance3, gApexLogFlavorTextOriginLocation_Lance4, gApexLogFlavorTextOriginLocation_Lance5,
    gApexLogFlavorTextOriginLocation_Bill0, gApexLogFlavorTextOriginLocation_Bill1, gApexLogFlavorTextOriginLocation_Bill2, gApexLogFlavorTextOriginLocation_Bill3, gApexLogFlavorTextOriginLocation_Bill4, gApexLogFlavorTextOriginLocation_Bill5,
    gApexLogFlavorTextOriginLocation_MrFuji0, gApexLogFlavorTextOriginLocation_MrFuji1, gApexLogFlavorTextOriginLocation_MrFuji2, gApexLogFlavorTextOriginLocation_MrFuji3, gApexLogFlavorTextOriginLocation_MrFuji4, gApexLogFlavorTextOriginLocation_MrFuji5,
    gApexLogFlavorTextOriginLocation_Giovanni0, gApexLogFlavorTextOriginLocation_Giovanni1, gApexLogFlavorTextOriginLocation_Giovanni2, gApexLogFlavorTextOriginLocation_Giovanni3, gApexLogFlavorTextOriginLocation_Giovanni4, gApexLogFlavorTextOriginLocation_Giovanni5
};

static const u8 *const sFlavorTextOriginObjectNameTexts[] = {
    gApexLogFlavorTextOriginObjectName_ProfOak0, gApexLogFlavorTextOriginObjectName_ProfOak1, gApexLogFlavorTextOriginObjectName_ProfOak2, gApexLogFlavorTextOriginObjectName_ProfOak3, gApexLogFlavorTextOriginObjectName_ProfOak4, gApexLogFlavorTextOriginObjectName_ProfOak5,
    gApexLogFlavorTextOriginObjectName_Daisy0, gApexLogFlavorTextOriginObjectName_Daisy1, gApexLogFlavorTextOriginObjectName_Daisy2, gApexLogFlavorTextOriginObjectName_Daisy3, gApexLogFlavorTextOriginObjectName_Daisy4, gApexLogFlavorTextOriginObjectName_Daisy5,
    gApexLogFlavorTextOriginObjectName_Brock0, gApexLogFlavorTextOriginObjectName_Brock1, gApexLogFlavorTextOriginObjectName_Brock2, gApexLogFlavorTextOriginObjectName_Brock3, gApexLogFlavorTextOriginObjectName_Brock4, gApexLogFlavorTextOriginObjectName_Brock5,
    gApexLogFlavorTextOriginObjectName_Misty0, gApexLogFlavorTextOriginObjectName_Misty1, gApexLogFlavorTextOriginObjectName_Misty2, gApexLogFlavorTextOriginObjectName_Misty3, gApexLogFlavorTextOriginObjectName_Misty4, gApexLogFlavorTextOriginObjectName_Misty5,
    gApexLogFlavorTextOriginObjectName_LtSurge0, gApexLogFlavorTextOriginObjectName_LtSurge1, gApexLogFlavorTextOriginObjectName_LtSurge2, gApexLogFlavorTextOriginObjectName_LtSurge3, gApexLogFlavorTextOriginObjectName_LtSurge4, gApexLogFlavorTextOriginObjectName_LtSurge5,
    gApexLogFlavorTextOriginObjectName_Erika0, gApexLogFlavorTextOriginObjectName_Erika1, gApexLogFlavorTextOriginObjectName_Erika2, gApexLogFlavorTextOriginObjectName_Erika3, gApexLogFlavorTextOriginObjectName_Erika4, gApexLogFlavorTextOriginObjectName_Erika5,
    gApexLogFlavorTextOriginObjectName_Koga0, gApexLogFlavorTextOriginObjectName_Koga1, gApexLogFlavorTextOriginObjectName_Koga2, gApexLogFlavorTextOriginObjectName_Koga3, gApexLogFlavorTextOriginObjectName_Koga4, gApexLogFlavorTextOriginObjectName_Koga5,
    gApexLogFlavorTextOriginObjectName_Sabrina0, gApexLogFlavorTextOriginObjectName_Sabrina1, gApexLogFlavorTextOriginObjectName_Sabrina2, gApexLogFlavorTextOriginObjectName_Sabrina3, gApexLogFlavorTextOriginObjectName_Sabrina4, gApexLogFlavorTextOriginObjectName_Sabrina5,
    gApexLogFlavorTextOriginObjectName_Blaine0, gApexLogFlavorTextOriginObjectName_Blaine1, gApexLogFlavorTextOriginObjectName_Blaine2, gApexLogFlavorTextOriginObjectName_Blaine3, gApexLogFlavorTextOriginObjectName_Blaine4, gApexLogFlavorTextOriginObjectName_Blaine5,
    gApexLogFlavorTextOriginObjectName_Lorelei0, gApexLogFlavorTextOriginObjectName_Lorelei1, gApexLogFlavorTextOriginObjectName_Lorelei2, gApexLogFlavorTextOriginObjectName_Lorelei3, gApexLogFlavorTextOriginObjectName_Lorelei4, gApexLogFlavorTextOriginObjectName_Lorelei5,
    gApexLogFlavorTextOriginObjectName_Bruno0, gApexLogFlavorTextOriginObjectName_Bruno1, gApexLogFlavorTextOriginObjectName_Bruno2, gApexLogFlavorTextOriginObjectName_Bruno3, gApexLogFlavorTextOriginObjectName_Bruno4, gApexLogFlavorTextOriginObjectName_Bruno5,
    gApexLogFlavorTextOriginObjectName_Agatha0, gApexLogFlavorTextOriginObjectName_Agatha1, gApexLogFlavorTextOriginObjectName_Agatha2, gApexLogFlavorTextOriginObjectName_Agatha3, gApexLogFlavorTextOriginObjectName_Agatha4, gApexLogFlavorTextOriginObjectName_Agatha5,
    gApexLogFlavorTextOriginObjectName_Lance0, gApexLogFlavorTextOriginObjectName_Lance1, gApexLogFlavorTextOriginObjectName_Lance2, gApexLogFlavorTextOriginObjectName_Lance3, gApexLogFlavorTextOriginObjectName_Lance4, gApexLogFlavorTextOriginObjectName_Lance5,
    gApexLogFlavorTextOriginObjectName_Bill0, gApexLogFlavorTextOriginObjectName_Bill1, gApexLogFlavorTextOriginObjectName_Bill2, gApexLogFlavorTextOriginObjectName_Bill3, gApexLogFlavorTextOriginObjectName_Bill4, gApexLogFlavorTextOriginObjectName_Bill5,
    gApexLogFlavorTextOriginObjectName_MrFuji0, gApexLogFlavorTextOriginObjectName_MrFuji1, gApexLogFlavorTextOriginObjectName_MrFuji2, gApexLogFlavorTextOriginObjectName_MrFuji3, gApexLogFlavorTextOriginObjectName_MrFuji4, gApexLogFlavorTextOriginObjectName_MrFuji5,
    gApexLogFlavorTextOriginObjectName_Giovanni0, gApexLogFlavorTextOriginObjectName_Giovanni1, gApexLogFlavorTextOriginObjectName_Giovanni2, gApexLogFlavorTextOriginObjectName_Giovanni3, gApexLogFlavorTextOriginObjectName_Giovanni4, gApexLogFlavorTextOriginObjectName_Giovanni5
};

static const struct SpriteSheet sUISpriteSheets[] = {
    {sSelectorCursorSpriteGfx,   0x400, SPRITETAG_SELECTOR_CURSOR},
    {sQuestionMarkSpriteGfx,     0x100, SPRITETAG_QUESTION_MARK},
    {sSpinningPokeballSpriteGfx, 0x1e0, SPRITETAG_SPINNING_POKEBALL},
    {sDaisySpriteGfx,            0x800, SPRITETAG_DAISY},
    {sFujiSpriteGfx,             0x800, SPRITETAG_FUJI},
    {sOakSpriteGfx,              0x800, SPRITETAG_OAK},
    {sBillSpriteGfx,             0x800, SPRITETAG_BILL},
    {}
};

static const struct SpritePalette sUISpritePalettes[] = {
    {sSelectorCursorSpritePalette, SPRITETAG_SELECTOR_CURSOR},
    {sSpinningPokeballSpritePalette, SPRITETAG_SPINNING_POKEBALL},
    {sSilhouettePalette, SPRITETAG_APEX_SILHOUETTE},
    {}
};

static const struct BgTemplate sUIBgTemplates[4] = {
    {
        .bg = 3,
        .charBaseIndex = 3,
        .mapBaseIndex =  30,
        .screenSize = 0,
        .paletteMode = FALSE,
        .priority = 3,
        .baseTile = 0x000
    },
    {
        .bg = 2,
        .charBaseIndex = 3,
        .mapBaseIndex =  27,
        .screenSize = 0,
        .paletteMode = FALSE,
        .priority = 2,
        .baseTile = 0x000
    },
    {
        .bg = 1,
        .charBaseIndex = 3,
        .mapBaseIndex =  28,
        .screenSize = 1,
        .paletteMode = FALSE,
        .priority = 0,
        .baseTile = 0x000
    },
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex =  31,
        .screenSize = 0,
        .paletteMode = FALSE,
        .priority = 2,
        .baseTile = 0x000
    },
};

static const struct WindowTemplate sUIWindowTemplates[] = {
    [FCWINDOWID_LIST] = {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 3,
        .width = 8,
        .height = 10,
        .paletteNum = 15,
        .baseBlock = 20
    },
    [FCWINDOWID_UIHELP] = {
        .bg = 0,
        .tilemapLeft = 6,
        .tilemapTop = 0,
        .width = 24,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 20 + 8 * 10
    },
    [FCWINDOWID_MSGBOX] = {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 26,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 20 + 8 * 10 + 24 * 2
    },
    [FCWINDOWID_ICONDESC] = {
        .bg = 0,
        .tilemapLeft = 15,
        .tilemapTop = 10,
        .width = 11,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 20 + 8 * 10 + 24 * 2 + 26 * 4
    },
    DUMMY_WIN_TEMPLATE
};

static const union AnimCmd sSelectorCursorAnim0[] = {
    ANIMCMD_FRAME( 0, 15),
    ANIMCMD_FRAME(16, 15),
    ANIMCMD_JUMP(0)
};

static const union AnimCmd *const sSelectorCursorAnims[] = {
    sSelectorCursorAnim0
};

static const struct OamData sSelectorCursorOamData = {
    .size = 2,
    .priority = 2
};

static const struct SpriteTemplate sSpriteTemplate_SelectorCursor = {
    SPRITETAG_SELECTOR_CURSOR, SPRITETAG_SELECTOR_CURSOR, &sSelectorCursorOamData, sSelectorCursorAnims, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
};

static const u8 sUnused[8] = {}; // ???

static const struct OamData sQuestionMarkTileOamData = {
    .shape = ST_OAM_V_RECTANGLE,
    .size = 2,
    .priority = 2
};

static const union AnimCmd sQuestionMarkTileAnim0[] = {
    ANIMCMD_FRAME( 0, 10),
    ANIMCMD_END
};

static const union AnimCmd *const sQuestionMarkTileAnims[] = {
    sQuestionMarkTileAnim0
};

static const struct SpriteTemplate sQuestionMarkTileSpriteTemplate = {
    SPRITETAG_QUESTION_MARK, 0xffff, &sQuestionMarkTileOamData, sQuestionMarkTileAnims, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
};

static const union AnimCmd sSpinningPokeballAnim0[] = {
    ANIMCMD_FRAME( 0, 10),
    ANIMCMD_END
};

static const union AnimCmd *const sSpinningPokeballAnims[] = {
    sSpinningPokeballAnim0
};

static const struct OamData sSpinningPokeballOamData = {
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .size = 2
};

static const union AffineAnimCmd sSpinningPokeballAffineAnim0[] = {
    AFFINEANIMCMD_FRAME(0, 0, 4, 20),
    AFFINEANIMCMD_JUMP(0)
};

static const union AffineAnimCmd *const sSpinningPokeballAffineAnims[] = {
    sSpinningPokeballAffineAnim0
};

static const struct SpriteTemplate sSpinningPokeballSpriteTemplate = {
    SPRITETAG_SPINNING_POKEBALL, SPRITETAG_SPINNING_POKEBALL, &sSpinningPokeballOamData, sSpinningPokeballAnims, NULL, sSpinningPokeballAffineAnims, SpriteCB_FCSpinningPokeball
};

static const union AnimCmd sDaisyFujiOakBillAnim0[] = {
    ANIMCMD_FRAME( 0, 15),
    ANIMCMD_END
};

static const union AnimCmd *const sDaisyFujiOakBillAnims[] = {
    sDaisyFujiOakBillAnim0
};

static const struct OamData sDaisyFujiOakBillOamData = {
    .size = 3
};

static const struct SpriteTemplate sDaisySpriteTemplate = {
    SPRITETAG_DAISY, 0xffff, &sDaisyFujiOakBillOamData, sDaisyFujiOakBillAnims, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
};

static const struct SpriteTemplate sFujiSpriteTemplate = {
    SPRITETAG_FUJI, 0xffff, &sDaisyFujiOakBillOamData, sDaisyFujiOakBillAnims, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
};

static const struct SpriteTemplate sOakSpriteTemplate = {
    SPRITETAG_OAK, 0xffff, &sDaisyFujiOakBillOamData, sDaisyFujiOakBillAnims, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
};

static const struct SpriteTemplate sBillSpriteTemplate = {
    SPRITETAG_BILL, 0xffff, &sDaisyFujiOakBillOamData, sDaisyFujiOakBillAnims, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
};

static void ApexLog_VBlankCallback(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void MainCB2_ApexLogMain(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

void UseApexLog(MainCallback savedCallback)
{
    SetVBlankCallback(NULL);
    sApexLogData = AllocZeroed(sizeof(struct ApexLogData));
    sApexLogData->savedCallback = savedCallback;
    sApexLogData->listMenuCurIdx = 0;
    sApexLogData->listMenuTopIdx2 = 0;
    sApexLogData->listMenuDrawnSelIdx = 0;
    sApexLogData->viewingFlavorText = FALSE;
    PlaySE(SE_M_SWIFT);
    SetMainCallback2(MainCB2_LoadApexLog);
}

void UseApexRumorDossier(MainCallback savedCallback, u8 apexSubquest)
{
    if (apexSubquest >= QUEST_3_SUB_COUNT)
        apexSubquest = SUB_QUEST_APEX_TANGROWTH;

    SetVBlankCallback(NULL);
    sApexLogData = AllocZeroed(sizeof(struct ApexLogData));
    sApexLogData->savedCallback = savedCallback;
    sApexLogData->listMenuCurIdx = 0;
    sApexLogData->listMenuTopIdx2 = 0;
    sApexLogData->listMenuDrawnSelIdx = 0;
    sApexLogData->viewingFlavorText = FALSE;
    sApexLogData->isApexDossier = TRUE;
    sApexLogData->apexSubquest = apexSubquest;
    PlaySE(SE_M_SWIFT);
    SetMainCallback2(MainCB2_LoadApexLog);
}

static void MainCB2_LoadApexLog(void)
{
    switch (gMain.state)
    {
        case 0:
            SetVBlankCallback(NULL);
            FCSetup_ClearVideoRegisters();
            gMain.state++;
            break;
        case 1:
            FCSetup_ResetTasksAndSpriteResources();
            gMain.state++;
            break;
        case 2:
            sBg3TilemapBuffer = AllocZeroed(BG_SCREEN_SIZE);     // 256x256
            sBg1TilemapBuffer = AllocZeroed(BG_SCREEN_SIZE * 2); // 512x256
            sBg2TilemapBuffer = AllocZeroed(BG_SCREEN_SIZE);     // 256x256
            ResetBgsAndClearDma3BusyFlags(0);
            InitBgsFromTemplates(0, sUIBgTemplates, NELEMS(sUIBgTemplates));
            SetBgTilemapBuffer(3, sBg3TilemapBuffer);
            SetBgTilemapBuffer(2, sBg2TilemapBuffer);
            SetBgTilemapBuffer(1, sBg1TilemapBuffer);
            FCSetup_ResetBGCoords();
            gMain.state++;
            break;
        case 3:
            LoadBgTiles(3, gApexLogBgTiles, sizeof(gApexLogBgTiles), 0);
            CopyToBgTilemapBufferRect(3, gApexLogBg3Tilemap, 0, 0, 32, 32);
            LoadPalette(&gApexLogBgPals[0], BG_PLTT_ID(0), 2 * PLTT_SIZE_4BPP);
            LoadPalette(&gApexLogBgPals[1], BG_PLTT_ID(1), PLTT_SIZE_4BPP);
            CopyToBgTilemapBufferRect(2, gApexLogBg2Tilemap, 0, 0, 32, 32);
            if (sApexLogData->isApexDossier)
            {
                FillBgTilemapBufferRect(2, 0x000, 0, 2, 10, 12, 0);
                FillBgTilemapBufferRect(2, 0x000, 14, 9, 13, 1, 0);
                FillBgTilemapBufferRect(2, 0x000, 14, 10, 13, 3, 0);
                FillBgTilemapBufferRect(2, 0x000, 14, 13, 13, 1, 0);
                CopyToBgTilemapBufferRect(2, &gApexLogBg2Tilemap[9 * 32 + 14], ApexLog_APEX_DOSSIER_INFOBOX_LEFT, 9, 13, 1);
                CopyToBgTilemapBufferRect(2, &gApexLogBg2Tilemap[13 * 32 + 14], ApexLog_APEX_DOSSIER_INFOBOX_LEFT, 13, 13, 1);
                UpdateInfoBoxTilemap(2, 2);
            }
            CopyToBgTilemapBufferRect_ChangePalette(1, sApexLogTilemap, 30, 0, 32, 32, 0x11);
            LoadPalette(GetTextWindowPalette(2), BG_PLTT_ID(15), PLTT_SIZE_4BPP);
            gMain.state++;
            break;
        case 4:
            if (IsDma3ManagerBusyWithBgCopy() != TRUE)
            {
                ShowBg(0);
                ShowBg(1);
                ShowBg(2);
                ShowBg(3);
                CopyBgTilemapBufferToVram(3);
                CopyBgTilemapBufferToVram(2);
                CopyBgTilemapBufferToVram(1);
                gMain.state++;
            }
            break;
        case 5:
            InitWindows(sUIWindowTemplates);
            if (sApexLogData->isApexDossier)
                SetWindowAttribute(FCWINDOWID_ICONDESC, WINDOW_TILEMAP_LEFT, ApexLog_APEX_DOSSIER_ICONDESC_LEFT);
            DeactivateAllTextPrinters();
            Setup_DrawMsgAndListBoxes();
            if (!sApexLogData->isApexDossier)
            {
                sListMenuItems = AllocZeroed(17 * sizeof(struct ListMenuItem));
                ApexLog_CreateListMenu();
            }
            gMain.state++;
            break;
        case 6:
            LoadUISpriteSheetsAndPalettes();
            CreateAllFlavorTextIcons(APEX_LOG_PERSON_OAK);
            WipeMsgBoxAndTransfer();
            BeginNormalPaletteFade(PALETTES_ALL,0, 16, 0, 0);
            gMain.state++;
            break;
        case 7:
            FCSetup_TurnOnDisplay();
            SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG0 | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 | BLDCNT_TGT2_OBJ | BLDCNT_TGT2_BD);
            SetGpuReg(REG_OFFSET_BLDALPHA, 0x07);
            SetGpuReg(REG_OFFSET_BLDY, 0x08);
            SetVBlankCallback(ApexLog_VBlankCallback);
            if (!sApexLogData->isApexDossier)
            {
                sApexLogData->listMenuTopIdx = 0;
                ApexLog_CreateScrollIndicatorArrowPair();
            }
            UpdateInfoBoxTilemap(1, 4);
            CreateTask(Task_WaitFadeOnInit, 0x08);
            SetMainCallback2(MainCB2_ApexLogMain);
            gMain.state = 0;
            break;
    }
}

static void LoadUISpriteSheetsAndPalettes(void)
{
    LoadSpriteSheets(sUISpriteSheets);
    LoadSpritePalettes(sUISpritePalettes);
}

static void Task_WaitFadeOnInit(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        if (sApexLogData->isApexDossier && sApexLogData->personHasUnlockedPanels)
            ApexLog_EnterFlavorTextSelection(taskId, 1, FALSE);
        else
            gTasks[taskId].func = Task_TopMenuHandleInput;
    }
}

static void Task_TopMenuHandleInput(u8 taskId)
{
    u16 cursorPos;
    struct Task *task = &gTasks[taskId];
    if (FindTaskIdByFunc(Task_FCOpenOrCloseInfoBox) == 0xFF)
    {
        RunTextPrinters();
        if ((JOY_NEW(SELECT_BUTTON)) && !sApexLogData->inPickMode && sApexLogData->savedCallback != CB2_BagMenuFromStartMenu)
            task->func = Task_StartToCloseApexLog;
        else if (JOY_NEW(START_BUTTON) && !sApexLogData->isApexDossier)
        {
            cursorPos = ApexLogGetCursorY();
            if (TryExitPickMode(taskId) == TRUE)
            {
                PlaySE(SE_M_LOCK_ON);
            }
            else if (cursorPos != sApexLogData->numUnlockedPersons - 1) // anything but CANCEL
            {
                PlaySE(SE_M_LOCK_ON);
                FillWindowPixelRect(FCWINDOWID_ICONDESC, PIXEL_FILL(0), 0, 0, 88, 32);
                ApexLog_PutWindowTilemapAndCopyWindowToVramMode3(FCWINDOWID_ICONDESC);
                UpdateInfoBoxTilemap(2, 4);
                UpdateInfoBoxTilemap(1, 5);
                PrintUIHelp(1);
                task->data[2] = CreatePersonPicSprite(sApexLogData->unlockedPersons[cursorPos]);
                gSprites[task->data[2]].x2 = 0xF0;
                gSprites[task->data[2]].data[0] = 1;
                task->data[3] = CreateSpinningPokeballSprite();
                gSprites[task->data[3]].x2 = 0xF0;
                gSprites[task->data[3]].data[0] = 1;
                task->func = Task_EnterPickMode;
            }
        }
        else if (JOY_NEW(A_BUTTON))
        {
            cursorPos = ListMenu_ProcessInput(0);
            if (cursorPos == sApexLogData->numUnlockedPersons - 1) // CANCEL
                task->func = Task_StartToCloseApexLog;
            else if (sApexLogData->inPickMode)
            {
                if (!IsTextPrinterActive(2) && HasUnlockedAllFlavorTextsForCurrentPerson() == TRUE)
                    GetPickModeText();
            }
            else if (sApexLogData->personHasUnlockedPanels)
            {
                ApexLog_EnterFlavorTextSelection(taskId, sApexLogData->isApexDossier ? 1 : task->data[1], TRUE);
            }
        }
        else if (JOY_NEW(B_BUTTON))
        {
            if (TryExitPickMode(taskId) != TRUE)
                task->func = Task_StartToCloseApexLog;
        }
        else
            ListMenu_ProcessInput(0);
    }
}

static void ApexLog_EnterFlavorTextSelection(u8 taskId, u8 initialSlot, bool8 playSound)
{
    u8 i;
    struct Task *task = &gTasks[taskId];

    if (playSound)
        PlaySE(SE_SELECT);

    task->data[1] = initialSlot;
    task->data[0] = CreateFlavorTextIconSelectorCursorSprite(task->data[1]);
    for (i = 0; i < 6; i++)
    {
        if (i != task->data[1])
            SetMessageSelectorIconObjMode(sApexLogData->spriteIds[i], ST_OAM_OBJ_BLEND);
    }
    gIconDescriptionBoxIsOpen = 0xFF;
    if (!sApexLogData->isApexDossier)
        PlaceListMenuCursor(FALSE);
    PrintUIHelp(2);
    if (gSprites[sApexLogData->spriteIds[task->data[1]]].data[1] != 0xFF) // not a ? tile
    {
        PrintSelectedNameInBrightGreen(taskId);
        UpdateIconDescriptionBox(task->data[1]);
    }
    FreeListMenuSelectorArrowPairResources();
    task->func = Task_FlavorTextDisplayHandleInput;
}

static bool8 TryExitPickMode(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    if (sApexLogData->inPickMode)
    {
        gSprites[task->data[2]].data[0] = 2;
        gSprites[task->data[2]].x2 += 10;
        gSprites[task->data[3]].data[0] = 2;
        gSprites[task->data[3]].x2 += 10;
        WipeMsgBoxAndTransfer();
        task->func = Task_ExitPickMode;
        MessageBoxPrintEmptyText();
        sApexLogData->pickModeOverCancel = FALSE;
        return TRUE;
    }
    return FALSE;
}

static void MessageBoxPrintEmptyText(void)
{
    AddTextPrinterParameterized2(FCWINDOWID_MSGBOX, FONT_NORMAL, gApexLogText_ClearTextbox, 0, NULL, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY);
}

static void Task_EnterPickMode(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    if (gSprites[task->data[2]].data[0] == 0)
    {
        GetPickModeText();
        sApexLogData->inPickMode = TRUE;
        task->func = Task_TopMenuHandleInput;
    }
    else
        ChangeBgX(1, 0xA00, 1);
}

static void Task_ExitPickMode(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    if (GetBgX(1) != 0)
        ChangeBgX(1, 0xA00, 2);
    else
        ChangeBgX(1, 0x000, 0);
    if (gSprites[task->data[2]].data[0] == 0)
    {
        if (sApexLogData->personHasUnlockedPanels)
            PrintUIHelp(0);
        UpdateInfoBoxTilemap(1, 4);
        UpdateInfoBoxTilemap(2, 2);
        sApexLogData->inPickMode = FALSE;
        DestroyPersonPicSprite(taskId, ApexLogGetCursorY());
        task->func = Task_TopMenuHandleInput;
        gSprites[task->data[3]].callback = SpriteCB_DestroySpinningPokeball;
    }
}

static void Task_FlavorTextDisplayHandleInput(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    s16 *data = gTasks[taskId].data;

    RunTextPrinters();
    if (JOY_NEW(A_BUTTON) && !IsTextPrinterActive(2))
    {
        u8 spriteId = sApexLogData->spriteIds[data[1]];
        if (gSprites[spriteId].data[1] != 0xFF)
            PrintSelectedNameInBrightGreen(taskId);
    }
    if (JOY_NEW(B_BUTTON))
    {
        u8 i;
        if (sApexLogData->isApexDossier)
        {
            gSprites[task->data[0]].callback = SpriteCB_DestroyFlavorTextIconSelectorCursor;
            Task_StartToCloseApexLog(taskId);
            return;
        }

        PlaySE(SE_SELECT);
        for (i = 0; i < 6; i++)
            SetMessageSelectorIconObjMode(sApexLogData->spriteIds[i], ST_OAM_OBJ_NORMAL);
        WipeMsgBoxAndTransfer();
        gSprites[task->data[0]].callback = SpriteCB_DestroyFlavorTextIconSelectorCursor;
        if (gIconDescriptionBoxIsOpen != 0xFF)
            UpdateIconDescriptionBoxOff();
        PlaceListMenuCursor(TRUE);
        PrintUIHelp(0);
        ApexLog_CreateScrollIndicatorArrowPair();
        MessageBoxPrintEmptyText();
        task->func = Task_TopMenuHandleInput;
    }
    else if (JOY_NEW(DPAD_UP) || JOY_NEW(DPAD_DOWN))
    {
        if (sApexLogData->isApexDossier)
        {
            ApexLog_MoveApexDossierCursor(taskId, ApexLog_IsApexDossierMonSlot(task->data[1]) ? 4 : 1);
        }
        else if (task->data[1] >= 3)
        {
            task->data[1] -= 3;
            ApexLog_MoveSelectorCursor(taskId, 0, -0x1b);
        }
        else
        {
            task->data[1] += 3;
            ApexLog_MoveSelectorCursor(taskId, 0, +0x1b);
        }
    }
    else if (JOY_NEW(DPAD_LEFT))
    {
        if (sApexLogData->isApexDossier)
        {
            if (task->data[1] > 3)
                ApexLog_MoveApexDossierCursor(taskId, task->data[1] - 1);
        }
        else if (task->data[1] == 0 || task->data[1] % 3 == 0)
        {
            task->data[1] += 2;
            ApexLog_MoveSelectorCursor(taskId, +0x5e, 0);
        }
        else
        {
            task->data[1]--;
            ApexLog_MoveSelectorCursor(taskId, -0x2f, 0);
        }
    }
    else if (JOY_NEW(DPAD_RIGHT))
    {
        if (sApexLogData->isApexDossier)
        {
            if (task->data[1] >= 3 && task->data[1] < 5)
                ApexLog_MoveApexDossierCursor(taskId, task->data[1] + 1);
        }
        else if ((task->data[1] + 1) % 3 == 0)
        {
            task->data[1] -= 2;
            ApexLog_MoveSelectorCursor(taskId, -0x5e, 0);
        }
        else
        {
            task->data[1]++;
            ApexLog_MoveSelectorCursor(taskId, +0x2f, 0);
        }
    }
}

static void ApexLog_MoveSelectorCursor(u8 taskId, s8 dx, s8 dy)
{
    u8 i;
    s16 *data = gTasks[taskId].data;
    PlaySE(SE_M_SWAGGER2);
    gSprites[data[0]].x += dx;
    gSprites[data[0]].y += dy;
    for (i = 0; i < 6; i++)
        SetMessageSelectorIconObjMode(sApexLogData->spriteIds[i], ST_OAM_OBJ_BLEND);
    FillWindowPixelRect(FCWINDOWID_MSGBOX, PIXEL_FILL(1), 0, 0, 0xd0, 0x20);
    MessageBoxPrintEmptyText();
    if (SetMessageSelectorIconObjMode(sApexLogData->spriteIds[data[1]], ST_OAM_OBJ_NORMAL) == TRUE)
    {
        PrintSelectedNameInBrightGreen(taskId);
        UpdateIconDescriptionBox(data[1]);
    }
    else if (gIconDescriptionBoxIsOpen != 0xFF)
        UpdateIconDescriptionBoxOff();
}

static void GetPickModeText(void)
{
    s32 whichText = 0;
    u16 who;

    if (sApexLogData->isApexDossier)
    {
        WipeMsgBoxAndTransfer();
        MessageBoxPrintEmptyText();
        return;
    }

    who = ApexLogGetCursorY();
    if (gSaveBlock1Ptr->apexLog[sApexLogData->unlockedPersons[who]].pickState != APEX_LOG_PICK_STATE_COLORED)
    {
        WipeMsgBoxAndTransfer();
        MessageBoxPrintEmptyText();
    }
    else
    {
        FillWindowPixelRect(FCWINDOWID_MSGBOX, PIXEL_FILL(1), 0, 0, 0xd0, 0x20);
        if (HasUnlockedAllFlavorTextsForCurrentPerson() == TRUE)
            whichText = NUM_APEX_LOG_PERSONS;
        StringExpandPlaceholders(gStringVar4, sApexLogNameAndQuotesPointers[sApexLogData->unlockedPersons[who] + whichText]);
        AddTextPrinterParameterized2(FCWINDOWID_MSGBOX, FONT_NORMAL, gStringVar4, GetTextSpeedSetting(), NULL, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY);
        ApexLog_PutWindowTilemapAndCopyWindowToVramMode3(FCWINDOWID_MSGBOX);
    }
}

static void PrintSelectedNameInBrightGreen(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u16 cursorPos;
    FillWindowPixelRect(FCWINDOWID_MSGBOX, PIXEL_FILL(1), 0, 0, 0xd0, 0x20);
    if (sApexLogData->isApexDossier)
    {
        if (ApexLog_IsApexDossierMonSlot(data[1]))
        {
            StringExpandPlaceholders(gStringVar4, ApexLog_HasEncounteredApexDossierMon() ? sApexMonRecorded : sApexMonUnknown);
        }
        else if (ApexLog_IsApexDossierWitnessSlot(data[1]) && ApexLog_HasApexDossierRumor(ApexLog_GetApexDossierRumorForSlot(data[1])))
        {
            StringExpandPlaceholders(gStringVar4, sApexRumorDossierEntries[sApexLogData->apexSubquest].rumors[ApexLog_GetApexDossierRumorForSlot(data[1])]);
        }
        else
        {
            StringExpandPlaceholders(gStringVar4, sApexRumorUnknown);
        }
    }
    else
    {
        cursorPos = ApexLogGetCursorY();
        StringExpandPlaceholders(gStringVar4, sApexLogFlavorTextPointers[sApexLogData->unlockedPersons[cursorPos] * 6 + data[1]]);
    }
    AddTextPrinterParameterized2(FCWINDOWID_MSGBOX, FONT_NORMAL, gStringVar4, GetTextSpeedSetting(), NULL, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY);
    ApexLog_PutWindowTilemapAndCopyWindowToVramMode3(FCWINDOWID_MSGBOX);
}

static void WipeMsgBoxAndTransfer(void)
{
    FillWindowPixelRect(FCWINDOWID_MSGBOX, PIXEL_FILL(1), 0, 0, 0xd0, 0x20);
    ApexLog_PutWindowTilemapAndCopyWindowToVramMode3(FCWINDOWID_MSGBOX);
}

static void Setup_DrawMsgAndListBoxes(void)
{
    LoadStdWindowFrameGfx();
    DrawDialogueFrame(FCWINDOWID_MSGBOX, TRUE);
    ApexLog_PutWindowTilemapAndCopyWindowToVramMode3(FCWINDOWID_MSGBOX);
    if (!sApexLogData->isApexDossier)
        ApexLog_PutWindowTilemapAndCopyWindowToVramMode3(FCWINDOWID_LIST);
}

static void ApexLog_PutWindowTilemapAndCopyWindowToVramMode3(u8 windowId)
{
    PutWindowTilemap(windowId);
    CopyWindowToVram(windowId, COPYWIN_FULL);
}

static bool8 SetMessageSelectorIconObjMode(u8 spriteId, u8 objMode)
{
    if (gSprites[spriteId].data[1] != 0xFF)
    {
        gSprites[spriteId].oam.objMode = objMode;
        return TRUE;
    }
    return FALSE;
}

static void Task_StartToCloseApexLog(u8 taskId)
{
    PlaySE(SE_M_SWIFT);
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, 0);
    gTasks[taskId].func = Task_DestroyAssetsAndCloseApexLog;
}

static void Task_DestroyAssetsAndCloseApexLog(u8 taskId)
{
    u8 i;
    bool8 isApexDossier;

    if (!gPaletteFade.active)
    {
        isApexDossier = sApexLogData->isApexDossier;
        if (sApexLogData->inPickMode)
        {
            DestroyPersonPicSprite(taskId, ApexLogGetCursorY());
            FreeSpriteOamMatrix(&gSprites[gTasks[taskId].data[3]]);
            DestroySprite(&gSprites[gTasks[taskId].data[3]]);
        }
        for (i = 0; i < 6; i++)
        {
            DestroySprite(&gSprites[sApexLogData->spriteIds[i]]);
        }
        FreeNonTrainerPicTiles();
        FreeSpinningPokeballSpriteResources();
        FreeSelectionCursorSpriteResources();
        FreeQuestionMarkSpriteResources();
        FreeListMenuSelectorArrowPairResources();
        SetMainCallback2(sApexLogData->savedCallback);
        if (!isApexDossier)
            DestroyListMenuTask(sApexLogData->listMenuTaskId, NULL, NULL);
        Free(sBg3TilemapBuffer);
        Free(sBg1TilemapBuffer);
        Free(sBg2TilemapBuffer);
        Free(sApexLogData);
        if (sListMenuItems != NULL)
        {
            Free(sListMenuItems);
            sListMenuItems = NULL;
        }
        if (!isApexDossier)
            ApexLog_DestroyWindow(FCWINDOWID_LIST);
        ApexLog_DestroyWindow(FCWINDOWID_UIHELP);
        ApexLog_DestroyWindow(FCWINDOWID_MSGBOX);
        ApexLog_DestroyWindow(FCWINDOWID_ICONDESC);
        FreeAllWindowBuffers();
        DestroyTask(taskId);
    }
}

static void ApexLog_DestroyWindow(u8 windowId)
{
    FillWindowPixelBuffer(windowId, 0);
    ClearWindowTilemap(windowId);
    CopyWindowToVram(windowId, COPYWIN_GFX);
    RemoveWindow(windowId);
}

static u8 AdjustGiovanniIndexIfBeatenInGym(u8 a0)
{
    if (HasTrainerBeenFought(TRAINER_LEADER_GIOVANNI) == TRUE)
    {
        if (a0 == 9)
            return APEX_LOG_PERSON_GIOVANNI;
        if (a0 > 9)
            return a0 - 1;
    }
    return a0;
}

static void PrintUIHelp(u8 state)
{
    s32 width;
    const u8 * src = gApexLogText_MainScreenUI;

    if (sApexLogData->isApexDossier)
        src = sApexDossierText_UI;
    else if (state != 0)
    {
        src = gApexLogText_FlavorTextUI;
        if (state == 1)
            src = gApexLogText_PickScreenUI;
    }
    width = GetStringWidth(FONT_SMALL, src, 0);
    FillWindowPixelRect(FCWINDOWID_UIHELP, PIXEL_FILL(0), 0, 0, 0xc0, 0x10);
    AddTextPrinterParameterized4(FCWINDOWID_UIHELP, FONT_SMALL, 188 - width, 0, 0, 2, sTextColor_White, -1, src);
    ApexLog_PutWindowTilemapAndCopyWindowToVramMode3(FCWINDOWID_UIHELP);
}

static void DestroyAllFlavorTextIcons(void)
{
    u8 i;
    for (i = 0; i < 6; i++)
    {
        DestroySprite(&gSprites[sApexLogData->spriteIds[i]]);
    }
}

static bool8 CreateAllFlavorTextIcons(u8 who)
{
    bool8 result = FALSE;
    u8 i;

    if (sApexLogData->isApexDossier)
    {
        for (i = 0; i < 6; i++)
        {
            if (ApexLog_IsApexDossierMonSlot(i))
            {
                sApexLogData->spriteIds[i] = CreateApexLogObject(
                    sApexRumorDossierEntries[sApexLogData->apexSubquest].apexGraphicsId,
                    i,
                    ApexLog_GetFlavorTextIconX(i),
                    ApexLog_GetFlavorTextIconY(i)
                );
                if (!ApexLog_HasEncounteredApexDossierMon())
                    SetApexDossierSilhouettePalette(sApexLogData->spriteIds[i]);
                result = TRUE;
            }
            else if (ApexLog_IsApexDossierWitnessSlot(i) && ApexLog_HasApexDossierRumor(ApexLog_GetApexDossierRumorForSlot(i)))
            {
                sApexLogData->spriteIds[i] = CreateApexLogObject(
                    sApexRumorDossierEntries[sApexLogData->apexSubquest].graphicsIds[ApexLog_GetApexDossierRumorForSlot(i)],
                    i,
                    ApexLog_GetFlavorTextIconX(i),
                    ApexLog_GetFlavorTextIconY(i)
                );
                result = TRUE;
            }
            else
            {
                if (ApexLog_IsApexDossierWitnessSlot(i))
                {
                    sApexLogData->spriteIds[i] = CreateApexLogObject(
                        sApexRumorDossierEntries[sApexLogData->apexSubquest].graphicsIds[ApexLog_GetApexDossierRumorForSlot(i)],
                        i,
                        ApexLog_GetFlavorTextIconX(i),
                        ApexLog_GetFlavorTextIconY(i)
                    );
                    SetApexDossierSilhouettePalette(sApexLogData->spriteIds[i]);
                }
                else
                {
                    sApexLogData->spriteIds[i] = PlaceQuestionMarkTile(
                        ApexLog_GetFlavorTextIconX(i),
                        ApexLog_GetFlavorTextIconY(i) - 16
                    );
                    gSprites[sApexLogData->spriteIds[i]].invisible = TRUE;
                }
                gSprites[sApexLogData->spriteIds[i]].data[1] = 0xFF;
            }
        }

        sApexLogData->personHasUnlockedPanels = result;
        PrintUIHelp(result ? 0 : 1);
        return result;
    }

    for (i = 0; i < 6; i++)
    {
        if ((gSaveBlock1Ptr->apexLog[sApexLogData->unlockedPersons[who]].flavorTextFlags >> i) & 1)
        {
            sApexLogData->spriteIds[i] = CreateApexLogObject(
                sApexLogArrayNpcGraphicsIds[sApexLogData->unlockedPersons[who] * 6 + i],
                i,
                ApexLog_GetFlavorTextIconX(i),
                ApexLog_GetFlavorTextIconY(i)
            );
            result = TRUE;
        }
        else
        {
            sApexLogData->spriteIds[i] = PlaceQuestionMarkTile(
                ApexLog_GetFlavorTextIconX(i),
                ApexLog_GetFlavorTextIconY(i) - 16
            );
            gSprites[sApexLogData->spriteIds[i]].data[1] = 0xFF;
        }
    }
    if (result == TRUE)
    {
        sApexLogData->personHasUnlockedPanels = TRUE;
        if (sApexLogData->inPickMode)
            PrintUIHelp(1);
        else
            PrintUIHelp(0);
    }
    else
    {
        sApexLogData->personHasUnlockedPanels = FALSE;
        PrintUIHelp(1);
    }
    return result;
}

void ResetApexLog(void)
{
    u8 i;
    for (i = 0; i < NUM_APEX_LOG_PERSONS; i++)
    {
        gSaveBlock1Ptr->apexLog[i].pickState = APEX_LOG_PICK_STATE_NO_DRAW;
        gSaveBlock1Ptr->apexLog[i].flavorTextFlags = 0;
        gSaveBlock1Ptr->apexLog[i].unk_0_E = 0;
    }
    gSaveBlock1Ptr->apexLog[APEX_LOG_PERSON_OAK].pickState = APEX_LOG_PICK_STATE_COLORED;
}

void FullyUnlockApexLog(void)
{
    u8 i, j;
    for (i = 0; i < NUM_APEX_LOG_PERSONS; i++)
    {
        gSaveBlock1Ptr->apexLog[i].pickState = APEX_LOG_PICK_STATE_COLORED;
        for (j = 0; j < 6; j++)
        {
            gSaveBlock1Ptr->apexLog[i].flavorTextFlags |= (1 << j);
        }
    }
}

static void FCSetup_ClearVideoRegisters(void)
{
    void *vram = (void *)VRAM;
    DmaClearLarge16(3, vram, VRAM_SIZE, 0x1000);
    DmaClear32(3, OAM, OAM_SIZE);
    DmaClear16(3, PLTT, PLTT_SIZE);
    SetGpuReg(REG_OFFSET_DISPCNT,  0);
    SetGpuReg(REG_OFFSET_BG0CNT,   0);
    SetGpuReg(REG_OFFSET_BG0HOFS,  0);
    SetGpuReg(REG_OFFSET_BG0VOFS,  0);
    SetGpuReg(REG_OFFSET_BG1CNT,   0);
    SetGpuReg(REG_OFFSET_BG1HOFS,  0);
    SetGpuReg(REG_OFFSET_BG1VOFS,  0);
    SetGpuReg(REG_OFFSET_BG2CNT,   0);
    SetGpuReg(REG_OFFSET_BG2HOFS,  0);
    SetGpuReg(REG_OFFSET_BG2VOFS,  0);
    SetGpuReg(REG_OFFSET_BG3CNT,   0);
    SetGpuReg(REG_OFFSET_BG3HOFS,  0);
    SetGpuReg(REG_OFFSET_BG3VOFS,  0);
    SetGpuReg(REG_OFFSET_WIN0H,    0);
    SetGpuReg(REG_OFFSET_WIN0V,    0);
    SetGpuReg(REG_OFFSET_WININ,    0);
    SetGpuReg(REG_OFFSET_WINOUT,   0);
    SetGpuReg(REG_OFFSET_BLDCNT,   0);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
    SetGpuReg(REG_OFFSET_BLDY,     0);
}

static void FCSetup_ResetTasksAndSpriteResources(void)
{
    ScanlineEffect_Stop();
    ResetTasks();
    ResetSpriteData();
    ResetAllPicSprites();
    ResetPaletteFade();
    InitObjectEventPalettes(0);
    gReservedSpritePaletteCount = 7;

}

static void FCSetup_TurnOnDisplay(void)
{
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON);
}

static void FCSetup_ResetBGCoords(void)
{
    ChangeBgX(0, 0, 0);
    ChangeBgY(0, 0, 0);
    ChangeBgX(1, 0, 0);
    ChangeBgY(1, 0, 0);
    ChangeBgX(2, 0, 0);
    ChangeBgY(2, 0, 0);
    ChangeBgX(3, 0, 0);
    ChangeBgY(3, 0, 0);
}

void SetFlavorTextFlagFromSpecialVars(void)
{
    if (gSpecialVar_0x8004 < NUM_APEX_LOG_PERSONS && gSpecialVar_0x8005 < 6)
    {
        gSaveBlock1Ptr->apexLog[gSpecialVar_0x8004].flavorTextFlags |= (1 << gSpecialVar_0x8005);
        gSpecialVar_0x8005 = APEX_LOG_PICK_STATE_SILHOUETTE;
        UpdatePickStateFromSpecialVar8005();
    }
}

void UpdatePickStateFromSpecialVar8005(void)
{
    if (gSpecialVar_0x8004 < NUM_APEX_LOG_PERSONS && gSpecialVar_0x8005 < 3)
    {
        if (gSpecialVar_0x8005 == APEX_LOG_PICK_STATE_NO_DRAW)
            return;
        if (   gSpecialVar_0x8005 == APEX_LOG_PICK_STATE_SILHOUETTE
            && gSaveBlock1Ptr->apexLog[gSpecialVar_0x8004].pickState == APEX_LOG_PICK_STATE_COLORED
           )
            return;
        gSaveBlock1Ptr->apexLog[gSpecialVar_0x8004].pickState = gSpecialVar_0x8005;
    }
}

static bool8 HasUnlockedAllFlavorTextsForCurrentPerson(void)
{
    u8 i;
    u8 who = sApexLogData->unlockedPersons[ApexLogGetCursorY()];
    for (i = 0; i < 6; i++)
    {
        if (!((gSaveBlock1Ptr->apexLog[who].flavorTextFlags >> i) & 1))
            return FALSE;
    }
    return TRUE;
}

static void FreeSelectionCursorSpriteResources(void)
{
    FreeSpriteTilesByTag(SPRITETAG_SELECTOR_CURSOR);
    FreeSpritePaletteByTag(SPRITETAG_SELECTOR_CURSOR);
}

static u8 CreateFlavorTextIconSelectorCursorSprite(s16 where)
{
    s16 x = ApexLog_GetFlavorTextIconX(where);
    s16 y = ApexLog_GetFlavorTextIconY(where) - 13;
    return CreateSprite(&sSpriteTemplate_SelectorCursor, x, y, 0);
}

static s16 ApexLog_GetFlavorTextIconX(u8 slot)
{
    if (sApexLogData->isApexDossier)
        return ApexLog_APEX_DOSSIER_ICON_X_LEFT + ApexLog_ICON_X_SPACING * (slot % 3);
    return ApexLog_ICON_X_LEFT + ApexLog_ICON_X_SPACING * (slot % 3);
}

static s16 ApexLog_GetFlavorTextIconY(u8 slot)
{
    if (sApexLogData->isApexDossier)
        return ApexLog_APEX_DOSSIER_ICON_Y_TOP + ApexLog_ICON_Y_SPACING * (slot / 3);
    return ApexLog_ICON_Y_TOP + ApexLog_ICON_Y_SPACING * (slot / 3);
}

static void SpriteCB_DestroyFlavorTextIconSelectorCursor(struct Sprite *sprite)
{
    DestroySprite(sprite);
}

static void FreeQuestionMarkSpriteResources(void)
{
    FreeSpriteTilesByTag(SPRITETAG_QUESTION_MARK);
}

static u8 PlaceQuestionMarkTile(u8 x, u8 y)
{
    u8 spriteId = CreateSprite(&sQuestionMarkTileSpriteTemplate, x, y, 8);
    gSprites[spriteId].oam.priority = 2;
    gSprites[spriteId].oam.paletteNum = 2;
    return spriteId;
}

static void FreeSpinningPokeballSpriteResources(void)
{
    FreeSpriteTilesByTag(SPRITETAG_SPINNING_POKEBALL);
    FreeSpritePaletteByTag(SPRITETAG_SPINNING_POKEBALL);
    FreeSpritePaletteByTag(SPRITETAG_APEX_SILHOUETTE);
}

static u8 CreateSpinningPokeballSprite(void)
{
    return CreateSprite(&sSpinningPokeballSpriteTemplate, 0xe2, 0x42, 0);
}

static void SpriteCB_DestroySpinningPokeball(struct Sprite *sprite)
{
    FreeSpriteOamMatrix(sprite);
    DestroySprite(sprite);
}

static void FreeNonTrainerPicTiles(void)
{
    FreeSpriteTilesByTag(SPRITETAG_DAISY);
    FreeSpriteTilesByTag(SPRITETAG_FUJI);
    FreeSpriteTilesByTag(SPRITETAG_OAK);
    FreeSpriteTilesByTag(SPRITETAG_BILL);
}

static void SpriteCB_FCSpinningPokeball(struct Sprite *sprite)
{
    if (sprite->data[0] == 1)
    {
        if (sprite->x2 - 10 < 0)
        {
            sprite->x2 = 0;
            sprite->data[0] = 0;
        }
        else
            sprite->x2 -= 10;
    }
    else if (sprite->data[0] == 2)
    {
        if (sprite->x2 > 240)
        {
            sprite->x2 = 240;
            sprite->data[0] = 0;
        }
        else
            sprite->x2 += 10;
    }
}

#define PERSON_PAL_NUM 6
#define PERSON_X  148
#define PERSON_Y   66

static u8 CreatePersonPicSprite(u8 fcPersonIdx)
{
    u8 spriteId;
    if (sApexLogData->isApexDossier)
        fcPersonIdx = APEX_LOG_PERSON_OAK;

    if (fcPersonIdx == APEX_LOG_PERSON_DAISY)
    {
        spriteId = CreateSprite(&sDaisySpriteTemplate, PERSON_X, PERSON_Y, 0);
        LoadPalette(sDaisySpritePalette, OBJ_PLTT_ID(PERSON_PAL_NUM), sizeof(sDaisySpritePalette));
        gSprites[spriteId].oam.paletteNum = PERSON_PAL_NUM;
    }
    else if (fcPersonIdx == APEX_LOG_PERSON_MRFUJI)
    {
        spriteId = CreateSprite(&sFujiSpriteTemplate, PERSON_X, PERSON_Y, 0);
        LoadPalette(sFujiSpritePalette, OBJ_PLTT_ID(PERSON_PAL_NUM), sizeof(sFujiSpritePalette));
        gSprites[spriteId].oam.paletteNum = PERSON_PAL_NUM;
    }
    else if (fcPersonIdx == APEX_LOG_PERSON_OAK)
    {
        spriteId = CreateSprite(&sOakSpriteTemplate, PERSON_X, PERSON_Y, 0);
        LoadPalette(sOakSpritePalette, OBJ_PLTT_ID(PERSON_PAL_NUM), sizeof(sOakSpritePalette));
        gSprites[spriteId].oam.paletteNum = PERSON_PAL_NUM;
    }
    else if (fcPersonIdx == APEX_LOG_PERSON_BILL)
    {
        spriteId = CreateSprite(&sBillSpriteTemplate, PERSON_X, PERSON_Y, 0);
        LoadPalette(sBillSpritePalette, OBJ_PLTT_ID(PERSON_PAL_NUM), sizeof(sBillSpritePalette));
        gSprites[spriteId].oam.paletteNum = PERSON_PAL_NUM;
    }
    else
    {
        spriteId = CreateTrainerPicSprite(sApexLogTrainerPicIdxs[fcPersonIdx], TRUE, PERSON_X, PERSON_Y, PERSON_PAL_NUM, TAG_NONE);
    }
    gSprites[spriteId].callback = SpriteCB_FCSpinningPokeball;
    if (gSaveBlock1Ptr->apexLog[fcPersonIdx].pickState == APEX_LOG_PICK_STATE_SILHOUETTE)
        LoadPalette(sSilhouettePalette, OBJ_PLTT_ID(PERSON_PAL_NUM), sizeof(sSilhouettePalette));
    return spriteId;
}

static void DestroyPersonPicSprite(u8 taskId, u16 who)
{
    s16 * data = gTasks[taskId].data;
    u16 who_copy = who;
    if (who == sApexLogData->numUnlockedPersons - 1)
        who_copy = who - 1;
    if (   sApexLogData->unlockedPersons[who_copy] == APEX_LOG_PERSON_DAISY
        || sApexLogData->unlockedPersons[who_copy] == APEX_LOG_PERSON_MRFUJI
        || sApexLogData->unlockedPersons[who_copy] == APEX_LOG_PERSON_OAK
        || sApexLogData->unlockedPersons[who_copy] == APEX_LOG_PERSON_BILL
    )
        DestroySprite(&gSprites[data[2]]);
    else
        FreeAndDestroyTrainerPicSprite(data[2]);
}

static void UpdateIconDescriptionBox(u8 whichText)
{
    s32 width;
    u32 idx = 6 * sApexLogData->unlockedPersons[ApexLogGetCursorY()] + whichText;
    HandleFlavorTextModeSwitch(TRUE);
    gIconDescriptionBoxIsOpen = 1;
    FillWindowPixelRect(FCWINDOWID_ICONDESC, PIXEL_FILL(0), 0, 0, 0x58, 0x20);

    if (sApexLogData->isApexDossier)
    {
        const u8 *location = sApexRumorLocationUnknown;
        const u8 *source = sApexRumorSourceUnknown;

        if (ApexLog_IsApexDossierMonSlot(whichText))
        {
            location = sApexLoc_Apex;
            source = ApexLog_HasEncounteredApexDossierMon() ? sApexSrc_FieldRecord : sApexRumorSourceUnknown;
        }
        else if (ApexLog_IsApexDossierWitnessSlot(whichText) && ApexLog_HasApexDossierRumor(ApexLog_GetApexDossierRumorForSlot(whichText)))
        {
            location = sApexRumorDossierEntries[sApexLogData->apexSubquest].locations[ApexLog_GetApexDossierRumorForSlot(whichText)];
            source = sApexRumorDossierEntries[sApexLogData->apexSubquest].sources[ApexLog_GetApexDossierRumorForSlot(whichText)];
        }

        width = (0x54 - GetStringWidth(FONT_SMALL, location, 0)) / 2;
        AddTextPrinterParameterized4(FCWINDOWID_ICONDESC, FONT_SMALL, width, 0, 0, 2, sTextColor_DkGrey, -1, location);
        StringExpandPlaceholders(gStringVar1, source);
    }
    else
    {
        width = (0x54 - GetStringWidth(FONT_SMALL, sFlavorTextOriginLocationTexts[idx], 0)) / 2;
        AddTextPrinterParameterized4(FCWINDOWID_ICONDESC, FONT_SMALL, width, 0, 0, 2, sTextColor_DkGrey, -1, sFlavorTextOriginLocationTexts[idx]);
        StringExpandPlaceholders(gStringVar1, sFlavorTextOriginObjectNameTexts[idx]);
    }

    width = (0x54 - GetStringWidth(FONT_SMALL, gStringVar1, 0)) / 2;
    AddTextPrinterParameterized4(FCWINDOWID_ICONDESC, FONT_SMALL, width, 10, 0, 2, sTextColor_DkGrey, -1, gStringVar1);
    ApexLog_PutWindowTilemapAndCopyWindowToVramMode3(FCWINDOWID_ICONDESC);
}

static void UpdateIconDescriptionBoxOff(void)
{
    HandleFlavorTextModeSwitch(FALSE);
    gIconDescriptionBoxIsOpen = 0xFF;
}

static void ApexLog_CreateListMenu(void)
{
    InitListMenuTemplate();
    sApexLogData->numUnlockedPersons = ApexLog_PopulateListMenu();
    sApexLogData->listMenuTaskId = ListMenuInit(&gApexLog_ListMenuTemplate, 0, 0);
    ApexLog_PutWindowTilemapAndCopyWindowToVramMode3_2(FCWINDOWID_LIST);
}

static void InitListMenuTemplate(void)
{
    gApexLog_ListMenuTemplate.items = sListMenuItems;
    gApexLog_ListMenuTemplate.moveCursorFunc = ApexLog_MoveCursorFunc;
    gApexLog_ListMenuTemplate.itemPrintFunc = NULL;
    gApexLog_ListMenuTemplate.totalItems = 1;
    gApexLog_ListMenuTemplate.maxShowed = 1;
    gApexLog_ListMenuTemplate.windowId = FCWINDOWID_LIST;
    gApexLog_ListMenuTemplate.header_X = 0;
    gApexLog_ListMenuTemplate.item_X = 8;
    gApexLog_ListMenuTemplate.cursor_X = 0;
    gApexLog_ListMenuTemplate.upText_Y = 4;
    gApexLog_ListMenuTemplate.cursorPal = 2;
    gApexLog_ListMenuTemplate.fillValue = 0;
    gApexLog_ListMenuTemplate.cursorShadowPal = 3;
    gApexLog_ListMenuTemplate.lettersSpacing = 0;
    gApexLog_ListMenuTemplate.itemVerticalPadding = 0;
    gApexLog_ListMenuTemplate.scrollMultiple = 0;
    gApexLog_ListMenuTemplate.fontId = FONT_NORMAL;
    gApexLog_ListMenuTemplate.cursorKind = 0;
}

static void ApexLog_MoveCursorFunc(s32 itemIndex, bool8 onInit, struct ListMenu *list)
{
    u16 listMenuTopIdx;
    u8 taskId;
    u16 personIdx;
    sLastMenuIdx = 0;
    personIdx = sApexLogData->listMenuTopIdx2 + sApexLogData->listMenuDrawnSelIdx;
    ApexLog_DoMoveCursor(itemIndex, onInit);
    taskId = FindTaskIdByFunc(Task_TopMenuHandleInput);
    if (taskId != 0xFF)
    {
        struct Task *task = &gTasks[taskId];
        PlaySE(SE_SELECT);
        task->data[1] = 0;
        ListMenuGetScrollAndRow(sApexLogData->listMenuTaskId, &listMenuTopIdx, NULL);
        sApexLogData->listMenuTopIdx = listMenuTopIdx;
        if (itemIndex != sApexLogData->numUnlockedPersons - 1)
        {
            DestroyAllFlavorTextIcons();
            CreateAllFlavorTextIcons(itemIndex);
            if (sApexLogData->inPickMode)
            {
                if (!sApexLogData->pickModeOverCancel)
                {
                    DestroyPersonPicSprite(taskId, personIdx);
                    sLastMenuIdx = itemIndex;
                    task->func = Task_SwitchToPickMode;
                }
                else
                {
                    gSprites[task->data[2]].invisible = FALSE;
                    sApexLogData->pickModeOverCancel = FALSE;
                    gSprites[task->data[2]].data[0] = 0;
                    GetPickModeText();
                }
            }
            else
            {
                FillWindowPixelRect(FCWINDOWID_MSGBOX, PIXEL_FILL(1), 0, 0, 0xd0, 0x20);
                ApexLog_PutWindowTilemapAndCopyWindowToVramMode3(FCWINDOWID_MSGBOX);
            }
        }
        else
        {
            PrintCancelDescription();
            if (sApexLogData->inPickMode)
            {
                gSprites[task->data[2]].invisible = TRUE;
                sApexLogData->pickModeOverCancel = TRUE;
            }
            else
            {
                u8 i;
                for (i = 0; i < 6; i++)
                {
                    gSprites[sApexLogData->spriteIds[i]].invisible = TRUE;
                }
            }
        }
    }
}

static void Task_SwitchToPickMode(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    task->data[2] = CreatePersonPicSprite(sApexLogData->unlockedPersons[sLastMenuIdx]);
    gSprites[task->data[2]].data[0] = 0;
    GetPickModeText();
    task->func = Task_TopMenuHandleInput;
}

static void PrintCancelDescription(void)
{
    FillWindowPixelRect(FCWINDOWID_MSGBOX, PIXEL_FILL(1), 0, 0, 0xd0, 0x20);
    AddTextPrinterParameterized2(FCWINDOWID_MSGBOX, FONT_NORMAL, gApexLogText_ApexLogWillBeClosed, 0, NULL, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY);
    ApexLog_PutWindowTilemapAndCopyWindowToVramMode3(FCWINDOWID_MSGBOX);
}

static void ApexLog_DoMoveCursor(s32 itemIndex, bool8 onInit)
{
    u16 listY;
    u16 cursorY;
    u16 who;
    ListMenuGetScrollAndRow(sApexLogData->listMenuTaskId, &listY, &cursorY);
    who = listY + cursorY;
    AddTextPrinterParameterized4(FCWINDOWID_LIST, FONT_NORMAL, 8, 14 * cursorY + 4, 0, 0, sTextColor_Green, 0, sListMenuItems[itemIndex].label);
    if (!onInit)
    {
        if (listY < sApexLogData->listMenuTopIdx2)
            sApexLogData->listMenuDrawnSelIdx++;
        else if (listY > sApexLogData->listMenuTopIdx2 && who != sApexLogData->numUnlockedPersons - 1)
            sApexLogData->listMenuDrawnSelIdx--;
        AddTextPrinterParameterized4(FCWINDOWID_LIST, FONT_NORMAL, 8, 14 * sApexLogData->listMenuDrawnSelIdx + 4, 0, 0, sTextColor_DkGrey, 0, sListMenuItems[sApexLogData->listMenuCurIdx].label);

    }
    sApexLogData->listMenuCurIdx = itemIndex;
    sApexLogData->listMenuDrawnSelIdx = cursorY;
    sApexLogData->listMenuTopIdx2 = listY;
}

static u8 ApexLog_PopulateListMenu(void)
{
    u8 nitems = 0;
    u8 i;

    if (sApexLogData->isApexDossier)
    {
        sListMenuItems[nitems].label = sApexRumorDossierEntries[sApexLogData->apexSubquest].name;
        sListMenuItems[nitems].index = nitems;
        sApexLogData->unlockedPersons[nitems] = APEX_LOG_PERSON_OAK;
        nitems++;
        sListMenuItems[nitems].label = gApexLogText_Cancel;
        sListMenuItems[nitems].index = nitems;
        sApexLogData->unlockedPersons[nitems] = 0xFF;
        nitems++;
        gApexLog_ListMenuTemplate.totalItems = nitems;
        gApexLog_ListMenuTemplate.maxShowed = nitems;
        return nitems;
    }

    for (i = 0; i < NUM_APEX_LOG_PERSONS; i++)
    {
        u8 apexLogIdx = AdjustGiovanniIndexIfBeatenInGym(i);
        if (gSaveBlock1Ptr->apexLog[apexLogIdx].pickState != APEX_LOG_PICK_STATE_NO_DRAW)
        {
            if (sTrainerIdxs[apexLogIdx] < ApexLog_NONTRAINER_START)
            {
                sListMenuItems[nitems].label = gTrainers[sTrainerIdxs[apexLogIdx]].trainerName;
                sListMenuItems[nitems].index = nitems;
            }
            else
            {
                sListMenuItems[nitems].label = sNonTrainerNamePointers[sTrainerIdxs[apexLogIdx] - ApexLog_NONTRAINER_START];
                sListMenuItems[nitems].index = nitems;
            }
            sApexLogData->unlockedPersons[nitems] = apexLogIdx;
            nitems++;
        }
    }
    sListMenuItems[nitems].label = gApexLogText_Cancel;
    sListMenuItems[nitems].index = nitems;
    sApexLogData->unlockedPersons[nitems] = 0xFF;
    nitems++;
    gApexLog_ListMenuTemplate.totalItems = nitems;
    if (nitems < 5)
        gApexLog_ListMenuTemplate.maxShowed = nitems;
    else
        gApexLog_ListMenuTemplate.maxShowed = 5;
    return nitems;
}

static bool8 ApexLog_HasApexDossierRumor(u8 rumor)
{
    return LogbookMenu_HasHeardApexRumor(sApexLogData->apexSubquest, rumor);
}

static bool8 ApexLog_HasEncounteredApexDossierMon(void)
{
    return FlagGet(sApexRumorDossierEntries[sApexLogData->apexSubquest].apexInteractedFlag);
}

static bool8 ApexLog_IsApexDossierMonSlot(u8 slot)
{
    return slot == 1;
}

static bool8 ApexLog_IsApexDossierWitnessSlot(u8 slot)
{
    return slot >= 3 && slot < 6;
}

static u8 ApexLog_GetApexDossierRumorForSlot(u8 slot)
{
    return slot - 3;
}

static void SetApexDossierSilhouettePalette(u8 spriteId)
{
    u8 paletteNum = IndexOfSpritePaletteTag(SPRITETAG_APEX_SILHOUETTE);

    if (spriteId != MAX_SPRITES && paletteNum != 0xFF)
        gSprites[spriteId].oam.paletteNum = paletteNum;
}

static void ApexLog_MoveApexDossierCursor(u8 taskId, u8 newSlot)
{
    s16 *data = gTasks[taskId].data;
    s16 oldX = 47 * (data[1] % 3) + 0x72;
    s16 oldY = 27 * (data[1] / 3) + 0x2F;
    s16 newX = 47 * (newSlot % 3) + 0x72;
    s16 newY = 27 * (newSlot / 3) + 0x2F;

    data[1] = newSlot;
    ApexLog_MoveSelectorCursor(taskId, newX - oldX, newY - oldY);
}

static void ApexLog_PutWindowTilemapAndCopyWindowToVramMode3_2(u8 windowId)
{
    PutWindowTilemap(windowId);
    CopyWindowToVram(windowId, COPYWIN_FULL);
}

static void ApexLog_CreateScrollIndicatorArrowPair(void)
{
    struct ScrollArrowsTemplate template = {
          2,
          40,
          26,
          3,
          40,
          100,
          0,
          0,
          SPRITETAG_SCROLL_INDICATORS,
          0xFFFF,
          1,
    };

    if (sApexLogData->numUnlockedPersons > 5)
    {
        template.fullyUpThreshold = 0;
        template.fullyDownThreshold = sApexLogData->numUnlockedPersons - 5;
        sApexLogData->scrollIndicatorPairTaskId = AddScrollIndicatorArrowPair(&template, &sApexLogData->listMenuTopIdx);
    }
}

static void FreeListMenuSelectorArrowPairResources(void)
{
    if (sApexLogData->numUnlockedPersons > 5)
        RemoveScrollIndicatorArrowPair(sApexLogData->scrollIndicatorPairTaskId);
}

static u16 ApexLogGetCursorY(void)
{
    u16 listY, cursorY;
    ListMenuGetScrollAndRow(sApexLogData->listMenuTaskId, &listY, &cursorY);
    return listY + cursorY;
}

static void HandleFlavorTextModeSwitch(bool8 state)
{
    if (sApexLogData->viewingFlavorText != state)
    {
        u8 taskId = FindTaskIdByFunc(Task_FCOpenOrCloseInfoBox);
        if (taskId == 0xFF)
            taskId = CreateTask(Task_FCOpenOrCloseInfoBox, 8);
        gTasks[taskId].data[0] = 0;
        gTasks[taskId].data[1] = 4;
        if (state == TRUE)
        {
            gTasks[taskId].data[2] = 1;
            sApexLogData->viewingFlavorText = TRUE;
        }
        else
        {
            gTasks[taskId].data[2] = 4;
            sApexLogData->viewingFlavorText = FALSE;
        }
    }
}

static void Task_FCOpenOrCloseInfoBox(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    switch (task->data[0])
    {
        case 0:
            if (--task->data[1] == 0)
            {
                UpdateInfoBoxTilemap(1, 0);
                task->data[1] = 4;
                task->data[0]++;
            }
            break;
        case 1:
            if (--task->data[1] == 0)
            {
                UpdateInfoBoxTilemap(1, task->data[2]);
                DestroyTask(taskId);
            }
            break;
    }
}

static void UpdateInfoBoxTilemap(u8 bg, s16 state)
{
    u8 left = (sApexLogData != NULL && sApexLogData->isApexDossier) ? ApexLog_APEX_DOSSIER_INFOBOX_LEFT : 14;

    if (state == 0 || state == 3)
    {
        FillBgTilemapBufferRect(bg, 0x8C, left,      10,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0xA1, left +  1, 10, 10,  1, 1);
        FillBgTilemapBufferRect(bg, 0x8D, left + 11, 10,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x8E, left + 12, 10,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x8F, left,      11,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x00, left +  1, 11, 11,  1, 1);
        FillBgTilemapBufferRect(bg, 0x90, left + 12, 11,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x91, left,      12,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0xA3, left +  1, 12, 10,  1, 1);
        FillBgTilemapBufferRect(bg, 0x92, left + 11, 12,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x93, left + 12, 12,  1,  1, 1);
    }
    else if (state == 1)
    {
        FillBgTilemapBufferRect(bg, 0x9B, left,      10,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x9C, left +  1, 10, 11,  1, 1);
        FillBgTilemapBufferRect(bg, 0x96, left + 12, 10,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x9D, left,      11,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x00, left +  1, 11, 11,  1, 1);
        FillBgTilemapBufferRect(bg, 0x90, left + 12, 11,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x9E, left,      12,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x9F, left +  1, 12, 11,  1, 1);
        FillBgTilemapBufferRect(bg, 0x99, left + 12, 12,  1,  1, 1);
    }
    else if (state == 2)
    {
        FillBgTilemapBufferRect(bg, 0x94, left,      10,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x95, left +  1, 10, 11,  1, 1);
        FillBgTilemapBufferRect(bg, 0x96, left + 12, 10,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x8F, left,      11,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x9A, left +  1, 11, 11,  1, 1);
        FillBgTilemapBufferRect(bg, 0x90, left + 12, 11,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x97, left,      12,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x98, left +  1, 12, 11,  1, 1);
        FillBgTilemapBufferRect(bg, 0x99, left + 12, 12,  1,  1, 1);
    }
    else if (state == 4)
    {
        FillBgTilemapBufferRect(bg, 0x83, left,      10,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0xA0, left +  1, 10, 10,  1, 1);
        FillBgTilemapBufferRect(bg, 0x84, left + 11, 10,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x85, left + 12, 10,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x86, left,      11,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0xA2, left +  1, 11, 10,  1, 1);
        FillBgTilemapBufferRect(bg, 0x87, left + 11, 11,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x88, left + 12, 11,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x83, left,      12,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0xA0, left +  1, 12, 10,  1, 1);
        FillBgTilemapBufferRect(bg, 0x84, left + 11, 12,  1,  1, 1);
        FillBgTilemapBufferRect(bg, 0x85, left + 12, 12,  1,  1, 1);
    }
    else if (state == 5)
    {
        FillBgTilemapBufferRect(bg, 0x00, left, 10, 13,  3, 1);
    }
    CopyBgTilemapBufferToVram(bg);
}

static void PlaceListMenuCursor(bool8 isActive)
{
    u16 cursorY = ListMenuGetYCoordForPrintingArrowCursor(sApexLogData->listMenuTaskId);
    if (isActive == TRUE)
        AddTextPrinterParameterized4(FCWINDOWID_LIST, FONT_NORMAL, 0, cursorY, 0, 0, sTextColor_DkGrey, 0, gText_SelectorArrow2);
    else
        AddTextPrinterParameterized4(FCWINDOWID_LIST, FONT_NORMAL, 0, cursorY, 0, 0, sTextColor_White, 0, gText_SelectorArrow2);
}
