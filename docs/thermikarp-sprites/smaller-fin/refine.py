from pathlib import Path
from PIL import Image,ImageDraw
import shutil
root=Path(__file__).resolve().parents[3];doc=Path(__file__).resolve().parent
p=root/'graphics/pokemon/cinnabar_magikarp/front.png'
if not (doc/'before.png').exists():shutil.copyfile(p,doc/'before.png')
a=Image.open(doc/'before.png');b=a.copy()
starts={17:27,18:27,19:28,20:30,21:32,22:33,23:34,24:35,25:36,26:37,27:38,28:39,29:40,30:40,31:41}
mask=set()
for y in range(5,32):
 for x in range(starts.get(y,20),51):
  if a.getpixel((x,y)) in (1,7,8,10,13):mask.add((x,y))
# Scale the isolated fin towards its lower attachment, then keep body pixels intact.
for x,y in mask:b.putpixel((x,y),0)
for y in range(5,33):
 for x in range(20,51):
  sx=round(22+(x-22)/0.84);sy=round(32+(y-32)/0.78)
  if (sx,sy) in mask and ((x,y) in mask or a.getpixel((x,y))==0):
   b.putpixel((x,y),a.getpixel((sx,sy)))
b.info['transparency']=0;b.save(p);b.save(doc/'front.png')
preview=Image.new('RGB',(768,416),'#e8e0c8');d=ImageDraw.Draw(preview)
for i,(label,im) in enumerate([('BEFORE',a),('SMALLER BACK FIN',b)]):
 im.info['transparency']=0;r=im.convert('RGBA').resize((384,384),Image.Resampling.NEAREST);preview.paste(r,(384*i,32),r);d.text((384*i+12,10),label,fill='black')
preview.save(doc/'comparison.png')
assert b.getpalette()==a.getpalette() and max(b.get_flattened_data())<16
assert all(a.getpixel((x,y))==b.getpixel((x,y)) for y in range(64) for x in range(64) if (x,y) not in mask and a.getpixel((x,y))!=0)
print('PASS: only dorsal fin pixels changed; palette preserved.')
