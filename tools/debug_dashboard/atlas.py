"""Render compiled FRLG map tiles and place edge-connected maps in tile space."""
import struct
import zlib
import threading
from collections import deque
from catalog import load_symbols


def arrange(maps):
    by_name = {m['name']: m for m in maps}
    by_id = {m['id']: m['name'] for m in maps}
    edges = {name: [] for name in by_name}
    for m in maps:
        for c in m['connections']:
            other = by_id.get(c['map'])
            if not other: continue
            n = by_name[other]; offset = int(c['offset'])
            delta = {'up': (offset, -n['height']), 'down': (offset, m['height']),
                     'left': (-n['width'], offset), 'right': (m['width'], offset)}.get(c['direction'])
            if delta:
                dx, dy = delta
                edges[m['name']].append((other, dx, dy))
                edges[other].append((m['name'], -dx, -dy))
    positions, groups, conflicts = {}, [], set()
    for seed in by_name:
        if seed in positions: continue
        positions[seed] = (0, 0); queue = deque([seed]); members = []
        while queue:
            name = queue.popleft(); members.append(name)
            x,y = positions[name]
            for other, dx,dy in edges[name]:
                desired = (x+dx,y+dy)
                if other not in positions:
                    positions[other] = desired; queue.append(other)
                elif positions[other] != desired:
                    conflicts.add(tuple(sorted((name,other))))
        x0=min(positions[n][0] for n in members); y0=min(positions[n][1] for n in members)
        x1=max(positions[n][0]+by_name[n]['width'] for n in members)
        y1=max(positions[n][1]+by_name[n]['height'] for n in members)
        groups.append(dict(names=members,width=x1-x0,height=y1-y0))
        for name in members:
            x,y=positions[name];positions[name]=(x-x0,y-y0)
    groups.sort(key=lambda g: -len(g['names']))
    x=y=row_height=0; placed=[]
    for i,g in enumerate(groups):
        if x and x+g['width']>650: x=0;y+=row_height+18;row_height=0
        g.update(id=i,x=x,y=y,label=('Connected region' if len(g['names'])>1 else g['names'][0]))
        for name in g['names']:
            px,py=positions[name];m=by_name[name]
            placed.append(dict(name=name,x=x+px,y=y+py,width=m['width'],height=m['height'],component=i))
        x+=g['width']+18;row_height=max(row_height,g['height'])
    overlaps=[]
    for i,a in enumerate(placed):
        for b in placed[i+1:]:
            if a['component']==b['component'] and max(a['x'],b['x'])<min(a['x']+a['width'],b['x']+b['width']) and max(a['y'],b['y'])<min(a['y']+a['height'],b['y']+b['height']):
                overlaps.append([a['name'],b['name']])
    return dict(maps=placed,components=groups,conflicts=sorted(conflicts),overlaps=overlaps)


def lz77(data):
    if data[0]!=0x10: raise ValueError('Unsupported tileset compression')
    size=int.from_bytes(data[1:4],'little'); out=bytearray();pos=4
    while len(out)<size:
        flags=data[pos];pos+=1
        for bit in range(7,-1,-1):
            if len(out)>=size:break
            if flags & (1<<bit):
                a,b=data[pos:pos+2];pos+=2
                count=(a>>4)+3;back=((a&15)<<8|b)+1
                if back>len(out):raise ValueError('Invalid tileset backreference')
                for _ in range(min(count,size-len(out))):out.append(out[-back])
            else:out.append(data[pos]);pos+=1
    return bytes(out)


def png(width,height,rgb):
    def chunk(kind,body):return struct.pack('>I',len(body))+kind+body+struct.pack('>I',zlib.crc32(kind+body)&0xffffffff)
    scan=b''.join(b'\0'+rgb[y*width*3:(y+1)*width*3] for y in range(height))
    return b'\x89PNG\r\n\x1a\n'+chunk(b'IHDR',struct.pack('>IIBBBBB',width,height,8,2,0,0,0))+chunk(b'IDAT',zlib.compress(scan))+chunk(b'IEND',b'')


