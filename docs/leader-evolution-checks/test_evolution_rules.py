"""Compile the production evolution selector with the six production table entries."""
from pathlib import Path
import re, subprocess, tempfile
ROOT = Path(__file__).resolve().parents[2]
source = (ROOT / 'src/pokemon.c').read_text()
function = source[source.index('u16 GetEvolutionTargetSpecies('):source.index('\nstatic u16 HoennPokedexNumToSpecies(')]
table = (ROOT / 'src/data/pokemon/evolution.h').read_text()
entries = []
for species in ['GRAVELER', 'POLIWHIRL', 'ELECTABUZZ', 'BUDEW', 'CHINGLING', 'GOLBAT']:
    entries.append(re.search(r'\[SPECIES_' + species + r'\]\s*=.*?(?=\n\s*\[|\n};)', table, re.S)[0])
c = r'''
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <stdio.h>
#include "constants/pokemon.h"
#include "constants/species.h"
#include "constants/items.h"
#include "constants/hold_effects.h"
typedef uint8_t u8; typedef uint16_t u16; typedef uint32_t u32;
struct Pokemon {u16 data[100];};
struct Evolution {u16 method,param,targetSpecies;};
static struct {struct {u8 holdEffect;} enigmaBerry;} save;
static typeof(save)* gSaveBlock1Ptr=&save;
static int day;
static int IsDayTime(void){return day;}
static int IsNightTime(void){return !day;}
static int IsNationalPokedexEnabled(void){return 1;}
static u32 GetMonData(struct Pokemon *m,int k,void *unused){return m->data[k];}
static void SetMonData(struct Pokemon *m,int k,const void *p){m->data[k]=*(const u16*)p;}
static u8 GetMonGender(struct Pokemon *m){return MON_FEMALE;}
static u8 ItemId_GetHoldEffect(u16 item){return item==ITEM_EVERSTONE?HOLD_EFFECT_PREVENT_EVOLVE:0;}
static const struct Evolution gEvolutionTable[NUM_SPECIES][EVOS_PER_MON]={
''' + '\n'.join(entries) + '\n};\n' + function + r'''
int main(void){
 unsigned checks=0;
 const u16 species[]={SPECIES_BUDEW,SPECIES_CHINGLING,SPECIES_GOLBAT};
 const u16 targets[]={SPECIES_ROSELIA,SPECIES_CHIMECHO,SPECIES_CROBAT};
 for(int i=0;i<3;i++)for(day=0;day<2;day++)for(int f=200;f<=220;f++){
  struct Pokemon m={0};m.data[MON_DATA_SPECIES]=species[i];m.data[MON_DATA_FRIENDSHIP]=f;m.data[MON_DATA_LEVEL]=31;
  int allowed=f>=220 && (i==2 || (i==0 && day) || (i==1 && !day));
  assert(GetEvolutionTargetSpecies(&m,EVO_MODE_NORMAL,0)==(allowed?targets[i]:0));checks++;
  m.data[MON_DATA_HELD_ITEM]=ITEM_EVERSTONE;
  assert(GetEvolutionTargetSpecies(&m,EVO_MODE_NORMAL,0)==0);checks++;
 }
 const u16 tradeSpecies[]={SPECIES_GRAVELER,SPECIES_POLIWHIRL,SPECIES_ELECTABUZZ};
 const u16 tradeTargets[]={SPECIES_GOLEM,SPECIES_POLITOED,SPECIES_ELECTIVIRE};
 const u16 items[]={0,ITEM_KINGS_ROCK,ITEM_ELECTIRIZER};
 for(int i=0;i<3;i++){
  struct Pokemon m={0};m.data[MON_DATA_SPECIES]=tradeSpecies[i];m.data[MON_DATA_HELD_ITEM]=items[i];
  assert(GetEvolutionTargetSpecies(&m,EVO_MODE_TRADE,0)==tradeTargets[i]);
  assert(m.data[MON_DATA_HELD_ITEM]==0);checks+=2;
  if(i){assert(GetEvolutionTargetSpecies(&m,EVO_MODE_TRADE,0)==0);checks++;}
 }
 printf("PASS: %u production evolution checks\n",checks);
}
'''
with tempfile.TemporaryDirectory() as tmp:
    p=Path(tmp)/'test.c';p.write_text(c)
    subprocess.run(['cc','-std=gnu11','-I',str(ROOT/'include'),str(p),'-o',str(Path(tmp)/'test')],check=True)
    subprocess.run([str(Path(tmp)/'test')],check=True)
