from pathlib import Path
import subprocess,tempfile,re
r=Path(__file__).resolve().parents[1]
s=(r/'src/battle_script_commands.c').read_text();i=s.index('                if (gBattleMoveDamage > 0 && gBattleMons');j=s.index('                if (gHitMarker & HITMARKER_SKIP_DMG_TRACK)',i);hit=s[i:j]
s=(r/'src/pokemon.c').read_text();i=s.index('    if (move == MOVE_RAGE_FIST && !powerOverride)');j=s.index('\n\n',i);power=s[i:j]
code='''#include <assert.h>
#include <stdint.h>
#include <stdio.h>
typedef uint8_t u8;
#define HITMARKER_PASSIVE_DAMAGE 1
#define HITMARKER_SKIP_DMG_TRACK 2
#define BS_TARGET 1
#define MOVE_PAIN_SPLIT 1
#define MOVE_RAGE_FIST 2
static struct {u8 rageFistHits[2][6];} mem,*gBattleStruct=&mem;
static struct {int hp;} gBattleMons[4];
static struct {int power;} gBattleMoves[4];
static int gBattleMoveDamage,gActiveBattler,gBattlerAttacker,gHitMarker,gCurrentMove,gBattlerPartyIndexes[4],gBattleMovePower;
static u8 instr[2],*gBattlescriptCurrInstr=instr;
static int GetBattlerSide(int b){return b%2;}
static void hit(void){
'''+hit+'''}
static int power(int battlerIdAtk){int move=MOVE_RAGE_FIST,powerOverride=0;
'''+power+'''return gBattleMovePower;}
int main(void){
 gActiveBattler=1;gBattlerAttacker=0;instr[1]=BS_TARGET;gBattleMoveDamage=10;gBattleMons[1].hp=100;gCurrentMove=2;gBattleMoves[2].power=50;
 assert(power(1)==50);
 for(int n=1;n<=10;n++){hit();assert(power(1)==50*(1+(n<6?n:6)));}
 gBattlerPartyIndexes[1]=1;assert(power(1)==50);hit();assert(power(1)==100);gBattlerPartyIndexes[1]=0;assert(power(1)==350);
 gBattlerPartyIndexes[1]=2;
 gHitMarker=HITMARKER_PASSIVE_DAMAGE;hit();assert(power(1)==50);
 gHitMarker=HITMARKER_SKIP_DMG_TRACK;hit();assert(power(1)==50);
 gHitMarker=0;gBattlerAttacker=1;hit();assert(power(1)==50);
 gBattlerAttacker=0;gCurrentMove=MOVE_PAIN_SPLIT;gBattleMoves[1].power=1;hit();assert(power(1)==50);
 gCurrentMove=2;gBattleMoveDamage=0;hit();assert(power(1)==50);
 gBattleMoveDamage=10;gBattleMons[1].hp=0;hit();assert(power(1)==50);
 puts("PASS: Rage Fist 50-350 power, per-hit increment, party-slot persistence, and passive/self/zero-damage exclusions.");
}
'''
assert s.index('move == MOVE_RAGE_FIST')>s.index('s32 CalculateBaseDamage')
learn=(r/'src/data/pokemon/level_up_learnsets.h').read_text()
for name in ('Primeape','Annihilape'):
 assert 'LEVEL_UP_MOVE(33, MOVE_RAGE_FIST)' in learn.split('s'+name+'LevelUpLearnset[]')[1].split('};')[0]
assert 'MOVE_RAGE_FIST' not in (r/'src/data/pokemon/tmhm_learnsets.h').read_text()
assert 'EVO_APEX_MOVE, MOVE_RAGE_FIST, SPECIES_ANNIHILAPE' in (r/'src/data/pokemon/evolution.h').read_text()
with tempfile.TemporaryDirectory() as t:
 p=Path(t);(p/'test.c').write_text(code);subprocess.run(['cc',str(p/'test.c'),'-o',str(p/'test')],check=True);subprocess.run([str(p/'test')],check=True)
