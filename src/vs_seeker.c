#include "global.h"
#include "gflib.h"
#include "constants/songs.h"
#include "task.h"
#include "event_object_movement.h"
#include "new_menu_helpers.h"
#include "item_use.h"
#include "event_scripts.h"
#include "event_data.h"
#include "script.h"
#include "event_object_lock.h"
#include "field_specials.h"
#include "item.h"
#include "item_menu.h"
#include "field_effect.h"
#include "script_movement.h"
#include "battle.h"
#include "battle_setup.h"
#include "random.h"
#include "field_player_avatar.h"
#include "vs_seeker.h"
#include "constants/event_object_movement.h"
#include "constants/event_objects.h"
#include "constants/maps.h"
#include "constants/items.h"
#include "constants/quest_log.h"
#include "constants/trainers.h"
#include "constants/trainer_types.h"

// Each trainer can have up to 6 parties, including their original party.
// Each rematch is unavailable until the player has progressed to a certain point in the story (see TryGetRematchTrainerIdGivenGameState).
// A list of the trainer ids for each party is in sRematches. If a party doesn't update for a progression point it will have SKIP instead,
// and that trainer id will be ignored.
#define MAX_REMATCH_PARTIES 6
#define SKIP 0xFFFF

#define NO_REMATCH_LOCALID LOCALID_PLAYER

enum
{
   VSSEEKER_NOT_CHARGED,
   VSSEEKER_NO_ONE_IN_RANGE,
   VSSEEKER_CAN_USE,
};

typedef enum
{
    VSSEEKER_SINGLE_RESP_RAND,
    VSSEEKER_SINGLE_RESP_NO,
    VSSEEKER_SINGLE_RESP_YES
} VsSeekerSingleRespCode;

typedef enum
{
    VSSEEKER_RESPONSE_NO_RESPONSE,
    VSSEEKER_RESPONSE_UNFOUGHT_TRAINERS,
    VSSEEKER_RESPONSE_FOUND_REMATCHES
} VsSeekerResponseCode;

struct RematchData
{
    u16 trainerIdxs[MAX_REMATCH_PARTIES];
    u16 mapGroup; // unused
    u16 mapNum; // unused
};

struct VsSeekerTrainerInfo
{
    const u8 *script;
    u16 trainerIdx;
    u8 localId;
    u8 objectEventId;
    s16 xCoord;
    s16 yCoord;
    u8 graphicsId;
};

struct VsSeekerStruct
{
    /*0x000*/ struct VsSeekerTrainerInfo trainerInfo[OBJECT_EVENTS_COUNT];
    /*0x100*/ u8 filler_100[0x300];
    /*0x400*/ u16 trainerIdxArray[OBJECT_EVENTS_COUNT];
    /*0x420*/ u8 runningBehaviourEtcArray[OBJECT_EVENTS_COUNT];
    /*0x430*/ u8 numRematchableTrainers;
    /*0x431*/ u8 trainerHasNotYetBeenFought:1;
    /*0x431*/ u8 trainerDoesNotWantRematch:1;
    /*0x431*/ u8 trainerWantsRematch:1;
    u8 responseCode:5;
};

// static declarations
static EWRAM_DATA struct VsSeekerStruct *sVsSeeker = NULL;

static void VsSeekerResetInBagStepCounter(void);
static void VsSeekerResetChargingStepCounter(void);
static void Task_ResetObjectsRematchWantedState(u8 taskId);
static void ResetMovementOfRematchableTrainers(void);
static void Task_VsSeeker_1(u8 taskId);
static void Task_VsSeeker_2(u8 taskId);
static void GatherNearbyTrainerInfo(void);
static void Task_VsSeeker_3(u8 taskId);
static bool8 CanUseVsSeeker(void);
static u8 GetVsSeekerResponseInArea(const struct RematchData * vsSeekerData);
static u8 GetRematchTrainerIdGivenGameState(const u16 *trainerIdxs, u8 rematchIdx);
static u8 ShouldTryRematchBattleInternal(const struct RematchData * vsSeekerData, u16 trainerBattleOpponent);
static u8 HasRematchTrainerAlreadyBeenFought(const struct RematchData * vsSeekerData, u16 trainerBattleOpponent);
static int LookupVsSeekerOpponentInArray(const struct RematchData * array, u16 trainerId);
static bool8 IsTrainerReadyForRematchInternal(const struct RematchData * array, u16 trainerIdx);
static u8 GetRunningBehaviorFromGraphicsId(u8 graphicsId);
static u16 GetTrainerFlagFromScript(const u8 * script);
static int GetRematchIdx(const struct RematchData * vsSeekerData, u16 trainerFlagIdx);
static bool32 IsThisTrainerRematchable(u32 localId);
static void ClearAllTrainerRematchStates(void);
static bool8 IsTrainerVisibleOnScreen(struct VsSeekerTrainerInfo * trainerInfo);
static u8 GetNextAvailableRematchTrainer(const struct RematchData * vsSeekerData, u16 trainerFlagNo, u8 * idxPtr);
static u8 GetRematchableTrainerLocalId(void);
static void StartTrainerObjectMovementScript(struct VsSeekerTrainerInfo * trainerInfo, const u8 * script);
static u8 GetCurVsSeekerResponse(s32 vsSeekerIdx, u16 trainerIdx);
static void StartAllRespondantIdleMovements(void);
static bool8 ObjectEventIdIsSane(u8 objectEventId);
static u8 GetRandomFaceDirectionMovementType();
static bool8 IsTrainerExcludedFromVsSeekerRematches(u16 trainerId);
static bool8 IsKantoVsSeekerMap(void);
static bool8 IsTrainerEligibleForDefaultRematch(u16 trainerId);

