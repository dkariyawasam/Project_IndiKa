from pathlib import Path
from PIL import Image,ImageDraw
R=Path(__file__).resolve().parents[3];D=Path(__file__).resolve().parent
ref=Image.open(D/'generated-reference.png').convert('RGB')
def readpal(p):return [tuple(map(int,l.split())) for l in p.read_text().splitlines()[3:]]
def writepal(p,c):p.write_text('JASC-PAL\n0100\n16\n'+'\n'.join(' '.join(map(str,x)) for x in c)+'\n')
for row,name in enumerate(['tynamo','eelektrik','eelektross']):
 root=R/'graphics/pokemon'/name;pal=readpal(root/'normal.pal');shiny=readpal(root/'shiny.pal')
 for kind in ['normal','shiny']:
  p=root/(kind+'.pal');backup=D/'before'/(name+'-'+kind+'.pal')
  if not backup.exists():backup.write_bytes(p.read_bytes())
 if name!='tynamo':
  ix=14 if name=='eelektrik' else 15
  pal[ix]=(74,139,156)
  # Match the new highlight to the established shiny body ramp.
  base=shiny[6 if name=='eelektrik' else 3]
  shiny[ix]=tuple(round(min(255,c+(255-c)*.23)/255*31)*255//31 for c in base)
 writepal(root/'normal.pal',pal);writepal(root/'shiny.pal',shiny)
 for col,view in enumerate(['front','back']):
  src=Image.open(D/'before'/(name+'-'+view+'.png'));dst=src.copy();changed=0
  for y in range(64):
   for x in range(64):
    old=src.getpixel((x,y));new=old
    rgb=ref.getpixel((int((col*64+x+.5)*ref.width/128),int((row*64+y+.5)*ref.height/192)))
    lum=.2126*rgb[0]+.7152*rgb[1]+.0722*rgb[2]
    # Preserve all outlines, feature colours and alpha; transfer only body-lighting choices.
    interior=0<x<63 and 0<y<63 and all(src.getpixel((x+dx,y+dy))!=0 for dx,dy in [(-1,0),(1,0),(0,-1),(0,1)])
    if interior:
     if name=='tynamo' and old in (1,3,6) and min(rgb)>120:
      new=3 if lum>242 else 6 if lum>218 else 1
     elif name=='eelektrik' and old==6 and rgb[2]>rgb[0]*1.25 and rgb[1]>rgb[0]*1.15:
      new=14 if lum>101 else 6
     elif name=='eelektross' and old in (1,3) and rgb[2]>rgb[0]*1.25 and rgb[1]>rgb[0]*1.15:
      new=15 if lum>98 else 3 if lum>68 else old
    if new!=old:dst.putpixel((x,y),new);changed+=1
  dst.putpalette([c for rgb in pal for c in rgb]+[0]*(768-48));dst.save(root/(view+'.png'),bits=4,transparency=0)
  assert all((src.getpixel((x,y))==0)==(dst.getpixel((x,y))==0) for y in range(64) for x in range(64))
  print(name,view,changed,'shading pixels changed')
# Original/current comparison using their corresponding palettes.
out=Image.new('RGB',(768,864),(224,216,192));d=ImageDraw.Draw(out)
for row,name in enumerate(['tynamo','eelektrik','eelektross']):
 for col,view in enumerate(['front','back']):
  for version in range(2):
   p=D/'before'/(name+'-'+view+'.png') if version==0 else R/'graphics/pokemon'/name/(view+'.png')
   im=Image.open(p);rgba=im.convert('RGBA');rgba.putalpha(Image.eval(im.convert('L'),lambda _:255))
   alpha=Image.new('L',im.size);alpha.putdata([0 if v==0 else 255 for v in im.getdata()]);rgba.putalpha(alpha)
   x=col*384+version*192;y=row*288;d.text((x+4,y+5),name+' '+view+(' before' if version==0 else ' after'),fill=(30,30,30));out.paste(rgba.resize((192,192),Image.Resampling.NEAREST),(x,y+30),rgba.resize((192,192),Image.Resampling.NEAREST))
out.save(D/'comparison.png')
