#!/usr/bin/env python3
"""Lance is the sole trainer allowed Primeval Pokémon."""
from pathlib import Path
import re
ROOT=Path(__file__).resolve().parents[1]
quests=(ROOT/'src/quests.c').read_text()
primeval=set(re.findall(r'SPECIES_\w+',quests.split('sPrimevalSpecies[] = {')[1].split('};')[0]))
parties=(ROOT/'src/data/trainer_parties.h').read_text()
assert len(primeval)==6
for name,body in re.findall(r'static const struct \w+ (\w+)\[\] = \{(.*?)\n\};',parties,re.S):
    found=primeval.intersection(re.findall(r'SPECIES_\w+',body))
    if name=='sParty_EliteFourLance':
        assert found=={'SPECIES_CINNABAR_FEEBAS','SPECIES_CINNABAR_MAGIKARP','SPECIES_AEROPTERYX'}
    else:
        assert not found,name
for path in (ROOT/'src').glob('*challenge*.c'):
    assert not primeval.intersection(re.findall(r'SPECIES_\w+',path.read_text())),path
print('PASS: only Lance uses Primeval species; other trainer parties and challenge generators exclude them')
