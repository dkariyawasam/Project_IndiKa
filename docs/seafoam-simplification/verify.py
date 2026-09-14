"""Verify the three-floor Seafoam route without Strength or current flags."""
from pathlib import Path
import json,os,re,struct
from map_paths import path
root=Path(__file__).resolve().parents[2];os.chdir(root)
def read(n):return json.loads(Path('data/maps',n,'map.json').read_text())
a,b,c='SeafoamIslands_1F','SeafoamIslands_B2F','SeafoamIslands_B4F'
active={n:read(n)for n in (a,b,c)}
expected={(a,0):(b,0),(a,1):(b,1),(b,0):(a,0),(b,1):(a,1),(b,2):(c,0),(c,0):(b,2)}
for (n,i),(other,j)in expected.items():
 w=active[n]['warp_events'][i];assert(w['dest_map'],int(w['dest_warp_id']))==('MAP_'+re.sub(r'(?<=[a-z])(?=[A-Z])','_',other).upper(),j)
for n,pairs in [(a,[(2,0),(3,1)]),(b,[(0,1),(0,2),(1,2)])]:
 w=active[n]['warp_events']
 for i,j in pairs:
  for u,v in [(i,j),(j,i)]:path(n,(w[u]['x'],w[u]['y']),(w[v]['x'],w[v]['y']),allow_warp=True)
for n in (a,b,c):
 m=active[n];w=m['warp_events'][0];start=(w['x'],w['y'])
 assert not any('BOULDER' in o['graphics_id']for o in m['object_events'])
 assert not m['coord_events']
 for o in m['object_events']:
  found=False
  for dx,dy in [(0,1),(1,0),(0,-1),(-1,0)]:
   try:path(n,start,(o['x']+dx,o['y']+dy),surf=n==c);found=True;break
   except AssertionError:pass
  assert found,(n,o['script'])
# Scan all metatiles, not just retained warp events, for leftover holes/current behavior.
layouts=json.loads(Path('data/layouts/layouts.json').read_text())['layouts']
headers=Path('src/data/tilesets/headers.h').read_text();attrs=Path('src/data/tilesets/metatiles.h').read_text()
for n,m in active.items():
 l=next(l for l in layouts if l['id']==m['layout']);tables=[]
 for key in ['primary_tileset','secondary_tileset']:
  body=headers.split('const struct Tileset '+l[key]+' =')[1].split('};')[0];sym=re.search(r'\.metatileAttributes = (\w+)',body)[1];f=re.search(r'\b'+sym+r'\[\] = INCBIN_U32\("([^"]+)"',attrs)[1];raw=Path(f).read_bytes();tables.append(struct.unpack('<'+'I'*(len(raw)//4),raw))
 raw=Path(l['blockdata_filepath']).read_bytes()
 for i,(v,)in enumerate(struct.iter_unpack('<H',raw)):
  t=v&1023;behavior=tables[t>=640][t-640 if t>=640 else t]&511
  if behavior==0x61:assert (i%l["width"],i//l["width"]) in {(w["x"],w["y"])for w in m["warp_events"]},(n,"orphan ladder",i)
  assert behavior not in (0x50,0x51,0x52,0x53,0x66),(n,i%l['width'],i//l['width'],hex(behavior))
for n in ('SeafoamIslands_B1F','SeafoamIslands_B3F'):
 m=read(n);assert not m['object_events'] and not m['warp_events']
 assert 'warp MAP_SEAFOAM_ISLANDS_1F, 6, 20' in Path('data/maps',n,'scripts.inc').read_text()
for p in Path('data/maps').glob('*/map.json'):
 for w in json.loads(p.read_text())['warp_events']:
  assert w['dest_map'] not in ('MAP_SEAFOAM_ISLANDS_B1F','MAP_SEAFOAM_ISLANDS_B3F'),p
assert len(active[b]['object_events'])==3
assert next(o for o in active[b]['object_events']if o['script'].endswith('ItemWaterStone'))['elevation']==4
s=Path('data/maps',c,'scripts.inc').read_text();assert 'IsApexRevealed' in s and 'FLAG_FOUGHT_ARTICUNO' in s and 'LOCALID_SEAFOAM_ARTICUNO' in s
q=Path('src/quests.c').read_text();assert re.search(r'SUB_QUEST_APEX_ARTICUNO,\s+MAP_SEAFOAM_ISLANDS_B4F,.*LOCALID_SEAFOAM_ARTICUNO',q)
ov=Path('src/overworld.c').read_text().split('bool8 MetatileBehavior_IsSurfableInSeafoamIslands(u16 metatileBehavior)')[1].split('static u8 GetAdjustedInitialDirection')[0];assert 'MAP_SEAFOAM_ISLANDS_B4F' not in ov
assert 'ResetSeafoamBoulders' not in Path('data/maps/Route20/scripts.inc').read_text()
print('PASS: three-floor reciprocal network; entrances, every item and Articuno reachable; no boulders, holes or currents; old-floor redirects and Apex gate retained.')
