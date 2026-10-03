#!/usr/bin/env python3
"""Check signature-move wiring and execute the actual Glacial Shell status handler."""
import re
import subprocess
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
def read(path):
    return (ROOT / path).read_text()

source = read('src/battle_script_commands.c')
body = source.split('    case VARIOUS_GLACIAL_SHELL:', 1)[1].split('    case VARIOUS_WAIT_FANFARE:', 1)[0]
body = body.rsplit('        break;', 1)[0]
code = r'''
#include <assert.h>
#include <stdint.h>
#define STATUS1_FREEZE 32
#define MULTISTRING_CHOOSER 5
#define BUFFER_A 0
#define REQUEST_STATUS_BATTLE 0
struct {uint32_t status1;} gBattleMons[4];
int gActiveBattler=1, gEffectBattler, writes, marks;
uint32_t savedStatus;
unsigned char gBattleCommunication[8];
void BtlController_EmitSetMonData(int a,int b,int c,int size,void *ptr) {
 assert(size==4); savedStatus=*(uint32_t *)ptr; writes++;
}
void MarkBattlerForControllerExec(int battler) {assert(battler==1); marks++;}
void apply(void) {
''' + body + r'''
}
int main(void) {
 gBattleMons[0].status1=8;
 apply();
 assert(gBattleMons[1].status1==32 && savedStatus==32);
 assert(gBattleCommunication[5]==1 && writes==1 && marks==1 && gEffectBattler==1);
 apply();
 assert(gBattleMons[1].status1==0 && savedStatus==0);
 assert(gBattleCommunication[5]==2 && writes==2 && marks==2);
 for(int status=1;status<=128;status*=2) {
  if(status==32) continue;
  gBattleMons[1].status1=status; apply();
  assert(gBattleMons[1].status1==status && gBattleCommunication[5]==0);
  assert(writes==2 && marks==2);
 }
 assert(gBattleMons[0].status1==8);
 return 0;
}
'''
with tempfile.TemporaryDirectory() as directory:
    c = Path(directory) / 'check.c'
    exe = Path(directory) / 'check'
    c.write_text(code)
    subprocess.run(['cc', str(c), '-o', str(exe)], check=True)
    subprocess.run([str(exe)], check=True)

scripts = read('data/battle_scripts_1.s')
assert 'setmoveeffect MOVE_EFFECT_BURN | MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN' in scripts
freeze = read('src/battle_util.c').split('case CANCELLER_FROZEN:',1)[1].split('case CANCELLER_TRUANT:',1)[0]
assert freeze.index('gCurrentMove == MOVE_GLACIAL_SHELL') < freeze.index('Random() % 5')
assert 'MOVE_EFFECT_AFFECTS_USER | STAT_CHANGE_ALLOW_PTR' in scripts.split('BattleScript_EffectGlacialShell::')[1]
for species, move in [('CINNABAR_MAGIKARP','SCALDING_DIVE'),('CINNABAR_FEEBAS','GLACIAL_SHELL')]:
    block = re.search(r'\[SPECIES_'+species+r'\]\s*=\s*\{.*?\n    \}',read('src/data/pokemon/species_info.h'),re.S)[0]
    assert '{ABILITY_SWIFT_SWIM, ABILITY_MARVEL_SCALE}' in block
    assert f'LEVEL_UP_MOVE(33, MOVE_{move})' in read('src/data/pokemon/level_up_learnsets.h')
    assert f'[MOVE_{move}]' in read('src/data/battle_moves.h')
    assert f'[MOVE_{move} - 1]' in read('src/move_descriptions.c')
print('Deep-form move wiring and freeze/thaw/status preservation checks passed.')
