"""Source overworld sheets and their declared walk sequences (no emulator writes)."""
import re
import struct
import zlib
from pathlib import Path


GROUPS = ('Gym Leaders & Elite Four', 'Civilians', 'Trainer Classes', 'Pokémon', 'Apex', 'Players & Named Trainers', 'Objects', 'Team Rocket')
LEADERS = {'Brock', 'Misty', 'MistyWater', 'LtSurge', 'Erika', 'Koga', 'Janine', 'Sabrina', 'Blaine', 'Giovanni', 'Lorelei', 'Bruno', 'Agatha', 'Lance'}
APEX = {'Tangrowth', 'Annihilape', 'Osscythe', 'MimeSr', 'Articuno', 'Zapdos', 'Moltres', 'Mewtwo'}

def group_for(label, frames):
    path = frames[0]['path']
    if label.startswith('RocketAce') or label in {'Giovanni', 'RocketArcher', 'RocketAriana', 'RocketPetrel', 'RocketProton', 'RocketM', 'RocketF'}: return GROUPS[7]
    if label in LEADERS: return GROUPS[0]
    if label in APEX: return GROUPS[4]
    if '/pokemon/' in path: return GROUPS[3]
    if '/civilians/' in path or label == 'RocketNurse': return GROUPS[1]
    if '/misc/' in path: return GROUPS[6]
    if label.startswith(('Red', 'Green', 'RocketArcher', 'RocketAriana', 'RocketPetrel', 'RocketProton')) or label == 'Blue': return GROUPS[5]
    return GROUPS[2]


def catalog(root):
    folder = root / 'src/data/object_events'
    graphics = (folder / 'object_event_graphics.h').read_text()
    pics = dict(re.findall(r'const u\d+\s+(\w+)\[\]\s*=\s*INCBIN_U\d+\("([^"]+\.4bpp)"\)', graphics))
    tables = {}
    for name, body in re.findall(r'static const struct SpriteFrameImage (\w+)\[\] = \{(.*?)\n\};', (folder / 'object_event_pic_tables.h').read_text(), re.S):
        tables[name] = [dict(path=str(Path(pics[p]).with_suffix('.png')), width=int(w)*8, height=int(h)*8, index=int(i)) for p,w,h,i in re.findall(r'overworld_frame\((\w+),\s*(\d+),\s*(\d+),\s*(\d+)\)',body) if p in pics]
    text = (folder / 'object_event_anims.h').read_text()
    anims = {}
    for name, body in re.findall(r'static const union AnimCmd (\w+)\[\] = \{(.*?)\n\};', text, re.S):
        # Unsupported control flow is reported instead of inventing a walking cycle.
        if re.search(r'ANIMCMD_JUMP\((?!0\))',body): continue
        frames = [dict(frame=int(i), duration=int(d), flip='hFlip = TRUE' in extra, vflip='vFlip = TRUE' in extra) for i,d,extra in re.findall(r'ANIMCMD_FRAME\((\d+),\s*(\d+)([^)]*)\)',body)]
        if frames: anims[name] = frames
    sequences = {}
    rechallenges = {}
    for name, body in re.findall(r'static const union AnimCmd \*const (\w+)\[\] = \{(.*?)\n\};',text,re.S):
        sequences[name] = {direction.lower(): anims.get(anim,[]) for direction,anim in re.findall(r'\[ANIM_STD_GO_(SOUTH|NORTH|WEST|EAST)\]\s*=\s*(\w+)',body)}
        match = re.search(r'\[ANIM_RAISE_HAND\]\s*=\s*(\w+)',body)
        rechallenges[name] = anims.get(match.group(1),[]) if match else []
    result=[]
    for name,body in re.findall(r'const struct ObjectEventGraphicsInfo (\w+) = \{(.*?)\n\};',(folder/'object_event_graphics_info.h').read_text(),re.S):
        field=lambda key: (re.search(r'\.'+key+r'\s*=\s*(\w+)',body).group(1) if re.search(r'\.'+key+r'\s*=\s*(\w+)',body) else '')
        frames=tables.get(field('images'),[])
        if not frames: continue
        walks=sequences.get(field('anims'),{})
        issues=[]
        if len(walks)!=4: issues.append('No supported four-direction walk table')
        for direction,seq in walks.items():
            if not seq or any(f['frame']>=len(frames) for f in seq): issues.append(direction+': missing frame definition')
        for f in frames:
            path=root/f['path']
            if not path.is_file(): issues.append('Missing source sheet');continue
            data=path.read_bytes();w,h=struct.unpack('>II',data[16:24])
            if f['index'] >= (w//f['width'])*(h//f['height']): issues.append('Frame points outside sheet')
        label=name.removeprefix('gObjectEventGraphicsInfo_')
        result.append(dict(group=group_for(label,frames),id=name,name=re.sub(r'(?<=[a-z])(?=[A-Z])',' ',label),frames=frames,walks=walks,rechallenge=rechallenges.get(field('anims'),[]),issues=sorted(set(issues)),table=field('anims')))
    return sorted(result,key=lambda x:x['name'].lower())


def transparent_png(path):
    """Set palette index zero transparent without removing same-RGB outline pixels."""
    data=path.read_bytes()
    if data[25]!=3: return data
    result=data[:8];offset=8
    while offset<len(data):
        size=struct.unpack('>I',data[offset:offset+4])[0];kind=data[offset+4:offset+8];chunk=data[offset:offset+12+size]
        if kind!=b'tRNS': result+=chunk
        if kind==b'PLTE':
            body=b'tRNS'+b'\0';result+=struct.pack('>I',1)+body+struct.pack('>I',zlib.crc32(body)&0xffffffff)
        offset+=12+size
    return result
