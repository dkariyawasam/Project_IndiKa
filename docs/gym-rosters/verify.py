"""Validate new gym roster wiring, placement and text using the current build."""
from pathlib import Path
import json, re, struct, subprocess
root=Path(__file__).resolve().parents[2]
rows=json.loads((root/'docs/gym-rosters/roster.json').read_text())
syms={l.split()[2]:int(l.split()[0],16) for l in subprocess.check_output(['/opt/devkitpro/devkitARM/bin/arm-none-eabi-nm',str(root/'pokefirered.elf')],text=True).splitlines() if len(l.split())==3}
rom=(root/'pokefirered.gba').read_bytes();start=syms['sFontNormalLatinGlyphWidths']-0x8000000;widths=rom[start:start+512]
chars={}
for line in (root/'charmap.txt').read_text().splitlines():
 m=re.match(r"'(.+)'\s*=\s*([0-9A-F]{2})$",line)
 if m:chars.setdefault(m[1].replace("\\'", "'"),int(m[2],16))
layouts=json.loads((root/'data/layouts/layouts.json').read_text())['layouts']
trainers=(root/'src/data/trainers.h').read_text();rematches=(root/'src/vs_seeker.c').read_text()
assert len({r['id'] for r in rows})==12
text_count=0
for r in rows:
 m=r['map'];j=json.loads((root/'data/maps'/m/'map.json').read_text());o=next(o for o in j['object_events'] if o['script']==r['label'])
 assert sum(o['x']==a['x'] and o['y']==a['y'] for a in j['object_events'])==1
 assert all((o['x'],o['y'])!=(a['x'],a['y']) for a in j['warp_events'])
 lay=next(l for l in layouts if l['name']==m+'_Layout');data=(root/lay['blockdata_filepath']).read_bytes();tile=struct.unpack_from('<H',data,2*(o['y']*lay['width']+o['x']))[0];assert tile>>10&3==0
 assert o['trainer_type']=='TRAINER_TYPE_NORMAL'
 assert f"[{r['constant']}]" in trainers
 assert '{'+r['constant']+', '+r['constant']+'}' in rematches
 script=(root/'data/maps'/m/'scripts.inc').read_text();assert 'trainerbattle_rematch '+r['constant']+',' in script
 text=(root/'data/maps'/m/'text.inc').read_text()
 for suffix in ['Intro','Defeat','After']:
  label=m+'_Text_'+r['name']+suffix
  block=re.search(re.escape(label)+r'::\n((?:\s*\.string[^\n]*\n)+)',text)[1]
  for line in re.findall(r'\.string "(.*)"',block):
   line=re.sub(r'\\[npl]|\$','',line)
   assert all(c in chars for c in line),(label,line)
   assert sum(widths[chars[c]] for c in line)<=208,(label,line)
  text_count+=1
for k in ['NUM_TRAINERS','MAX_TRAINERS_COUNT']:
 assert re.search(r'#define '+k+r'\s+785\b',(root/'include/constants/opponents.h').read_text())
print(f'PASS: {len(rows)} trainer/party/rematch entries, collision/warp placement checks, {text_count} dialogue boxes, unchanged trainer flag allocation.')
