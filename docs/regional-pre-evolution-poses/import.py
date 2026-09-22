from pathlib import Path
from PIL import Image,ImageDraw
import re,json
R=Path(__file__).resolve().parents[2];D=Path(__file__).parent
items=[('pichu','PICHU_ALOLAN'),('pikachu','PIKACHU_ALOLAN'),('exeggcute','EXEGGCUTE_ALOLAN'),('cubone','CUBONE_ALOLAN'),('koffing','KOFFING_GALARIAN')]
def read(p):return (R/p).read_text()
def pal(p):return [tuple(map(int,l.split())) for l in p.read_text().splitlines()[3:19]]
def symbol(table,base):return re.search(r'SPECIES_\w+\('+base.upper()+r',\s*(\w+)',table)[1]
front=read('src/data/pokemon_graphics/front_pic_table.h');back=read('src/data/pokemon_graphics/back_pic_table.h');normal=read('src/data/pokemon_graphics/palette_table.h');shiny=read('src/data/pokemon_graphics/shiny_palette_table.h');foot=read('src/data/pokemon_graphics/footprint_table.h');icons=read('src/pokemon_icon.c');defs=read('src/data/graphics/pokemon.h');header=read('include/graphics.h');coordinates={s:read('src/data/pokemon_graphics/'+s+'_pic_coordinates.h') for s in ['front','back']};decl=[]
preview=Image.new('RGB',(640,750),(232,221,190));draw=ImageDraw.Draw(preview)
for row,(base,species) in enumerate(items):
 out=R/'graphics/pokemon'/species.lower();out.mkdir(exist_ok=True);colors=pal(R/f'graphics/pokemon/{base}/normal.pal');draw.text((0,row*150),species,fill='black')
 for side in ['front','back']:
  a=Image.open(D/f'source/{base}/{side}.png').convert('RGBA');a=a.crop(a.getbbox())
  bound=60 if side=='front' else 64
  if max(a.size)>bound:
   scale=bound/max(a.size);a=a.resize((round(a.width*scale),round(a.height*scale)),Image.Resampling.NEAREST)
  canvas=Image.new('RGBA',(64,64));canvas.paste(a,((64-a.width)//2,(64-a.height)//2 if side=='front' else 64-a.height))
  data=[]
  for c in canvas.getdata():data.append(0 if c[3]<128 else min(range(1,16),key=lambda i:sum((c[k]-colors[i][k])**2 for k in range(3))))
  if base=='pichu' and side=='back':
   yellow_roles={(197,172,16):3,(246,230,82):2,(164,123,8):4,(98,74,16):6}
   data=[yellow_roles.get(c[:3],v) if c[3]>=128 else 0 for c,v in zip(canvas.getdata(),data)]
  im=Image.new('P',(64,64));im.putpalette([v for c in colors for v in c]+[0]*720);im.putdata(data);im.save(out/(side+'.png'),bits=4,transparency=0)
  sym='gMon'+('Front' if side=='front' else 'Back')+'Pic_RegionalPose'+base.capitalize()
  line=f'const u32 {sym}[] = INCBIN_U32("graphics/pokemon/{species.lower()}/{side}.4bpp.lz");'
  if line not in defs:defs+='\n'+line
  decl.append('extern const u32 '+sym+'[];')
  if side=='front':front=re.sub(r'(SPECIES_SPRITE\('+species+r',\s*)\w+',lambda m:m[1]+sym,front)
  else:back=re.sub(r'(SPECIES_SPRITE\('+species+r',\s*)\w+',lambda m:m[1]+sym,back)
  w=(a.width+7)//8*8;h=(a.height+7)//8*8;y=(64-a.height)//2 if side=='front' else 0
  coordinates[side]=re.sub(r'(\[SPECIES_'+species+r'\]\s*=\s*\{).*?(\n    \})',lambda m:m[1]+f'\n        .size = MON_COORDS_SIZE({w}, {h}),\n        .y_offset = {y},'+m[2],coordinates[side],flags=re.S)
  for col,source in [(0 if side=='front' else 2,R/f'graphics/pokemon/{base}/{side}.png'),(1 if side=='front' else 3,out/(side+'.png'))]:
   view=Image.open(source).convert('RGBA').resize((128,128),Image.Resampling.NEAREST);preview.paste(view,(col*160,row*150+20),view)
 for table_name in ['normal','shiny']:
  table=normal if table_name=='normal' else shiny;sym=symbol(table,base);table=re.sub(r'(SPECIES_\w+\('+species+r',\s*)\w+',lambda m:m[1]+sym,table)
  if table_name=='normal':normal=table
  else:shiny=table
 fs=re.search(r'\[SPECIES_'+base.upper()+r'\]\s*=\s*(\w+)',foot)[1];foot=re.sub(r'(\[SPECIES_'+species+r'\]\s*=\s*)\w+',lambda m:m[1]+fs,foot)
 ins=re.search(r'\[SPECIES_'+base.upper()+r'\s*\]\s*=\s*(gMonIcon_\w+)',icons)[1];icons=re.sub(r'(\[SPECIES_'+species+r'\s*\]\s*=\s*)gMonIcon_\w+',lambda m:m[1]+ins,icons)
 slot=re.search(r'\[SPECIES_'+base.upper()+r'\s*\]\s*=\s*(\d+),',icons)[1];icons=re.sub(r'(\[SPECIES_'+species+r'\s*\]\s*=\s*)\d+,',lambda m:m[1]+slot+',',icons)
for file,s in [('front_pic_table.h',front),('back_pic_table.h',back),('palette_table.h',normal),('shiny_palette_table.h',shiny),('footprint_table.h',foot)]: (R/'src/data/pokemon_graphics'/file).write_text(s)
for side,s in coordinates.items():(R/f'src/data/pokemon_graphics/{side}_pic_coordinates.h').write_text(s)
(R/'src/data/graphics/pokemon.h').write_text(defs+'\n');(R/'src/pokemon_icon.c').write_text(icons);pos=header.rfind('#endif');header=header[:pos]+'\n'.join(v for v in decl if v not in header)+'\n'+header[pos:];(R/'include/graphics.h').write_text(header)
preview.save(D/'comparison.png');print('Five regional pre-evolution front/back pose sets imported; base palettes, icons and footprints shared.')
