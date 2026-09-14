from pathlib import Path
import json,re
maps=['PewterCity_Gym','CeruleanCity_Gym','VermilionCity_Gym','CeladonCity_Gym','FuchsiaCity_Gym','SaffronCity_Gym','CinnabarIsland_Gym','ViridianCity_Gym','MtMoon_B1F','RockTunnel_B2F','DiglettsCave_Southside_B1F','ViridianForest','CinnabarVolcano_1F','CinnabarVolcano_LeftCorridor_2F','CinnabarVolcano_RightCorridor_3F','PokemonTower_3F']
syms=json.loads(Path('/tmp/gym-story-qa/symbols.json').read_text());rom=Path('pokefirered.gba').read_bytes();widths=rom[syms['sFontNormalLatinGlyphWidths']-0x8000000:][:512];chars={}
for l in Path('charmap.txt').read_text().splitlines():
 m=re.match(r"'(.+)'\s*=\s*([0-9A-F]{2})$",l)
 if m:chars.setdefault(m[1],int(m[2],16))
rows=[];issues=[]
for name in maps:
 for p in Path('data/maps',name).glob('*.inc'):
  for m in re.finditer(r'(\w+)::?\n((?:[ \t]*\.string[^\n]*\n)+)',p.read_text()):
   label=m[1];t=''.join(re.findall(r'\.string "(.*)"',m[2]));rows.append(dict(label=label,source=str(p),text=t))
   for para in t.split('\\p'):
    if para.count('\\n')>1:issues.append([str(p),label,'third-line',para])
   for line in re.split(r'\\[npl]|\$',t):
    clean=re.sub(r'\{(?:PLAYER|RIVAL)\}','WWWWWWW',line);clean=re.sub(r'\{[^}]+\}','',clean)
    w=sum(widths[chars.get(c,0)] for c in clean)
    if w>208:issues.append([str(p),label,'width '+str(w),line])
Path('docs/gym-story-playthrough/dialogue-source-audit.json').write_text(json.dumps(rows,indent=2)+'\n')
Path('/tmp/gym-story-qa/text-issues.json').write_text(json.dumps(issues,indent=2))
print(len(rows),'messages,',len(issues),'layout candidates')
for i in issues:print(i)
