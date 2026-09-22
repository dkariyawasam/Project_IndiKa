from pathlib import Path
import re,subprocess,sys,json,math
from PIL import Image,ImageDraw
R=Path('/Users/deekariyawasam/pokefirered');sys.path.insert(0,str(R/'tools'));from sync_active_pokedex import derive
D=R/'docs/pokedex-size-chart-fix';D.mkdir(exist_ok=True)
p=R/'src/data/pokemon/pokedex_entries.h';text=p.read_text();orig=subprocess.check_output(['git','show','f3cc06ebc^:src/data/pokemon/pokedex_entries.h'],cwd=R,text=True)
vanilla=set(re.findall(r'\[NATIONAL_DEX_(\w+)\]',orig));active=derive()[0]
pics=dict(re.findall(r'SPECIES_SPRITE\((\w+),\s*(\w+)\)',(R/'src/data/pokemon_graphics/front_pic_table.h').read_text()))
paths=dict(re.findall(r'const u32 (\w+)\[\] = INCBIN_U32\("([^"]+)"\)',(R/'src/data/graphics/pokemon.h').read_text()))
def mask(path):
 im=Image.open(path).crop((0,0,64,64));out=Image.new('L',(64,64));out.putdata([255 if x else 0 for x in im.get_flattened_data()]);return out
trainer=mask(R/'graphics/trainers/front_pics/red_front_pic.png');leaf=mask(R/'graphics/trainers/front_pics/leaf_front_pic.png');tb=trainer.getbbox();lb=leaf.getbbox();th=tb[3]-tb[1];tw=max(tb[2]-tb[0],lb[2]-lb[0]);rows=[];skipped=[];cards=[]
for n in active:
 if n in vanilla:continue
 if 'QuestionMark' in pics.get(n,''):skipped.append(n);continue
 path=paths[pics[n]].replace('.4bpp.lz','.png');mon=mask(R/path);bb=mon.getbbox();h=bb[3]-bb[1];w=bb[2]-bb[0]
 pattern=r'(\[NATIONAL_DEX_'+n+r'\]\s*=\s*\{)(.*?)(\n    \},)';m=re.search(pattern,text,re.S);b=m[2];fields=dict((k,int(v)) for k,v in re.findall(r'\.(height|pokemonScale|pokemonOffset|trainerScale|trainerOffset)\s*=\s*(-?\d+)',b));height=fields['height']/10
 # Tynamo is measured along its horizontal body; standing/posed silhouette height is the other entries' approximation.
 measured=w if n=='TYNAMO' else h
 unit=height/measured
 ppm=min(44/(h*unit),44/1.5,46/(w*unit),30/(tw/th*1.5),1/unit,th/1.5)
 ps=math.ceil(256/(unit*ppm));ts=math.ceil(256/(1.5/th*ppm))
 po=round(126-104-(bb[3]-32)*256/ps);to=round(126-104-(max(tb[3],lb[3])-32)*256/ts)
 new={'pokemonScale':ps,'pokemonOffset':po,'trainerScale':ts,'trainerOffset':to}
 for k,v in new.items():b=re.sub(r'(\.'+k+r'\s*=\s*)-?\d+',lambda m:m[1]+str(v),b)
 text=text[:m.start(2)]+b+text[m.end(2):]
 rows.append({'species':n,'height_m':height,'before':fields,'after':new,'axis':'length' if n=='TYNAMO' else 'posed height'})
 canvas=Image.new('RGB',(320,120),'#e8e0c8');draw=ImageDraw.Draw(canvas)
 for i,f in enumerate([fields,new]):
  draw.text((i*160+3,3),n,fill='black');draw.text((i*160+3,17),('Before' if i==0 else 'After')+' '+str(height)+'m',fill='black')
  for im,scale,offset,x,col in [(mon,f['pokemonScale'],f['pokemonOffset'],40,'#333333'),(trainer,f['trainerScale'],f['trainerOffset'],80,'#666666'),(leaf,f['trainerScale'],f['trainerOffset'],124,'#888888')]:
   scaled=im.transform((64,64),Image.Transform.AFFINE,(scale/256,0,32-32*scale/256,0,scale/256,32-32*scale/256),resample=Image.Resampling.NEAREST)
   canvas.paste(col,(i*160+x-32,65+offset-32),scaled)
 cards.append(canvas)
p.write_text(text)
(D/'calibration.json').write_text(json.dumps({'trainer_height_m':1.5,'updated':rows,'placeholders_skipped':skipped},indent=2)+'\n')
for start in range(0,len(cards),12):
 out=Image.new('RGB',(960,480),'white')
 for i,im in enumerate(cards[start:start+12]):out.paste(im,((i%3)*320,(i//3)*120))
 out.save(D/f'comparison-{start//12+1}.png')
print('Calibrated',len(rows),'real sprites; skipped',len(skipped),'placeholders.');print([r['species'] for r in rows])
