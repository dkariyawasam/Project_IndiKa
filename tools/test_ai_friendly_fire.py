#!/usr/bin/env python3
"""Host regression checks for the production doubles friendly-fire scoring pass.

Damage/type calculations are stubbed; these checks exercise scoring, gating,
immune-result handling, and preservation of battle globals, not damage formulas.
"""
from pathlib import Path
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
source = (root / 'src/battle_ai_script_commands.c').read_text()
function = source.split('static void BattleAI_ScoreFriendlyFire(void)', 1)[1].split('\nu8 BattleAI_ChooseMoveOrAction', 1)[0]
header = r'''
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
typedef uint8_t u8; typedef uint16_t u16; typedef int32_t s32;
#define BIT_FLANK 2
#define BATTLE_TYPE_DOUBLE 1
#define MAX_MON_MOVES 4
#define MOVE_TARGET_FOES_AND_ALLY 8
#define MOVE_EARTHQUAKE 1
#define MOVE_MAGNITUDE 2
#define MOVE_RESULT_NO_EFFECT 7
#define STATUS3_UNDERGROUND 128
struct BattlePokemon { u16 hp, moves[4]; u8 pp[4]; } gBattleMons[4];
struct { u8 target, power; } gBattleMoves[10];
struct { u8 helpingHand; } gProtectStructs[4];
struct { u8 score[4]; } thinking;
#define AI_THINKING_STRUCT (&thinking)
u8 gBattlerAttacker, gBattlerTarget, gCritMultiplier, gAbsentBattlerFlags;
u16 gCurrentMove, gBattleMovePower, gSideStatuses[2];
s32 gBattleMoveDamage;
unsigned gBattleTypeFlags, gStatuses3[4], gBitTable[4] = {1,2,4,8};
s32 predicted; u8 typeFlags; u16 observedPower;
u8 GetBattlerSide(u8 n) { return n & 1; }
s32 CalculateBaseDamage(struct BattlePokemon *a, struct BattlePokemon *b,
 u16 move, u16 side, u16 power, u8 type, u8 attacker, u8 defender) {
 assert(attacker == 1 && defender == 3 && gBattlerTarget == 3);
 assert(gCritMultiplier == 1 && gCurrentMove == move);
 observedPower = power; gBattleMovePower = 999; return predicted;
}
u8 TypeCalc(u16 move, u8 attacker, u8 defender) { return typeFlags; }
'''
checks = r'''
static void reset(void) {
 memset(gBattleMons,0,sizeof(gBattleMons)); memset(gStatuses3,0,sizeof(gStatuses3));
 memset(gProtectStructs,0,sizeof(gProtectStructs)); memset(gBattleMoves,0,sizeof(gBattleMoves));
 gBattlerAttacker=1; gBattlerTarget=0; gCritMultiplier=2;
 gCurrentMove=9; gBattleMovePower=321; gBattleMoveDamage=456;
 gBattleTypeFlags=BATTLE_TYPE_DOUBLE; gAbsentBattlerFlags=0;
 gBattleMons[3].hp=100; predicted=20; typeFlags=0; observedPower=0;
 for(int i=0;i<4;i++) { thinking.score[i]=100; gBattleMons[1].pp[i]=10; }
 gBattleMons[1].moves[0]=1;
 gBattleMoves[1].target=MOVE_TARGET_FOES_AND_ALLY; gBattleMoves[1].power=100;
}
static void run(void) {
 BattleAI_ScoreFriendlyFire();
 assert(gBattlerTarget==0 && gCritMultiplier==2 && gCurrentMove==9);
 assert(gBattleMovePower==321 && gBattleMoveDamage==456);
 assert(thinking.score[1]==100);
}
int main(void) {
 reset();run();assert(thinking.score[0]==84); // chip damage
 reset();predicted=99;run();assert(thinking.score[0]==61);
 reset();predicted=100;thinking.score[0]=110;run();assert(thinking.score[0]==30); // KO bonus outweighed
 reset();typeFlags=2;run();assert(thinking.score[0]==100); // Flying / Levitate no-effect
 reset();typeFlags=1;run();assert(thinking.score[0]==100); // Wonder Guard missed
 reset();gBattleTypeFlags=0;run();assert(thinking.score[0]==100);
 reset();gAbsentBattlerFlags=8;run();assert(thinking.score[0]==100);
 reset();gBattleMons[3].hp=0;run();assert(thinking.score[0]==100);
 reset();thinking.score[0]=0;run();assert(thinking.score[0]==0);
 reset();gBattleMons[1].pp[0]=0;run();assert(thinking.score[0]==100);
 reset();thinking.score[0]=10;predicted=100;run();assert(thinking.score[0]==1);
 reset();gBattleMoves[1].target=0;run();assert(thinking.score[0]==100);
 reset();gBattleMoves[1].power=0;run();assert(thinking.score[0]==100);
 reset();gBattleMons[1].moves[0]=MOVE_MAGNITUDE;gBattleMoves[2]=gBattleMoves[1];
 gBattleMoves[2].power=1;run();assert(observedPower==71 && thinking.score[0]<100);
 reset();predicted=50;gStatuses3[3]=STATUS3_UNDERGROUND;run();assert(thinking.score[0]==20);
 reset();predicted=70;gProtectStructs[1].helpingHand=1;run();assert(thinking.score[0]==20);
 puts("16 friendly-fire scoring scenarios passed; battle globals preserved.");
}
'''
with tempfile.TemporaryDirectory() as tmp:
    c = Path(tmp) / 'test.c'
    exe = Path(tmp) / 'test'
    c.write_text(header + '\nstatic void BattleAI_ScoreFriendlyFire(void)' + function + checks)
    subprocess.run(['cc', '-std=c99', str(c), '-o', str(exe)], check=True)
    subprocess.run([str(exe)], check=True)
