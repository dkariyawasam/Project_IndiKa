#include "global.h"
#include "strings.h"
#include "bg.h"
#include "data.h"
#include "decompress.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "item.h"
#include "item_menu.h"
#include "item_menu_icons.h"
#include "list_menu.h"
#include "item_use.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "menu_helpers.h"
#include "palette.h"
#include "party_menu.h"
#include "pokedex.h"
#include "scanline_effect.h"
#include "sound.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "text_window.h"
#include "quests.h"
#include "fame_checker.h"
#include "overworld.h"
#include "event_data.h"
#include "new_menu_helpers.h"
#include "menu_indicators.h"
#include "constants/items.h"
#include "constants/field_weather.h"
#include "constants/flags.h"
#include "constants/quests.h"
#include "constants/vars.h"
#include "constants/songs.h"
#include "constants/rgb.h"
#include "constants/event_objects.h"
#include "event_object_movement.h"
#include "pokemon_icon.h"
#include "ui_hint_header.h"

#include "random.h"

#define tPageItems      data[4]
#define tItemPcParam    data[6]

struct LogbookMenuResources
{
	MainCallback savedCallback;
	u8 moveModeOrigPos;
	u8 spriteIconSlot;
	u16 oldPaletteTag;
	u8 maxShowed;
	u8 nItems;
	u8 scrollIndicatorArrowPairId;
	s16 data[3];
	u8 filterMode;
	u8 parentQuest;
	bool8 restoreCursor;
};

struct LogbookMenuStaticResources
{
	MainCallback savedCallback;
	u16 scroll;
	u16 row;
	u8 initialized;
	u16 storedScrollOffset;
	u16 storedRowPosition;
};

// RAM
EWRAM_DATA static struct LogbookMenuResources *sStateDataPtr = NULL;
EWRAM_DATA static u8 *sBg1TilemapBuffer = NULL;
EWRAM_DATA static struct ListMenuItem *sListMenuItems = NULL;
EWRAM_DATA static struct LogbookMenuStaticResources sListMenuState = {0};
EWRAM_DATA static u8 sItemMenuIconSpriteIds[12] = {0};        // from pokefirered src/item_menu_icons.c
EWRAM_DATA static void *questNamePointer = NULL;
EWRAM_DATA static u8 **questNameArray = NULL;
EWRAM_DATA static u8 sPendingApexDossierSubquest = 0;
EWRAM_DATA static bool8 sReturningFromApexDossier = FALSE;

// This File's Functions
void LogbookMenu_Init(u8 a0, MainCallback callback);
const u8 *GetDynamicSubquestDesc(u8 parentQuest, u8 subquestId);
static void MainCB(void);
static void VBlankCB(void);
static void RunSetup(void);

static bool8 SetupGraphics(void);
static bool8 LoadGraphics(void);
static void LogbookMenu_InitWindows(void);
static bool8 InitBackgrounds(void);
static void InitItems(void);
static bool8 AllocateResourcesForListMenu(void);
static void AllocateMemoryForArray();
static void PlaceTopMenuScrollIndicatorArrows(void);
static void SetInitializedFlag(u8 a0);

static u8 GetCursorPosition(void);
static void SetCursorPosition(void);
static void SetScrollPosition(void);
static bool8 IfScrollIsOutOfBounds(void);
static bool8 IfRowIsOutOfBounds(void);
static void SaveScrollAndRow(s16 *data);

static void ClearModeOnStartup(void);
static u8 ManageMode(u8 action);
static u8 ToggleSubquestMode(u8 mode);
static bool8 IsSubquestMode(void);

static u16 BuildMenuTemplate(void);
static u8 GetModeAndGenerateList();
static u8 CountNumberListRows();
static u8 *DefineQuestOrder();
static u8 GenerateSubquestList();
static u8 GenerateList(void);

static u8 CountUnlockedQuests(void);
static u8 CountCompletedQuests(void);
static u8 CountFavoriteQuests(void);

static void PopulateEmptyRow(u8 countQuest);
static void PrependQuestNumber(u8 countQuest);
static void SetFavoriteQuest(u8 countQuest);
static void PopulateQuestName(u8 countQuest);
static void PopulateSubquestName(u8 parentQuest, u8 countQuest);
static u8 PopulateListRowNameAndId(u8 row, u8 countQuest);
static bool8 DoesQuestHaveChildrenAndNotInactive(u16 itemId);

static void LogbookMenu_AddTextPrinterParameterized(u8 windowId, u8 fontId,
            const u8 *str, u8 x, u8 y, u8 letterSpacing, u8 lineSpacing, u8 speed,
            u8 colorIdx);

static void MoveCursorFunc(s32 itemIndex, bool8 onInit,
                           struct ListMenu *list);
static void PlayCursorSound(bool8 firstRun);
static void FillQuestFooterWindow(void);
static void PrintDetailsForCancel();
static void GenerateAndPrintQuestDetails(s32 questId);
static void GenerateQuestFlavorText(s32 questId);
static void UpdateQuestFlavorText(s32 questId);
static void PrintQuestFlavorText(s32 questId);

static bool8 IsQuestUnlocked(s32 questId);
static bool8 IsQuestActiveState(s32 questId);
static bool8 IsQuestInactiveState(s32 questId);
static bool8 IsQuestRewardState(s32 questId);
static bool8 IsQuestCompletedState(s32 questId);
static bool8 IsSubquestUnlockedState(s32 questId);

static void DetermineSpriteType(s32 questId);
static void LogbookMenu_CreateSprite(u16 itemId, u8 idx, u8 spriteType);
static void ResetSpriteState(void);
static void LogbookMenu_DestroySprite(u8 idx);

static void GenerateStateAndPrint(u8 windowId, u32 itemId, u8 y);
static bool8 TryGenerateNatureSubquestCounter(u8 subquestId, u8 *colorIndex);
static bool8 TryGenerateGymTrialState(u8 subquestId, u8 *colorIndex);
static u8 GenerateSubquestState(u8 questId);
static u8 GenerateQuestState(u8 questId);
static void PrintQuestState(u8 windowId, u8 y, u8 colorIndex);

static void GenerateAndPrintHeader(void);
static void GenerateDenominatorNumQuests(void);
static void GenerateNumeratorNumQuests(void);
static void GenerateMenuContext(void);
static void PrintMenuContext(bool8 selectedRowAcceptsA);
static bool8 DoesSelectedRowAcceptA(s32 questId);

static void Task_Main(u8 taskId);
static u8 ManageFavorites(u8 index);
static void Task_LogbookMenuCleanUp(u8 taskId);
static void RestoreSavedScrollAndRow(s16 *data);
static void ResetCursorToTop(s16 *data);
static void LogbookMenu_RemoveScrollIndicatorArrowPair(void);
static void EnterSubquestModeAndCleanUp(u8 taskId, s16 *data, s32 input);
static void ToggleFavoriteAndCleanUp(u8 taskId, u8 selectedQuestId);
static bool8 CheckSelectedIsCancel(u8 selectedQuestId);
static void ReturnFromSubquestAndCleanUp(u8 taskId);

static void SetGpuRegBaseForFade(void);
static void InitFadeVariables(u8 taskId, u8 blendWeight, u8 frameDelay,
                              u8 frameTimerBase, u8 delta);
static void PrepareFadeOut(u8 taskId);
static bool8 HandleFadeOut(u8 taskId);
static void PrepareFadeIn(u8 taskId);
static bool8 HandleFadeIn(u8 taskId);
static void Task_FadeOut(u8 taskId);
static void Task_FadeIn(u8 taskId);

static void Task_LogbookMenuWaitFadeAndBail(u8 taskId);
static void FadeAndBail(void);
static void FreeResources(void);
static void TurnOffLogbookMenu(u8 taskId);
static void OpenApexDossierAndCleanUp(u8 taskId, u8 subquestId);
static void CB2_OpenApexRumorDossier(void);
static void CB2_ReturnToApexSubquestMenu(void);
static void Task_LogbookMenuTurnOff1(u8 taskId);
static void Task_LogbookMenuTurnOff2(u8 taskId);

// Tiles, palettes and tilemaps for the Logbook Menu
static const u32 sLogbookMenuTiles[] =
        INCBIN_U32("graphics/logbook_menu/menu.4bpp.lz");
static const u32 sLogbookMenuBgPals[] =
        INCBIN_U32("graphics/item_menu/bg.gbapal.lz");
static const u32 sLogbookMenuTilemap[] =
        INCBIN_U32("graphics/logbook_menu/menu.bin.lz");
static const u16 sLogbookMenuWindowPal[] =
        INCBIN_U16("graphics/item_menu/bag_window_pal.gbapal");

#define LOGBOOK_MENU_FOOTER_BG_COLOR 15
#define LOGBOOK_MENU_FOOTER_BORDER_DARK_COLOR 13
#define LOGBOOK_MENU_FOOTER_BORDER_LIGHT_COLOR 12
#define LOGBOOK_MENU_FOOTER_BOTTOM_MID_COLOR 14
#define LOGBOOK_MENU_FOOTER_TEXT_COLOR 10
#define LOGBOOK_MENU_FOOTER_SHADOW_COLOR 13

//Strings used for the Logbook Menu
static const u8 sText_Empty[] = _("");
static const u8 sText_AllHeader[] = _("ALL MISSIONS");
static const u8 sText_QuestNumberDisplay[] =
      _("{STR_VAR_1}/{STR_VAR_2}");
static const u8 sText_Unk[] = _("??????");
static const u8 sText_Active[] = _("Active");
static const u8 sText_Reward[] = _("Reward");
static const u8 sText_Complete[] = _("Done");
static const u8 sText_StartForMore[] =
      _("Start for more details.");
static const u8 sText_ReturnRecieveReward[] =
      _("Return to {STR_VAR_2}\nto recieve your reward!");
