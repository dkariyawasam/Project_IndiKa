#ifndef GUARD_LEAGUE_CHALLENGE_H
#define GUARD_LEAGUE_CHALLENGE_H

#include "global.h"

const struct LeagueRecord *GetLeagueRecord(u8 type);
void RecordLeagueChallengeLoss(void);
void RecordRocketLeagueCoins(void);
void BufferLeagueTerminalStatus(void);
void ShowLeagueRecords(void);

bool8 IsIndigoLeagueTrainerBattle(void);
void PrepareIndigoLeagueBattle(void);
void RestoreIndigoLeagueParty(void);

void StartIndigoLeagueChallenge(void);
void StartRocketLeagueChallenge(void);
void ResetLeagueChallenge(void);
void AdvanceLeagueChallengeRound(void);
void GetLeagueChallengeCurrentTrainer(void);
void GetLeagueChallengeTrainerForRound(void);
void GetLeagueChallengeRound(void);
void GetLeagueChallengeType(void);
void IsLeagueChallengeActive(void);

#endif // GUARD_LEAGUE_CHALLENGE_H
