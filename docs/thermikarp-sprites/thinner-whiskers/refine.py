from pathlib import Path
from PIL import Image,ImageDraw
import shutil
root=Path(__file__).resolve().parents[3];doc=Path(__file__).resolve().parent;p=root/'graphics/pokemon/cinnabar_magikarp/front.png'
if not (doc/'before.png').exists():shutil.copyfile(p,doc/'before.png')
a=Image.open(doc/'before.png');b=a.copy()
# Replace only the hanging barbels below the face, retaining their roots.
for y in range(48,60):
 for x in range(19,35):b.putpixel((x,y),0)
for y in range(46,56):
 xs=range(7,10) if y<48 else range(6,9) if y==48 else range(5,15) if y==49 else range(5,16)
 for x in xs:b.putpixel((x,y),0)
d=ImageDraw.Draw(b)
paths=[[(8,46),(7,49),(7,52),(9,54),(12,54),(14,52),(13,50),(11,50),(11,51)],[(22,47),(21,50),(21,53),(22,55),(24,57),(28,57),(31,55),(31,51),(29,50),(27,51),(27,53),(28,54)]]
for path in paths:
 d.line(path,fill=7,width=3)
 d.line(path,fill=8,width=1)
b.info['transparency']=0;b.save(p);b.save(doc/'front.png')
assert a.getpalette()==b.getpalette() and max(b.get_flattened_data())<16
preview=Image.new('RGB',(768,416),'#e8e0c8');d=ImageDraw.Draw(preview)
for i,(label,im) in enumerate([('BEFORE',a),('THINNER WHISKERS',b)]):
 im.info['transparency']=0;r=im.convert('RGBA').resize((384,384),Image.Resampling.NEAREST);preview.paste(r,(384*i,32),r);d.text((384*i+12,10),label,fill='black')
preview.save(doc/'comparison.png')
print('PASS: indexed palette preserved.')
