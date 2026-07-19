#include "global.h"
#include "gflib.h"
#include "quest_log.h"
#include "list_menu.h"
#include "diploma.h"
#include "script.h"
#include "field_player_avatar.h"
#include "item.h"
#include "overworld.h"
#include "field_message_box.h"
#include "event_data.h"
#include "strings.h"
#include "battle.h"
#include "fieldmap.h"
#include "field_specials.h"
#include "region_map.h"
#include "task.h"
#include "battle_tower.h"
#include "field_camera.h"
#include "field_effect.h"
#include "field_effect_helpers.h"
#include "field_weather.h"
#include "day_night.h"
#include "event_object_lock.h"
#include "event_object_movement.h"
#include "menu_indicators.h"
#include "random.h"
#include "mail_data.h"
#include "m4a.h"
#include "pokemon_storage_system.h"
#include "script_menu.h"
#include "data.h"
#include "pokedex.h"
#include "text_window.h"
#include "menu.h"
#include "mystery_gift.h"
#include "naming_screen.h"
#include "palette.h"
#include "party_menu.h"
#include "dynamic_placeholder_text_util.h"
#include "new_menu_helpers.h"
#include "constants/songs.h"
#include "constants/items.h"
#include "constants/maps.h"
#include "constants/region_map_sections.h"
#include "constants/moves.h"
#include "constants/menu.h"
#include "constants/event_objects.h"
#include "constants/flags.h"
#include "constants/metatile_labels.h"

#define TAG_MEWTWO_FLASHBACK_BEAM      0x1260
#define PAL_TAG_MEWTWO_FLASHBACK_BEAM  0x1261
#define TAG_MEWTWO_FLASHBACK_CIRCLE    0x1262
#define PAL_TAG_MEWTWO_FLASHBACK_CIRCLE 0x1263
#define MEWTWO_FLASHBACK_BEAM_SPRITE_COUNT 2
#define FLASHBACK_BG_GREYSCALE_PALETTE_COUNT 13
#define MEWTWO_FLASHBACK_EFFECT_X 10
#define MEWTWO_FLASHBACK_EFFECT_Y 59
#define MEWTWO_FLASHBACK_EFFECT_PRIORITY 1
#define MEWTWO_FLASHBACK_CIRCLE_SCREEN_X 120
#define MEWTWO_FLASHBACK_CIRCLE_SCREEN_Y 56

static EWRAM_DATA u8 sElevatorCurrentFloorWindowId = 0;
static EWRAM_DATA u16 sElevatorScroll = 0;
static EWRAM_DATA u16 sElevatorCursorPos = 0;
static EWRAM_DATA struct ListMenuItem * sListMenuItems = NULL;
static EWRAM_DATA u16 sListMenuLastScrollPosition = 0;
static EWRAM_DATA u8 sPCBoxToSendMon = 0;
static EWRAM_DATA u8 sBrailleTextCursorSpriteID = 0;
static EWRAM_DATA u16 sFlashbackPaletteBackup[PLTT_BUFFER_SIZE] = {0};
static EWRAM_DATA bool8 sFlashbackPaletteBackupValid = FALSE;
static EWRAM_DATA u16 sFlashbackCameraFocusX = 0;
static EWRAM_DATA u16 sFlashbackCameraFocusY = 0;
static EWRAM_DATA bool8 sFlashbackCameraFocusValid = FALSE;
static EWRAM_DATA s16 sFlashbackPlayerX = 0;
static EWRAM_DATA s16 sFlashbackPlayerY = 0;
static EWRAM_DATA u8 sFlashbackPlayerDirection = DIR_SOUTH;
static EWRAM_DATA u16 sFlashbackPlayerTransitionFlags = PLAYER_AVATAR_FLAG_ON_FOOT;
static EWRAM_DATA bool8 sFlashbackPlayerPositionValid = FALSE;
static EWRAM_DATA bool8 sFlashbackSuppressTerrainEffects = FALSE;
static EWRAM_DATA u8 sMewtwoFlashbackBeamSpriteIds[MEWTWO_FLASHBACK_BEAM_SPRITE_COUNT] = {MAX_SPRITES, MAX_SPRITES};
static EWRAM_DATA u8 sMewtwoFlashbackChargeCircleSpriteId = MAX_SPRITES;

COMMON_DATA struct ListMenuTemplate sFieldSpecialsListMenuTemplate = {0};
COMMON_DATA u16 sFieldSpecialsListMenuScrollBuffer = 0;

static void Task_AnimatePcTurnOn(u8 taskId);
void HideMewtwoFlashbackBeam(void);
void HoldFlashbackFadeWhite(void);
void SpawnFlashbackDustAtCoords(void);
void SlowEliteFourFlashbackMusic(void);
void RestoreEliteFourFlashbackMusic(void);
void SuppressFlashbackPlayerGroundEffects(void);
void RestoreFlashbackPlayerGroundEffects(void);
static void PcTurnOnUpdateMetatileId(bool16 flag);
static void Task_ShakeScreen(u8 taskId);
static void Task_EndScreenShake(u8 taskId);
static u16 SamplePokemonRequestMon(void);
static u16 SamplePokemonRequestReward(void);
static void Task_ElevatorShake(u8 taskId);
static void AnimateElevatorWindowView(u16 nfloors, bool8 direction);
static void Task_AnimateElevatorWindowView(u8 taskId);
static void Task_CreateScriptListMenu(u8 taskId);
static void CreateScriptListMenu(void);
static void ScriptListMenuMoveCursorFunction(s32 nothing, bool8 is, struct ListMenu * used);
static void Task_ListMenuHandleInput(u8 taskId);
static void Task_DestroyListMenu(u8 taskId);
static void Task_SuspendListMenu(u8 taskId);
static void Task_RedrawScrollArrowsAndWaitInput(u8 taskId);
static void Task_CreateMenuRemoveScrollIndicatorArrowPair(u8 taskId);
static void Task_ListMenuRemoveScrollIndicatorArrowPair(u8 taskId);
static u16 GetStarterSpeciesById(u16 starterIdx);
static void ChangeBoxPokemonNickname_CB(void);
static void ChangePokemonNickname_CB(void);
static void Task_RunPokemonLeagueLightingEffect(u8 taskId);
static void Task_CancelPokemonLeagueLightingEffect(u8 taskId);
static void Task_WingFlapSound(u8 taskId);
static void Task_MewtwoFlashbackSparkleLoop(u8 taskId);
static void Task_RevealMewtwoFlashbackBeam(u8 taskId);
static void SpawnMewtwoFlashbackSparkle(s16 x, s16 y, u8 priority);
static void SpawnMewtwoFlashbackDust(s16 x, s16 y, u8 priority);
static void SpriteCB_MewtwoFlashbackDust(struct Sprite *sprite);
void ShowMewtwoFlashbackChargeCircle(void);
static void HideMewtwoFlashbackChargeCircle(void);
static void RetintFlashbackEffectPalettes(void);
static void TintFlashbackPaletteBuffer(u16 *palette);
static void RestoreFlashbackFieldEffectPalettes(void);
static void RestoreFlashbackFieldEffectPalette(const struct SpritePalette *spritePalette);
static void WhitenFlashbackFieldEffectPalettes(void);
static void WhitenFlashbackFieldEffectPalette(const struct SpritePalette *spritePalette);
static bool8 IsFlashbackFieldEffectSprite(const struct Sprite *sprite);
static void ClearFlashbackTerrainFieldEffects(void);
static void CacheFlashbackPlayerPosition(void);
static void ParkFlashbackPlayerObject(void);
static void RestoreFlashbackPlayerObjectEvents(void);
static void ResetFlashbackPlayerGroundEffectState(struct ObjectEvent *playerObj, struct Sprite *playerSprite);
static u16 GetFlashbackPlayerTransitionFlags(void);
u8 GetPlayerAvatarBike(void);

extern const u16 gFieldEffectPal_SmallSparkle[];

static const u16 sMewtwoFlashbackBeamGfx[] = INCBIN_U16("graphics/field_effects/pics/mewtwo_flashback_beam.4bpp");
static const u16 sMewtwoFlashbackChargeCircleGfx[] = INCBIN_U16("graphics/field_effects/pics/mewtwo_flashback_charge_circle.4bpp");

static const struct SpriteSheet sMewtwoFlashbackBeamSpriteSheet = {
    .data = sMewtwoFlashbackBeamGfx,
    .size = 0x400,
    .tag = TAG_MEWTWO_FLASHBACK_BEAM
};

static const struct SpritePalette sMewtwoFlashbackBeamSpritePalette = {
    .data = gFieldEffectPal_SmallSparkle,
    .tag = PAL_TAG_MEWTWO_FLASHBACK_BEAM
};

static const struct SpriteSheet sMewtwoFlashbackChargeCircleSpriteSheet = {
    .data = sMewtwoFlashbackChargeCircleGfx,
    .size = 0x80,
    .tag = TAG_MEWTWO_FLASHBACK_CIRCLE
};

static const struct SpritePalette sMewtwoFlashbackChargeCircleSpritePalette = {
    .data = gFieldEffectPal_SmallSparkle,
    .tag = PAL_TAG_MEWTWO_FLASHBACK_CIRCLE
};

static const struct OamData sMewtwoFlashbackBeamOam = {
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x64),
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x64),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0
};

static const struct OamData sMewtwoFlashbackChargeCircleOam = {
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x16),
    .matrixNum = 0,
    .size = SPRITE_SIZE(16x16),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0
};

static const struct SpriteTemplate sMewtwoFlashbackBeamTemplate = {
    .tileTag = TAG_MEWTWO_FLASHBACK_BEAM,
    .paletteTag = PAL_TAG_MEWTWO_FLASHBACK_BEAM,
    .oam = &sMewtwoFlashbackBeamOam,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

static const struct SpriteTemplate sMewtwoFlashbackChargeCircleTemplate = {
    .tileTag = TAG_MEWTWO_FLASHBACK_CIRCLE,
    .paletteTag = PAL_TAG_MEWTWO_FLASHBACK_CIRCLE,
    .oam = &sMewtwoFlashbackChargeCircleOam,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

static u8 *const sStringVarPtrs[] = {
    gStringVar1,
    gStringVar2,
    gStringVar3
};

void ShowDiploma(void)
{
    QuestLog_CutRecording();
    SetMainCallback2(CB2_ShowDiploma);
    LockPlayerFieldControls();
}

void ForcePlayerOntoBike(void)
{
    if (gPlayerAvatar.flags & PLAYER_AVATAR_FLAG_ON_FOOT)
        SetPlayerAvatarTransitionFlags(PLAYER_AVATAR_FLAG_MACH_BIKE);
    Overworld_SetSavedMusic(MUS_CYCLING);
    Overworld_ChangeMusicTo(MUS_CYCLING);
}

void EndBikeRental(void)
{
    FlagClear(FLAG_BIKE_RENTAL_ACTIVE);
    if (!CheckBagHasItem(ITEM_BICYCLE, 1) && GetPlayerAvatarBike() != 0)
    {
        SetPlayerAvatarTransitionFlags(PLAYER_AVATAR_FLAG_ON_FOOT);
        Overworld_ChangeMusicToDefault();
    }
}

void ResetCyclingRoadChallengeData(void)
{

}

u8 GetPlayerAvatarBike(void)
{
    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_ACRO_BIKE))
        return 1;
    else if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_MACH_BIKE))
        return 2;
    else
        return 0;
}

void ShowFieldMessageStringVar4(void)
{
    ShowFieldMessage(gStringVar4);
}

void GetPlayerXY(void)
{
    gSpecialVar_0x8004 = gSaveBlock1Ptr->pos.x;
    gSpecialVar_0x8005 = gSaveBlock1Ptr->pos.y;
}

u8 GetPlayerTrainerIdOnesDigit(void)
{
    u16 playerId = (gSaveBlock2Ptr->playerTrainerId[1] << 8) | gSaveBlock2Ptr->playerTrainerId[0];
    return playerId % 10;
}

void BufferBigGuyOrBigGirlString(void)
{
    if (gSaveBlock2Ptr->playerGender == MALE)
        StringCopy(gStringVar1, gText_BigGuy);
    else
        StringCopy(gStringVar1, gText_BigGirl);
}

void BufferSonOrDaughterString(void)
{
    if (gSaveBlock2Ptr->playerGender == MALE)
        StringCopy(gStringVar1, gText_Daughter);
    else
        StringCopy(gStringVar1, gText_Son);
}

u8 GetBattleOutcome(void)
{
    return gBattleOutcome;
}

void SetHiddenItemFlag(void)
{
    FlagSet(gSpecialVar_0x8004);
}

u8 GetLeadMonFriendship(void)
{
    struct Pokemon * pokemon = &gPlayerParty[GetLeadMonIndex()];
    if (GetMonData(pokemon, MON_DATA_FRIENDSHIP) == 255)
        return 6;
    else if (GetMonData(pokemon, MON_DATA_FRIENDSHIP) >= 200)
        return 5;
    else if (GetMonData(pokemon, MON_DATA_FRIENDSHIP) >= 150)
        return 4;
    else if (GetMonData(pokemon, MON_DATA_FRIENDSHIP) >= 100)
        return 3;
    else if (GetMonData(pokemon, MON_DATA_FRIENDSHIP) >= 50)
        return 2;
    else if (GetMonData(pokemon, MON_DATA_FRIENDSHIP) > 0)
        return 1;
    else
        return 0;
}

void ShowTownMap(void)
{
    QuestLog_CutRecording();
    InitRegionMapWithExitCB(REGIONMAP_TYPE_WALL, CB2_ReturnToFieldContinueScriptPlayMapMusic);
}

bool8 PlayerHasGrassPokemonInParty(void)
{
    u8 i;
    struct Pokemon * pokemon;
    u16 species;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        pokemon = &gPlayerParty[i];
        if (GetMonData(pokemon, MON_DATA_SANITY_HAS_SPECIES)
         && !GetMonData(pokemon, MON_DATA_IS_EGG)
        )
        {
            species = GetMonData(pokemon, MON_DATA_SPECIES);
            if (gSpeciesInfo[species].types[0] == TYPE_GRASS || gSpeciesInfo[species].types[1] == TYPE_GRASS)
                return TRUE;
        }
    }
    return FALSE;
}

