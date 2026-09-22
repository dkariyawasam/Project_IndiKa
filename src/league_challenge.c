#include "global.h"
#include "event_data.h"
#include "league_challenge.h"
#include "random.h"
#include "string_util.h"
#include "battle.h"
#include "pokemon.h"
#include "script_pokemon_util.h"
#include "constants/maps.h"
#include "constants/species.h"
#include "constants/opponents.h"
#include "constants/vars.h"

enum { LEAGUE_RESULT_NONE, LEAGUE_RESULT_ACTIVE, LEAGUE_RESULT_WON, LEAGUE_RESULT_LOST, LEAGUE_RESULT_WITHDRAWN };

static struct LeagueRecord *MutableLeagueRecord(u8 type)
{
    struct LeagueRecordsData *data = &gSaveBlock2Ptr->leagueRecords;
    if (type != LEAGUE_CHALLENGE_INDIGO && type != LEAGUE_CHALLENGE_ROCKET)
        return NULL;
    if (data->magic != 0x4C47 || data->version != 1)
    {
        memset(data, 0, sizeof(*data));
        data->magic = 0x4C47;
        data->version = 1;
    }
    return &data->league[type - 1];
}

const struct LeagueRecord *GetLeagueRecord(u8 type)
{
    return MutableLeagueRecord(type);
}

static void IncrementLeagueCounter(u16 *counter)
{
    if (*counter < 65535)
        (*counter)++;
}

static void RecordLeagueWin(void)
{
    struct LeagueRecord *record = MutableLeagueRecord(gSaveBlock2Ptr->leagueChallenge.type);
    if (record == NULL)
        return;
    IncrementLeagueCounter(&record->wins);
    if (gSaveBlock2Ptr->leagueChallenge.round < LEAGUE_CHALLENGE_POOL_BATTLE_COUNT)
        record->lastPoolWins = gSaveBlock2Ptr->leagueChallenge.round + 1;
    else
    {
        record->lastPoolWins = LEAGUE_CHALLENGE_POOL_BATTLE_COUNT;
        record->lastResult = LEAGUE_RESULT_WON;
        IncrementLeagueCounter(&record->championships);
        IncrementLeagueCounter(&record->streak);
        if (record->streak > record->bestStreak)
            record->bestStreak = record->streak;
    }
}

void RecordLeagueChallengeLoss(void)
{
    struct LeagueRecord *record;
    if (!gSaveBlock2Ptr->leagueChallenge.active)
        return;
    record = MutableLeagueRecord(gSaveBlock2Ptr->leagueChallenge.type);
    if (record == NULL)
        return;
    IncrementLeagueCounter(&record->losses);
    record->lastPoolWins = gSaveBlock2Ptr->leagueChallenge.round;
    record->lastResult = LEAGUE_RESULT_LOST;
    record->streak = 0;
    // The subsequent reset must not also count this as a withdrawal.
    gSaveBlock2Ptr->leagueChallenge.active = FALSE;
}

void RecordRocketLeagueCoins(void)
{
    struct LeagueRecord *record = MutableLeagueRecord(LEAGUE_CHALLENGE_ROCKET);
    u32 coins = record->coinsLow | ((u32)record->coinsHigh << 16);
    if (coins > 999999999 - gSpecialVar_0x8004)
        coins = 999999999;
    else
        coins += gSpecialVar_0x8004;
    record->coinsLow = coins;
    record->coinsHigh = coins >> 16;
}

static const u16 sIndigoLeagueTrainerPool[] = {
    TRAINER_ELITE_FOUR_LORELEI,
    TRAINER_ELITE_FOUR_BRUNO,
    TRAINER_ELITE_FOUR_AGATHA,
    TRAINER_ELITE_FOUR_LANCE,
    TRAINER_LEADER_BROCK_7,
    TRAINER_LEADER_MISTY_7,
    TRAINER_LEADER_LT_SURGE_7,
    TRAINER_LEADER_ERIKA_7,
    TRAINER_LEADER_KOGA_7,
    TRAINER_LEADER_BLAINE_7,
    TRAINER_LEADER_SABRINA_7,
    TRAINER_LEADER_GIOVANNI,
};

