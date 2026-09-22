import sys,json,struct,collections
from pathlib import Path
from movement_rules import allowed,neighbors
name=sys.argv[1];start=tuple(map(int,sys.argv[2:4]));goal=tuple(map(int,sys.argv[4:6]))
m=json.load(open('data/maps/'+name+'/map.json'));l=next(x for x in json.load(open('data/layouts/layouts.json'))['layouts'] if x['id']==m['layout']);w,h=l['width'],l['height'];b=struct.unpack('<'+'H'*(w*h),Path(l['blockdata_filepath']).read_bytes());occ={(o['x'],o['y']) for o in m['object_events']};warps={(a['x'],a['y']):a for a in m['warp_events']}
q=collections.deque([start]);prev={start:None}
while q:
 p=q.popleft()
 if p==goal:break
 for n in neighbors(l,b,w,h,p):
  x,y=n
  if not(0<=x<w and 0<=y<h) or n in occ or (b[y*w+x]>>10)&3 or not allowed(l,b,w,p,n):continue
  to=n;warp=warps.get(n)
  if warp:
   if warp['dest_map']!=m['id']:continue
   d=m['warp_events'][int(warp['dest_warp_id'])];to=(d['x'],d['y'])
  if to in prev:continue
  prev[to]=(p,n,bool(warp));q.append(to)
assert goal in prev
seq=[];p=goal
while prev[p]:
 old,n,warp=prev[p]
 if warp:seq.append((old,n,p))
 p=old
print(list(reversed(seq)))