static const struct RematchData sRematches[] = {

   { {TRAINER_FISHERMAN_TOMMY, TRAINER_FISHERMAN_TOMMY_2},
      MAP(MAP_ROUTE25) },

   { {TRAINER_FISHERMAN_TYLOR, TRAINER_FISHERMAN_TYLOR_2},
      MAP(MAP_ROUTE25) },

   { {TRAINER_PICNICKER_DIANA, TRAINER_PICNICKER_DIANA_2},
      MAP(MAP_CERULEAN_CITY_GYM) },

   { {TRAINER_LASS_ANN, TRAINER_LASS_ANN_2},
      MAP(MAP_SSANNE_1F_ROOM2) },

   { {TRAINER_LASS_DAWN, TRAINER_LASS_DAWN_2},
      MAP(MAP_SSANNE_2F_ROOM4) },

   { {TRAINER_CAMPER_JOHNSON, TRAINER_CAMPER_JOHNSON_2},
      MAP(MAP_ROUTE13) },

   { {TRAINER_PICNICKER_ARIANA, TRAINER_PICNICKER_ARIANA_2},
      MAP(MAP_ROCK_TUNNEL_1F) },

   { {TRAINER_LASS_KAY, TRAINER_LASS_KAY_2},
      MAP(MAP_CELADON_CITY_GYM) },

   { {TRAINER_PICNICKER_LEAH, TRAINER_PICNICKER_LEAH_2},
      MAP(MAP_ROCK_TUNNEL_1F) },

   { {TRAINER_LASS_LISA, TRAINER_LASS_LISA_2},
      MAP(MAP_CELADON_CITY_GYM) },

   { {TRAINER_TRIATHLETE_M_LAND, TRAINER_TRIATHLETE_M_LAND_2},
      MAP(MAP_ROUTE25) },

   { {TRAINER_TRIATHLETE_F_LAND, TRAINER_TRIATHLETE_F_LAND_2},
      MAP(MAP_ROUTE25) },

   { {TRAINER_YOUNGSTER_JOSH, TRAINER_YOUNGSTER_JOSH_2},
      MAP(MAP_MT_MOON_1F) },

   { {TRAINER_LASS_MIRIAM, TRAINER_LASS_MIRIAM_2},
      MAP(MAP_MT_MOON_1F) },

   { {TRAINER_ROUTE11_POKEMANIAC_CHAPLIN, TRAINER_ROUTE11_POKEMANIAC_CHAPLIN_2},
      MAP(MAP_ROUTE11) },

   { {TRAINER_PSYCHIC_JOHAN, TRAINER_PSYCHIC_JOHAN_2},
      MAP(MAP_SAFFRON_CITY_GYM) },

   { {TRAINER_TUBER_IRIS, TRAINER_TUBER_IRIS_2},
      MAP(MAP_ROUTE6) },

   { {TRAINER_SAILOR_TYRON, TRAINER_SAILOR_TYRON_2},
      MAP(MAP_ROUTE6) },

   { {TRAINER_PICNICKER_SOFIA, TRAINER_PICNICKER_SOFIA_2},
      MAP(MAP_ROCK_TUNNEL_B1F) },

   { {TRAINER_PSYCHIC_CAMERON, TRAINER_PSYCHIC_CAMERON_2},
      MAP(MAP_SAFFRON_CITY_GYM) },

   { {TRAINER_YOUNGSTER_TYLER, TRAINER_YOUNGSTER_TYLER_2},
      MAP(MAP_SSANNE_1F_ROOM2) },

   { {TRAINER_PICNICKER_MARTHA, TRAINER_PICNICKER_MARTHA_2},
      MAP(MAP_ROCK_TUNNEL_B1F) },

   { {TRAINER_JUGGLER_KAYDEN, TRAINER_JUGGLER_KAYDEN_2},
      MAP(MAP_FUCHSIA_CITY_GYM) },
   { {TRAINER_JUGGLER_SHAWN, TRAINER_JUGGLER_SHAWN_2},
      MAP(MAP_FUCHSIA_CITY_GYM) },
   { {TRAINER_JUGGLER_KIRK, TRAINER_NONE},
      MAP(MAP_FUCHSIA_CITY_GYM) },
   { {TRAINER_TAMER_EDGAR, TRAINER_TAMER_EDGAR_2},
      MAP(MAP_FUCHSIA_CITY_GYM) },
   { {TRAINER_TAMER_PHIL, TRAINER_NONE},
      MAP(MAP_FUCHSIA_CITY_GYM) },
   { {TRAINER_BUG_CATCHER_ROBBY, TRAINER_BUG_CATCHER_ROBBY_2},
      MAP(MAP_MT_MOON_1F) },
   { {TRAINER_SUPER_NERD_JOVAN, TRAINER_SUPER_NERD_JOVAN_2},
      MAP(MAP_MT_MOON_1F) },
   { {TRAINER_BUG_CATCHER_KENT, TRAINER_BUG_CATCHER_KENT_2},
      MAP(MAP_MT_MOON_1F) },
   { {TRAINER_HIKER_MARCOS, TRAINER_HIKER_MARCOS_2},
      MAP(MAP_MT_MOON_1F) },
   { {TRAINER_SCIENTIST_TED, TRAINER_SCIENTIST_TED_2},
      MAP(MAP_POKEMON_MANSION_1F) },
   { {TRAINER_BURGLAR_ARNIE, TRAINER_BURGLAR_ARNIE_2},
      MAP(MAP_POKEMON_MANSION_2F) },
   { {TRAINER_BURGLAR_SIMON, TRAINER_NONE},
      MAP(MAP_POKEMON_MANSION_3F) },
   { {TRAINER_SCIENTIST_BRAYDON, TRAINER_SCIENTIST_BRAYDON_2},
      MAP(MAP_POKEMON_MANSION_3F) },
   { {TRAINER_BURGLAR_LEWIS, TRAINER_BURGLAR_LEWIS_2},
      MAP(MAP_POKEMON_MANSION_B1F) },
   { {TRAINER_SCIENTIST_IVAN, TRAINER_NONE},
      MAP(MAP_POKEMON_MANSION_B1F) },
   { {TRAINER_CHANNELER_PATRICIA, TRAINER_NONE},
      MAP(MAP_POKEMON_TOWER_2F) },
   { {TRAINER_CHANNELER_CARLY, TRAINER_CHANNELER_CARLY_2},
      MAP(MAP_SAFFRON_CITY_DOJO) },
   { {TRAINER_CHANNELER_KARINA, TRAINER_CHANNELER_KARINA_2},
      MAP(MAP_CINNABAR_VOLCANO_3F) },
   { {TRAINER_CHANNELER_HOPE, TRAINER_NONE},
      MAP(MAP_POKEMON_TOWER_2F) },
   { {TRAINER_CHANNELER_ANGELICA, TRAINER_CHANNELER_ANGELICA_2},
      MAP(MAP_POKEMON_TOWER_3F) },
   { {TRAINER_CHANNELER_JENNIFER, TRAINER_CHANNELER_JENNIFER_2},
      MAP(MAP_POKEMON_TOWER_3F) },
   { {TRAINER_CHANNELER_EMILIA, TRAINER_CHANNELER_EMILIA_2},
      MAP(MAP_POKEMON_TOWER_3F) },
   { {TRAINER_HIKER_LUCAS, TRAINER_HIKER_LUCAS_2},
      MAP(MAP_ROCK_TUNNEL_1F) },
   { {TRAINER_HIKER_OLIVER, TRAINER_HIKER_OLIVER_2},
      MAP(MAP_ROCK_TUNNEL_1F) },
   { {TRAINER_HIKER_LENNY, TRAINER_HIKER_LENNY_2},
      MAP(MAP_ROCK_TUNNEL_1F) },
   { {TRAINER_POKEMANIAC_ASHTON, TRAINER_POKEMANIAC_ASHTON_2},
      MAP(MAP_ROCK_TUNNEL_1F) },
   { {TRAINER_POKEMANIAC_COOPER, TRAINER_POKEMANIAC_COOPER_2},
      MAP(MAP_ROCK_TUNNEL_B1F) },
   { {TRAINER_HIKER_DUDLEY, TRAINER_HIKER_DUDLEY_2},
      MAP(MAP_ROCK_TUNNEL_B1F) },
   { {TRAINER_HIKER_ALLEN, TRAINER_HIKER_ALLEN_2},
      MAP(MAP_ROCK_TUNNEL_B1F) },
   { {TRAINER_HIKER_ERIC, TRAINER_HIKER_ERIC_2},
      MAP(MAP_ROCK_TUNNEL_B1F) },
   { {TRAINER_POKEMANIAC_STEVE, TRAINER_POKEMANIAC_STEVE_2},
      MAP(MAP_ROCK_TUNNEL_B1F) },
   { {TRAINER_POKEMANIAC_WINSTON, TRAINER_POKEMANIAC_WINSTON_2},
      MAP(MAP_ROCK_TUNNEL_B1F) },
   { {TRAINER_GENTLEMAN_ARTHUR, TRAINER_NONE},
      MAP(MAP_SSANNE_1F_ROOM5) },
   { {TRAINER_GENTLEMAN_THOMAS, TRAINER_NONE},
      MAP(MAP_SSANNE_1F_ROOM7) },
   { {TRAINER_FISHERMAN_DALE, TRAINER_FISHERMAN_DALE_2},
      MAP(MAP_SSANNE_2F_ROOM2) },
   { {TRAINER_GENTLEMAN_BROOKS, TRAINER_NONE},
      MAP(MAP_SSANNE_2F_ROOM2) },
   { {TRAINER_GENTLEMAN_LAMAR, TRAINER_NONE},
      MAP(MAP_SSANNE_2F_ROOM4) },
   { {TRAINER_FISHERMAN_BARNY, TRAINER_NONE},
      MAP(MAP_SSANNE_B1F_ROOM1) },
   { {TRAINER_SAILOR_PHILLIP, TRAINER_NONE},
      MAP(MAP_SSANNE_B1F_ROOM1) },
   { {TRAINER_SAILOR_HUEY, TRAINER_NONE},
      MAP(MAP_SSANNE_B1F_ROOM2) },
   { {TRAINER_SAILOR_DYLAN, TRAINER_NONE},
      MAP(MAP_SSANNE_B1F_ROOM3) },
   { {TRAINER_SAILOR_DUNCAN, TRAINER_NONE},
      MAP(MAP_SSANNE_B1F_ROOM4) },
   { {TRAINER_SAILOR_TREVOR, TRAINER_SAILOR_TREVOR_2},
      MAP(MAP_SSANNE_DECK) },
   { {TRAINER_SAILOR_EDMOND, TRAINER_NONE},
      MAP(MAP_SSANNE_DECK) },
   { {TRAINER_BLACK_BELT_HITOSHI, TRAINER_NONE},
      MAP(MAP_SAFFRON_CITY_DOJO) },
   { {TRAINER_BLACK_BELT_HIDEKI, TRAINER_NONE},
      MAP(MAP_SAFFRON_CITY_DOJO) },
   { {TRAINER_BLACK_BELT_AARON, TRAINER_NONE},
      MAP(MAP_SAFFRON_CITY_DOJO) },
   { {TRAINER_BLACK_BELT_MIKE, TRAINER_NONE},
      MAP(MAP_SAFFRON_CITY_DOJO) },
   { {TRAINER_BLACK_BELT_KOICHI, TRAINER_NONE},
      MAP(MAP_SAFFRON_CITY_DOJO) },
   { {TRAINER_SCIENTIST_TRAVIS, TRAINER_NONE},
      MAP(MAP_SILPH_CO_10F) },
   { {TRAINER_TEAM_ROCKET_GRUNT_39, TRAINER_NONE},
      MAP(MAP_SILPH_CO_10F) },
   { {TRAINER_TEAM_ROCKET_GRUNT_40, TRAINER_NONE},
      MAP(MAP_SILPH_CO_11F) },
   { {TRAINER_TEAM_ROCKET_GRUNT_41, TRAINER_NONE},
      MAP(MAP_SILPH_CO_11F) },
   { {TRAINER_TEAM_ROCKET_GRUNT_24, TRAINER_NONE},
      MAP(MAP_SILPH_CO_2F) },
   { {TRAINER_SCIENTIST_JERRY, TRAINER_NONE},
      MAP(MAP_SILPH_CO_2F) },
   { {TRAINER_TEAM_ROCKET_GRUNT_23, TRAINER_NONE},
      MAP(MAP_SILPH_CO_2F) },
   { {TRAINER_SCIENTIST_CONNOR, TRAINER_NONE},
      MAP(MAP_SILPH_CO_2F) },
   { {TRAINER_TEAM_ROCKET_GRUNT_25, TRAINER_NONE},
      MAP(MAP_SILPH_CO_3F) },
   { {TRAINER_SCIENTIST_JOSE, TRAINER_NONE},
      MAP(MAP_SILPH_CO_3F) },
   { {TRAINER_TEAM_ROCKET_GRUNT_27, TRAINER_NONE},
      MAP(MAP_SILPH_CO_4F) },
   { {TRAINER_TEAM_ROCKET_GRUNT_26, TRAINER_TEAM_ROCKET_GRUNT_26_2},
      MAP(MAP_SILPH_CO_4F) },
   { {TRAINER_SCIENTIST_RODNEY, TRAINER_NONE},
      MAP(MAP_SILPH_CO_4F) },
   { {TRAINER_TEAM_ROCKET_GRUNT_29, TRAINER_NONE},
      MAP(MAP_SILPH_CO_5F) },
   { {TRAINER_JUGGLER_DALTON, TRAINER_NONE},
      MAP(MAP_SILPH_CO_5F) },
   { {TRAINER_SCIENTIST_BEAU, TRAINER_NONE},
      MAP(MAP_SILPH_CO_5F) },
   { {TRAINER_TEAM_ROCKET_GRUNT_28, TRAINER_NONE},
      MAP(MAP_SILPH_CO_5F) },
   { {TRAINER_TEAM_ROCKET_GRUNT_30, TRAINER_NONE},
      MAP(MAP_SILPH_CO_6F) },
   { {TRAINER_SCIENTIST_TAYLOR, TRAINER_NONE},
      MAP(MAP_SILPH_CO_6F) },
   { {TRAINER_TEAM_ROCKET_GRUNT_31, TRAINER_NONE},
      MAP(MAP_SILPH_CO_6F) },
   { {TRAINER_TEAM_ROCKET_GRUNT_33, TRAINER_NONE},
      MAP(MAP_SILPH_CO_7F) },
   { {TRAINER_TEAM_ROCKET_GRUNT_34, TRAINER_NONE},
      MAP(MAP_SILPH_CO_7F) },
   { {TRAINER_TEAM_ROCKET_GRUNT_35, TRAINER_NONE},
      MAP(MAP_SILPH_CO_7F) },
   { {TRAINER_SCIENTIST_JOSHUA, TRAINER_NONE},
      MAP(MAP_SILPH_CO_7F) },
   { {TRAINER_TEAM_ROCKET_GRUNT_32, TRAINER_NONE},
      MAP(MAP_SILPH_CO_8F) },
   { {TRAINER_TEAM_ROCKET_GRUNT_36, TRAINER_NONE},
      MAP(MAP_SILPH_CO_8F) },
   { {TRAINER_SCIENTIST_PARKER, TRAINER_NONE},
      MAP(MAP_SILPH_CO_8F) },
   { {TRAINER_SCIENTIST_ED, TRAINER_NONE},
      MAP(MAP_SILPH_CO_9F) },
   { {TRAINER_TEAM_ROCKET_GRUNT_38, TRAINER_NONE},
      MAP(MAP_SILPH_CO_9F) },
   { {TRAINER_TEAM_ROCKET_GRUNT_37, TRAINER_NONE},
      MAP(MAP_SILPH_CO_9F) },
   { {TRAINER_ACE_TRAINER_ROLANDO, TRAINER_ACE_TRAINER_ROLANDO_2},
      MAP(MAP_VICTORY_ROAD_1F) },
   { {TRAINER_ACE_TRAINER_NAOMI, TRAINER_ACE_TRAINER_NAOMI_2},
      MAP(MAP_VICTORY_ROAD_1F) },
   { {TRAINER_POKEMANIAC_DAWSON, TRAINER_POKEMANIAC_DAWSON_2},
      MAP(MAP_VICTORY_ROAD_2F) },
   { {TRAINER_BLACK_BELT_DAISUKE, TRAINER_BLACK_BELT_DAISUKE_2},
      MAP(MAP_VICTORY_ROAD_2F) },
   { {TRAINER_JUGGLER_NELSON, TRAINER_JUGGLER_NELSON_2},
      MAP(MAP_VICTORY_ROAD_2F) },
   { {TRAINER_TAMER_VINCENT, TRAINER_TAMER_VINCENT_2},
      MAP(MAP_VICTORY_ROAD_2F) },
   { {TRAINER_JUGGLER_GREGORY, TRAINER_JUGGLER_GREGORY_2},
      MAP(MAP_VICTORY_ROAD_2F) },
   { {TRAINER_ACE_TRAINER_GEORGE, TRAINER_ACE_TRAINER_GEORGE_2},
      MAP(MAP_VICTORY_ROAD_3F) },
   { {TRAINER_ACE_TRAINER_ALEXA, TRAINER_ACE_TRAINER_ALEXA_2},
      MAP(MAP_VICTORY_ROAD_3F) },
   { {TRAINER_ACE_TRAINER_COLBY, TRAINER_ACE_TRAINER_COLBY_2},
      MAP(MAP_VICTORY_ROAD_3F) },
   { {TRAINER_ACE_TRAINER_CAROLINE, TRAINER_ACE_TRAINER_CAROLINE_2},
      MAP(MAP_VICTORY_ROAD_3F) },
   { {TRAINER_COOL_COUPLE_RAY_TYRA, TRAINER_COOL_COUPLE_RAY_TYRA_2},
      MAP(MAP_VICTORY_ROAD_3F) },
   { {TRAINER_BLACK_BELT_TAKASHI, TRAINER_NONE},
      MAP(MAP_VIRIDIAN_CITY_GYM) },
   { {TRAINER_ACE_TRAINER_YUJI, TRAINER_NONE},
      MAP(MAP_VIRIDIAN_CITY_GYM) },
   { {TRAINER_BLACK_BELT_ATSUSHI, TRAINER_NONE},
      MAP(MAP_VIRIDIAN_CITY_GYM) },
   { {TRAINER_TAMER_JASON, TRAINER_NONE},
      MAP(MAP_VIRIDIAN_CITY_GYM) },
   { {TRAINER_BLACK_BELT_KIYO, TRAINER_NONE},
      MAP(MAP_VIRIDIAN_CITY_GYM) },
   { {TRAINER_ACE_TRAINER_SAMUEL, TRAINER_NONE},
      MAP(MAP_VIRIDIAN_CITY_GYM) },
   { {TRAINER_ACE_TRAINER_WARREN, TRAINER_NONE},
      MAP(MAP_VIRIDIAN_CITY_GYM) },
   { {TRAINER_BUG_CATCHER_RICK, TRAINER_BUG_CATCHER_RICK_2},
      MAP(MAP_VIRIDIAN_FOREST) },
   { {TRAINER_BUG_CATCHER_DOUG, TRAINER_BUG_CATCHER_DOUG_2},
      MAP(MAP_VIRIDIAN_FOREST) },
   { {TRAINER_BUG_CATCHER_SAMMY, TRAINER_BUG_CATCHER_SAMMY_2},
      MAP(MAP_VIRIDIAN_FOREST) },
   { {TRAINER_BUG_CATCHER_ANTHONY, TRAINER_BUG_CATCHER_ANTHONY_2},
      MAP(MAP_VIRIDIAN_FOREST) },
   { {TRAINER_BUG_CATCHER_CHARLIE, TRAINER_BUG_CATCHER_CHARLIE_2},
      MAP(MAP_VIRIDIAN_FOREST) },
};

