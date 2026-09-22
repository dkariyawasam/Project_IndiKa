"""Complete existing indexed fronts with HGSS backs and GBA party icons."""
from pathlib import Path
from PIL import Image, ImageDraw
import re
ROOT=Path(__file__).resolve().parents[2]
DOC=Path(__file__).resolve().parent

def pal(p):return [tuple(map(int,l.split())) for l in p.read_text().splitlines()[3:19]]
def indexed(im, palette, opaque=None):
    rgba=im.convert('RGBA');out=Image.new('P',im.size);out.putpalette([v for c in palette for v in c]+[0]*(768-48))
    pixels=[]
    for i,c in enumerate(rgba.get_flattened_data()):
        visible=opaque[i] if opaque is not None else c[3]!=0
        pixels.append(min(range(1,16),key=lambda j:sum((c[k]-palette[j][k])**2 for k in range(3))) if visible else 0)
    out.putdata(pixels);out.info['transparency']=0;return out

def coords(im):
    mask=Image.new('L',im.size);mask.putdata([255 if v else 0 for v in im.get_flattened_data()]);a,b,c,d=mask.getbbox()
    return ((c-a+7)//8*8,(d-b+7)//8*8,(64-(d-b))//2), (a,b,c,d)

preview=Image.new('RGB',(512,288),'#e8e0c8');draw=ImageDraw.Draw(preview)
for row,n in enumerate(('bronzor','bronzong')):
    out=ROOT/'graphics/pokemon'/n;source=DOC/'source'/n
    palette=pal(out/'normal.pal');shiny=pal(out/'shiny.pal')
    # Keep the existing front geometry and color ramps; map HGSS back into them.
    im=Image.open(source/'back.png').convert('RGBA');im=im.crop(im.getbbox())
    if im.width>64 or im.height>64:
        scale=min(64/im.width,64/im.height)
        im=im.resize((round(im.width*scale),round(im.height*scale)),Image.Resampling.NEAREST)
    canvas=Image.new('RGBA',(64,64));canvas.paste(im,((64-im.width)//2,64-im.height))
    back=indexed(canvas,palette);back.save(out/'back.png')
    original=Image.open(source/'icon.png');opaque=[i!=0 for i in original.get_flattened_data()]
    choices=[]
    for slot in range(3):
        ip=pal(ROOT/f'graphics/pokemon/icon_palettes/icon_palette_{slot}.pal')
        icon=indexed(original,ip,opaque)
        rgb=original.convert('RGB');mapped=icon.convert('RGB')
        error=sum(sum((a-b)**2 for a,b in zip(c,d)) for i,(c,d) in enumerate(zip(rgb.get_flattened_data(),mapped.get_flattened_data())) if opaque[i])
        choices.append((error,slot,icon))
    _,slot,icon=min(choices,key=lambda x:x[0]);icon.save(out/'icon.png')
    p=ROOT/'src/pokemon_icon.c';s=p.read_text();s=re.sub(r'(\[SPECIES_'+n.upper()+r'\s*\]\s*=\s*)gMonIcon_QuestionMark',r'\1gMonIcon_'+n.capitalize(),s);s=re.sub(r'(\[SPECIES_'+n.upper()+r'\s*\]\s*=\s*)\d+,',lambda m:m[1]+str(slot)+',',s);p.write_text(s)
    for side in ('front','back'):
        asset=Image.open(out/(side+'.png')); (w,h,y),box=coords(asset)
        y=64-box[3]
        p=ROOT/f'src/data/pokemon_graphics/{side}_pic_coordinates.h';s=p.read_text();s=re.sub(r'(\[SPECIES_'+n.upper()+r'\]\s*=\s*\{).*?(\n    \})',lambda m:m[1]+f'\n        .size = MON_COORDS_SIZE({w}, {h}),\n        .y_offset = {y},'+m[2],s,flags=re.S);p.write_text(s)
        print(n,side,'bounds',box,'coords',w,h,y)
        for variant,colors in enumerate((palette,shiny)):
            view=asset.copy();view.putpalette([v for c in colors for v in c]+[0]*720);view.info['transparency']=0
            x=(variant*2+(side=='back'))*128
            view=view.convert('RGBA').resize((128,128),Image.Resampling.NEAREST);preview.paste(view,(x,row*144+16),view)
    draw.text((0,row*144),n.upper()+'   NORMAL FRONT / BACK       SHINY FRONT / BACK',fill='black')
    print(n,'icon palette',slot)
preview.save(DOC/'battle-preview.png')
