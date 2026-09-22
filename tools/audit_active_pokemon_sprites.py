#!/usr/bin/env python3
"""Audit active Dex species' referenced sprite and palette source assets."""
from pathlib import Path
import re,json,sys
from collections import Counter
from PIL import Image
sys.path.insert(0,str(Path(__file__).parent))
from sync_active_pokedex import derive,ROOT
active,excluded,entries,names,stats=derive()
def read(p):return (ROOT/p).read_text()
def macro(p):return dict(re.findall(r'SPECIES_\w+\((\w+),\s*(\w+)\)',read(p)))
def direct(p,prefix):return dict(re.findall(r'\[SPECIES_(\w+)\s*\]\s*=\s*('+prefix+r'\w+)',read(p)))
tables={k:macro('src/data/pokemon_graphics/'+f) for k,f in [('front','front_pic_table.h'),('back','back_pic_table.h'),('normal','palette_table.h'),('shiny','shiny_palette_table.h')]}
tables['icon']=direct('src/pokemon_icon.c','gMonIcon_');tables['footprint']=direct('src/data/pokemon_graphics/footprint_table.h','gMonFootprint_')
defs={}
for p in (ROOT/'src/data/graphics').glob('*.h'):
 defs.update(re.findall(r'(\w+)\[\]\s*=\s*INCBIN_\w+\(\s*"([^"]+)"\)',p.read_text()))
rows=[]
for species in active:
 row={'species':species,'name':names[species],'assets':{}}
 for kind,table in tables.items():
  symbol=table.get(species);path=defs.get(symbol);status='ok';details=[]
  if not symbol:status='missing table entry'
  elif 'QuestionMark' in symbol:status='placeholder'
  elif not path:status='unresolved symbol'
  source=None
  if path:
   source=re.sub(r'\.(?:4bpp|1bpp)(?:\.lz)?$','.png',path) if kind in ['front','back','icon','footprint'] else re.sub(r'\.gbapal(?:\.lz)?$','.pal',path)
   if not (ROOT/source).exists():status='missing source'
   elif source.endswith('.png'):
    im=Image.open(ROOT/source)
    if kind in ['front','back'] and (im.width!=64 or im.height%64):details.append('unexpected battle dimensions')
    if kind=='icon' and im.size!=(32,64):details.append('unexpected icon dimensions')
    if len(set(im.getdata()))<=1 and kind!='footprint':details.append('blank image')
    if kind in ['front','back','icon'] and (im.mode!='P' or max(im.getdata())>15):details.append('not 4bpp indexed')
   elif len((ROOT/source).read_text().splitlines())<19:details.append('short palette')
  row['assets'][kind]={'status':status,'symbol':symbol,'source':source,'warnings':details}
 rows.append(row)
out=ROOT/'docs/active-pokemon-sprite-audit';out.mkdir(exist_ok=True)
(out/'data.json').write_text(json.dumps(rows,indent=2)+'\n')
counts={k:dict(Counter(r['assets'][k]['status'] for r in rows)) for k in tables}
lines=['# Active Pokémon sprite audit','',f'{len(active)} active species checked. Retired/reserved IDs excluded. Checks compiled-table references, source existence, blank images, palette size and sprite dimensions. This is an asset audit, not a visual or live-game quality certification. Shiny graphics share battle sprites and use separate palettes. Overworld sprites are not required for every species. Blank footprint images can be intentional for species without footprints and are not treated as missing.','', '## Counts','']
for k,c in counts.items():lines.append(f'- {k}: '+', '.join(f'{v} {s}' for s,v in c.items()))
lines+=['','## Missing or placeholder assets','','| Species | Internal species | Assets |','|---|---|---|']
for row in rows:
 issues=[f'{k}: {v["status"]}' for k,v in row['assets'].items() if v['status']!='ok']
 if issues:lines.append(f'| {row["name"]} | {row["species"]} | '+ '; '.join(issues)+' |')
lines+=['','## Other source warnings','']
for row in rows:
 for k,v in row['assets'].items():
  if v['warnings']:lines.append(f'- {row["species"]} {k}: '+', '.join(v['warnings']))
(out/'report.md').write_text('\n'.join(lines)+'\n')
print(json.dumps(counts,indent=2))
for row in rows:
 issues=[k for k,v in row['assets'].items() if v['status']!='ok']
 if issues:print(row['species'],':',','.join(issues))