static const u8 sMovementScript_Wait48[] = {
    MOVEMENT_ACTION_DELAY_16,
    MOVEMENT_ACTION_DELAY_16,
    MOVEMENT_ACTION_DELAY_16,
    MOVEMENT_ACTION_STEP_END
};

static const u8 sMovementScript_TrainerUnfought[] = {
    MOVEMENT_ACTION_EMOTE_EXCLAMATION_MARK,
    MOVEMENT_ACTION_STEP_END
};

static const u8 sMovementScript_TrainerNoRematch[] = {
    MOVEMENT_ACTION_EMOTE_X,
    MOVEMENT_ACTION_STEP_END
};

static const u8 sMovementScript_TrainerRematch[] = {
    MOVEMENT_ACTION_WALK_IN_PLACE_FASTER_DOWN,
    MOVEMENT_ACTION_EMOTE_DOUBLE_EXCL_MARK,
    MOVEMENT_ACTION_STEP_END
};

static const u8 sFaceDirectionMovementTypeByFacingDirection[] = {
    MOVEMENT_TYPE_FACE_DOWN,
    MOVEMENT_TYPE_FACE_DOWN,
    MOVEMENT_TYPE_FACE_UP,
    MOVEMENT_TYPE_FACE_LEFT,
    MOVEMENT_TYPE_FACE_RIGHT
};

