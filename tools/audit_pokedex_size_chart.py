#!/usr/bin/env python3
"""Audit current size-chart assets/settings; no ROM edits. Requires Pillow."""
import csv, re
from pathlib import Path
from PIL import Image, ImageDraw
ROOT = Path(__file__).resolve().parents[1]
def read(p): return (ROOT / p).read_text()
entries = {}
for name, body in re.findall(r'\[NATIONAL_DEX_(\w+)\]\s*=\s*\{(.*?)\n    \}', read('src/data/pokemon/pokedex_entries.h'), re.S):
    entries[name] = {k:int(v) for k,v in re.findall(r'\.(height|pokemonScale|pokemonOffset|trainerScale|trainerOffset)\s*=\s*(-?\d+)',body)}
retired = set(re.findall(r'\[SPECIES_(\w+)\]\s*=\s*\{0\}',read('src/data/pokemon/species_info.h')))
mapbody = read('src/pokemon.c').split('static const u16 sSpeciesToNationalPokedexNum')[1].split('};')[0]
roster = list(dict.fromkeys(re.findall(r'SPECIES_TO_NATIONAL\((\w+)\)',mapbody)))
active_names = re.findall(r'\[NATIONAL_DEX_(\w+)\] = TRUE', read('src/data/pokemon/active_pokedex.h'))
active_set = set(active_names)
pics = dict(re.findall(r'SPECIES_SPRITE\((\w+),\s*(\w+)\)',read('src/data/pokemon_graphics/front_pic_table.h')))
paths = dict(re.findall(r'const u32 (\w+)\[\] = INCBIN_U32\("([^"]+)"\)',read('src/data/graphics/pokemon.h')))
def mask(path):
    im = Image.open(path).crop((0,0,64,64))
    assert im.mode == 'P', path
    result = Image.new('L', im.size)
    result.putdata([255 if p else 0 for p in im.get_flattened_data()])
    return result
trainers = [mask(ROOT / f'graphics/trainers/front_pics/{gender}_front_pic.png') for gender in ('red','leaf')]
def bounds(im, scale, offset, x):
    a,b,c,d = im.getbbox()
    return (x+(a-32)*256/scale,104+offset+(b-32)*256/scale,x+(c-32)*256/scale,104+offset+(d-32)*256/scale)
out = ROOT/'docs/pokedex-size-chart-audit'; out.mkdir(exist_ok=True)
rows=[]; images=[]; image_by_species={}
for name in roster:
    if name in retired or name.startswith('OLD_UNOWN_'): continue
    e=entries.get(name,{}); issues=[]
    if 'QuestionMark' in pics.get(name,''): issues.append('placeholder question-mark sprite')
    row={'species':name,'active_pokedex':name in active_set,**e}
    path=paths.get(pics.get(name,''),'').replace('.4bpp.lz','.png')
    if name == 'CASTFORM': path = 'graphics/pokemon/castform/normal/front.png'
    if not e: issues.append('missing dex entry')
    elif not e.get('height'): issues.append('zero height')
    if not path or not (ROOT/path).exists(): issues.append('unresolved front asset')
    if e and path and (ROOT/path).exists():
        mon=mask(ROOT/path)
        if not mon.getbbox(): issues.append('blank front')
        elif e.get('pokemonScale',0)<=0 or e.get('trainerScale',0)<=0: issues.append('invalid scale')
        else:
            ps,po,ts,to=[e[k] for k in ('pokemonScale','pokemonOffset','trainerScale','trainerOffset')]
            if (ps,po,ts,to)==(256,0,256,-2): issues.append('default calibration: review')
            mb=bounds(mon,ps,po,40)
            for gender,t in zip(('red','leaf'),trainers):
                tb=bounds(t,ts,to,80)
                row[gender+'_height_ratio']=round((mb[3]-mb[1])/(tb[3]-tb[1]),3)
                row[gender+'_baseline_gap_px']=round(mb[3]-tb[3],1)
                if any((b[0]<16 or b[2]>96 or b[1]<80 or b[3]>128) for b in (mb,tb)):
                    issues.append(gender+' silhouette crosses size panel interior')
            canvas=Image.new('RGB',(160,112),'#e8e0c8');draw=ImageDraw.Draw(canvas)
            draw.text((2,0),name,fill='black');draw.text((2,12),f'{e["height"]/10:g}m  P:{ps} T:{ts}',fill='black')
            for m,s,o,x,color in ((mon,ps,po,40,'#454545'),(trainers[0],ts,to,80,'#454545'),(trainers[1],ts,to,124,'#777777')):
                # Sample inverse affine transformation as GBA does, clipped to normal 64x64 OBJ.
                transformed=m.transform((64,64),Image.Transform.AFFINE,(s/256,0,32-32*s/256,0,s/256,32-32*s/256),resample=Image.Resampling.NEAREST)
                canvas.paste(color,(x-32,56+o-32),transformed)
            images.append(canvas); image_by_species[name]=canvas
    row['issues']='; '.join(issues);rows.append(row)
