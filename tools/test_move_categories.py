#!/usr/bin/env python3
"""Check all move categories and exercise the production damage calculator."""
from pathlib import Path
import re, subprocess, tempfile
root = Path(__file__).resolve().parents[1]
text = (root/'src/data/battle_moves.h').read_text()
blocks = dict(re.findall(r'\[(MOVE_\w+)\] =\s*\{(.*?)\n    \}', text, re.S))
assert len(blocks) == int(re.search(r'#define MOVES_COUNT (\d+)', (root/'include/constants/moves.h').read_text())[1])
for name, block in blocks.items():
    assert len(re.findall(r'\.category = MOVE_CATEGORY_(?:PHYSICAL|SPECIAL|STATUS)', block)) == 1, name
for move, cat in {'WATERFALL':'PHYSICAL','SURF':'SPECIAL','FIRE_PUNCH':'PHYSICAL','SHADOW_BALL':'SPECIAL','BITE':'PHYSICAL','HIDDEN_POWER':'SPECIAL','COUNTER':'PHYSICAL','MIRROR_COAT':'SPECIAL','SCALDING_DIVE':'PHYSICAL','GLACIAL_SHELL':'STATUS'}.items():
    assert '.category = MOVE_CATEGORY_'+cat in blocks['MOVE_'+move]
source = (root/'src/pokemon.c').read_text()
function = source[source.index('#define APPLY_STAT_MOD'):source.index('\nu8 CountAliveMonsInBattle')]
header = r'''
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "constants/pokemon.h"
#include "constants/battle.h"
#include "constants/moves.h"
#include "constants/abilities.h"
#include "constants/items.h"
#include "constants/hold_effects.h"
#include "constants/species.h"
#include "constants/battle_move_effects.h"
typedef uint8_t u8; typedef uint16_t u16; typedef uint32_t u32; typedef int32_t s32;
#define ARRAY_COUNT(a) (sizeof(a)/sizeof((a)[0]))
#define IS_MOVE_PHYSICAL(m) (gBattleMoves[m].category == MOVE_CATEGORY_PHYSICAL)
#define IS_MOVE_SPECIAL(m) (gBattleMoves[m].category == MOVE_CATEGORY_SPECIAL)
#define ABILITY_ON_FIELD2(a) 0
#define WEATHER_HAS_EFFECT2 1
#define DYNAMIC_TYPE_MASK 0x3f
#define RESOURCE_FLAG_FLASH_FIRE 1
#define ABILITYEFFECT_FIELD_SPORT 0
#define ABILITYEFFECT_MUD_SPORT 1
#define ABILITYEFFECT_WATER_SPORT 2
#define BATTLE_ALIVE_DEF_SIDE 0
struct BattleMove { u8 effect,power,type,accuracy,pp,category,secondaryEffectChance,target; int8_t priority; u8 flags; };
#include "data/battle_moves.h"
struct BattlePokemon {u16 attack,defense,spAttack,spDefense,item,species,hp,maxHP;u8 ability,level,statStages[8];u32 status1;};
struct {u8 holdEffect,holdEffectParam;} gEnigmaBerries[4];
struct {u8 rageFistHits[2][6];} bs;
#define gBattleStruct (&bs)
struct {u8 flags[4];} flags;
struct {void *unused; typeof(&flags) flags;} resources={0,&flags};
#define gBattleResources (&resources)
u8 gBattlerPartyIndexes[4],gCritMultiplier=1;
u16 gCurrentMove,gBattleMovePower,gBattleWeather;
u32 gBattleTypeFlags;
const u8 sHoldEffectToType[][2]={{HOLD_EFFECT_WATER_POWER,TYPE_WATER}};
const u8 gStatStageRatios[13][2]={{2,8},{2,7},{2,6},{2,5},{2,4},{2,3},{2,2},{3,2},{4,2},{5,2},{6,2},{7,2},{8,2}};
u8 GetBattlerSide(u8 id){return id&1;}
u8 ItemId_GetHoldEffect(u16 i){return i;}
u8 ItemId_GetHoldEffectParam(u16 i){return 20;}
u8 AbilityBattleEffects(int a,int b,int c,int d,int e){return 0;}
u8 CountAliveMonsInBattle(u8 id){return 1;}
'''
checks = r'''
struct BattlePokemon a,b;
void reset(void){memset(&a,0,sizeof(a));memset(&b,0,sizeof(b));memset(&flags,0,sizeof(flags));a.level=b.level=50;a.attack=200;a.spAttack=50;b.defense=b.spDefense=100;a.hp=b.hp=a.maxHP=b.maxHP=100;for(int i=0;i<8;i++)a.statStages[i]=b.statStages[i]=6;gBattleWeather=0;gBattleTypeFlags=0;}
int damage(int move,int screen,int type){gCurrentMove=move;return CalculateBaseDamage(&a,&b,move,screen,80,type,0,1);}
int main(void){
 reset();int physical=damage(MOVE_WATERFALL,0,0),special=damage(MOVE_SURF,0,0);assert(physical>special*3);
 a.status1=STATUS1_BURN;assert(damage(MOVE_WATERFALL,0,0)<physical);assert(damage(MOVE_SURF,0,0)==special);
 reset();assert(damage(MOVE_WATERFALL,SIDE_STATUS_REFLECT,0)<physical);assert(damage(MOVE_WATERFALL,SIDE_STATUS_LIGHTSCREEN,0)==physical);assert(damage(MOVE_SURF,SIDE_STATUS_LIGHTSCREEN,0)<special);assert(damage(MOVE_SURF,SIDE_STATUS_REFLECT,0)==special);
 assert(damage(MOVE_WATERFALL,0,TYPE_FIRE)==physical);assert(damage(MOVE_SURF,0,TYPE_FIGHTING)==special);
 gBattleWeather=B_WEATHER_RAIN_TEMPORARY;assert(damage(MOVE_WATERFALL,0,0)>physical);assert(damage(MOVE_SURF,0,0)>special);
 reset();int fire=damage(MOVE_FIRE_PUNCH,0,0);b.ability=ABILITY_THICK_FAT;assert(damage(MOVE_FIRE_PUNCH,0,0)<fire);
 reset();b.ability=ABILITY_INVISIBLE_WALL;assert(damage(MOVE_WATERFALL,0,0)<physical);assert(damage(MOVE_SURF,0,0)==special);
 reset();flags.flags[0]=RESOURCE_FLAG_FLASH_FIRE;assert(damage(MOVE_FIRE_PUNCH,0,0)>fire);
 puts("All move categories verified; production damage category, burn, screens, dynamic type, weather, Thick Fat, Flash Fire and Invisible Wall checks passed.");
}
'''
with tempfile.TemporaryDirectory() as td:
    p=Path(td)/'test.c';p.write_text(header.replace('#include "data/battle_moves.h"', '\n'.join(line for line in (root/'include/battle.h').read_text().splitlines() if line.startswith('#define MOVE_TARGET') or line.startswith('#define ABILITYEFFECT_') or line.startswith('#define BATTLE_ALIVE_')) + '\n#include "data/battle_moves.h"')+function+checks)
    subprocess.run(['cc','-std=gnu99','-I'+str(root/'include'),'-I'+str(root/'src'),str(p),'-o',td+'/test'],check=True)
    subprocess.run([td+'/test'],check=True)