// text

void VsSeekerFreezeObjectsAfterChargeComplete(void)
{
    CreateTask(Task_ResetObjectsRematchWantedState, 80);
}

static void Task_ResetObjectsRematchWantedState(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    u8 i;

    if (task->data[0] == 0 && walkrun_is_standing_still() == TRUE)
    {
        HandleEnforcedLookDirectionOnPlayerStopMoving();
        task->data[0] = 1;
    }

    if (task->data[1] == 0)
    {
        for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
        {
            if (ObjectEventIdIsSane(i) == TRUE)
            {
                if (gObjectEvents[i].singleMovementActive)
                    return;
                FreezeObjectEvent(&gObjectEvents[i]);
            }
        }
    }

    task->data[1] = 1;
    if (task->data[0] != 0)
    {
        DestroyTask(taskId);
        StopPlayerAvatar();
        ScriptContext_Enable();
    }
}

void VsSeekerResetObjectMovementAfterChargeComplete(void)
{
    struct ObjectEventTemplate * templates = gSaveBlock1Ptr->objectEventTemplates;
    u8 i;
    u8 movementType;
    u8 objEventId;
    struct ObjectEvent * objectEvent;

    for (i = 0; i < gMapHeader.events->objectEventCount; i++)
    {
        if ((templates[i].objUnion.normal.trainerType == TRAINER_TYPE_NORMAL
          || templates[i].objUnion.normal.trainerType == TRAINER_TYPE_BURIED)
         && (templates[i].objUnion.normal.movementType == MOVEMENT_TYPE_RAISE_HAND_AND_STOP
          || templates[i].objUnion.normal.movementType == MOVEMENT_TYPE_RAISE_HAND_AND_JUMP
          || templates[i].objUnion.normal.movementType == MOVEMENT_TYPE_RAISE_HAND_AND_SWIM))
        {
            movementType = GetRandomFaceDirectionMovementType();
            TryGetObjectEventIdByLocalIdAndMap(templates[i].localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, &objEventId);
            objectEvent = &gObjectEvents[objEventId];
            if (ObjectEventIdIsSane(objEventId) == TRUE)
            {
                SetTrainerMovementType(objectEvent, movementType);
            }
            templates[i].objUnion.normal.movementType = movementType;
        }
    }
}

bool8 UpdateVsSeekerStepCounter(void)
{
    u8 x = 0;

    if (CheckBagHasItem(ITEM_VS_SEEKER, 1) == TRUE)
    {
        if ((gSaveBlock1Ptr->trainerRematchStepCounter & 0xFF) < 100)
            gSaveBlock1Ptr->trainerRematchStepCounter++;
    }

    if (FlagGet(FLAG_SYS_VS_SEEKER_CHARGING) == TRUE)
    {
        if (((gSaveBlock1Ptr->trainerRematchStepCounter >> 8) & 0xFF) < 100)
        {
            x = (((gSaveBlock1Ptr->trainerRematchStepCounter >> 8) & 0xFF) + 1);
            gSaveBlock1Ptr->trainerRematchStepCounter = (gSaveBlock1Ptr->trainerRematchStepCounter & 0xFF) | (x << 8);
        }
        if (((gSaveBlock1Ptr->trainerRematchStepCounter >> 8) & 0xFF) == 100)
        {
            FlagClear(FLAG_SYS_VS_SEEKER_CHARGING);
            VsSeekerResetChargingStepCounter();
            ClearAllTrainerRematchStates();
            return TRUE;
        }
    }

    return FALSE;
}

