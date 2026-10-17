#!/usr/bin/env python3
"""Host checks for new regional evolutions and battle-only critical-hit tracking."""
from pathlib import Path
import re,subprocess,tempfile,json
R=Path(__file__).resolve().parents[1]
def read(p):return (R/p).read_text()
def section(s,start,end):a=s.index(start);return s[a:s.index(end,a)]
record=section(read('src/battle_main.c'),'void RecordCriticalHitEvolution(void)\n{','static void FreeResetData_ReturnToOvOrDoEvolutions(void)\n{')
evolve=section(read('src/pokemon.c'),'    case EVO_MODE_BATTLE_END:', '    case EVO_MODE_TRADE:')
steady=section(read('src/battle_util.c'),'                if (gBattleMons[gBattlerAttacker].ability == ABILITY_STEADFAST','                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;')
code=r'''
#include <assert.h>
#include <string.h>
#include "constants/species.h"
#include "constants/pokemon.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/abilities.h"
#include "constants/battle.h"
typedef unsigned char u8;typedef unsigned short u16;
#define PARTY_SIZE 6
struct Pokemon {u16 species;} gPlayerParty[6];
struct {u16 species;int ability;int statStages[8];} gBattleMons[4];
struct {int animArg1,animArg2,battler;} gBattleScripting;
u8 gCriticalHitsInBattle[6],gBattlerAttacker,gBattlerTarget=1,gBattlerPartyIndexes[4];
int gBattleTypeFlags,gLastUsedAbility,gBattlescriptCurrInstr,recorded;
#define BattleScript_MoveUsedFlinchedSteadfast 17
int GetMonData(struct Pokemon *mon,int field){return mon->species;}
int GetBattlerSide(int battler){return battler&1;}
void RecordAbilityBattle(int battler,int ability){recorded++;}
struct Evolution {u16 method,param,targetSpecies;};
#include "data/pokemon/evolution.h"
'''+record+r'''
int evolve(struct Pokemon *mon){int i;u16 species=mon->species,targetSpecies=0;switch(EVO_MODE_BATTLE_END){
'''+evolve+r'''
}return targetSpecies;}
void flinch(void){
'''+steady+r'''
}
int main(void){
 gPlayerParty[0].species=SPECIES_FARFETCHD_GALARIAN;
 gBattleMons[0].species=SPECIES_FARFETCHD_GALARIAN;
 RecordCriticalHitEvolution();RecordCriticalHitEvolution();
 assert(gCriticalHitsInBattle[0]==2 && evolve(&gPlayerParty[0])==SPECIES_NONE);
 RecordCriticalHitEvolution();RecordCriticalHitEvolution();
 assert(gCriticalHitsInBattle[0]==3 && evolve(&gPlayerParty[0])==SPECIES_SIRFETCHD);
 gBattlerPartyIndexes[0]=1;gPlayerParty[1].species=SPECIES_FARFETCHD_GALARIAN;
 RecordCriticalHitEvolution();assert(gCriticalHitsInBattle[1]==1 && gCriticalHitsInBattle[0]==3);
 gBattlerTarget=2;RecordCriticalHitEvolution();assert(gCriticalHitsInBattle[1]==1);
 gBattlerTarget=1;gBattleTypeFlags=BATTLE_TYPE_LINK;
 RecordCriticalHitEvolution();assert(gCriticalHitsInBattle[1]==1);
 gBattleTypeFlags=0;gPlayerParty[1].species=SPECIES_DITTO;
 RecordCriticalHitEvolution();assert(gCriticalHitsInBattle[1]==1);
 memset(gCriticalHitsInBattle,0,sizeof(gCriticalHitsInBattle));
 assert(evolve(&gPlayerParty[0])==SPECIES_NONE);
 assert(gEvolutionTable[SPECIES_WOOPER_PALDEAN][0].param==20);
 assert(gEvolutionTable[SPECIES_WOOPER_PALDEAN][0].targetSpecies==SPECIES_CLODSIRE);
 assert(gEvolutionTable[SPECIES_WOOPER][0].targetSpecies==SPECIES_QUAGSIRE);
 gBattleMons[0].ability=ABILITY_STEADFAST;gBattleMons[0].statStages[STAT_SPEED]=6;
 flinch();assert(gBattleMons[0].statStages[STAT_SPEED]==7 && recorded==1);
 assert(gLastUsedAbility==ABILITY_STEADFAST && gBattlescriptCurrInstr==17);
 gBattleMons[0].statStages[STAT_SPEED]=MAX_STAT_STAGE;
 flinch();assert(gBattleMons[0].statStages[STAT_SPEED]==MAX_STAT_STAGE && recorded==1);
 gBattleMons[0].ability=ABILITY_NONE;gBattleMons[0].statStages[STAT_SPEED]=6;
 flinch();assert(gBattleMons[0].statStages[STAT_SPEED]==6);
 return 0;
}
'''
with tempfile.TemporaryDirectory() as tmp:
 p=Path(tmp);(p/'test.c').write_text(code)
 subprocess.run(['cc','-I'+str(R/'include'),'-I'+str(R/'src'),str(p/'test.c'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
# Verify battle setup clears progress, and the existing win/catch gate controls the evolution pass.
s=read('src/battle_main.c')
assert 'memset(gCriticalHitsInBattle, 0, sizeof(gCriticalHitsInBattle));' in s
end=section(s,'static void FreeResetData_ReturnToOvOrDoEvolutions(void)\n{','static void WaitForEvoSceneToFinish(void)\n{')
assert 'gLeveledUpInBattle |= gBitTable[i]' in end
assert 'gBattleOutcome != B_OUTCOME_WON && gBattleOutcome != B_OUTCOME_CAUGHT' in end
assert 'EVO_MODE_BATTLE_END' in end
for k in ['SIRFETCHD','WOOPER_PALDEAN','CLODSIRE']:
 if k != 'SIRFETCHD':
  assert f'[NATIONAL_DEX_{k}] = TRUE' in read('src/data/pokemon/obtainable_pokedex.h')
 for a in ['front.png','back.png','icon.png','footprint.png','normal.pal','shiny.pal']:
  assert (R/'graphics/pokemon'/k.lower()/a).is_file()
print('PASS: critical-hit thresholds, party slots, exclusions/reset, end-battle eligibility, Wooper branches, Steadfast and asset/obtainability coverage')

for k in ['FARFETCHD_GALARIAN','SIRFETCHD']:
 assert f'[NATIONAL_DEX_{k}] = TRUE' not in read('src/data/pokemon/obtainable_pokedex.h')
 assert f'SPECIES_{k}' not in read('src/data/wild_encounters.json')