static const u8 sText_PickBack[] = _("{DPAD_UPDOWN}PICK {B_BUTTON}BACK");
static const u8 sText_Caught[] = _("Caught");
static const u8 sText_Found[] = _("Found");
static const u8 sText_Read[] = _("Read");
static const u8 sText_DotSpace[] = _(". ");
static const u8 sText_CloseLogbook[] = _("CLOSE LOGBOOK");
static const u8 sText_ColorGreen[] = _("{COLOR}{GREEN}");
static const u8 sText_InProgress[] = _("Active");
static const u8 sText_InProgressCaps[] = _("In progress");
static const u8 sText_CompletedCaps[] = _("Done");
static const u8 sText_TradedCaps[] = _("Traded");
static const u8 sText_ApexAnnihilapeRumor[] = _("There are rumours of a POKéMON\nlosing control of its emotions.");
static const u8 sText_ApexAnnihilapeConfirmed[] = _("Rumour confirmed.\nLet's investigate MT. MOON.");
static const u8 sText_ApexAnnihilapeRecorded[] = _("APEX POKéMON ANNIHILAPE\nrecorded in MT. MOON.");
static const u8 sText_ApexMimeSrRumor[] = _("There are rumours of an isolated\nMR. MIME community.");
static const u8 sText_ApexMimeSrConfirmed[] = _("Rumour confirmed.\nLet's investigate DIGLETT CAVE.");
static const u8 sText_ApexMimeSrRecorded[] = _("APEX POKéMON MIME SR.\nrecorded in DIGLETT CAVE.");
static const u8 sText_ApexArticunoRumor[] = _("There are rumours of sudden\nchange at SEAFOAM ISLANDS.");
static const u8 sText_ApexArticunoConfirmed[] = _("Rumour confirmed.\nLet's investigate SEAFOAM.");
static const u8 sText_ApexArticunoRecorded[] = _("APEX POKéMON ARTICUNO\nrecorded in SEAFOAM ISLANDS.");
static const u8 sText_ApexOsscytheRumor[] = _("There are rumours of a spirit\nmourning its lost family.");
static const u8 sText_ApexOsscytheConfirmed[] = _("Rumour confirmed.\nLet's investigate POKéMON TOWER.");
static const u8 sText_ApexOsscytheRecorded[] = _("APEX POKéMON OSSCYTHE\nrecorded in POKéMON TOWER.");
static const u8 sText_ApexTangrowthRumor[] = _("There are old legends about\nVIRIDIAN FOREST coming alive.");
static const u8 sText_ApexTangrowthConfirmed[] = _("Rumour confirmed.\nLet's investigate VIRIDIAN FOREST.");
static const u8 sText_ApexTangrowthRecorded[] = _("APEX POKéMON TANGROWTH\nrecorded in VIRIDIAN FOREST.");
static const u8 sText_ApexZapdosRumor[] = _("There are rumours of endless\nnatural power at the POWER PLANT.");
static const u8 sText_ApexZapdosConfirmed[] = _("Rumour confirmed.\nLet's investigate the POWER PLANT.");
static const u8 sText_ApexZapdosRecorded[] = _("APEX POKéMON ZAPDOS\nrecorded in the POWER PLANT.");
static const u8 sText_ApexMoltresRumor[] = _("There are rumours of renewing\nenergy inside CINNABAR VOLCANO.");
static const u8 sText_ApexMoltresConfirmed[] = _("Rumour confirmed.\nLet's investigate CINNABAR VOLCANO.");
static const u8 sText_ApexMoltresRecorded[] = _("APEX POKéMON MOLTRES\nrecorded in CINNABAR VOLCANO.");

///////////////////////////////////////////////////////////////////////////////
//////////////////////BEGIN SUBQUEST CUSTOMIZATION/////////////////////////////

//Declaration of subquest structures. Edits to subquests are made here.
#define sub_quest(i, n, d, m, s, st, t) {.id = i, .name = n, .desc = d, .map = m, .sprite = s, .spritetype = st, .type = t}
static const struct SubQuest sSubQuests1[QUEST_1_SUB_COUNT] =
{
	sub_quest(
	      0,
	      gText_SubQuest1_Name1,
	      gText_SubQuest1_Desc1,
	      gText_QuestTheNatureOfEvolutionMap1,
	      OBJ_EVENT_GFX_PROF_OAK,
	      OBJECT,
	      sText_Found
	),

	sub_quest(
	      1,
	      gText_SubQuest1_Name2,
	      gText_SubQuest1_Desc2,
	      gText_QuestTheNatureOfEvolutionMap1,
	      OBJ_EVENT_GFX_PROF_OAK,
	      OBJECT,
	      sText_Found
	),

	sub_quest(
	      2,
	      gText_SubQuest1_Name3,
	      gText_SubQuest1_Desc3,
	      gText_QuestTheNatureOfEvolutionMap1,
	      OBJ_EVENT_GFX_PROF_OAK,
	      OBJECT,
	      sText_Found
	),
};

static const struct SubQuest sSubQuests2[QUEST_2_SUB_COUNT] =
{
	sub_quest(
	      3,
	      gText_SubQuest2_Name1,
	      gText_SubQuest2_Desc1,
	      gText_QuestTheNatureOfEvolutionMap1,
	      OBJ_EVENT_GFX_BROCK,
	      OBJECT,
	      sText_Found
	),

	sub_quest(
	      4,
	      gText_SubQuest2_Name2,
	      gText_SubQuest2_Desc2,
	      gText_SideQuestMap2,
	      OBJ_EVENT_GFX_MISTY,
	      OBJECT,
	      sText_Found
	),

	sub_quest(
	      5,
	      gText_SubQuest2_Name3,
	      gText_SubQuest2_Desc3,
	      gText_SideQuestMap3,
	      OBJ_EVENT_GFX_LT_SURGE,
	      OBJECT,
	      sText_Found
	),

	sub_quest(
	      6,
	      gText_SubQuest2_Name4,
	      gText_SubQuest2_Desc4,
	      gText_SideQuestMap4,
	      OBJ_EVENT_GFX_ERIKA,
	      OBJECT,
	      sText_Found
	),

	sub_quest(
	      7,
	      gText_SubQuest2_Name5,
	      gText_SubQuest2_Desc5,
	      gText_SideQuestMap5,
	      OBJ_EVENT_GFX_KOGA,
	      OBJECT,
	      sText_Found
	),

	sub_quest(
	      8,
	      gText_SubQuest2_Name6,
	      gText_SubQuest2_Desc6,
	      gText_SideQuestMap6,
	      OBJ_EVENT_GFX_SABRINA,
	      OBJECT,
	      sText_Found
	),

	sub_quest(
	      9,
	      gText_SubQuest2_Name7,
	      gText_SubQuest2_Desc7,
	      gText_SideQuestMap7,
	      OBJ_EVENT_GFX_BLAINE,
	      OBJECT,
	      sText_Found
	),

	sub_quest(
	      10,
	      gText_SubQuest2_Name8,
	      gText_SubQuest2_Desc8,
	      gText_SideQuestMap8,
	      OBJ_EVENT_GFX_GIOVANNI,
	      OBJECT,
	      sText_Found
	)
};

static const struct SubQuest sSubQuests3[QUEST_3_SUB_COUNT] =
{
	sub_quest(
	      11,
	      gText_SubQuest3_Name1,
	      gText_SubQuest3_Desc1,
	      gText_SubQuest3_Map1,
	      SPECIES_TANGROWTH,
	      PKMN,
	      sText_Found
	),

	sub_quest(
	      12,
	      gText_SubQuest3_Name2,
	      gText_SubQuest3_Desc2,
	      gText_SubQuest3_Map2,
	      SPECIES_ZAPDOS,
	      PKMN,
	      sText_Found
	),

	sub_quest(
	      13,
	      gText_SubQuest3_Name3,
	      gText_SubQuest3_Desc3,
	      gText_SubQuest3_Map3,
	      SPECIES_ARTICUNO,
	      PKMN,
	      sText_Found
	),

	sub_quest(
	      14,
	      gText_SubQuest3_Name4,
	      gText_SubQuest3_Desc4,
	      gText_SubQuest3_Map4,
	      SPECIES_MEWTWO,
	      PKMN,
	      sText_Found
	),

	sub_quest(
	      15,
	      gText_SubQuest3_Name5,
	      gText_SubQuest3_Desc5,
	      gText_SubQuest3_Map5,
	      SPECIES_OSSCYTHE,
	      PKMN,
	      sText_Found
	),

	sub_quest(
	      16,
	      gText_SubQuest3_Name6,
	      gText_SubQuest3_Desc6,
	      gText_SubQuest3_Map6,
	      SPECIES_MOLTRES,
	      PKMN,
	      sText_Found
	),

	sub_quest(
	      17,
	      gText_SubQuest3_Name7,
	      gText_SubQuest3_Desc7,
	      gText_SubQuest3_Map7,
	      SPECIES_MIME_SR,
	      PKMN,
	      sText_Found
	),

	sub_quest(
	      18,
	      gText_SubQuest3_Name8,
	      gText_SubQuest3_Desc8,
	      gText_SubQuest3_Map8,
	      SPECIES_ANNIHILAPE,
	      PKMN,
	      sText_Found
	)
};

////////////////////////END SUBQUEST CUSTOMIZATION/////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
////////////////////////BEGIN QUEST CUSTOMIZATION//////////////////////////////

//Declaration of side quest structures. Edits to quests are made here.
#define side_quest(n, d, dd, m, s, st, sq, ns) {.name = n, .desc = d, .donedesc = dd, .map = m, .sprite = s, .spritetype = st, .subquests = sq, .numSubquests = ns}
static const struct SideQuest sSideQuests[QUEST_COUNT] =
{
	side_quest(
	      gText_QuestTheNatureOfEvolutionName_1,
	      gText_QuestTheNatureOfEvolutionDesc_1,
	      gText_QuestTheNatureOfEvolutionDoneDesc_1,
	      gText_QuestTheNatureOfEvolutionMap1,
	      OBJ_EVENT_GFX_PROF_OAK,
	      OBJECT,
		  sSubQuests1,
	      QUEST_1_SUB_COUNT
	),
	side_quest(
	      gText_SideQuestName_2,
	      gText_SideQuestDesc_2,
	      gText_SideQuestDoneDesc_2,
	      gText_SideQuestMap2,
	      OBJ_EVENT_GFX_BLUE,
	      OBJECT,
	      sSubQuests2,
	      QUEST_2_SUB_COUNT
	),
	side_quest(
	      gText_SideQuestName_3,
	      gText_SideQuestDesc_3,
	      gText_SideQuestDoneDesc_3,
	      gText_SideQuestMap3,
	      SPECIES_TANGROWTH,
	      PKMN,
	      sSubQuests3,
	      QUEST_3_SUB_COUNT
	),
	side_quest(
	      gText_SideQuestName_4,
	      gText_SideQuestDesc_4,
	      gText_SideQuestDoneDesc_4,
	      gText_SideQuestMap4,
	      OBJ_EVENT_GFX_BLUE,
	      OBJECT,
	      NULL,
	      0
	),
	side_quest(
	      gText_SideQuestName_5,
	      gText_SideQuestDesc_5,
	      gText_SideQuestDoneDesc_5,
	      gText_SideQuestMap5,
	      OBJ_EVENT_GFX_BLUE,
	      OBJECT,
	      NULL,
	      0
	),
};

#define APEX_RUMORS_REQUIRED 3
#define APEX_RUMOR_RESULT_NONE 0
#define APEX_RUMOR_RESULT_RECORDED 1
#define APEX_RUMOR_RESULT_UPDATED 2

static u8 GetApexRumorCount(u8 apexSubquest)
{
    u8 i;
    u8 count = 0;

    for (i = 0; i < APEX_RUMORS_REQUIRED; i++)
    {
        u8 bit = apexSubquest * APEX_RUMORS_REQUIRED + i;
        u16 var = bit < 16 ? VAR_APEX_RUMOR_BITS_1 : VAR_APEX_RUMOR_BITS_2;
        u16 mask = 1 << (bit % 16);

        if (VarGet(var) & mask)
            count++;
    }

    return count;
}

bool8 LogbookMenu_HasHeardApexRumor(u8 apexSubquest, u8 rumor)
{
    u8 bit;
    u16 var;
    u16 mask;

    if (apexSubquest >= QUEST_3_SUB_COUNT || rumor >= APEX_RUMORS_REQUIRED)
        return FALSE;

    bit = apexSubquest * APEX_RUMORS_REQUIRED + rumor;
    var = bit < 16 ? VAR_APEX_RUMOR_BITS_1 : VAR_APEX_RUMOR_BITS_2;
    mask = 1 << (bit % 16);

    return (VarGet(var) & mask) != 0;
}

