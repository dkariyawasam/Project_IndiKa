from pathlib import Path
import json,struct,re
from collections import deque
import os
os.chdir(Path(__file__).resolve().parents[2])
def solve_push(name,start,rock,goal,placements=None):
 m=json.load(open('data/maps/'+name+'/map.json'));d=next(x for x in json.load(open('data/layouts/layouts.json'))['layouts']if x['id']==m['layout']);w,h=d['width'],d['height'];raw=Path(d['blockdata_filepath']).read_bytes()

 for index,position in (placements or {}).items():m['object_events'][index]['x'],m['object_events'][index]['y']=position
 b=struct.unpack('<'+'H'*(w*h),raw)
 blocked={(o['x'],o['y'])for o in m['object_events']}|{(o['x'],o['y'])for o in m['warp_events']}
 barrier=Path('data/maps/'+name+'/scripts.inc').read_text().split(name+'_EventScript_SetRockBarrier::')[1].split('return')[0]
 blocked.update((int(x),int(y))for x,y in re.findall(r'setmetatile (\d+), (\d+), \w+, 1',barrier))
 blocked.discard(rock);blocked.discard(start)
 allowed={(x,y)for y in range(h)for x in range(w)if not(b[y*w+x]>>10&3)}-blocked
 adj={p:[]for p in allowed}
 for p in allowed:
  for dx,dy in [(0,-1),(1,0),(0,1),(-1,0)]:
   t=(p[0]+dx,p[1]+dy)
   if t in allowed:
    a=b[p[1]*w+p[0]]>>12;c=b[t[1]*w+t[0]]>>12
    if a==c or a in(0,15)or c in(0,15):adj[p].append(t)
 def reach(p,r):
  q=deque([p]);prev={p:None}
  while q:
   a=q.popleft()
   for t in adj.get(a,[]):
    if t not in prev and t!=r:prev[t]=a;q.append(t)
  return prev
 initial=(start,rock);q=deque([initial]);parents={initial:None};seen=set();found=None
 while q:
  st=q.popleft();p,r=st;walk=reach(p,r);key=(min(walk),r)
  if key in seen:continue
  seen.add(key)
  if r==goal:found=st;break
  for dx,dy in [(0,-1),(1,0),(0,1),(-1,0)]:
   before=(r[0]-dx,r[1]-dy);after=(r[0]+dx,r[1]+dy)
   if before not in walk or after not in adj.get(r,[]):continue
   new=(r,after)
   if new in parents:continue
   steps=[];t=before
   while walk[t]is not None:steps.append(t);t=walk[t]
   parents[new]=(st,list(reversed(steps)),r,(dx,dy));q.append(new)
 assert found,(name,start,rock,goal)
 seq=[]
 while parents[found]:
  old,steps,r,dr=parents[found];seq.append((steps,r,dr));found=old
 return seq[::-1]

solution=solve_push('VictoryRoad_1F',(11,20),(7,18),(20,16))
assert solution
print('PASS: Victory Road first-floor switch can be reached with every trainer present.')

try:
 solve_push('VictoryRoad_1F',(11,20),(7,18),(20,16),{7:(18,18)})
except AssertionError:
 print('PASS: Regression fixture detects Ivo blocking the original push route.')
else:
 raise AssertionError('Original obstructing placement must not pass')
