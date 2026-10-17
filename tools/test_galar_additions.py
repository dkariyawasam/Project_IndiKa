#!/usr/bin/env python3
"""Regression checks for the seven imported Galar species and save-safe IDs."""
from pathlib import Path
import re,subprocess,tempfile,json
R=Path(__file__).resolve().parents[1]
KEYS=['PONYTA_GALARIAN','RAPIDASH_GALARIAN','SLOWPOKE_GALARIAN','SLOWBRO_GALARIAN','SLOWKING_GALARIAN','MR_MIME_GALARIAN','MR_RIME']
def read(p):return (R/p).read_text()
# No released species or national dex ID may move.
old=subprocess.check_output(['git','show','HEAD:include/constants/species.h'],cwd=R,text=True)
oldkeys=dict(re.findall(r'^#define SPECIES_(\w+)\s+(\d+)',old,re.M));newkeys=dict(re.findall(r'^#define SPECIES_(\w+)\s+(\d+)',read('include/constants/species.h'),re.M))
for key,value in oldkeys.items():
 if key!='EGG':assert newkeys[key]==value,key
old_dex=subprocess.check_output(['git','show','HEAD:include/constants/pokedex.h'],cwd=R,text=True).split('};')[0]
old_dex=re.findall(r'\bNATIONAL_DEX_(\w+)',old_dex);new_dex=re.findall(r'\bNATIONAL_DEX_(\w+)',read('include/constants/pokedex.h').split('};')[0])
for key in old_dex:
 if key not in ('COUNT',) and not key.startswith('OLD_UNOWN_'):assert old_dex.index(key)==new_dex.index(key),key
assert re.search(r'#define DEX_FLAGS_NO 62\b',read('include/global.h'))
assert max(new_dex.index(k) for k in KEYS)<62*8
for k in KEYS:
 assert f'[NATIONAL_DEX_{k}] = TRUE' in read('src/data/pokemon/active_pokedex.h')
 assert f'[NATIONAL_DEX_{k}] = TRUE' in read('src/data/pokemon/obtainable_pokedex.h')
 assert f'SPECIES_{k}' in (read('src/data/pokemon/pokedex_categories.h') + read('src/data/pokemon/pokedex_forms.h'))
 folder=R/'graphics/pokemon'/k.lower()
 for asset in ['front.png','back.png','normal.pal','shiny.pal','icon.png','footprint.png']:assert (folder/asset).exists(),(k,asset)
# Compile the actual evolution table and verify distinct item branches.
code='''#include <assert.h>
#include "constants/species.h"
#include "constants/pokemon.h"
#include "constants/items.h"
#include "constants/moves.h"
typedef unsigned short u16;
struct Evolution { u16 method, param, targetSpecies; };
#include "data/pokemon/evolution.h"
int main(void) {
assert(gEvolutionTable[SPECIES_PONYTA_GALARIAN][0].method==EVO_LEVEL);
assert(gEvolutionTable[SPECIES_PONYTA_GALARIAN][0].param==40);
assert(gEvolutionTable[SPECIES_PONYTA_GALARIAN][0].targetSpecies==SPECIES_RAPIDASH_GALARIAN);
assert(gEvolutionTable[SPECIES_MR_MIME_GALARIAN][0].param==42);
assert(gEvolutionTable[SPECIES_MR_MIME_GALARIAN][0].targetSpecies==SPECIES_MR_RIME);
assert(gEvolutionTable[SPECIES_SLOWPOKE_GALARIAN][0].method==EVO_ITEM);
assert(gEvolutionTable[SPECIES_SLOWPOKE_GALARIAN][0].param==ITEM_GALARICA_CUFF);
assert(gEvolutionTable[SPECIES_SLOWPOKE_GALARIAN][0].targetSpecies==SPECIES_SLOWBRO_GALARIAN);
assert(gEvolutionTable[SPECIES_SLOWPOKE_GALARIAN][1].param==ITEM_GALARICA_WREATH);
assert(gEvolutionTable[SPECIES_SLOWPOKE_GALARIAN][1].targetSpecies==SPECIES_SLOWKING_GALARIAN);
}'''
with tempfile.TemporaryDirectory() as tmp:
 p=Path(tmp);(p/'test.c').write_text(code)
 subprocess.run(['cc','-I'+str(R/'include'),'-I'+str(R/'src'),str(p/'test.c'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
# Execute the actual weather-healing branch, including ability identity and minimum heal.
s=read('src/battle_util.c');branch=s[s.index('                case ABILITY_RAIN_DISH:'):s.index('                case ABILITY_SHED_SKIN:',s.index('                case ABILITY_RAIN_DISH:'))]
code='''#include <assert.h>
#include "constants/abilities.h"
#define B_WEATHER_RAIN 1
#define B_WEATHER_HAIL 2
#define WEATHER_HAS_EFFECT weatherWorks
#define BattleScript_RainDishActivates 1
int weatherWorks=1,gBattleWeather,gLastUsedAbility,gBattleMoveDamage,effect,calls;
struct {int maxHP,hp;} gBattleMons[1];
void BattleScriptPushCursorAndCallback(int script){calls++;}
void step(void){int battler=0;effect=calls=gBattleMoveDamage=0;switch(gLastUsedAbility){
'''+branch+'''}}
int main(void){
gBattleMons[0].maxHP=160;gBattleMons[0].hp=80;gLastUsedAbility=ABILITY_ICE_BODY;gBattleWeather=B_WEATHER_HAIL;step();assert(effect==1&&calls==1&&gBattleMoveDamage==-10&&gLastUsedAbility==ABILITY_ICE_BODY);
gBattleWeather=B_WEATHER_RAIN;step();assert(!effect&&!calls);
gLastUsedAbility=ABILITY_RAIN_DISH;step();assert(effect==1&&gBattleMoveDamage==-10);
weatherWorks=0;step();assert(!effect);weatherWorks=1;
gLastUsedAbility=ABILITY_ICE_BODY;gBattleWeather=B_WEATHER_HAIL;gBattleMons[0].hp=160;step();assert(!effect);
gBattleMons[0].maxHP=10;gBattleMons[0].hp=5;step();assert(gBattleMoveDamage==-1);
}'''
with tempfile.TemporaryDirectory() as tmp:
 p=Path(tmp);(p/'test.c').write_text(code)
 subprocess.run(['cc','-I'+str(R/'include'),str(p/'test.c'),'-o',str(p/'test')],check=True);subprocess.run([str(p/'test')],check=True)
print('PASS: stable released IDs/save flag sizes, seven complete obtainable species, evolution branches and Ice Body weather healing.')
