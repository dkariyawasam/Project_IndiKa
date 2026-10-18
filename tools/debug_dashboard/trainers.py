"""Trainer reference and conflict-checked party editing from repository source."""
import hashlib
import json
import re
from datetime import datetime

PARTIES='src/data/trainer_parties.h'
def digest(text):return hashlib.sha256(text.encode()).hexdigest()
def parties(text):
    return {m[2]:m for m in re.finditer(r'static const struct (TrainerMon\w+) (\w+)\[\] = \{(.*?)\n\};',text,re.S)}
def team(body):
    result=[]
    for b in re.findall(r'\{([^{}]*(?:\{[^{}]*\}[^{}]*)*)\}',body,re.S):
        def field(n):
            m=re.search(r'\.'+n+r'\s*=\s*(\w+)',b)
            return m[1] if m else None
        if not field('species'):continue
        moves=re.search(r'\.moves\s*=\s*\{([^}]+)',b)
        result.append(dict(species=field('species'),level=int(field('lvl')),iv=int(field('iv')),item=field('heldItem'),moves=re.findall(r'MOVE_\w+',moves[1]) if moves else None))
    return result

def catalog(root):
    source=(root/PARTIES).read_text();ps=parties(source)
    definitions=(root/'src/data/trainers.h').read_text()
    labels={};locations={}
    for path in (root/'data/maps').glob('*/scripts.inc'):
        text=path.read_text()
        for key in set(re.findall(r'\bTRAINER_\w+',text)):
            locations.setdefault(key,[]).append(path.parent.name)
        for match in re.finditer(r'^(\w+)::?\n(.*?)(?=^\w+::?|\Z)',text,re.M|re.S):labels[match[1]]=match[2]
    for path in (root/'data/maps').glob('*/text.inc'):
        for match in re.finditer(r'^(\w+)::?\n(.*?)(?=^\w+::?|\Z)',path.read_text(),re.M|re.S):labels[match[1]]=match[2]
    linked={}
    for label,script in labels.items():
        for key in set(re.findall(r'\bTRAINER_\w+',script)):
            linked.setdefault(key,[]).append((label,script))
    objects={}
    for path in (root/'data/maps').glob('*/map.json'):
        for obj in json.loads(path.read_text()).get('object_events',[]):
            objects.setdefault(obj.get('script'),[]).append(obj.get('graphics_id'))
    from overworlds import catalog as overworld_catalog
    ow={e['id']:e for e in overworld_catalog(root)}
    pointers=dict(re.findall(r'\[(OBJ_EVENT_GFX_\w+)\]\s*=\s*&(\w+)',(root/'src/data/object_events/object_event_graphics_info_pointers.h').read_text()))
    entries=[]
    for key,body in re.findall(r'\[(TRAINER_\w+)\]\s*=\s*\{(.*?)\n    \}',definitions,re.S):
        party=re.search(r'\.party\s*=\s*(\w+)\((\w+)\)',body)
        if not party or party[2] not in ps:continue
        name=re.search(r'\.trainerName\s*=\s*_\("([^"]*)"\)',body)
        pic=re.search(r'\.trainerPic\s*=\s*(\w+)',body)
        cls=re.search(r'\.trainerClass\s*=\s*(\w+)',body)
        dialogue=[]
        graphics=set()
        for label,script in linked.get(key,[]):
            graphics.update(objects.get(label,[]))
            graphics.update(re.findall(r'setobjectgfx[^\n]*?\b(OBJ_EVENT_GFX_\w+)',script))
            refs=re.findall(r'\b\w*Text_\w+\b',script)
            for ref in dict.fromkeys(refs):
                if ref in labels:
                    strings=re.findall(r'"(?:\\.|[^"\\])*"',labels[ref])
                    text=''.join(json.loads(t.replace(r'\p', r'\n\n').replace(r'\l', r'\n')) for t in strings).replace('\\p','\n\n').replace('\\l','\n').rstrip('$')
                    dialogue.append(dict(label=ref,text=text.replace(r'\n','\n')))
        m=ps[party[2]]
        entries.append(dict(key=key,name=name[1] if name else key,trainerClass=cls[1] if cls else '',picture=pic[1] if pic else '',party=party[2],format=party[1],team=team(m[3]),revision=digest(m[0]),locations=locations.get(key,[]),dialogue=dialogue,overworlds=[ow[pointers[g]] for g in sorted(graphics) if g in pointers and pointers[g] in ow]))
    active=set(re.findall(r'\[NATIONAL_DEX_(\w+)\] = TRUE',(root/'src/data/pokemon/active_pokedex.h').read_text()))
    result=dict(trainers=entries,options={kind:sorted(set(re.findall(r'#define ('+prefix+r'\w+)\s+\d+',(root/path).read_text()))) for kind,prefix,path in [('species','SPECIES_','include/constants/species.h'),('moves','MOVE_','include/constants/moves.h'),('items','ITEM_','include/constants/items.h')]})
    result['options']['species']=[s for s in result['options']['species'] if s.removeprefix('SPECIES_') in active]
    return result

