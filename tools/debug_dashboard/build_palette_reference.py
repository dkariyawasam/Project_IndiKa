"""Freeze the first 151 vanilla FireRed used palettes from local pret history."""
import io,json,subprocess
from pathlib import Path
from collections import Counter
from PIL import Image
ROOT=Path(__file__).resolve().parents[2]
REV='037335f4c725d7c9aecdac87066f2002b4bd7e14'
def read(path):return subprocess.check_output(['git','show',REV+':'+path],cwd=ROOT)
import re
ids={k:int(v) for k,v in re.findall(r'#define SPECIES_(\w+)\s+(\d+)',read('include/constants/species.h').decode()) if 1<=int(v)<=151}
graphics=dict(re.findall(r'(\w+)\[\]\s*=\s*INCBIN_U\d+\(\s*"([^"]+)"',read('src/data/graphics/pokemon.h').decode()))
def table(file):return dict(re.findall(r'SPECIES_\w+\((\w+),\s*(\w+)\)',read('src/data/pokemon_graphics/'+file).decode()))
front=table('front_pic_table.h');normal=table('palette_table.h');shiny=table('shiny_palette_table.h')
rows=[]
for k in ids:
 path=graphics[front[k]].replace('.4bpp.lz','.png');im=Image.open(io.BytesIO(read(path))).crop((0,0,64,64));counts=Counter(im.getdata());counts.pop(0,None)
 palettes={}
 for label,t in [('normal',normal),('shiny',shiny)]:
  p=graphics[t[k]].replace('.gbapal.lz','.pal');palettes[label]=[list(map(int,l.split())) for l in read(p).decode().splitlines()[3:19]]
 rows.append(dict(key=k,counts=dict(counts),**palettes))
assert len(rows)==151
(ROOT/'tools/debug_dashboard/firered_palette_reference.json').write_text(json.dumps(dict(repository='https://github.com/pret/pokefirered',revision=REV,species=rows),separators=(',',':'))+'\n')
