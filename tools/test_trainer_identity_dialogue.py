#!/usr/bin/env python3
"""Regression checks for field trainer identity, appearance and dialogue wiring.

Uses compiled event inclusion / map reachability, resolves numeric aliases, and
checks script-selected league appearances separately from ordinary map objects.
"""
import json
import re
from pathlib import Path
from audit_trainer_class_heatmap import definitions, resolve
from audit_trainer_dialogue import collect
from audit_trainer_overworlds import parse_trainers

ROOT = Path(__file__).resolve().parents[1]

def main():
    data = collect()
    assert not data['missing_scripts'], data['missing_scripts']
    assert not data['missing_texts'], data['missing_texts']
    assert not data['pointer_errors'], data['pointer_errors']
    assert not data['rematch_identity_mismatches'], data['rematch_identity_mismatches']
    defs = definitions(ROOT / 'include/constants/opponents.h')
    classes = definitions(ROOT / 'include/constants/trainers.h')
    trainers = {resolve(k, defs):v for k,v in parse_trainers().items()}
    for shared in data['shared_flags_across_maps']:
        assert trainers[shared['id']]['class'] == 'TRAINER_CLASS_LEADER', shared
    fixes = json.loads((ROOT/'docs/trainer-dialogue-audit/identity-fixes.json').read_text())
    assert len({resolve(f['token'],defs) for f in fixes}) == len(fixes) == 20
    assert resolve('MAX_TRAINERS_COUNT', defs) == 792  # Preserve existing system flag offsets.
    reserved = {329,330,331,435,436,437,745,746,747}
    assert not reserved.intersection(f['id'] for f in fixes)
    for f in fixes:
        assert resolve(f['token'],defs) == f['id']

    # Match front portraits to actual overworld identities, without accepting a
    # different gender/activity just because its broad trainer class matches.
    front_to_ow = {
        'FISHERMAN': {'FISHER'}, 'BLACK_BELT_M': {'BLACK_BELT'},
        'BLACK_BELT_F': {'CRUSH_GIRL'}, 'POKEMANIAC': {'POKE_MANIAC'},
        'POKEMON_BREEDER': {'BREEDER','POKEMON_BREEDER'},
        'RIVAL_EARLY': {'BLUE'}, 'RIVAL_LATE': {'BLUE'}, 'CHAMPION_RIVAL': {'BLUE'},
        'ROCKET_GRUNT_M': {'ROCKET_M'}, 'ROCKET_GRUNT_F': {'ROCKET_F'},
        'SIS_AND_BRO': {'SWIMMER_F_WATER','SWIMMER_F_LAND','TUBER_M_WATER','TUBER_M_LAND'},
        'CRUSH_KIN': {'BLACK_BELT','CRUSH_GIRL'},
        # FRLG's shared two-person front is explicitly aliased for Trendsetters.
        'ACES': {'ACE_TRAINER_M','ACE_TRAINER_F','TRENDSETTER_M','TRENDSETTER_F'},
    }
    for role in ['SWIMMER_M','SWIMMER_F','TUBER_M','TUBER_F']:
        front_to_ow[role] = {role+'_LAND',role+'_WATER'}
    for name in ['BROCK','MISTY','LT_SURGE','ERIKA','KOGA','BLAINE','SABRINA','GIOVANNI']:
        front_to_ow['LEADER_'+name] = {name}
    for name in ['LORELEI','BRUNO','AGATHA','LANCE']:
        front_to_ow['ELITE_FOUR_'+name] = {name}
    for name in ['ARCHER','ARIANA','PROTON','PETREL']:
        front_to_ow['ROCKET_ADMIN_'+name] = {'ROCKET_'+name}

    class_front = {
        'TRIATHLETE': {f'TRIATHLETE_{sex}_{activity}' for sex in ['M','F'] for activity in ['LAND','WATER','CYCLING']},
        'SCOUT': {'SCOUT_M','SCOUT_F'}, 'BLACK_BELT': {'BLACK_BELT_M','BLACK_BELT_F'},
        'EXPERT': {'EXPERT_M','EXPERT_F'}, 'POKEFAN': {'POKEFAN_M','POKEFAN_F'},
        'TUBER': {'TUBER_M','TUBER_F'}, 'SWIMMER': {'SWIMMER_M','SWIMMER_F'},
        'PSYCHIC': {'PSYCHIC_M','PSYCHIC_F'}, 'ACE_TRAINER': {'ACE_TRAINER_M','ACE_TRAINER_F'},
        'PKMN_BREEDER': {'POKEMON_BREEDER'}, 'PKMN_RANGER': {'POKEMON_RANGER_M','POKEMON_RANGER_F'},
        'TEAM_ROCKET': {'ROCKET_GRUNT_M','ROCKET_GRUNT_F'}, 'ROCKET_ACE': {'ROCKET_GRUNT_M','ROCKET_GRUNT_F'},
        'ROCKET_ADMIN': {'ROCKET_ADMIN_'+n for n in ['ARCHER','ARIANA','PROTON','PETREL']},
        'LEADER': {'LEADER_'+n for n in ['BROCK','MISTY','LT_SURGE','ERIKA','KOGA','BLAINE','SABRINA','GIOVANNI']},
        'BOSS': {'LEADER_GIOVANNI'}, 'ELITE_FOUR': {'ELITE_FOUR_'+n for n in ['LORELEI','BRUNO','AGATHA','LANCE']},
        'CHAMPION': {'CHAMPION_RIVAL','LEADER_GIOVANNI'}, 'TRENDSETTERS': {'ACES'},
    }
    active_ids = {tid for e in data['encounters'] for tid in e['trainer_ids']}
    active_ids.update(resolve(f['token'],defs) for f in fixes)
    fronts = (ROOT/'src/data/trainer_graphics/front_pic_tables.h').read_text()
    for tid in active_ids:
        t = trainers[tid]; cls=t['class'].removeprefix('TRAINER_CLASS_'); front=t['pic'].removeprefix('TRAINER_PIC_')
        assert front in class_front.get(cls,{cls}), (tid,t)
        assert resolve(t['pic'], classes) in {resolve('TRAINER_PIC_' + p, classes) for p in re.findall(r'TRAINER_SPRITE\((\w+),', fronts)}, t

    checked=set()
    for e in data['encounters']:
        # The arena's NPCs are shared actors, changed by the chosen battle branch.
        if e['map']=='RocketLeague_Arena': continue
        map_data=json.loads((ROOT/f"data/maps/{e['map']}/map.json").read_text())
        for i,obj in enumerate(map_data.get('object_events',[]),1):
            if obj.get('script') not in e['roots']: continue
            if obj['graphics_id'].startswith('OBJ_EVENT_GFX_VAR_'): continue
            actual=obj['graphics_id'].removeprefix('OBJ_EVENT_GFX_')
            for tid in e['trainer_ids']:
                front=trainers[tid]['pic'].removeprefix('TRAINER_PIC_')
                assert actual in front_to_ow.get(front,{front}), (e['map'],i,trainers[tid],actual)
            checked.add((e['map'],i))

    def blocks(path):
        return {m[1]:m[2] for m in re.finditer(r'^(\w+)::\n(.*?)(?=^\w+::|\Z)',path.read_text(),re.M|re.S)}
    rocket=blocks(ROOT/'data/maps/RocketLeague_Arena/scripts.inc')
    league=blocks(ROOT/'data/maps/PokemonLeague_BrunosRoom/scripts.inc')
    for block in league.values():
        if 'trainerbattle_no_intro_double' not in block:continue
        tid=resolve(re.search(r'trainerbattle_no_intro_double (\w+)',block)[1],defs)
        gfx=re.search(r'setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_(\w+)',block)[1]
        front=trainers[tid]['pic'].removeprefix('TRAINER_PIC_')
        assert gfx in front_to_ow.get(front,{front}), (tid,gfx)
    ace_intros=set(); ace_losses=set()
    for name,block in rocket.items():
        if 'trainerbattle_no_intro ' not in block:continue
        token=re.search(r'trainerbattle_no_intro (\w+)',block)[1]
        front=trainers[resolve(token,defs)]['pic'].removeprefix('TRAINER_PIC_')
        if token.endswith('PETREL'):
            assert 'EventScript_PetrelEnter' in block
            continue
        gfx=re.search(r'setobjectgfx \w+, OBJ_EVENT_GFX_(\w+)',block)[1]
        assert gfx in front_to_ow.get(front,{front}), (token,gfx)
        if token.startswith('TRAINER_ROCKET_ACE_'):
            ace_intros.add(re.search(r'msgbox (\w+Intro)',block)[1])
            ace_losses.add(re.search(r'trainerbattle_no_intro \w+, (\w+)',block)[1])
    assert len(ace_intros)==len(ace_losses)==8

    # New dialogue must fit the actual Latin field fonts, not just a character limit.
    font_source=(ROOT/'src/text.c').read_text()
    widths=[]
    for name in ['Male','Female','Normal']:
        body=re.search(r'sFont'+name+r'LatinGlyphWidths\[\]\s*=\s*\{(.*?)\};',font_source,re.S)[1]
        widths.append([int(v) for v in re.findall(r'\d+',body)])
    charmap={m[1]:int(m[2],16) for m in re.finditer(r"^'(.)'\s*=\s*([0-9A-F]{2})\s*$",(ROOT/'charmap.txt').read_text(),re.M)}
    charmap["'"] = 0xB4  # Charmap spells the ASCII apostrophe as an escaped quote.
    max_width=0; labels=0
    for fn in ['dialogue-fixes.json','league-voices.json']:
        for label,change in json.loads((ROOT/'docs/trainer-dialogue-audit'/fn).read_text()).items():
            source=(ROOT/change['file']).read_text()
            text=''.join(re.findall(r'\.string "(.*)"',re.search(r'^'+re.escape(label)+r'::?\s*\n(.*?)(?=^\w+::?|\Z)',source,re.M|re.S)[1]))
            assert text==change['after'],label
            assert text.endswith('$'), label
            for line in re.split(r'\\[nlp]|\$',text):
                for font in widths:
                    w=sum(font[charmap[c]] for c in line)
                    assert w<=208,(label,w,line)
                    max_width=max(max_width,w)
            labels+=1
    print(f'PASS: {len(checked)} placed trainer objects, class/front identities, all league actors, 20 dedicated IDs, rematch identities, {labels} edited texts (max {max_width}px).')

if __name__=='__main__':main()
