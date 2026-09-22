from pathlib import Path
from PIL import Image
root=Path(__file__).resolve().parents[3]
folder=Path(__file__).resolve().parent
preview=Image.new('RGBA',(128,128),(224,232,220,255))
for row,name in enumerate(['houndour','houndoom']):
 original=Image.open(folder/f'{name}-before.png');out=original.copy();count=0
 for y in range(2,62):
  for x in range(2,62):
   c=original.getpixel((x,y))
   if c not in (4,5) or (x+y)%2:continue
   # Avoid the external silhouette and black linework; soften only the
   # interior boundary between successive body shadow shades.
   near=[original.getpixel((x+dx,y+dy)) for dx,dy in [(1,0),(-1,0),(0,1),(0,-1)]]
   if 0 in near or 15 in near:continue
   light=c-1
   close=light in near
   if close:
    out.putpixel((x,y),light);count+=1
 # Break up the hard edges of the lighter grey body patches as well.
 for y in range(2,62):
  for x in range(2,62):
   c=original.getpixel((x,y))
   if c not in (2,3) or (x+y)%2 != 1:continue
   near=[original.getpixel((x+dx,y+dy)) for dx,dy in [(1,0),(-1,0),(0,1),(0,-1)]]
   if any(v in (0,1,14,15) for v in near):continue
   darker=c+1
   close=darker in near
   if close:
    out.putpixel((x,y),darker);count+=1
 assert out.getpalette()==original.getpalette()
 assert all(a==b for a,b in zip(original.getdata(),out.getdata()) if a not in (2,3,4,5))
 out.save(root/f'graphics/pokemon/{name}/front.png',transparency=0)
 for col,im in enumerate([original,out]):
  im.info['transparency']=0;preview.alpha_composite(im.convert('RGBA'),(col*64,row*64))
 print(name,count,'interior shadow pixels dithered')
preview.resize((768,768),Image.Resampling.NEAREST).save(folder/'comparison.png')