#define tState data[0]
#define tTimer data[1]

void AnimatePcTurnOn(void)
{
    u8 taskId;

    if (FuncIsActiveTask(Task_AnimatePcTurnOn) != TRUE)
    {
        taskId = CreateTask(Task_AnimatePcTurnOn, 8);
        gTasks[taskId].tState = 0;
        gTasks[taskId].tTimer = 0;
    }
}

// PC flickers on and off while turning on
static void Task_AnimatePcTurnOn(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (tTimer == 6)
    {
        PcTurnOnUpdateMetatileId(tState & 1);
        DrawWholeMapView();
        tTimer = 0;
        tState++;
        if (tState == 5)
            DestroyTask(taskId);
    }
    tTimer++;
}

#undef tState
#undef tTimer

static void PcTurnOnUpdateMetatileId(bool16 flickerOff)
{
    u16 metatileId = 0;
    s8 deltaX = 0;
    s8 deltaY = 0;
    u8 direction = GetPlayerFacingDirection();

    switch (direction)
    {
    case DIR_NORTH:
        deltaX = 0;
        deltaY = -1;
        break;
    case DIR_WEST:
        deltaX = -1;
        deltaY = -1;
        break;
    case DIR_EAST:
        deltaX = 1;
        deltaY = -1;
        break;
    }
    if (flickerOff)
    {
        if (gSpecialVar_0x8004 == 0)
            metatileId = METATILE_Building_PCOff;
        else if (gSpecialVar_0x8004 == 1)
            metatileId = METATILE_GenericBuilding1_PlayersPCOff;
        else if (gSpecialVar_0x8004 == 2)
            metatileId = METATILE_GenericBuilding1_PlayersPCOff;
    }
    else
    {
        if (gSpecialVar_0x8004 == 0)
            metatileId = METATILE_Building_PCOn;
        else if (gSpecialVar_0x8004 == 1)
            metatileId = METATILE_GenericBuilding1_PlayersPCOn;
        else if (gSpecialVar_0x8004 == 2)
            metatileId = METATILE_GenericBuilding1_PlayersPCOn;
    }
    MapGridSetMetatileIdAt(gSaveBlock1Ptr->pos.x + deltaX + MAP_OFFSET, gSaveBlock1Ptr->pos.y + deltaY + MAP_OFFSET, metatileId | MAPGRID_COLLISION_MASK);
}

void AnimatePcTurnOff()
{
    u16 metatileId = 0;
    s8 deltaX = 0;
    s8 deltaY = 0;
    u8 direction = GetPlayerFacingDirection();

    switch (direction)
    {
    case DIR_NORTH:
        deltaX = 0;
        deltaY = -1;
        break;
    case DIR_WEST:
        deltaX = -1;
        deltaY = -1;
        break;
    case DIR_EAST:
        deltaX = 1;
        deltaY = -1;
        break;
    }
    if (gSpecialVar_0x8004 == 0)
        metatileId = METATILE_Building_PCOff;
    else if (gSpecialVar_0x8004 == 1)
        metatileId = METATILE_GenericBuilding1_PlayersPCOff;
    else if (gSpecialVar_0x8004 == 2)
        metatileId = METATILE_GenericBuilding1_PlayersPCOff;
    MapGridSetMetatileIdAt(gSaveBlock1Ptr->pos.x + deltaX + MAP_OFFSET, gSaveBlock1Ptr->pos.y + deltaY + MAP_OFFSET, metatileId | MAPGRID_COLLISION_MASK);
    DrawWholeMapView();
}

void SpawnCameraObject(void)
{
    u8 objectEventId = SpawnSpecialObjectEventParameterized(OBJ_EVENT_GFX_YOUNGSTER, 8, LOCALID_CAMERA, gSaveBlock1Ptr->pos.x + MAP_OFFSET, gSaveBlock1Ptr->pos.y + MAP_OFFSET, 3);
    gObjectEvents[objectEventId].invisible = TRUE;
    CameraObjectSetFollowedObjectId(gObjectEvents[objectEventId].spriteId);
}

void SetFlashbackCameraToCoords(void)
{
    u8 objectEventId;
    u16 focusX = gSpecialVar_0x8004 + MAP_OFFSET;
    u16 focusY = gSpecialVar_0x8005 + MAP_OFFSET;

    ClearFlashbackTerrainFieldEffects();
    HideMewtwoFlashbackBeam();
    CacheFlashbackPlayerPosition();
    GetCameraFocusCoords(&sFlashbackCameraFocusX, &sFlashbackCameraFocusY);
    sFlashbackCameraFocusValid = TRUE;
    ResetCameraUpdateInfo();

    SetCameraFocusCoords(focusX, focusY);
    DrawWholeMapView();

    objectEventId = SpawnSpecialObjectEventParameterized(OBJ_EVENT_GFX_YOUNGSTER, 8, LOCALID_CAMERA, focusX, focusY, 3);
    if (objectEventId != OBJECT_EVENTS_COUNT)
    {
        gObjectEvents[objectEventId].invisible = TRUE;
        gSprites[gObjectEvents[objectEventId].spriteId].invisible = TRUE;
        CameraObjectSetFollowedObjectId(gObjectEvents[objectEventId].spriteId);
    }

    ParkFlashbackPlayerObject();
    ClearFlashbackTerrainFieldEffects();
}

void RestoreFlashbackCamera(void)
{
    ClearFlashbackTerrainFieldEffects();
    HideMewtwoFlashbackBeam();
    CameraObjectSetFollowedObjectId(GetPlayerAvatarObjectId());
    RemoveObjectEventByLocalIdAndMap(LOCALID_CAMERA, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);

    if (sFlashbackCameraFocusValid)
    {
        SetCameraFocusCoords(sFlashbackCameraFocusX, sFlashbackCameraFocusY);
        sFlashbackCameraFocusValid = FALSE;
    }

    ResetCameraUpdateInfo();
    RestoreFlashbackPlayerObjectEvents();
    DrawWholeMapView();
    CameraObjectSetFollowedObjectId(GetPlayerAvatarObjectId());
    CameraObjectReset1();
    RestoreFlashbackFieldEffectPalettes();
}

static void RestoreFlashbackPlayerObjectEvents(void)
{
    struct ObjectEvent *playerObj = NULL;

    if (sFlashbackPlayerPositionValid)
    {
        if (gPlayerAvatar.objectEventId < OBJECT_EVENTS_COUNT)
        {
            struct ObjectEvent *oldPlayerObj = &gObjectEvents[gPlayerAvatar.objectEventId];

            if (oldPlayerObj->spriteId < MAX_SPRITES && gSprites[oldPlayerObj->spriteId].inUse)
                DestroySprite(&gSprites[oldPlayerObj->spriteId]);

            oldPlayerObj->active = FALSE;
            oldPlayerObj->isPlayer = FALSE;
        }

        ClearPlayerAvatarInfo();
        InitPlayerAvatar(sFlashbackPlayerX, sFlashbackPlayerY, sFlashbackPlayerDirection, gSaveBlock2Ptr->playerGender);
    }

    if (gPlayerAvatar.objectEventId < OBJECT_EVENTS_COUNT)
    {
        u8 metatileBehavior;

        playerObj = &gObjectEvents[gPlayerAvatar.objectEventId];
        metatileBehavior = MapGridGetMetatileBehaviorAt(playerObj->currentCoords.x, playerObj->currentCoords.y);
        playerObj->previousCoords = playerObj->currentCoords;
        playerObj->currentMetatileBehavior = metatileBehavior;
        playerObj->previousMetatileBehavior = metatileBehavior;
    }

    SetPlayerAvatarStateMask(PLAYER_AVATAR_FLAG_CONTROLLABLE | sFlashbackPlayerTransitionFlags);

    if (gPlayerAvatar.spriteId < MAX_SPRITES)
    {
        InitCameraUpdateCallback(gPlayerAvatar.spriteId);
        CameraObjectSetFollowedObjectId(gPlayerAvatar.spriteId);
        SetPlayerInvisibility(FALSE);
        gSprites[gPlayerAvatar.spriteId].invisible = FALSE;
        gSprites[gPlayerAvatar.spriteId].coordOffsetEnabled = TRUE;
        gSprites[gPlayerAvatar.spriteId].x2 = 0;
        gSprites[gPlayerAvatar.spriteId].y2 = 0;
        if (playerObj != NULL)
            ResetFlashbackPlayerGroundEffectState(playerObj, &gSprites[gPlayerAvatar.spriteId]);
        CameraObjectReset1();
    }

    sFlashbackPlayerPositionValid = FALSE;
    ClearPlayerHeldMovementAndUnfreezeObjectEvents();
    UnlockPlayerFieldControls();
}

static void ResetFlashbackPlayerGroundEffectState(struct ObjectEvent *playerObj, struct Sprite *playerSprite)
{
    static const u8 sDefaultSubspriteTableByElevation[] = {
        1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 0, 0, 1,
    };
    const struct ObjectEventGraphicsInfo *graphicsInfo = GetObjectEventGraphicsInfo(playerObj->graphicsId);
    u8 elevation = playerObj->previousElevation;

    playerObj->singleMovementActive = FALSE;
    playerObj->triggerGroundEffectsOnMove = TRUE;
    playerObj->triggerGroundEffectsOnStop = TRUE;
    playerObj->disableCoveringGroundEffects = FALSE;
    playerObj->hasShadow = FALSE;
    playerObj->hasReflection = FALSE;
    playerObj->inShortGrass = FALSE;
    playerObj->inShallowFlowingWater = FALSE;
    playerObj->inSandPile = FALSE;
    playerObj->inHotSprings = FALSE;
    ObjectEventClearHeldMovement(playerObj);

    if (elevation >= NELEMS(sDefaultSubspriteTableByElevation))
        elevation = 0;

    if (graphicsInfo->subspriteTables != NULL)
    {
        SetSubspriteTables(playerSprite, graphicsInfo->subspriteTables);
        playerSprite->subspriteMode = SUBSPRITES_IGNORE_PRIORITY;
        playerSprite->subspriteTableNum = sDefaultSubspriteTableByElevation[elevation];
    }
    else
    {
        playerSprite->subspriteMode = SUBSPRITES_OFF;
        playerSprite->subspriteTableNum = 0;
    }
}

static void CacheFlashbackPlayerPosition(void)
{
    struct ObjectEvent *playerObj = &gObjectEvents[gPlayerAvatar.objectEventId];

    sFlashbackPlayerX = playerObj->currentCoords.x;
    sFlashbackPlayerY = playerObj->currentCoords.y;
    sFlashbackPlayerDirection = playerObj->facingDirection;
    sFlashbackPlayerTransitionFlags = GetFlashbackPlayerTransitionFlags();
    sFlashbackPlayerPositionValid = TRUE;
}

static void ParkFlashbackPlayerObject(void)
{
    if (gPlayerAvatar.objectEventId >= OBJECT_EVENTS_COUNT)
        return;

    MoveObjectEventToMapCoords(&gObjectEvents[gPlayerAvatar.objectEventId], MAP_OFFSET, MAP_OFFSET);
    ClearFlashbackTerrainFieldEffects();
}

static u16 GetFlashbackPlayerTransitionFlags(void)
{
    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_MACH_BIKE))
        return PLAYER_AVATAR_FLAG_MACH_BIKE;
    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_ACRO_BIKE))
        return PLAYER_AVATAR_FLAG_ACRO_BIKE;
    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING))
        return PLAYER_AVATAR_FLAG_SURFING;
    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_UNDERWATER))
        return PLAYER_AVATAR_FLAG_UNDERWATER;
    return PLAYER_AVATAR_FLAG_ON_FOOT;
}

static void ClearFlashbackTerrainFieldEffects(void)
{
    u8 i;

    for (i = 0; i < MAX_SPRITES; i++)
    {
        if (!gSprites[i].inUse)
            continue;

        if (gSprites[i].callback == UpdateTallGrassFieldEffect)
        {
            gSprites[i].invisible = TRUE;
            FieldEffectStop(&gSprites[i], FLDEFF_TALL_GRASS);
        }
        else if (gSprites[i].callback == UpdateLongGrassFieldEffect)
        {
            gSprites[i].invisible = TRUE;
            FieldEffectStop(&gSprites[i], FLDEFF_LONG_GRASS);
        }
        else if (gSprites[i].callback == UpdateShortGrassFieldEffect)
        {
            gSprites[i].invisible = TRUE;
            FieldEffectStop(&gSprites[i], FLDEFF_SHORT_GRASS);
        }
        else if (gSprites[i].callback == UpdateShadowFieldEffect
              && gSprites[i].data[0] == LOCALID_PLAYER)
        {
            gSprites[i].invisible = TRUE;
            FieldEffectStop(&gSprites[i], FLDEFF_SHADOW);
        }
        else if (gSprites[i].callback == UpdateJumpImpactEffect
              && (gSprites[i].data[1] == FLDEFF_JUMP_TALL_GRASS
               || gSprites[i].data[1] == FLDEFF_JUMP_LONG_GRASS
               || gSprites[i].data[1] == FLDEFF_DUST))
        {
            gSprites[i].invisible = TRUE;
            FieldEffectStop(&gSprites[i], gSprites[i].data[1]);
        }
        else if (gSprites[i].callback == UpdateSparkleFieldEffect)
        {
            gSprites[i].invisible = TRUE;
            FieldEffectStop(&gSprites[i], FLDEFF_SPARKLE);
        }
        else if (IsFlashbackFieldEffectSprite(&gSprites[i]))
        {
            gSprites[i].invisible = TRUE;
        }
    }

    FieldEffectActiveListRemove(FLDEFF_JUMP_TALL_GRASS);
    FieldEffectActiveListRemove(FLDEFF_JUMP_LONG_GRASS);
    FieldEffectActiveListRemove(FLDEFF_DUST);
    FieldEffectActiveListRemove(FLDEFF_SPARKLE);
    FieldEffectActiveListRemove(FLDEFF_TALL_GRASS);
    FieldEffectActiveListRemove(FLDEFF_LONG_GRASS);
    FieldEffectActiveListRemove(FLDEFF_SHORT_GRASS);
    FieldEffectActiveListRemove(FLDEFF_SHADOW);
}

