#!/usr/bin/env python3
"""Static fixed-sight chokepoint candidates; not a timed traversal simulation.
Ignores elevation, tile behaviors, moving-object phases, and script barriers.
Treats source object positions as occupied and collision-zero tiles as traversable.
Review candidates before editing. Run from repository root.
"""
import json,struct
from pathlib import Path
L={x['id']:x for x in json.load(open('data/layouts/layouts.json'))['layouts']}
D={'DOWN':(0,1),'UP':(0,-1),'LEFT':(-1,0),'RIGHT':(1,0)}
def comps(cells):
 out=[]; cells=set(cells)
 while cells:
  q=[cells.pop()]; seen=set(q)
  for x,y in q:
   for dx,dy in D.values():
    p=x+dx,y+dy
    if p in cells: cells.remove(p);seen.add(p);q.append(p)
  out.append(seen)
 return out
for p in sorted(Path('data/maps').glob('*/map.json')):
 m=json.loads(p.read_text()); l=L[m['layout']];w,h=l['width'],l['height'];a=struct.unpack('<'+'H'*(w*h),Path(l['blockdata_filepath']).read_bytes());walk={(i%w,i//w) for i,t in enumerate(a) if not(t&0xc00)}
 obs=m.get('object_events',[]);occupied={(o['x'],o['y']) for o in obs if o.get('type')!='clone'}; base=walk-occupied
 for i,o in enumerate(obs,1):
  if o.get('trainer_type')!='TRAINER_TYPE_NORMAL':continue
  d=o['movement_type'].removeprefix('MOVEMENT_TYPE_FACE_')
  if d not in D:continue
  ray=set();dx,dy=D[d]
  for n in range(1,int(o['trainer_sight_or_berry_tree_id'])+1):
   xy=o['x']+dx*n,o['y']+dy*n
   if xy not in base:break
   ray.add(xy)
  if not ray:continue
  region=next((c for c in comps(base) if ray&c),set())
  sizes=sorted([len(c) for c in comps(region-ray)],reverse=True)
  if len(sizes)>1 and sizes[1]>=4:print(m['name'],i,o['script'],d,sizes)
