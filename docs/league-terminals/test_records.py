"""Run the production record/progression functions with host save/RNG fixtures."""
from pathlib import Path
import re,subprocess,tempfile
root=Path(__file__).resolve().parents[2]
source=(root/'src/league_challenge.c').read_text().split('enum { LEAGUE_RESULT_NONE')[1].split('// Keep the original party')[0]
source='enum { LEAGUE_RESULT_NONE'+source
ids=set(re.findall(r'\b(?:TRAINER_\w+|FLAG_SYS_GAME_CLEAR|VAR_STARTER_MON)\b',source))
structs=(root/'include/global.h').read_text();record=structs[structs.index('struct LeagueRecord\n'):structs.index('// logbook menu')]
prefix='''#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
typedef uint8_t u8; typedef uint16_t u16; typedef uint32_t u32; typedef u8 bool8;
#define TRUE 1
#define FALSE 0
#define NELEMS(a) (sizeof(a)/sizeof((a)[0]))
#define LEAGUE_CHALLENGE_NONE 0
#define LEAGUE_CHALLENGE_INDIGO 1
#define LEAGUE_CHALLENGE_ROCKET 2
#define LEAGUE_CHALLENGE_POOL_BATTLE_COUNT 4
#define LEAGUE_CHALLENGE_TOTAL_BATTLE_COUNT 5
#define LEAGUE_CHALLENGE_SELECTED_PARTY_SIZE 4
'''+''.join(f'#define {k} {i+1}\n' for i,k in enumerate(sorted(ids)))+record+'''
struct { unsigned before; struct { u8 active,type,round,selectedPartyMons[4]; u16 trainerIds[5]; } leagueChallenge; struct LeagueRecordsData leagueRecords; unsigned after; } save,*gSaveBlock2Ptr=&save;
u16 gSpecialVar_Result,gSpecialVar_0x8004;
u32 seed=1;
u16 Random(void) {seed=seed*1664525+1013904223;return seed>>16;}
u16 VarGet(u16 v) {return 0;}
u8 FlagGet(u16 f) {return 0;}
void ResetLeagueChallenge(void);
'''
main=r'''
int main(void) {
 const struct LeagueRecord *a,*b;
 struct LeagueRecordsData persisted;
 memset(&save,0xAA,sizeof(save)); save.before=123;save.after=456;save.leagueChallenge.active=0;
 a=GetLeagueRecord(1);assert(a->wins==0 && a->losses==0);assert(save.before==123 && save.after==456);
 for(int run=0;run<2;run++) {
  StartIndigoLeagueChallenge();
  for(int i=0;i<4;i++)for(int j=0;j<i;j++)assert(save.leagueChallenge.trainerIds[i]!=save.leagueChallenge.trainerIds[j]);
  for(int i=0;i<4;i++)AdvanceLeagueChallengeRound();
  assert(save.leagueChallenge.round==4 && a->championships==run);
  AdvanceLeagueChallengeRound(); assert(!save.leagueChallenge.active);
  assert(a->championships==run+1 && a->streak==run+1 && a->bestStreak==run+1);
  ResetLeagueChallenge();assert(a->streak==run+1);
 }
 assert(a->wins==10);
 StartIndigoLeagueChallenge();AdvanceLeagueChallengeRound();ResetLeagueChallenge();
 assert(a->wins==11 && a->losses==0 && a->streak==0 && a->bestStreak==2 && a->lastPoolWins==1 && a->lastResult==LEAGUE_RESULT_WITHDRAWN);
 StartRocketLeagueChallenge();b=GetLeagueRecord(2);AdvanceLeagueChallengeRound();AdvanceLeagueChallengeRound();
 RecordLeagueChallengeLoss();RecordLeagueChallengeLoss();ResetLeagueChallenge();
 assert(b->wins==2 && b->losses==1 && b->lastPoolWins==2 && b->lastResult==LEAGUE_RESULT_LOST);
 assert(a->wins==11 && a->losses==0);
 StartRocketLeagueChallenge();for(int i=0;i<5;i++) {AdvanceLeagueChallengeRound();gSpecialVar_0x8004=i==4?1000:3000;RecordRocketLeagueCoins();}
 assert(b->wins==7 && b->championships==1 && b->streak==1 && b->coinsLow==13000 && b->coinsHigh==0);
 persisted=save.leagueRecords;memset(&save.leagueRecords,0,sizeof(persisted));save.leagueRecords=persisted;
 assert(GetLeagueRecord(2)->wins==7 && GetLeagueRecord(1)->wins==11);
 AdvanceLeagueChallengeRound();assert(b->wins==7);
 ((struct LeagueRecord*)b)->wins=65535;((struct LeagueRecord*)b)->losses=65535;
 StartRocketLeagueChallenge();AdvanceLeagueChallengeRound();RecordLeagueChallengeLoss();ResetLeagueChallenge();
 assert(b->wins==65535 && b->losses==65535 && b->streak==0);
 ((struct LeagueRecord*)b)->coinsLow=999999999&65535;((struct LeagueRecord*)b)->coinsHigh=999999999>>16;
 gSpecialVar_0x8004=3000;RecordRocketLeagueCoins();assert((b->coinsLow|((u32)b->coinsHigh<<16))==999999999);
 assert(save.before==123 && save.after==456);
 puts("PASS: legacy initialization, distinct leagues, 4+Champion progression, title streaks, withdrawals, single-count losses, coin totals, save roundtrip and saturation.");
}
'''
with tempfile.TemporaryDirectory(prefix='league-records-') as d:
 p=Path(d);(p/'test.c').write_text(prefix+source+main)
 subprocess.run(['cc','-std=c99','-Wall','-Werror',str(p/'test.c'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