static bool8 IsFlashbackFieldEffectSprite(const struct Sprite *sprite)
{
    u8 general0Slot = IndexOfSpritePaletteTag(FLDEFF_PAL_TAG_GENERAL_0);
    u8 general1Slot = IndexOfSpritePaletteTag(FLDEFF_PAL_TAG_GENERAL_1);

    return (general0Slot != 0xFF && sprite->oam.paletteNum == general0Slot)
        || (general1Slot != 0xFF && sprite->oam.paletteNum == general1Slot);
}

static void SpawnMewtwoFlashbackSparkle(s16 x, s16 y, u8 priority)
{
    gFieldEffectArguments[0] = x;
    gFieldEffectArguments[1] = y;
    gFieldEffectArguments[2] = priority;
    FieldEffectStart(FLDEFF_SPARKLE);
    RetintFlashbackEffectPalettes();
}

static void SpawnMewtwoFlashbackDust(s16 x, s16 y, u8 priority)
{
    u8 spriteId;
    struct Sprite *sprite;

    x += MAP_OFFSET;
    y += MAP_OFFSET;
    SetSpritePosToOffsetMapCoords(&x, &y, 8, 12);
    LoadSpritePalette(&gSpritePalette_GeneralFieldEffect0);
    spriteId = CreateSpriteAtEnd(gFieldEffectObjectTemplatePointers[FLDEFFOBJ_GROUND_IMPACT_DUST], x, y, 0x51);
    if (spriteId != MAX_SPRITES)
    {
        sprite = &gSprites[spriteId];
        sprite->coordOffsetEnabled = TRUE;
        sprite->oam.priority = priority;
        sprite->callback = SpriteCB_MewtwoFlashbackDust;
        sprite->data[1] = FLDEFF_DUST;
    }
    RetintFlashbackEffectPalettes();
}

static void SpriteCB_MewtwoFlashbackDust(struct Sprite *sprite)
{
    if (sprite->animEnded)
        DestroySprite(sprite);
    else
        UpdateObjectEventSpriteInvisibility(sprite, FALSE);
}

static void RetintFlashbackEffectPalettes(void)
{
    if (sFlashbackPaletteBackupValid)
    {
        TintFlashbackPaletteBuffer(gPlttBufferUnfaded);
        TintFlashbackPaletteBuffer(gPlttBufferFaded);
    }
}

static void TintFlashbackPaletteBuffer(u16 *palette)
{
    TintPalette_GrayScale(&palette[BG_PLTT_ID(0)], FLASHBACK_BG_GREYSCALE_PALETTE_COUNT * 16);
    TintPalette_GrayScale(&palette[OBJ_PLTT_ID(0)], OBJ_PLTT_SIZE / sizeof(u16));
}

static void RestoreFlashbackFieldEffectPalettes(void)
{
    RestoreFlashbackFieldEffectPalette(&gSpritePalette_GeneralFieldEffect0);
    RestoreFlashbackFieldEffectPalette(&gSpritePalette_GeneralFieldEffect1);
}

static void RestoreFlashbackFieldEffectPalette(const struct SpritePalette *spritePalette)
{
    u8 paletteSlot = IndexOfSpritePaletteTag(spritePalette->tag);
    u16 paletteOffset;

    if (paletteSlot == 0xFF)
        return;

    paletteOffset = OBJ_PLTT_ID(paletteSlot);
    CpuFastCopy(spritePalette->data, &gPlttBufferUnfaded[paletteOffset], PLTT_SIZE_4BPP);
    CpuFastCopy(spritePalette->data, &gPlttBufferFaded[paletteOffset], PLTT_SIZE_4BPP);
    ApplyGlobalFieldPaletteTint(paletteSlot);
    UpdateSpritePaletteWithWeather(paletteSlot);
    if (spritePalette->tag == FLDEFF_PAL_TAG_GENERAL_0)
        ApplyNightTintToSandFootprintsEffect();
    else if (spritePalette->tag == FLDEFF_PAL_TAG_GENERAL_1)
        ApplyNightTintToTallGrassEffect();
    CpuFastCopy(&gPlttBufferFaded[paletteOffset], (void *)(OBJ_PLTT + paletteOffset * sizeof(u16)), PLTT_SIZE_4BPP);
}

static void WhitenFlashbackFieldEffectPalettes(void)
{
    WhitenFlashbackFieldEffectPalette(&gSpritePalette_GeneralFieldEffect0);
    WhitenFlashbackFieldEffectPalette(&gSpritePalette_GeneralFieldEffect1);
}

static void WhitenFlashbackFieldEffectPalette(const struct SpritePalette *spritePalette)
{
    u8 paletteSlot = IndexOfSpritePaletteTag(spritePalette->tag);
    u16 paletteOffset;

    if (paletteSlot == 0xFF)
        return;

    paletteOffset = OBJ_PLTT_ID(paletteSlot);
    CpuFastFill16(RGB_WHITE, &gPlttBufferUnfaded[paletteOffset], PLTT_SIZE_4BPP);
    CpuFastFill16(RGB_WHITE, &gPlttBufferFaded[paletteOffset], PLTT_SIZE_4BPP);
    CpuFastFill16(RGB_WHITE, (void *)(OBJ_PLTT + paletteOffset * sizeof(u16)), PLTT_SIZE_4BPP);
}

void StartMewtwoFlashbackSparkleLoop(void)
{
    u8 taskId = FindTaskIdByFunc(Task_MewtwoFlashbackSparkleLoop);

    if (taskId != TASK_NONE)
        DestroyTask(taskId);

    taskId = CreateTask(Task_MewtwoFlashbackSparkleLoop, 8);
    gTasks[taskId].data[0] = 0;
    gTasks[taskId].data[1] = MEWTWO_FLASHBACK_EFFECT_X;
    gTasks[taskId].data[2] = MEWTWO_FLASHBACK_EFFECT_Y;
    gTasks[taskId].data[3] = MEWTWO_FLASHBACK_EFFECT_PRIORITY;
    gTasks[taskId].data[4] = 48;
    SpawnMewtwoFlashbackSparkle(gTasks[taskId].data[1], gTasks[taskId].data[2], gTasks[taskId].data[3]);
    SpawnMewtwoFlashbackDust(gTasks[taskId].data[1], gTasks[taskId].data[2], gTasks[taskId].data[3]);
}

void AccelerateMewtwoFlashbackSparkleLoop(void)
{
    u8 taskId = FindTaskIdByFunc(Task_MewtwoFlashbackSparkleLoop);

    if (taskId != TASK_NONE)
    {
        gTasks[taskId].data[0] = 0;
        gTasks[taskId].data[4] = 16;
        SpawnMewtwoFlashbackSparkle(gTasks[taskId].data[1], gTasks[taskId].data[2], gTasks[taskId].data[3]);
        SpawnMewtwoFlashbackDust(gTasks[taskId].data[1], gTasks[taskId].data[2], gTasks[taskId].data[3]);
    }
}

void StopMewtwoFlashbackSparkleLoop(void)
{
    u8 taskId = FindTaskIdByFunc(Task_MewtwoFlashbackSparkleLoop);

    if (taskId != TASK_NONE)
        DestroyTask(taskId);
}

void ShowMewtwoFlashbackChargeCircle(void)
{
    HideMewtwoFlashbackChargeCircle();
    LoadSpriteSheet(&sMewtwoFlashbackChargeCircleSpriteSheet);
    LoadSpritePalette(&sMewtwoFlashbackChargeCircleSpritePalette);

    sMewtwoFlashbackChargeCircleSpriteId = CreateSprite(&sMewtwoFlashbackChargeCircleTemplate,
                                                        MEWTWO_FLASHBACK_CIRCLE_SCREEN_X,
                                                        MEWTWO_FLASHBACK_CIRCLE_SCREEN_Y,
                                                        0);
    if (sMewtwoFlashbackChargeCircleSpriteId != MAX_SPRITES)
        gSprites[sMewtwoFlashbackChargeCircleSpriteId].oam.priority = 0;

    RetintFlashbackEffectPalettes();
}

static void HideMewtwoFlashbackChargeCircle(void)
{
    if (sMewtwoFlashbackChargeCircleSpriteId != MAX_SPRITES)
    {
        DestroySprite(&gSprites[sMewtwoFlashbackChargeCircleSpriteId]);
        sMewtwoFlashbackChargeCircleSpriteId = MAX_SPRITES;
    }
    FreeSpriteTilesByTag(TAG_MEWTWO_FLASHBACK_CIRCLE);
    FreeSpritePaletteByTag(PAL_TAG_MEWTWO_FLASHBACK_CIRCLE);
}

void ShowMewtwoFlashbackBeam(void)
{
    u8 i;

    HideMewtwoFlashbackBeam();
    LoadSpriteSheet(&sMewtwoFlashbackBeamSpriteSheet);
    LoadSpritePalette(&sMewtwoFlashbackBeamSpritePalette);

    for (i = 0; i < MEWTWO_FLASHBACK_BEAM_SPRITE_COUNT; i++)
    {
        sMewtwoFlashbackBeamSpriteIds[i] = CreateSprite(&sMewtwoFlashbackBeamTemplate, 120, 24 - i * 64, 0);
        if (sMewtwoFlashbackBeamSpriteIds[i] != MAX_SPRITES)
        {
            gSprites[sMewtwoFlashbackBeamSpriteIds[i]].oam.priority = 0;
            gSprites[sMewtwoFlashbackBeamSpriteIds[i]].invisible = TRUE;
        }
    }
    ShowMewtwoFlashbackChargeCircle();
    RetintFlashbackEffectPalettes();
    CreateTask(Task_RevealMewtwoFlashbackBeam, 8);
}

void HideMewtwoFlashbackBeam(void)
{
    u8 i;
    u8 taskId = FindTaskIdByFunc(Task_RevealMewtwoFlashbackBeam);

    if (taskId != TASK_NONE)
        DestroyTask(taskId);

    for (i = 0; i < MEWTWO_FLASHBACK_BEAM_SPRITE_COUNT; i++)
    {
        if (sMewtwoFlashbackBeamSpriteIds[i] != MAX_SPRITES)
        {
            DestroySprite(&gSprites[sMewtwoFlashbackBeamSpriteIds[i]]);
            sMewtwoFlashbackBeamSpriteIds[i] = MAX_SPRITES;
        }
    }
    FreeSpriteTilesByTag(TAG_MEWTWO_FLASHBACK_BEAM);
    FreeSpritePaletteByTag(PAL_TAG_MEWTWO_FLASHBACK_BEAM);
    HideMewtwoFlashbackChargeCircle();
}

void SpawnFlashbackDustAtCoords(void)
{
    SpawnMewtwoFlashbackDust(gSpecialVar_0x8004, gSpecialVar_0x8005, 0);
}

void HoldFlashbackFadeWhite(void)
{
    ClearFlashbackTerrainFieldEffects();
    WhitenFlashbackFieldEffectPalettes();
    CpuFastFill16(RGB_WHITE, gPlttBufferFaded, PLTT_SIZE);
    CpuFastFill16(RGB_WHITE, (void *)PLTT, PLTT_SIZE);
}

void SlowEliteFourFlashbackMusic(void)
{
    m4aMPlayTempoControl(&gMPlayInfo_BGM, 0x40);
}

void RestoreEliteFourFlashbackMusic(void)
{
    m4aMPlayTempoControl(&gMPlayInfo_BGM, 0x100);
}

void SuppressFlashbackPlayerGroundEffects(void)
{
    u8 i;

    sFlashbackSuppressTerrainEffects = TRUE;
    WhitenFlashbackFieldEffectPalettes();

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        if (!gObjectEvents[i].active)
            continue;

        gObjectEvents[i].triggerGroundEffectsOnMove = FALSE;
        gObjectEvents[i].triggerGroundEffectsOnStop = FALSE;
        gObjectEvents[i].disableCoveringGroundEffects = TRUE;
        gObjectEvents[i].inShortGrass = FALSE;
        gObjectEvents[i].inShallowFlowingWater = FALSE;
        gObjectEvents[i].inSandPile = FALSE;
        gObjectEvents[i].inHotSprings = FALSE;
    }

    ClearFlashbackTerrainFieldEffects();
}

void RestoreFlashbackPlayerGroundEffects(void)
{
    u8 i;

    ClearFlashbackTerrainFieldEffects();
    RestoreFlashbackFieldEffectPalettes();
    sFlashbackSuppressTerrainEffects = FALSE;

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        if (!gObjectEvents[i].active)
            continue;

        gObjectEvents[i].triggerGroundEffectsOnMove = TRUE;
        gObjectEvents[i].triggerGroundEffectsOnStop = TRUE;
        gObjectEvents[i].disableCoveringGroundEffects = FALSE;
    }
}

bool8 AreFlashbackTerrainFieldEffectsSuppressed(void)
{
    return sFlashbackSuppressTerrainEffects;
}

static void Task_RevealMewtwoFlashbackBeam(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u8 spriteId;

    if (data[0] == 0)
    {
        spriteId = sMewtwoFlashbackBeamSpriteIds[0];
        if (spriteId != MAX_SPRITES)
            gSprites[spriteId].invisible = FALSE;
    }
    else if (data[0] == 4)
    {
        spriteId = sMewtwoFlashbackBeamSpriteIds[1];
        if (spriteId != MAX_SPRITES)
            gSprites[spriteId].invisible = FALSE;
        DestroyTask(taskId);
        return;
    }

    data[0]++;
}