void MapResetTrainerRematches(u16 mapGroup, u16 mapNum)
{
    FlagClear(FLAG_SYS_VS_SEEKER_CHARGING);
    VsSeekerResetChargingStepCounter();
    ClearAllTrainerRematchStates();
    ResetMovementOfRematchableTrainers();
}

static void ResetMovementOfRematchableTrainers(void)
{
    u8 i;

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        struct ObjectEvent * objectEvent = &gObjectEvents[i];
        if (objectEvent->movementType == MOVEMENT_TYPE_RAISE_HAND_AND_STOP
                || objectEvent->movementType == MOVEMENT_TYPE_RAISE_HAND_AND_JUMP
                || objectEvent->movementType == MOVEMENT_TYPE_RAISE_HAND_AND_SWIM)
        {
            u8 movementType = GetRandomFaceDirectionMovementType();
            if (objectEvent->active && gSprites[objectEvent->spriteId].data[0] == i)
            {
                gSprites[objectEvent->spriteId].x2 = 0;
                gSprites[objectEvent->spriteId].y2 = 0;
                SetTrainerMovementType(objectEvent, movementType);
            }
        }
    }
}

static void VsSeekerResetInBagStepCounter(void)
{
    gSaveBlock1Ptr->trainerRematchStepCounter &= 0xFF00;
}

static void VsSeekerSetStepCounterInBagFull(void)
{
    gSaveBlock1Ptr->trainerRematchStepCounter &= 0xFF00;
    gSaveBlock1Ptr->trainerRematchStepCounter |= 100;
}

static void VsSeekerResetChargingStepCounter(void)
{
    gSaveBlock1Ptr->trainerRematchStepCounter &= 0x00FF;
}

static void VsSeekerSetStepCounterFullyCharged(void)
{
    gSaveBlock1Ptr->trainerRematchStepCounter &= 0x00FF;
    gSaveBlock1Ptr->trainerRematchStepCounter |= (100 << 8);
}

void Task_VsSeeker_0(u8 taskId)
{
    u8 i;
    u8 respval;

    for (i = 0; i < 16; i++)
        gTasks[taskId].data[i] = 0;

    sVsSeeker = AllocZeroed(sizeof(struct VsSeekerStruct));
    GatherNearbyTrainerInfo();
    respval = CanUseVsSeeker();
    if (respval == VSSEEKER_NOT_CHARGED)
    {
        Free(sVsSeeker);
        DisplayItemMessageOnField(taskId, FONT_NORMAL, VSSeeker_Text_BatteryNotChargedNeedXSteps, Task_ItemUse_CloseMessageBoxAndReturnToField_VsSeeker);
    }
    else if (respval == VSSEEKER_NO_ONE_IN_RANGE)
    {
        Free(sVsSeeker);
        DisplayItemMessageOnField(taskId, FONT_NORMAL, VSSeeker_Text_NoTrainersWithinRange, Task_ItemUse_CloseMessageBoxAndReturnToField_VsSeeker);
    }
    else if (respval == VSSEEKER_CAN_USE)
    {
        FieldEffectStart(FLDEFF_USE_VS_SEEKER);
        gTasks[taskId].func = Task_VsSeeker_1;
        gTasks[taskId].data[0] = 15;
    }
}

static void Task_VsSeeker_1(u8 taskId)
{
    if (--gTasks[taskId].data[0] == 0)
    {
        gTasks[taskId].func = Task_VsSeeker_2;
        gTasks[taskId].data[1] = 16;
    }
}

static void Task_VsSeeker_2(u8 taskId)
{
    s16 * data = gTasks[taskId].data;

    if (data[2] != 2 && --data[1] == 0)
    {
        PlaySE(SE_CONTEST_MONS_TURN);
        data[1] = 11;
        data[2]++;
    }

    if (!FieldEffectActiveListContains(FLDEFF_USE_VS_SEEKER))
    {
        data[1] = 0;
        data[2] = 0;
        VsSeekerResetInBagStepCounter();
        sVsSeeker->responseCode = GetVsSeekerResponseInArea(sRematches);
        ScriptMovement_StartObjectMovementScript(0xFF, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, sMovementScript_Wait48);
        gTasks[taskId].func = Task_VsSeeker_3;
    }
}

static void GatherNearbyTrainerInfo(void)
{
    struct ObjectEventTemplate *templates = gSaveBlock1Ptr->objectEventTemplates;
    u8 objectEventId = 0;
    u8 vsSeekerObjectIdx = 0;
    s32 objectEventIdx;

    for (objectEventIdx = 0; objectEventIdx < gMapHeader.events->objectEventCount; objectEventIdx++)
    {
        if (templates[objectEventIdx].objUnion.normal.trainerType == TRAINER_TYPE_NORMAL || templates[objectEventIdx].objUnion.normal.trainerType == TRAINER_TYPE_BURIED)
        {
            sVsSeeker->trainerInfo[vsSeekerObjectIdx].script = templates[objectEventIdx].script;
            sVsSeeker->trainerInfo[vsSeekerObjectIdx].trainerIdx = GetTrainerFlagFromScript(templates[objectEventIdx].script);
            sVsSeeker->trainerInfo[vsSeekerObjectIdx].localId = templates[objectEventIdx].localId;
            TryGetObjectEventIdByLocalIdAndMap(templates[objectEventIdx].localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, &objectEventId);
            sVsSeeker->trainerInfo[vsSeekerObjectIdx].objectEventId = objectEventId;
            sVsSeeker->trainerInfo[vsSeekerObjectIdx].xCoord = gObjectEvents[objectEventId].currentCoords.x - 7;
            sVsSeeker->trainerInfo[vsSeekerObjectIdx].yCoord = gObjectEvents[objectEventId].currentCoords.y - 7;
            sVsSeeker->trainerInfo[vsSeekerObjectIdx].graphicsId = templates[objectEventIdx].graphicsId;
            vsSeekerObjectIdx++;
        }
    }
    sVsSeeker->trainerInfo[vsSeekerObjectIdx].localId = NO_REMATCH_LOCALID;
}

static void Task_VsSeeker_3(u8 taskId)
{
    if (ScriptMovement_IsObjectMovementFinished(NO_REMATCH_LOCALID, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup))
    {
        if (sVsSeeker->responseCode == VSSEEKER_RESPONSE_NO_RESPONSE)
        {
            DisplayItemMessageOnField(taskId, FONT_NORMAL, VSSeeker_Text_TrainersNotReady, Task_ItemUse_CloseMessageBoxAndReturnToField_VsSeeker);
        }
        else
        {
            if (sVsSeeker->responseCode == VSSEEKER_RESPONSE_FOUND_REMATCHES)
                StartAllRespondantIdleMovements();
            ClearDialogWindowAndFrame(0, TRUE);
            ClearPlayerHeldMovementAndUnfreezeObjectEvents();
            UnlockPlayerFieldControls();
            DestroyTask(taskId);
        }
        Free(sVsSeeker);
    }
}

