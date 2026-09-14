from pathlib import Path
import re,struct,json
root=Path(__file__).resolve().parents[3]
orders=[tuple(map(int,m)) for m in re.findall(r'SUBSTRUCT_CASE\(\s*\d+,([0-3]),([0-3]),([0-3]),([0-3])\)',(root/'src/pokemon.c').read_text())]
def party(path):
 raw=Path(path).read_bytes();out=[]
 for i in range(6):
  b=raw[i*100:(i+1)*100];p,ot=struct.unpack_from('<II',b);d=b''.join(struct.pack('<I',v^p^ot) for v in struct.unpack_from('<12I',b,32));g,a,e,m=[d[j*12:j*12+12] for j in orders[p%24]]
  out.append(dict(species=int.from_bytes(g[:2],'little'),item=int.from_bytes(g[2:4],'little'),exp=int.from_bytes(g[4:8],'little'),friendship=g[9],moves=list(struct.unpack_from('<4H',a)),pp=list(a[8:12]),evs=list(e[:6]),iv=int.from_bytes(m[4:8],'little'),level=b[84],hp=int.from_bytes(b[86:88],'little'),maxhp=int.from_bytes(b[88:90],'little'),status=int.from_bytes(b[80:84],'little')))
 return out
if __name__=='__main__':
 import sys
 for f in sys.argv[1:]:print(f,json.dumps(party(f)))