static void Task_MewtwoFlashbackSparkleLoop(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (++data[0] >= data[4])
    {
        data[0] = 0;
        SpawnMewtwoFlashbackSparkle(data[1], data[2], data[3]);
        SpawnMewtwoFlashbackDust(data[1], data[2], data[3]);
    }
}

void RemoveCameraObject(void)
{
    CameraObjectSetFollowedObjectId(GetPlayerAvatarObjectId());
    RemoveObjectEventByLocalIdAndMap(LOCALID_CAMERA, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
}

void BufferEReaderTrainerName(void)
{
    CopyEReaderTrainerName5(gStringVar1);
}

// Unused
static const u8 sSlotMachineRandomSeeds[] = {
    13,
    14,
    15,
    16,
    17,
    18,
    19,
    20,
    21,
    22,
    12,
    2,
    4,
    5,
    1,
    8,
    7,
    11,
    3,
    10,
    9,
    6
};

static const u8 sSlotMachineIndices[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    1,
    2,
    2,
    2,
    3,
    3,
    3,
    4,
    4,
    5
};

u8 GetRandomSlotMachineId(void)
{
    u16 rval = Random() % NELEMS(sSlotMachineIndices);
    return sSlotMachineIndices[rval];
}

bool8 LeadMonHasEffortRibbon(void)
{
    u8 leadMonIdx = GetLeadMonIndex();
    return GetMonData(&gPlayerParty[leadMonIdx], MON_DATA_EFFORT_RIBBON, NULL);
}

void GiveLeadMonEffortRibbon(void)
{
    u8 leadMonIdx;
    bool8 param;
    IncrementGameStat(GAME_STAT_RECEIVED_RIBBONS);
    FlagSet(FLAG_SYS_RIBBON_GET);
    param = TRUE;
    leadMonIdx = GetLeadMonIndex();
    SetMonData(&gPlayerParty[leadMonIdx], MON_DATA_EFFORT_RIBBON, &param);
}

bool8 AreLeadMonEVsMaxedOut(void)
{
    u8 leadMonIndex = GetLeadMonIndex();
    if (GetMonEVCount(&gPlayerParty[leadMonIndex]) >= 510)
        return TRUE;
    else
        return FALSE;
}

bool8 IsStarterFirstStageInParty(void)
{
    u16 species = GetStarterSpeciesById(VarGet(VAR_STARTER_MON));
    u8 partyCount = CalculatePlayerPartyCount();
    u8 i;
    for (i = 0; i < partyCount; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES_OR_EGG, NULL) == species)
            return TRUE;
    }
    return FALSE;
}

bool8 IsThereRoomInAnyBoxForMorePokemon(void)
{
    u16 i;
    u16 j;
    for (i = 0; i < TOTAL_BOXES_COUNT; i++)
    {
        for (j = 0; j < IN_BOX_COUNT; j++)
        {
            if (GetBoxMonDataAt(i, j, MON_DATA_SPECIES) == SPECIES_NONE)
                return TRUE;
        }
    }
    return FALSE;
}

bool8 IsPokerusInParty(void)
{
    if (!CheckPartyPokerus(gPlayerParty, 0x3F))
        return FALSE;
    else
        return TRUE;
}

#define tXtrans   data[0]
#define tTimer    data[1]
#define tNremain  data[2]
#define tDuration data[3]
#define tYtrans   data[4]

void ShakeScreen(void)
{
    /*
     * 0x8004 = x translation
     * 0x8005 = y translation
     * 0x8006 = num interations
     * 0x8007 = duration of an iteration
     */
    u8 taskId = CreateTask(Task_ShakeScreen, 9);
    gTasks[taskId].tXtrans = gSpecialVar_0x8005;
    gTasks[taskId].tTimer = 0;
    gTasks[taskId].tNremain = gSpecialVar_0x8006;
    gTasks[taskId].tDuration = gSpecialVar_0x8007;
    gTasks[taskId].tYtrans = gSpecialVar_0x8004;
    SetCameraPanningCallback(NULL);
    PlaySE(SE_M_STRENGTH);
}

static void Task_ShakeScreen(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    tTimer++;
    if (tTimer % tDuration == 0)
    {
        tTimer = 0;
        tNremain--;
        tXtrans = -tXtrans;
        tYtrans = -tYtrans;
        SetCameraPanning(tXtrans, tYtrans);
        if (tNremain == 0)
        {
            Task_EndScreenShake(taskId);
            InstallCameraPanAheadCallback();
        }
    }
}

static void Task_EndScreenShake(u8 taskId)
{
    DestroyTask(taskId);
    ScriptContext_Enable();
}

#undef tYtrans
#undef tDuration
#undef tNremain
#undef tTimer
#undef tXtrans

u8 GetLeadMonIndex(void)
{
    u8 partyCount = CalculatePlayerPartyCount();
    u8 i;
    struct Pokemon * pokemon;
    for (i = 0; i < partyCount; i++)
    {
        pokemon = &gPlayerParty[i];
        if (GetMonData(pokemon, MON_DATA_SPECIES_OR_EGG, NULL) != SPECIES_EGG && GetMonData(pokemon, MON_DATA_SPECIES_OR_EGG, NULL) != SPECIES_NONE)
            return i;
    }
    return 0;
}

u16 GetPartyMonSpecies(void)
{
    return GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_SPECIES_OR_EGG, NULL);
}

bool8 IsMonOTNameNotPlayers(void)
{
    GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_OT_NAME, gStringVar1);
    
    if (!StringCompare(gSaveBlock2Ptr->playerName, gStringVar1))
        return FALSE;
    else
        return TRUE;
}

// Used to nop all the unused specials from RS
void NullFieldSpecial(void)
{

}

void DoPicboxCancel(void)
{
    u8 t = EOS;
    AddTextPrinterParameterized(0, FONT_NORMAL, &t, 0, 1, 0, NULL);
    PicboxCancel();
}

static const u16 sPokemonRequestDeluxeRewards[] = {
    ITEM_BIG_PEARL,
    ITEM_PEARL,
    ITEM_STARDUST,
    ITEM_STAR_PIECE,
    ITEM_NUGGET,
    ITEM_RARE_CANDY
};

void IncrementPokemonRequestRewardStepCounter(void)
{
    u16 stepCounter = VarGet(VAR_POKEMON_REQUEST_REWARD_STEP_COUNTER);
    if (VarGet(VAR_POKEMON_REQUEST_REWARD_MON) != SPECIES_NONE)
    {
        stepCounter++;
        if (stepCounter >= 250)
        {
            VarSet(VAR_POKEMON_REQUEST_REWARD_MON, 0xFFFF);
            VarSet(VAR_POKEMON_REQUEST_REWARD_STEP_COUNTER, 0);
        }
        else
        {
            VarSet(VAR_POKEMON_REQUEST_REWARD_STEP_COUNTER, stepCounter);
        }
    }
}

void SamplePokemonRequestMonAndReward(void)
{
    u16 requestedSpecies = VarGet(VAR_POKEMON_REQUEST_REWARD_MON);
    if (requestedSpecies == SPECIES_NONE || requestedSpecies == 0xFFFF)
    {
        VarSet(VAR_POKEMON_REQUEST_REWARD_MON, SamplePokemonRequestMon());
        VarSet(VAR_POKEMON_REQUEST_REWARD_ITEM, SamplePokemonRequestReward());
        VarSet(VAR_POKEMON_REQUEST_REWARD_STEP_COUNTER, 0);
    }
    StringCopy(gStringVar1, gSpeciesNames[VarGet(VAR_POKEMON_REQUEST_REWARD_MON)]);
}

static u16 SamplePokemonRequestMon(void)
{
    u16 i;
    u16 species;
    for (i = 0; i < 100; i++)
    {
        species = (Random() % (NUM_SPECIES - 1)) + 1;
        if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), 0) == TRUE)
            return species;
    }
    while (GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), 0) != TRUE)
    {
        if (species == SPECIES_BULBASAUR)
            species = NUM_SPECIES - 1;
        else
            species--;
    }
    return species;
}

static u16 SamplePokemonRequestReward(void)
{
    if ((Random() % 100) >= 30)
        return ITEM_LUXURY_BALL;
    else
        return sPokemonRequestDeluxeRewards[Random() % NELEMS(sPokemonRequestDeluxeRewards)];
}

bool8 CheckAddCoins(void)
{
    if (gSpecialVar_Result + gSpecialVar_0x8006 > 9999)
        return FALSE;
    else
        return TRUE;
}

static const struct WindowTemplate sElevatorCurrentFloorWindowTemplate = {
    .bg = 0,
    .tilemapLeft = 22,
    .tilemapTop = 1,
    .width = 7,
    .height = 4,
    .paletteNum = 15,
    .baseBlock = 0x008
};

static const u8 *const sFloorNamePointers[] = {
    gText_B4F,
    gText_B3F,
    gText_B2F,
    gText_B1F,
    gText_1F,
    gText_2F,
    gText_3F,
    gText_4F,
    gText_5F,
    gText_6F,
    gText_7F,
    gText_8F,
    gText_9F,
    gText_10F,
    gText_11F,
    gText_Rooftop
};

// Unused
static const u8 sFloorNameWidthPadding[] = {
    26,
    26,
    26,
    26,
    30,
    30,
    30,
    30,
    30,
    30,
    30,
    30,
    30,
    26,
    26,
    18
};

static const u16 sElevatorWindowMetatilesGoingUp[][3] = {
    {
        METATILE_SilphCo_ElevatorWindow_Top0, 
        METATILE_SilphCo_ElevatorWindow_Top1, 
        METATILE_SilphCo_ElevatorWindow_Top2
    },
    {
        METATILE_SilphCo_ElevatorWindow_Mid0, 
        METATILE_SilphCo_ElevatorWindow_Mid1, 
        METATILE_SilphCo_ElevatorWindow_Mid2
    },
    {
        METATILE_SilphCo_ElevatorWindow_Bottom0, 
        METATILE_SilphCo_ElevatorWindow_Bottom1, 
        METATILE_SilphCo_ElevatorWindow_Bottom2
    }
};

static const u16 sElevatorWindowMetatilesGoingDown[][3] = {
    {
        METATILE_SilphCo_ElevatorWindow_Top0, 
        METATILE_SilphCo_ElevatorWindow_Top2, 
        METATILE_SilphCo_ElevatorWindow_Top1
    },
    {
        METATILE_SilphCo_ElevatorWindow_Mid0, 
        METATILE_SilphCo_ElevatorWindow_Mid2, 
        METATILE_SilphCo_ElevatorWindow_Mid1
    },
    {
        METATILE_SilphCo_ElevatorWindow_Bottom0, 
        METATILE_SilphCo_ElevatorWindow_Bottom2, 
        METATILE_SilphCo_ElevatorWindow_Bottom1
    }
};

static const u8 sElevatorAnimationDuration[] = {
    8,
    16,
    24,
    32,
    38,
    46,
    53,
    56,
    57
};

static const u8 sElevatorWindowAnimDuration[] = {
    3,
    6,
    9,
    12,
    15,
    18,
    21,
    24,
    27
};

void GetElevatorFloor(void)
{
    u16 floor = 4;
    if (gSaveBlock1Ptr->dynamicWarp.mapGroup == MAP_GROUP(MAP_SILPH_CO_1F))
    {
        switch (gSaveBlock1Ptr->dynamicWarp.mapNum)
        {
        case MAP_NUM(MAP_SILPH_CO_1F):
            floor = 4;
            break;
        case MAP_NUM(MAP_SILPH_CO_2F):
            floor = 5;
            break;
        case MAP_NUM(MAP_SILPH_CO_3F):
            floor = 6;
            break;
        case MAP_NUM(MAP_SILPH_CO_4F):
            floor = 7;
            break;
        case MAP_NUM(MAP_SILPH_CO_5F):
            floor = 8;
            break;
        case MAP_NUM(MAP_SILPH_CO_6F):
            floor = 9;
            break;
        case MAP_NUM(MAP_SILPH_CO_7F):
            floor = 10;
            break;
        case MAP_NUM(MAP_SILPH_CO_8F):
            floor = 11;
            break;
        case MAP_NUM(MAP_SILPH_CO_9F):
            floor = 12;
            break;
        case MAP_NUM(MAP_SILPH_CO_10F):
            floor = 13;
            break;
        case MAP_NUM(MAP_SILPH_CO_11F):
            floor = 14;
            break;
        }
    }
    if (gSaveBlock1Ptr->dynamicWarp.mapGroup == MAP_GROUP(MAP_CELADON_CITY_DEPARTMENT_STORE_1F))
    {
        switch (gSaveBlock1Ptr->dynamicWarp.mapNum)
        {
        case MAP_NUM(MAP_CELADON_CITY_DEPARTMENT_STORE_1F):
            floor = 4;
            break;
        case MAP_NUM(MAP_CELADON_CITY_DEPARTMENT_STORE_2F):
            floor = 5;
            break;
        case MAP_NUM(MAP_CELADON_CITY_DEPARTMENT_STORE_3F):
            floor = 6;
            break;
        case MAP_NUM(MAP_CELADON_CITY_DEPARTMENT_STORE_4F):
            floor = 7;
            break;
        case MAP_NUM(MAP_CELADON_CITY_DEPARTMENT_STORE_5F):
            floor = 8;
            break;
        }
    }
    VarSet(VAR_ELEVATOR_FLOOR, floor);
}

