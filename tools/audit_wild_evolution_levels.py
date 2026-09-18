#!/usr/bin/env python3
"""Report wild slots reaching a natural level-evolution threshold; no game edits."""
import json,re,csv
from pathlib import Path
from collections import Counter
ROOT=Path(__file__).resolve().parents[1]
s=(ROOT/'src/data/pokemon/evolution.h').read_text()
thresholds={}
for match in re.finditer(r'\[(SPECIES_\w+)\]\s*=\s*\{(.*?)\}\s*,',s,re.S):
 evos=re.findall(r'\{(EVO_LEVEL(?:_ATK_GT_DEF|_ATK_EQ_DEF|_ATK_LT_DEF|_SILCOON|_CASCOON|_NINJASK|_SHEDINJA)?),\s*(\d+),\s*(SPECIES_\w+)',match[2])
 if evos:thresholds[match[1]]=min(int(e[1]) for e in evos)
rows=[];slots=0;tables=0
for group in json.loads((ROOT/'src/data/wild_encounters.json').read_text())['wild_encounter_groups']:
 for table in group['encounters']:
  tables+=1
  for method,info in table.items():
   if not isinstance(info,dict) or 'mons' not in info:continue
   for index,mon in enumerate(info['mons'],1):
    slots+=1;species=mon['species'];threshold=thresholds.get(species);lo=min(mon['min_level'],mon['max_level']);hi=max(mon['min_level'],mon['max_level'])
    if threshold and hi>=threshold:
     rows.append(dict(map=table.get('map',''),table=table['base_label'],method=method,slot=index,species=species.removeprefix('SPECIES_'),min_level=lo,max_level=hi,evolution_level=threshold,affected_levels=f'{max(lo,threshold)}–{hi}'))
out=ROOT/'docs/wild-evolution-level-audit.csv'
with out.open('w') as f:
 writer=csv.DictWriter(f,fieldnames=list(rows[0]) if rows else ['map']);writer.writeheader();writer.writerows(rows)
counts=Counter(r['species'] for r in rows)
lines=['# Wild evolution-level audit','',f'Checked {slots} slots across {tables} encounter tables, including day/night variants and all encounter methods.',f'Found {len(rows)} violating slots across {len(counts)} species and {len(set(r["map"] for r in rows))} maps.','', 'Inclusive maximum levels are checked against the project evolution table. Includes level/stat and level/personality branches; excludes friendship, trade, item, held-item/time and beauty requirements, which have no natural fixed level cutoff. This checks unevolved Pokémon at or above their next evolution threshold, not evolved Pokémon below their acquisition level. No encounter data changed.','', '| Species | Evolution level | Slots | Highest wild level |','|---|---:|---:|---:|']
for species,count in counts.most_common():lines.append(f'| {species} | {thresholds["SPECIES_"+species]} | {count} | {max(r["max_level"] for r in rows if r["species"]==species)} |')
(ROOT/'docs/wild-evolution-level-audit.md').write_text('\n'.join(lines)+'\n')
print('\n'.join(lines[:5]));print(counts)
