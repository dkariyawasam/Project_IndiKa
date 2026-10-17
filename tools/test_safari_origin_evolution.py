#!/usr/bin/env python3
"""Execute the production item-evolution branch and legacy-form conversion."""
from pathlib import Path
import subprocess,tempfile
R=Path(__file__).resolve().parents[1]
s=(R/'src/pokemon.c').read_text()
a=s.index('    case EVO_MODE_ITEM_USE:',s.index('u16 GetEvolutionTargetSpecies('));b=s.index('        break;\n    }',a)
branch=s[a:b]
a=s.index('static bool8 NormalizeSafariPreEvolution(');b=s.index('\nvoid MigrateSafariPreEvolutions',a)
normalize=s[a:b]
a=s.index('static bool8 HasSafariEvolutionOrigin(');b=s.index('\nu16 GetEvolutionTargetSpecies',a)
origin=s[a:b]
a=s.index('            case EVO_LEVEL_NON_SAFARI:');b=s.index('            case EVO_LEVEL_NIGHT:',a)
levels=s[a:b]
a=s.index('            case EVO_LEVEL_MOVE:');b=s.index('            case EVO_LEVEL_ATK_GT_DEF:',a)
levels+=s[a:b]
code=r'''
#include <assert.h>
#include <string.h>
#include <stddef.h>
#include "constants/species.h"
#include "constants/pokemon.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/region_map_sections.h"
typedef unsigned char u8; typedef unsigned short u16; typedef int bool8;
#define TRUE 1
#define FALSE 0
#define POKEMON_NAME_LENGTH 10
#define MAX_MON_MOVES 4
struct BoxPokemon {u16 species;u8 origin;char name[POKEMON_NAME_LENGTH+1];};
struct Pokemon {struct BoxPokemon box;};
struct Evolution {u16 method,param,targetSpecies;};
#include "data/pokemon/evolution.h"
char gSpeciesNames[NUM_SPECIES][POKEMON_NAME_LENGTH+1];
int metLevel=5,night=0;
u16 moves[4]={0};
int IsNightTime(void){return night;}
int GetBoxMonData(struct BoxPokemon *m,int f,void *out) {
 if(f==MON_DATA_MET_LEVEL)return metLevel;
 if(f==MON_DATA_NICKNAME){strcpy(out,m->name);return 0;}
 return f==MON_DATA_SPECIES?m->species:m->origin;
}
void SetBoxMonData(struct BoxPokemon *m,int f,const void *v) {
 if(f==MON_DATA_SPECIES)m->species=*(const u16*)v;
 else if(f==MON_DATA_MET_LOCATION)m->origin=*(const u8*)v;
 else if(f==MON_DATA_MET_LEVEL)metLevel=*(const u8*)v;
 else strcpy(m->name,v);
}
int GetMonData(struct Pokemon *m,int f,void *out){if(f>=MON_DATA_MOVE1 && f<=MON_DATA_MOVE4)return moves[f-MON_DATA_MOVE1];if(f==MON_DATA_MET_LEVEL)return metLevel;return GetBoxMonData(&m->box,f,out);}
int GetMonGender(struct Pokemon *m){return MON_MALE;}
#define StringCompare(a,b) strcmp((const char *)(a),(const char *)(b))
'''+origin+normalize+r'''
u16 evolve(struct Pokemon *mon,int type,int evolutionItem) {
 int i;u16 species=mon->box.species,targetSpecies=0;
 switch(type){
'''+branch+r'''
 break;
 }
 return targetSpecies;
}
u16 levelEvolve(struct Pokemon *mon,int level){
 int i;u16 species=mon->box.species,targetSpecies=0;
 for(i=0;i<EVOS_PER_MON;i++)switch(gEvolutionTable[species][i].method){
'''+levels+r'''
 }
 return targetSpecies;
}
int main(void){
 struct Pokemon m={{SPECIES_PIKACHU,MAPSEC_KANTO_SAFARI_ZONE,"SPARK"}};
 for(int mode=EVO_MODE_ITEM_USE;mode<=EVO_MODE_ITEM_CHECK;mode++){
  m.box.origin=MAPSEC_KANTO_SAFARI_ZONE;
  assert(evolve(&m,mode,ITEM_THUNDER_STONE)==SPECIES_RAICHU_ALOLAN);
  assert(evolve(&m,mode,ITEM_FIRE_STONE)==SPECIES_NONE);
  m.box.origin=MAPSEC_PALLET_TOWN;
  assert(evolve(&m,mode,ITEM_THUNDER_STONE)==SPECIES_RAICHU);
 }
 m.box.origin=MAPSEC_KANTO_SAFARI_ZONE;metLevel=0;
 assert(evolve(&m,EVO_MODE_ITEM_USE,ITEM_THUNDER_STONE)==SPECIES_RAICHU);
 metLevel=5;
 m.box.species=SPECIES_PICHU;m.box.origin=MAPSEC_KANTO_SAFARI_ZONE;
 assert(evolve(&m,EVO_MODE_ITEM_USE,ITEM_THUNDER_STONE)==SPECIES_NONE);
 assert(gEvolutionTable[SPECIES_PICHU][0].method==EVO_FRIENDSHIP);
 assert(gEvolutionTable[SPECIES_PICHU][0].targetSpecies==SPECIES_PIKACHU);
 m.box.species=SPECIES_EXEGGCUTE;
 for(int mode=EVO_MODE_ITEM_USE;mode<=EVO_MODE_ITEM_CHECK;mode++){
  m.box.origin=MAPSEC_KANTO_SAFARI_ZONE;
  assert(evolve(&m,mode,ITEM_LEAF_STONE)==SPECIES_EXEGGUTOR_ALOLAN);
  m.box.origin=MAPSEC_PALLET_TOWN;
  assert(evolve(&m,mode,ITEM_LEAF_STONE)==SPECIES_EXEGGUTOR);
 }
 m.box.species=SPECIES_CUBONE;m.box.origin=MAPSEC_KANTO_SAFARI_ZONE;
 night=0;assert(levelEvolve(&m,28)==SPECIES_NONE);
 night=1;assert(levelEvolve(&m,27)==SPECIES_NONE);
 assert(levelEvolve(&m,28)==SPECIES_MAROWAK_ALOLAN);
 assert(levelEvolve(&m,50)==SPECIES_MAROWAK_ALOLAN);
 m.box.origin=MAPSEC_PALLET_TOWN;
 assert(levelEvolve(&m,28)==SPECIES_MAROWAK);
 night=0;assert(levelEvolve(&m,28)==SPECIES_MAROWAK);
 m.box.origin=MAPSEC_KANTO_SAFARI_ZONE;metLevel=0;
 assert(levelEvolve(&m,28)==SPECIES_MAROWAK);
 metLevel=5;m.box.species=SPECIES_KOFFING;
 assert(levelEvolve(&m,34)==SPECIES_NONE);
 assert(levelEvolve(&m,35)==SPECIES_WEEZING_GALARIAN);
 m.box.origin=MAPSEC_PALLET_TOWN;
 assert(levelEvolve(&m,35)==SPECIES_WEEZING);
 m.box.species=SPECIES_MIME_JR;m.box.origin=MAPSEC_KANTO_SAFARI_ZONE;
 assert(levelEvolve(&m,20)==SPECIES_NONE);
 moves[2]=MOVE_MIMIC;
 assert(levelEvolve(&m,20)==SPECIES_MR_MIME_GALARIAN);
 m.box.origin=MAPSEC_PALLET_TOWN;
 assert(levelEvolve(&m,20)==SPECIES_MR_MIME);
 m.box.origin=MAPSEC_KANTO_SAFARI_ZONE;metLevel=0;
 assert(levelEvolve(&m,20)==SPECIES_MR_MIME);
 moves[2]=MOVE_NONE;assert(levelEvolve(&m,20)==SPECIES_NONE);metLevel=5;
 const u16 old[]={SPECIES_PICHU_ALOLAN,SPECIES_PIKACHU_ALOLAN,SPECIES_EXEGGCUTE_ALOLAN,SPECIES_CUBONE_ALOLAN,SPECIES_KOFFING_GALARIAN};
 const u16 replacement[]={SPECIES_PICHU,SPECIES_PIKACHU,SPECIES_EXEGGCUTE,SPECIES_CUBONE,SPECIES_KOFFING};
 for(int i=0;i<5;i++){
  m.box.species=old[i];
  m.box.origin=MAPSEC_PALLET_TOWN;
  assert(NormalizeSafariPreEvolution(&m.box));
  assert(m.box.species==replacement[i]);
  assert(m.box.origin==MAPSEC_KANTO_SAFARI_ZONE);
  assert(!strcmp(m.box.name,"SPARK"));
  assert(!NormalizeSafariPreEvolution(&m.box));
 }
 return 0;
}
'''
with tempfile.TemporaryDirectory() as tmp:
 p=Path(tmp);(p/'test.c').write_text(code)
 subprocess.run(['cc','-I'+str(R/'include'),'-I'+str(R/'src'),str(p/'test.c'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
print('PASS: Safari origin branches, item checks, level/night gates, hatch exclusion and five legacy conversions')
