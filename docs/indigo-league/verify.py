"""Check arena connections, all pool entries, walkability, and built text widths."""
from pathlib import Path
import json,re,struct,subprocess
r=Path(__file__).resolve().parents[2]
load=lambda m:json.loads((r/f'data/maps/{m}/map.json').read_text())
arena=load('PokemonLeague_BrunosRoom');lobby=load('IndigoPlateau_PokemonCenter_1F')
assert lobby['warp_events'][1]['dest_map']==arena['id']
assert arena['warp_events'][1]['dest_map']=='MAP_POKEMON_LEAGUE_CHAMPIONS_ROOM'
s=(r/'data/maps/PokemonLeague_BrunosRoom/scripts.inc').read_text()
pool=re.search(r'sIndigoLeagueTrainerPool\[\] = \{(.*?)\};',(r/'src/league_challenge.c').read_text(),re.S)[1]
ids=re.findall(r'TRAINER_\w+',pool)
for t in ids:
 assert f'trainerbattle_no_intro_double {t},' in s
 assert f'goto_if_eq VAR_RESULT, {t},' in s
assert len(ids)==len(set(ids))==12
assert len(re.findall(r'setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_',s))==12
blocks=struct.unpack('<169H',(r/'data/layouts/PokemonLeague_BrunosRoom/map.bin').read_bytes())
for x,y in [(6,3),(6,4),(6,5),(6,10),(6,11),(6,12)]+[(x,8) for x in range(4,9)]:assert blocks[y*13+x]>>10&3==0
syms={v.split()[2]:int(v.split()[0],16) for v in subprocess.check_output(['/opt/devkitpro/devkitARM/bin/arm-none-eabi-nm',str(r/'pokefirered.elf')],text=True).splitlines() if len(v.split())==3}
rom=(r/'pokefirered.gba').read_bytes()
layout=syms['PokemonLeague_BrunosRoom_Layout']-0x8000000
map_start=struct.unpack_from('<I',rom,layout+12)[0]-0x8000000
assert rom[map_start:map_start+338]==(r/'data/layouts/PokemonLeague_BrunosRoom/map.bin').read_bytes(), 'ROM contains stale arena tiles'
a=syms['sFontNormalLatinGlyphWidths']-0x8000000;w=rom[a:a+512];chars={}
for l in (r/'charmap.txt').read_text().splitlines():
 m=re.match(r"'(.+)'\s*=\s*([0-9A-F]{2})$",l)
 if m:chars.setdefault(m[1].replace("\\'","'"),int(m[2],16))
for mapname in ['PokemonLeague_BrunosRoom','IndigoPlateau_PokemonCenter_1F']:
 for label,body in re.findall(r'(\w+)::\n((?:\s*\.string[^\n]*\n)+)',(r/f'data/maps/{mapname}/text.inc').read_text()):
  if mapname.startswith('Indigo') and not label.endswith(('GymGuyAdvice','FaceEliteFourGoodLuck')):continue
  for l in re.findall(r'\.string "(.*)"',body):
   l=re.sub(r'\\[npl]|\$','',l).replace('{STR_VAR_1}','4');assert sum(w[chars[c]] for c in l)<=208,(label,l)
print('PASS: lobby/arena/Champion connections, 12 pool battle and sprite branches, opponent/trigger floor collision, dialogue widths.')
