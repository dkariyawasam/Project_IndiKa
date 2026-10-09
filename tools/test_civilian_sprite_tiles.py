#!/usr/bin/env python3
"""Verify relocated civilian sheets have frame-aware rules and correct built tiles."""
from pathlib import Path
import re
from PIL import Image
ROOT=Path(__file__).resolve().parents[1]
rules=(ROOT/'spritesheet_rules.mk').read_text()
count=0
for path in sorted((ROOT/'graphics/object_events/pics/people/civilians').glob('*.png')):
 target='$(OBJEVENTGFXDIR)/people/civilians/'+path.stem+'.4bpp:'
 if target not in rules:
  # A few alternate sheets are not registered as standalone build targets.
  continue
 line=rules.split(target,1)[1].splitlines()[1]
 mw=int(re.search(r'-mwidth (\d+)',line)[1]);mh=int(re.search(r'-mheight (\d+)',line)[1]);w=mw*8;h=mh*8
 im=Image.open(path);data=path.with_suffix('.4bpp').read_bytes()
 assert im.width%w==0 and im.height==h,(path,im.size,w,h)
 assert len(data)==im.width*im.height//2,path
 for frame in range(im.width//w):
  for y in range(h):
   for x in range(w):
    off=frame*w*h//2+((y//8)*mw+x//8)*32+(y%8)*4+(x%8)//2
    assert ((data[off]>>(4*(x%2)))&15)==im.getpixel((frame*w+x,y)),(path,frame,x,y)
 count+=1
assert '$(OBJEVENTGFXDIR)/people/civilians/civ_man_2.4bpp:' in rules
print(f'PASS: {count} relocated civilian sprites have correct frame-by-frame tile data.')