void RecordApexRumor(void)
{
    u8 apexSubquest = VarGet(VAR_0x8004);
    u8 rumor = VarGet(VAR_0x8005);
    u8 oldCount;
    u8 bit;
    u16 var;
    u16 mask;

    gSpecialVar_Result = APEX_RUMOR_RESULT_NONE;

    if (apexSubquest >= QUEST_3_SUB_COUNT || rumor >= APEX_RUMORS_REQUIRED)
        return;

    bit = apexSubquest * APEX_RUMORS_REQUIRED + rumor;
    var = bit < 16 ? VAR_APEX_RUMOR_BITS_1 : VAR_APEX_RUMOR_BITS_2;
    mask = 1 << (bit % 16);

    if (VarGet(var) & mask)
        return;

    oldCount = GetApexRumorCount(apexSubquest);
    VarSet(var, VarGet(var) | mask);
    LogbookMenu_GetSetQuestState(QUEST_APEX_POKEMON, FLAG_SET_UNLOCKED);
    if (!LogbookMenu_GetSetQuestState(QUEST_APEX_POKEMON, FLAG_GET_COMPLETED))
        LogbookMenu_GetSetQuestState(QUEST_APEX_POKEMON, FLAG_SET_ACTIVE);
    LogbookMenu_GetSetSubquestState(QUEST_APEX_POKEMON, FLAG_SET_UNLOCKED, apexSubquest);
    gSpecialVar_Result = oldCount == 0 ? APEX_RUMOR_RESULT_RECORDED : APEX_RUMOR_RESULT_UPDATED;
}

u16 IsApexRevealed(void)
{
    u8 apexSubquest = VarGet(VAR_0x8004);

    if (apexSubquest >= QUEST_3_SUB_COUNT)
        return FALSE;

    return GetApexRumorCount(apexSubquest) >= APEX_RUMORS_REQUIRED;
}

void TryCompleteApexInstinctQuest(void)
{
    static const struct
    {
        u16 flag;
        u8 subquest;
    } sApexQuestProgress[] =
    {
        {FLAG_INTERACTED_APEX_TANGROWTH, SUB_QUEST_APEX_TANGROWTH},
        {FLAG_INTERACTED_APEX_ZAPDOS, SUB_QUEST_APEX_ZAPDOS},
        {FLAG_INTERACTED_APEX_ARTICUNO, SUB_QUEST_APEX_ARTICUNO},
        {FLAG_INTERACTED_APEX_MEWTWO, SUB_QUEST_APEX_MEWTWO},
        {FLAG_INTERACTED_APEX_OSSCYTHE, SUB_QUEST_APEX_OSSCYTHE},
        {FLAG_INTERACTED_APEX_MOLTRES, SUB_QUEST_APEX_MOLTRES},
        {FLAG_INTERACTED_APEX_MIME_SR, SUB_QUEST_APEX_MIME_SR},
        {FLAG_INTERACTED_APEX_ANNIHILAPE, SUB_QUEST_APEX_ANNIHILAPE},
    };
    u8 i;
    u8 count = 0;

    for (i = 0; i < NELEMS(sApexQuestProgress); i++)
    {
        if (FlagGet(sApexQuestProgress[i].flag))
        {
            count++;
            LogbookMenu_GetSetQuestState(QUEST_APEX_POKEMON, FLAG_SET_UNLOCKED);
            LogbookMenu_GetSetQuestState(QUEST_APEX_POKEMON, FLAG_SET_ACTIVE);
            LogbookMenu_GetSetSubquestState(QUEST_APEX_POKEMON, FLAG_SET_UNLOCKED, sApexQuestProgress[i].subquest);
            LogbookMenu_GetSetSubquestState(QUEST_APEX_POKEMON, FLAG_SET_COMPLETED, sApexQuestProgress[i].subquest);
        }
    }

    if (count >= 2 && !LogbookMenu_GetSetSubquestState(QUEST_THE_NATURE_OF_EVOLUTION, FLAG_GET_COMPLETED, SUB_QUEST_EVOLUTION_THROUGH_INSTINCT))
        LogbookMenu_GetSetSubquestState(QUEST_THE_NATURE_OF_EVOLUTION, FLAG_SET_COMPLETED, SUB_QUEST_EVOLUTION_THROUGH_INSTINCT);

    if (count >= QUEST_3_SUB_COUNT)
    {
        LogbookMenu_GetSetQuestState(QUEST_APEX_POKEMON, FLAG_SET_COMPLETED);
        LogbookMenu_GetSetQuestState(QUEST_APEX_POKEMON, FLAG_REMOVE_ACTIVE);
    }
}

////////////////////////END QUEST CUSTOMIZATION////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//BG layer defintions
static const struct BgTemplate sLogbookMenuBgTemplates[2] =
{
	{
		//All text and content is loaded to this window
		.bg = 0,
		.charBaseIndex = 0,
		.mapBaseIndex = 31,
		.priority = 1
	},
	{
		///Backgrounds and UI elements are loaded to this window
		.bg = 1,
		.charBaseIndex = 3,
		.mapBaseIndex = 30,
		.priority = 2
	}
};

//Window definitions
static const struct WindowTemplate sLogbookMenuHeaderWindowTemplates[] =
{
	{
		//0: Content window
		.bg = 0,
		.tilemapLeft = 0,
		.tilemapTop = 3,
		.width = 30,
		.height = 10,
		.paletteNum = 15,
		.baseBlock = 1
	},
	{
		//1: Footer window
		.bg = 0,
		.tilemapLeft = 0,
		.tilemapTop = 14,
		.width = 30,
		.height = 6,
		.paletteNum = 1,
		.baseBlock = 361
	},
	{
		// 2: Header window
		.bg = 0,
		.tilemapLeft = 0,
		.tilemapTop = 0,
		.width = 30,
		.height = 2,
		.paletteNum = 14,
		.baseBlock = 721
	},
	DUMMY_WIN_TEMPLATE
};

//Font color combinations for printed text
static const u8 sLogbookMenuWindowFontColors[][4] =
{
	{
		//Header of Logbook Menu
		TEXT_COLOR_TRANSPARENT,
		TEXT_COLOR_WHITE,
		TEXT_COLOR_DARK_GRAY
	},
	{
		//Reward state progress indicator
		TEXT_COLOR_TRANSPARENT,
		TEXT_COLOR_RED,
		TEXT_COLOR_LIGHT_RED
	},
	{
		//Done state progress indicator
		TEXT_COLOR_TRANSPARENT,
		TEXT_COLOR_GREEN,
		TEXT_COLOR_LIGHT_GREEN
	},
	{
		//Active state progress indicator
		TEXT_COLOR_TRANSPARENT,
		TEXT_COLOR_BLUE,
		TEXT_COLOR_LIGHT_BLUE
	},
	{
		//Footer flavor text
		TEXT_COLOR_TRANSPARENT,
		LOGBOOK_MENU_FOOTER_TEXT_COLOR,
		LOGBOOK_MENU_FOOTER_SHADOW_COLOR
	},
};

//Functions begin here

//ported from firered by ghoulslash
void LogbookMenu_Init(u8 a0, MainCallback callback)
{
	u8 i;

	if (a0 >= 2)
	{
		SetMainCallback2(callback);
		return;
	}

	if ((sStateDataPtr = Alloc(sizeof(struct LogbookMenuResources))) == NULL)
	{
		SetMainCallback2(callback);
		return;
	}

	if (a0 != 1)
	{
		sListMenuState.savedCallback = callback;
		sListMenuState.scroll = sListMenuState.row = 0;
	}

	sStateDataPtr->moveModeOrigPos = 0xFF;
	sStateDataPtr->spriteIconSlot = 0;
	sStateDataPtr->scrollIndicatorArrowPairId = 0xFF;
	sStateDataPtr->savedCallback = 0;
	for (i = 0; i < 3; i++)
	{
		sStateDataPtr->data[i] = 0;
	}

	SetMainCallback2(RunSetup);
}

static void MainCB(void)
{
	RunTasks();
	AnimateSprites();
	BuildOamBuffer();
	DoScheduledBgTilemapCopiesToVram();
	UpdatePaletteFade();
}

static void VBlankCB(void)
{
	LoadOam();
	ProcessSpriteCopyRequests();
	TransferPlttBuffer();
}

static void RunSetup(void)
{
	while (1)
	{
		if (SetupGraphics() == TRUE)
		{
			break;
		}
	}
}

static bool8 SetupGraphics(void)
{
	u8 taskId;
	switch (gMain.state)
	{
		case 0:
			SetVBlankHBlankCallbacksToNull();
			ClearScheduledBgCopiesToVram();
			gMain.state++;
			break;
		case 1:
			ScanlineEffect_Stop();
			gMain.state++;
			break;
		case 2:
			FreeAllSpritePalettes();
			gMain.state++;
			break;
		case 3:
			ResetPaletteFade();
			gMain.state++;
			break;
		case 4:
			ResetSpriteData();
			gMain.state++;
			break;
		case 5:
			ResetSpriteState();
			gMain.state++;
			break;
		case 6:
			ResetTasks();
			gMain.state++;
			break;
		case 7:
			if (InitBackgrounds())
			{
				sStateDataPtr->data[0] = 0;
				gMain.state++;
			}
			else
			{
				FadeAndBail();
				return TRUE;
			}
			break;
		case 8:
			if (LoadGraphics() == TRUE)
			{
				gMain.state++;
			}
			break;
		case 9:
			LogbookMenu_InitWindows();
			gMain.state++;
			break;
		case 10:
			ClearModeOnStartup();
			InitItems();
			SetCursorPosition();
			SetScrollPosition();
			gMain.state++;
			break;
		case 11:
			if (AllocateResourcesForListMenu())
			{
				gMain.state++;
			}
			else
			{
				FadeAndBail();
				return TRUE;
			}
			break;
		case 12:
			AllocateMemoryForArray();
			BuildMenuTemplate();
			gMain.state++;
			break;
		case 13:
			GenerateAndPrintHeader();
			gMain.state++;
			break;
		case 14:
			gMain.state++;
			break;
		case 15:
			taskId = CreateTask(Task_Main, 0);
			gTasks[taskId].data[0] = ListMenuInit(&gMultiuseListMenuTemplate,
			                                      sListMenuState.scroll,
			                                      sListMenuState.row);
			gMain.state++;
			break;
		case 16:
			PlaceTopMenuScrollIndicatorArrows();
			gMain.state++;
			break;
		case 17:
			gMain.state++;
			break;
		case 18:
			if (sListMenuState.initialized == 1)
			{
				BlendPalettes(0xFFFFFFFF, 16, RGB_BLACK);
			}
			gMain.state++;
			break;
		case 19:
			if (sListMenuState.initialized == 1)
			{
				BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
			}
			else
			{

				BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
				SetInitializedFlag(1);
			}
			gMain.state++;
			break;
		default:
			SetVBlankCallback(VBlankCB);
			SetMainCallback2(MainCB);
			return TRUE;
	}
	return FALSE;
}

static bool8 LoadGraphics(void)
{
	switch (sStateDataPtr->data[0])
	{
		case 0:
			ResetTempTileDataBuffers();
			DecompressAndCopyTileDataToVram(1, sLogbookMenuTiles, 0, 0, 0);
			sStateDataPtr->data[0]++;
			break;
		case 1:
			if (FreeTempTileDataBuffersIfPossible() != TRUE)
			{
				LZDecompressWram(sLogbookMenuTilemap, sBg1TilemapBuffer);
				sStateDataPtr->data[0]++;
			}
			break;
		case 2:
			LoadCompressedPalette(sLogbookMenuBgPals, 0x00, 0x60);
			sStateDataPtr->data[0]++;
			break;
		case 3:
			LoadPalette(sLogbookMenuWindowPal, BG_PLTT_ID(15), PLTT_SIZE_4BPP);
			LoadPalette(gUiHintHeaderPalette, BG_PLTT_ID(14), PLTT_SIZE_4BPP);
			sStateDataPtr->data[0]++;
			break;
		default:
			sStateDataPtr->data[0] = 0;
			return TRUE;
	}
	return FALSE;
}

