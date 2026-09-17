from pathlib import Path
from PIL import Image
import re,struct,subprocess,json,os
ROOT = Path(__file__).resolve().parents[1]
os.chdir(ROOT)
rom=Path('pokefirered.gba').read_bytes();out=subprocess.check_output(['/opt/devkitpro/devkitARM/bin/arm-none-eabi-nm','pokefirered.elf'],text=True);syms={p[2]:int(p[0],16)-0x8000000 for l in out.splitlines() if len(p:=l.split())==3}
def lz(addr):
 size=int.from_bytes(rom[addr+1:addr+4],'little');pos=addr+4;b=bytearray();assert rom[addr]==16
 while len(b)<size:
  flags=rom[pos];pos+=1
  for bit in range(7,-1,-1):
   if len(b)>=size:break
   if flags&(1<<bit):
    a,c=rom[pos:pos+2];pos+=2;length=(a>>4)+3;dist=((a&15)<<8|c)+1
    for _ in range(length):b.append(b[-dist])
   else:b.append(rom[pos]);pos+=1
 return b[:size]
s=Path('src/data/graphics/trainers.h').read_text();paths={name:Path(path.replace('.4bpp.lz','.png')) for name,path in re.findall(r'const u32 (\w+)\[\] = INCBIN_U32\("([^"]+)"',s)}
t=Path('src/data/trainer_graphics/front_pic_tables.h').read_text();pics=dict(re.findall(r'TRAINER_SPRITE\((\w+), (\w+),',t));pals=dict(re.findall(r'TRAINER_PAL\((\w+), (\w+)\)',t));result=[]
for key,pic in pics.items():
 if key not in pals:continue
 pal=pals[key];im=Image.open(paths[pic]);pngpal=im.getpalette()
 if not pngpal:continue
 colors=struct.unpack('<16H',lz(syms[pal]));bad=[]
 for idx in sorted(set(im.tobytes())-{0}):
  rgb=pngpal[idx*3:idx*3+3];expected=sum((v>>3)<<(5*k) for k,v in enumerate(rgb))
  if colors[idx]!=expected:bad.append(idx)
 result.append({'pic':key,'palette':pal,'mismatched_indices':bad})
print('Battle pairs',len(result));print(json.dumps([r for r in result if r['mismatched_indices']],indent=2))
report = ROOT / 'docs/trainer-palette-audit/battle-palettes.json'
report.parent.mkdir(parents=True, exist_ok=True)
report.write_text(json.dumps(result,indent=2)+'\n')
assert not any(r['mismatched_indices'] for r in result), 'Battle palette mismatch'
