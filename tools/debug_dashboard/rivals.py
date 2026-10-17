"""Rival placements and script branches read directly from map/event sources."""
import json
import re


def catalog(root):
    blocks = {}
    for path in sorted((root / 'data').rglob('*.inc')):
        text = path.read_text()
        labels = list(re.finditer(r'^(\w+)::?\s*$', text, re.M))
        for i, match in enumerate(labels):
            body = text[match.end():labels[i+1].start() if i+1 < len(labels) else len(text)]
            blocks[match[1]] = (body, str(path.relative_to(root)), text[:match.start()].count('\n') + 1)
    encounters = []
    for path in sorted((root / 'data/maps').glob('*/map.json')):
        data = json.loads(path.read_text())
        for index, obj in enumerate(data.get('object_events', [])):
            if obj.get('graphics_id') != 'OBJ_EVENT_GFX_BLUE':
                continue
            name = path.parent.name
            # Include coordinate/table entry points even where the rival object has no talk script.
            seeds = {obj.get('script', '')}
            triggers = [e for e in data.get('coord_events', []) if 'Rival' in e.get('script', '') or 'RIVAL' in e.get('var', '')]
            seeds.update(e['script'] for e in triggers)
            seeds.update(k for k in blocks if k.startswith(name + '_') and 'Rival' in k and '_Text_' not in k and '_Movement_' not in k)
            if name == 'PokemonLeague_ChampionsRoom':
                seeds.add(name + '_EventScript_EnterRoom')
            pending = list(seeds); seen = set(); branches = []; battles = set(); state = set()
            while pending:
                label = pending.pop()
                if label in seen or label not in blocks:
                    continue
                seen.add(label)
                body, source, line = blocks[label]
                for offset, raw in enumerate(body.splitlines(), 1):
                    code = raw.strip().split('@')[0].strip()
                    if not code or code.startswith('.string'):
                        continue
                    if code.startswith(('goto_if_', 'call_if_', 'case ', 'switch ')):
                        branches.append(dict(script=label, condition=code, source=source, line=line+offset))
                    battles.update(re.findall(r'\bTRAINER_(?:RIVAL|CHAMPION)_\w+', code))
                    state.update(re.findall(r'\b(?:VAR|FLAG)_\w+', code))
                    # Follow only this map's flow, not shared scripts or text/movement data.
                    if code.startswith(('goto', 'call', 'case ', 'trainerbattle')):
                        pending.extend(k for k in re.findall(r'\b\w+\b', code) if (k.startswith(name+'_') or k.startswith('EventScript_Rival')) and '_Text_' not in k and '_Movement_' not in k and k in blocks)
            encounters.append(dict(map=name, mapId=data['id'], x=obj['x'], y=obj['y'],
                localId=obj.get('local_id',index+1), hiddenBy=obj.get('flag','0'),
                interaction=obj.get('script','0x0'), triggers=triggers,
                branches=sorted(branches,key=lambda b:(b['source'],b['line'])), battles=sorted(battles),
                state=sorted(state), source=str(path.relative_to(root))))
    return dict(encounters=encounters, sourceOnly=True)