u16 InitElevatorFloorSelectMenuPos(void)
{
    sElevatorScroll = 0;
    sElevatorCursorPos = 0;

    if (gSaveBlock1Ptr->dynamicWarp.mapGroup == MAP_GROUP(MAP_SILPH_CO_1F))
    {
        switch (gSaveBlock1Ptr->dynamicWarp.mapNum)
        {
        case MAP_NUM(MAP_SILPH_CO_11F):
            sElevatorScroll = 0;
            sElevatorCursorPos = 0;
            break;
        case MAP_NUM(MAP_SILPH_CO_10F):
            sElevatorScroll = 0;
            sElevatorCursorPos = 1;
            break;
        case MAP_NUM(MAP_SILPH_CO_9F):
            sElevatorScroll = 0;
            sElevatorCursorPos = 2;
            break;
        case MAP_NUM(MAP_SILPH_CO_8F):
            sElevatorScroll = 0;
            sElevatorCursorPos = 3;
            break;
        case MAP_NUM(MAP_SILPH_CO_7F):
            sElevatorScroll = 0;
            sElevatorCursorPos = 4;
            break;
        case MAP_NUM(MAP_SILPH_CO_6F):
            sElevatorScroll = 1;
            sElevatorCursorPos = 4;
            break;
        case MAP_NUM(MAP_SILPH_CO_5F):
            sElevatorScroll = 2;
            sElevatorCursorPos = 4;
            break;
        case MAP_NUM(MAP_SILPH_CO_4F):
            sElevatorScroll = 3;
            sElevatorCursorPos = 4;
            break;
        case MAP_NUM(MAP_SILPH_CO_3F):
            sElevatorScroll = 4;
            sElevatorCursorPos = 4;
            break;
        case MAP_NUM(MAP_SILPH_CO_2F):
            sElevatorScroll = 5;
            sElevatorCursorPos = 4;
            break;
        case MAP_NUM(MAP_SILPH_CO_1F):
            sElevatorScroll = 5;
            sElevatorCursorPos = 5;
            break;
        }
    }
    if (gSaveBlock1Ptr->dynamicWarp.mapGroup == MAP_GROUP(MAP_CELADON_CITY_DEPARTMENT_STORE_1F))
    {
        switch (gSaveBlock1Ptr->dynamicWarp.mapNum)
        {
        case MAP_NUM(MAP_CELADON_CITY_DEPARTMENT_STORE_5F):
            sElevatorScroll = 0;
            sElevatorCursorPos = 0;
            break;
        case MAP_NUM(MAP_CELADON_CITY_DEPARTMENT_STORE_4F):
            sElevatorScroll = 0;
            sElevatorCursorPos = 1;
            break;
        case MAP_NUM(MAP_CELADON_CITY_DEPARTMENT_STORE_3F):
            sElevatorScroll = 0;
            sElevatorCursorPos = 2;
            break;
        case MAP_NUM(MAP_CELADON_CITY_DEPARTMENT_STORE_2F):
            sElevatorScroll = 0;
            sElevatorCursorPos = 3;
            break;
        case MAP_NUM(MAP_CELADON_CITY_DEPARTMENT_STORE_1F):
            sElevatorScroll = 0;
            sElevatorCursorPos = 4;
            break;
        }
    }
    return sElevatorCursorPos;
}

void AnimateElevator(void)
{
    u16 nfloors;
    s16 *data = gTasks[CreateTask(Task_ElevatorShake, 9)].data;
    data[1] = 0;
    data[2] = 0;
    data[4] = 1;
    if (gSpecialVar_0x8005 > gSpecialVar_0x8006)
    {
        nfloors = gSpecialVar_0x8005 - gSpecialVar_0x8006;
        data[6] = 1;
    }
    else
    {
        nfloors = gSpecialVar_0x8006 - gSpecialVar_0x8005;
        data[6] = 0;
    }
    if (nfloors > 8)
        nfloors = 8;
    data[5] = sElevatorAnimationDuration[nfloors];
    SetCameraPanningCallback(NULL);
    AnimateElevatorWindowView(nfloors, data[6]);
    PlaySE(SE_ELEVATOR);
}

static void Task_ElevatorShake(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    data[1]++;
    if ((data[1] % 3) == 0)
    {
        data[1] = 0;
        data[2]++;
        data[4] = -data[4];
        SetCameraPanning(0, data[4]);
        if (data[2] == data[5])
        {
            PlaySE(SE_DING_DONG);
            DestroyTask(taskId);
            ScriptContext_Enable();
            InstallCameraPanAheadCallback();
        }
    }
}

void DrawElevatorCurrentFloorWindow(void)
{
    const u8 *floorname;
    u32 strwidth;

    if (QL_AvoidDisplay(QL_DestroyAbortedDisplay) == TRUE)
        return;

    sElevatorCurrentFloorWindowId = AddWindow(&sElevatorCurrentFloorWindowTemplate);
    LoadStdWindowGfx(sElevatorCurrentFloorWindowId, 0x21D, BG_PLTT_ID(13));
    DrawStdFrameWithCustomTileAndPalette(sElevatorCurrentFloorWindowId, FALSE, 0x21D, 13);
    AddTextPrinterParameterized(sElevatorCurrentFloorWindowId, FONT_NORMAL, gText_NowOn, 0, 2, 0xFF, NULL);
    floorname = sFloorNamePointers[gSpecialVar_0x8005];
    strwidth = GetStringWidth(FONT_NORMAL, floorname, 0);
    AddTextPrinterParameterized(sElevatorCurrentFloorWindowId, FONT_NORMAL, floorname, 56 - strwidth, 16, 0xFF, NULL);
    PutWindowTilemap(sElevatorCurrentFloorWindowId);
    CopyWindowToVram(sElevatorCurrentFloorWindowId, COPYWIN_FULL);
}

void CloseElevatorCurrentFloorWindow(void)
{
    ClearStdWindowAndFrameToTransparent(sElevatorCurrentFloorWindowId, TRUE);
    RemoveWindow(sElevatorCurrentFloorWindowId);
}

static void AnimateElevatorWindowView(u16 nfloors, u8 direction)
{
    u8 taskId;
    if (FuncIsActiveTask(Task_AnimateElevatorWindowView) != TRUE)
    {
        taskId = CreateTask(Task_AnimateElevatorWindowView, 8);
        gTasks[taskId].data[0] = 0;
        gTasks[taskId].data[1] = 0;
        gTasks[taskId].data[2] = direction;
        gTasks[taskId].data[3] = sElevatorWindowAnimDuration[nfloors];
    }
}

static void Task_AnimateElevatorWindowView(u8 taskId)
{
    u8 i;
    u8 j;
    s16 *data = gTasks[taskId].data;
    if (data[1] == 6)
    {
        data[0]++;
        if (data[2] == 0)
        {
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 3; j++)
                    MapGridSetMetatileIdAt(j + 1 + MAP_OFFSET, i + MAP_OFFSET, sElevatorWindowMetatilesGoingUp[i][data[0] % 3] | MAPGRID_COLLISION_MASK);
            }
        }
        else
        {
            for (i = 0; i < 3; i++)
            {
                for (j = 0; j < 3; j++)
                    MapGridSetMetatileIdAt(j + 1 + MAP_OFFSET, i + MAP_OFFSET, sElevatorWindowMetatilesGoingDown[i][data[0] % 3] | MAPGRID_COLLISION_MASK);
            }
        }
        DrawWholeMapView();
        data[1] = 0;
        if (data[0] == data[3])
            DestroyTask(taskId);
    }
    data[1]++;
}

void ListMenu(void)
{
    u8 taskId;
    struct Task *task;

    if (QL_AvoidDisplay(QL_DestroyAbortedDisplay) == TRUE)
        return;
        
    taskId = CreateTask(Task_CreateScriptListMenu, 8);
    task = &gTasks[taskId];
    switch (gSpecialVar_0x8004)
    {
    case LISTMENU_BADGES:
        task->data[0] = 4;
        task->data[1] = 9;
        task->data[2] = 1;
        task->data[3] = 1;
        task->data[4] = 12;
        task->data[5] = 7;
        task->data[6] = 1;
        task->data[15] = taskId;
        break;
    case LISTMENU_SILPHCO_FLOORS:
        task->data[0] = 7;
        task->data[1] = 12;
        task->data[2] = 1;
        task->data[3] = 1;
        task->data[4] = 8;
        task->data[5] = 12;
        task->data[6] = 0;
        task->data[15] = taskId;
        task->data[7] = sElevatorScroll;
        task->data[8] = sElevatorCursorPos;
        break;
    case LISTMENU_DEPT_STORE_FLOORS: // Multichoice used instead
        task->data[0] = 4;
        task->data[1] = 6;
        task->data[2] = 1;
        task->data[3] = 1;
        task->data[4] = 8;
        task->data[5] = 8;
        task->data[6] = 0;
        task->data[15] = taskId;
        break;
    case LISTMENU_WIRELESS_LECTURE_HEADERS: // Multichoice used instead
        task->data[0] = 4;
        task->data[1] = 4;
        task->data[2] = 1;
        task->data[3] = 1;
        task->data[4] = 17;
        task->data[5] = 8;
        task->data[6] = 1;
        task->data[15] = taskId;
        break;
    case 99:
        break;
    default:
        gSpecialVar_Result = 0x7F;
        DestroyTask(taskId);
        break;
    }
}

static const u8 *const sListMenuLabels[][12] = {
    [LISTMENU_BADGES] = 
    {
        gText_BoulderBadge,
        gText_CascadeBadge,
        gText_ThunderBadge,
        gText_RainbowBadge,
        gText_SoulBadge,
        gText_MarshBadge,
        gText_VolcanoBadge,
        gText_EarthBadge,
        gOtherText_Exit,
    }, 
    [LISTMENU_SILPHCO_FLOORS] = 
    {
        gText_11F,
        gText_10F,
        gText_9F,
        gText_8F,
        gText_7F,
        gText_6F,
        gText_5F,
        gText_4F,
        gText_3F,
        gText_2F,
        gText_1F,
        gOtherText_Exit,
    }, 
    [LISTMENU_DEPT_STORE_FLOORS] = // Unncessary, MULTICHOICE_DEPT_STORE_ELEVATOR is used instead
    {
        gText_5F,
        gText_4F,
        gText_3F,
        gText_2F,
        gText_1F,
        gOtherText_Exit,
    }, 
    [LISTMENU_WIRELESS_LECTURE_HEADERS] = // Unnecessary, MULTICHOICE_LINKED_DIRECT_UNION is used instead
    {
        gText_LinkedGamePlay,
        gText_DirectCorner,
        gText_UnionRoom,
        gOtherText_Quit,
    }, 
};

static void Task_CreateScriptListMenu(u8 taskId)
{
    struct WindowTemplate template;
    u8 i;
    s32 width;
    s32 mwidth;
    struct Task *task = &gTasks[taskId];
    u8 windowId;
    LockPlayerFieldControls();
    if (gSpecialVar_0x8004 == LISTMENU_SILPHCO_FLOORS)
        sListMenuLastScrollPosition = sElevatorScroll;
    else
        sListMenuLastScrollPosition = 0;
    sListMenuItems = AllocZeroed(task->data[1] * sizeof(struct ListMenuItem));
    CreateScriptListMenu();
    mwidth = 0;
    for (i = 0; i < task->data[1]; i++)
    {
        sListMenuItems[i].label = sListMenuLabels[gSpecialVar_0x8004][i];
        sListMenuItems[i].index = i;
        width = GetStringWidth(FONT_NORMAL, sListMenuItems[i].label, 0);
        if (width > mwidth)
            mwidth = width;
    }
    task->data[4] = (mwidth + 9) / 8 + 1;
    if (task->data[2] + task->data[4] > 29)
        task->data[2] = 29 - task->data[4];
    template = SetWindowTemplateFields(0, task->data[2], task->data[3], task->data[4], task->data[5], 15, 0x038);
    task->data[13] = windowId = AddWindow(&template);
    SetStdWindowBorderStyle(task->data[13], 0);
    sFieldSpecialsListMenuTemplate.totalItems = task->data[1];
    sFieldSpecialsListMenuTemplate.maxShowed = task->data[0];
    sFieldSpecialsListMenuTemplate.windowId = task->data[13];
    Task_CreateMenuRemoveScrollIndicatorArrowPair(taskId);
    task->data[14] = ListMenuInit(&sFieldSpecialsListMenuTemplate, task->data[7], task->data[8]);
    PutWindowTilemap(task->data[13]);
    CopyWindowToVram(task->data[13], COPYWIN_FULL);
    gTasks[taskId].func = Task_ListMenuHandleInput;
}

static void CreateScriptListMenu(void)
{
    sFieldSpecialsListMenuTemplate.items = sListMenuItems;
    sFieldSpecialsListMenuTemplate.moveCursorFunc = ScriptListMenuMoveCursorFunction;
    sFieldSpecialsListMenuTemplate.itemPrintFunc = NULL;
    sFieldSpecialsListMenuTemplate.totalItems = 1;
    sFieldSpecialsListMenuTemplate.maxShowed = 1;
    sFieldSpecialsListMenuTemplate.windowId = 0;
    sFieldSpecialsListMenuTemplate.header_X = 0;
    sFieldSpecialsListMenuTemplate.item_X = 8;
    sFieldSpecialsListMenuTemplate.cursor_X = 0;
    sFieldSpecialsListMenuTemplate.upText_Y = 0;
    sFieldSpecialsListMenuTemplate.cursorPal = 2;
    sFieldSpecialsListMenuTemplate.fillValue = 1;
    sFieldSpecialsListMenuTemplate.cursorShadowPal = 3;
    sFieldSpecialsListMenuTemplate.lettersSpacing = 1;
    sFieldSpecialsListMenuTemplate.itemVerticalPadding = 0;
    sFieldSpecialsListMenuTemplate.scrollMultiple = 0;
    sFieldSpecialsListMenuTemplate.fontId = FONT_NORMAL;
    sFieldSpecialsListMenuTemplate.cursorKind = 0;
}

static void ScriptListMenuMoveCursorFunction(s32 nothing, bool8 is, struct ListMenu * used)
{
    u8 taskId;
    struct Task *task;
    PlaySE(SE_SELECT);
    taskId = FindTaskIdByFunc(Task_ListMenuHandleInput);
    if (taskId != 0xFF)
    {
        task = &gTasks[taskId];
        ListMenuGetScrollAndRow(task->data[14], &sFieldSpecialsListMenuScrollBuffer, NULL);
        sListMenuLastScrollPosition = sFieldSpecialsListMenuScrollBuffer;
    }
}

