"""Render the arena's existing map tiles for visual review (not an emulator capture)."""
from pathlib import Path
from PIL import Image
import struct
root=Path(__file__).resolve().parents[2]
a=root/'data/tilesets/primary/building';b=root/'data/tilesets/secondary/pokemon_league'
tiles=a.joinpath('tiles.4bpp').read_bytes().ljust(640*32,b'\0')+b.joinpath('tiles.4bpp').read_bytes()
metas=a.joinpath('metatiles.bin').read_bytes().ljust(512*16,b'\0')+b.joinpath('metatiles.bin').read_bytes()
pal=[]
for i in range(16):
 vals=struct.unpack('<16H',(a if i<7 else b).joinpath(f'palettes/{i:02}.gbapal').read_bytes())
 pal.append([((v&31)*255//31,((v>>5)&31)*255//31,((v>>10)&31)*255//31) for v in vals])
blocks=list(struct.unpack('<169H',(root/'data/layouts/PokemonLeague_BrunosRoom/map.bin').read_bytes()))
for opened in [False,True]:
 blocks[19]=0x28e if opened else 0x28b;blocks[32]=0x296 if opened else 0x293
 im=Image.new('RGB',(208,208))
 for k,block in enumerate(blocks):
  entries=struct.unpack_from('<8H',metas,(block&1023)*16)
  for n,t in enumerate(entries):
   tile=t&1023;data=tiles[tile*32:tile*32+32]
   for y in range(8):
    for x in range(8):
     px=7-x if t&1024 else x;py=7-y if t&2048 else y
     c=(data[py*4+px//2]>>(4*(px%2)))&15
     if n<4 or c: im.putpixel((k%13*16+n%2*8+x,k//13*16+(n%4)//2*8+y),pal[t>>12][c])
 im.resize((624,624),Image.Resampling.NEAREST).save(root/f'docs/indigo-league/arena-{"open" if opened else "closed"}.png')