static u8 CanUseVsSeeker(void)
{
    u8 vsSeekerChargeSteps = gSaveBlock1Ptr->trainerRematchStepCounter;
    if (vsSeekerChargeSteps == 100)
    {
        if (GetRematchableTrainerLocalId() == NO_REMATCH_LOCALID)
            return VSSEEKER_NO_ONE_IN_RANGE;
        else
            return VSSEEKER_CAN_USE;
    }
    else
    {
        TV_PrintIntToStringVar(0, 100 - vsSeekerChargeSteps);
        return VSSEEKER_NOT_CHARGED;
    }
}

static u8 GetVsSeekerResponseInArea(const struct RematchData * vsSeekerData)
{
    u16 trainerIdx = 0;
    u16 rval = 0;
    u8 rematchTrainerIdx;
    u8 unusedIdx = 0;
    u8 response = 0;
    s32 vsSeekerIdx = 0;

    while (sVsSeeker->trainerInfo[vsSeekerIdx].localId != NO_REMATCH_LOCALID)
    {
        if (IsTrainerVisibleOnScreen(&sVsSeeker->trainerInfo[vsSeekerIdx]) == TRUE)
        {
            trainerIdx = sVsSeeker->trainerInfo[vsSeekerIdx].trainerIdx;
            if (!HasTrainerBeenFought(trainerIdx))
            {
                StartTrainerObjectMovementScript(&sVsSeeker->trainerInfo[vsSeekerIdx], sMovementScript_TrainerUnfought);
                sVsSeeker->trainerHasNotYetBeenFought = 1;
                vsSeekerIdx++;
                continue;
            }
            rematchTrainerIdx = GetNextAvailableRematchTrainer(vsSeekerData, trainerIdx, &unusedIdx);
            if (rematchTrainerIdx == 0)
            {
                StartTrainerObjectMovementScript(&sVsSeeker->trainerInfo[vsSeekerIdx], sMovementScript_TrainerNoRematch);
                sVsSeeker->trainerDoesNotWantRematch = 1;
            }
            else
            {
                rval = Random() % 100; // Even if it's overwritten below, it progresses the RNG.
                response = GetCurVsSeekerResponse(vsSeekerIdx, trainerIdx);
                if (response == VSSEEKER_SINGLE_RESP_YES)
                    rval = 100; // Definitely yes
                else if (response == VSSEEKER_SINGLE_RESP_NO)
                    rval = 0; // Definitely no
                // Otherwise it's a 70% chance to want a rematch
                if (rval < 30)
                {
                    StartTrainerObjectMovementScript(&sVsSeeker->trainerInfo[vsSeekerIdx], sMovementScript_TrainerNoRematch);
                    sVsSeeker->trainerDoesNotWantRematch = 1;
                }
                else
                {
                    gSaveBlock1Ptr->trainerRematches[sVsSeeker->trainerInfo[vsSeekerIdx].localId] = rematchTrainerIdx;
                    ShiftStillObjectEventCoords(&gObjectEvents[sVsSeeker->trainerInfo[vsSeekerIdx].objectEventId]);
                    StartTrainerObjectMovementScript(&sVsSeeker->trainerInfo[vsSeekerIdx], sMovementScript_TrainerRematch);
                    sVsSeeker->trainerIdxArray[sVsSeeker->numRematchableTrainers] = trainerIdx;
                    sVsSeeker->runningBehaviourEtcArray[sVsSeeker->numRematchableTrainers] = GetRunningBehaviorFromGraphicsId(sVsSeeker->trainerInfo[vsSeekerIdx].graphicsId);
                    sVsSeeker->numRematchableTrainers++;
                    sVsSeeker->trainerWantsRematch = 1;
                }
            }
        }
        vsSeekerIdx++;
    }

    if (sVsSeeker->trainerWantsRematch)
    {
        PlaySE(SE_PIN);
        FlagSet(FLAG_SYS_VS_SEEKER_CHARGING);
        VsSeekerResetChargingStepCounter();
        return VSSEEKER_RESPONSE_FOUND_REMATCHES;
    }
    if (sVsSeeker->trainerHasNotYetBeenFought)
        return VSSEEKER_RESPONSE_UNFOUGHT_TRAINERS;
    return VSSEEKER_RESPONSE_NO_RESPONSE;
}

void ClearRematchStateByTrainerId(void)
{
    u8 objEventId = 0;
    struct ObjectEventTemplate *objectEventTemplates = gSaveBlock1Ptr->objectEventTemplates;
    int vsSeekerDataIdx = LookupVsSeekerOpponentInArray(sRematches, gTrainerBattleOpponent_A);

    if (vsSeekerDataIdx != -1)
    {
        int i;

        for (i = 0; i < gMapHeader.events->objectEventCount; i++)
        {
            if ((objectEventTemplates[i].objUnion.normal.trainerType == TRAINER_TYPE_NORMAL
              || objectEventTemplates[i].objUnion.normal.trainerType == TRAINER_TYPE_BURIED)
              && vsSeekerDataIdx == LookupVsSeekerOpponentInArray(sRematches, GetTrainerFlagFromScript(objectEventTemplates[i].script)))
            {
                struct ObjectEvent *objectEvent;

                TryGetObjectEventIdByLocalIdAndMap(objectEventTemplates[i].localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, &objEventId);
                objectEvent = &gObjectEvents[objEventId];
                #if __STDC_VERSION__ < 202311L
                GetRandomFaceDirectionMovementType(&objectEventTemplates[i]); // You are using this function incorrectly.  Please consult the manual.
                #else
                GetRandomFaceDirectionMovementType();
                #endif
                OverrideMovementTypeForObjectEvent(objectEvent, sFaceDirectionMovementTypeByFacingDirection[objectEvent->facingDirection]);
                gSaveBlock1Ptr->trainerRematches[objectEventTemplates[i].localId] = 0;
                if (gSelectedObjectEvent == objEventId)
                    objectEvent->movementType = sFaceDirectionMovementTypeByFacingDirection[objectEvent->facingDirection];
                else
                    objectEvent->movementType = MOVEMENT_TYPE_FACE_DOWN;
            }
        }
    }
}

static void TryGetRematchTrainerIdGivenGameState(const u16 * trainerIdxs, u8 * rematchIdx_p)
{
    switch (*rematchIdx_p)
    {
     case 0:
         break;
     case 1:
         if (!FlagGet(FLAG_GOT_VS_SEEKER))
             *rematchIdx_p = GetRematchTrainerIdGivenGameState(trainerIdxs, *rematchIdx_p);
         break;
     case 2:
         if (!FlagGet(FLAG_WORLD_MAP_CELADON_CITY))
             *rematchIdx_p = GetRematchTrainerIdGivenGameState(trainerIdxs, *rematchIdx_p);
         break;
     case 3:
         if (!FlagGet(FLAG_WORLD_MAP_FUCHSIA_CITY))
             *rematchIdx_p = GetRematchTrainerIdGivenGameState(trainerIdxs, *rematchIdx_p);
         break;
     case 4:
         if (!FlagGet(FLAG_SYS_GAME_CLEAR))
             *rematchIdx_p = GetRematchTrainerIdGivenGameState(trainerIdxs, *rematchIdx_p);
         break;
     case 5:
         if (!FlagGet(FLAG_SYS_GAME_CLEAR))
             *rematchIdx_p = GetRematchTrainerIdGivenGameState(trainerIdxs, *rematchIdx_p);
         break;
    }
}

