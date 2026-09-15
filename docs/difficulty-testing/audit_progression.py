"""Read-only progression audit of trainer tiers, rewards and early wild tables."""
from pathlib import Path
import json,re
root=Path(__file__).resolve().parents[2]
s=(root/'src/data/trainer_parties.h').read_text()
rows=[]
for leader in ['Brock','Misty','LtSurge','Erika','Koga','Sabrina','Blaine']:
 for tier in range(1,8):
  m=re.search(r'sParty_Leader'+leader+str(tier)+r'\[\] = \{(.*?)\n\};',s,re.S)
  mons=re.findall(r'\.lvl = (\d+).*?\.species = SPECIES_(\w+)',m[1],re.S)
  rows.append(dict(leader=leader,badges=tier-1,levels=[int(l)for l,n in mons],species=[n for l,n in mons]))
json.dump(rows,open(root/'docs/difficulty-testing/gym-tiers.json','w'),indent=2)
wild=json.loads((root/'src/data/wild_encounters.json').read_text())['wild_encounter_groups'][0]
rates=next(x['encounter_rates']for x in wild['fields']if x['type']=='land_mons')
w=[]
for entry in wild['encounters']:
 if entry['map'] not in ['MAP_ROUTE1','MAP_ROUTE2','MAP_ROUTE3','MAP_VIRIDIAN_FOREST','MAP_MT_MOON_1F']:continue
 mons=entry.get('land_mons',{}).get('mons',[])
 if not mons:continue
 w.append(dict(map=entry['map'],label=entry['base_label'],slots=[dict(m,chance=rates[i])for i,m in enumerate(mons[:12])],unreachable_extra_slots=mons[12:]))
json.dump(w,open(root/'docs/difficulty-testing/early-wild.json','w'),indent=2)
for x in w:
 high=sum(m['chance'] for m in x['slots'] if m['min_level']>=18)
 print(x['label'],f'{high}% slots start at level 18+', 'max',max(m['max_level']for m in x['slots']))
