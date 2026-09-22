import struct,re
from pathlib import Path
from functools import lru_cache
ROOT=Path('/Users/deekariyawasam/pokefirered')
@lru_cache(None)
def attrs(name,secondary):
 name=name.removeprefix('gTileset_');name=re.sub(r'(?<!^)(?=[A-Z])','_',name).lower()
 p=ROOT/'data/tilesets'/('secondary' if secondary else 'primary')/name/'metatile_attributes.bin'
 return p.read_bytes() if p.exists() else b''
def behavior(layout,blocks,w,p):
 ident=blocks[p[1]*w+p[0]]&1023;secondary=ident>=640
 data=attrs(layout['secondary_tileset' if secondary else 'primary_tileset'],secondary);offset=(ident-640 if secondary else ident)*4
 return struct.unpack_from('<I',data,offset)[0]&511 if offset+4<=len(data) else 0

def allowed(layout,blocks,w,src,dst):
 se=blocks[src[1]*w+src[0]]>>12;de=blocks[dst[1]*w+dst[0]]>>12
 if se not in (0,15) and de not in (0,15) and se!=de:return False
 a,b=behavior(layout,blocks,w,src),behavior(layout,blocks,w,dst);d=(dst[0]-src[0],dst[1]-src[1]);d=tuple((v>0)-(v<0) for v in d)
 blocked={0x30:{(1,0)},0x31:{(-1,0)},0x32:{(0,-1)},0x33:{(0,1)},0x34:{(1,0),(0,-1)},0x35:{(-1,0),(0,-1)},0x36:{(1,0),(0,1)},0x37:{(-1,0),(0,1)}}
 return d not in blocked.get(a,set()) and (-d[0],-d[1]) not in blocked.get(b,set()) and b not in (0x10,0x11,0x12,0x13,0x15)

def neighbors(layout,blocks,w,h,p):
 for dx,dy in [(0,-1),(-1,0),(1,0),(0,1)]:
  n=p[0]+dx,p[1]+dy
  if not(0<=n[0]<w and 0<=n[1]<h):continue
  ledge=behavior(layout,blocks,w,n)
  if ledge in (0x38,0x39,0x3a,0x3b):
   if (dx,dy)!={0x38:(1,0),0x39:(-1,0),0x3a:(0,-1),0x3b:(0,1)}[ledge]:continue
   n=n[0]+dx,n[1]+dy
  yield n
