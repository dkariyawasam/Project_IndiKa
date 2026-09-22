"""Import pinned GBA sprite assets; pass source checkout as first argument."""
from pathlib import Path
from PIL import Image,ImageDraw
import re,sys,shutil,json
R=Path(__file__).resolve().parents[2];D=Path(__file__).parent;S=Path(sys.argv[1])/'graphics/pokemon'
bases=['rattata','raticate','raichu','sandshrew','sandslash','diglett','dugtrio','meowth','persian','geodude','graveler','golem','grimer','muk','vulpix','ninetales','exeggutor','marowak']
def read(p):return (R/p).read_text()
def pal(p):return [tuple(map(int,l.split())) for l in p.read_text().splitlines()[3:19]]
files={k:read('src/data/pokemon_graphics/'+v) for k,v in [('front','front_pic_table.h'),('back','back_pic_table.h'),('normal','palette_table.h'),('shiny','shiny_palette_table.h'),('footprint','footprint_table.h')]}
paths=dict(zip(files,['front_pic_table.h','back_pic_table.h','palette_table.h','shiny_palette_table.h','footprint_table.h']))
icons=read('src/pokemon_icon.c');defs=read('src/data/graphics/pokemon.h');header=read('include/graphics.h');decls=[];coords={k:read('src/data/pokemon_graphics/'+k+'_pic_coordinates.h') for k in ['front','back']};rows=[]
def map_symbol(kind,species,symbol):
 global icons
 if kind=='icon':icons=re.sub(r'(\[SPECIES_'+species+r'\s*\]\s*=\s*)gMonIcon_\w+',lambda m:m[1]+symbol,icons)
 elif kind=='footprint':files[kind]=re.sub(r'(\[SPECIES_'+species+r'\s*\]\s*=\s*)\w+',lambda m:m[1]+symbol,files[kind])
 else:files[kind]=re.sub(r'(SPECIES_\w+\('+species+r',\s*)\w+',lambda m:m[1]+symbol,files[kind])
for base in ['roserade']+bases:
 isrose=base=='roserade';name=base if isrose else base+'_alolan';species=name.upper();symbol='Roserade' if isrose else 'Alolan'+base.capitalize();src=S/base if isrose else S/base/'alola';out=R/'graphics/pokemon'/name;out.mkdir(exist_ok=True);archive=D/'source'/name;archive.mkdir(parents=True,exist_ok=True)
 for f in ['anim_front.png','back.png','icon.png','normal.pal','shiny.pal']:shutil.copy2(src/f,archive/f)
 for side in ['front','back']:
  im=Image.open(src/('anim_front.png' if side=='front' else 'back.png')).crop((0,0,64,64));im.info['transparency']=0;im.save(out/(side+'.png'),bits=4,transparency=0)
  mask=Image.new('L',im.size);mask.putdata([255 if i else 0 for i in im.getdata()]);box=mask.getbbox();assert box
  w=(box[2]-box[0]+7)//8*8;h=(box[3]-box[1]+7)//8*8;y=64-box[3]
  coords[side]=re.sub(r'(\[SPECIES_'+species+r'\]\s*=\s*\{).*?(\n    \})',lambda m:m[1]+f'\n        .size = MON_COORDS_SIZE({w}, {h}),\n        .y_offset = {y},'+m[2],coords[side],flags=re.S)
 for variant in ['normal','shiny']:shutil.copy2(src/(variant+'.pal'),out/(variant+'.pal'))
 a=Image.open(src/'icon.png');assert a.size==(32,64);rgb=a.convert('RGB');pixels=list(a.getdata());options=[]
 for slot in range(3):
  colors=pal(R/f'graphics/pokemon/icon_palettes/icon_palette_{slot}.pal');lookup={c:min(range(1,16),key=lambda i:sum((c[j]-colors[i][j])**2 for j in range(3))) for c in set(rgb.getdata())};data=[lookup[c] if v else 0 for c,v in zip(rgb.getdata(),pixels)];error=sum(sum((c[j]-colors[i][j])**2 for j in range(3)) for c,i in zip(rgb.getdata(),data) if i);options.append((error,slot,colors,data))
 _,slot,colors,data=min(options,key=lambda x:x[0]);im=Image.new('P',(32,64));im.putpalette([v for c in colors for v in c]+[0]*720);im.putdata(data);im.save(out/'icon.png',bits=4,transparency=0)
 icons=re.sub(r'(\[SPECIES_'+species+r'\s*\]\s*=\s*)\d+,',lambda m:m[1]+str(slot)+',',icons)
 for kind,prefix,ctype,suffix in [('front','gMonFrontPic_','u32','front.4bpp.lz'),('back','gMonBackPic_','u32','back.4bpp.lz'),('normal','gMonPalette_','u32','normal.gbapal.lz'),('shiny','gMonShinyPalette_','u32','shiny.gbapal.lz'),('icon','gMonIcon_','u8','icon.4bpp')]:
  sym=prefix+symbol;map_symbol(kind,species,sym);defs+=f'\nconst {ctype} {sym}[] = INCBIN_{ctype.upper()}("graphics/pokemon/{name}/{suffix}");';decls.append(f'extern const {ctype} {sym}[];')
 if not isrose:
  standard=re.search(r'\[SPECIES_'+base.upper()+r'\]\s*=\s*(\w+)',files['footprint'])[1];map_symbol('footprint',species,standard)
 rows.append({'species':species,'name':name,'icon_palette':slot})
