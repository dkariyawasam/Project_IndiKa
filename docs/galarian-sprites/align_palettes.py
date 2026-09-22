from pathlib import Path
from PIL import Image,ImageDraw
import re,json,shutil
R=Path(__file__).resolve().parents[2];D=Path(__file__).parent
# Build colour references from the first 151 species' normal palettes.
names=re.findall(r'^\s*NATIONAL_DEX_(\w+),',(R/'include/constants/pokedex.h').read_text().split('};',1)[0],re.M)
names=[n for n in names if n!='NONE'];names=names[:names.index('MEW')+1]
table=dict(re.findall(r'SPECIES_PAL\((\w+),\s*(\w+)\)',(R/'src/data/pokemon_graphics/palette_table.h').read_text()))
defs=dict(re.findall(r'(\w+)\[\]\s*=\s*INCBIN_\w+\(\s*"([^"]+)"',(R/'src/data/graphics/pokemon.h').read_text()))
def palette(p):return [tuple(map(int,l.split())) for l in p.read_text().splitlines()[3:19]]
pool={}
for name in names:
 path=defs.get(table.get(name,''),'');path=re.sub(r'\.gbapal(?:\.lz)?$','.pal',path)
 if not path:continue
 for c in palette(R/path)[1:]:
  if c!=(255,0,255):pool.setdefault(c,name)
rows=[]
for out in [R/'graphics/pokemon'/e['name'] for e in json.loads((D/'manifest.json').read_text())]:
 if not (out/'normal.pal').exists():continue
 entry={'species':out.name,'palettes':{}};archive=D/'before'/out.name;archive.mkdir(parents=True,exist_ok=True)
 for variant in ['normal','shiny']:
  p=out/(variant+'.pal');shutil.copy2(p,archive/p.name);old=palette(p);new=[old[0]];used=set();mapping={}
  for c in old[1:]:
   if c in mapping:target=mapping[c]
   else:
    target=min((x for x in pool if x not in used),key=lambda x:sum((x[i]-c[i])**2 for i in range(3)))
    mapping[c]=target;used.add(target)
   new.append(target)
  p.write_text('JASC-PAL\n0100\n16\n'+''.join(' '.join(map(str,c))+'\n' for c in new))
  entry['palettes'][variant]=[{'from':a,'to':b,'reference':pool.get(b,'transparent')} for a,b in zip(old,new)]
 normal=palette(out/'normal.pal')
 for side in ['front','back']:
  p=out/(side+'.png');shutil.copy2(p,archive/p.name);a=Image.open(p);data=a.tobytes();a.putpalette([v for c in normal for v in c]+[0]*720);a.save(p,bits=4,transparency=0);assert Image.open(p).tobytes()==data
 rows.append(entry)
(D/'mapping.json').write_text(json.dumps(rows,indent=2)+'\n')
for start in range(0,len(rows),6):
 group=rows[start:start+6];im=Image.new('RGB',(512,len(group)*145),(232,221,190));draw=ImageDraw.Draw(im)
 for row,e in enumerate(group):
  n=e['species'];draw.text((2,row*145),n,fill='black')
  for col,(folder,variant) in enumerate([(D/'before'/n,'normal'),(R/'graphics/pokemon'/n,'normal'),(D/'before'/n,'shiny'),(R/'graphics/pokemon'/n,'shiny')]):
   a=Image.open(folder/'front.png');p=palette(folder/(variant+'.pal'));a.putpalette([v for c in p for v in c]+[0]*720);a.info['transparency']=0;a=a.convert('RGBA').resize((128,128),Image.Resampling.NEAREST);im.paste(a,(128*col,row*145+16),a)
 im.save(D/f'comparison-{start//6+1}.png')
print(len(rows),'regional forms: normal and shiny colours mapped; pixel indices preserved.')