static void LogbookMenu_InitWindows(void)
{
	u8 i;

	InitWindows(sLogbookMenuHeaderWindowTemplates);
	DeactivateAllTextPrinters();

	for (i = 0; i < 3; i++)
	{
		FillWindowPixelBuffer(i, 0x00);
		PutWindowTilemap(i);
	}

	ScheduleBgCopyTilemapToVram(0);
}

static bool8 InitBackgrounds(void)
{
	ResetAllBgsCoordinatesAndBgCntRegs();
	sBg1TilemapBuffer = Alloc(0x800);
	if (sBg1TilemapBuffer == NULL)
	{
		return FALSE;
	}

	memset(sBg1TilemapBuffer, 0, 0x800);
	ResetBgsAndClearDma3BusyFlags(0);
	InitBgsFromTemplates(0, sLogbookMenuBgTemplates,
	                     NELEMS(sLogbookMenuBgTemplates));
	SetBgTilemapBuffer(1, sBg1TilemapBuffer);
	ScheduleBgCopyTilemapToVram(1);
	SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
	SetGpuReg(REG_OFFSET_BLDCNT, 0);
	ShowBg(0);
	ShowBg(1);
	return TRUE;
}

static void InitItems(void)
{
	sStateDataPtr->nItems = (CountNumberListRows()) - 1;

	sStateDataPtr->maxShowed = sStateDataPtr->nItems + 1 <= 5 ?
	                           sStateDataPtr->nItems + 1 : 5;
}

#define try_alloc(ptr__, size) ({ \
		void ** ptr = (void **)&(ptr__);             \
		*ptr = Alloc(size);                 \
		if (*ptr == NULL)                   \
		{                                   \
			FreeResources();                  \
			FadeAndBail();                  \
			return FALSE;                   \
		}                                   \
	})

static bool8 AllocateResourcesForListMenu(void)
{
	try_alloc(sListMenuItems,
	          sizeof(struct ListMenuItem) * CountNumberListRows() + 1);
	return TRUE;
}

void AllocateMemoryForArray(void)
{
	u8 i;
	u8 allocateRows = QUEST_ARRAY_COUNT + 1;

	questNameArray = Alloc(sizeof(void *) * allocateRows);

	for (i = 0; i < allocateRows; i++)
	{
		questNameArray[i] = Alloc(sizeof(u8) * 32);
	}
}

static void PlaceTopMenuScrollIndicatorArrows(void)
{
	u8 listSize = CountNumberListRows();

	if (listSize < sStateDataPtr->maxShowed)
	{
		listSize = sStateDataPtr->maxShowed;
	}

	sStateDataPtr->scrollIndicatorArrowPairId =
	      AddScrollIndicatorArrowPairParameterized(2, 94, 25, 107,
	                  (listSize - sStateDataPtr->maxShowed), 110, 110, &sListMenuState.scroll);
}

static void SetInitializedFlag(u8 a0)
{
	sListMenuState.initialized = a0;
}

static u8 GetCursorPosition(void)
{
	return sListMenuState.scroll + sListMenuState.row;
}

static void SetCursorPosition(void)
{
	if (IfScrollIsOutOfBounds())
	{
		sListMenuState.scroll = (sStateDataPtr->nItems + 1) -
		                        sStateDataPtr->maxShowed;
	}

	if (IfRowIsOutOfBounds())
	{
		if (sStateDataPtr->nItems + 1 < 2)
		{
			sListMenuState.row = 0;
		}
		else
		{
			sListMenuState.row = sStateDataPtr->nItems;
		}
	}
}


static void SetScrollPosition(void)
{
	u8 i;

	if (sListMenuState.row > 4)
	{
		for (i = 0; i <= sListMenuState.row - 4;
		            sListMenuState.row--, sListMenuState.scroll++, i++)
		{
			if (sListMenuState.scroll + sStateDataPtr->maxShowed ==
			            sStateDataPtr->nItems + 1)
			{
				break;
			}
		}
	}
}

