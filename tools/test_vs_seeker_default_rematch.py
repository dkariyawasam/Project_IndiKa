#!/usr/bin/env python3
"""Run the engine's rematch selection functions against table and fallback cases."""
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
source = (ROOT / 'src/vs_seeker.c').read_text()

def function(signature):
    start = source.index(signature + '\n{')
    brace = source.index('{', start)
    depth = 1
    end = brace + 1
    while depth:
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    return source[start:end]

program = r'''
#include <assert.h>
#include <stddef.h>
typedef unsigned char u8;
typedef unsigned short u16;
#define MAX_REMATCH_PARTIES 6
#define TRAINER_NONE 0
#define SKIP 65535
#define NELEMS(a) (sizeof(a) / sizeof((a)[0]))
struct RematchData { u16 trainerIdxs[MAX_REMATCH_PARTIES]; };
static const struct RematchData sRematches[] = {
    {{10, 11, 12}}, {{20}}, {{50, 51}}
};
static int eligible = 1, progressed = 1, fought11 = 0;
static int IsTrainerExcludedFromVsSeekerRematches(u16 id) { return id == 40 || id == 50; }
static int IsTrainerEligibleForDefaultRematch(u16 id) { (void)id; return eligible; }
static int HasTrainerBeenFought(u16 id) { return id == 11 && fought11; }
static void TryGetRematchTrainerIdGivenGameState(const u16 *ids, u8 *idx) {
    (void)ids; if (!progressed) *idx = 0;
}
'''
for signature in [
    'static int GetRematchIdx(const struct RematchData * vsSeekerData, u16 trainerFlagIdx)',
    'static u8 GetNextAvailableRematchTrainer(const struct RematchData * vsSeekerData, u16 trainerFlagNo, u8 * idxPtr)',
    'int GetRematchTrainerId(u16 trainerId)',
]:
    program += '\n' + function(signature) + '\n'
program += r'''
int main(void) {
    assert(GetRematchTrainerId(30) == 30); /* Unlisted trainer must never become row zero. */
    assert(GetRematchTrainerId(20) == 20); /* Listed trainer with no upgraded party. */
    assert(GetRematchTrainerId(10) == 11);
    fought11 = 1;
    assert(GetRematchTrainerId(10) == 12);
    progressed = 0;
    assert(GetRematchTrainerId(10) == 10);
    assert(GetRematchTrainerId(40) == 0);
    assert(GetRematchTrainerId(50) == 0);
    eligible = 0;
    assert(GetRematchTrainerId(30) == 0);
    return 0;
}
'''
with tempfile.TemporaryDirectory() as tmp:
    c = Path(tmp) / 'test.c'
    exe = Path(tmp) / 'test'
    c.write_text(program)
    subprocess.run(['cc', '-std=c99', str(c), '-o', str(exe)], check=True)
    subprocess.run([str(exe)], check=True)
print('PASS: default, dedicated, progression-gated and excluded rematch selection')
