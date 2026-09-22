#!/usr/bin/env python3
"""Exercise the actual evolution function and table with host-side Pokémon data."""
from pathlib import Path
import re,subprocess,tempfile
root=Path(__file__).resolve().parents[1]
s=(root/'src/pokemon.c').read_text();start=s.index('u16 GetEvolutionTargetSpecies(');end=s.index('\n}',start)+2
helper_start=s.index('static bool8 CanEvolveIntoApexInHabitat(')
fn=s[helper_start:s.index('\n}',helper_start)+2]+'\n'+s[start:end]
code=r'''
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <stdio.h>
typedef uint8_t bool8;
#define TRUE 1
#define FALSE 0
#define MAX_MON_MOVES 4
typedef uint8_t u8; typedef uint16_t u16; typedef uint32_t u32;
#include "include/constants/pokemon.h"
#include "include/constants/species.h"
#include "include/constants/items.h"
#include "include/constants/hold_effects.h"
#include "include/constants/moves.h"
#include "include/constants/flags.h"
#include "include/constants/region_map_sections.h"
static unsigned char flags[4096];
static int FlagGet(int flag){return flags[flag];}
static struct {u8 regionMapSectionId;} gMapHeader;
struct Evolution {u16 method,param,targetSpecies;};
#include "src/data/pokemon/evolution.h"
#define MAX_MON_MOVES 4
struct Pokemon {u32 data[128];};
static u32 GetMonData(struct Pokemon *m,int field,void *unused){return m->data[field];}
static void SetMonData(struct Pokemon *m,int field,const u16 *v){m->data[field]=*v;}
static u8 ItemId_GetHoldEffect(u16 item){return item==ITEM_EVERSTONE?HOLD_EFFECT_PREVENT_EVOLVE:0;}
static struct {struct {u8 holdEffect;} enigmaBerry;} save;
static __typeof__(save) *gSaveBlock1Ptr=&save;
static int daytime=1;
static int IsDayTime(void){return daytime;}
static int IsNightTime(void){return !daytime;}
static int IsNationalPokedexEnabled(void){return 1;}
static int gender=MON_FEMALE;
static int GetMonGender(struct Pokemon *m){return gender;}
static struct {u8 type;} gBattleMoves[MOVES_COUNT];
'''+fn+r'''
int main(void){
 struct Pokemon m={0}; m.data[MON_DATA_SPECIES]=SPECIES_EEVEE;m.data[MON_DATA_LEVEL]=20;
 gBattleMoves[MOVE_BABY_DOLL_EYES].type=TYPE_FAIRY;
 gBattleMoves[MOVE_CHARM].type=TYPE_FAIRY;
 for(int day=0;day<=1;day++){
  daytime=day;
  for(int slot=0;slot<4;slot++){
   for(int i=0;i<4;i++)m.data[MON_DATA_MOVE1+i]=MOVE_NONE;
   m.data[MON_DATA_MOVE1+slot]=MOVE_BABY_DOLL_EYES;
   m.data[MON_DATA_FRIENDSHIP]=219;
   assert(GetEvolutionTargetSpecies(&m,EVO_MODE_NORMAL,0)==0);
   m.data[MON_DATA_FRIENDSHIP]=220;
   assert(GetEvolutionTargetSpecies(&m,EVO_MODE_NORMAL,0)==SPECIES_SYLVEON);
   m.data[MON_DATA_MOVE1+slot]=MOVE_CHARM;
   assert(GetEvolutionTargetSpecies(&m,EVO_MODE_NORMAL,0)==SPECIES_SYLVEON);
  }
  for(int i=0;i<4;i++)m.data[MON_DATA_MOVE1+i]=MOVE_NONE;
  assert(GetEvolutionTargetSpecies(&m,EVO_MODE_NORMAL,0)==(day?SPECIES_ESPEON:SPECIES_UMBREON));
 }
 m.data[MON_DATA_MOVE1]=MOVE_CHARM;
 const u16 stones[]={ITEM_WATER_STONE,ITEM_THUNDER_STONE,ITEM_FIRE_STONE,ITEM_LEAF_STONE,ITEM_ICE_STONE};
 const u16 targets[]={SPECIES_VAPOREON,SPECIES_JOLTEON,SPECIES_FLAREON,SPECIES_LEAFEON,SPECIES_GLACEON};
 for(int i=0;i<5;i++)assert(GetEvolutionTargetSpecies(&m,EVO_MODE_ITEM_USE,stones[i])==targets[i]);
 assert(GetEvolutionTargetSpecies(&m,EVO_MODE_TRADE,0)==0);
 m.data[MON_DATA_HELD_ITEM]=ITEM_EVERSTONE;
 assert(GetEvolutionTargetSpecies(&m,EVO_MODE_NORMAL,0)==0);
 const u16 bases[]={SPECIES_TANGELA,SPECIES_PRIMEAPE,SPECIES_MR_MIME,SPECIES_MAROWAK};
 const u16 apex[]={SPECIES_TANGROWTH,SPECIES_ANNIHILAPE,SPECIES_MIME_SR,SPECIES_OSSCYTHE};
 const u16 discovery[]={FLAG_INTERACTED_APEX_TANGROWTH,FLAG_INTERACTED_APEX_ANNIHILAPE,FLAG_INTERACTED_APEX_MIME_SR,FLAG_INTERACTED_APEX_OSSCYTHE};
 const u8 habitats[]={MAPSEC_VIRIDIAN_FOREST,MAPSEC_MT_MOON,MAPSEC_DIGLETTS_CAVE,MAPSEC_POKEMON_TOWER};
 for(int n=0;n<4;n++){
  m.data[MON_DATA_SPECIES]=bases[n];m.data[MON_DATA_HELD_ITEM]=ITEM_NONE;
  u16 required=0;
  for(int e=0;e<EVOS_PER_MON;e++)if(gEvolutionTable[bases[n]][e].method==EVO_APEX_MOVE)required=gEvolutionTable[bases[n]][e].param;
  assert(required);
  for(int slot=0;slot<4;slot++){
   for(int j=0;j<4;j++)m.data[MON_DATA_MOVE1+j]=MOVE_NONE;
   m.data[MON_DATA_MOVE1+slot]=required;gMapHeader.regionMapSectionId=habitats[n];
   flags[discovery[n]]=0;assert(GetEvolutionTargetSpecies(&m,EVO_MODE_NORMAL,0)==0);
   flags[discovery[n]]=1;gMapHeader.regionMapSectionId=MAPSEC_PALLET_TOWN;
   assert(GetEvolutionTargetSpecies(&m,EVO_MODE_NORMAL,0)==0);
   gMapHeader.regionMapSectionId=habitats[n];assert(GetEvolutionTargetSpecies(&m,EVO_MODE_NORMAL,0)==apex[n]);
   m.data[MON_DATA_HELD_ITEM]=ITEM_EVERSTONE;assert(GetEvolutionTargetSpecies(&m,EVO_MODE_NORMAL,0)==0);m.data[MON_DATA_HELD_ITEM]=ITEM_NONE;
   m.data[MON_DATA_MOVE1+slot]=MOVE_NONE;assert(GetEvolutionTargetSpecies(&m,EVO_MODE_NORMAL,0)==0);
  }
 }
 for(int n=0;n<2;n++){
  m.data[MON_DATA_SPECIES]=n?SPECIES_LICKITUNG:SPECIES_AIPOM;
  m.data[MON_DATA_HELD_ITEM]=ITEM_NONE;
  for(int f=0;f<4096;f++)flags[f]=0;
  for(int slot=0;slot<4;slot++){
   for(int j=0;j<4;j++)m.data[MON_DATA_MOVE1+j]=MOVE_NONE;
   m.data[MON_DATA_MOVE1+slot]=n?MOVE_ROLLOUT:MOVE_DOUBLE_HIT;
   gMapHeader.regionMapSectionId=n?MAPSEC_CERULEAN_CAVE:MAPSEC_FUCHSIA_FOREST;
   if(n){
    flags[FLAG_DEFEATED_APEX_MEWTWO]=0;
    flags[FLAG_FOUGHT_MEWTWO]=1;
    flags[FLAG_INTERACTED_APEX_MEWTWO]=1;
    assert(GetEvolutionTargetSpecies(&m,EVO_MODE_NORMAL,0)==0);
    flags[FLAG_DEFEATED_APEX_MEWTWO]=1;
   }
   assert(GetEvolutionTargetSpecies(&m,EVO_MODE_NORMAL,0)==(n?SPECIES_LICKILICKY:SPECIES_AMBIPOM));
   gMapHeader.regionMapSectionId=MAPSEC_CELADON_CAVE;
   assert(GetEvolutionTargetSpecies(&m,EVO_MODE_NORMAL,0)==0);
  }
 }
 m.data[MON_DATA_SPECIES]=SPECIES_PILOSWINE;m.data[MON_DATA_MOVE1]=MOVE_ANCIENT_POWER;
 for(int map=0;map<MAPSEC_NONE;map++){
  gMapHeader.regionMapSectionId=map;
  assert(GetEvolutionTargetSpecies(&m,EVO_MODE_NORMAL,0)==0);
 }

 const u16 levelBases[]={SPECIES_TYNAMO,SPECIES_DIGLETT_ALOLAN,SPECIES_MEOWTH_GALARIAN,SPECIES_GEODUDE_ALOLAN,SPECIES_GRIMER_ALOLAN,SPECIES_CORSOLA_GALARIAN,SPECIES_KOFFING_GALARIAN};
 const u16 levelTargets[]={SPECIES_EELEKTRIK,SPECIES_DUGTRIO_ALOLAN,SPECIES_PERRSERKER,SPECIES_GRAVELER_ALOLAN,SPECIES_MUK_ALOLAN,SPECIES_CURSOLA,SPECIES_WEEZING_GALARIAN};
 const u16 levels[]={39,26,28,25,38,38,35};
 for(int i=0;i<7;i++){
  m=(struct Pokemon){0};m.data[MON_DATA_SPECIES]=levelBases[i];
  m.data[MON_DATA_LEVEL]=levels[i]-1;assert(!GetEvolutionTargetSpecies(&m,EVO_MODE_NORMAL,0));
  m.data[MON_DATA_LEVEL]=levels[i];assert(GetEvolutionTargetSpecies(&m,EVO_MODE_NORMAL,0)==levelTargets[i]);
  m.data[MON_DATA_HELD_ITEM]=ITEM_EVERSTONE;assert(!GetEvolutionTargetSpecies(&m,EVO_MODE_NORMAL,0));
 }
 for(int i=0;i<2;i++){
  m=(struct Pokemon){0};m.data[MON_DATA_SPECIES]=i?SPECIES_CUBONE_ALOLAN:SPECIES_RATTATA_ALOLAN;
  int threshold=i?28:20;u16 target=i?SPECIES_MAROWAK_ALOLAN:SPECIES_RATICATE_ALOLAN;
  for(int day=0;day<2;day++)for(int level=threshold-1;level<=threshold+1;level++){
   daytime=day;m.data[MON_DATA_LEVEL]=level;
   assert(GetEvolutionTargetSpecies(&m,EVO_MODE_NORMAL,0)==(!day && level>=threshold?target:0));
  }
 }
 m=(struct Pokemon){0};m.data[MON_DATA_SPECIES]=SPECIES_COMBEE;
 for(int sex=0;sex<3;sex++)for(int level=20;level<=22;level++){
  gender=sex==0?MON_FEMALE:sex==1?MON_MALE:MON_GENDERLESS;m.data[MON_DATA_LEVEL]=level;
  assert(GetEvolutionTargetSpecies(&m,EVO_MODE_NORMAL,0)==(sex==0 && level>=21?SPECIES_VESPIQUEN:0));
 }
 m=(struct Pokemon){0};m.data[MON_DATA_SPECIES]=SPECIES_MIME_JR;
 assert(!GetEvolutionTargetSpecies(&m,EVO_MODE_NORMAL,0));
 for(int slot=0;slot<4;slot++){
  m.data[MON_DATA_MOVE1+slot]=MOVE_MIMIC;assert(GetEvolutionTargetSpecies(&m,EVO_MODE_NORMAL,0)==SPECIES_MR_MIME);
  m.data[MON_DATA_HELD_ITEM]=ITEM_EVERSTONE;assert(!GetEvolutionTargetSpecies(&m,EVO_MODE_NORMAL,0));
  m.data[MON_DATA_HELD_ITEM]=0;m.data[MON_DATA_MOVE1+slot]=0;
 }
 m=(struct Pokemon){0};m.data[MON_DATA_SPECIES]=SPECIES_EELEKTRIK;
 assert(!GetEvolutionTargetSpecies(&m,EVO_MODE_ITEM_USE,ITEM_FIRE_STONE));
 assert(GetEvolutionTargetSpecies(&m,EVO_MODE_ITEM_USE,ITEM_THUNDER_STONE)==SPECIES_EELEKTROSS);
 for(int i=0;i<2;i++){
  m=(struct Pokemon){0};m.data[MON_DATA_SPECIES]=i?SPECIES_MEOWTH_ALOLAN:SPECIES_PICHU_ALOLAN;
  m.data[MON_DATA_FRIENDSHIP]=219;assert(!GetEvolutionTargetSpecies(&m,EVO_MODE_NORMAL,0));
  m.data[MON_DATA_FRIENDSHIP]=220;assert(GetEvolutionTargetSpecies(&m,EVO_MODE_NORMAL,0)==(i?SPECIES_PERSIAN_ALOLAN:SPECIES_PIKACHU_ALOLAN));
 }
 m=(struct Pokemon){0};m.data[MON_DATA_SPECIES]=SPECIES_GRAVELER_ALOLAN;
 assert(!GetEvolutionTargetSpecies(&m,EVO_MODE_NORMAL,0));
 assert(GetEvolutionTargetSpecies(&m,EVO_MODE_TRADE,0)==SPECIES_GOLEM_ALOLAN);
 puts("PASS: 15 restored evolution links; level, gender, night, friendship, move, item, trade and Everstone checks.");
 puts("PASS: Ambipom/Fuchsia Forest and Lickilicky/Cerulean Cave after Mewtwo victory; no Piloswine evolution.");
 puts("PASS: four Apex species, discovery, habitat, known move in every slot, and Everstone.");
 puts("PASS: Sylveon friendship threshold, all move slots, day/night priority, other seven evolutions and Everstone.");
}
'''
learn=(root/'src/data/pokemon/level_up_learnsets.h').read_text().split('sEeveeLevelUpLearnset[] = {')[1].split('};')[0]
assert 'LEVEL_UP_MOVE(9, MOVE_BABY_DOLL_EYES)' in learn
battle=(root/'src/data/battle_moves.h').read_text().split('[MOVE_BABY_DOLL_EYES] =')[1].split('},')[0]
assert '.type = TYPE_FAIRY' in battle
with tempfile.TemporaryDirectory(prefix='eevee-test-') as tmp:
 p=Path(tmp);(p/'test.c').write_text(code)
 subprocess.run(['cc','-std=gnu99','-I',str(root),'-I',str(root/'include'),str(p/'test.c'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