bool8 IfScrollIsOutOfBounds(void)
{
	if (sListMenuState.scroll != 0
	            && sListMenuState.scroll + sStateDataPtr->maxShowed >
	            sStateDataPtr->nItems + 1)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool8 IfRowIsOutOfBounds(void)
{
	if (sListMenuState.scroll + sListMenuState.row >= sStateDataPtr->nItems +
	            1)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

static void SaveScrollAndRow(s16 *data)
{
	ListMenuGetScrollAndRow(data[0], &sListMenuState.storedScrollOffset,
	                        &sListMenuState.storedRowPosition);
}


void ClearModeOnStartup(void)
{
	if (sReturningFromApexDossier)
	{
		sStateDataPtr->filterMode = SORT_SUBQUEST;
		sStateDataPtr->parentQuest = QUEST_APEX_POKEMON;
		sReturningFromApexDossier = FALSE;
	}
	else
	{
		sStateDataPtr->filterMode = 0;
	}
}

static u8 ManageMode(u8 action)
{
	u8 mode = sStateDataPtr->filterMode;

	if (action == SUB)
		mode = ToggleSubquestMode(mode);

	return mode;
}

u8 ToggleSubquestMode(u8 mode)
{
	if (IsSubquestMode())
	{
		mode -= SORT_SUBQUEST;
		sStateDataPtr->restoreCursor = TRUE;
	}
	else
	{
		mode += SORT_SUBQUEST;
		sStateDataPtr->restoreCursor = FALSE;
	}

	return mode;
}

static bool8 IsSubquestMode(void)
{
	if (sStateDataPtr->filterMode >= SORT_SUBQUEST)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

static u16 BuildMenuTemplate(void)
{
	GetModeAndGenerateList();

	gMultiuseListMenuTemplate.totalItems = CountNumberListRows();
	gMultiuseListMenuTemplate.items = sListMenuItems;
	gMultiuseListMenuTemplate.windowId = 0;
	gMultiuseListMenuTemplate.header_X = 0;
	gMultiuseListMenuTemplate.cursor_X = 15;
	gMultiuseListMenuTemplate.item_X = 23;
	gMultiuseListMenuTemplate.lettersSpacing = 1;
	gMultiuseListMenuTemplate.itemVerticalPadding = 2;
	gMultiuseListMenuTemplate.upText_Y = 3;
	gMultiuseListMenuTemplate.maxShowed = sStateDataPtr->maxShowed;
	gMultiuseListMenuTemplate.fontId = FONT_NORMAL;
	gMultiuseListMenuTemplate.cursorPal = TEXT_COLOR_DARK_GRAY;
	gMultiuseListMenuTemplate.fillValue = TEXT_COLOR_TRANSPARENT;
	gMultiuseListMenuTemplate.cursorShadowPal = TEXT_COLOR_LIGHT_GRAY;
	gMultiuseListMenuTemplate.moveCursorFunc = MoveCursorFunc;
	gMultiuseListMenuTemplate.itemPrintFunc = GenerateStateAndPrint;
	gMultiuseListMenuTemplate.scrollMultiple = LIST_MULTIPLE_SCROLL_DPAD;
	gMultiuseListMenuTemplate.cursorKind = 0;
}

u8 GetModeAndGenerateList()
{
	if (IsSubquestMode())
	{
		return GenerateSubquestList();
	}
	else
	{
		return GenerateList();
	}
}

static u8 CountNumberListRows()
{
	if (IsSubquestMode())
	{
		return sSideQuests[sStateDataPtr->parentQuest].numSubquests;
	}

	return QUEST_COUNT;
}

u8 *DefineQuestOrder()
{
	static u8 sortedList[QUEST_COUNT];
	u8 a;

	for (a = 0; a < QUEST_COUNT; a++)
	{
		sortedList[a] = a;
	}

	return sortedList;
}

u8 GenerateSubquestList()
{
	u8 parentQuest = sStateDataPtr->parentQuest;
	u8 lastRow = 0, numRow = 0, countQuest = 0;

	for (numRow = 0; numRow < sSideQuests[parentQuest].numSubquests; numRow++)
	{
		PopulateEmptyRow(countQuest);
		PopulateSubquestName(parentQuest, countQuest);
		PopulateListRowNameAndId(numRow, countQuest);

		countQuest++;
		lastRow = numRow + 1;
	}
	return lastRow;
}

u8 GenerateList(void)
{
	u8 lastRow = 0, numRow = 0, offset = 0, newRow = 0, countQuest = 0,
	   selectedQuestId = 0;
	u8 *sortedQuestList;

	sortedQuestList = DefineQuestOrder();

	for (countQuest = 0; countQuest < QUEST_COUNT; countQuest++)
	{
		selectedQuestId = *(sortedQuestList + countQuest);

		PopulateEmptyRow(selectedQuestId);

		if (LogbookMenu_GetSetQuestState(selectedQuestId, FLAG_GET_FAVORITE))
		{
			SetFavoriteQuest(selectedQuestId);
			newRow = numRow;
			numRow++;
		}
		else
		{
			newRow = CountFavoriteQuests() + offset;
			offset++;
		}

		PopulateQuestName(selectedQuestId);
		PopulateListRowNameAndId(newRow, selectedQuestId);
	}
	return numRow + offset;
}

u8 LogbookMenu_GetSetSubquestState(u8 quest, u8 caseId, u8 childQuest)
{
    u8 uniqueId;
    u8 index;
    u8 bit;
    u8 mask;

    if (childQuest >= sSideQuests[quest].numSubquests)
        return 0;

    uniqueId = sSideQuests[quest].subquests[childQuest].id;
    index = uniqueId / 8;
    bit = uniqueId % 8;
    mask = 1 << bit;

    switch (caseId)
    {
        case FLAG_GET_UNLOCKED:
            return gSaveBlock2Ptr->unlockedSubquests[index] & mask;

        case FLAG_SET_UNLOCKED:
            gSaveBlock2Ptr->unlockedSubquests[index] |= mask;
            return 1;

        case FLAG_GET_COMPLETED:
            return gSaveBlock2Ptr->subQuests[index] & mask;

        case FLAG_SET_COMPLETED:
            gSaveBlock2Ptr->subQuests[index] |= mask;
            return 1;
    }

    return 0;
}

u8 LogbookMenu_GetSetQuestState(u8 quest, u8 caseId)
{
	u8 index = quest * 5 / 8;
	u8 bit = quest * 5 % 8;
	u8 mask = 0, index2 = 0, bit2 = 0, index3 = 0, bit3 = 0, mask2 = 0,
	   mask3 = 0;

	// 0 : locked
	// 1 : actived
	// 2 : rewarded
	// 3 : completed
	// 4 : favorited

	switch (caseId)
	{
		case FLAG_GET_UNLOCKED:
		case FLAG_SET_UNLOCKED:
			break;
		case FLAG_GET_INACTIVE:
		case FLAG_GET_ACTIVE:
		case FLAG_SET_ACTIVE:
		case FLAG_REMOVE_ACTIVE:
			bit += 1;
			break;
		case FLAG_GET_REWARD:
		case FLAG_SET_REWARD:
		case FLAG_REMOVE_REWARD:
			bit += 2;
			break;
		case FLAG_GET_COMPLETED:
		case FLAG_SET_COMPLETED:
			bit += 3;
			break;
		case FLAG_GET_FAVORITE:
		case FLAG_SET_FAVORITE:
		case FLAG_REMOVE_FAVORITE:
			bit += 4;
			break;
	}
	if (bit >= 8)
	{
		index += 1;
		bit %= 8;
	}
	mask = 1 << bit;

	switch (caseId)
	{
		case FLAG_GET_UNLOCKED:
			return gSaveBlock2Ptr->questData[index] & mask;
		case FLAG_SET_UNLOCKED:
			gSaveBlock2Ptr->questData[index] |= mask;
			return 1;
		case FLAG_GET_INACTIVE:
			bit2 = bit + 1;
			bit3 = bit + 2;
			index2 = index;
			index3 = index;

			if (bit2 >= 8)
			{
				index2 += 1;
				bit2 %= 8;
			}
			if (bit3 >= 8)
			{
				index3 += 1;
				bit3 %= 8;
			}

			mask2 = 1 << bit2;
			mask3 = 1 << bit3;
			return !(gSaveBlock2Ptr->questData[index] & mask) && \
			       !(gSaveBlock2Ptr->questData[index2] & mask2) && \
			       !(gSaveBlock2Ptr->questData[index3] & mask3);
		case FLAG_GET_ACTIVE:
			return gSaveBlock2Ptr->questData[index] & mask;
		case FLAG_SET_ACTIVE:
			gSaveBlock2Ptr->questData[index] |= mask;
			return 1;
		case FLAG_REMOVE_ACTIVE:
			gSaveBlock2Ptr->questData[index] &= ~mask;
			return 1;
		case FLAG_GET_REWARD:
			return gSaveBlock2Ptr->questData[index] & mask;
		case FLAG_SET_REWARD:
			gSaveBlock2Ptr->questData[index] |= mask;
			return 1;
		case FLAG_REMOVE_REWARD:
			gSaveBlock2Ptr->questData[index] &= ~mask;
			return 1;
		case FLAG_GET_COMPLETED:
			return gSaveBlock2Ptr->questData[index] & mask;
		case FLAG_SET_COMPLETED:
			gSaveBlock2Ptr->questData[index] |= mask;
			return 1;
		case FLAG_GET_FAVORITE:
			return gSaveBlock2Ptr->questData[index] & mask;
		case FLAG_SET_FAVORITE:
			gSaveBlock2Ptr->questData[index] |= mask;
			return 1;
		case FLAG_REMOVE_FAVORITE:
			gSaveBlock2Ptr->questData[index] &= ~mask;
			return 1;
	}
	return -1;  //failure
}

u8 CountUnlockedQuests(void)
{
	u8 q = 0, i = 0;

	for (i = 0; i < QUEST_COUNT; i++)
	{
		if (LogbookMenu_GetSetQuestState(i, FLAG_GET_UNLOCKED))
		{
			q++;
		}
	}
	return q;
}

u8 CountCompletedQuests(void)
{
	u8 q = 0, i = 0;

	u8 parentQuest = sStateDataPtr->parentQuest;

	if (IsSubquestMode())
	{
		for (i = 0; i < sSideQuests[parentQuest].numSubquests; i++)
		{
			if (LogbookMenu_GetSetSubquestState(parentQuest, FLAG_GET_COMPLETED, i))
			{
				q++;
			}
		}
	}
	else
	{
		for (i = 0; i < QUEST_COUNT; i++)
		{
			if (LogbookMenu_GetSetQuestState(i, FLAG_GET_COMPLETED))
			{
				q++;
			}
		}
	}

	return q;
}

u8 CountFavoriteQuests(void)
{
	u8 q = 0, i = 0;

	for (i = 0; i < QUEST_COUNT; i++)
	{
		if (LogbookMenu_GetSetQuestState(i, FLAG_GET_FAVORITE))
		{
			q++;
		}
	}

	return q;
}

void PopulateEmptyRow(u8 countQuest)
{
	questNamePointer = StringCopy(questNameArray[countQuest], sText_Empty);
}
void PrependQuestNumber(u8 countQuest)
{
	questNamePointer = ConvertIntToDecimalStringN(questNameArray[countQuest],
	                   countQuest + 1, STR_CONV_MODE_LEFT_ALIGN, 2);
	questNamePointer = StringAppend(questNamePointer,
	                                sText_DotSpace);
}

void SetFavoriteQuest(u8 countQuest)
{
	questNamePointer = StringAppend(questNameArray[countQuest],
	                                sText_ColorGreen);
}

void PopulateQuestName(u8 countQuest)
{
	if (LogbookMenu_GetSetQuestState(countQuest, FLAG_GET_UNLOCKED))
	{
		questNamePointer = StringAppend(questNameArray[countQuest],
		                                sSideQuests[countQuest].name);
	}
	else
	{
		StringAppend(questNameArray[countQuest], sText_Unk);
	}
}

void PopulateSubquestName(u8 parentQuest, u8 countQuest)
{
    if (parentQuest == QUEST_GYM_LEADER_TRIALS
     && countQuest == SUB_QUEST_BROCK
     && VarGet(VAR_BROCK_TRIAL_STATE) == 0)
    {
        questNamePointer = StringAppend(questNamePointer, sText_Unk);
        return;
    }

    if (LogbookMenu_GetSetSubquestState(parentQuest, FLAG_GET_UNLOCKED, countQuest))
    {
        questNamePointer = StringAppend(
            questNamePointer,
            sSideQuests[parentQuest].subquests[countQuest].name
        );
    }
	else
	{
	    questNamePointer = StringAppend(questNamePointer, sText_Unk);
	}

}

u8 PopulateListRowNameAndId(u8 row, u8 countQuest)
{
	sListMenuItems[row].label = questNameArray[countQuest];
	sListMenuItems[row].index = countQuest;
}

static bool8 DoesQuestHaveChildrenAndNotInactive(u16 itemId)
{
	if (sSideQuests[itemId].numSubquests != 0
	            && LogbookMenu_GetSetQuestState(itemId, FLAG_GET_UNLOCKED)
	            && !LogbookMenu_GetSetQuestState(itemId, FLAG_GET_INACTIVE))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

static void LogbookMenu_AddTextPrinterParameterized(u8 windowId, u8 fontId,
            const u8 *str, u8 x, u8 y,
            u8 letterSpacing, u8 lineSpacing, u8 speed, u8 colorIdx)
{
	AddTextPrinterParameterized4(windowId, fontId, x, y, letterSpacing,
	                             lineSpacing,
	                             sLogbookMenuWindowFontColors[colorIdx], speed, str);
}

static void MoveCursorFunc(s32 questId, bool8 onInit,
                           struct ListMenu *list)
{
	PlayCursorSound(onInit);

	if (sStateDataPtr->moveModeOrigPos == 0xFF)
	{
		LogbookMenu_DestroySprite(sStateDataPtr->spriteIconSlot ^ 1);
		sStateDataPtr->spriteIconSlot ^= 1;

		if (questId == LIST_CANCEL)
		{
			PrintDetailsForCancel();
		}
		else
		{
			GenerateAndPrintQuestDetails(questId);
			DetermineSpriteType(questId);
		}
		PrintMenuContext(DoesSelectedRowAcceptA(questId));
	}
}

static void PlayCursorSound(bool8 firstRun)
{
	if (firstRun == FALSE)
	{
		PlaySE(SE_BAG_CURSOR);
	}
}

static void FillQuestFooterWindow(void)
{
	FillWindowPixelBuffer(1, PIXEL_FILL(LOGBOOK_MENU_FOOTER_BG_COLOR));
	FillWindowPixelRect(1, PIXEL_FILL(LOGBOOK_MENU_FOOTER_BORDER_DARK_COLOR), 0, 0, 240, 2);
	FillWindowPixelRect(1, PIXEL_FILL(LOGBOOK_MENU_FOOTER_BORDER_LIGHT_COLOR), 0, 2, 240, 1);
	FillWindowPixelRect(1, PIXEL_FILL(LOGBOOK_MENU_FOOTER_BOTTOM_MID_COLOR), 0, 45, 240, 1);
	FillWindowPixelRect(1, PIXEL_FILL(LOGBOOK_MENU_FOOTER_BORDER_DARK_COLOR), 0, 46, 240, 2);
}

static void PrintDetailsForCancel()
{
	FillQuestFooterWindow();

	LogbookMenu_AddTextPrinterParameterized(1, 2, sText_CloseLogbook, 40, 3, 2, 0, 0,
	                                      4);

	LogbookMenu_CreateSprite(-1, sStateDataPtr->spriteIconSlot, ITEM);
}

void GenerateAndPrintQuestDetails(s32 questId)
{
	GenerateQuestFlavorText(questId);
	PrintQuestFlavorText(questId);
}
void GenerateQuestFlavorText(s32 questId)
{
	if (IsSubquestMode() == FALSE)
	{
		if (IsQuestInactiveState(questId) == TRUE)
		{
			StringCopy(gStringVar1, sText_StartForMore);
		}
		if (IsQuestActiveState(questId) == TRUE)
		{
			UpdateQuestFlavorText(questId);
		}
		if (IsQuestRewardState(questId) == TRUE)
		{
			StringCopy(gStringVar1, sText_ReturnRecieveReward);
		}
		if (IsQuestCompletedState(questId) == TRUE)
		{
			StringCopy(gStringVar1, sSideQuests[questId].donedesc);
		}
	}
	else
{
    if (sStateDataPtr->parentQuest == QUEST_GYM_LEADER_TRIALS
     && questId == SUB_QUEST_BROCK
     && VarGet(VAR_BROCK_TRIAL_STATE) == 0)
    {
        StringCopy(gStringVar1, sText_Empty);
    }
    else if (LogbookMenu_GetSetSubquestState(sStateDataPtr->parentQuest, FLAG_GET_UNLOCKED, questId))
    {
        const u8 *desc = GetDynamicSubquestDesc(sStateDataPtr->parentQuest, questId);

        if (desc != NULL)
            StringCopy(gStringVar1, desc);
        else
            StringCopy(gStringVar1,
                       sSideQuests[sStateDataPtr->parentQuest].subquests[questId].desc);
    }
    else
    {
        StringCopy(gStringVar1, sText_Empty);
    }
}

	StringExpandPlaceholders(gStringVar3, gStringVar1);
}
void UpdateQuestFlavorText(s32 questId)
{
	StringCopy(gStringVar1, sSideQuests[questId].desc);
}
void PrintQuestFlavorText(s32 questId)
{
	FillQuestFooterWindow();
	LogbookMenu_AddTextPrinterParameterized(1, 2, gStringVar3, 40, 3, 2, 0, 0,
	                                      4);
}

bool8 IsSubquestUnlockedState(s32 questId)
{
	if (LogbookMenu_GetSetSubquestState(sStateDataPtr->parentQuest,
	                                  FLAG_GET_UNLOCKED,
	                                  questId))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
bool8 IsQuestRewardState(s32 questId)
{
	if (LogbookMenu_GetSetQuestState(questId, FLAG_GET_REWARD))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool8 IsQuestInactiveState(s32 questId)
{
	if (!LogbookMenu_GetSetQuestState(questId, FLAG_GET_ACTIVE))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool8 IsQuestActiveState(s32 questId)
{
	if (LogbookMenu_GetSetQuestState(questId, FLAG_GET_ACTIVE))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool8 IsQuestCompletedState(s32 questId)
{
	if (LogbookMenu_GetSetQuestState(questId, FLAG_GET_COMPLETED))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool8 IsQuestUnlocked(s32 questId)
{
	if (LogbookMenu_GetSetQuestState(questId, FLAG_GET_UNLOCKED))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void DetermineSpriteType(s32 questId)
{
	u16 spriteId;
	u8 spriteType;

	if (IsSubquestMode() == FALSE)
	{
		spriteId = sSideQuests[questId].sprite;
		spriteType = sSideQuests[questId].spritetype;

		LogbookMenu_CreateSprite(spriteId, sStateDataPtr->spriteIconSlot,
		                       spriteType);
	}
	else if (IsSubquestUnlockedState(questId) == TRUE)
	{
		spriteId =
		      sSideQuests[sStateDataPtr->parentQuest].subquests[questId].sprite;
		spriteType =
		      sSideQuests[sStateDataPtr->parentQuest].subquests[questId].spritetype;
		LogbookMenu_CreateSprite(spriteId, sStateDataPtr->spriteIconSlot,
		                       spriteType);
	}
	else
	{
		LogbookMenu_CreateSprite(ITEM_NONE, sStateDataPtr->spriteIconSlot, ITEM);
	}
	LogbookMenu_DestroySprite(sStateDataPtr->spriteIconSlot ^ 1);
	sStateDataPtr->spriteIconSlot ^= 1;
}
static void LogbookMenu_CreateSprite(u16 itemId, u8 idx, u8 spriteType)
{
	u8 *ptr = &sItemMenuIconSpriteIds[10];
	u8 spriteId;
	struct SpriteSheet spriteSheet;
	struct CompressedSpritePalette spritePalette;
	struct SpriteTemplate *spriteTemplate;

	if (ptr[idx] == 0xFF)
	{
		FreeSpriteTilesByTag(102 + idx);
		FreeSpritePaletteByTag(102 + idx);

		switch (spriteType)
		{
			case OBJECT:
				spriteId = CreateObjectGraphicsSprite(itemId, SpriteCallbackDummy, 20,
				                                      132, 0);
				break;
			case ITEM:
				spriteId = AddItemIconObject(102 + idx, 102 + idx, itemId);
				break;
			case PKMN:
				LoadMonIconPalettes();
				spriteId = CreateMonIcon(itemId, SpriteCallbackDummy, 20, 132, 0, 1, 1);
				break;
			default:
				break;
		}

		if (spriteId != MAX_SPRITES)
		{
			ptr[idx] = spriteId;
			gSprites[spriteId].oam.objMode = ST_OAM_OBJ_NORMAL;
			gSprites[spriteId].oam.priority = 0;

			if (spriteType == ITEM)
			{
				gSprites[spriteId].x2 = 24;
				gSprites[spriteId].y2 = 140;
			}
		}
	}
}

void ResetSpriteState(void)
{
	u16 i;

	for (i = 0; i < NELEMS(sItemMenuIconSpriteIds); i++)
	{
		sItemMenuIconSpriteIds[i] = 0xFF;
	}
}

static void LogbookMenu_DestroySprite(u8 idx)
{
	u8 *ptr = &sItemMenuIconSpriteIds[10];

	if (ptr[idx] != 0xFF)
	{
		u16 palTag = GetSpritePaletteTagByPaletteNum(
		                   gSprites[ptr[idx]].oam.paletteNum);
		DestroySprite(&gSprites[ptr[idx]]);
		ptr[idx] = 0xFF;

		if (sStateDataPtr->oldPaletteTag != palTag)
		{
			if (sStateDataPtr->oldPaletteTag != 0)
			{
				FreeSpriteTilesByTag(sStateDataPtr->oldPaletteTag);
				sStateDataPtr->oldPaletteTag = palTag;
			}
		}
	}
}
static void GenerateStateAndPrint(u8 windowId, u32 questId,
                                  u8 y)
{
	u8 colorIndex;

	if (questId != LIST_CANCEL)
	{
		if (IsSubquestMode())
		{
			colorIndex = GenerateSubquestState(questId);
		}
		else
		{
			colorIndex = GenerateQuestState(questId);
		}

		PrintQuestState(windowId, y, colorIndex);
	}
}

static u8 CountApexInteractionsForNatureQuest(void)
{
    static const u16 sApexInteractionFlags[] =
    {
        FLAG_INTERACTED_APEX_TANGROWTH,
        FLAG_INTERACTED_APEX_ZAPDOS,
        FLAG_INTERACTED_APEX_ARTICUNO,
        FLAG_INTERACTED_APEX_MEWTWO,
        FLAG_INTERACTED_APEX_OSSCYTHE,
        FLAG_INTERACTED_APEX_MOLTRES,
        FLAG_INTERACTED_APEX_MIME_SR,
        FLAG_INTERACTED_APEX_ANNIHILAPE,
    };
    u8 i;
    u8 count = 0;

    for (i = 0; i < NELEMS(sApexInteractionFlags); i++)
    {
        if (FlagGet(sApexInteractionFlags[i]))
            count++;
    }

    return count;
}

static bool8 IsSpeciesCaught(u16 species)
{
    return GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT);
}

static u8 CountEvolutionThroughBondMilestones(void)
{
    u8 count = 0;

    if (FlagGet(FLAG_DID_BROCK_GRAVELER_TRADE))
        count++;
    if (FlagGet(FLAG_DID_MISTY_POLIWHIRL_TRADE))
        count++;
    if (FlagGet(FLAG_DID_ELECTABUZZ_TRADE))
        count++;
    if (IsSpeciesCaught(SPECIES_ROSELIA))
        count++;
    if (IsSpeciesCaught(SPECIES_CROBAT))
        count++;
    if (IsSpeciesCaught(SPECIES_CHIMECHO))
        count++;
    if (IsSpeciesCaught(SPECIES_PORYGON2))
        count++;
    if (IsSpeciesCaught(SPECIES_RHYPERIOR))
        count++;

    return count;
}

static u8 CountEvolutionThroughDesignMilestones(void)
{
    u8 count = 0;

    if (IsSpeciesCaught(SPECIES_PORYGON))
        count++;
    if (IsSpeciesCaught(SPECIES_OMANYTE))
        count++;
    if (IsSpeciesCaught(SPECIES_OMASTAR))
        count++;
    if (IsSpeciesCaught(SPECIES_OMATO))
        count++;
    if (IsSpeciesCaught(SPECIES_OMATOPS))
        count++;
    if (IsSpeciesCaught(SPECIES_KABUTO))
        count++;
    if (IsSpeciesCaught(SPECIES_KABUTOPS))
        count++;
    if (IsSpeciesCaught(SPECIES_KABUSTAR))
        count++;
    if (IsSpeciesCaught(SPECIES_KABUKNIGHT))
        count++;
    if (IsSpeciesCaught(SPECIES_AERODACTYL))
        count++;

    return count;
}

static bool8 TryGenerateNatureSubquestCounter(u8 subquestId, u8 *colorIndex)
{
    u8 count;
    const u8 target = 2;

    switch (subquestId)
    {
    case SUB_QUEST_EVOLUTION_THROUGH_BOND:
        count = CountEvolutionThroughBondMilestones();
        break;
    case SUB_QUEST_EVOLUTION_THROUGH_INSTINCT:
        count = CountApexInteractionsForNatureQuest();
        break;
    case SUB_QUEST_EVOLUTION_THROUGH_DESIGN:
        count = CountEvolutionThroughDesignMilestones();
        break;
    default:
        return FALSE;
    }

    ConvertIntToDecimalStringN(gStringVar1, count, STR_CONV_MODE_LEFT_ALIGN, 1);
    ConvertIntToDecimalStringN(gStringVar2, target, STR_CONV_MODE_LEFT_ALIGN, 1);
    StringExpandPlaceholders(gStringVar4, sText_QuestNumberDisplay);
    *colorIndex = (count >= target) ? 2 : 3;
    return TRUE;
}

static bool8 IsGymTrialTraded(u8 subquestId)
{
    if (LogbookMenu_GetSetSubquestState(QUEST_GYM_LEADER_TRIALS, FLAG_GET_COMPLETED, subquestId))
        return TRUE;

    switch (subquestId)
    {
    case SUB_QUEST_BROCK:
        return FlagGet(FLAG_DID_BROCK_GRAVELER_TRADE) || VarGet(VAR_BROCK_TRIAL_STATE) >= 5;
    case SUB_QUEST_MISTY:
        return FlagGet(FLAG_DID_MISTY_POLIWHIRL_TRADE) || VarGet(VAR_MISTY_TRIAL_STATE) >= 11;
    case SUB_QUEST_LTSURGE:
        return FlagGet(FLAG_DID_ELECTABUZZ_TRADE) || VarGet(VAR_LT_SURGE_TRIAL_STATE) >= 5;
    case SUB_QUEST_ERIKA:
        return VarGet(VAR_ERIKA_TRIAL_STATE) >= 6;
    case SUB_QUEST_KOGA:
        return VarGet(VAR_KOGA_TRIAL_STATE) >= 6;
    case SUB_QUEST_SABRINA:
        return VarGet(VAR_SABRINA_TRIAL_STATE) >= 5;
    default:
        return FALSE;
    }
}

static bool8 IsGymTrialCompleted(u8 subquestId)
{
    switch (subquestId)
    {
    case SUB_QUEST_BROCK:
        return FlagGet(FLAG_DEFEATED_BROCK) || VarGet(VAR_BROCK_TRIAL_STATE) >= 4;
    case SUB_QUEST_MISTY:
        return FlagGet(FLAG_DEFEATED_MISTY) || VarGet(VAR_MISTY_TRIAL_STATE) >= 10;
    case SUB_QUEST_LTSURGE:
        return FlagGet(FLAG_DEFEATED_LT_SURGE) || VarGet(VAR_LT_SURGE_TRIAL_STATE) >= 4;
    case SUB_QUEST_ERIKA:
        return FlagGet(FLAG_DEFEATED_ERIKA) || VarGet(VAR_ERIKA_TRIAL_STATE) >= 4;
    case SUB_QUEST_KOGA:
        return FlagGet(FLAG_DEFEATED_KOGA) || VarGet(VAR_KOGA_TRIAL_STATE) >= 4;
    case SUB_QUEST_SABRINA:
        return FlagGet(FLAG_DEFEATED_SABRINA) || VarGet(VAR_SABRINA_TRIAL_STATE) >= 4;
    case SUB_QUEST_BLAINE:
        return FlagGet(FLAG_DEFEATED_BLAINE);
    case SUB_QUEST_GIOVANNI:
        return FlagGet(FLAG_DEFEATED_LEADER_GIOVANNI);
    default:
        return FALSE;
    }
}

static bool8 TryGenerateGymTrialState(u8 subquestId, u8 *colorIndex)
{
    if (!LogbookMenu_GetSetSubquestState(QUEST_GYM_LEADER_TRIALS, FLAG_GET_UNLOCKED, subquestId))
        return FALSE;

    if (IsGymTrialTraded(subquestId))
    {
        StringCopy(gStringVar4, sText_TradedCaps);
        *colorIndex = 2;
    }
    else if (IsGymTrialCompleted(subquestId))
    {
        StringCopy(gStringVar4, sText_CompletedCaps);
        *colorIndex = 2;
    }
    else
    {
        StringCopy(gStringVar4, sText_InProgressCaps);
        *colorIndex = 3;
    }

    return TRUE;
}

u8 GenerateSubquestState(u8 questId)
{
	u8 parentQuest = sStateDataPtr->parentQuest;
    u8 colorIndex;

    if (parentQuest == QUEST_THE_NATURE_OF_EVOLUTION
     && TryGenerateNatureSubquestCounter(questId, &colorIndex))
    {
        return colorIndex;
    }
    else if (parentQuest == QUEST_GYM_LEADER_TRIALS
          && TryGenerateGymTrialState(questId, &colorIndex))
    {
        return colorIndex;
    }
    else if (LogbookMenu_GetSetSubquestState(parentQuest, FLAG_GET_COMPLETED,
	                                  questId))
	{
		StringCopy(gStringVar4, sSideQuests[parentQuest].subquests[questId].type);
	}
	else if (parentQuest == QUEST_APEX_POKEMON
	      && LogbookMenu_GetSetSubquestState(parentQuest, FLAG_GET_UNLOCKED, questId))
	{
		StringCopy(gStringVar4, sText_InProgress);
	}
	else
	{
		StringCopy(gStringVar4, sText_Empty);
	}

	return 2;
}

u8 GenerateQuestState(u8 questId)
{
	if (LogbookMenu_GetSetQuestState(questId, FLAG_GET_COMPLETED))
	{
		StringCopy(gStringVar4, sText_Complete);
		return 2;
	}
	else if (LogbookMenu_GetSetQuestState(questId, FLAG_GET_REWARD))
	{
		StringCopy(gStringVar4, sText_Reward);
		return 1;
	}
	else if (LogbookMenu_GetSetQuestState(questId, FLAG_GET_ACTIVE))
	{
		StringCopy(gStringVar4, sText_Active);
		return 3;
	}
	else
	{
		StringCopy(gStringVar4, sText_Empty);
	}
}

void PrintQuestState(u8 windowId, u8 y, u8 colorIndex)
{
	LogbookMenu_AddTextPrinterParameterized(windowId, FONT_NORMAL, gStringVar4,
	                                      192, y, 0, 0, 0xFF, colorIndex);
}

static void GenerateAndPrintHeader(void)
{
	GenerateDenominatorNumQuests();
	GenerateNumeratorNumQuests();
	GenerateMenuContext();

	PrintMenuContext(FALSE);
}
static void GenerateDenominatorNumQuests(void)
{
	ConvertIntToDecimalStringN(gStringVar2, QUEST_COUNT,
	                           STR_CONV_MODE_LEFT_ALIGN, 6);
}

static void GenerateNumeratorNumQuests(void)
{
	u8 parentQuest = sStateDataPtr->parentQuest;

	ConvertIntToDecimalStringN(gStringVar1, CountUnlockedQuests(),
	                           STR_CONV_MODE_LEFT_ALIGN, 6);

	if (IsSubquestMode())
	{
		ConvertIntToDecimalStringN(gStringVar2,
		                           sSideQuests[parentQuest].numSubquests,
		                           STR_CONV_MODE_LEFT_ALIGN, 6);
		ConvertIntToDecimalStringN(gStringVar1, CountCompletedQuests(),
		                           STR_CONV_MODE_LEFT_ALIGN,
		                           6);
	}
}

static void GenerateMenuContext(void)
{
	u8 parentQuest = sStateDataPtr->parentQuest;

	questNamePointer = StringCopy(questNameArray[QUEST_ARRAY_COUNT],
	                              sText_AllHeader);

	if (IsSubquestMode())
	{
		questNamePointer = StringCopy(questNameArray[QUEST_ARRAY_COUNT],
		                              sSideQuests[parentQuest].name);

	}
}

static void PrintMenuContext(bool8 selectedRowAcceptsA)
{
	DrawUiHintHeader(2, selectedRowAcceptsA ? gText_PickOKExit : sText_PickBack,
	                 8, 10, 0, FALSE);
	LogbookMenu_AddTextPrinterParameterized(2, FONT_NORMAL,
	                                      questNameArray[QUEST_ARRAY_COUNT],
	                                      8, 1, 0, 0, 0, 0);
	CopyWindowToVram(2, COPYWIN_GFX);
}

static bool8 DoesSelectedRowAcceptA(s32 questId)
{
	if (questId == LIST_CANCEL)
		return FALSE;

	if (!IsSubquestMode())
		return DoesQuestHaveChildrenAndNotInactive(questId);

	return sStateDataPtr->parentQuest == QUEST_APEX_POKEMON;
}

static void Task_Main(u8 taskId)
{
	s16 *data = gTasks[taskId].data;
	s32 input = ListMenu_ProcessInput(data[0]);

	u8 selectedQuestId = sListMenuItems[GetCursorPosition()].index;

	if (!gPaletteFade.active)
	{
		ListMenuGetScrollAndRow(data[0], &sListMenuState.scroll,
		                        &sListMenuState.row);

		switch (input)
		{
			case LIST_NOTHING_CHOSEN:
				if (JOY_NEW(SELECT_BUTTON))
				{
					ToggleFavoriteAndCleanUp(taskId, selectedQuestId);
				}
				break;

			case LIST_CANCEL:
				if (IsSubquestMode())
				{
					ReturnFromSubquestAndCleanUp(taskId);
				}
				else
				{
					TurnOffLogbookMenu(taskId);
				}
				break;

			default:
				if (!IsSubquestMode())
				{
					EnterSubquestModeAndCleanUp(taskId, data, input);
				}
				else if (sStateDataPtr->parentQuest == QUEST_APEX_POKEMON
				         && !CheckSelectedIsCancel(selectedQuestId))
				{
					OpenApexDossierAndCleanUp(taskId, selectedQuestId);
				}
				break;
		}
	}
}

u8 ManageFavorites(u8 selectedQuestId)
{
	if (LogbookMenu_GetSetQuestState(selectedQuestId, FLAG_GET_FAVORITE))
	{
		LogbookMenu_GetSetQuestState(selectedQuestId, FLAG_REMOVE_FAVORITE);
	}
	else
	{
		LogbookMenu_GetSetQuestState(selectedQuestId, FLAG_SET_FAVORITE);
	}
}

static void Task_LogbookMenuCleanUp(u8 taskId)
{
	s16 *data = gTasks[taskId].data;

	LogbookMenu_RemoveScrollIndicatorArrowPair();
	DestroyListMenuTask(data[0], &sListMenuState.scroll, &sListMenuState.row);
	ClearStdWindowAndFrameToTransparent(2, FALSE);

	InitItems();
	GenerateAndPrintHeader();
	AllocateResourcesForListMenu();
	BuildMenuTemplate();
	PlaceTopMenuScrollIndicatorArrows();

	if (sStateDataPtr->restoreCursor == TRUE)
	{
		RestoreSavedScrollAndRow(data);
	}
	else
	{
		ResetCursorToTop(data);
	}

}

static void RestoreSavedScrollAndRow(s16 *data)
{
	data[0] = ListMenuInit(&gMultiuseListMenuTemplate,
	                       sListMenuState.storedScrollOffset,
	                       sListMenuState.storedRowPosition);
}
static void ResetCursorToTop(s16 *data)
{
	sListMenuState.row = 0;
	sListMenuState.scroll = 0;
	data[0] = ListMenuInit(&gMultiuseListMenuTemplate, sListMenuState.scroll,
	                       sListMenuState.row);
}

static void LogbookMenu_RemoveScrollIndicatorArrowPair(void)
{
	if (sStateDataPtr->scrollIndicatorArrowPairId != 0xFF)
	{
		RemoveScrollIndicatorArrowPair(sStateDataPtr->scrollIndicatorArrowPairId);
		sStateDataPtr->scrollIndicatorArrowPairId = 0xFF;
	}
}


void EnterSubquestModeAndCleanUp(u8 taskId, s16 *data,
                                 s32 input)
{
	if (DoesQuestHaveChildrenAndNotInactive(input))
	{
		PlaySE(SE_SELECT);
		sStateDataPtr->parentQuest = input;
		sStateDataPtr->filterMode = ManageMode(SUB);
		SaveScrollAndRow(data);
		Task_LogbookMenuCleanUp(taskId);
	}
}
void ToggleFavoriteAndCleanUp(u8 taskId, u8 selectedQuestId)
{
	if (!IsSubquestMode()
	            && !CheckSelectedIsCancel(selectedQuestId))
	{
		PlaySE(SE_SELECT);
		ManageFavorites(selectedQuestId);
		sStateDataPtr->restoreCursor = FALSE;
		Task_LogbookMenuCleanUp(taskId);
	}
}
bool8 CheckSelectedIsCancel(u8 selectedQuestId)
{
	if (selectedQuestId == (0xFF - 1))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
void ReturnFromSubquestAndCleanUp(u8 taskId)
{
	PlaySE(SE_SELECT);
	sStateDataPtr->filterMode = ManageMode(SUB);
	Task_LogbookMenuCleanUp(taskId);
}

static void SetGpuRegBaseForFade()
{
	//Sets the GPU registers to prepare for a hardware fade
	SetGpuReg(REG_OFFSET_BLDCNT,
	          BLDCNT_TGT1_OBJ | BLDCNT_TGT1_BG0 | BLDCNT_TGT2_BG1 |
	          BLDCNT_EFFECT_BLEND);      //Blend Sprites and BG0 into BG1
	SetGpuReg(REG_OFFSET_BLDY, 0);
}

#define MAX_FADE_INTENSITY 16
#define MIN_FADE_INTENSITY 0

void InitFadeVariables(u8 taskId, u8 blendWeight, u8 frameDelay,
                       u8 frameTimerBase, u8 delta)
{
	gTasks[taskId].data[1] = blendWeight;
	gTasks[taskId].data[2] = frameDelay;
	gTasks[taskId].data[3] = gTasks[taskId].data[frameTimerBase];
	gTasks[taskId].data[4] = delta;
}


static void PrepareFadeOut(u8 taskId)
{
	SetGpuRegBaseForFade();
	SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(MAX_FADE_INTENSITY, 0));
	InitFadeVariables(taskId, MAX_FADE_INTENSITY, 0, 2, 2);
}

static bool8 HandleFadeOut(u8 taskId)
{
	if (gTasks[taskId].data[3]-- != 0)
	{
		return FALSE;
	}

	//Set the timer, decrease the fade weight by the delta, increase the delta by the timer
	gTasks[taskId].data[3] = gTasks[taskId].data[2];
	gTasks[taskId].data[1] -= gTasks[taskId].data[4];
	gTasks[taskId].data[2] += gTasks[taskId].data[3];

	//When blend weight runs out, set final blend and quit
	if (gTasks[taskId].data[1] <= 0)
	{
		SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, gTasks[taskId].data[1]));
		return TRUE;
	}
	//Set intermediate blend state
	SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gTasks[taskId].data[1],
	            MAX_FADE_INTENSITY - gTasks[taskId].data[1]));
	return FALSE;
}

static void PrepareFadeIn(u8 taskId)
{
	SetGpuRegBaseForFade();
	SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0,
	            MAX_FADE_INTENSITY));
	InitFadeVariables(taskId, MIN_FADE_INTENSITY, 0, 1, 2);
}

static bool8 HandleFadeIn(u8 taskId)
{
	//Set the timer, ncrease the fade weight by the delta,
	gTasks[taskId].data[3] = gTasks[taskId].data[2];
	gTasks[taskId].data[1] += gTasks[taskId].data[4];

	//When blend weight reaches max, set final blend and quit
	if (gTasks[taskId].data[1] >= MAX_FADE_INTENSITY)
	{
		SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(MAX_FADE_INTENSITY,
		            MIN_FADE_INTENSITY));
		return TRUE;
	}
	//Set intermediate blend state
	SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(gTasks[taskId].data[1],
	            MAX_FADE_INTENSITY - gTasks[taskId].data[1]));
	return FALSE;
}

static void Task_FadeOut(u8 taskId)
{
	if (HandleFadeOut(taskId))
	{
		PrepareFadeIn(taskId);
		Task_LogbookMenuCleanUp(taskId);
		gTasks[taskId].func = Task_FadeIn;
	}
}

static void Task_FadeIn(u8 taskId)
{
	if (HandleFadeIn(taskId))
	{
		gTasks[taskId].func = Task_Main;
	}
}

static void Task_LogbookMenuWaitFadeAndBail(u8 taskId)
{
	if (!gPaletteFade.active)
	{
		SetMainCallback2(sListMenuState.savedCallback);
		FreeResources();
		DestroyTask(taskId);
	}
}

static void FadeAndBail(void)
{
	BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
	CreateTask(Task_LogbookMenuWaitFadeAndBail, 0);
	SetVBlankCallback(VBlankCB);
	SetMainCallback2(MainCB);
}


#define try_free(ptr) ({        \
		void ** ptr__ = (void **)&(ptr);   \
		if (*ptr__ != NULL)                \
			Free(*ptr__);                  \
	})

