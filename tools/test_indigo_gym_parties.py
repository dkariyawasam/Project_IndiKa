#!/usr/bin/env python3
"""Check League-only party selection and the global prohibited-species rule."""
import re
import subprocess
import tempfile
from pathlib import Path
from audit_trainer_rosters import APEX_SPECIES, parse_parties

ROOT = Path(__file__).resolve().parents[1]
source = (ROOT / 'src/data/indigo_gym_parties.h').read_text()
parties = parse_parties(source)
assert len(parties) == 8
for name, species in parties.items():
    assert len(species) == 6, name
    assert len(set(species)) == 6, name
    assert not set(species) & (APEX_SPECIES | {'SPECIES_KINKABUTO', 'SPECIES_AMUNYTE'}), name
assert source.count('.iv = 250') == 48
assert source.count('.lvl = 50') == 48
assert 'ITEM_NONE' not in source
# Compile the actual selector, verifying that Gym/outside-League battles get no override.
constants = sorted(set(re.findall(r'\b(?:SPECIES|MOVE|ITEM|TRAINER)_[A-Z0-9_]+\b', source)))
program = '#include <stddef.h>\n#include <assert.h>\ntypedef unsigned short u16;\n'
program += 'struct TrainerMonItemCustomMoves {u16 iv,lvl,species,heldItem,moves[4];};\n'
program += 'static int active; static int IsIndigoLeagueTrainerBattle(void) {return active;}\n'
program += ''.join(f'#define {c} {i+1}\n' for i,c in enumerate(constants))
program += source
program += '\nint main(void) {\n'
for trainer, party in re.findall(r'case (TRAINER_\w+): return (sParty_\w+);', source):
    program += f'active=0; assert(GetIndigoGymLeaderParty({trainer})==NULL);\n'
    program += f'active=1; assert(GetIndigoGymLeaderParty({trainer})=={party});\n'
program += 'assert(GetIndigoGymLeaderParty(65535)==NULL); return 0;}\n'
with tempfile.TemporaryDirectory() as directory:
    c = Path(directory) / 'test.c'
    binary = Path(directory) / 'test'
    c.write_text(program)
    subprocess.run(['cc', '-Wall', '-Werror', str(c), '-o', str(binary)], check=True)
    subprocess.run([str(binary)], check=True)
print('Eight six-member Indigo teams verified; overrides disabled outside Indigo.')