static void Task_ListMenuHandleInput(u8 taskId)
{
    s32 input;
    struct Task *task;

    task = &gTasks[taskId];
    task++;task--;
    input = ListMenu_ProcessInput(task->data[14]);
    switch (input)
    {
    case -1:
        break;
    case -2:
        gSpecialVar_Result = 0x7F;
        PlaySE(SE_SELECT);
        Task_DestroyListMenu(taskId);
        break;
    default:
        gSpecialVar_Result = input;
        PlaySE(SE_SELECT);
        if (task->data[6] == 0 || input == task->data[1] - 1)
        {
            Task_DestroyListMenu(taskId);
        }
        else
        {
            Task_ListMenuRemoveScrollIndicatorArrowPair(taskId);
            task->func = Task_SuspendListMenu;
            ScriptContext_Enable();
        }
        break;
    }
}

static void Task_DestroyListMenu(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    Task_ListMenuRemoveScrollIndicatorArrowPair(taskId);
    DestroyListMenuTask(task->data[14], NULL, NULL);
    Free(sListMenuItems);
    ClearStdWindowAndFrameToTransparent(task->data[13], TRUE);
    FillWindowPixelBuffer(task->data[13], PIXEL_FILL(0));
    ClearWindowTilemap(task->data[13]);
    CopyWindowToVram(task->data[13], COPYWIN_GFX);
    RemoveWindow(task->data[13]);
    DestroyTask(taskId);
    ScriptContext_Enable();
}

static void Task_SuspendListMenu(u8 taskId)
{
    switch (gTasks[taskId].data[6])
    {
    case 1:
        break;
    case 2:
        gTasks[taskId].data[6] = 1;
        gTasks[taskId].func = Task_RedrawScrollArrowsAndWaitInput;
        break;
    }
}

void ReturnToListMenu(void)
{
    u8 taskId = FindTaskIdByFunc(Task_SuspendListMenu);
    if (taskId == 0xFF)
        ScriptContext_Enable();
    else
        gTasks[taskId].data[6]++;
}

static void Task_RedrawScrollArrowsAndWaitInput(u8 taskId)
{
    LockPlayerFieldControls();
    Task_CreateMenuRemoveScrollIndicatorArrowPair(taskId);
    gTasks[taskId].func = Task_ListMenuHandleInput;
}

static void Task_CreateMenuRemoveScrollIndicatorArrowPair(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    struct ScrollArrowsTemplate template = {
        .firstArrowType = 2,
        .secondArrowType = 3,
        .tileTag = 2000,
        .palTag = 100
    };
    if (task->data[0] != task->data[1])
    {
        template.firstX = 4 * task->data[4] + 8 * task->data[2];
        template.firstY = 8;
        template.secondX = 4 * task->data[4] + 8 * task->data[2];
        template.secondY = 8 * task->data[5] + 10;
        template.fullyUpThreshold = 0;
        template.fullyDownThreshold = task->data[1] - task->data[0];
        task->data[12] = AddScrollIndicatorArrowPair(&template, &sListMenuLastScrollPosition);
    }
}

static void Task_ListMenuRemoveScrollIndicatorArrowPair(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    if (task->data[0] != task->data[1])
        RemoveScrollIndicatorArrowPair(task->data[12]);
}

void ForcePlayerToStartSurfing(void)
{
    SetPlayerAvatarTransitionFlags(PLAYER_AVATAR_FLAG_SURFING);
}

static const u16 sStarterSpecies[] = {
    SPECIES_BULBASAUR,
    SPECIES_SQUIRTLE,
    SPECIES_CHARMANDER
};

static u16 GetStarterSpeciesById(u16 idx)
{
    if (idx >= NELEMS(sStarterSpecies))
        idx = 0;
    return sStarterSpecies[idx];
}

u16 GetStarterSpecies(void)
{
    return GetStarterSpeciesById(VarGet(VAR_STARTER_MON));
}

void SetSeenMon(void)
{
    GetSetPokedexFlag(SpeciesToNationalPokedexNum(gSpecialVar_0x8004), 2);
}

void ResetContextNpcTextColor(void)
{
    gSelectedObjectEvent = 0;
    gSpecialVar_TextColor = NPC_TEXT_COLOR_DEFAULT;
}

u8 ContextNpcGetTextColor(void)
{
    u8 gfxId;
    if (gSpecialVar_TextColor != NPC_TEXT_COLOR_DEFAULT)
    {
        // A text color has been specified, use that
        return gSpecialVar_TextColor;
    }
    else if (gSelectedObjectEvent == 0)
    {
        // No text color specified and no object selected, use neutral
        return NPC_TEXT_COLOR_NEUTRAL;
    }
    else
    {
        // An object is selected and no color has been specified.
        // Use the text color normally associated with this object's sprite.
        gfxId = gObjectEvents[gSelectedObjectEvent].graphicsId;
        if (gfxId >= OBJ_EVENT_GFX_VAR_0)
            gfxId = VarGetObjectEventGraphicsId(gfxId - OBJ_EVENT_GFX_VAR_0);
        return GetColorFromTextColorTable(gfxId);
    }
}

static bool8 HasMonBeenRenamed(u8 idx)
{
    struct Pokemon * pokemon = &gPlayerParty[idx];
    u8 language;
    GetMonData(pokemon, MON_DATA_NICKNAME, gStringVar1);
    language = GetMonData(pokemon, MON_DATA_LANGUAGE, &language);
    if (language != LANGUAGE_ENGLISH)
        return TRUE;
    else if (StringCompare(gSpeciesNames[GetMonData(pokemon, MON_DATA_SPECIES, NULL)], gStringVar1) != 0)
        return TRUE;
    else
        return FALSE;
}

bool8 HasLeadMonBeenRenamed(void)
{
    return HasMonBeenRenamed(GetLeadMonIndex());
}

void TV_PrintIntToStringVar(u8 varidx, s32 number)
{
    s32 n = CountDigits(number);
    ConvertIntToDecimalStringN(sStringVarPtrs[varidx], number, STR_CONV_MODE_LEFT_ALIGN, n);
}

s32 CountDigits(s32 number)
{
    if (number / 10 == 0)
        return 1;
    else if (number / 100 == 0)
        return 2;
    else if (number / 1000 == 0)
        return 3;
    else if (number / 10000 == 0)
        return 4;
    else if (number / 100000 == 0)
        return 5;
    else if (number / 1000000 == 0)
        return 6;
    else if (number / 10000000 == 0)
        return 7;
    else if (number / 100000000 == 0)
        return 8;
    else
        return 1;
}

bool8 NameRaterWasNicknameChanged(void)
{
    struct Pokemon * pokemon = &gPlayerParty[gSpecialVar_0x8004];
    GetMonData(pokemon, MON_DATA_NICKNAME, gStringVar1);
    if (StringCompare(gStringVar3, gStringVar1) == 0)
        return FALSE;
    else
        return TRUE;
}

void ChangeBoxPokemonNickname(void)
{
    struct BoxPokemon * pokemon = GetBoxedMonPtr(gSpecialVar_MonBoxId, gSpecialVar_MonBoxPos);
    u16 species;
    u8 gender;
    u32 personality;


    GetBoxMonData(pokemon, MON_DATA_NICKNAME, gStringVar3);
    GetBoxMonData(pokemon, MON_DATA_NICKNAME, gStringVar2);
    species = GetBoxMonData(pokemon, MON_DATA_SPECIES, NULL);
    gender = GetBoxMonGender(pokemon);
    personality = GetBoxMonData(pokemon, MON_DATA_PERSONALITY, NULL);
    DoNamingScreen(NAMING_SCREEN_NICKNAME, gStringVar2, species, gender, personality, ChangeBoxPokemonNickname_CB);
}

static void ChangeBoxPokemonNickname_CB(void)
{
    SetBoxMonNickAt(gSpecialVar_MonBoxId, gSpecialVar_MonBoxPos, gStringVar2);
    CB2_ReturnToFieldContinueScriptPlayMapMusic();
}

void ChangePokemonNickname(void)
{
    u16 species;
    u8 gender;
    u32 personality;

    GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_NICKNAME, gStringVar3);
    GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_NICKNAME, gStringVar2);
    species = GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_SPECIES, NULL);
    gender = GetMonGender(&gPlayerParty[gSpecialVar_0x8004]);
    personality = GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_PERSONALITY, NULL);
    DoNamingScreen(NAMING_SCREEN_NICKNAME, gStringVar2, species, gender, personality, ChangePokemonNickname_CB);
}

void SetPartyMonHighFriendship(void)
{
    u8 friendship = 200;

    if (gSpecialVar_0x8004 < PARTY_SIZE)
        SetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_FRIENDSHIP, &friendship);
}

void SetBoxMonHighFriendship(void)
{
    u8 friendship = 200;

    SetBoxMonData(GetBoxedMonPtr(gSpecialVar_MonBoxId, gSpecialVar_MonBoxPos), MON_DATA_FRIENDSHIP, &friendship);
}

void Script_IsSpeciesCaught(void)
{
    gSpecialVar_Result = GetSetPokedexFlag(SpeciesToNationalPokedexNum(gSpecialVar_0x8004), FLAG_GET_CAUGHT);
}

static void ChangePokemonNickname_CB(void)
{
    SetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_NICKNAME, gStringVar2);
    CB2_ReturnToFieldContinueScriptPlayMapMusic();
}

void BufferMonNickname(void)
{
    GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_NICKNAME, gStringVar1);
    StringGet_Nickname(gStringVar1);
}

void IsMonOTIDNotPlayers(void)
{
    if (GetPlayerTrainerId() == GetMonData(&gPlayerParty[gSpecialVar_0x8004], MON_DATA_OT_ID, NULL))
        gSpecialVar_Result = FALSE;
    else
        gSpecialVar_Result = TRUE;
}

u32 GetPlayerTrainerId(void)
{
    return (gSaveBlock2Ptr->playerTrainerId[3] << 24) | (gSaveBlock2Ptr->playerTrainerId[2] << 16) | (gSaveBlock2Ptr->playerTrainerId[1] << 8) | gSaveBlock2Ptr->playerTrainerId[0];
}

void UpdateTrainerCardPhotoIcons(void)
{
    u16 species[PARTY_SIZE];
    u32 personality[PARTY_SIZE];
    u8 i;
    u8 partyCount;
    for (i = 0; i < PARTY_SIZE; i++)
        species[i] = SPECIES_NONE;
    partyCount = CalculatePlayerPartyCount();
    for (i = 0; i < partyCount; i++)
    {
        species[i] = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES_OR_EGG, NULL);
        personality[i] = GetMonData(&gPlayerParty[i], MON_DATA_PERSONALITY, NULL);
    }
    VarSet(VAR_TRAINER_CARD_MON_ICON_1, SpeciesToMailSpecies(species[0], personality[0]));
    VarSet(VAR_TRAINER_CARD_MON_ICON_2, SpeciesToMailSpecies(species[1], personality[1]));
    VarSet(VAR_TRAINER_CARD_MON_ICON_3, SpeciesToMailSpecies(species[2], personality[2]));
    VarSet(VAR_TRAINER_CARD_MON_ICON_4, SpeciesToMailSpecies(species[3], personality[3]));
    VarSet(VAR_TRAINER_CARD_MON_ICON_5, SpeciesToMailSpecies(species[4], personality[4]));
    VarSet(VAR_TRAINER_CARD_MON_ICON_6, SpeciesToMailSpecies(species[5], personality[5]));
    VarSet(VAR_TRAINER_CARD_MON_ICON_TINT_IDX, gSpecialVar_0x8004);
}

u16 StickerManGetBragFlags(void)
{
    u16 result = 0;
    u32 numEggs;
    gSpecialVar_0x8004 = GetGameStat(GAME_STAT_ENTERED_HOF);
    numEggs = GetGameStat(GAME_STAT_HATCHED_EGGS);
    gSpecialVar_0x8006 = GetGameStat(GAME_STAT_LINK_BATTLE_WINS);
    if (numEggs > 0xFFFF)
        gSpecialVar_0x8005 = 0xFFFF;
    else
        gSpecialVar_0x8005 = numEggs;
    if (gSpecialVar_0x8004 != 0)
        result |= 1 << 0;
    if (gSpecialVar_0x8005 != 0)
        result |= 1 << 1;
    if (gSpecialVar_0x8006 != 0)
        result |= 1 << 2;
    return result;
}

u16 GetHiddenItemAttr(u32 hiddenItem, u8 attr)
{
    if (attr == HIDDEN_ITEM_ITEM)
        return GET_HIDDEN_ITEM_ITEM(hiddenItem);
    else if (attr == HIDDEN_ITEM_FLAG)
        return GET_HIDDEN_ITEM_FLAG(hiddenItem) + FLAG_HIDDEN_ITEMS_START;
    else if (attr == HIDDEN_ITEM_QUANTITY)
        return GET_HIDDEN_ITEM_QUANTITY(hiddenItem);
    else if (attr == HIDDEN_ITEM_UNDERFOOT)
        return GET_HIDDEN_ITEM_UNDERFOOT(hiddenItem);
    else // Invalid
        return 1;
}

bool8 DoesPlayerPartyContainSpecies(void)
{
    u8 partyCount = CalculatePlayerPartyCount();
    u8 i;
    for (i = 0; i < partyCount; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES_OR_EGG, NULL) == gSpecialVar_0x8004)
            return TRUE;
    }
    return FALSE;
}

static const u8 sMartMaps[][3] = {
    {MAP(MAP_VIRIDIAN_CITY_MART),   1},
    {MAP(MAP_PEWTER_CITY_MART),     3},
    {MAP(MAP_CERULEAN_CITY_MART),   1},
    {MAP(MAP_VERMILION_CITY_MART),  1},
    {MAP(MAP_FUCHSIA_CITY_MART),    1},
    {MAP(MAP_CINNABAR_ISLAND_MART), 1},
    {MAP(MAP_SAFFRON_CITY_MART),    1}
};