static void FreeResources(void)
{
	int i;

	try_free(sStateDataPtr);
	try_free(sBg1TilemapBuffer);
	try_free(sListMenuItems);

	for (i = QUEST_ARRAY_COUNT; i > -1; i--)
	{
		try_free(questNameArray[i]);
	}

	try_free(questNameArray);
	FreeAllWindowBuffers();
}

void TurnOffLogbookMenu(u8 taskId)
{
	SetInitializedFlag(0);
	gTasks[taskId].func = Task_LogbookMenuTurnOff1;
}

static void OpenApexDossierAndCleanUp(u8 taskId, u8 subquestId)
{
	PlaySE(SE_SELECT);
	sPendingApexDossierSubquest = subquestId;
	sStateDataPtr->savedCallback = CB2_OpenApexRumorDossier;
	TurnOffLogbookMenu(taskId);
}

static void CB2_OpenApexRumorDossier(void)
{
	UseApexRumorDossier(CB2_ReturnToApexSubquestMenu, sPendingApexDossierSubquest);
}

static void CB2_ReturnToApexSubquestMenu(void)
{
	sReturningFromApexDossier = TRUE;
	LogbookMenu_Init(1, sListMenuState.savedCallback);
}

static void Task_LogbookMenuTurnOff1(u8 taskId)
{
	BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
	gTasks[taskId].func = Task_LogbookMenuTurnOff2;
}