static const u16 sRocketLeagueTrainerPool[] = {
    TRAINER_ROCKET_ADMIN_PETREL,
    TRAINER_ROCKET_ADMIN_ARCHER,
    TRAINER_ROCKET_ADMIN_ARIANA,
    TRAINER_ROCKET_ADMIN_PROTON,
    TRAINER_ROCKET_ACE_RAZOR,
    TRAINER_ROCKET_ACE_VEX,
    TRAINER_ROCKET_ACE_NOVA,
    TRAINER_ROCKET_ACE_KNELL,
    TRAINER_ROCKET_ACE_MAKO,
    TRAINER_ROCKET_ACE_SABLE,
    TRAINER_ROCKET_ACE_JET,
    TRAINER_ROCKET_ACE_VIPER,
};

static u16 GetIndigoChampionTrainerId(void)
{
    switch (VarGet(VAR_STARTER_MON))
    {
    case 0:
        return TRAINER_CHAMPION_FIRST_CHARMANDER;
    case 1:
        return TRAINER_CHAMPION_FIRST_BULBASAUR;
    case 2:
    default:
        return TRAINER_CHAMPION_FIRST_SQUIRTLE;
    }
}

static bool8 HasTrainerAlreadyBeenChosen(u16 trainerId, u8 numChosen)
{
    u8 i;

    for (i = 0; i < numChosen; i++)
    {
        if (gSaveBlock2Ptr->leagueChallenge.trainerIds[i] == trainerId)
            return TRUE;
    }

    return FALSE;
}

static void ChooseLeagueChallengePoolTrainers(const u16 *pool, u8 poolCount)
{
    u8 i;
    u16 trainerId;

    for (i = 0; i < LEAGUE_CHALLENGE_POOL_BATTLE_COUNT; i++)
    {
        do
        {
            trainerId = pool[Random() % poolCount];
        } while (HasTrainerAlreadyBeenChosen(trainerId, i));

        gSaveBlock2Ptr->leagueChallenge.trainerIds[i] = trainerId;
    }
}

static void StartLeagueChallenge(u8 type)
{
    ResetLeagueChallenge();
    gSaveBlock2Ptr->leagueChallenge.type = type;

    switch (type)
    {
    case LEAGUE_CHALLENGE_INDIGO:
        ChooseLeagueChallengePoolTrainers(sIndigoLeagueTrainerPool, NELEMS(sIndigoLeagueTrainerPool));
        gSaveBlock2Ptr->leagueChallenge.trainerIds[LEAGUE_CHALLENGE_TOTAL_BATTLE_COUNT - 1] = GetIndigoChampionTrainerId();
        gSaveBlock2Ptr->leagueChallenge.active = TRUE;
        break;
    case LEAGUE_CHALLENGE_ROCKET:
        ChooseLeagueChallengePoolTrainers(sRocketLeagueTrainerPool, NELEMS(sRocketLeagueTrainerPool));
        gSaveBlock2Ptr->leagueChallenge.trainerIds[LEAGUE_CHALLENGE_TOTAL_BATTLE_COUNT - 1] = TRAINER_ROCKET_LEAGUE_CHAMPION_GIOVANNI;
        gSaveBlock2Ptr->leagueChallenge.active = TRUE;
        break;
    default:
        gSaveBlock2Ptr->leagueChallenge.type = LEAGUE_CHALLENGE_NONE;
        break;
    }

    if (gSaveBlock2Ptr->leagueChallenge.active)
    {
        struct LeagueRecord *record = MutableLeagueRecord(type);
        record->lastPoolWins = 0;
        record->lastResult = LEAGUE_RESULT_ACTIVE;
    }
    gSpecialVar_Result = gSaveBlock2Ptr->leagueChallenge.trainerIds[0];
}

void StartIndigoLeagueChallenge(void)
{
    StartLeagueChallenge(LEAGUE_CHALLENGE_INDIGO);
}

void StartRocketLeagueChallenge(void)
{
    StartLeagueChallenge(LEAGUE_CHALLENGE_ROCKET);
}

