"""Build the Expedition town-map tilemap from native FRLG tiles and route stencils.

Coordinates are the same 22x15 cells used by the cursor. No scaling or colour
quantisation is involved: route stencils use the existing 4-bit terrain palette.
"""
import json,re,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
p=ROOT/'src/data/region_map/region_map_layout_kanto.h'
s=p.read_text()
layers={}
for layer in ('LAYER_MAP','LAYER_DUNGEON'):
 part=s.split('['+layer+'] =')[1].split('\n    }')[0]
 layers[layer]=[[v.strip() for v in row.split(',')] for row in re.findall(r'\{(MAPSEC_[^{}]+)\}',part)]
m,d=layers['LAYER_MAP'],layers['LAYER_DUNGEON']
# Replace stale custom locations before placing their actual approaches.
for grid in (m,d):
 for row in grid:
  for x,v in enumerate(row):
   if v in ('MAPSEC_VERMILION_HARBOR','MAPSEC_FUCHSIA_FOREST','MAPSEC_VIRIDIAN_CHANNEL'):row[x]='MAPSEC_NONE'
for x in (5,6):m[6][x]='MAPSEC_VIRIDIAN_CHANNEL'
# City -> harbour -> forest -> Route 15 runs north to south.
m[10][14]='MAPSEC_VERMILION_HARBOR'
m[11][14]='MAPSEC_FUCHSIA_FOREST';d[11][14]='MAPSEC_FUCHSIA_FOREST'
m[11][15]='MAPSEC_ROUTE_14';m[11][16]='MAPSEC_ROUTE_13'
m[12][15]='MAPSEC_ROUTE_14';m[12][16]='MAPSEC_NONE'
for x in range(15,19):m[1][x]='MAPSEC_ROUTE_25'
m[2][18]='MAPSEC_ROUTE_10'
for x in range(5,10):m[12][x]='MAPSEC_ROUTE_18'
m[13][7]='MAPSEC_ROUTE_18'
# The islands sit beneath the bridge, not to its east.
for row in d:
 for x,v in enumerate(row):
  if v=='MAPSEC_SEAFOAM_ISLANDS':row[x]='MAPSEC_NONE'
d[14][7]='MAPSEC_SEAFOAM_ISLANDS'
# Align the northern cave above CAVE; the western Diglett entrance is
# immediately east of the forest. Keep the Route 11 entrance in place.
for row in d:
 for x,v in enumerate(row):
  if v=='MAPSEC_CERULEAN_CAVE' or (v=='MAPSEC_DIGLETTS_CAVE' and x<10):row[x]='MAPSEC_NONE'
d[3][12]='MAPSEC_CERULEAN_CAVE'
d[6][5]='MAPSEC_DIGLETTS_CAVE'
d[5][12]='MAPSEC_CELADON_CAVE'
d[6][11]='MAPSEC_LEAGUE_ROCKET'
d[1][16]='MAPSEC_BILLS_HOUSE'
d[2][14]='MAPSEC_NUGGET_BRIDGE'
# Landmark icons sit beside their cities without adding new road cells.
for row in d:
 for x,v in enumerate(row):
  if v in ('MAPSEC_CINNABAR_VOLCANO','MAPSEC_KANTO_SAFARI_ZONE'):row[x]='MAPSEC_NONE'
d[14][3]='MAPSEC_CINNABAR_VOLCANO'
d[11][10]='MAPSEC_KANTO_SAFARI_ZONE'

for layer,grid in layers.items():
 replacement='['+layer+'] =\n    {\n'+''.join('        {'+', '.join(row)+'},\n' for row in grid)+'    }'
 s=re.sub(r'\['+layer+r'\] =\n    \{.*?\n    \}',lambda _:replacement,s,flags=re.S)
p.write_text(s)
# Bounds must agree with selectable cells; use the dungeon layer for landmarks.
p=ROOT/'src/data/region_map/region_map_sections.json';j=json.loads(p.read_text())
for section in j['map_sections']:
 points=[(x,y) for y,row in enumerate(m) for x,v in enumerate(row) if v==section['id']]
 if not points:points=[(x,y) for y,row in enumerate(d) for x,v in enumerate(row) if v==section['id']]
 if points:
  xs,ys=zip(*points);section.update(x=min(xs),y=min(ys),width=max(xs)-min(xs)+1,height=max(ys)-min(ys)+1)
 if section['id']=='MAPSEC_CELADON_CAVE':section['name']='CAVE'
