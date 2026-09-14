#!/usr/bin/env python3
"""Check that the retired facility has no map or scripted entry point."""
from pathlib import Path
import re

root = Path(__file__).resolve().parents[2]
pattern = re.compile(r'battle[ _]?tower|battle[ _]?frontier|trainer[ _]?tower', re.I)
paths = list((root / 'data/maps').rglob('*.json'))
paths += list((root / 'data/maps').rglob('*.inc'))
paths += list((root / 'data/scripts').rglob('*.inc'))
paths += [root / 'include/constants/map_groups.h']
errors = []
# Catch entry specials whose names do not mention the facility as well.
source = (root / 'data/specials.inc').read_text()
tower = (root / 'src/battle_tower.c').read_text()
names = [name for name in re.findall(r'def_special (\w+)', source)
         if re.search(r'^void ' + name + r'\(', tower, re.M)]
for path in paths:
    for number, line in enumerate(path.read_text().splitlines(), 1):
        if pattern.search(line) or any(re.search(r'\b' + name + r'\b', line) for name in names):
            errors.append(f'{path.relative_to(root)}:{number}: {line.strip()}')
assert not errors, '\n'.join(errors)
print(f'PASS: no retired facility maps, references or entry scripts in {len(paths)} files')
