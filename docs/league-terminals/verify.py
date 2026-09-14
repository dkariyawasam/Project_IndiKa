from pathlib import Path
import json,re,struct,subprocess
root=Path(__file__).resolve().parents[2]
syms={v.split()[2]:int(v.split()[0],16) for v in subprocess.check_output(['/opt/devkitpro/devkitARM/bin/arm-none-eabi-nm',str(root/'pokefirered.elf')],text=True).splitlines() if len(v.split())==3}
rom=(root/'pokefirered.gba').read_bytes();a=syms['sFontNormalLatinGlyphWidths']-0x8000000;w=rom[a:a+512];chars={}
for l in (root/'charmap.txt').read_text().splitlines():
 m=re.match(r"'(.+)'\s*=\s*([0-9A-F]{2})$",l)
 if m:chars.setdefault(m[1].replace("\\'","'"),int(m[2],16))
s=(root/'data/scripts/pokemon_league.inc').read_text().split('LeagueTerminal_Text_Indigo::')[1]
count=0
for chunk in re.findall(r'\.string "(.*)"',s):
 for line in re.split(r'\\[npl]|\$',chunk):
  assert sum(w[chars[c]] for c in line)<=208,line
 count+=1
for m,x,y,script in [('IndigoPlateau_PokemonCenter_1F',2,2,'LeagueTerminal_EventScript_Indigo'),('RocketLeague_Lobby',8,7,'LeagueTerminal_EventScript_Rocket')]:
 data=json.loads((root/f'data/maps/{m}/map.json').read_text());assert any(o['x']==x and o['y']==y and o['script']==script for o in data['bg_events'])
 assert not any(o['x']==x and o['y']==y for o in data['object_events'])
for m in ['RocketLeague_Arena','RocketLeague_ChampionsRoom']:
 s=(root/f'data/maps/{m}/scripts.inc').read_text()
 for amount in re.findall(r'\taddcoins (\d+)\n',s):assert f'addcoins {amount}\n\tsetvar VAR_0x8004, {amount}\n\tspecial RecordRocketLeagueCoins' in s
print(f'PASS: {count} terminal text strings fit; both interaction events; received-coin accounting hooks.')
