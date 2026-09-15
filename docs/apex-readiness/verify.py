from pathlib import Path
import re,runpy
root=Path(__file__).resolve().parents[2]
entries=[]
for p in (root/'data/maps').glob('*/scripts.inc'):
 s=p.read_text()
 for match in re.finditer(r'(?m)^(\w+)::\n((?:(?!^\w+::).)*?special StartApexBattle.*?)\n(?=\w+::|\Z)',s,re.S):
  label,block=match.groups();species,level=re.search(r'seteventmon SPECIES_(\w+), (\d+)',block).groups()
  assert f'setvar VAR_0x8004, {level}\n\tcall EventScript_ConfirmApexEncounter\n\tgoto_if_eq VAR_RESULT, NO, EventScript_DeclineApexEncounter' in block,label
  pos=block.index('call EventScript_ConfirmApexEncounter')
  for event in ['seteventmon','call EventScript_ApexEncounterShake','setflag FLAG_INTERACTED_APEX','special StartApexBattle']:
   assert pos<block.index(event),(label,event)
  entries.append((label,species,int(level)))
assert len(entries)==8,entries
s=(root/'data/scripts/static_pokemon.inc').read_text()
body=s.split('EventScript_DeclineApexEncounter::')[1].split('Text_ApexEncounterReadiness::')[0]
assert body.split()==['release','end']
font=runpy.run_path(str(root/'docs/league-terminals/verify.py'))
text=s.split('Text_ApexEncounterReadiness::')[1].split('EventScript_ApexEncounterShake::')[0]
for chunk in re.findall(r'\.string "(.*)"',text):
 for line in re.split(r'\\[npl]|\$',chunk.replace('{STR_VAR_1}','70')):
  assert sum(font['w'][font['chars'][c]]for c in line)<=208,line
print('PASS: all eight Apex entries confirm before side effects; decline exits; prompt fits.')
