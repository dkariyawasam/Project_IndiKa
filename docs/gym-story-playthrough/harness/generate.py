from pathlib import Path
import re,json
D={}
for p in Path('include/constants').glob('*.h'):
 for m in re.finditer(r'^#define\s+(\w+)\s+([^\n]+)',p.read_text(),re.M):D[m[1]]=m[2].split('//')[0].strip()
def val(k,depth=0):
 assert depth<25,k
 if k not in D:return int(k,0)
 x=D[k];x=re.sub(r'\b[A-Za-z_]\w*\b',lambda m:str(val(m[0],depth+1)),x)
 assert re.fullmatch(r'[0-9xXa-fA-F\s()+|&<>~*/%-]+',x),x
 return eval(x,{'__builtins__':{}})
S=json.loads(Path('/tmp/gym-story-qa/symbols.json').read_text())
spec=[('Brock','PewterCity_Gym','BROCK',3,'ITEM_BOULDER_KEY'),('Misty','CeruleanCity_Gym','MISTY',9,'ITEM_CASCADE_BOARD'),('Surge','VermilionCity_Gym','LT_SURGE',3,'ITEM_THUNDER_PASS'),('Erika','ViridianForest','ERIKA',3,'ITEM_RAINBOW_KEY'),('Koga','CinnabarVolcano_RightCorridor_3F','KOGA',3,'ITEM_SOUL_PASS'),('Sabrina','PokemonTower_3F','SABRINA',3,'ITEM_MARSH_FINDER'),('Blaine','CinnabarIsland_Gym','BLAINE',None,None),('Giovanni','ViridianCity_Gym','GIOVANNI',None,None)]
rows=[]
for i,(name,mapname,gfx,state,item) in enumerate(spec):
 j=json.loads(Path('data/maps',mapname,'map.json').read_text());o=next((n,o) for n,o in enumerate(j['object_events']) if o['graphics_id']=='OBJ_EVENT_GFX_'+gfx);n,o=o
 mapid=val(j['id']);oid=o.get('local_id',n+1);oid=val(oid) if isinstance(oid,str) else oid
 row=dict(name=name,g=mapid>>8,m=mapid&255,x=o['x'],y=o['y']+1,id=oid,script=S[o['script']],hide=val(o['flag']) if isinstance(o['flag'],str) else o['flag'],badge=val('FLAG_BADGE0'+str(i+1)+'_GET'),defeat=val('FLAG_DEFEATED_'+('LEADER_GIOVANNI' if gfx=='GIOVANNI' else gfx)))
 if state is not None:row.update(var=val('VAR_'+gfx+'_TRIAL_STATE'),ready=state)
 if item:row['item']=val(item)
 rows.append(row)
Path('/tmp/gym-story-qa/cases.lua').write_text('qa.cases={\n'+',\n'.join('{'+','.join(k+'='+json.dumps(v) for k,v in r.items())+'}' for r in rows)+'\n}\n')
Path('docs/gym-story-playthrough/battle-fixtures.json').write_text(json.dumps(rows,indent=2)+'\n')
Path('/tmp/gym-story-qa/constants.json').write_text(json.dumps({k:val(k) for k in ['FLAG_SYS_POKEDEX_GET','FLAG_SYS_POKEMON_GET','FLAG_SYS_FLASH_ACTIVE','FLAG_KOGA_VOLCANO_JANINE_ARRIVED','FLAG_HIDE_CELADON_CITY_GYM_GUIDE']}))
print(rows)
