#!/usr/bin/env python3
"""Exercise the production expedition gate and ordered Oak research reactions."""
from pathlib import Path
import subprocess
import tempfile
import re

ROOT = Path(__file__).resolve().parents[1]
source = (ROOT / 'src/quests.c').read_text()


def function(name):
    start = re.search(r'(?:bool8|u8|u16) ' + name + r'\(void\)\n\{', source).start()
    return source[start:source.index('\n}', start) + 2]


code = r'''
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "constants/flags.h"
#include "constants/vars.h"
#include "constants/quests.h"
#include "constants/species.h"
typedef uint8_t u8;
typedef uint16_t u16;
typedef u8 bool8;
#define TRUE 1
#define FALSE 0
#define NELEMS(a) (sizeof(a)/sizeof((a)[0]))
static u8 flags[65536], completed[8], traded[8];
static u16 vars[65536];
static int instinct, design;
static u8 caught[NUM_SPECIES];
static bool8 IsSpeciesCaught(u16 n) {return caught[n];}
static bool8 FlagGet(u16 n) {return flags[n];}
static u16 VarGet(u16 n) {return vars[n];}
static bool8 IsGymTrialCompleted(u8 n) {return completed[n];}
static bool8 IsGymTrialTraded(u8 n) {return traded[n];}
static int CountApexInteractionsForNatureQuest(void) {return instinct;}
static int CountEvolutionThroughDesignMilestones(void) {return design;}
'''
code += function('CountEvolutionThroughBondMilestones') + '\n'
code += function('GetOakResearchReaction') + '\n' + function('IsOakExpeditionReady')
code += r'''
int main(void) {
 const u16 gates[] = {FLAG_SYS_GAME_CLEAR,
   FLAG_DEFEATED_LEADER_GIOVANNI, FLAG_SAW_GIOVANNI_POKEMON_MANSION,
   FLAG_OAK_ACKNOWLEDGED_NATURE};
 const u16 scenes[] = {VAR_ROUTE3_RIVAL_CONVERSATION, VAR_MAP_SCENE_CELADON_RIVAL,
   VAR_ROUTE23_RIVAL_CONVERSATION, VAR_MAP_SCENE_CINNABAR_RIVAL,
   VAR_MAP_SCENE_POKEMON_MANSION_3F_RIVAL, VAR_MAP_SCENE_SILPH_CO_7F,
   VAR_MAP_SCENE_ROUTE11_RIVAL, VAR_MAP_SCENE_POKEMON_TOWER_1F, VAR_FOREST_RIVAL_APEX};
 int i;
 assert(!IsOakExpeditionReady());
 instinct=design=2;
 caught[SPECIES_CROBAT]=caught[SPECIES_CHIMECHO]=1;
 assert(!GetOakResearchReaction());
 flags[FLAG_SYS_POKEDEX_GET]=1;
 assert(GetOakResearchReaction()==1); flags[FLAG_OAK_ACKNOWLEDGED_BOND]=1;
 assert(GetOakResearchReaction()==2); flags[FLAG_OAK_ACKNOWLEDGED_INSTINCT]=1;
 assert(GetOakResearchReaction()==3); flags[FLAG_OAK_ACKNOWLEDGED_DESIGN]=1;
 assert(GetOakResearchReaction()==4); flags[FLAG_OAK_ACKNOWLEDGED_NATURE]=1;
 assert(GetOakResearchReaction()==0);
 for(i=0;i<NELEMS(gates);i++) flags[gates[i]]=1;
 for(i=0;i<NELEMS(scenes);i++) vars[scenes[i]]=1;
 memset(completed,1,sizeof(completed));
 instinct=7; assert(!IsOakExpeditionReady());
 instinct=8; assert(IsOakExpeditionReady());
 // Hall of Fame has already cleared this transient victory-scene flag.
 assert(!flags[FLAG_DEFEATED_CHAMP]);
 assert(IsOakExpeditionReady());
 for(i=0;i<NELEMS(gates);i++) {
   flags[gates[i]]=0; assert(!IsOakExpeditionReady()); flags[gates[i]]=1;
 }
 for(i=0;i<NELEMS(scenes);i++) {
   vars[scenes[i]]=0; assert(!IsOakExpeditionReady()); vars[scenes[i]]=1;
 }
 for(i=0;i<8;i++) {
   completed[i]=0; assert(!IsOakExpeditionReady());
   traded[i]=1; assert(IsOakExpeditionReady());
 }
 // Milotic can supply the second Bond milestone, but Feebas alone cannot.
 memset(flags,0,sizeof(flags)); memset(caught,0,sizeof(caught));
 flags[FLAG_SYS_POKEDEX_GET]=1; instinct=design=0;
 caught[SPECIES_CROBAT]=caught[SPECIES_FEEBAS]=1;
 assert(CountEvolutionThroughBondMilestones()==1);
 assert(GetOakResearchReaction()==0);
 caught[SPECIES_MILOTIC]=1;
 assert(CountEvolutionThroughBondMilestones()==2);
 assert(GetOakResearchReaction()==1);
 flags[FLAG_OAK_ACKNOWLEDGED_BOND]=1;
 assert(GetOakResearchReaction()==0);
 puts("PASS: ordered research reactions, expedition prerequisites, Milotic Bond credit, and no duplicate Bond reaction");
}
'''
with tempfile.TemporaryDirectory() as tmp:
    path = Path(tmp) / 'oak.c'
    path.write_text(code)
    exe = path.with_suffix('')
    subprocess.run(['cc', '-I', str(ROOT / 'include'), str(path), '-o', str(exe)], check=True)
    subprocess.run([str(exe)], check=True)
