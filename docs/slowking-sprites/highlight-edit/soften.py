from pathlib import Path
from PIL import Image
root=Path(__file__).resolve().parents[3]
for view in ['front','back']:
 im=Image.open(Path(__file__).with_name(view+'.png'));out=im.copy();changed=0
 for y in range(64):
  for x in range(64):
   if im.getpixel((x,y))!=15:continue
   shell=(view=='front' and y<=19 and (x,y)!=(32,10)) or (view=='back' and y<=30)
   snout=view=='front' and ((y==27 and 27<=x<=29) or (y==28 and 27<=x<=28) or (y==29 and 25<=x<=28))
   if shell or snout:
    out.putpixel((x,y),14 if shell else 10);changed+=1
 assert out.getpalette()==im.getpalette()
 out.save(root/f'graphics/pokemon/slowking/{view}.png',transparency=0)
 if view=='front':out.convert('RGBA').resize((512,512),Image.Resampling.NEAREST).save(Path(__file__).with_name('preview.png'))
 print(view,changed,'highlight pixels softened')
