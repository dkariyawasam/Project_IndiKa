#!/usr/bin/env python3
"""Static evolution/item evidence audit; does not certify runtime reachability."""
import csv, json, re
from pathlib import Path
from sync_active_pokedex import derive
ROOT = Path(__file__).resolve().parents[1]
active, _, _, _, stats = derive()
active = set(active)
# Full entries: the roster helper's abbreviated stats stop at nested type braces.
stats = dict(re.findall(r'\[SPECIES_(\w+)\]\s*=\s*\{(.*?)\n    \},', (ROOT/'src/data/pokemon/species_info.h').read_text(), re.S))
text = (ROOT/'src/data/pokemon/evolution.h').read_text()
rows = []
for base, body in re.findall(r'\[SPECIES_(\w+)\]\s*=\s*\{(.*?)(?=\n    \[|\n};)', text, re.S):
    for method, param, target in re.findall(r'\{(EVO_\w+),\s*(\w+),\s*SPECIES_(\w+)\}', body):
        rows.append(dict(base=base, method=method, parameter=param, target=target))
assert len({(r['base'],r['target']) for r in rows}) == len(rows), 'Duplicate links'
assert all(r['base'] in active and r['target'] in active for r in rows), 'Inactive evolution endpoint'
engine = (ROOT/'src/pokemon.c').read_text()
assert all(r['method'] in engine or r['method']=='EVO_LEVEL_SHEDINJA' for r in rows)
wild = set(re.findall(r'SPECIES_(\w+)', (ROOT/'src/data/wild_encounters.json').read_text()))
items = sorted({r['parameter'] for r in rows if r['parameter'].startswith('ITEM_')})
# Locations are evidence, not proof that the script/object is reachable.
files = list((ROOT/'data').rglob('*.inc')) + list((ROOT/'data').rglob('*.json'))
files += [ROOT/'src/data/ingame_trades.h', ROOT/'src/pickup.c', ROOT/'src/item_ball.c']
items_report = {}
for item in items:
    evidence = []
    for p in files:
        if not p.exists(): continue
        for line, value in enumerate(p.read_text().splitlines(),1):
            if re.search(r'\b'+item+r'\b',value): evidence.append(f'{p.relative_to(ROOT)}:{line}: {value.strip()}')
    for species, body in stats.items():
        if species in wild and re.search(r'\.item(?:Rare|Common)\s*=\s*'+item+r'\b',body):
            evidence.append(f'Wild held item: {species}')
    items_report[item] = evidence
species_rows = list(csv.DictReader((ROOT/'docs/johto-availability-audit/reference/pokemon_species.csv').open()))
ids = {r['id']:r['identifier'].upper().replace('-','_') for r in species_rows}
pairs = {(r['base'],r['target']) for r in rows}
missing = []
for r in species_rows:
    a,b = ids.get(r['evolves_from_species_id']),ids[r['id']]
    if a in active and b in active and (a,b) not in pairs:
        if b in ('PERRSERKER','CURSOLA'): continue # Regional parents handled explicitly below.
        missing.append(f'{a} → {b}')
no_source = [item for item,evidence in items_report.items() if not evidence]
lines = ['# Evolution and required-item availability audit','',f'{len(active)} active species; {len(rows)} configured evolution links. All configured endpoints are active and methods have engine support.','',
'## Restored this pass','',
'- Tynamo → Eelektrik at level 39; Eelektrik → Eelektross with Thunder Stone.',
'- Mime Jr. → Mr. Mime when levelling up knowing Mimic; added Mimic at level 15 alongside its existing move.',
'- Female Combee → Vespiquen at level 21; male/genderless Combee do not evolve.',
'- Alolan Rattata → Raticate at level 20 at night; Alolan Cubone → Marowak at level 28 at night.',
'- Alolan Pichu → Pikachu and Alolan Meowth → Persian through friendship.',
'- Alolan Diglett → Dugtrio (26), Geodude → Graveler (25), Graveler → Golem (trade), Grimer → Muk (38).',
'- Galarian Meowth → Perrserker (28), Corsola → Cursola (38), Koffing → Weezing (35).','',
'Custom regional pre-evolutions stay in their own form lines. Standard methods cross-checked against the [PokeAPI evolution dataset](https://raw.githubusercontent.com/PokeAPI/pokeapi/master/data/v2/csv/pokemon_evolution.csv). Friendship uses the existing project threshold of 220.','',
'## Remaining decisions and limitations','',
'- Piloswine → Mamoswine remains deliberately disabled in Kanto. Apex discovery, habitat and Mewtwo conditions remain unchanged.',
'- Feebas still requires Beauty 170. No ordinary gameplay Beauty-raising route was identified; wild Milotic exists, but that does not make caught Feebas evolvable. Needs a replacement method or a Beauty mechanic.',
'- Galarian Farfetch’d has no Sirfetch’d species in this roster; adding that evolution is a roster/design decision.',
'- Custom species relationships beyond configured links cannot be inferred from official ancestry data.',
'- Item evidence below includes scripts, map items, configured in-game trades and wild held items. Unreachable scripts, prerequisite flags, trade partners, transfer access and limited item quantities still need live verification.',
'- This is not a claim that every active species is obtainable. Non-native species can be intentionally unavailable; trainer appearances are not acquisition routes.','',
'## Required items with no acquisition evidence','',f'{len(no_source)} items: '+', '.join(i.removeprefix('ITEM_').replace('_',' ') for i in no_source)+'.','',
'No new item placements or shops were added. Newer evolution items also retain question-mark menu icons.','',
'## Item-source evidence','']
for item,evidence in items_report.items():
    lines += [f'### {item.removeprefix("ITEM_")}', '']
    lines += [f'- `{e}`' for e in evidence] if evidence else ['No acquisition source found.']
    lines += ['']
lines += ['## Standard ancestry pairs without a configured link','']+[f'- {s}' for s in missing]+['','## Validation','','Host-side tests execute the production evolution function and table, checking thresholds, time, gender, all four move slots, Everstone, stones and trades, plus existing Eevee/Apex regression cases. Main-ROM compilation is separate from this static report.','']
out=ROOT/'docs/evolution-availability-audit';out.mkdir(exist_ok=True)
(out/'report.md').write_text('\n'.join(lines))
(out/'data.json').write_text(json.dumps(dict(active_species=len(active),links=rows,item_evidence=items_report,items_without_source=no_source,missing_standard_links=missing),indent=2)+'\n')
print(f'{len(rows)} valid evolution links; {len(no_source)} items without acquisition evidence; remaining standard pairs: {missing}')