void ResetLeagueChallenge(void)
{
    u8 i;

    if (gSaveBlock2Ptr->leagueChallenge.active)
    {
        struct LeagueRecord *record = MutableLeagueRecord(gSaveBlock2Ptr->leagueChallenge.type);
        if (record != NULL)
        {
            record->lastPoolWins = gSaveBlock2Ptr->leagueChallenge.round;
            record->lastResult = LEAGUE_RESULT_WITHDRAWN;
            record->streak = 0;
        }
    }
    gSaveBlock2Ptr->leagueChallenge.active = FALSE;
    gSaveBlock2Ptr->leagueChallenge.type = LEAGUE_CHALLENGE_NONE;
    gSaveBlock2Ptr->leagueChallenge.round = 0;

    for (i = 0; i < LEAGUE_CHALLENGE_SELECTED_PARTY_SIZE; i++)
        gSaveBlock2Ptr->leagueChallenge.selectedPartyMons[i] = 0;

    for (i = 0; i < LEAGUE_CHALLENGE_TOTAL_BATTLE_COUNT; i++)
        gSaveBlock2Ptr->leagueChallenge.trainerIds[i] = 0;

    gSpecialVar_Result = 0;
}

// Resolve the champion from the current roster, including League runs saved before
// the alternate Champion parties were removed.
static u16 GetLeagueTrainerId(u8 round)
{
    if (gSaveBlock2Ptr->leagueChallenge.type == LEAGUE_CHALLENGE_INDIGO
     && round == LEAGUE_CHALLENGE_TOTAL_BATTLE_COUNT - 1)
        return GetIndigoChampionTrainerId();
    return gSaveBlock2Ptr->leagueChallenge.trainerIds[round];
}

void AdvanceLeagueChallengeRound(void)
{
    if (!gSaveBlock2Ptr->leagueChallenge.active)
    {
        gSpecialVar_Result = 0;
        return;
    }

    RecordLeagueWin();
    if (gSaveBlock2Ptr->leagueChallenge.round < LEAGUE_CHALLENGE_TOTAL_BATTLE_COUNT - 1)
    {
        gSaveBlock2Ptr->leagueChallenge.round++;
        gSpecialVar_Result = GetLeagueTrainerId(gSaveBlock2Ptr->leagueChallenge.round);
    }
    else
    {
        gSaveBlock2Ptr->leagueChallenge.active = FALSE;
        gSaveBlock2Ptr->leagueChallenge.type = LEAGUE_CHALLENGE_NONE;
        gSpecialVar_Result = 0;
    }
}

void GetLeagueChallengeCurrentTrainer(void)
{
    if (!gSaveBlock2Ptr->leagueChallenge.active)
        gSpecialVar_Result = 0;
    else
        gSpecialVar_Result = GetLeagueTrainerId(gSaveBlock2Ptr->leagueChallenge.round);
}

void GetLeagueChallengeTrainerForRound(void)
{
    if (gSpecialVar_0x8004 >= LEAGUE_CHALLENGE_TOTAL_BATTLE_COUNT)
        gSpecialVar_Result = 0;
    else
        gSpecialVar_Result = GetLeagueTrainerId(gSpecialVar_0x8004);
}

void GetLeagueChallengeRound(void)
{
    gSpecialVar_Result = gSaveBlock2Ptr->leagueChallenge.round;
}

void GetLeagueChallengeType(void)
{
    gSpecialVar_Result = gSaveBlock2Ptr->leagueChallenge.type;
}

void IsLeagueChallengeActive(void)
{
    gSpecialVar_Result = gSaveBlock2Ptr->leagueChallenge.active;
}

// Keep the original party outside the movable save blocks and battle heap.
// A whole-party snapshot also restores consumed/stolen held items and party order.
static EWRAM_DATA struct Pokemon sIndigoOriginalParty[PARTY_SIZE] = {0};
static EWRAM_DATA bool8 sIndigoPartySaved = FALSE;

bool8 IsIndigoLeagueTrainerBattle(void)
{
    return (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        && !(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_EREADER_TRAINER))
        && gSaveBlock2Ptr->leagueChallenge.active
        && gSaveBlock2Ptr->leagueChallenge.type == LEAGUE_CHALLENGE_INDIGO
        && gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_POKEMON_LEAGUE_BRUNOS_ROOM)
        && (gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_POKEMON_LEAGUE_BRUNOS_ROOM)
         || gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_POKEMON_LEAGUE_CHAMPIONS_ROOM));
}

