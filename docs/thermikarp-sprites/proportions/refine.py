from pathlib import Path
from PIL import Image, ImageDraw
import shutil, re
ROOT=Path(__file__).resolve().parents[3]
DOC=Path(__file__).resolve().parent
asset=ROOT/'graphics/pokemon/cinnabar_magikarp/front.png'
backup=DOC/'before.png'
if not backup.exists(): shutil.copyfile(asset,backup)
old=Image.open(backup)
new=Image.new('P',(64,64),0);new.putpalette(old.getpalette());new.info['transparency']=0
# Piecewise nearest-pixel remapping keeps indexed colours exact: more room for
# the torso behind the cheek, a slightly smaller dorsal crest and shorter curls.
def sample(v, pairs):
 for (a,b),(c,d) in zip(pairs,pairs[1:]):
  if a<=v<=c:return b+(v-a)*(d-b)/(c-a)
 return pairs[0][1] if v<pairs[0][0] else pairs[-1][1]
for y in range(64):
 sy=sample(y,[(0,0),(9,6),(20,20),(45,45),(58,59),(63,63)])
 for x in range(64):
  sx=sample(x,[(0,0),(5,5),(17,18),(24,25),(47,43),(59,56),(63,63)])
  # Keep the tall fin narrower than the fuller torso below it.
  if y<23:
   w=max(0,min(1,(23-y)/7))
   fin_sx=sample(x,[(0,0),(17,18),(24,25),(43,43),(59,56),(63,63)])
   sx=sx*(1-w)+fin_sx*w
  new.putpixel((x,y),old.getpixel((min(63,round(sx)),min(63,round(sy)))))
new.save(asset);new.save(DOC/'front.png')
mask=new.point(lambda v:255 if v else 0).convert('L')
# Palette-independent alpha bounds.
mask=Image.new('L',(64,64));mask.putdata([255 if p else 0 for p in new.get_flattened_data()])
a,b,c,d=mask.getbbox()
p=ROOT/'src/data/pokemon_graphics/front_pic_coordinates.h'
s=p.read_text();s=re.sub(r'(\[SPECIES_CINNABAR_MAGIKARP\]\s*=\s*\{).*?(\n    \})',lambda m:m[1]+f'\n        .size = MON_COORDS_SIZE({(c-a+7)//8*8}, {(d-b+7)//8*8}),\n        .y_offset = {64-d},'+m[2],s,flags=re.S);p.write_text(s)
canvas=Image.new('RGB',(768,416),'#e8e0c8');draw=ImageDraw.Draw(canvas)
for i,(label,im) in enumerate([('BEFORE',old),('REBALANCED',new)]):
 im.info['transparency']=0
 rgba=im.convert('RGBA').resize((384,384),Image.Resampling.NEAREST)
 canvas.paste(rgba,(i*384,32),rgba);draw.text((i*384+12,10),label,fill='black')
canvas.save(DOC/'comparison.png')
assert new.mode=='P' and new.size==(64,64) and max(new.get_flattened_data())<16
assert new.getpalette()==old.getpalette()
print('PASS indexed palette preserved; bounds:',(a,b,c,d))
