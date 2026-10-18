"""Source-backed encounter planning and asset presence checks."""
import hashlib
import json
import re
from datetime import datetime

WILD = 'src/data/wild_encounters.json'
def revision(raw):
    return hashlib.sha256(raw.encode()).hexdigest()

def encounters(root):
    raw=(root/WILD).read_text()
    tables=[]
    for gi,group in enumerate(json.loads(raw)['wild_encounter_groups']):
        for ei,enc in enumerate(group['encounters']):
            for field in group['fields']:
                kind=field['type']
                if kind not in enc: continue
                table=enc[kind]
                warnings=[]
                if len(table['mons'])!=len(field['encounter_rates']): warnings.append('Slot count differs from configured weights; extra slots have no configured chance. Review the engine selector before relying on these encounters.')
                if kind=='fishing_mons': warnings.append('The current fishing selector combines slots 1 and 2 into slot 2. Configured weights below are not effective in-game odds.')
                if kind=='rock_smash_mons': warnings.append('Rock Smash uses the water selector in the current engine. Configured rock weights are not effective in-game odds.')
                tables.append(dict(id=f'{gi}:{ei}:{kind}',map=enc.get('map',enc.get('base_label','Unknown')),method=kind,rate=table['encounter_rate'],slots=table['mons'],warnings=warnings,weights=(field['encounter_rates']+[0]*len(table['mons']))[:len(table['mons'])],groups=field.get('groups',{})))
    return dict(revision=revision(raw),tables=tables)

def save_encounters(root,species,payload):
    path=root/WILD;raw=path.read_text()
    if payload.get('revision')!=revision(raw): raise ValueError('Encounters changed on disk. Reload before saving.')
    entry=next((e for e in encounters(root)['tables'] if e['id']==payload.get('id')),None)
    if entry is None: raise ValueError('Unknown encounter table')
    slots=payload.get('slots');rate=payload.get('rate')
    if type(rate) is not int or not 0<=rate<=255: raise ValueError('Encounter rate must be 0–255')
    if not isinstance(slots,list) or len(slots)!=len(entry['slots']): raise ValueError('Keep the original slot count')
    valid={'SPECIES_'+s['key'] for s in species}
    for slot in slots:
        if not isinstance(slot,dict) or set(slot)!={'species','min_level','max_level'}: raise ValueError('Invalid slot')
        if slot['species'] not in valid: raise ValueError('Unknown species')
        if any(type(slot[k]) is not int for k in ('min_level','max_level')) or not 1<=slot['min_level']<=slot['max_level']<=100: raise ValueError('Levels must be 1–100, minimum ≤ maximum')
    doc=json.loads(raw);gi,ei,kind=entry['id'].split(':')
    doc['wild_encounter_groups'][int(gi)]['encounters'][int(ei)][kind].update(encounter_rate=rate,mons=slots)
    if path.read_text()!=raw: raise ValueError('Encounters changed during save')
    backup=root/'.cache/debug-dashboard/source-backups'/datetime.now().strftime('%Y%m%d-%H%M%S-%f')
    backup.mkdir(parents=True);(backup/'wild_encounters.json').write_text(raw)
    temp=path.with_suffix('.json.tmp');temp.write_text(json.dumps(doc,indent=2)+'\n');temp.replace(path)
    return dict(message='Encounter table saved. Rebuild the ROM to apply it.',backup=str(backup))

def assets(root,species):
    from pokedex import catalog, details
    from overworlds import catalog as overworld_catalog
    symbols=dict(re.findall(r'const u\d+ (\w+)\[\] = INCBIN_U\d+\(\s*"([^"]+)"\)',(root/'src/data/graphics/pokemon.h').read_text()))
    icons=dict(re.findall(r'\[SPECIES_(\w+)\s*\]\s*=\s*(gMonIcon_\w+)',(root/'src/pokemon_icon.c').read_text()))
    rows=[]
    for mon in catalog(root,species)['species']:
        detail=details(root,mon)
        paths={k:mon[k] for k in ('front','back','normal','shiny')}
        paths.update(footprint=detail['footprint'],cry=detail['cry'],icon=symbols.get(icons.get(mon['key'],''),'').replace('.4bpp','.png'))
        checks={k:bool(p and (root/p).is_file()) for k,p in paths.items()}
        mon['issues'].extend('Placeholder '+k+' artwork' for k,p in paths.items() if 'question_mark' in p)
        rows.append(dict(name=mon.get('name',mon['key']),key=mon['key'],group='Pokémon',checks=checks,paths=paths,issues=list(dict.fromkeys(mon['issues']+['Missing '+k for k,ok in checks.items() if not ok]))))
    for ow in overworld_catalog(root):
        paths={f['path'] for f in ow['frames']}
        issues=list(ow.get('issues',[]))+['Missing '+p for p in paths if not (root/p).is_file()]
        rows.append(dict(name=ow['name'],key=ow['id'],group='Overworld',checks={'sheets':all((root/p).is_file() for p in paths)},paths={str(i):p for i,p in enumerate(sorted(paths))},issues=issues))
    return dict(rows=rows,note='Checks verify registered source files and animation definitions, not artistic completeness. Shared or placeholder artwork still needs visual review.')
