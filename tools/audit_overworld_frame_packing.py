#!/usr/bin/env python3
"""Compare every declared overworld_frame against its indexed source PNG in ROM."""
from pathlib import Path
import json,re,subprocess,os,shutil
from PIL import Image
ROOT=Path(__file__).resolve().parents[1]
rom=(ROOT/'pokefirered.gba').read_bytes()
nm=shutil.which('arm-none-eabi-nm') or '/opt/devkitpro/devkitARM/bin/arm-none-eabi-nm'
out=subprocess.check_output([nm,str(ROOT/'pokefirered.elf')],text=True)
symbols={p[2]:int(p[0],16)-0x08000000 for line in out.splitlines() if len(p:=line.split())==3}
graphics=(ROOT/'src/data/object_events/object_event_graphics.h').read_text()
paths={name:Path(path).with_suffix('.png') for name,path in re.findall(r'(\w+)\[\]\s*=\s*INCBIN_U(?:8|16|32)\("([^"]+\.4bpp)"',graphics)}
frames=sorted(set((name,int(w),int(h),int(f)) for name,w,h,f in re.findall(r'overworld_frame\((\w+),\s*(\d+),\s*(\d+),\s*(\d+)\)',(ROOT/'src/data/object_events/object_event_pic_tables.h').read_text())))
results={};skipped=[]
for name,tw,th,frame in frames:
 if name not in paths or name not in symbols:
  skipped.append([name,frame,'no compiled image/source mapping']);continue
 path=paths[name];im=Image.open(ROOT/path)
 if im.mode!='P':skipped.append([name,frame,'not indexed']);continue
 w,h=tw*8,th*8;cols=im.width//w
 if not (cols and im.width%w==0 and im.height%h==0):
  skipped.append([name,frame,'special frame geometry']);continue
 result=results.setdefault(str(path),{'symbol':name,'tile_width':tw,'tile_height':th,'frames':0,'bad_pixels':0,'bad_frames':[]})
 sx=(frame%cols)*w;sy=(frame//cols)*h
 assert sy+h<=im.height,(path,frame)
 start=symbols[name]+frame*tw*th*32;bad=0
 for y in range(h):
  for x in range(w):
   offset=start+((y//8)*tw+x//8)*32+(y%8)*4+(x%8)//2
   bad+=((rom[offset]>>((x&1)*4))&15)!=im.getpixel((sx+x,sy+y))
 result['frames']+=1;result['bad_pixels']+=bad
 if bad:result['bad_frames'].append(frame)
report={'sheets':results,'skipped':skipped}
p=ROOT/'docs/overworld-frame-packing/audit.json';p.parent.mkdir(parents=True,exist_ok=True);p.write_text(json.dumps(report,indent=2)+'\n')
print('Checked',len(results),'sheets,',sum(r['frames'] for r in results.values()),'frames; skipped',len(skipped))
for path,r in results.items():
 if r['bad_pixels']:print(path,r['bad_pixels'],'incorrect pixels; frames',r['bad_frames'])
raise SystemExit(any(r['bad_pixels'] for r in results.values()))