static void Task_LogbookMenuTurnOff2(u8 taskId)
{
	s16 *data = gTasks[taskId].data;

	if (!gPaletteFade.active)
	{
		DestroyListMenuTask(data[0], &sListMenuState.scroll, &sListMenuState.row);
		if (sStateDataPtr->savedCallback != NULL)
		{
			SetMainCallback2(sStateDataPtr->savedCallback);
		}
		else
		{
			SetMainCallback2(sListMenuState.savedCallback);
		}

		LogbookMenu_RemoveScrollIndicatorArrowPair();
		FreeResources();
		DestroyTask(taskId);
	}
}

void Task_LogbookMenu_OpenFromStartMenu(u8 taskId)
{
	s16 *data = gTasks[taskId].data;
	if (!gPaletteFade.active)
	{
		CleanupOverworldWindowsAndTilemaps();
		LogbookMenu_Init(tItemPcParam, CB2_ReturnToFieldWithOpenMenu);
		DestroyTask(taskId);
	}
}

void LogbookMenu_CopyQuestName(u8 *dst, u8 questId)
{
	StringCopy(dst, sSideQuests[questId].name);
}

void LogbookMenu_CopySubquestName(u8 *dst, u8 parentId, u8 childId)
{
	StringCopy(dst, sSideQuests[parentId].subquests[childId].name);
}

