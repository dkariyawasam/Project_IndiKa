"""Run production league snapshot/scaling code against a host Pokémon API fixture.
This checks control flow and data preservation; it is not a full emulator test.
"""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[2]
source=(root/'src/league_challenge.c').read_text().split('// Keep the original party')[1].split('static const u8 sTerminalActive')[0]
prefix=r'''
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
typedef uint8_t u8; typedef uint16_t u16; typedef uint32_t u32; typedef u8 bool8;
#define EWRAM_DATA
#define TRUE 1
#define FALSE 0
#define PARTY_SIZE 6
#define MAX_MON_MOVES 4
#define SPECIES_NONE 0
#define SPECIES_EGG 99
#define BATTLE_TYPE_TRAINER 1
#define BATTLE_TYPE_LINK 2
#define BATTLE_TYPE_BATTLE_TOWER 4
#define BATTLE_TYPE_EREADER_TRAINER 8
#define LEAGUE_CHALLENGE_INDIGO 1
#define MAP_POKEMON_LEAGUE_BRUNOS_ROOM 328
#define MAP_POKEMON_LEAGUE_CHAMPIONS_ROOM 330
#define MAP_GROUP(x) ((x)>>8)
#define MAP_NUM(x) ((x)&255)
enum {MON_DATA_SPECIES_OR_EGG,MON_DATA_EXP,MON_DATA_MAX_HP,MON_DATA_HP,MON_DATA_STATUS,MON_DATA_PP_BONUSES,MON_DATA_MOVE1,MON_DATA_PP1=10};
struct Pokemon { u32 fields[14]; u8 level; u8 preserved[30]; };
struct Pokemon gPlayerParty[6],gEnemyParty[6];
struct { struct {u8 mapGroup,mapNum;} location;} save1,*gSaveBlock1Ptr=&save1;
struct { struct {u8 active,type;} leagueChallenge;} save2,*gSaveBlock2Ptr=&save2;
u32 gBattleTypeFlags, gExperienceTables[1][101];
struct {u8 growthRate;} gSpeciesInfo[100];
u32 GetMonData(struct Pokemon *m,int field) {return m->fields[field];}
void SetMonData(struct Pokemon *m,int field,const void *v) {
 if(field==MON_DATA_HP) m->fields[field]=*(const u16*)v;
 else if(field>=MON_DATA_PP1) m->fields[field]=*(const u8*)v;
 else memcpy(&m->fields[field],v,4);
}
void CalculateMonStats(struct Pokemon *m) { m->level=m->fields[MON_DATA_EXP]/100; m->fields[MON_DATA_MAX_HP]=m->level*3; }
u8 CalculatePPWithBonus(u16 move,u8 bonus,u8 slot) {return move?10+bonus:0;}
void HealPlayerParty(void) {for(int i=0;i<6;i++){struct Pokemon *m=&gPlayerParty[i];m->fields[MON_DATA_HP]=m->fields[MON_DATA_MAX_HP];m->fields[MON_DATA_STATUS]=0;for(int j=0;j<4;j++)m->fields[MON_DATA_PP1+j]=CalculatePPWithBonus(m->fields[MON_DATA_MOVE1+j],m->fields[MON_DATA_PP_BONUSES],j);}}
'''
main=r'''
int main(void) {
 struct Pokemon expected[6], enemies[6], tmp;
 gExperienceTables[0][50]=5000;
 gBattleTypeFlags=BATTLE_TYPE_TRAINER;
 save1.location.mapGroup=1; save1.location.mapNum=72;
 save2.leagueChallenge.active=1;save2.leagueChallenge.type=1;
 for(int i=0;i<6;i++) {
  struct Pokemon *m=&gPlayerParty[i];
  m->fields[MON_DATA_SPECIES_OR_EGG]=i+1;
  m->fields[MON_DATA_EXP]=(10+i*17)*100+37;
  CalculateMonStats(m);m->fields[MON_DATA_HP]=i;
  m->fields[MON_DATA_STATUS]=8;m->fields[MON_DATA_PP_BONUSES]=3;
  for(int j=0;j<4;j++)m->fields[MON_DATA_MOVE1+j]=j+1;
  memset(m->preserved,20+i,sizeof(m->preserved));
 }
 memcpy(gEnemyParty,gPlayerParty,sizeof(gPlayerParty));
 memcpy(enemies,gEnemyParty,sizeof(enemies));
 memcpy(expected,gPlayerParty,sizeof(expected));
 // Rocket, unrelated maps, inactive challenges and link battles are untouched.
 for(int test=0;test<4;test++) {
  save2.leagueChallenge.type=test==0?2:1;
  save1.location.mapNum=test==1?20:72;
  save2.leagueChallenge.active=test==2?0:1;
  gBattleTypeFlags=test==3?BATTLE_TYPE_TRAINER|BATTLE_TYPE_LINK:BATTLE_TYPE_TRAINER;
  PrepareIndigoLeagueBattle(); assert(!memcmp(expected,gPlayerParty,sizeof(expected)));assert(!sIndigoPartySaved);
 }
 save1.location.mapNum=72;save2.leagueChallenge.active=1;gBattleTypeFlags=BATTLE_TYPE_TRAINER;
 // Both arena and Champion, plus every terminal outcome: restoration is unconditional.
 for(int room=72;room<=74;room+=2) for(int outcome=0;outcome<3;outcome++) {
  save1.location.mapNum=room;
  PrepareIndigoLeagueBattle();assert(sIndigoPartySaved);
  for(int i=0;i<6;i++) {assert(gPlayerParty[i].level==50);assert(gEnemyParty[i].level==50);assert(gPlayerParty[i].fields[MON_DATA_HP]==150);assert(gPlayerParty[i].fields[MON_DATA_STATUS]==0);assert(!memcmp(gPlayerParty[i].preserved,expected[i].preserved,30));}
  PrepareIndigoLeagueBattle(); // Re-entry must not overwrite the original snapshot.
  tmp=gPlayerParty[0];gPlayerParty[0]=gPlayerParty[5];gPlayerParty[5]=tmp;
  memset(gPlayerParty[2].preserved,0,30); // Consumed item / battle mutations.
  gPlayerParty[0].fields[MON_DATA_HP]=0;
  RestoreIndigoLeagueParty(); assert(!sIndigoPartySaved);
  memcpy(gEnemyParty,gPlayerParty,sizeof(gPlayerParty));
  // Expected original Pokémon, with only HP/status/PP recovered.
  struct Pokemon actual[6];memcpy(actual,gPlayerParty,sizeof(actual));
  memcpy(gPlayerParty,expected,sizeof(expected));HealPlayerParty();
  assert(!memcmp(actual,gPlayerParty,sizeof(actual)));
  memcpy(expected,gPlayerParty,sizeof(expected));
  RestoreIndigoLeagueParty();assert(!memcmp(expected,gPlayerParty,sizeof(expected)));
 }
 puts("PASS: both teams scale; six-party identity/order/EXP/items survive; healing and idempotent restoration; arena/Champion and non-Indigo isolation.");
}
'''
with tempfile.TemporaryDirectory(prefix='indigo-rules-') as d:
 p=Path(d);(p/'test.c').write_text(prefix+'\n// Keep the original party'+source+main)
 subprocess.run(['cc','-std=c99','-Wall','-Werror',str(p/'test.c'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
