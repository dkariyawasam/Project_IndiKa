"""Collision, elevation and directional traversal checks against built-in map data."""
from pathlib import Path
import json,sys,re
from collections import deque
import struct

def path(name,start,end,allow_warp=False,surf=False,water_only=False):
 layouts=json.load(open('data/layouts/layouts.json'))['layouts']
 d=next(x for x in layouts if x['id']==json.load(open('data/maps/'+name+'/map.json'))['layout']);w,h=d['width'],d['height']
 b=struct.unpack('<'+'H'*(w*h),Path(d['blockdata_filepath']).read_bytes())
 m=json.load(open('data/maps/'+name+'/map.json'))
 headers=Path('src/data/tilesets/headers.h').read_text();attrs=Path('src/data/tilesets/metatiles.h').read_text();tables=[]
 for k in ['primary_tileset','secondary_tileset']:
  body=headers.split('const struct Tileset '+d[k]+' =')[1].split('};')[0]
  sym=re.search(r'\.metatileAttributes = (\w+)',body)[1]
  filename=re.search(r'\b'+sym+r'\[\] = INCBIN_U32\("([^"]+)"',attrs)[1]
  raw=Path(filename).read_bytes();tables.append(struct.unpack('<'+'I'*(len(raw)//4),raw))
 def behavior(pos):
  tile=b[pos[1]*w+pos[0]]&0x3ff;t=0 if tile<640 else 1;index=tile if t==0 else tile-640
  return tables[t][index]&0x1ff if index<len(tables[t]) else 0
 sides={0x30:{(1,0)},0x31:{(-1,0)},0x32:{(0,-1)},0x33:{(0,1)},0x34:{(0,-1),(1,0)},0x35:{(0,-1),(-1,0)},0x36:{(0,1),(1,0)},0x37:{(0,1),(-1,0)}}
 def direction_ok(a,t,dx,dy):
  ea=b[a[1]*w+a[0]]>>12;et=b[t[1]*w+t[0]]>>12
  if ea not in (0,15) and et not in (0,15) and ea!=et and not(surf and (behavior(a) in {0x10,0x11,0x12,0x13,0x15,0x1a,0x1b,0x50,0x51,0x52,0x53} or behavior(t) in {0x10,0x11,0x12,0x13,0x15,0x1a,0x1b,0x50,0x51,0x52,0x53})):return False
  return (dx,dy) not in sides.get(behavior(a),set()) and (-dx,-dy) not in sides.get(behavior(t),set())

 blocked={(o['x'],o['y']) for o in m['object_events']}|{(o['x'],o['y']) for o in m['warp_events']}
 blocked.discard(start)
 if allow_warp:blocked.discard(end)
 q=deque([start]);prev={start:None}
 while q:
  x,y=q.popleft()
  if (x,y)==end:break
  for dx,dy in [(0,-1),(1,0),(-1,0),(0,1)]:
   t=(x+dx,y+dy)
   if 0<=t[0]<w and 0<=t[1]<h and behavior(t) in range(0x38,0x3c):
    if {0x38:(1,0),0x39:(-1,0),0x3a:(0,-1),0x3b:(0,1)}[behavior(t)]!=(dx,dy):continue
    t=(x+2*dx,y+2*dy)
   if 0<=t[0]<w and 0<=t[1]<h and not(b[t[1]*w+t[0]]>>10&3) and (surf or behavior(t) not in {0x10,0x11,0x12,0x13,0x15,0x1a,0x1b,0x50,0x51,0x52,0x53}) and (not water_only or t==end or behavior(t) in {0x10,0x11,0x12,0x13,0x15,0x1a,0x1b,0x50,0x51,0x52,0x53}) and t not in prev and t not in blocked and direction_ok((x,y),t,dx,dy):prev[t]=(x,y);q.append(t)
 assert end in prev,(name,start,end)
 chain=[];p=end
 while p is not None:chain.append(p);p=prev[p]
 chain=chain[::-1];ends=[];last=None
 for i in range(1,len(chain)):
  direction=(chain[i][0]-chain[i-1][0],chain[i][1]-chain[i-1][1])
  if last is not None and direction!=last:ends.append(chain[i-1])
  last=direction
 if len(chain)>1:ends.append(end)
 return ["{kind='walk',x=%d,y=%d}"%t for t in ends]
