"""Conservative acquisition evidence: follow map-linked script labels, not item mentions."""
import json
import re


def acquisition_sources(root):
    result={}
    def add(item,kind,path,line,text):
        if item!='ITEM_NONE':result.setdefault(item,[]).append(dict(kind=kind,path=str(path),line=line,text=text.strip()))
    blocks={}; roots=set()
    for path in sorted((root/'data').rglob('*.inc')):
        label=None
        for n,line in enumerate(path.read_text().splitlines(),1):
            match=re.match(r'^(\w+)::?',line)
            if match:label=match[1];blocks[label]=[]
            if label:blocks[label].append((path.relative_to(root).as_posix(),n,line.split('@')[0]))
    for path in sorted((root/'data/maps').glob('*/map.json')):
        data=json.loads(path.read_text());relative=path.relative_to(root).as_posix()
        def visit(obj):
            if isinstance(obj,dict):
                if obj.get('type')=='hidden_item' and str(obj.get('item','')).startswith('ITEM_'):
                    add(obj['item'],'Hidden item',relative,0,f"{path.parent.name}: ({obj.get('x')}, {obj.get('y')})")
                for value in obj.values():visit(value)
            elif isinstance(obj,list):
                for value in obj:visit(value)
            elif isinstance(obj,str) and obj in blocks:roots.add(obj)
        visit(data)
        roots.add(path.parent.name+'_MapScripts')
    reachable=set();pending=list(roots)
    while pending:
        label=pending.pop()
        if label in reachable or label not in blocks:continue
        reachable.add(label)
        for _,_,line in blocks[label]:
            pending.extend(t for t in re.findall(r'\b\w+\b',line) if t in blocks and t not in reachable)
    shops=set()
    for label in reachable:
        for _,_,line in blocks[label]:
            shops.update(re.findall(r'\bpokemart\s+(\w+)',line))
    for label in sorted(reachable):
        for path,n,line in blocks[label]:
            for item in re.findall(r'\b(?:giveitem|additem|finditem)\s+(ITEM_\w+)',line):add(item,'Gift / pickup',path,n,line)
            for item in re.findall(r'\bgiveitem_msg\s+\w+,\s*(ITEM_\w+)',line):add(item,'Gift',path,n,line)
            # Reviewed variable-driven distribution scripts; checks/consumption alone do not count.
            reviewed={'data/maps/CeladonCity_DepartmentStore_Roof/scripts.inc':'VAR_TEMP_0',
                      'data/maps/CeladonCity_BerryPatch/scripts.inc':'VAR_0x8008',
                      'data/maps/CeladonCity_GameCorner_PrizeRoom/scripts.inc':'VAR_TEMP_1'}
            if path in reviewed:
                for item in re.findall(r'\bsetvar\s+'+reviewed[path]+r',\s*(ITEM_\w+)',line):add(item,'Scripted purchase / harvest',path,n,line)
            if label in shops:
                for item in re.findall(r'\.2byte\s+(ITEM_\w+)',line):add(item,'Shop',path,n,line)
    # Literal C awards are separate evidence; never treat generic checks as awards.
    for path in sorted((root/'src').glob('*.c')):
        if 'debug' in path.name:continue
        for n,line in enumerate(path.read_text().splitlines(),1):
            for item in re.findall(r'\bAddBagItem\(\s*(ITEM_\w+)',line.split('//')[0]):
                add(item,'Engine award',path.relative_to(root).as_posix(),n,line)
    # Page rewards choose the candy via a tier, then award the variable.
    path=root/'src/pokedex_screen.c'
    for n,line in enumerate(path.read_text().splitlines(),1):
        if 'u16 candy = tier' in line:
            for item in re.findall(r'ITEM_EXP_CANDY_\w+',line):add(item,'Pokédex page reward',path.relative_to(root).as_posix(),n,line)
    for filename,table,kind in [('src/shop.c','sRocketCoinTms','Rocket League coin shop'),('src/battle_script_commands.c','sPickupItems','Pickup ability')]:
        text=(root/filename).read_text()
        block=re.search(r'\b'+table+r'\[\]\s*=\s*\{(.*?)\n\};',text,re.S)
        if block:
            for m in re.finditer(r'ITEM_\w+',block[1]):
                add(m[0],kind,filename,text[:block.start(1)+m.start()].count('\n')+1,table)
    wild=set(re.findall(r'SPECIES_\w+',(root/'src/data/wild_encounters.json').read_text()))
    path=root/'src/data/pokemon/species_info.h';text=path.read_text()
    for match in re.finditer(r'\[(SPECIES_\w+)\]\s*=\s*\{(.*?)\n    \}',text,re.S):
        if match[1] not in wild:continue
        for m in re.finditer(r'\.item(?:Common|Rare)\s*=\s*(ITEM_\w+)',match[2]):
            add(m[1],'Wild held item',path.relative_to(root).as_posix(),text[:match.start(2)+m.start()].count('\n')+1,match[1])
    return result