# User-approved standard appearances for pre-evolutions. Share actual assets.
for base in ['pichu','pikachu','exeggcute','cubone']:
 species=base.upper()+'_ALOLAN'
 for kind in files:
  if kind=='footprint':sym=re.search(r'\[SPECIES_'+base.upper()+r'\]\s*=\s*(\w+)',files[kind])[1]
  else:sym=re.search(r'SPECIES_\w+\('+base.upper()+r',\s*(\w+)',files[kind])[1]
  map_symbol(kind,species,sym)
 sym=re.search(r'\[SPECIES_'+base.upper()+r'\s*\]\s*=\s*(gMonIcon_\w+)',icons)[1];map_symbol('icon',species,sym)
 slot=re.search(r'\[SPECIES_'+base.upper()+r'\s*\]\s*=\s*(\d+),',icons)[1];icons=re.sub(r'(\[SPECIES_'+species+r'\s*\]\s*=\s*)\d+,',lambda m:m[1]+slot+',',icons)
 for side in coords:
  body=re.search(r'\[SPECIES_'+base.upper()+r'\]\s*=\s*(\{.*?\n    \})',coords[side],re.S)[1];coords[side]=re.sub(r'(\[SPECIES_'+species+r'\]\s*=\s*)\{.*?\n    \}',lambda m:m[1]+body,coords[side],flags=re.S)
for kind,s in files.items():(R/'src/data/pokemon_graphics'/paths[kind]).write_text(s)
for side,s in coords.items():(R/f'src/data/pokemon_graphics/{side}_pic_coordinates.h').write_text(s)
(R/'src/pokemon_icon.c').write_text(icons);(R/'src/data/graphics/pokemon.h').write_text(defs+'\n');pos=header.rfind('#endif');header=header[:pos]+'\n'+'\n'.join(decls)+'\n\n'+header[pos:];(R/'include/graphics.h').write_text(header)
(D/'manifest.json').write_text(json.dumps(rows,indent=2)+'\n')
for start in range(0,len(rows),5):
 group=rows[start:start+5];sheet=Image.new('RGB',(640, len(group)*150),(232,221,190));draw=ImageDraw.Draw(sheet)
 for row,entry in enumerate(group):
  out=R/'graphics/pokemon'/entry['name'];draw.text((4,row*150+2),entry['species'],fill='black')
  for col,(side,variant) in enumerate([('front','normal'),('back','normal'),('front','shiny'),('back','shiny')]):
   a=Image.open(out/(side+'.png'));p=pal(out/(variant+'.pal'));a.putpalette([v for c in p for v in c]+[0]*720);a.info['transparency']=0;a=a.convert('RGBA').resize((128,128),Image.Resampling.NEAREST);sheet.paste(a,(col*128,row*150+20),a)
  a=Image.open(out/'icon.png').convert('RGBA').resize((64,128),Image.Resampling.NEAREST);sheet.paste(a,(550,row*150+20),a)
 sheet.save(D/f'preview-{start//5+1}.png')
print('Imported Roserade and 18 Alolan forms; linked four standard pre-evolution appearances.')