def save(root,payload):
    data=catalog(root);entry=next((t for t in data['trainers'] if t['key']==payload.get('trainer')),None)
    if not entry:raise ValueError('Unknown trainer')
    if payload.get('revision')!=entry['revision']:raise ValueError('Party changed on disk. Reload before saving.')
    rows=payload.get('team')
    if not isinstance(rows,list) or not 1<=len(rows)<=6:raise ValueError('Choose one to six Pokémon')
    if not entry['team']:raise ValueError('Unsupported party format')
    has_items='ITEM_' in entry['format'] and not entry['format'].startswith('NO_ITEM')
    custom='CUSTOM' in entry['format']
    for row in rows:
        if not isinstance(row,dict):raise ValueError('Invalid party row')
        if row.get('species') not in data['options']['species'] or row['species'] in ('SPECIES_NONE','SPECIES_EGG'):raise ValueError('Invalid species')
        for k,lo,hi in [('level',1,100),('iv',0,255)]:
            if type(row.get(k)) is not int or not lo<=row[k]<=hi:raise ValueError('Invalid '+k)
        if has_items and row.get('item') not in data['options']['items']:raise ValueError('Invalid held item')
        if custom and (not isinstance(row.get('moves'),list) or len(row['moves'])!=4 or any(m not in data['options']['moves'] for m in row['moves']) or all(m=='MOVE_NONE' for m in row['moves'])):raise ValueError('Choose four move slots, including at least one move')
    path=root/PARTIES;source=path.read_text();m=parties(source)[entry['party']]
    if digest(m[0])!=payload['revision']:raise ValueError('Party changed during save')
    body='\n'
    for row in rows:
        body+='    {\n        .iv = '+str(row['iv'])+',\n        .lvl = '+str(row['level'])+',\n        .species = '+row['species']+',\n'
        if has_items:body+='        .heldItem = '+row['item']+',\n'
        if custom:body+='        .moves = {'+', '.join(row['moves'])+'},\n'
        body+='    },\n'
    updated=source[:m.start(3)]+body.rstrip('\n')+source[m.end(3):]
    backup=root/'.cache/debug-dashboard/source-backups'/datetime.now().strftime('%Y%m%d-%H%M%S-%f');backup.mkdir(parents=True)
    (backup/'trainer_parties.h').write_text(source)
    temp=path.with_suffix('.h.tmp');temp.write_text(updated);temp.replace(path)
    return dict(message='Team saved. Rebuild the ROM to use it in-game.',backup=str(backup),sharedWith=[t['name'] for t in data['trainers'] if t['party']==entry['party'] and t['key']!=entry['key']])

def portrait(root,key):
    entry=next((t for t in catalog(root)['trainers'] if t['key']==key),None)
    if not entry:raise ValueError('Unknown trainer')
    pic=entry['picture'].removeprefix('TRAINER_PIC_')
    table=(root/'src/data/trainer_graphics/front_pic_tables.h').read_text()
    graphics=(root/'src/data/graphics/trainers.h').read_text()
    symbols=dict(re.findall(r'const u32 (\w+)\[\] = INCBIN_U32\("([^"]+)"\)',graphics))
    front=re.search(r'TRAINER_SPRITE\('+pic+r',\s*(\w+)',table)
    pal=re.search(r'TRAINER_PAL\('+pic+r',\s*(\w+)',table)
    if not front or not pal:raise ValueError('Portrait unavailable')
    from pokedex import sprite
    return sprite(root,dict(front=symbols[front[1]].replace('.4bpp.lz','.png'),normal=symbols[pal[1]].replace('.gbapal.lz','.pal')),'front')
