#!/usr/bin/env python3
"""Read-only static collision audit. Build ROM first; candidates require review.
Not a simulation of script-driven barriers, Surf, ledges, or dynamic objects.
Reports are written under /tmp/expedition-collision-*.json.
"""
import sys,json,struct,threading,collections
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
import os
os.chdir(ROOT)
sys.path.insert(0,str(ROOT/'tools/debug_dashboard'))
from atlas import AtlasRenderer
from catalog import build_catalog,load_symbols
c=build_catalog();r=AtlasRenderer.__new__(AtlasRenderer);r.rom=Path('pokefirered.gba').read_bytes();r.symbols=load_symbols(Path('pokefirered.elf'));r.maps={m['name']:m for m in c['maps']};r.cache={};r.tilesets={};r.pairs={};r.lock=threading.Lock();findings=[];maps={}
import re
max_map_data_size=int(re.search(r'#define MAX_MAP_DATA_SIZE\s+(0x[0-9A-Fa-f]+|\d+)', (ROOT/'include/fieldmap.h').read_text())[1],0)
for m in c['maps']:
 w,h,_,blocks,p,s=r.words(r.symbols[m['layoutSymbol']],6);a=struct.unpack('<'+'H'*(w*h),r.read(blocks,w*h*2));attrs=struct.unpack('<1024I',r.tileset(p,False)[3]+r.tileset(s,True)[3]);maps[m['id']]=(m,w,h,a,attrs)
 for i,o in enumerate(m['objects'],1):
  if o.get('type')=='clone':continue
  x,y=o['x'],o['y'];detail=dict(map=m['name'],kind='',event=i,x=x,y=y,gfx=o.get('graphics_id'),script=o.get('script'))
  if not(0<=x<w and 0<=y<h):detail['kind']='object out of bounds';findings.append(detail);continue
  t=a[y*w+x];el=t>>12;collision=(t>>10)&3;b=attrs[t&1023]&511
  detail.update(tile=hex(t),behavior=hex(b),elevation=o['elevation'])
  if o['trainer_type']=='TRAINER_TYPE_NORMAL':
   if collision:detail['kind']='trainer on blocked tile';findings.append(detail.copy())
   if o['elevation'] not in [0,15] and el not in [0,15,o['elevation']]:detail['kind']='trainer elevation mismatch';findings.append(detail.copy())
   water=b in [0x10,0x11,0x12,0x13,0x15,0x1b]
   if water and '_WATER' not in o['graphics_id']:detail['kind']='land trainer in water';findings.append(detail.copy())
   if not water and '_WATER' in o['graphics_id']:detail['kind']='water trainer on land';findings.append(detail.copy())
 for i,o in enumerate(m['warps']):
  if not(0<=o['x']<w and 0<=o['y']<h):findings.append(dict(map=m['name'],kind='warp out of bounds',event=i,**o))
 if (w+15)*(h+14)>max_map_data_size:findings.append(dict(map=m['name'],kind='map exceeds buffer',width=w,height=h))
Path('/tmp/expedition-collision-events.json').write_text(json.dumps(findings,indent=2));print(len(maps),'maps',len(findings),'findings');print(collections.Counter(f['kind'] for f in findings))

waterbad=[];seams=[]
for id,(m,w,h,a,attrs) in maps.items():
 for y in range(h):
  for x in range(w):
   t=a[y*w+x];b=attrs[t&1023]&511
   if b in [0x10,0x11,0x12,0x13,0x15,0x1a,0x1b,0x50,0x51,0x52,0x53] and t>>12 not in [0,1,15] and not ((t>>10)&3):waterbad.append((m['name'],x,y,hex(t)))
 for co in m['connections']:
  if co['map'] not in maps:continue
  n,nw,nh,na,nattrs=maps[co['map']];d=co['direction'];off=int(co['offset']);bad=[];cross=0
  for i in range(w if d in ['up','down'] else h):
   x,y=(i,0 if d=='up' else h-1) if d in ['up','down'] else (0 if d=='left' else w-1,i)
   nx,ny=(i-off,nh-1 if d=='up' else 0) if d in ['up','down'] else (nw-1 if d=='left' else 0,i-off)
   if not(0<=nx<nw and 0<=ny<nh):continue
   t=a[y*w+x];u=na[ny*nw+nx]
   if not((t>>10)&3) and not((u>>10)&3):
    e,f=t>>12,u>>12
    if e not in [0,15] and f not in [0,15,e]:bad.append((x,y,nx,ny,e,f))
    else:cross+=1
  if bad or not cross:seams.append(dict(map=m['name'],to=n['name'],crossings=cross,mismatches=bad))
print('Water elevation candidates:',len(waterbad),dict(collections.Counter(n for n,x,y,t in waterbad)))
print('Connection candidates:',json.dumps(seams))
Path('/tmp/expedition-collision-water.json').write_text(json.dumps(waterbad,indent=2))
Path('/tmp/expedition-collision-seams.json').write_text(json.dumps(seams,indent=2))

stats=collections.defaultdict(collections.Counter);locs=collections.defaultdict(list)
for m,w,h,a,attrs in maps.values():
 layout=r.words(r.symbols[m['layoutSymbol']],6);p,s=layout[4:6]
 for idx,t in enumerate(a):
  tile=t&1023;key=(p if tile<640 else s,tile);coll=(t>>10)&3;stats[key][coll]+=1;locs[(key,coll)].append((m['name'],idx%w,idx//w,hex(t)))
out=[]
for key,count in stats.items():
 total=sum(count.values());dom,n=count.most_common(1)[0]
 if total>=100 and n/total>=.99:
  for coll,num in count.items():
   if coll!=dom:out.append((dom,locs[(key,coll)]))

Path('/tmp/expedition-collision-outliers.json').write_text(json.dumps(out,indent=2))
print('Collision-pattern candidates:',sum(len(v) for _,v in out))
warp_issues=[]
for m,w,h,a,attrs in maps.values():
 for i,warp in enumerate(m['warps']):
  target=maps.get(warp['dest_map'])
  try: index=int(warp['dest_warp_id'])
  except ValueError: continue
  if target and index not in [127,255] and not 0<=index<len(target[0]['warps']):
   warp_issues.append(dict(map=m['name'],warp=i,target=target[0]['name'],target_warp=index))
Path('/tmp/expedition-collision-warps.json').write_text(json.dumps(warp_issues,indent=2))
print('Invalid fixed warp targets:',len(warp_issues))
