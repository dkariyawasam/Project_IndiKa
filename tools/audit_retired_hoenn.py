#!/usr/bin/env python3
"""Verify retired Hoenn slots remain blank and absent from gameplay content."""
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
RETIRED = 'TREECKO GROVYLE SCEPTILE TORCHIC COMBUSKEN BLAZIKEN MUDKIP MARSHTOMP SWAMPERT ARON LAIRON AGGRON LATIAS LATIOS KYOGRE GROUDON RAYQUAZA JIRACHI DEOXYS BAGON SHELGON SALAMENCE BELDUM METANG METAGROSS CACNEA CACTURNE CORPHISH CRAWDAUNT ELECTRIKE MANECTRIC LOTAD LOMBRE LUDICOLO NOSEPASS SEVIPER SHUPPET BANETTE SPHEAL SEALEO WALREIN SURSKIT MASQUERAIN TORKOAL TRAPINCH VIBRAVA FLYGON ZANGOOSE REGIROCK REGICE REGISTEEL TROPIUS KECLEON CASTFORM CARVANHA SHARPEDO SKITTY DELCATTY SNORUNT GLALIE FROSLASS SPIRITOMB WAILMER WAILORD'.split()
stats = (ROOT / 'src/data/pokemon/species_info.h').read_text()
names = (ROOT / 'src/data/text/species_names.h').read_text()
errors = []
for species in RETIRED:
    if (ROOT / "graphics/pokemon" / species.lower()).exists():
        errors.append(species + ": retired artwork directory still exists")
    if not re.search(r'\[SPECIES_' + species + r'\]\s*=\s*\{0\}', stats):
        errors.append(species + ': species data is not blank')
    if not re.search(r'\[SPECIES_' + species + r'\]\s*=\s*_\("\?\?\?\?\?"\)', names):
        errors.append(species + ': display name is not blank')
pattern = re.compile(r'\bSPECIES_(?:' + '|'.join(RETIRED) + r')\b')
# Engine compatibility branches and indexed species tables deliberately retain IDs.
paths = list((ROOT / 'data/maps').rglob('*.inc')) + list((ROOT / 'data/maps').rglob('*.json'))
paths += list((ROOT / 'data/scripts').rglob('*.inc'))
paths += [ROOT / p for p in ['src/data/trainer_parties.h', 'src/data/wild_encounters.json', 'src/data/pokemon/evolution.h', 'src/in_game_trade.c', 'src/roamer.c']]
paths += list((ROOT / 'src').glob('*league*.c'))
for path in paths:
    if path.exists():
        for number, line in enumerate(path.read_text().splitlines(), 1):
            if pattern.search(line):
                errors.append(f'{path.relative_to(ROOT)}:{number}: {line.strip()}')
if errors:
    raise SystemExit('\n'.join(errors))
print(f'PASS: {len(RETIRED)} retired Hoenn slots are blank; no gameplay references in {len(paths)} files.')