static u8 GetRematchTrainerIdGivenGameState(const u16 *trainerIdxs, u8 rematchIdx)
{
    while (--rematchIdx != 0)
    {
        const u16 *rematch_p = trainerIdxs + rematchIdx;
        if (*rematch_p != SKIP)
            return rematchIdx;
    }
    return 0;
}

bool8 ShouldTryRematchBattle(void)
{
    if (ShouldTryRematchBattleInternal(sRematches, gTrainerBattleOpponent_A))
    {
        return TRUE;
    }
    return HasRematchTrainerAlreadyBeenFought(sRematches, gTrainerBattleOpponent_A);
}

static bool8 ShouldTryRematchBattleInternal(const struct RematchData *vsSeekerData, u16 trainerBattleOpponent)
{
    s32 rematchIdx = GetRematchIdx(vsSeekerData, trainerBattleOpponent);

    if (IsTrainerExcludedFromVsSeekerRematches(trainerBattleOpponent))
        return FALSE;
    if (rematchIdx == -1)
    {
        if (IsTrainerEligibleForDefaultRematch(trainerBattleOpponent) && IsThisTrainerRematchable(gSpecialVar_LastTalked))
            return TRUE;
        return FALSE;
    }
    if (rematchIdx >= 0 && rematchIdx < NELEMS(sRematches))
    {
        if (IsThisTrainerRematchable(gSpecialVar_LastTalked))
            return TRUE;
    }
    return FALSE;
}

static bool8 HasRematchTrainerAlreadyBeenFought(const struct RematchData *vsSeekerData, u16 trainerBattleOpponent)
{
    s32 rematchIdx = GetRematchIdx(vsSeekerData, trainerBattleOpponent);

    if (IsTrainerExcludedFromVsSeekerRematches(trainerBattleOpponent))
        return FALSE;
    if (rematchIdx == -1)
        return FALSE;
    if (!HasTrainerBeenFought(vsSeekerData[rematchIdx].trainerIdxs[0]))
        return FALSE;
    return TRUE;
}

void ClearRematchStateOfLastTalked(void)
{
    gSaveBlock1Ptr->trainerRematches[gSpecialVar_LastTalked] = 0;
    SetBattledTrainerFlag();
}

static int LookupVsSeekerOpponentInArray(const struct RematchData * array, u16 trainerId)
{
    int i, j;

    for (i = 0; i < NELEMS(sRematches); i++)
    {
        for (j = 0; j < MAX_REMATCH_PARTIES; j++)
        {
            u16 testTrainerId;
            if (array[i].trainerIdxs[j] == 0)
                break;
            testTrainerId = array[i].trainerIdxs[j];
            if (testTrainerId == SKIP)
                continue;
            if (testTrainerId == trainerId)
                return i;
        }
    }

    return -1;
}

int GetRematchTrainerId(u16 trainerId)
{
    u8 i;
    u8 j;

    if (IsTrainerExcludedFromVsSeekerRematches(trainerId))
        return 0;
    j = GetNextAvailableRematchTrainer(sRematches, trainerId, &i);
    if (!j)
    {
        if (IsTrainerEligibleForDefaultRematch(trainerId))
            return trainerId;
        return 0;
    }
    TryGetRematchTrainerIdGivenGameState(sRematches[i].trainerIdxs, &j);
    if (sRematches[i].trainerIdxs[j] == TRAINER_NONE)
        return trainerId;
    return sRematches[i].trainerIdxs[j];
}

u8 IsTrainerReadyForRematch(void)
{
    return IsTrainerReadyForRematchInternal(sRematches, gTrainerBattleOpponent_A);
}

static bool8 IsTrainerReadyForRematchInternal(const struct RematchData * array, u16 trainerId)
{
    int rematchTrainerIdx = LookupVsSeekerOpponentInArray(array, trainerId);

    if (IsTrainerExcludedFromVsSeekerRematches(trainerId))
        return FALSE;
    if (rematchTrainerIdx == -1)
    {
        if (IsTrainerEligibleForDefaultRematch(trainerId) && IsThisTrainerRematchable(gSpecialVar_LastTalked))
            return TRUE;
        return FALSE;
    }
    if (rematchTrainerIdx >= NELEMS(sRematches))
        return FALSE;
    if (!IsThisTrainerRematchable(gSpecialVar_LastTalked))
        return FALSE;
    return TRUE;
}

static bool8 ObjectEventIdIsSane(u8 objectEventId)
{
    struct ObjectEvent *objectEvent = &gObjectEvents[objectEventId];

    if (objectEvent->active && gMapHeader.events->objectEventCount >= objectEvent->localId && gSprites[objectEvent->spriteId].data[0] == objectEventId)
        return TRUE;
    return FALSE;
}

static u8 GetRandomFaceDirectionMovementType()
{
    u16 r1 = Random() % 4;

    switch (r1)
    {
        case 0:
            return MOVEMENT_TYPE_FACE_UP;
        case 1:
            return MOVEMENT_TYPE_FACE_DOWN;
        case 2:
            return MOVEMENT_TYPE_FACE_LEFT;
        case 3:
            return MOVEMENT_TYPE_FACE_RIGHT;
        default:
            return MOVEMENT_TYPE_FACE_DOWN;
    }
}

static u8 GetRunningBehaviorFromGraphicsId(u8 graphicsId)
{
    switch (graphicsId)
    {
        case OBJ_EVENT_GFX_LITTLE_GIRL:
        case OBJ_EVENT_GFX_YOUNGSTER:
        case OBJ_EVENT_GFX_BOY:
        case OBJ_EVENT_GFX_BUG_CATCHER:
        case OBJ_EVENT_GFX_LASS:
        case OBJ_EVENT_GFX_WOMAN_1:
        case OBJ_EVENT_GFX_CRUSH_GIRL:
        case OBJ_EVENT_GFX_MAN:
        case OBJ_EVENT_GFX_ROCKER:
        case OBJ_EVENT_GFX_WOMAN_2:
        case OBJ_EVENT_GFX_BEAUTY:
        case OBJ_EVENT_GFX_BALDING_MAN:
        case OBJ_EVENT_GFX_TUBER_F:
        case OBJ_EVENT_GFX_SCOUT_M:
        case OBJ_EVENT_GFX_SCOUT_F:
        case OBJ_EVENT_GFX_ACE_TRAINER_M:
        case OBJ_EVENT_GFX_ACE_TRAINER_F:
        case OBJ_EVENT_GFX_SWIMMER_M_LAND:
        case OBJ_EVENT_GFX_SWIMMER_F_LAND:
        case OBJ_EVENT_GFX_BLACK_BELT:
        case OBJ_EVENT_GFX_HIKER:
        case OBJ_EVENT_GFX_SAILOR:
            return MOVEMENT_TYPE_RAISE_HAND_AND_JUMP;
        case OBJ_EVENT_GFX_TUBER_M_WATER:
        case OBJ_EVENT_GFX_SWIMMER_M_WATER:
        case OBJ_EVENT_GFX_SWIMMER_F_WATER:
            return MOVEMENT_TYPE_RAISE_HAND_AND_SWIM;
        default:
            return MOVEMENT_TYPE_RAISE_HAND_AND_STOP;
    }
}

static u16 GetTrainerFlagFromScript(const u8 *script)
/*
 * The trainer flag is a little-endian short located +2 from
 * the script pointer, assuming the trainerbattle command is
 * first in the script.  Because scripts are unaligned, and
 * because the ARM processor requires shorts to be 16-bit
 * aligned, this function needs to perform explicit bitwise
 * operations to get the correct flag.
 *
 * 5c XX YY ZZ ...
 *       -- --
 */
{
    u16 trainerFlag;

    script += 2;
    trainerFlag = script[0];
    trainerFlag |= script[1] << 8;
    return trainerFlag;
}

