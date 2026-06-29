#include "global.h"
#include "event_data.h"
#include "league_challenge.h"
#include "random.h"
#include "constants/opponents.h"
#include "constants/vars.h"

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
        if (FlagGet(FLAG_SYS_GAME_CLEAR))
            return TRAINER_CHAMPION_REMATCH_CHARMANDER;
        else
            return TRAINER_CHAMPION_FIRST_CHARMANDER;
    case 1:
        if (FlagGet(FLAG_SYS_GAME_CLEAR))
            return TRAINER_CHAMPION_REMATCH_BULBASAUR;
        else
            return TRAINER_CHAMPION_FIRST_BULBASAUR;
    case 2:
    default:
        if (FlagGet(FLAG_SYS_GAME_CLEAR))
            return TRAINER_CHAMPION_REMATCH_SQUIRTLE;
        else
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

    gSaveBlock2Ptr->leagueChallenge.active = FALSE;
    gSaveBlock2Ptr->leagueChallenge.type = LEAGUE_CHALLENGE_NONE;
    gSaveBlock2Ptr->leagueChallenge.round = 0;

    for (i = 0; i < LEAGUE_CHALLENGE_SELECTED_PARTY_SIZE; i++)
        gSaveBlock2Ptr->leagueChallenge.selectedPartyMons[i] = 0;

    for (i = 0; i < LEAGUE_CHALLENGE_TOTAL_BATTLE_COUNT; i++)
        gSaveBlock2Ptr->leagueChallenge.trainerIds[i] = 0;

    gSpecialVar_Result = 0;
}

void AdvanceLeagueChallengeRound(void)
{
    if (!gSaveBlock2Ptr->leagueChallenge.active)
    {
        gSpecialVar_Result = 0;
        return;
    }

    if (gSaveBlock2Ptr->leagueChallenge.round < LEAGUE_CHALLENGE_TOTAL_BATTLE_COUNT - 1)
    {
        gSaveBlock2Ptr->leagueChallenge.round++;
        gSpecialVar_Result = gSaveBlock2Ptr->leagueChallenge.trainerIds[gSaveBlock2Ptr->leagueChallenge.round];
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
        gSpecialVar_Result = gSaveBlock2Ptr->leagueChallenge.trainerIds[gSaveBlock2Ptr->leagueChallenge.round];
}

void GetLeagueChallengeTrainerForRound(void)
{
    if (gSpecialVar_0x8004 >= LEAGUE_CHALLENGE_TOTAL_BATTLE_COUNT)
        gSpecialVar_Result = 0;
    else
        gSpecialVar_Result = gSaveBlock2Ptr->leagueChallenge.trainerIds[gSpecialVar_0x8004];
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