void LogbookMenu_ResetMenuSaveData(void)
{
	memset(&gSaveBlock2Ptr->questData, 0,
	       sizeof(gSaveBlock2Ptr->questData));
	memset(&gSaveBlock2Ptr->subQuests, 0,
	       sizeof(gSaveBlock2Ptr->subQuests));
	memset(&gSaveBlock2Ptr->unlockedSubquests, 0,
	       sizeof(gSaveBlock2Ptr->unlockedSubquests));
}

const u8 *GetDynamicSubquestDesc(u8 parentQuest, u8 subquestId)
{
    if (parentQuest == QUEST_GYM_LEADER_TRIALS)
    {
        switch (subquestId)
        {
            case SUB_QUEST_BROCK:
                switch (VarGet(VAR_BROCK_TRIAL_STATE))
                {
                    default:
                        return sText_Empty;
                    case 1:
                        return gText_BrockTrialPhase1;
                    case 2:
                        return gText_BrockTrialPhase2;
                    case 3:
                        return gText_BrockTrialPhase3;
                    case 4:
                        return gText_BrockTrialPhase4;
                    case 5:
                        return gText_BrockTrialPhase5;
                }
			case SUB_QUEST_MISTY:
				switch (VarGet(VAR_MISTY_TRIAL_STATE))
				{
					default:
						return sText_Empty;
					case 1:
						return gText_MistyTrialPhase1;
					case 2:
					case 3:
					case 4:
					case 5:
					case 6:
					case 7:
					case 8:
						return gText_MistyTrialPhase2;
					case 9:
						return gText_MistyTrialPhase3;
					case 10:
						return gText_MistyTrialPhase4;
					case 11:
						return gText_MistyTrialPhase5;
				}
			case SUB_QUEST_LTSURGE:
				switch (VarGet(VAR_LT_SURGE_TRIAL_STATE))
				{
					default:
						return sText_Empty;
					case 1:
						return gText_LtSurgeTrialPhase1;
					case 2:
						return gText_LtSurgeTrialPhase2;
					case 3:
						return gText_LtSurgeTrialPhase3;
					case 4:
						return gText_LtSurgeTrialPhase4;
					case 5:
						return gText_LtSurgeTrialPhase5;
				}
			case SUB_QUEST_ERIKA:
				switch (VarGet(VAR_ERIKA_TRIAL_STATE))
				{
					default:
						return sText_Empty;
					case 1:
						return gText_ErikaTrialPhase1;
					case 2:
						return gText_ErikaTrialPhase2;
					case 3:
						return gText_ErikaTrialPhase3;
					case 4:
						return gText_ErikaTrialPhase4;
					case 5:
					case 6:
						return gText_ErikaTrialPhase5;
				}
			case SUB_QUEST_KOGA:
				switch (VarGet(VAR_KOGA_TRIAL_STATE))
				{
					default:
						return sText_Empty;
					case 1:
						return gText_KogaTrialPhase1;
					case 2:
						return gText_KogaTrialPhase2;
					case 3:
						return gText_KogaTrialPhase3;
					case 4:
						return gText_KogaTrialPhase4;
					case 5:
					case 6:
						return gText_KogaTrialPhase5;
				}
			case SUB_QUEST_SABRINA:
				switch (VarGet(VAR_SABRINA_TRIAL_STATE))
				{
					default:
						return sText_Empty;
					case 1:
						return gText_SabrinaTrialPhase1;
					case 2:
						return gText_SabrinaTrialPhase2;
					case 3:
						return gText_SabrinaTrialPhase3;
					case 4:
						return gText_SabrinaTrialPhase4;
					case 5:
						return gText_SabrinaTrialPhase5;
				}
        }
    }
    else if (parentQuest == QUEST_APEX_POKEMON)
    {
        if (subquestId == SUB_QUEST_APEX_ANNIHILAPE)
        {
            if (LogbookMenu_GetSetSubquestState(parentQuest, FLAG_GET_COMPLETED, subquestId))
                return sText_ApexAnnihilapeRecorded;
            else if (GetApexRumorCount(subquestId) >= APEX_RUMORS_REQUIRED)
                return sText_ApexAnnihilapeConfirmed;
            else if (GetApexRumorCount(subquestId) > 0)
                return sText_ApexAnnihilapeRumor;
        }
        else if (subquestId == SUB_QUEST_APEX_MIME_SR)
        {
            if (LogbookMenu_GetSetSubquestState(parentQuest, FLAG_GET_COMPLETED, subquestId))
                return sText_ApexMimeSrRecorded;
            else if (GetApexRumorCount(subquestId) >= APEX_RUMORS_REQUIRED)
                return sText_ApexMimeSrConfirmed;
            else if (GetApexRumorCount(subquestId) > 0)
                return sText_ApexMimeSrRumor;
        }
        else if (subquestId == SUB_QUEST_APEX_ARTICUNO)
        {
            if (LogbookMenu_GetSetSubquestState(parentQuest, FLAG_GET_COMPLETED, subquestId))
                return sText_ApexArticunoRecorded;
            else if (GetApexRumorCount(subquestId) >= APEX_RUMORS_REQUIRED)
                return sText_ApexArticunoConfirmed;
            else if (GetApexRumorCount(subquestId) > 0)
                return sText_ApexArticunoRumor;
        }
        else if (subquestId == SUB_QUEST_APEX_OSSCYTHE)
        {
            if (LogbookMenu_GetSetSubquestState(parentQuest, FLAG_GET_COMPLETED, subquestId))
                return sText_ApexOsscytheRecorded;
            else if (GetApexRumorCount(subquestId) >= APEX_RUMORS_REQUIRED)
                return sText_ApexOsscytheConfirmed;
            else if (GetApexRumorCount(subquestId) > 0)
                return sText_ApexOsscytheRumor;
        }
        else if (subquestId == SUB_QUEST_APEX_TANGROWTH)
        {
            if (LogbookMenu_GetSetSubquestState(parentQuest, FLAG_GET_COMPLETED, subquestId))
                return sText_ApexTangrowthRecorded;
            else if (GetApexRumorCount(subquestId) >= APEX_RUMORS_REQUIRED)
                return sText_ApexTangrowthConfirmed;
            else if (GetApexRumorCount(subquestId) > 0)
                return sText_ApexTangrowthRumor;
        }
        else if (subquestId == SUB_QUEST_APEX_ZAPDOS)
        {
            if (LogbookMenu_GetSetSubquestState(parentQuest, FLAG_GET_COMPLETED, subquestId))
                return sText_ApexZapdosRecorded;
            else if (GetApexRumorCount(subquestId) >= APEX_RUMORS_REQUIRED)
                return sText_ApexZapdosConfirmed;
            else if (GetApexRumorCount(subquestId) > 0)
                return sText_ApexZapdosRumor;
        }
        else if (subquestId == SUB_QUEST_APEX_MOLTRES)
        {
            if (LogbookMenu_GetSetSubquestState(parentQuest, FLAG_GET_COMPLETED, subquestId))
                return sText_ApexMoltresRecorded;
            else if (GetApexRumorCount(subquestId) >= APEX_RUMORS_REQUIRED)
                return sText_ApexMoltresConfirmed;
            else if (GetApexRumorCount(subquestId) > 0)
                return sText_ApexMoltresRumor;
        }
    }

    return NULL;
}
