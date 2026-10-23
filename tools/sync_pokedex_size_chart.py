#!/usr/bin/env python3
"""Calibrate active dex silhouettes to listed heights and a 1.5 m trainer.

Requires Pillow. Run after front sprite/height changes; --check verifies calibration.
The full visible silhouette represents the listed height (poses/lengths approximate).
"""
from pathlib import Path
import math
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
    # Fit the pair together with a six-pixel gap, rather than fixed-width columns.
    # Double-size affine OBJs permit up to 2x enlargement without clipping.
    mh, mw = d-b, c-a
    tl, tr = min(trainer[0], leaf[0]), max(trainer[2], leaf[2])
    th, tw = trainer[3]-trainer[1], tr-tl
    ppm = min(86/max(height, 1.5), 98/(mw*height/mh + tw*1.5/th),
              2*mh/height, 2*th/1.5)
    ps = max(128, math.ceil(256*mh/(height*ppm)))
    ts = max(128, math.ceil(256*th/(1.5*ppm)))
    mon_width, trainer_width = mw*256/ps, tw*256/ts
    pair_left = 16 + (104-mon_width-trainer_width-6)/2
    px = round(pair_left-(a-32)*256/ps)
    tx = round(pair_left+mon_width+6-(tl-32)*256/ts)
    # Ground both at y136; sprite centers are y112.
    po = round(24-(d-32)*256/ps)
    to = round(24-(trainer[3]-32)*256/ts)
    assert abs((mh/ps)/(th/ts)-height/1.5) <= max(.015, height/1.5*.01), name
    gap = tx+(tl-32)*256/ts - (px+(c-32)*256/ps)
    assert 5 <= gap <= 7, (name, gap)
    for box, scale, offset, x in (((a,b,c,d), ps, po, px), (trainer, ts, to, tx), (leaf, ts, to, tx)):
        left, top, right, bottom = box
        assert x+(left-32)*256/scale >= 15.5, name
        assert x+(right-32)*256/scale <= 120.5, name
        assert 112+offset+(top-32)*256/scale >= 48, name
        assert 112+offset+(bottom-32)*256/scale <= 144, name
    for key, value in zip(('pokemonScale','pokemonOffset','trainerScale','trainerOffset','pokemonX','trainerX'), (ps,po,ts,to,px,tx)):
        if re.search(r'\.'+key+r' = ', body):
            body = re.sub(r'(\.'+key+r' = )-?\d+', lambda m: m[1]+str(value), body)
        else:
            body += '\n        .'+key+' = '+str(value)+','
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
