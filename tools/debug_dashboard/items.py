"""Item descriptions, behavior and source evidence for the dashboard."""
import json
import re


INTENTIONALLY_UNAVAILABLE = {
    'ITEM_PATCH_DISC': 'Reserved for a future PORYGON upgrade; not distributed yet.',
    'ITEM_DAWN_STONE': 'Reserved for JOHTO Route 34 and its wild RALTS; not distributed in KANTO.',
}

def catalog(root):
    source=(root/'src/data/items.h').read_text()
    strings={name: ''.join(json.loads(s) for s in re.findall(r'"(?:\\.|[^"\\])*"',body)) for name,body in re.findall(r'const u8 (\w+)\[\]\s*=\s*_\((.*?)\);',source+'\n'+(root/'src/move_descriptions.c').read_text(),re.S)}
    references={}
    excluded={'src/data/items.h','src/data/item_icon_table.h','src/data/item_effects.h'}
    for folder in ('data','src'):
        for path in sorted((root/folder).rglob('*')):
            if path.suffix not in ('.c','.h','.inc','.json') or not path.is_file():continue
            relative=path.relative_to(root).as_posix()
            if relative in excluded or relative.startswith('src/data/items.') or 'graphics' in relative or 'debug_dashboard' in relative:continue
            for number,line in enumerate(path.read_text(errors='replace').splitlines(),1):
                line=line.split('//')[0]
                if line.lstrip().startswith(('@','/*','*')):continue
                for key in set(re.findall(r'\bITEM_[A-Z0-9_]+\b',line)):
                    references.setdefault(key,[]).append(dict(path=relative,line=number,text=line.strip()))
    evolution=(root/'src/data/pokemon/evolution.h').read_text()
    evolutions={}
    for parent,body in re.findall(r'\[SPECIES_(\w+)\]\s*=\s*(.*?)(?=\[SPECIES_|\Z)',evolution,re.S):
        for method,item,target in re.findall(r'\{(EVO_\w+),\s*(ITEM_\w+),\s*SPECIES_(\w+)\}',body):
            evolutions.setdefault(item,[]).append(f'{parent.replace("_"," ")} → {target.replace("_"," ")} ({method.removeprefix("EVO_").replace("_"," ").lower()})')
    from item_availability import acquisition_sources
    acquisition=acquisition_sources(root)
    icons=dict((key,(pic,pal)) for key,pic,pal in re.findall(r'\[(ITEM_\w+)\]\s*=\s*\{(\w+),\s*(\w+)\}',(root/'src/data/item_icon_table.h').read_text()))
    assets=dict(re.findall(r'const u\d+ (\w+)\[\]\s*=\s*INCBIN_U\d+\("([^"]+)"\)',(root/'src/data/graphics/items.h').read_text()))
    result=[]
    for key,body in re.findall(r'\[(ITEM_\w+)\]\s*=\s*\{(.*?)\}',source,re.S):
        if key=='ITEM_NONE':continue
        values=dict(re.findall(r'\.(\w+)\s*=\s*([^,\n]+)',body))
        name=re.search(r'\.name\s*=\s*_\("([^"]+)"\)',body)
        if not name or name[1]=='????????':continue
        field=values.get('fieldUseFunc','NULL').strip();battle=values.get('battleUseFunc','NULL').strip();held=values.get('holdEffect','HOLD_EFFECT_NONE').strip()
        usage=[]
        description_symbol=values.get('description','').strip()
        if description_symbol.startswith('gMoveDescription_'):usage.append('Teaches '+re.sub(r'(?<!^)(?=[A-Z])',' ',description_symbol.removeprefix('gMoveDescription_'))+' to a compatible Pokémon')
        if field not in ('NULL','FieldUseFunc_OakStopsYou'):
            label=re.sub(r'(?<!^)(?=[A-Z])',' ',field.removeprefix('FieldUseFunc_'))
            usage.append('Field: '+label)
        if values.get('battleUsage','0').strip()!='0':usage.append('Battle: '+re.sub(r'(?<!^)(?=[A-Z])',' ',battle.removeprefix('BattleUseFunc_')))
        if held!='HOLD_EFFECT_NONE':usage.append('Held: '+held.removeprefix('HOLD_EFFECT_').replace('_',' ').lower()+' (parameter '+values.get('holdEffectParam','0').strip()+')')
        usage.extend(evolutions.get(key,[]))
        if values.get('registrability','0').strip()!='0':usage.append('Can be registered for quick use')
        if not usage:usage.append('No direct bag/battle action; see description and script references.')
        refs=references.get(key,[])
        pic,pal=icons.get(key,('',''))
        icon=re.sub(r'\.4bpp(?:\.lz)?$', '.png',assets.get(pic,''))
        palette=re.sub(r'\.gbapal(?:\.lz)?$', '.pal',assets.get(pal,''))
        sources=acquisition.get(key,[])
        availability='source' if sources else 'unconfirmed'
        if key in INTENTIONALLY_UNAVAILABLE and not sources:availability='unavailable'

        result.append(dict(key=key,name=name[1],description=strings.get(values.get('description','').strip(),'No description found.'),pocket=values.get('pocket','').strip().removeprefix('POCKET_').replace('_',' '),price=values.get('price','0').strip(),usage=usage,references=refs,referenced=bool(refs),icon=icon,iconPalette=palette,availability=availability,availabilityNote=INTENTIONALLY_UNAVAILABLE.get(key,''),acquisition=sources,handlers=dict(field=field,battle=battle,held=held)))
    return dict(items=result)


def icon(root,key):
    from pokedex import sprite
    if not re.fullmatch(r'ITEM_\w+',key):raise ValueError('Invalid item')
    match=re.search(r'\['+key+r'\]\s*=\s*\{(\w+),\s*(\w+)\}',(root/'src/data/item_icon_table.h').read_text())
    if not match:raise ValueError('No item icon')
    assets=dict(re.findall(r'const u\d+ (\w+)\[\]\s*=\s*INCBIN_U\d+\("([^"]+)"\)',(root/'src/data/graphics/items.h').read_text()))
    pic=re.sub(r'\.4bpp(?:\.lz)?$', '.png',assets[match[1]])
    pal=re.sub(r'\.gbapal(?:\.lz)?$', '.pal',assets[match[2]])
    return sprite(root,dict(front=pic,normal=pal),'front')