class AtlasRenderer:
    def __init__(self,directory,catalog):
        self.rom=(directory/'expedition-debug.gba').read_bytes()
        self.symbols=load_symbols(directory/'expedition-debug.elf')
        self.maps={m['name']:m for m in catalog['maps']}
        self.manifest=arrange(catalog['maps']);self.cache={};self.tilesets={};self.pairs={};self.lock=threading.Lock()
    def read(self,address,count):
        offset=address-0x08000000
        if offset<0 or offset+count>len(self.rom):raise ValueError('Map asset outside ROM')
        return self.rom[offset:offset+count]
    def words(self,address,count):return struct.unpack('<'+'I'*count,self.read(address,count*4))
    def tileset(self,address,secondary):
        if address in self.tilesets:return self.tilesets[address]
        tiles,palettes,metatiles,callback,attributes=self.words(address+4,5)
        count=384 if secondary else 640
        if self.read(address,1)[0]: raw=lz77(self.rom[tiles-0x08000000:])
        else:raw=self.read(tiles,count*32)
        raw=raw[:count*32].ljust(count*32,b'\0')
        pixels=bytes(v for b in raw for v in (b&15,b>>4))
        result=(pixels,self.read(palettes,16*16*2),self.read(metatiles,count*16),self.read(attributes,count*4))
        self.tilesets[address]=result;return result
    def pair(self,primary,secondary):
        key=(primary,secondary)
        if key in self.pairs:return self.pairs[key]
        a,b=self.tileset(primary,False),self.tileset(secondary,True)
        pixels=a[0]+b[0];palette=a[1][:7*32]+b[1][7*32:13*32]+bytes(3*32)
        colors=[bytes(((c&31)*255//31,((c>>5)&31)*255//31,((c>>10)&31)*255//31)) for c in struct.unpack('<256H',palette)]
        colors[0]=bytes(3)
        entries=struct.unpack('<8192H',a[2]+b[2]);attrs=struct.unpack('<1024I',a[3]+b[3]);result=[]
        def stamp(image,entry,q):
            tile=entry&1023;pal=(entry>>12)*16;flipx=bool(entry&1024);flipy=bool(entry&2048)
            for y in range(8):
                for x in range(8):
                    index=pixels[tile*64+(7-y if flipy else y)*8+(7-x if flipx else x)]
                    if index:
                        offset=(((q//2)*8+y)*16+(q%2)*8+x)*3;image[offset:offset+3]=colors[pal+index]
        for i in range(1024):
            image=bytearray(16*16*3)
            # Normal-layer maps place tile 0x3014 beneath both metatile layers.
            if ((attrs[i]>>29)&3)==0:
                for q in range(4):stamp(image,0x3014,q)
            for layer in range(2):
                for q in range(4):stamp(image,entries[i*8+layer*4+q],q)
            result.append(bytes(image))
        self.pairs[key]=result;return result
    def render(self,name):
        if name not in self.maps:raise KeyError('Unknown map')
        with self.lock:
            if name in self.cache:return self.cache[name]
            m=self.maps[name];layout=self.symbols[m['layoutSymbol']]
            width,height,border,blocks,primary,secondary=self.words(layout,6)
            if not 0<width<=256 or not 0<height<=256:raise ValueError('Unsupported map dimensions')
            tiles=self.pair(primary,secondary);grid=struct.unpack('<'+'H'*(width*height),self.read(blocks,width*height*2))
            image=bytearray(width*height*16*16*3);stride=width*16*3
            for y in range(height):
                for x in range(width):
                    tile=tiles[grid[y*width+x]&1023]
                    for row in range(16):
                        dest=(y*16+row)*stride+x*16*3;image[dest:dest+48]=tile[row*48:(row+1)*48]
            result=png(width*16,height*16,image);self.cache[name]=result;return result