static int GetRematchIdx(const struct RematchData * vsSeekerData, u16 trainerFlagIdx)
{
    int i;

    for (i = 0; i < NELEMS(sRematches); i++)
    {
        if (vsSeekerData[i].trainerIdxs[0] == trainerFlagIdx)
            return i;
    }

    return -1;
}

static bool32 IsThisTrainerRematchable(u32 localId)
{
    if (!gSaveBlock1Ptr->trainerRematches[localId])
        return FALSE;
    return TRUE;
}

static void ClearAllTrainerRematchStates(void)
{
    u8 i;

    for (i = 0; i < NELEMS(gSaveBlock1Ptr->trainerRematches); i++)
        gSaveBlock1Ptr->trainerRematches[i] = 0;
}

static bool8 IsTrainerVisibleOnScreen(struct VsSeekerTrainerInfo * trainerInfo)
{
    s16 x;
    s16 y;

    PlayerGetDestCoords(&x, &y);
    x -= 7;
    y -= 7;

    if (   x - 7 <= trainerInfo->xCoord
        && x + 7 >= trainerInfo->xCoord
        && y - 5 <= trainerInfo->yCoord
        && y + 5 >= trainerInfo->yCoord
        && ObjectEventIdIsSane(trainerInfo->objectEventId) == 1)
        return TRUE;
    return FALSE;
}

static u8 GetNextAvailableRematchTrainer(const struct RematchData * vsSeekerData, u16 trainerFlagNo, u8 * idxPtr)
{
    int i, j;

    if (IsTrainerExcludedFromVsSeekerRematches(trainerFlagNo))
        return 0;

    for (i = 0; i < NELEMS(sRematches); i++)
    {
        if (vsSeekerData[i].trainerIdxs[0] == trainerFlagNo)
        {
            *idxPtr = i;
            for (j = 1; j < MAX_REMATCH_PARTIES; j++)
            {
                if (vsSeekerData[i].trainerIdxs[j] == TRAINER_NONE)
                {
                    if (j == 1 && IsTrainerEligibleForDefaultRematch(trainerFlagNo))
                        return j;
                    return j - 1;
                }
                if (vsSeekerData[i].trainerIdxs[j] == SKIP)
                    continue;
                if (HasTrainerBeenFought(vsSeekerData[i].trainerIdxs[j]))
                    continue;
                return j;
            }
            return j - 1;
        }
    }

    *idxPtr = 0;
    if (IsTrainerEligibleForDefaultRematch(trainerFlagNo))
        return 1;
    return 0;
}

static bool8 IsTrainerExcludedFromVsSeekerRematches(u16 trainerId)
{
    u8 mapGroup = gSaveBlock1Ptr->location.mapGroup;
    u8 mapNum = gSaveBlock1Ptr->location.mapNum;

    if (mapGroup == 1 && mapNum >= 5 && mapNum <= 29)
        return TRUE;
    if (mapGroup == 14 && mapNum == 2)
        return TRUE;
    if (mapGroup == 1 && mapNum >= 43 && mapNum <= 53 && gTrainers[trainerId].trainerClass == TRAINER_CLASS_TEAM_ROCKET)
        return TRUE;
    return FALSE;
}

static bool8 IsKantoVsSeekerMap(void)
{
    u8 mapGroup = gSaveBlock1Ptr->location.mapGroup;
    u8 mapNum = gSaveBlock1Ptr->location.mapNum;

    if (mapGroup == 1)
        return TRUE;
    if (mapGroup == 2 && mapNum >= 23 && mapNum <= 30)
        return TRUE;
    if (mapGroup == 3 && (mapNum <= 11 || (mapNum >= 14 && mapNum <= 40)))
        return TRUE;
    if (mapGroup >= 4 && mapGroup <= 30)
        return TRUE;
    return FALSE;
}

static bool8 IsTrainerEligibleForDefaultRematch(u16 trainerId)
{
    switch (gTrainers[trainerId].trainerClass)
    {
        case TRAINER_CLASS_RIVAL_EARLY:
        case TRAINER_CLASS_BOSS:
        case TRAINER_CLASS_LEADER:
        case TRAINER_CLASS_ELITE_FOUR:
        case TRAINER_CLASS_RIVAL_LATE:
        case TRAINER_CLASS_CHAMPION:
        case TRAINER_CLASS_ROCKET_ADMIN:
            return FALSE;
        default:
            return IsKantoVsSeekerMap();
    }
}

static u8 GetRematchableTrainerLocalId(void)
{
    u8 idx;
    u8 i;

    for (i = 0; sVsSeeker->trainerInfo[i].localId != NO_REMATCH_LOCALID; i++)
    {
        if (IsTrainerVisibleOnScreen(&sVsSeeker->trainerInfo[i]) == 1)
        {
            if (HasTrainerBeenFought(sVsSeeker->trainerInfo[i].trainerIdx) != 1 || GetNextAvailableRematchTrainer(sRematches, sVsSeeker->trainerInfo[i].trainerIdx, &idx))
                return sVsSeeker->trainerInfo[i].localId;
        }
    }

    return NO_REMATCH_LOCALID;
}

static void StartTrainerObjectMovementScript(struct VsSeekerTrainerInfo * trainerInfo, const u8 * script)
{
    UnfreezeObjectEvent(&gObjectEvents[trainerInfo->objectEventId]);
    ScriptMovement_StartObjectMovementScript(trainerInfo->localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, script);
}

static u8 GetCurVsSeekerResponse(s32 vsSeekerIdx, u16 trainerIdx)
{
    s32 i;
    s32 j;

    for (i = 0; i < vsSeekerIdx; i++)
    {
        if (IsTrainerVisibleOnScreen(&sVsSeeker->trainerInfo[i]) == 1 && sVsSeeker->trainerInfo[i].trainerIdx == trainerIdx)
        {
            for (j = 0; j < sVsSeeker->numRematchableTrainers; j++)
            {
                if (sVsSeeker->trainerIdxArray[j] == sVsSeeker->trainerInfo[i].trainerIdx)
                    return VSSEEKER_SINGLE_RESP_YES;
            }
            return VSSEEKER_SINGLE_RESP_NO;
        }
    }
    return VSSEEKER_SINGLE_RESP_RAND;
}

static void StartAllRespondantIdleMovements(void)
{
    u8 dummy = 0;
    s32 i;
    s32 j;

    for (i = 0; i < sVsSeeker->numRematchableTrainers; i++)
    {
        for (j = 0; sVsSeeker->trainerInfo[j].localId != NO_REMATCH_LOCALID; j++)
        {
            if (sVsSeeker->trainerInfo[j].trainerIdx == sVsSeeker->trainerIdxArray[i])
            {
                struct ObjectEvent *objectEvent = &gObjectEvents[sVsSeeker->trainerInfo[j].objectEventId];

                if (ObjectEventIdIsSane(sVsSeeker->trainerInfo[j].objectEventId) == 1)
                    SetTrainerMovementType(objectEvent, sVsSeeker->runningBehaviourEtcArray[i]);
                OverrideMovementTypeForObjectEvent(objectEvent, sVsSeeker->runningBehaviourEtcArray[i]);
                gSaveBlock1Ptr->trainerRematches[sVsSeeker->trainerInfo[j].localId] = GetNextAvailableRematchTrainer(sRematches, sVsSeeker->trainerInfo[j].trainerIdx, &dummy);
            }
        }
    }
}
