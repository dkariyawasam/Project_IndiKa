from pathlib import Path
from PIL import Image
root=Path(__file__).resolve().parents[3]
folder=Path(__file__).resolve().parent
original=Image.open(folder/'before.png');out=original.copy()
# Small specular accents like Slowbro/Slowpoke, restricted to pink body pixels.
highlights=[(28,23),(29,23),(28,24),(40,35),(41,35),(40,36),
            (25,46),(26,46),(27,46),(25,47),(40,53),(41,53)]
added=0
for x,y in highlights:
 if original.getpixel((x,y)) in (2,6,7):out.putpixel((x,y),15);added+=1
# Sparse single-pixel dither along torso/limb shadow transitions only.
changed=0
for y in range(44,59):
 for x in range(17,46):
  c=original.getpixel((x,y))
  if c not in (2,3,5,6) or (x+y)%2 or x%2:continue
  ns=[original.getpixel((x+dx,y+dy)) for dx,dy in [(1,0),(-1,0),(0,1),(0,-1)]]
  if any(n in (0,1,8,9,10,11,12,13,14,15) for n in ns):continue
  light=7 if c in (2,6) else 2
  if light in ns:
   out.putpixel((x,y),light);changed+=1
assert out.getpalette()==original.getpalette()
out.save(root/'graphics/pokemon/slowking/front.png',transparency=0)
preview=Image.new('RGBA',(128,64),(224,232,220,255))
for col,im in enumerate([original,out]):
 im.info['transparency']=0;preview.alpha_composite(im.convert('RGBA'),(64*col,0))
preview.resize((768,384),Image.Resampling.NEAREST).save(folder/'comparison.png')
print(added,'highlight pixels;',changed,'dither pixels')
