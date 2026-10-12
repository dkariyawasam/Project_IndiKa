"""Validate captured native battle fixtures; does not modify any ROM or save."""
from pathlib import Path
import json,re,struct
root=Path.cwd();out=root/'docs/release-playthrough/balance-follow-up/koga-erika'
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
 p=out/(c['name']+'-before.bin')
 if not p.exists():continue
 b=p.read_bytes()
 for i,m in enumerate(c['team']):
  mon=decode(b[i*100:(i+1)*100]);assert mon['species']==m['id'] and mon['level']==m['level'],c['name'];assert not any(mon['evs']);checked+=1
 e=out/(c['name']+'-enemy.bin')
 if e.exists():
  raw=e.read_bytes();enemy=[decode(raw[i*100:(i+1)*100]) for i in range(6) if struct.unpack_from('<H',raw,i*100+88)[0]>0]
  actual=[(m['species'],m['level']) for m in enemy]
  expected=[(169,38),(24,39),(89,40),(110,41),(368,42)] if c['name'].startswith('koga-') else [(450,41),(103,42),(425,45),(71,45),(45,46),(114,47)]
  assert actual==expected, (c['name'],actual)
  print(c['name'],actual)
print('Validated',checked,'player Pokemon checksums, species, levels and zero EVs.')
