"""Build-aware catalogs. Never execute source text as Python."""
import ast
import json
import re
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
TOOLCHAIN = Path('/opt/devkitpro/devkitARM/bin')


def constants():
    expressions = {}
    for path in (ROOT / 'include/constants').glob('*.h'):
        for name, value in re.findall(r'^#define[ \t]+(\w+)[ \t]+([^\n]+)', path.read_text(), re.M):
            expressions[name] = value.split('//')[0].split('/*')[0].strip()
    values = {}
    def resolve(name, seen=()):
        if name in values:
            return values[name]
        if name in seen or name not in expressions:
            raise ValueError(name)
        def visit(node):
            if isinstance(node, ast.Constant) and type(node.value) is int:
                return node.value
            if isinstance(node, ast.Name):
                return resolve(node.id, seen + (name,))
            if isinstance(node, ast.BinOp):
                a, b = visit(node.left), visit(node.right)
                if isinstance(node.op, ast.Add): return a + b
                if isinstance(node.op, ast.Sub): return a - b
                if isinstance(node.op, ast.BitOr): return a | b
                if isinstance(node.op, ast.LShift) and 0 <= b < 32: return a << b
            raise ValueError(name)
        values[name] = visit(ast.parse(expressions[name], mode='eval').body)
        return values[name]
    for name in expressions:
        try: resolve(name)
        except (ValueError, SyntaxError, TypeError): pass
    return values


def load_symbols(elf):
    nm = TOOLCHAIN / 'arm-none-eabi-nm'
    result = subprocess.run([str(nm), str(elf)], check=True, capture_output=True, text=True)
    return {p[2]: int(p[0], 16) for line in result.stdout.splitlines()
            if len(p := line.split()) == 3 and re.fullmatch('[0-9a-fA-F]+', p[0])}


def build_catalog():
    c = constants()
    groups = json.loads((ROOT / 'data/maps/map_groups.json').read_text())
    layouts = {x['id']: x for x in json.loads((ROOT / 'data/layouts/layouts.json').read_text())['layouts']}
    maps = []
    for group, key in enumerate(groups['group_order']):
        for number, name in enumerate(groups[key]):
            path = ROOT / 'data/maps' / name / 'map.json'
            if not path.exists(): continue
            data = json.loads(path.read_text())
            layout = layouts[data['layout']]
            warps = data.get('warp_events', [])
            maps.append(dict(name=name, layoutSymbol=layout['name'], id=data['id'], group=group, number=number,
                             width=layout['width'], height=layout['height'],
                             x=warps[0]['x'] if warps else layout['width']//2,
                             y=warps[0]['y'] if warps else layout['height']//2,
                             type=data['map_type'], running=data['allow_running'],
                             objects=data.get('object_events', []), warps=warps,
                             connections=data.get('connections') or []))
    sys.path.insert(0, str(ROOT / 'tools'))
    from sync_active_pokedex import derive
    active, _, _, names, _ = derive()
    species = [{'id': c['SPECIES_'+n], 'name': names[n], 'key': n} for n in active if 'SPECIES_'+n in c]
    trainers = []
    source = (ROOT / 'src/data/trainers.h').read_text()
    for match in re.finditer(r'\[(TRAINER_\w+)\]\s*=\s*\{(.*?)\n    \}', source, re.S):
        key, body = match.groups()
        if key not in c: continue
        name = re.search(r'\.trainerName\s*=\s*_\("([^"]+)"\)', body)
        trainers.append(dict(id=c[key], key=key, name=name[1] if name else key,
                             double=bool(re.search(r'\.doubleBattle\s*=\s*TRUE', body))))
    def header_keys(header):
        return set(re.findall(r'^#define[ \t]+(\w+)[ \t]+', (ROOT/'include/constants'/header).read_text(), re.M))
    flag_keys, var_keys = header_keys('flags.h'), header_keys('vars.h')
    flags = [{'key': k, 'id': v} for k, v in c.items() if k in flag_keys and k.startswith('FLAG_') and 0 < v < c['FLAGS_COUNT'] and not k.endswith('_START')]
    variables = [{'key': k, 'id': v} for k, v in c.items() if k in var_keys and k.startswith('VAR_') and c['VARS_START'] <= v <= c['VARS_END']]
    def named(prefix, maximum, header):
        seen = set(); result = []
        keys = re.findall(r'^#define[ \t]+(' + prefix + r'\w+)[ \t]+', (ROOT/'include/constants'/header).read_text(), re.M)
        for key in keys:
            value = c.get(key, -1)
            if key.startswith(prefix) and 0 < value < maximum and value not in seen:
                result.append(dict(id=value, key=key, name=key[len(prefix):].replace('_', ' '))); seen.add(value)
        return result
    chars = {int(code,16): char for char,code in reversed(re.findall(r"^'(.)'\s*=\s*([0-9A-F]{2})\s*$", (ROOT/'charmap.txt').read_text(), re.M))}
    return dict(maps=maps, network=map_network(maps), species=species, trainers=trainers, flags=flags, variables=variables,
                items=named('ITEM_',c['ITEMS_COUNT'],'items.h'), moves=named('MOVE_',c['MOVES_COUNT'],'moves.h'), chars=chars)


ORDERS = ['GAEM','GAME','GEAM','GEMA','GMAE','GMEA','AGEM','AGME','AEGM','AEMG','AMGE','AMEG',
          'EGAM','EGMA','EAGM','EAMG','EMGA','EMAG','MGAE','MGEA','MAGE','MAEG','MEGA','MEAG']

def decode_party(hex_data, catalog):
    raw = bytes.fromhex(hex_data)
    if len(raw) != 100: raise ValueError('Invalid party record')
    personality, ot = struct.unpack_from('<II', raw)
    key = personality ^ ot
    secure = b''.join(struct.pack('<I', n ^ key) for n in struct.unpack_from('<12I', raw,32))
    valid = sum(struct.unpack('<24H', secure)) & 65535 == struct.unpack_from('<H', raw,28)[0]
    parts = {name: secure[i*12:i*12+12] for i, name in enumerate(ORDERS[personality%24])}
    species, item = struct.unpack_from('<HH',parts['G'])
    chars = catalog['chars']; name = ''.join(chars.get(x,'?') for x in raw[8:18].split(b'\xff')[0])
    return dict(name=name, species=species, item=item, level=raw[84], hp=struct.unpack_from('<H',raw,86)[0],
                maxHP=struct.unpack_from('<H',raw,88)[0], friendship=parts['G'][9],
                moves=list(struct.unpack_from('<4H',parts['A'])), pp=list(parts['A'][8:12]),
                status=struct.unpack_from('<I',raw,80)[0], checksumValid=valid)


def map_network(maps):
    """Directed, source-defined links; scripted travel is not inferred."""
    names = {m['id']: m['name'] for m in maps}
    links, unresolved = [], []
    for m in maps:
        for kind, entries in [('connection', m['connections']), ('warp', m['warps'])]:
            for index, entry in enumerate(entries):
                destination = entry.get('map' if kind == 'connection' else 'dest_map')
                link = dict(source=m['name'], target=names.get(destination), kind=kind,
                            destination=destination, index=index)
                if kind == 'connection':
                    link.update(direction=entry.get('direction'), offset=entry.get('offset'))
                else:
                    link.update(x=entry['x'], y=entry['y'], destinationWarp=entry.get('dest_warp_id'))
                (links if link['target'] else unresolved).append(link)
    return dict(links=links, unresolved=unresolved)