u8 GetMartClerkObjectId(void)
{
    u8 i;
    for (i = 0; i < NELEMS(sMartMaps); i++)
    {
        if (gSaveBlock1Ptr->location.mapGroup == sMartMaps[i][0] && gSaveBlock1Ptr->location.mapNum == sMartMaps[i][1])
            return sMartMaps[i][2];
    }
    return 1;
}

static const struct {
    u16 inside_grp;
    u16 inside_num;
    u16 outside_grp;
    u16 outside_num;
} sInsideOutsidePairs[] = {
    [QL_LOCATION_HOME]               = {MAP(MAP_PALLET_TOWN_PLAYERS_HOUSE_1F),          MAP(MAP_PALLET_TOWN)},
    [QL_LOCATION_OAKS_LAB]           = {MAP(MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB),        MAP(MAP_PALLET_TOWN)},
    [QL_LOCATION_VIRIDIAN_GYM]       = {MAP(MAP_VIRIDIAN_CITY_GYM),                     MAP(MAP_VIRIDIAN_CITY)},
    [QL_LOCATION_LEAGUE_GATE_1]      = {MAP(MAP_ROUTE22_NORTH_ENTRANCE),                MAP(MAP_ROUTE22)},
    [QL_LOCATION_LEAGUE_GATE_2]      = {MAP(MAP_ROUTE22_NORTH_ENTRANCE),                MAP(MAP_ROUTE23)},
    [QL_LOCATION_VIRIDIAN_FOREST_1]  = {MAP(MAP_VIRIDIAN_FOREST),                       MAP(MAP_ROUTE2_VIRIDIAN_FOREST_SOUTH_ENTRANCE)},
    [QL_LOCATION_VIRIDIAN_FOREST_2]  = {MAP(MAP_VIRIDIAN_FOREST),                       MAP(MAP_ROUTE2_VIRIDIAN_FOREST_NORTH_ENTRANCE)},
    [QL_LOCATION_PEWTER_MUSEUM]      = {MAP(MAP_PEWTER_CITY_MUSEUM_1F),                 MAP(MAP_PEWTER_CITY)},
    [QL_LOCATION_PEWTER_GYM]         = {MAP(MAP_PEWTER_CITY_GYM),                       MAP(MAP_PEWTER_CITY)},
    [QL_LOCATION_MT_MOON_1]          = {MAP(MAP_MT_MOON_1F),                            MAP(MAP_ROUTE4)},
    [QL_LOCATION_MT_MOON_2]          = {MAP(MAP_MT_MOON_B1F),                           MAP(MAP_ROUTE4)},
    [QL_LOCATION_CERULEAN_GYM]       = {MAP(MAP_CERULEAN_CITY_GYM),                     MAP(MAP_CERULEAN_CITY)},
    [QL_LOCATION_BIKE_SHOP]          = {MAP(MAP_CERULEAN_CITY_BIKE_SHOP),               MAP(MAP_CERULEAN_CITY)},
    [QL_LOCATION_BILLS_HOUSE]        = {MAP(MAP_ROUTE25_SEA_COTTAGE),                   MAP(MAP_ROUTE25)},
    [QL_LOCATION_DAY_CARE]           = {MAP(MAP_ROUTE5_POKEMON_DAY_CARE),               MAP(MAP_ROUTE5)},
    [QL_LOCATION_UNDERGROUND_PATH_1] = {MAP(MAP_UNDERGROUND_PATH_NORTH_ENTRANCE),       MAP(MAP_ROUTE5)},
    [QL_LOCATION_UNDERGROUND_PATH_2] = {MAP(MAP_UNDERGROUND_PATH_SOUTH_ENTRANCE),       MAP(MAP_ROUTE6)},
    [QL_LOCATION_PKMN_FAN_CLUB]      = {MAP(MAP_VERMILION_CITY_POKEMON_FAN_CLUB),       MAP(MAP_VERMILION_CITY)},
    [QL_LOCATION_VERMILION_GYM]      = {MAP(MAP_VERMILION_CITY_GYM),                    MAP(MAP_VERMILION_CITY)},
    [QL_LOCATION_SS_ANNE]            = {MAP(MAP_SSANNE_1F_CORRIDOR),                    MAP(MAP_VERMILION_CITY)},
    [QL_LOCATION_DIGLETTS_CAVE_1]    = {MAP(MAP_DIGLETTS_CAVE_NORTH_ENTRANCE),          MAP(MAP_ROUTE2)},
    [QL_LOCATION_DIGLETTS_CAVE_2]    = {MAP(MAP_DIGLETTS_CAVE_SOUTH_ENTRANCE),          MAP(MAP_ROUTE11)},
    [QL_LOCATION_ROCK_TUNNEL_1]      = {MAP(MAP_ROCK_TUNNEL_1F),                        MAP(MAP_ROUTE10)},
    [QL_LOCATION_ROCK_TUNNEL_2]      = {MAP(MAP_ROCK_TUNNEL_1F),                        MAP(MAP_ROUTE10)},
    [QL_LOCATION_POWER_PLANT]        = {MAP(MAP_POWER_PLANT),                           MAP(MAP_ROUTE10)},
    [QL_LOCATION_PKMN_TOWER]         = {MAP(MAP_POKEMON_TOWER_1F),                      MAP(MAP_LAVENDER_TOWN)},
    [QL_LOCATION_VOLUNTEER_HOUSE]    = {MAP(MAP_LAVENDER_TOWN_VOLUNTEER_POKEMON_HOUSE), MAP(MAP_LAVENDER_TOWN)},
    [QL_LOCATION_NAME_RATERS_HOUSE]  = {MAP(MAP_LAVENDER_TOWN_HOUSE2),                  MAP(MAP_LAVENDER_TOWN)},
    [QL_LOCATION_UNDERGROUND_PATH_3] = {MAP(MAP_UNDERGROUND_PATH_EAST_ENTRANCE),        MAP(MAP_ROUTE8)},
    [QL_LOCATION_UNDERGROUND_PATH_4] = {MAP(MAP_UNDERGROUND_PATH_WEST_ENTRANCE),        MAP(MAP_ROUTE7)},
    [QL_LOCATION_CELADON_DEPT_STORE] = {MAP(MAP_CELADON_CITY_DEPARTMENT_STORE_1F),      MAP(MAP_CELADON_CITY)},
    [QL_LOCATION_CELADON_MANSION]    = {MAP(MAP_CELADON_CITY_CONDOMINIUMS_1F),          MAP(MAP_CELADON_CITY)},
    [QL_LOCATION_GAME_CORNER]        = {MAP(MAP_CELADON_CITY_GAME_CORNER),              MAP(MAP_CELADON_CITY)},
    [QL_LOCATION_CELADON_GYM]        = {MAP(MAP_CELADON_CITY_GYM),                      MAP(MAP_CELADON_CITY)},
    [QL_LOCATION_CELADON_RESTAURANT] = {MAP(MAP_CELADON_CITY_RESTAURANT),               MAP(MAP_CELADON_CITY)},
    [QL_LOCATION_SAFARI_ZONE]        = {MAP(MAP_SAFARI_ZONE_CENTER),                    MAP(MAP_FUCHSIA_CITY_SAFARI_ZONE_ENTRANCE)},
    [QL_LOCATION_FUCHSIA_GYM]        = {MAP(MAP_FUCHSIA_CITY_GYM),                      MAP(MAP_FUCHSIA_CITY)},
    [QL_LOCATION_WARDENS_HOME]       = {MAP(MAP_FUCHSIA_CITY_WARDENS_HOUSE),            MAP(MAP_FUCHSIA_CITY)},
    [QL_LOCATION_FIGHTING_DOJO]      = {MAP(MAP_SAFFRON_CITY_DOJO),                     MAP(MAP_SAFFRON_CITY)},
    [QL_LOCATION_SAFFRON_GYM]        = {MAP(MAP_SAFFRON_CITY_GYM),                      MAP(MAP_SAFFRON_CITY)},
    [QL_LOCATION_SILPH_CO]           = {MAP(MAP_SILPH_CO_1F),                           MAP(MAP_SAFFRON_CITY)},
    [QL_LOCATION_SEAFOAM_ISLANDS_1]  = {MAP(MAP_SEAFOAM_ISLANDS_1F),                    MAP(MAP_ROUTE20)},
    [QL_LOCATION_SEAFOAM_ISLANDS_2]  = {MAP(MAP_SEAFOAM_ISLANDS_1F),                    MAP(MAP_ROUTE20)},
    [QL_LOCATION_PKMN_MANSION]       = {MAP(MAP_POKEMON_MANSION_1F),                    MAP(MAP_CINNABAR_ISLAND)},
    [QL_LOCATION_CINNABAR_GYM]       = {MAP(MAP_CINNABAR_ISLAND_GYM),                   MAP(MAP_CINNABAR_ISLAND)},
    [QL_LOCATION_CINNABAR_LAB]       = {MAP(MAP_CINNABAR_ISLAND_POKEMON_LAB_ENTRANCE),  MAP(MAP_CINNABAR_ISLAND)},
    [QL_LOCATION_VICTORY_ROAD_1]     = {MAP(MAP_VICTORY_ROAD_1F),                       MAP(MAP_ROUTE23)},
    [QL_LOCATION_VICTORY_ROAD_2]     = {MAP(MAP_VICTORY_ROAD_2F),                       MAP(MAP_ROUTE23)},
    [QL_LOCATION_PKMN_LEAGUE]        = {MAP(MAP_INDIGO_PLATEAU_POKEMON_CENTER_1F),      MAP(MAP_INDIGO_PLATEAU_EXTERIOR)},
    [QL_LOCATION_CERULEAN_CAVE]      = {MAP(MAP_CERULEAN_CAVE_1F),                      MAP(MAP_CERULEAN_CITY)}
};

void QuestLog_CheckDepartingIndoorsMap(void)
{
}

void QuestLog_TryRecordDepartedLocation(void)
{
}

u16 GetMysteryGiftCardStat(void)
{
    switch (gSpecialVar_Result)
    {
    case GET_NUM_STAMPS:
        return MysteryGift_GetCardStat(CARD_STAT_NUM_STAMPS);
    case GET_MAX_STAMPS:
        return MysteryGift_GetCardStat(CARD_STAT_MAX_STAMPS);
    case GET_CARD_BATTLES_WON:
        return MysteryGift_GetCardStat(CARD_STAT_BATTLES_WON);
    case GET_CARD_BATTLES_LOST:
        return MysteryGift_GetCardStat(CARD_STAT_BATTLES_LOST);
    case GET_CARD_NUM_TRADES:
        return MysteryGift_GetCardStat(CARD_STAT_NUM_TRADES);
    default:
        AGB_ASSERT_EX(0, ABSPATH("scr_tool.c"), 3873);
        return 0;
    }
}

void SetPCBoxToSendMon(u8 boxId)
{
    sPCBoxToSendMon = boxId;
}

u16 GetPCBoxToSendMon(void)
{
    return sPCBoxToSendMon;
}

bool8 ShouldShowBoxWasFullMessage(void)
{
    if (FlagGet(FLAG_SHOWN_BOX_WAS_FULL_MESSAGE))
        return FALSE;
    if (StorageGetCurrentBox() == VarGet(VAR_PC_BOX_TO_SEND_MON))
        return FALSE;
    FlagSet(FLAG_SHOWN_BOX_WAS_FULL_MESSAGE);
    return TRUE;
}

bool8 IsDestinationBoxFull(void)
{
    s32 i;
    s32 j;
    SetPCBoxToSendMon(VarGet(VAR_PC_BOX_TO_SEND_MON));
    i = StorageGetCurrentBox();
    do
    {
        for (j = 0; j < IN_BOX_COUNT; j++)
        {
            if (GetBoxMonData(GetBoxedMonPtr(i, j), MON_DATA_SPECIES, NULL) == SPECIES_NONE)
            {
                if (GetPCBoxToSendMon() != i)
                    FlagClear(FLAG_SHOWN_BOX_WAS_FULL_MESSAGE);
                VarSet(VAR_PC_BOX_TO_SEND_MON, i);
                return ShouldShowBoxWasFullMessage();
            }
        }
        i++;
        if (i == TOTAL_BOXES_COUNT)
            i = 0;
    } while (i != StorageGetCurrentBox());
    return FALSE;
}

const u16 sPokeCenter1FMaps[] = {
    MAP_VIRIDIAN_CITY_POKEMON_CENTER_1F,
    MAP_PEWTER_CITY_POKEMON_CENTER_1F,
    MAP_CERULEAN_CITY_POKEMON_CENTER_1F,
    MAP_VERMILION_CITY_POKEMON_CENTER_1F,
    MAP_CELADON_CITY_POKEMON_CENTER_1F,
    MAP_FUCHSIA_CITY_POKEMON_CENTER_1F,
    MAP_CINNABAR_ISLAND_POKEMON_CENTER_1F,
    MAP_INDIGO_PLATEAU_POKEMON_CENTER_1F,
    MAP_SAFFRON_CITY_POKEMON_CENTER_1F,
    MAP_UNION_ROOM,
    MAP_UNDEFINED
};

bool8 UsedPokemonCenterWarp(void)
{
    s32 i;
    u16 mapno = (gLastUsedWarp.mapGroup << 8) + gLastUsedWarp.mapNum;
    for (i = 0; sPokeCenter1FMaps[i] != MAP_UNDEFINED; i++)
    {
        if (sPokeCenter1FMaps[i] == mapno)
            return TRUE;
    }
    return FALSE;
}

bool8 BufferTMHMMoveName(void)
{
    // 8004 = item ID
    if (gSpecialVar_0x8004 >= ITEM_TM01 && gSpecialVar_0x8004 <= ITEM_TM59)
    {
        StringCopy(gStringVar1, gMoveNames[ItemIdToBattleMoveId(gSpecialVar_0x8004)]);
        return TRUE;
    }
    else
        return FALSE;
}

void RunMassageCooldownStepCounter(void)
{
    u16 count = VarGet(VAR_MASSAGE_COOLDOWN_STEP_COUNTER);
    if (count < 500)
        VarSet(VAR_MASSAGE_COOLDOWN_STEP_COUNTER, count + 1);
}

