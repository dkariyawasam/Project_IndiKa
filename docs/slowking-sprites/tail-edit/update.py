from pathlib import Path
from PIL import Image
root=Path(__file__).resolve().parents[3]
source=Path(__file__).resolve().parent
pal=(root/'graphics/pokemon/slowking/normal.pal').read_text().splitlines()
ref=(root/'graphics/pokemon/slowbro/normal.pal').read_text().splitlines()
for a,b in [(11,9),(12,8),(13,7),(14,6)]:pal[a+3]=ref[b+3]
(root/'graphics/pokemon/slowking/normal.pal').write_text('\n'.join(pal)+'\n')
colours=[int(v) for l in pal[3:] for v in l.split()]
for view in ['front','back']:
 im=Image.open(source/(view+'.png'));out=im.copy()
 if view=='front':
  # Open out the exposed tail from its attachment point, retaining native
  # pixel rows and the tip outline instead of stretching the whole body.
  for y in range(43,55):
   hinge=45 if y<46 else 47
   xs=[x for x in range(hinge,64) if im.getpixel((x,y))]
   if not xs:continue
   right=max(xs);oldwidth=right-hinge+1
   extra=2 if y in (43,44,53,54) else 3
   for x in range(hinge,right+extra+1):
    sx=hinge+min(oldwidth-1,(x-hinge)*oldwidth//(oldwidth+extra))
    out.putpixel((x,y),im.getpixel((sx,y)))
  assert out.crop((0,0,64,43)).tobytes()==im.crop((0,0,64,43)).tobytes()
  assert out.crop((0,55,64,64)).tobytes()==im.crop((0,55,64,64)).tobytes()
 out.putpalette(colours+[0]*(768-len(colours)))
 out.save(root/f'graphics/pokemon/slowking/{view}.png',transparency=0)
 if view=='front':out.convert('RGBA').resize((512,512),Image.Resampling.NEAREST).save(source/'preview.png')