keys=list(dict.fromkeys(k for r in rows for k in r))
with (out/'all-species.csv').open('w') as f:
    w=csv.DictWriter(f,fieldnames=keys);w.writeheader();w.writerows(rows)
for start in range(0,len(images),60):
    page=Image.new('RGB',(960,1120),'white')
    for i,im in enumerate(images[start:start+60]):page.paste(im,((i%6)*160,(i//6)*112))
    page.save(out/f'contact-sheet-{start//60+1}.png')
by_name = {r['species']:r for r in rows}
missing_active = [n for n in active_names if n not in by_name]
assert not missing_active, f'Active species missing from audit: {missing_active}'
active_rows = [by_name[n] for n in active_names]
with (out/'active-pokedex.csv').open('w') as f:
    w=csv.DictWriter(f,fieldnames=keys);w.writeheader();w.writerows(active_rows)
for start in range(0,len(active_names),60):
    page=Image.new('RGB',(960,1120),'white')
    for i,n in enumerate(active_names[start:start+60]):
        if n in image_by_species: page.paste(image_by_species[n],((i%6)*160,(i//6)*112))
    page.save(out/f'active-contact-sheet-{start//60+1}.png')
active_defaults = [r for r in active_rows if 'default calibration' in r['issues']]
active_placeholders = [r for r in active_rows if 'placeholder' in r['issues']]
defaults=[r for r in rows if 'default calibration' in r['issues']]
structural=[r for r in rows if any(x in r['issues'] for x in ('missing','zero height','unresolved','blank','invalid'))]
overflow=[r for r in rows if 'crosses' in r['issues']]
placeholders=[r for r in rows if 'placeholder' in r['issues']]
real_defaults=[r for r in defaults if 'placeholder' not in r['issues']]
report=f'''# Pokédex size-chart audit

Audited {len(rows)} non-retired species in the species-to-National-Dex mapping; excluded legacy OLD_UNOWN placeholders; {len(retired)} explicitly zeroed species are absent from the mapped roster. No game data changed.

- {len(structural)} missing/blank/invalid entry or front-asset findings.
- {len(defaults)} species use the identical default calibration (256, 0, 256, -2); {len(real_defaults)} have actual species sprites.
- {len(placeholders)} mapped entries use question-mark placeholder sprites. Mapping coverage does not mean all these species are obtainable in-game.
- {len(overflow)} species have a calculated silhouette crossing the size panel interior (x16–96, y80–128), for at least one player. These are layout review flags, not necessarily visible corruption.

The chart uses hand-authored inverse affine scale/offset values, not the height field. Updating a height or front sprite does not recalibrate the chart. Both RED and LEAF were checked. CASTFORM uses its normal form. UNOWN uses A as the audit representative; the game can show the saved personality form, and SPINDA spots depend on the save. Contact sheets approximate current affine silhouettes with RED in dark gray and LEAF in gray at right; they are static reconstructions, not emulator captures.

## Active Pokédex coverage

All {len(active_rows)} active entries are covered, in the game's Pokédex order, from the explicit active roster (including implemented species beyond MEW). Zero missing active entries. See active-pokedex.csv and active-contact-sheet-*.png.

{len(active_defaults)} active entries have default calibration; {len(active_placeholders)} active entries use question-mark sprites. Default values are review candidates, not automatic failures: some original species legitimately use these values.

Active placeholder entries: {', '.join(r['species'] for r in active_placeholders)}.

## Default calibration requiring review

'''+', '.join(r['species'] for r in defaults)+'''.

## Placeholder sprites

'''+', '.join(r['species'] for r in placeholders)+'''.

## Interpretation

Pixel-height ratios are diagnostics, not proof of biological height: tails, wings, curled bodies, floating poses and species measured by length require individual judgment. Existing vanilla calibrations should not be blindly replaced with bounding-box normalization. This audit does not verify canonical height lore against external sources and is not a live emulator sign-off.

See all-species.csv for every species, both player ratios, baseline differences and layout flags. See contact-sheet-*.png for visual review.
'''
(out/'report.md').write_text(report)
print(report)
for n in ('CHINGLING','MAMOSWINE','SYLVEON','SLOWKING'):
    print(next((r for r in rows if r['species']==n),n+' excluded'))