void DaisyMassageServices(void)
{
    AdjustFriendship(&gPlayerParty[gSpecialVar_0x8004], FRIENDSHIP_EVENT_MASSAGE);
    VarSet(VAR_MASSAGE_COOLDOWN_STEP_COUNTER, 0);
}

static const u16 sEliteFourLightingPalettes[][16] = {
    INCBIN_U16("graphics/field_specials/elite_four_lighting_0.gbapal"),
    INCBIN_U16("graphics/field_specials/elite_four_lighting_1.gbapal"),
    INCBIN_U16("graphics/field_specials/elite_four_lighting_2.gbapal"),
    INCBIN_U16("graphics/field_specials/elite_four_lighting_3.gbapal"),
    INCBIN_U16("graphics/field_specials/elite_four_lighting_4.gbapal"),
    INCBIN_U16("graphics/field_specials/elite_four_lighting_5.gbapal"),
    INCBIN_U16("graphics/field_specials/elite_four_lighting_6.gbapal"),
    INCBIN_U16("graphics/field_specials/elite_four_lighting_7.gbapal"),
    INCBIN_U16("graphics/field_specials/elite_four_lighting_8.gbapal"),
    INCBIN_U16("graphics/field_specials/elite_four_lighting_9.gbapal"),
    INCBIN_U16("graphics/field_specials/elite_four_lighting_10.gbapal"),
    INCBIN_U16("graphics/field_specials/elite_four_lighting_11.gbapal")
};

static const u16 sChampionRoomLightingPalettes[][16] = {
    INCBIN_U16("graphics/field_specials/champion_room_lighting_0.gbapal"),
    INCBIN_U16("graphics/field_specials/champion_room_lighting_1.gbapal"),
    INCBIN_U16("graphics/field_specials/champion_room_lighting_2.gbapal"),
    INCBIN_U16("graphics/field_specials/champion_room_lighting_3.gbapal"),
    INCBIN_U16("graphics/field_specials/champion_room_lighting_4.gbapal"),
    INCBIN_U16("graphics/field_specials/champion_room_lighting_5.gbapal"),
    INCBIN_U16("graphics/field_specials/champion_room_lighting_6.gbapal"),
    INCBIN_U16("graphics/field_specials/champion_room_lighting_7.gbapal"),
    INCBIN_U16("graphics/field_specials/champion_room_lighting_8.gbapal")
};

static const u8 sEliteFourLightingTimers[] = {
    40,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12,
    12
};

static const u8 sChampionRoomLightingTimers[] = {
    20,
     8,
     8,
     8,
     8,
     8,
     8,
     8
};

void DoPokemonLeagueLightingEffect(void)
{
    u8 taskId = CreateTask(Task_RunPokemonLeagueLightingEffect, 8);
    s16 *data = gTasks[taskId].data;
    if (FlagGet(FLAG_TEMP_3) == TRUE)
    {
        gTasks[taskId].func = Task_CancelPokemonLeagueLightingEffect;
    }
    else
    {
        if (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_POKEMON_LEAGUE_CHAMPIONS_ROOM) && gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_POKEMON_LEAGUE_CHAMPIONS_ROOM))
        {
            data[0] = sChampionRoomLightingTimers[0];
            data[2] = 8;
            LoadPalette(sChampionRoomLightingPalettes[0], BG_PLTT_ID(7), PLTT_SIZE_4BPP);
        }
        else
        {
            data[0] = sEliteFourLightingTimers[0];
            data[2] = 11;
            LoadPalette(sEliteFourLightingPalettes[0], BG_PLTT_ID(7), PLTT_SIZE_4BPP);
        }
        data[1] = 0;
        ApplyGlobalTintToPaletteSlot(7, 1);
    }
}

static void Task_RunPokemonLeagueLightingEffect(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (!gPaletteFade.active
     && FlagGet(FLAG_TEMP_2) != FALSE
     && FlagGet(FLAG_TEMP_5) != TRUE
     && gGlobalFieldTintMode != QL_TINT_BACKUP_GRAYSCALE
     && --data[0] == 0
    )
    {
        if (++data[1] == data[2])
            data[1] = 0;

        if (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_POKEMON_LEAGUE_CHAMPIONS_ROOM) && gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_POKEMON_LEAGUE_CHAMPIONS_ROOM))
        {
            data[0] = sChampionRoomLightingTimers[data[1]];
            LoadPalette(sChampionRoomLightingPalettes[data[1]], BG_PLTT_ID(7), PLTT_SIZE_4BPP);
        }
        else
        {
            data[0] = sEliteFourLightingTimers[data[1]];
            LoadPalette(sEliteFourLightingPalettes[data[1]], BG_PLTT_ID(7), PLTT_SIZE_4BPP);
        }
        ApplyGlobalTintToPaletteSlot(7, 1);
    }
}

static void Task_CancelPokemonLeagueLightingEffect(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (FlagGet(FLAG_TEMP_4) != FALSE)
    {
        if (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_POKEMON_LEAGUE_CHAMPIONS_ROOM) && gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_POKEMON_LEAGUE_CHAMPIONS_ROOM))
            LoadPalette(sChampionRoomLightingPalettes[8], BG_PLTT_ID(7), PLTT_SIZE_4BPP);
        else
            LoadPalette(sEliteFourLightingPalettes[11], BG_PLTT_ID(7), PLTT_SIZE_4BPP);
        ApplyGlobalTintToPaletteSlot(7, 1);
        if (gPaletteFade.active)
        {
            BlendPalettes(0x00000080, 16, RGB_BLACK);
        }
        DestroyTask(taskId);
    }
}

void StopPokemonLeagueLightingEffectTask(void)
{
    if (FuncIsActiveTask(Task_RunPokemonLeagueLightingEffect) == TRUE)
    {
        DestroyTask(FindTaskIdByFunc(Task_RunPokemonLeagueLightingEffect));
    }
}

static const u8 sCapeBrinkCompatibleSpecies[] = {
    SPECIES_VENUSAUR,
    SPECIES_CHARIZARD,
    SPECIES_BLASTOISE
};

bool8 CapeBrinkGetMoveToTeachLeadPokemon(void)
{
    // Returns:
    //   8005 = Move tutor index
    //   8006 = Num moves known by lead mon
    //   8007 = Index of lead mon
    //   to specialvar = whether a move can be taught in the first place
    u8 tutorMonId = 0;
    u8 numMovesKnown = 0;
    u8 leadMonSlot = GetLeadMonIndex();
    u8 i;
    gSpecialVar_0x8007 = leadMonSlot;
    for (i = 0; i < NELEMS(sCapeBrinkCompatibleSpecies); i++)
    {
        if (GetMonData(&gPlayerParty[leadMonSlot], MON_DATA_SPECIES_OR_EGG, NULL) == sCapeBrinkCompatibleSpecies[i])
        {
            tutorMonId = i;
            break;
        }
    }
    if (i == NELEMS(sCapeBrinkCompatibleSpecies) || GetMonData(&gPlayerParty[leadMonSlot], MON_DATA_FRIENDSHIP) != 255)
        return FALSE;
    if (tutorMonId == 0)
    {
        StringCopy(gStringVar2, gMoveNames[MOVE_FRENZY_PLANT]);
        gSpecialVar_0x8005 = MOVETUTOR_FRENZY_PLANT;
        if (FlagGet(FLAG_TUTOR_FRENZY_PLANT) == TRUE)
            return FALSE;
    }
    else if (tutorMonId == 1)
    {
        StringCopy(gStringVar2, gMoveNames[MOVE_BLAST_BURN]);
        gSpecialVar_0x8005 = MOVETUTOR_BLAST_BURN;
        if (FlagGet(FLAG_TUTOR_BLAST_BURN) == TRUE)
            return FALSE;
    }
    else
    {
        StringCopy(gStringVar2, gMoveNames[MOVE_HYDRO_CANNON]);
        gSpecialVar_0x8005 = MOVETUTOR_HYDRO_CANNON;
        if (FlagGet(FLAG_TUTOR_HYDRO_CANNON) == TRUE)
            return FALSE;
    }
    if (GetMonData(&gPlayerParty[leadMonSlot], MON_DATA_MOVE1) != MOVE_NONE)
        numMovesKnown++;
    if (GetMonData(&gPlayerParty[leadMonSlot], MON_DATA_MOVE2) != MOVE_NONE)
        numMovesKnown++;
    if (GetMonData(&gPlayerParty[leadMonSlot], MON_DATA_MOVE3) != MOVE_NONE)
        numMovesKnown++;
    if (GetMonData(&gPlayerParty[leadMonSlot], MON_DATA_MOVE4) != MOVE_NONE)
        numMovesKnown++;
    gSpecialVar_0x8006 = numMovesKnown;
    return TRUE;
}

bool8 HasLearnedAllMovesFromCapeBrinkTutor(void)
{
    // 8005 is set by CapeBrinkGetMoveToTeachLeadPokemon
    u8 r4 = 0;
    if (gSpecialVar_0x8005 == MOVETUTOR_FRENZY_PLANT)
        FlagSet(FLAG_TUTOR_FRENZY_PLANT);
    else if (gSpecialVar_0x8005 == MOVETUTOR_BLAST_BURN)
        FlagSet(FLAG_TUTOR_BLAST_BURN);
    else
        FlagSet(FLAG_TUTOR_HYDRO_CANNON);
    if (FlagGet(FLAG_TUTOR_FRENZY_PLANT) == TRUE)
        r4++;
    if (FlagGet(FLAG_TUTOR_BLAST_BURN) == TRUE)
        r4++;
    if (FlagGet(FLAG_TUTOR_HYDRO_CANNON) == TRUE)
        r4++;
    if (r4 == 3)
        return TRUE;
    else
        return FALSE;
}

bool8 IsBadEggInParty(void)
{
    u8 partyCount = CalculatePlayerPartyCount();
    u8 i;
    for (i = 0; i < partyCount; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_SANITY_IS_BAD_EGG) == TRUE)
            return TRUE;
    }
    return FALSE;
}

bool8 IsPlayerNotInRocketLeagueLobby(void)
{
    if (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_ROCKET_LEAGUE_LOBBY) && gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_ROCKET_LEAGUE_LOBBY))
        return FALSE;
    else
        return TRUE;
}

void BrailleCursorToggle(void)
{
    // 8004 = x - 27
    // 8005 = y
    // 8006 = action (0 = create, 1 = delete)
    u16 x;
    if (gQuestLogState != QL_STATE_PLAYBACK)
    {
        x = gSpecialVar_0x8004 + 27;
        if (gSpecialVar_0x8006 == 0)
            sBrailleTextCursorSpriteID = CreateTextCursorSprite(0, x, gSpecialVar_0x8005, 0, 0);
        else
            DestroyTextCursorSprite(sBrailleTextCursorSpriteID);
    }
}

bool8 PlayerPartyContainsSpeciesWithPlayerID(void)
{
    // 8004 = species
    u8 playerCount = CalculatePlayerPartyCount();
    u8 i;
    for (i = 0; i < playerCount; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES_OR_EGG, NULL) == gSpecialVar_0x8004 
            && GetPlayerTrainerId() == GetMonData(&gPlayerParty[i], MON_DATA_OT_ID, NULL))
            return TRUE;
    }
    return FALSE;
}

/*
 * Determines which of Lorelei's doll collection to show
 * based on how many times you've entered the Hall of Fame.
 */
void UpdateLoreleiDollCollection(void)
{
    u32 numHofClears = GetGameStat(GAME_STAT_ENTERED_HOF);
    if (numHofClears >= 25)
    {
        FlagClear(FLAG_HIDE_LORELEI_HOUSE_MEOWTH_DOLL);
        if (numHofClears >= 50)
            FlagClear(FLAG_HIDE_LORELEI_HOUSE_CHANSEY_DOLL);
        if (numHofClears >= 75)
            FlagClear(FLAG_HIDE_LORELEIS_HOUSE_NIDORAN_F_DOLL);
        if (numHofClears >= 100)
            FlagClear(FLAG_HIDE_LORELEI_HOUSE_JIGGLYPUFF_DOLL);
        if (numHofClears >= 125)
            FlagClear(FLAG_HIDE_LORELEIS_HOUSE_NIDORAN_M_DOLL);
        if (numHofClears >= 150)
            FlagClear(FLAG_HIDE_LORELEIS_HOUSE_FEAROW_DOLL);
        if (numHofClears >= 175)
            FlagClear(FLAG_HIDE_LORELEIS_HOUSE_PIDGEOT_DOLL);
        if (numHofClears >= 200)
            FlagClear(FLAG_HIDE_LORELEIS_HOUSE_LAPRAS_DOLL);
    }
}

void LoopWingFlapSound(void)
{
    // 8004 = Num flaps
    // 8005 = Frame delay between flaps
    CreateTask(Task_WingFlapSound, 8);
    PlaySE(SE_M_WING_ATTACK);
}

static void Task_WingFlapSound(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    data[1]++;
    if (data[1] == gSpecialVar_0x8005)
    {
        data[0]++;
        data[1] = 0;
        PlaySE(SE_M_WING_ATTACK);
    }
    if (data[0] == gSpecialVar_0x8004 - 1)
        DestroyTask(taskId);
}

void ApplyFlashbackGreyscaleTint(void)
{
    CpuFastCopy(gPlttBufferUnfaded, sFlashbackPaletteBackup, PLTT_SIZE);
    sFlashbackPaletteBackupValid = TRUE;

    TintFlashbackPaletteBuffer(gPlttBufferUnfaded);
    CpuFastCopy(gPlttBufferUnfaded, gPlttBufferFaded, PLTT_SIZE);
}

void RestoreFlashbackGreyscaleTint(void)
{
    if (sFlashbackPaletteBackupValid)
    {
        CpuFastCopy(sFlashbackPaletteBackup, gPlttBufferUnfaded, PLTT_SIZE);
        sFlashbackPaletteBackupValid = FALSE;
    }

    RestoreFlashbackFieldEffectPalettes();
    HoldFlashbackFadeWhite();
}