p.write_text(json.dumps(j,indent=2,ensure_ascii=False)+'\n')
# Build each changed cell from its original terrain tile. The original city
# markers use palette 1 and are left intact; paths meet their outside edges.
gfx=ROOT/'graphics/region_map';base=list(struct.unpack('<600H',(ROOT/'tools/region_map/kanto_base.bin').read_bytes()));raw=(gfx/'region_map.4bpp').read_bytes()
# The old selectable exit tile is replaced by the surrounding sea.
for y in range(16,19):
 for x in range(24,27):base[y*30+x]=base[y*30+23]
extra=[];cache={};new=base[:]
def decode(e):
 tile=e&1023;out=[]
 for y in range(8):
  for x in range(8):
   xx=7-x if e&1024 else x;yy=7-y if e&2048 else y
   b=raw[tile*32+yy*4+xx//2];out.append((b>>(4*(xx%2)))&15)
 return out
def encode(p):return bytes(p[i]|p[i+1]<<4 for i in range(0,64,2))
def route(v):return v!='MAPSEC_NONE'
# Adjacent nodes link only when their real maps share an approach. Most vanilla
# paths are retained. These pairs must not imply new crossings or ferry stops.
blocked={frozenset(('MAPSEC_FUCHSIA_FOREST','MAPSEC_ROUTE_14'))}
for y,row in enumerate(m):
 for x,v in enumerate(row):
  if not route(v):continue
  tileindex=(y+4)*30+x+4;e=base[tileindex]
  if e>>12 == 1 and e&1023 == 1:continue
  pixels=decode(e)
  if e>>12 == 1:
   pixels=[i-6 if i>=12 else 9 for i in pixels]
  sea=v in ('MAPSEC_ROUTE_19','MAPSEC_ROUTE_20','MAPSEC_ROUTE_21','MAPSEC_VERMILION_HARBOR')
  # Route 18 is mixed: its west and south arms require Surf.
  sea = sea or (v == 'MAPSEC_ROUTE_18' and (x < 7 or y > 12))
  dirs=[]
  for dx,dy in ((0,-1),(0,1),(-1,0),(1,0)):
   nx,ny=x+dx,y+dy
   if 0<=nx<22 and 0<=ny<15 and route(m[ny][nx]) and frozenset((v,m[ny][nx])) not in blocked:dirs.append((dx,dy))
  # Draw a three-pixel corridor with a one-pixel light centre.
  for dx,dy in dirs:
   water_arm = sea or (v == 'MAPSEC_ROUTE_18' and (x,y) == (7,12)
                       and (dx,dy) in ((-1,0),(0,1)))
   edge,fill=(12,14) if water_arm else (6,9)
   for n in range(5):
    px,py=3+dx*n,3+dy*n
    if not(0<=px<8 and 0<=py<8):continue
    for off in (-1,0,1):
     xx,yy=(px+off,py) if dy else (px,py+off)
     if 0<=xx<8 and 0<=yy<8:pixels[yy*8+xx]=edge
   for n in range(5):
    px,py=3+dx*n,3+dy*n
    if 0<=px<8 and 0<=py<8:pixels[py*8+px]=fill
  if v=='MAPSEC_FUCHSIA_FOREST':
   # A small grove, using the existing forest shades.
   for xx,yy in ((2,2),(5,2),(3,5)):
    for dy in range(2):
     for dx in range(2):pixels[(yy+dy)*8+xx+dx]=1 if dy else 2
  payload=encode(pixels)
  if payload not in cache:cache[payload]=320+len(extra);extra.append(payload)
  new[tileindex]=cache[payload]
assert 320+len(extra)<=512, 'Map graphics exceed BG0 character bank'
(gfx/'expedition_routes.bin').write_bytes(b''.join(extra))
(gfx/'kanto.bin').write_bytes(struct.pack('<600H',*new))
print(f'Built {len(extra)} route tiles; cursor grid and section bounds updated.')
