#!/usr/bin/env python3
"""Exercise Bill's actual script branches with mocked engine commands."""
from pathlib import Path
import re, json
R=Path(__file__).resolve().parents[1]
s=(R/'data/maps/Route25_SeaCottage/scripts.inc').read_text()
labels={m[1]:m[2].strip().splitlines() for m in re.finditer(r'^(\w+)::\n(.*?)(?=^\w+::|\Z)',s,re.M|re.S)}
prefix='Route25_SeaCottage_EventScript_'
def run(won=False, claimed=False, accept=True, victory=True, space=0):
    flags={'FLAG_GOT_EEVEE'} if claimed else set()
    label=prefix+'Challenge';result=0;events=[]
    for _ in range(100):
        jump=None
        for line in labels[label]:
            cmd,_,rest=line.strip().partition(' ')
            a=[v.strip() for v in rest.split(',')]
            if cmd=='goto_if_defeated' and won:jump=a[1]
            elif cmd=='goto_if_set' and a[0] in flags:jump=a[1]
            elif cmd=='msgbox':
                if a[0].endswith('BattleOffer'):result=int(accept)
                elif a[0]=='Text_GiveNicknameToThisMon':result=0
            elif cmd=='goto_if_eq':
                value={'NO':0}.get(a[1],a[1])
                if result==int(value):jump=a[2]
            elif cmd=='trainerbattle_no_intro':
                events.append('battle')
                if not victory:return flags,events
                won=True
            elif cmd=='givemon':events.append('give');result=space
            elif cmd=='special':events.append(rest)
            elif cmd=='setflag':flags.add(rest)
            elif cmd=='call':events.append(rest)
            elif cmd=='goto':jump=rest
            elif cmd=='end':return flags,events
            if jump:
                if jump=='EventScript_NoMoreRoomForPokemon':return flags,events
                label=jump;break
        else:raise AssertionError(label)
    raise AssertionError('script did not terminate')
for space,special in [(0,'SetPartyMonHighFriendship'),(1,'SetBoxMonHighFriendship')]:
    flags,events=run(space=space)
    assert 'FLAG_GOT_EEVEE' in flags and special in events
    assert events.count('battle')==events.count('give')==1
    if space==1:assert 'EventScript_TransferredToPC' in events
assert run(accept=False)[1]==[]
assert run(victory=False)[1]==['battle']
assert 'FLAG_GOT_EEVEE' not in run(space=2)[0]
assert 'battle' not in run(won=True,space=0)[1] # Retry gift without repeating battle.
assert 'give' not in run(won=True,claimed=True)[1]
assert 'givemon SPECIES_EEVEE, 8' in s
assert 'LEVEL_UP_MOVE(9, MOVE_BABY_DOLL_EYES)' in (R/'src/data/pokemon/level_up_learnsets.h').read_text()
assert 'goto_if_unset FLAG_GOT_SS_TICKET' in s
roof=R/'data/maps/CeladonCity_Condominiums_RoofRoom'
assert 'givemon' not in (roof/'scripts.inc').read_text()
assert all(o.get('local_id')!='LOCALID_EEVEE_POKEBALL' for o in json.loads((roof/'map.json').read_text())['object_events'])
party=(R/'src/data/trainer_parties.h').read_text().split('sParty_Bill[] = {')[1].split('\n};')[0]
assert re.findall(r'\.species = SPECIES_(\w+)',party)==['FLAREON','VAPOREON','JOLTEON','LEAFEON','GLACEON']
print('PASS: Bill victory/loss/decline, party/PC/full storage/retry, one-time gift, level and team, removed rooftop gift')
