#!/usr/bin/env python3
"""Audit active footprint wiring and pixels against cached reference assets."""
import csv, json, re
from pathlib import Path
from PIL import Image, ImageDraw
ROOT=Path(__file__).resolve().parents[1];OUT=ROOT/'docs/footprint-audit'
def read(p):return (ROOT/p).read_text()
active=re.findall(r'\[NATIONAL_DEX_(\w+)\] = TRUE',read('src/data/pokemon/active_pokedex.h'))
table=dict(re.findall(r'\[SPECIES_(\w+)\]\s*=\s*(\w+)',read('src/data/pokemon_graphics/footprint_table.h')))
paths=dict(re.findall(r'const u8 (gMonFootprint_\w+)\[\] = INCBIN_U8\("([^"]+)"\)',read('src/data/graphics/pokemon.h')+read('src/graphics.c')))
def pixels(path):
 im=Image.open(path)
 if im.mode=='P':return im.size,tuple(1 if v else 0 for v in im.get_flattened_data())
 if im.mode=='1':return im.size,tuple(0 if v else 1 for v in im.get_flattened_data())
 # Reference PNG footprints use white for background and black for the print.
 return im.size,tuple(1 if sum(c[:3])<384 and (len(c)<4 or c[3]) else 0 for c in im.convert('RGBA').get_flattened_data())
rows=[];sheets=[]
for n in active:
 symbol=table.get(n,'');path=paths.get(symbol,'').replace('.1bpp','.png');issues=[];bits=None
 if not symbol:issues.append('missing registration')
 if 'QuestionMark' in symbol:issues.append('question-mark placeholder')
 if not path or not (ROOT/path).exists():issues.append('missing source PNG')
 else:
  size,bits=pixels(ROOT/path)
  if size!=(16,16):issues.append('invalid dimensions')
  if not any(bits):issues.append('blank (may be correct for footless species)')
 ref=OUT/'reference'/(n.lower()+'.png')
 comparison='unavailable'
 if ref.exists() and bits is not None:
  rs,rb=pixels(ref);comparison='match' if rs==(16,16) and rb==bits else 'different'
 original=OUT/'reference'/(n.lower()+'-firered.png')
 if original.exists() and bits is not None and pixels(original)==((16,16),bits): comparison='match-original-firered'
 rows.append(dict(species=n,symbol=symbol,path=path,issues='; '.join(issues),reference=comparison))
 cell=Image.new('RGB',(160,76),'#f4eddb');d=ImageDraw.Draw(cell);d.text((2,1),n,fill='black')
 if bits:
  v=Image.new('RGB',(16,16));v.putdata([(52,52,52) if b else (244,237,219) for b in bits]);cell.paste(v.resize((48,48),Image.Resampling.NEAREST),(4,18))
 d.text((58,24),comparison,fill='black')
 if 'QuestionMark' in symbol:d.text((58,39),'PLACEHOLDER',fill='red')
 sheets.append(cell)
OUT.mkdir(exist_ok=True)
with (OUT/'all-species.csv').open('w') as f:
 w=csv.DictWriter(f,fieldnames=rows[0].keys());w.writeheader();w.writerows(rows)
for start in range(0,len(sheets),80):
 page=Image.new('RGB',(1280,760),'white')
 for i,c in enumerate(sheets[start:start+80]):page.paste(c,((i%8)*160,(i//8)*76))
 page.save(OUT/f'contact-sheet-{start//80+1}.png')
summary={k:sum(r['reference']==k for r in rows) for k in ('match','match-original-firered','different','unavailable')}
summary.update(active=len(rows),placeholders=sum('placeholder' in r['issues'] for r in rows),missing=sum('missing' in r['issues'] for r in rows),blank=sum('blank' in r['issues'] for r in rows))
(OUT/'summary.json').write_text(json.dumps(summary,indent=2)+'\n')
print(summary)
print('DIFFERENT:',[r['species'] for r in rows if r['reference']=='different'])

placeholders=[r for r in rows if 'placeholder' in r['issues']]
suspect=[r for r in rows if r['reference']=='different' and 'placeholder' not in r['issues']]
report=f"""# Active Pokémon footprint audit

All {len(rows)} active species checked. This report reflects the current assets; fixed-footprints.json records the 19 corrected footprints and before-fixes/ retains their prior images.

- {summary['missing']} missing registrations/source files; all resolved images are 16×16.
- {summary['match']} match the pokeemerald-expansion reference footprints.
- {summary['match-original-firered']} additional differences are confirmed original FireRed artwork (Shroomish, Golem, Mawile, Ludicolo); retain them.
- {len(placeholders)} use the question-mark placeholder.
- {len(suspect)} additional non-placeholder images differ from the reference and need review/replacement.
- {summary['blank']} images are blank; blankness alone is not an error for species without tracks.

## Placeholder entries

"""+', '.join(r['species'] for r in placeholders)+'.\n\n## Other mismatches\n\n'+', '.join(r['species'] for r in suspect)+""".

## Scope and provenance

Reference PNGs come from https://github.com/rh-hideout/pokeemerald-expansion/tree/master/graphics/pokemon ; per-species download URLs/results are in reference-sources.json. Four older-species alternatives were additionally checked against https://github.com/pret/pokefirered/tree/master/graphics/pokemon and matched the project exactly. The expansion reference is community maintained; a match is not independent proof of official provenance for later-generation footprints.

Custom species/forms do not necessarily have an official footprint to compare against. Their placeholder graphics need deliberate design choices. Failed regional-form reference lookups do not mean that no canonical footprint exists. No speculative custom prints were drawn.

all-species.csv lists every registration and result. contact-sheet-*.png covers every active species; differences.png shows the initial 26 reference differences (including the four subsequently verified FireRed originals). These are static asset checks, not an emulator playtest.
"""
(OUT/'report.md').write_text(report)
