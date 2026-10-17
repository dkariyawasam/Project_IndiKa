#!/usr/bin/env python3
"""Calibrate active dex silhouettes to listed heights and a 1.5 m trainer.

Requires Pillow. Run after front sprite/height changes; --check verifies calibration.
The full visible silhouette represents the listed height (poses/lengths approximate).
"""
from pathlib import Path
import argparse
import re
from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--check', action='store_true')
args = parser.parse_args()
def read(path):
    return (ROOT / path).read_text()
active = set(re.findall(r'\[NATIONAL_DEX_(\w+)\] = TRUE', read('src/data/pokemon/active_pokedex.h')))
pics = dict(re.findall(r'SPECIES_SPRITE\((\w+),\s*(\w+)\)', read('src/data/pokemon_graphics/front_pic_table.h')))
paths = dict(re.findall(r'const u32 (\w+)\[\] = INCBIN_U32\("([^"]+)"\)', read('src/data/graphics/pokemon.h')))
def bbox(path):
    im = Image.open(ROOT / path).crop((0, 0, 64, 64))
    assert im.mode == 'P', path
    return im.getbbox()
trainer = bbox('graphics/trainers/front_pics/red_front_pic.png')
leaf = bbox('graphics/trainers/front_pics/leaf_front_pic.png')
assert (trainer[1], trainer[3], trainer[2]-trainer[0]) == (leaf[1], leaf[3], leaf[2]-leaf[0])
seen = set()
changed = []
def update(match):
    name, body = match.groups()
    if name not in active:
        return match.group(0)
    path = paths[pics[name]].replace('.4bpp.lz', '.png')
    if name == 'CASTFORM':
        path = 'graphics/pokemon/castform/normal/front.png'
    a,b,c,d = bbox(path)
    height = int(re.search(r'\.height = (\d+)', body)[1]) / 10
    assert height > 0
    # Both subjects share pixels/metre; constrain each to its own panel space.
    mh, mw = d-b, c-a
    th = trainer[3]-trainer[1]
    ppm = min(mh/height, th/1.5, 46/height, 46/1.5, 46*mh/(mw*height), 30*th/(40*1.5))
    ps = max(256, round(256*mh/(height*ppm)))
    ts = max(256, round(256*th/(1.5*ppm)))
    # Ground both at y127; centers are y104. Affine scales are inverse.
    po = round(23-(d-32)*256/ps)
    to = round(23-(trainer[3]-32)*256/ts)
    assert abs((mh/ps)/(th/ts)-height/1.5) <= max(.015, height/1.5*.01), name
    for box, scale, offset, x in (( (a,b,c,d), ps, po, 40), (trainer, ts, to, 80), (leaf, ts, to, 80)):
        left, top, right, bottom = box
        assert x+(left-32)*256/scale >= 16, name
        assert x+(right-32)*256/scale <= 96, name
        assert 104+offset+(top-32)*256/scale >= 80, name
        assert 104+offset+(bottom-32)*256/scale <= 128, name
    for key, value in zip(('pokemonScale','pokemonOffset','trainerScale','trainerOffset'), (ps,po,ts,to)):
        body = re.sub(r'(\.'+key+r' = )-?\d+', lambda m: m[1]+str(value), body)
    result = '[NATIONAL_DEX_'+name+'] =\n    {'+body+'\n    }'
    if result != match.group(0):
        changed.append(name)
    seen.add(name)
    return result
p = ROOT/'src/data/pokemon/pokedex_entries.h'
source = p.read_text()
result = re.sub(r'\[NATIONAL_DEX_(\w+)\]\s*=\s*\{(.*?)\n    \}', update, source, flags=re.S)
assert seen == active, active-seen
if args.check:
    assert not changed, 'Stale size comparison: '+', '.join(changed)
else:
    p.write_text(result)
print(f'{len(seen)} species checked; {len(changed)} calibrations '+('need updating' if args.check else 'updated'))