static void SetIndigoPartyLevel(struct Pokemon *party)
{
    u8 i, slot;
    u32 exp, status = 0;
    u16 species, hp, move;
    u8 pp, bonuses;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        species = GetMonData(&party[i], MON_DATA_SPECIES_OR_EGG);
        if (species == SPECIES_NONE || species == SPECIES_EGG)
            continue;
        exp = gExperienceTables[gSpeciesInfo[species].growthRate][50];
        SetMonData(&party[i], MON_DATA_EXP, &exp);
        CalculateMonStats(&party[i]);
        hp = GetMonData(&party[i], MON_DATA_MAX_HP);
        SetMonData(&party[i], MON_DATA_HP, &hp);
        SetMonData(&party[i], MON_DATA_STATUS, &status);
        bonuses = GetMonData(&party[i], MON_DATA_PP_BONUSES);
        for (slot = 0; slot < MAX_MON_MOVES; slot++)
        {
            move = GetMonData(&party[i], MON_DATA_MOVE1 + slot);
            pp = CalculatePPWithBonus(move, bonuses, slot);
            SetMonData(&party[i], MON_DATA_PP1 + slot, &pp);
        }
    }
}

void PrepareIndigoLeagueBattle(void)
{
    if (!IsIndigoLeagueTrainerBattle() || sIndigoPartySaved)
        return;
    memcpy(sIndigoOriginalParty, gPlayerParty, sizeof(sIndigoOriginalParty));
    sIndigoPartySaved = TRUE;
    SetIndigoPartyLevel(gPlayerParty);
    SetIndigoPartyLevel(gEnemyParty);
}

void RestoreIndigoLeagueParty(void)
{
    if (!sIndigoPartySaved)
        return;
    memcpy(gPlayerParty, sIndigoOriginalParty, sizeof(sIndigoOriginalParty));
    sIndigoPartySaved = FALSE;
    HealPlayerParty();
}

static const u8 sTerminalActive[] = _("Pool victories: {STR_VAR_1} of 4.\n{STR_VAR_2}");
static const u8 sTerminalInactive[] = _("No active challenge.\pLast run: {STR_VAR_1} of 4 pool wins.\n{STR_VAR_2}");
static const u8 sTerminalNew[] = _("No challenge recorded yet.\pEnter the League to begin!");
static const u8 sTerminalChampion[] = _("The CHAMPION is next.");
static const u8 sTerminalNext[] = _("Return to the arena to continue.");
static const u8 sTerminalWon[] = _("CHAMPION defeated!");
static const u8 sTerminalLost[] = _("The run ended in defeat.");
static const u8 sTerminalWithdrawn[] = _("The run was withdrawn.");

void BufferLeagueTerminalStatus(void)
{
    u8 type = gSpecialVar_0x8004;
    const struct LeagueRecord *record = GetLeagueRecord(type);
    if (record == NULL)
        return;
    if (gSaveBlock2Ptr->leagueChallenge.active && gSaveBlock2Ptr->leagueChallenge.type == type)
    {
        u8 round = gSaveBlock2Ptr->leagueChallenge.round;
        ConvertIntToDecimalStringN(gStringVar1, round, STR_CONV_MODE_LEFT_ALIGN, 1);
        StringCopy(gStringVar2, round == 4 ? sTerminalChampion : sTerminalNext);
        StringExpandPlaceholders(gStringVar4, sTerminalActive);
    }
    else if (record->lastResult == LEAGUE_RESULT_NONE)
        StringCopy(gStringVar4, sTerminalNew);
    else
    {
        ConvertIntToDecimalStringN(gStringVar1, record->lastPoolWins, STR_CONV_MODE_LEFT_ALIGN, 1);
        StringCopy(gStringVar2, record->lastResult == LEAGUE_RESULT_WON ? sTerminalWon : record->lastResult == LEAGUE_RESULT_LOST ? sTerminalLost : sTerminalWithdrawn);
        StringExpandPlaceholders(gStringVar4, sTerminalInactive);
    }
}
