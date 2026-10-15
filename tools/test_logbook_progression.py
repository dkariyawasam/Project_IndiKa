#!/usr/bin/env python3
"""Exercise production logbook state readers with synthetic old and new saves."""
from pathlib import Path
import re
import subprocess
import tempfile
ROOT = Path(__file__).resolve().parents[1]
source = (ROOT/'src/quests.c').read_text()
def function(name):
    m = re.search(r'(?:static )?(?:u8|s8|bool8|void) '+name+r'\([^;]*?\)\n\{', source)
    assert m, name
    return source[m.start():source.index('\n}', m.end())+2]
enum = re.search(r'enum QuestCases\s*\{.*?\};', (ROOT/'include/quests.h').read_text(), re.S)[0]
promises = re.search(r'static const struct \{ u16 heard; u16 done; \} sFieldPromises\[\] = \{.*?\n\};', source, re.S)[0]
code = r'''
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "constants/flags.h"
#include "constants/vars.h"
#include "constants/species.h"
#include "constants/opponents.h"
#include "constants/quests.h"
typedef uint32_t u32; typedef uint8_t u8; typedef uint16_t u16; typedef int8_t s8; typedef u8 bool8;
#define TRUE 1
#define FALSE 0
#define NELEMS(a) (sizeof(a)/sizeof((a)[0]))
#define LEAGUE_CHALLENGE_ROCKET 2
#define LEAGUE_CHALLENGE_INDIGO 1
static u8 flags[65536], caught[NUM_SPECIES], seen[NUM_SPECIES];
static u16 vars[65536];
static bool8 ready;
static bool8 FlagGet(u16 n) {return flags[n];}
static void FlagSet(u16 n) {flags[n]=1;}
static u16 VarGet(u16 n) {return vars[n];}
static u16 SpeciesToNationalPokedexNum(u16 n) {return n;}
static u8 GetSetPokedexFlag(u16 n, u8 mode) {return mode == 1 ? caught[n] : seen[n];}
#define FLAG_GET_SEEN 0
#define FLAG_GET_CAUGHT 1
static bool8 IsSpeciesCaught(u16 n) {return caught[n];}
static bool8 IsOakExpeditionReady(void) {return ready;}
struct LeagueRecord {u16 wins, championships, lastResult;};
static struct LeagueRecord records[3];
static const struct LeagueRecord *GetLeagueRecord(u8 type) {return &records[type];}
static struct {u8 questData[6], subQuests[3], unlockedSubquests[3];} save, *gSaveBlock2Ptr = &save;
static const struct {u8 id;} children[24] = {{0},{1},{2},{3},{4},{5},{6},{7},{8},{9},{10},{11},{12},{13},{14},{15},{16},{17},{18},{19},{20},{21},{22},{23}};
static const struct {u8 numSubquests; const void *unused; const typeof(children[0]) *subquests;} sSideQuests[9] = {
 {3,0,children},{8,0,children+3},{8,0,children+11},{5,0,children+19},{0},{0},{0},{21},{6}
};
'''+enum+'\n'+promises+'\n'
code += re.search(r'static const u16 sPrimevalSpecies\[\] = \{.*?\n\};', source, re.S)[0]+'\n'
code += (ROOT/'src/data/nature_research.h').read_text()+'\n'
code += r'''
#define STR_CONV_MODE_LEFT_ALIGN 0
static u8 gStringVar1[32], gStringVar2[32], gStringVar4[64], sText_QuestNumberDisplay[1];
static void ConvertIntToDecimalStringN(u8 *dst, unsigned value, int mode, int digits) {sprintf((char *)dst, "%u", value);}
static void StringExpandPlaceholders(u8 *dst, const u8 *src) {sprintf((char *)dst, "%s/%s", gStringVar1, gStringVar2);}
'''
for name in ['IsPrimevalRecorded','RecordNatureResearchEvolution','CountEvolutionThroughBondMilestones','CountEvolutionThroughDesignMilestones','CountApexInteractionsForNatureQuest','TryGenerateNatureSubquestCounter','IsGymTrialTraded','IsGymTrialCompleted','IsPromiseKnown','GetDerivedQuestState','LogbookMenu_GetSetQuestState','LogbookMenu_GetSetSubquestState']:
    code += function(name)+'\n'
