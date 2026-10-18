"""Source Pokémon artwork and battle placement; no ROM or save writes."""
import re
import struct
import zlib
from pathlib import Path
from overworlds import transparent_png


def catalog(root, species):
    folder=root/'src/data/pokemon_graphics'
    symbols=dict(re.findall(r'const u\d+ (\w+)\[\] = INCBIN_U\d+\(\s*"([^"]+)"\)',(root/'src/data/graphics/pokemon.h').read_text()))
    def table(name):return dict(re.findall(r'SPECIES_(?:SPRITE|PAL|SHINY_PAL)\((\w+),\s*(\w+)\)',(folder/name).read_text()))
    front,back,normal,shiny=[table(n) for n in ['front_pic_table.h','back_pic_table.h','palette_table.h','shiny_palette_table.h']]
    def offsets(name):return {k:int(v) for k,v in re.findall(r'\[SPECIES_(\w+)\]\s*=\s*\{[^}]*\.y_offset\s*=\s*(\d+)',(folder/name).read_text())}
    fo,bo=offsets('front_pic_coordinates.h'),offsets('back_pic_coordinates.h')
    elevation={k:int(v) for k,v in re.findall(r'\[SPECIES_(\w+)\]\s*=\s*(\d+)',(folder/'enemy_mon_elevation.h').read_text())}
    source=(root/'src/battle_anim_mons.c').read_text();block=re.search(r'sBattlerCoords\[\]\[MAX_BATTLERS_COUNT\]\s*=\s*\{(.*?)\n\};',source,re.S).group(1)
    anchors=[list(map(int,m)) for m in re.findall(r'\{\s*(\d+),\s*(\d+)\s*\}',block)]
    entries=[]
    for mon in species:
        key=mon['key'];e=dict(mon);e['issues']=[]
        for label,t,suffix in [('front',front,'.png'),('back',back,'.png'),('normal',normal,'.pal'),('shiny',shiny,'.pal')]:
            path=symbols.get(t.get(key,''),'');path=re.sub(r'\.(?:4bpp|gbapal)(?:\.lz)?$',suffix,path)
            e[label]=path
            if not path or not (root/path).is_file():e['issues'].append('Missing '+label+' source')
        e.update(frontOffset=fo.get(key,0),backOffset=bo.get(key,0),elevation=elevation.get(key,0))
        if key=='CASTFORM':
            normal_values=re.findall(r'\[CASTFORM_NORMAL\]\s*=\s*(?:\{[^}]*\.y_offset\s*=\s*)?(\d+)',source)
            if len(normal_values)>=3:e.update(frontOffset=int(normal_values[0]),elevation=int(normal_values[1]),backOffset=int(normal_values[2]))
        if key not in fo or key not in bo:e['issues'].append('Missing coordinate definition')
        entries.append(e)
    return dict(species=entries,anchors=dict(single=anchors[:4],double=anchors[4:8]))


