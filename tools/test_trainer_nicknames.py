#!/usr/bin/env python3
"""Exercise the production nickname lookup for every roster with nickname rules."""
import re
import subprocess
import tempfile
from pathlib import Path
from audit_trainer_rosters import parse_parties, parse_trainers

ROOT = Path(__file__).resolve().parents[1]
source = (ROOT / 'src/trainer_nicknames.c').read_text()
ids = dict(re.findall(r'^#define\s+(TRAINER_\w+)\s+(\w+)', (ROOT / 'include/constants/opponents.h').read_text(), re.M))
def resolve(trainer):
    while trainer in ids:
        trainer = ids[trainer]
    return trainer
ruled = {resolve(t) for t in re.findall(r'case (TRAINER_\w+):', source)}
parties = parse_parties((ROOT / 'src/data/trainer_parties.h').read_text())
rosters = [(t['id'], parties[t['party']]) for t in parse_trainers((ROOT / 'src/data/trainers.h').read_text()) if resolve(t['id']) in ruled]
league = (ROOT / 'src/data/indigo_gym_parties.h').read_text()
league_parties = parse_parties(league)
rosters += [(t, league_parties[p]) for t,p in re.findall(r'case (TRAINER_\w+): return (sParty_\w+);', league)]
program = '''#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "constants/species.h"
#include "constants/opponents.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#define POKEMON_NAME_LENGTH 10
#define ARRAY_COUNT(a) (sizeof(a)/sizeof((a)[0]))
#define _(s) s
'''
program += re.sub(r'^#include.*\n', '', source, flags=re.M)
program += '\nint main(void) { const u8 *name;\n'
for trainer, species in rosters:
    for slot, mon in enumerate(species):
        program += f'name=GetTrainerPokemonNickname({trainer},{mon},{slot}); assert(name != NULL && strlen((const char *)name)>0 && strlen((const char *)name)<=10);\n'
program += 'assert(GetTrainerPokemonNickname(TRAINER_BOSS_GIOVANNI_2,SPECIES_PERSIAN,0)==NULL); return 0;}\n'
with tempfile.TemporaryDirectory() as directory:
    c = Path(directory) / 'test.c'
    binary = Path(directory) / 'test'
    c.write_text(program)
    subprocess.run(['cc','-Wall','-Werror','-I',str(ROOT/'include'),str(c),'-o',str(binary)],check=True)
    subprocess.run([str(binary)],check=True)
print(f'PASS: {len(rosters)} named trainer rosters, {sum(len(s) for _,s in rosters)} party slots, including Indigo overrides.')
