import json,struct,re,sys,collections
from pathlib import Path
name=sys.argv[1];goal=tuple(map(int,sys.argv[2:4]));m=json.load(open('data/maps/'+name+'/map.json'));l=next(x for x in json.load(open('data/layouts/layouts.json'))['layouts'] if x['id']==m['layout']);w,h=l['width'],l['height'];b=struct.unpack('<'+'H'*(w*h),Path(l['blockdata_filepath']).read_bytes());occ={(o['x'],o['y']) for o in m['object_events']};status=Path('/tmp/kanto-release-playtest/status.txt').read_text();start=tuple(map(int,re.search(r'xy=(\d+),(\d+)',status).groups()));prev={start:None};q=collections.deque([start])
while q:
 p=q.popleft()
 if p==goal:break
 x,y=p
 for n in [(x,y-1),(x-1,y),(x+1,y),(x,y+1)]:
  a,c=n
  if 0<=a<w and 0<=c<h and n not in prev and n not in occ and not (b[c*w+a]>>10)&3:prev[n]=p;q.append(n)
assert goal in prev,(start,goal,'No clear path')
path=[];p=goal
while p!=start:path.append(p);p=prev[p]
path=path[::-1];corners=[];last=start
for i,p in enumerate(path):
 if i==len(path)-1 or (p[0]-last[0],p[1]-last[1])!=(path[i+1][0]-p[0],path[i+1][1]-p[1]):corners.append(p)
 last=p
cmd='qa.path({'+','.join('{%d,%d}'%p for p in corners)+'})\n';print(start,'->',goal,'steps',len(path),cmd.strip());Path('/tmp/kanto-release-playtest/command.lua').write_text(cmd)
