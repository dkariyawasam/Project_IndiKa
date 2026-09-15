from pathlib import Path
import json,re,struct
root=Path(__file__).resolve().parents[2];out=Path(__file__).resolve().parent
cases=json.loads((out/'cases.json').read_text())
orders={int(n):list(map(int,v))for n,*v in re.findall(r'SUBSTRUCT_CASE\(\s*(\d+),(\d+),(\d+),(\d+),(\d+)\)',(root/'src/pokemon.c').read_text())}
def decode(b):
 p,o=struct.unpack_from('<II',b);d=b''.join(struct.pack('<I',struct.unpack_from('<I',b,j)[0]^p^o)for j in range(32,80,4));g,a,e,_=orders[p%24]
 assert sum(struct.unpack('<24H',d))%65536==struct.unpack_from('<H',b,28)[0]
 return d,g,a,e
for c in cases:
 before=(out/(c['name']+'-before.bin')).read_bytes()
 for i,m in enumerate(c['team']):
  raw=before[i*100:(i+1)*100];d,g,a,e=decode(raw)
  assert struct.unpack_from('<H',d,g*12)[0]==m['id'] and raw[84]==50
  assert list(struct.unpack_from('<4H',d,a*12))==m['moves']
  assert not any(d[e*12:e*12+6])
 if c['mode']!='baseline':
  baseline=c.get('baselineName',c['name'].replace('-improved-','-baseline-').replace('-refined-','-baseline-'))
  original=(out/(baseline+'-before.bin')).read_bytes()
  for i,slot in enumerate(c.get('order',list(range(6)))):
   b=original[slot*100:(slot+1)*100];n=before[i*100:(i+1)*100];d,g,a,e=decode(b);nd,*_=decode(n)
   assert b[:28]==n[:28] and b[30:32]==n[30:32] and b[80:]==n[80:],c['name']
   assert d[:a*12]==nd[:a*12] and d[(a+1)*12:]==nd[(a+1)*12:],c['name']
results={l.split('\t')[0]for l in (out/'results.tsv').read_text().splitlines()}
assert results=={c['name']for c in cases}
print(f'PASS: {len(cases)} native battle results; prescribed moves; paired identities, IVs, EVs, stats, friendship, levels and HP unchanged.')
