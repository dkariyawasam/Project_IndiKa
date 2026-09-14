exec(open('/tmp/gym-story-qa/generate.py').read().split('spec=')[0])
rows=[]
def add(name,mapname,label,var,expected,ready=None,x=None,y=None,auto=False,item=None):
 j=json.loads(Path('data/maps',mapname,'map.json').read_text());mp=val(j['id']);o=None
 if x is None:
  for n,z in enumerate(j['object_events']):
   if any(k in z['graphics_id'] for k in ['BROCK','MISTY','SURGE','ERIKA','KOGA','SABRINA']):o=z;oid=z.get('local_id',n+1);oid=val(oid) if isinstance(oid,str) else oid;break
  x=o['x'];y=o['y']+1
 else:oid=1
 r=dict(name=name,g=mp>>8,m=mp&255,x=x,y=y,id=oid,script=S[label] if label else 0,var=val(var),expected=expected)
 if ready is not None:r['ready']=ready
 if o and o['flag']!='0':r['hide']=val(o['flag']) if isinstance(o['flag'],str) else o['flag']
 if auto:r['auto']=True
 if item:r['item']=val(item)
 rows.append(r)
for mapn,var in [('PewterCity_Gym','BROCK'),('CeruleanCity_Gym','MISTY'),('VermilionCity_Gym','LT_SURGE'),('CeladonCity_Gym','ERIKA'),('FuchsiaCity_Gym','KOGA'),('SaffronCity_Gym','SABRINA')]:
 j=json.loads(Path('data/maps',mapn,'map.json').read_text());w=j['warp_events'][1]
 add(var+'-guide',mapn,None,'VAR_'+var+'_TRIAL_STATE',1,0,w['x'],w['y']-1,True)
add('Brock-first-meeting','MtMoon_B1F','MtMoon_B1F_EventScript_Brock','VAR_BROCK_TRIAL_STATE',2)
add('Brock-fossil-return','MtMoon_B1F','MtMoon_B1F_EventScript_Brock','VAR_BROCK_TRIAL_STATE',3,item='ITEM_HELIX_FOSSIL_SHARD')
add('Misty-first-meeting','RockTunnel_B2F','RockTunnel_B2F_EventScript_Misty','VAR_MISTY_TRIAL_STATE',2)
for name,label,expected,x,y in [('first-current','ChooseRight',3,11,5),('first-generator','MistyAtGeneratorFromEdge',4,21,20),('lower-request','MistyLowerScoutRequest',5,11,38),('second-current','LowerChooseRight',6,11,38),('second-generator','MistySecondGeneratorFromEdge',7,20,41),('third-current','ThirdChooseLeft',8,35,32),('final-generator','MistyAtFinalGenerator',9,42,23)]:
 add('Misty-'+name,'RockTunnel_B2F','RockTunnel_B2F_EventScript_'+label,'VAR_MISTY_TRIAL_STATE',expected,x=x,y=y)
add('Surge-tutoring','DiglettsCave_Southside_B1F','DiglettsCave_Southside_B1F_EventScript_LtSurge','VAR_LT_SURGE_TRIAL_STATE',3)
add('Erika-root-request','ViridianForest','ViridianForest_EventScript_Erika','VAR_ERIKA_TRIAL_STATE',2)
add('Erika-root-shown','ViridianForest','ViridianForest_EventScript_Erika','VAR_ERIKA_TRIAL_STATE',3,item='ITEM_ENERGY_ROOT')
add('Koga-volcano-request','CinnabarVolcano_1F','CinnabarVolcano_1F_EventScript_Koga','VAR_KOGA_TRIAL_STATE',2)
add('Koga-volcano-checkpoint','CinnabarVolcano_LeftCorridor_2F','CinnabarVolcano_LeftCorridor_2F_EventScript_Koga','VAR_KOGA_TRIAL_STATE',3)
add('Sabrina-tutoring','PokemonTower_3F','PokemonTower_3F_EventScript_Sabrina','VAR_SABRINA_TRIAL_STATE',3)
Path('/tmp/gym-story-qa/fields.lua').write_text('qa.fields={\n'+',\n'.join('{'+','.join(k+'='+('true' if v is True else json.dumps(v)) for k,v in r.items())+'}' for r in rows)+'\n}\n')
Path('docs/gym-story-playthrough/field-fixtures.json').write_text(json.dumps(rows,indent=2)+'\n')
print(len(rows),'field cases')
