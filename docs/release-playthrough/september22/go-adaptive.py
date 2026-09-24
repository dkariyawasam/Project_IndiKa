from movement_rules import allowed, neighbors, behavior
import json,struct,re,sys,collections,time
from pathlib import Path
base=Path('/tmp/kanto-release-september22')
def state():
 s=(base/'status.txt').read_text();m=re.search(r'frame=(\d+) map=(\d+),(\d+) xy=(\d+),(\d+).*lock=(\d+)',s)
 return tuple(map(int,m.groups()))
surf='--surf' in sys.argv;name=sys.argv[1];groups=json.load(open('data/maps/map_groups.json'));v=state();assert groups[groups['group_order'][v[1]]][v[2]]==name, ('Wrong map',v,name)
goal=tuple(map(int,sys.argv[2:4]));m=json.load(open('data/maps/'+name+'/map.json'));l=next(x for x in json.load(open('data/layouts/layouts.json'))['layouts'] if x['id']==m['layout']);w,h=l['width'],l['height'];b=struct.unpack('<'+'H'*(w*h),Path(l['blockdata_filepath']).read_bytes());
try:
 liveMap=(base/'live-map.txt').read_text().splitlines();meta=tuple(map(int,liveMap[0].split()))
 if meta==(v[1],v[2],w,h):
  liveBlocks=tuple(map(int,liveMap[1].split()))
  if len(liveBlocks)==w*h:b=liveBlocks
except (FileNotFoundError,ValueError,IndexError):pass

def plan():
 v=state()
 occ={(o['x'],o['y']) for o in m['object_events']}
 try:
  live=[tuple(map(int,line.split())) for line in (base/'live-objects.tsv').read_text().splitlines()]
  if live[0]==v[1:3]:
   for ident,x,y in live[1:]:
    if 1<=ident<=len(m['object_events']):
     obj=m['object_events'][ident-1];occ.discard((obj['x'],obj['y']))
    occ.add((x,y))
 except (FileNotFoundError,IndexError,ValueError):pass
 s=state();start=s[3:5];prev={start:None};q=collections.deque([start])
 while q:
  p=q.popleft()
  if p==goal:break
  for n in neighbors(l,b,w,h,p):
   x,y=n
   if 0<=x<w and 0<=y<h and n not in prev and n not in occ and not (b[y*w+x]>>10)&3 and (allowed(l,b,w,p,n) or (surf and behavior(l,b,w,p) in (0x10,0x11,0x12,0x15) and behavior(l,b,w,n) in (0x10,0x11,0x12,0x15))):prev[n]=p;q.append(n)
 assert goal in prev,(name,start,goal,'No path')
 path=[];p=goal
 while p!=start:path.append(p);p=prev[p]
 path=path[::-1];corners=[];last=start
 for i,p in enumerate(path):
  if i==len(path)-1 or (p[0]-last[0],p[1]-last[1])!=(path[i+1][0]-p[0],path[i+1][1]-p[1]):corners.append(p)
  last=p
 (base/'command.lua').write_text('qa.path({'+','.join('{%d,%d}'%p for p in corners)+'})')

s=state();plan()
t=time.time();lastpos=s[3:5];lastmove=t;replans=0
while time.time()-t<45:
 time.sleep(.3)
 try:
  v=state()
  if v[3:5]!=lastpos:lastpos=v[3:5];lastmove=time.time()
  elif v[-1]==0 and time.time()-lastmove>1.5 and replans<12 and v[1:3]==s[1:3]:
   try:plan()
   except AssertionError:pass
   lastmove=time.time();replans+=1
  if v[0]>s[0]+120 and (v[3:5]==goal or v[1:3]!=s[1:3]):print(name,'reached',v,flush=True);time.sleep(0.7);break
 except (AttributeError,ValueError):pass
else:raise RuntimeError('Travel paused: '+(base/'status.txt').read_text()[:250])