def sprite(root, entry, view, shiny=False):
    data=transparent_png(root/entry[view]);pal=(root/entry['shiny' if shiny else 'normal']).read_text().splitlines()
    colors=[tuple(map(int,line.split())) for line in pal[3:19]]
    if len(colors)!=16:raise ValueError('Expected 16 palette colours')
    # Match GBA 5-bit channel conversion; PNG palette entries retain their indices.
    colors=[tuple((v>>3)*255//31 for v in c) for c in colors]
    result=data[:8];offset=8
    while offset<len(data):
        size=struct.unpack('>I',data[offset:offset+4])[0];kind=data[offset+4:offset+8];chunk=data[offset:offset+12+size]
        if kind==b'PLTE':
            payload=bytes(v for c in colors for v in c)
            # Pad unused PNG palette entries when the PNG declares an 8-bit palette.
            payload+=b'\0'*max(0,size-len(payload));body=kind+payload
            chunk=struct.pack('>I',len(payload))+body+struct.pack('>I',zlib.crc32(body)&0xffffffff)
        result+=chunk;offset+=12+size
    return result


OFFSET_FIELDS = ('frontOffset', 'backOffset', 'elevation')


def save_offsets(root, species, payload):
    """Validate against current source, then replace only the requested numeric fields."""
    key = payload.get('species')
    entry = next((e for e in catalog(root, species)['species'] if e['key'] == key), None)
    if entry is None:
        raise ValueError('Unknown species')
    values = payload.get('values', {})
    expected = payload.get('expected', {})
    for field in OFFSET_FIELDS:
        if type(values.get(field)) is not int or not 0 <= values[field] <= 255:
            raise ValueError('Offsets must be whole numbers from 0 to 255')
        if expected.get(field) != entry[field]:
            raise ValueError('Source offsets changed. Reload source before saving.')
    edits = {}
    def replace(path, pattern, value):
        original = edits.get(path, path.read_text())
        updated, count = re.subn(pattern, lambda m: m[1] + str(value), original)
        if count != 1:
            raise ValueError('Could not uniquely locate source coordinate')
        edits[path] = updated
    if key == 'CASTFORM':
        path = root/'src/battle_anim_mons.c'
        for field, table in zip(OFFSET_FIELDS, ('gCastformFrontSpriteCoords', 'sCastformBackSpriteYCoords', 'sCastformElevations')):
            pattern = r'(' + table + r'\[[^;]*?\[CASTFORM_NORMAL\]\s*=\s*' + (r'\{[^}]*?\.y_offset\s*=\s*' if field == 'frontOffset' else '') + r')(\d+)'
            replace(path, pattern, values[field])
    else:
        folder = root/'src/data/pokemon_graphics'
        for field, name in [('frontOffset', 'front_pic_coordinates.h'), ('backOffset', 'back_pic_coordinates.h')]:
            replace(folder/name, r'(\[SPECIES_' + re.escape(key) + r'\]\s*=\s*\{[^}]*?\.y_offset\s*=\s*)(\d+)', values[field])
        path = folder/'enemy_mon_elevation.h'
        pattern = r'(\[SPECIES_' + re.escape(key) + r'\]\s*=\s*)(\d+)'
        if re.search(pattern, path.read_text()):
            replace(path, pattern, values['elevation'])
        elif values['elevation']:
            edits[path] = path.read_text().replace('\n};', f'\n    [SPECIES_{key}] = {values["elevation"]},\n}};')
    for path, text in edits.items():
        if text != path.read_text():
            temp = path.with_suffix(path.suffix + '.tmp')
            temp.write_text(text)
            temp.replace(path)
    return dict(species=key, values=values, message='Saved to source. Rebuild the ROM to use these offsets in-game.')


def battle_info(root, key):
    """Read the mod's stats and ability text, rather than external species defaults."""
    source = (root/'src/data/pokemon/species_info.h').read_text()
    blocks = dict(re.findall(r'\[SPECIES_(\w+)\]\s*=\s*\{((?:[^{}]|\{[^{}]*\})*)\}', source, re.S))
    block = blocks.get(key, '')
    fields = [('hp','baseHP'), ('attack','baseAttack'), ('defense','baseDefense'),
              ('specialAttack','baseSpAttack'), ('specialDefense','baseSpDefense'), ('speed','baseSpeed')]
    stats = {}
    for label, field in fields:
        value = re.search(r'\.'+field+r'\s*=\s*(\d+)', block)
        stats[label] = int(value[1]) if value else None
    text = (root/'src/data/text/abilities.h').read_text()
    names = dict(re.findall(r'\[(ABILITY_\w+)\]\s*=\s*_\("([^"\n]*)"\)', text))
    descriptions = dict(re.findall(r'static const u8 (\w+)\[\] = _\("([^"\n]*)"\);', text))
    pointers = dict(re.findall(r'\[(ABILITY_\w+)\]\s*=\s*(\w+Description)', text))
    slots = re.search(r'\.abilities\s*=\s*\{([^}]+)\}', block)
    keys = list(dict.fromkeys(re.findall(r'ABILITY_\w+', slots[1]))) if slots else []
    abilities = [dict(key=k, name=names.get(k, k.removeprefix('ABILITY_').replace('_',' ')),
                      description=descriptions.get(pointers.get(k,''), 'Description unavailable.'))
                 for k in keys if k != 'ABILITY_NONE']
    return dict(stats=stats, baseStatTotal=sum(stats.values()) if all(v is not None for v in stats.values()) else None,
                abilities=abilities)


def details(root, entry):
    import json
    from official_encounters import for_species
    key = entry['key']
    text = (root/'src/data/pokemon/pokedex_text_fr.h').read_text()
    texts = {name: ''.join(json.loads(s) for s in re.findall(r'"(?:\\.|[^"\\])*"', body))
             for name, body in re.findall(r'const u8 (\w+)\[\] = _\((.*?)\);', text, re.S)}
    blocks = dict(re.findall(r'\[NATIONAL_DEX_(\w+)\]\s*=\s*\{(.*?)\n    \}', (root/'src/data/pokemon/pokedex_entries.h').read_text(), re.S))
    block = blocks.get(key, '')
    def field(name):
        m = re.search(r'\.'+name+r'\s*=\s*(\w+)', block)
        return m[1] if m else ''
    category = re.search(r'\.categoryName\s*=\s*_\("([^"]*)"\)', block)
    footprints = dict(re.findall(r'\[SPECIES_(\w+)\]\s*=\s*(\w+)', (root/'src/data/pokemon_graphics/footprint_table.h').read_text()))
    assets = dict(re.findall(r'const u\d+ (\w+)\[\] = INCBIN_U\d+\(\s*"([^"]+)"\)', (root/'src/data/graphics/pokemon.h').read_text()))
    footprint = assets.get(footprints.get(key, ''), '').replace('.1bpp', '.png')
    cry_constants = dict(re.findall(r'(CRY_\w+)\s*=\s*(\d+)', (root/'include/constants/hoenn_cries.h').read_text()))
    mapping = dict(re.findall(r'\[SPECIES_(\w+) - HOENN_MON_SPECIES_START\]\s*=\s*(\w+)', (root/'src/data/pokemon/cry_ids.h').read_text()))
    cry_id = entry['id'] - 1
    if entry['id'] >= 277:
        value = mapping.get(key, '0');cry_id = int(cry_constants.get(value, value))
    overrides = re.findall(r'if \(species == SPECIES_(\w+) - 1\)\s*return (\d+);', (root/'src/pokemon.c').read_text())
    if key in dict(overrides):cry_id = int(dict(overrides)[key])
    cry_table = (root/'sound/cry_tables.inc').read_text().split('gCryTable::',1)[1].split('gCryTable_Reverse',1)[0]
    cries = re.findall(r'^\s*cry (\w+)', cry_table, re.M)
    samples = dict(re.findall(r'(Cry_\w+)::\s*\.incbin "([^"]+)"', (root/'sound/direct_sound_data.inc').read_text()))
    cry = samples.get(cries[cry_id], '').replace('.bin','.wav') if cry_id < len(cries) else ''
    edges = []
    evolution = (root/'src/data/pokemon/evolution.h').read_text()
    for parent, body in re.findall(r'\[SPECIES_(\w+)\]\s*=\s*(.*?)(?=\[SPECIES_|\Z)', evolution, re.S):
        for method, parameter, target in re.findall(r'\{(EVO_\w+),\s*(\w+),\s*SPECIES_(\w+)\}', body):
            edges.append(dict(source=parent,target=target,method=method,parameter=parameter))
    family = {key}
    while True:
        expanded = family | {n for e in edges if e['source'] in family or e['target'] in family for n in (e['source'],e['target'])}
        if expanded == family:break
        family = expanded
    palettes = {}
    for kind in ('normal','shiny'):
        colors = [list(map(int,line.split())) for line in (root/entry[kind]).read_text().splitlines()[3:19]]
        palettes[kind] = dict(path=entry[kind],colors=['#'+''.join(f'{(v>>3)*255//31:02x}' for v in c) for c in colors])
    return dict(**battle_info(root, key), officialEncounters=for_species(key),description=texts.get(field('description'),'No entry found.'),category=category[1] if category else '',height=field('height'),weight=field('weight'),footprint=footprint,cry=cry,cryId=cry_id,palettes=palettes,evolutions=[e for e in edges if e['source'] in family])
