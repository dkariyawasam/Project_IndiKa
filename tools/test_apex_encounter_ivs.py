#!/usr/bin/env python3
"""Run the actual Apex/ordinary battle starters against IV mutation spies."""
from pathlib import Path
import re
import subprocess
import tempfile
ROOT = Path(__file__).resolve().parents[1]
source = (ROOT / 'src/battle_setup.c').read_text()
def function(name):
    start = source.index('void ' + name + '(void)\n{')
    return source[start:source.index('\n}', start) + 2]
functions = '\n'.join(function(n) for n in ['StartApexBattle', 'StartScriptedWildBattle'])
constants = sorted(set(re.findall(r'\b(?:MON_DATA_|SPECIES_|BATTLE_TYPE_|B_TRANSITION_|MUS_|GAME_STAT_)\w+', functions)))
code = '''
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
typedef uint8_t u8;
typedef uint16_t u16;
struct Pokemon { u8 iv[6]; u16 species; };
static struct Pokemon gEnemyParty[6];
static struct {void (*savedCallback)(void);} gMain;
static unsigned gBattleTypeFlags;
static int writes, recalculations;
static uint32_t rng = 1;
static u16 Random(void) {rng = rng * 1103515245u + 12345u; return rng >> 16;}
'''
code += '\n'.join(f'#define {c} {i+1}' for i,c in enumerate(constants))
code += '''
static const int fields[] = {MON_DATA_HP_IV, MON_DATA_ATK_IV, MON_DATA_DEF_IV,
 MON_DATA_SPEED_IV, MON_DATA_SPATK_IV, MON_DATA_SPDEF_IV};
static void SetMonData(struct Pokemon *mon, int field, const void *value) {
 assert(mon == &gEnemyParty[0]);
 for (int i=0;i<6;i++) if (fields[i]==field) {mon->iv[i]=*(const u8*)value;writes++;return;}
 assert(0);
}
static void CalculateMonStats(struct Pokemon *mon) {
 int perfect=0;
 for (int i=0;i<6;i++) {assert(mon->iv[i]==7 || mon->iv[i]==31);perfect += mon->iv[i]==31;}
 assert(perfect>=3);
 recalculations++;
}
static u16 GetMonData(struct Pokemon *mon,int field) {return mon->species;}
static void LockPlayerFieldControls(void) {}
static void CB2_EndScriptedWildBattle(void) {}
static void CreateBattleStartTask(int transition,int music) {}
static int GetWildBattleTransition(void) {return 0;}
static void IncrementGameStat(int stat) {}
'''
code += functions
code += '''
int main(void) {
 for (int species=0;species<1024;species++) {
  memset(gEnemyParty,7,sizeof(gEnemyParty));gEnemyParty[0].species=species;
  writes=recalculations=0;StartApexBattle();
  assert(writes==3 && recalculations==1);
  int perfect=0;
  for(int i=0;i<6;i++) perfect += gEnemyParty[0].iv[i]==31;
  assert(perfect==3);
  for(int i=0;i<6;i++) assert(gEnemyParty[1].iv[i]==7);
  memset(gEnemyParty,7,sizeof(gEnemyParty));writes=recalculations=0;
  StartScriptedWildBattle();assert(writes==0 && recalculations==0);
  for(int i=0;i<6;i++) assert(gEnemyParty[0].iv[i]==7);
 }
 puts("PASS: three distinct perfect IVs, other rolls preserved, stat recalculation, untouched party slots and ordinary encounters.");
}
'''
callers = [p for p in (ROOT/'data/maps').glob('*/scripts.inc') if 'special StartApexBattle' in p.read_text()]
assert len(callers)==8, f'Review Apex encounter scope: {callers}'
with tempfile.TemporaryDirectory() as tmp:
    c=Path(tmp)/'test.c';exe=Path(tmp)/'test';c.write_text(code)
    subprocess.run(['cc','-std=c99',str(c),'-o',str(exe)],check=True)
    subprocess.run([str(exe)],check=True)
print('PASS: dedicated starter is used by exactly eight map encounters.')