code += r'''
static void reset(void) {memset(&save,0,sizeof(save));memset(flags,0,sizeof(flags));memset(caught,0,sizeof(caught));memset(seen,0,sizeof(seen));memset(vars,0,sizeof(vars));memset(records,0,sizeof(records));ready=0;}
int main(void) {
 int i; u8 before[6], color;
 assert(QUEST_COUNT==9 && SUB_QUEST_COUNT==24 && sizeof(save.questData)==6); // original save arrays stay six/three/three bytes
 reset();
 for(i=0;i<QUEST_COUNT;i++) assert(!LogbookMenu_GetSetQuestState(i,FLAG_GET_UNLOCKED));
 LogbookMenu_GetSetQuestState(QUEST_THE_ROCKET_LEAGUE,FLAG_SET_UNLOCKED);
 assert(LogbookMenu_GetSetQuestState(QUEST_THE_ROCKET_LEAGUE,FLAG_GET_ACTIVE));
 records[2].championships=1;
 assert(LogbookMenu_GetSetQuestState(4,FLAG_GET_COMPLETED));assert(!LogbookMenu_GetSetQuestState(4,FLAG_GET_ACTIVE));
 flags[FLAG_SYS_GAME_CLEAR]=1;
 assert(LogbookMenu_GetSetQuestState(5,FLAG_GET_UNLOCKED));assert(LogbookMenu_GetSetQuestState(5,FLAG_GET_COMPLETED));
 reset();
 assert(!LogbookMenu_GetSetQuestState(7,FLAG_GET_UNLOCKED));
 flags[FLAG_HEARD_HAUNTER_REQUEST]=1;
 assert(LogbookMenu_GetSetSubquestState(7,FLAG_GET_UNLOCKED,0));assert(!LogbookMenu_GetSetSubquestState(7,FLAG_GET_UNLOCKED,1));
 assert(LogbookMenu_GetSetQuestState(7,FLAG_GET_ACTIVE));
 flags[FLAG_DID_HAUNTER_TRADE]=1;
 assert(LogbookMenu_GetSetQuestState(7,FLAG_GET_COMPLETED));
 flags[FLAG_HEARD_KADABRA_REQUEST]=1;
 assert(!LogbookMenu_GetSetQuestState(7,FLAG_GET_COMPLETED));
 reset();flags[FLAG_DID_KADABRA_TRADE]=1;
 assert(LogbookMenu_GetSetSubquestState(7,FLAG_GET_UNLOCKED,1)); // old save, no discovery flag
 flags[TRAINER_FLAGS_START+TRAINER_BLACK_BELT_KOICHI]=1;
 assert(LogbookMenu_GetSetSubquestState(7,FLAG_GET_UNLOCKED,16));
 assert(!LogbookMenu_GetSetSubquestState(7,FLAG_GET_COMPLETED,16));
 flags[FLAG_GOT_DOJO_BLACK_BELT]=1;
 assert(LogbookMenu_GetSetSubquestState(7,FLAG_GET_COMPLETED,16));
 reset();flags[FLAG_DEFEATED_ERIKA]=1;
 assert(LogbookMenu_GetSetSubquestState(7,FLAG_GET_UNLOCKED,17));
 assert(!LogbookMenu_GetSetSubquestState(7,FLAG_GET_COMPLETED,17));
 flags[FLAG_GOT_BUDEW_FROM_ERIKA]=1;assert(LogbookMenu_GetSetSubquestState(7,FLAG_GET_COMPLETED,17));
 reset();caught[SPECIES_PORYGON2]=caught[SPECIES_RHYPERIOR]=1;
 assert(CountEvolutionThroughBondMilestones()==0);assert(CountEvolutionThroughDesignMilestones()==0);
 RecordNatureResearchEvolution(SPECIES_PORYGON,SPECIES_PORYGON2,TRUE);
 RecordNatureResearchEvolution(SPECIES_PORYGON2,SPECIES_PORYGON3,TRUE);
 assert(CountEvolutionThroughDesignMilestones()==2);
 RecordNatureResearchEvolution(SPECIES_PORYGON2,SPECIES_PORYGON3,TRUE);
 assert(CountEvolutionThroughDesignMilestones()==2); // repeat does not double count
 RecordNatureResearchEvolution(SPECIES_ELECTABUZZ,SPECIES_ELECTIVIRE,TRUE);
 assert(CountEvolutionThroughBondMilestones()==1 && CountEvolutionThroughDesignMilestones()==2);
 flags[FLAG_OAK_ACKNOWLEDGED_BOND]=1;assert(CountEvolutionThroughBondMilestones()>=2);
 LogbookMenu_GetSetQuestState(0,FLAG_SET_UNLOCKED);
 flags[FLAG_INTERACTED_APEX_TANGROWTH]=flags[FLAG_INTERACTED_APEX_ZAPDOS]=1;
 assert(LogbookMenu_GetSetQuestState(0,FLAG_GET_COMPLETED));
 for(i=0;i<3;i++) assert(LogbookMenu_GetSetSubquestState(0,FLAG_GET_COMPLETED,i));
 memcpy(before,save.questData,6);
 for(i=0;i<QUEST_COUNT;i++) LogbookMenu_GetSetQuestState(i,FLAG_GET_COMPLETED);
 assert(!memcmp(before,save.questData,6)); // reads never corrupt old state/favorites
 reset();flags[FLAG_OAK_ACKNOWLEDGED_NATURE]=1;
 assert(LogbookMenu_GetSetQuestState(6,FLAG_GET_ACTIVE));
 ready=1;assert(LogbookMenu_GetSetQuestState(6,FLAG_GET_REWARD));assert(!LogbookMenu_GetSetQuestState(6,FLAG_GET_COMPLETED));
 flags[FLAG_OAK_EXPEDITION_COMPLETE]=1;
 assert(LogbookMenu_GetSetQuestState(6,FLAG_GET_COMPLETED));assert(!LogbookMenu_GetSetQuestState(6,FLAG_GET_REWARD));
 reset();
 for(i=0;i<NELEMS(sDesignMilestones);i++) RecordNatureResearchEvolution(sDesignMilestones[i].before,sDesignMilestones[i].after,TRUE);
 for(i=0;i<NELEMS(sRevivalMilestones);i++) flags[sRevivalMilestones[i]]=1;
 flags[FLAG_GOT_PORYGON_FROM_BLAINE]=1;
 assert(CountEvolutionThroughDesignMilestones()==13);
 assert(TryGenerateNatureSubquestCounter(SUB_QUEST_EVOLUTION_THROUGH_DESIGN,&color));
 assert(!strcmp((char *)gStringVar4,"13/2") && color==2);
 for(i=0;i<NELEMS(sBondMilestones);i++) {
   RecordNatureResearchEvolution(sBondMilestones[i].before,sBondMilestones[i].after,sBondMilestones[i].trade);
   if(sBondMilestones[i].legacy) flags[sBondMilestones[i].legacy]=1;
 }
 assert(CountEvolutionThroughBondMilestones()==33); // legacy flags and hooks count once
 assert(TryGenerateNatureSubquestCounter(SUB_QUEST_EVOLUTION_THROUGH_BOND,&color));
 assert(!strcmp((char *)gStringVar4,"33/2"));
 flags[FLAG_INTERACTED_APEX_TANGROWTH]=flags[FLAG_INTERACTED_APEX_ZAPDOS]=flags[FLAG_INTERACTED_APEX_ARTICUNO]=flags[FLAG_INTERACTED_APEX_MEWTWO]=flags[FLAG_INTERACTED_APEX_OSSCYTHE]=flags[FLAG_INTERACTED_APEX_MOLTRES]=flags[FLAG_INTERACTED_APEX_MIME_SR]=flags[FLAG_INTERACTED_APEX_ANNIHILAPE]=1;
 assert(TryGenerateNatureSubquestCounter(SUB_QUEST_EVOLUTION_THROUGH_INSTINCT,&color));
 assert(!strcmp((char *)gStringVar4,"8/2"));
 reset();
 assert(!LogbookMenu_GetSetQuestState(QUEST_LIVING_RELICS,FLAG_GET_UNLOCKED));
 seen[SPECIES_RELICANTH]=1;
 assert(LogbookMenu_GetSetQuestState(QUEST_LIVING_RELICS,FLAG_GET_ACTIVE));
 assert(LogbookMenu_GetSetSubquestState(QUEST_LIVING_RELICS,FLAG_GET_UNLOCKED,2));
 assert(!LogbookMenu_GetSetSubquestState(QUEST_LIVING_RELICS,FLAG_GET_UNLOCKED,0));
 for(i=0;i<NELEMS(sPrimevalSpecies);i++) caught[sPrimevalSpecies[i]]=1;
 assert(LogbookMenu_GetSetQuestState(QUEST_LIVING_RELICS,FLAG_GET_COMPLETED));
 assert(!LogbookMenu_GetSetQuestState(QUEST_LIVING_RELICS,FLAG_GET_ACTIVE));
 LogbookMenu_GetSetQuestState(QUEST_LIVING_RELICS,FLAG_SET_FAVORITE);
 assert(LogbookMenu_GetSetQuestState(QUEST_LIVING_RELICS,FLAG_GET_FAVORITE));
 assert(!LogbookMenu_GetSetSubquestState(QUEST_LIVING_RELICS,FLAG_GET_UNLOCKED,6));
 puts("PASS: save-compatible quest states, League records, discovered requests, reward retries, research classification and finale");
}
'''
with tempfile.TemporaryDirectory() as tmp:
    path=Path(tmp)/'logbook.c';path.write_text(code);exe=path.with_suffix('')
    subprocess.run(['cc','-I',str(ROOT/'include'),str(path),'-o',str(exe)],check=True)
    subprocess.run([str(exe)],check=True)
