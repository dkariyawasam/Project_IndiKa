"""Validate captured native battle fixtures; does not modify any ROM or save."""
from pathlib import Path
import json,re,struct
root=Path(__file__).resolve().parents[2];out=Path(__file__).resolve().parent
cases=json.loads((out/'cases.json').read_text())
orders={int(n):list(map(int,v))for n,*v in re.findall(r'SUBSTRUCT_CASE\(\s*(\d+),(\d+),(\d+),(\d+),(\d+)\)',(root/'src/pokemon.c').read_text())}
def decode(raw):
 pid,ot=struct.unpack_from('<II',raw)
 d=b''.join(struct.pack('<I',struct.unpack_from('<I',raw,j)[0]^pid^ot)for j in range(32,80,4))
 g,a,e,_=orders[pid%24]
 assert sum(struct.unpack('<24H',d))%65536==struct.unpack_from('<H',raw,28)[0], 'Invalid Pokémon checksum'
 return dict(species=struct.unpack_from('<H',d,g*12)[0],level=raw[84],moves=list(struct.unpack_from('<4H',d,a*12)),evs=list(d[e*12:e*12+6]))
checked=0
for c in cases:
 b=(out/(c['name']+'-before.bin')).read_bytes()
 for i,m in enumerate(c['team']):
  mon=decode(b[i*100:(i+1)*100]);assert mon['species']==m['id'] and mon['level']==m['level'],c['name']
  assert not any(mon['evs']),c['name']
  if 'moves' in m:assert mon['moves']==m['moves'],c['name']
  checked+=1
 if c.get('league')==1:
  enemy=(out/(c['name']+'-enemy.bin')).read_bytes()
  for i in range(6):
   mon=decode(enemy[i*100:(i+1)*100])
   if mon['species']:assert mon['level']==50,c['name']
results={line.split('\t')[0]:list(map(int,line.split('\t')[1:])) for line in (out/'results.tsv').read_text().splitlines()}
assert set(results)=={c['name']for c in cases}, 'Missing or unexpected test results'
print(f'PASS: {len(cases)} completed native battles; {checked} valid fixture Pokémon; all sampled Indigo enemies level 50.')
