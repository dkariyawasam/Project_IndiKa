from pathlib import Path
from PIL import Image,ImageDraw
root=Path(__file__).resolve().parents[3];doc=Path(__file__).resolve().parent
# Restore the dorsal fin from the snapshot preceding the misunderstood edit.
a=Image.open(root/'docs/thermikarp-sprites/smaller-fin/before.png');a.save(doc/'before.png');b=a.copy()
starts={31:49,32:49,33:49,34:49,35:48,36:47,37:47,38:47,39:46,40:46,41:46,42:46,43:44,44:43,45:43}
mask={(x,y) for y in range(31,55) for x in range(starts.get(y,44),60) if a.getpixel((x,y))}
for xy in mask:b.putpixel(xy,0)
for y in range(31,55):
 for x in range(43,60):
  sx=round(46+(x-46)/0.8);sy=round(41+(y-41)/0.78)
  if (sx,sy) in mask and ((x,y) in mask or not a.getpixel((x,y))):b.putpixel((x,y),a.getpixel((sx,sy)))
b.info['transparency']=0;b.save(root/'graphics/pokemon/cinnabar_magikarp/front.png');b.save(doc/'front.png')
assert a.getpalette()==b.getpalette() and max(b.get_flattened_data())<16
assert all(a.getpixel((x,y))==b.getpixel((x,y)) for y in range(31) for x in range(64))
preview=Image.new('RGB',(768,416),'#e8e0c8');draw=ImageDraw.Draw(preview)
for i,(label,im) in enumerate([('ORIGINAL TAIL',a),('SMALLER TAIL',b)]):
 im.info['transparency']=0;r=im.convert('RGBA').resize((384,384),Image.Resampling.NEAREST);preview.paste(r,(384*i,32),r);draw.text((384*i+12,10),label,fill='black')
preview.save(doc/'comparison.png')
print('PASS: dorsal fin restored; palette preserved; tail reduced.')
