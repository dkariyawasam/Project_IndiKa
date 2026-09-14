exec(open('/tmp/gym-story-qa/generate.py').read().split('spec=')[0])
rows=[]
for name,mapn,label,v,state,flag,mon in [
 ('Brock-trade','PewterCity_Gym','PewterCity_Gym_EventScript_Brock','BROCK',4,'FLAG_DID_BROCK_GRAVELER_TRADE','SPECIES_RELICANTH'),
 ('Misty-trade','CeruleanCity_Gym','CeruleanCity_Gym_EventScript_Misty','MISTY',10,'FLAG_DID_MISTY_POLIWHIRL_TRADE','SPECIES_GYARADOS'),
 ('Surge-trade','VermilionCity_Gym','VermilionCity_Gym_EventScript_LtSurge','LT_SURGE',4,'FLAG_DID_ELECTABUZZ_TRADE','SPECIES_RAICHU'),
 ('Erika-gift','CeladonCity_Gym','CeladonCity_Gym_EventScript_Erika','ERIKA',4,'FLAG_GOT_BUDEW_FROM_ERIKA',None),
 ('Koga-gift','FuchsiaCity_Gym','FuchsiaCity_Gym_EventScript_Koga','KOGA',4,'FLAG_GOT_GOLBAT_FROM_KOGA',None),
 ('Blaine-generator','CinnabarIsland_Gym','CinnabarIsland_Gym_EventScript_BlaineFujiPhoto',None,None,'FLAG_GOT_PORYGON_FROM_BLAINE',None)]:
 j=json.loads(Path('data/maps',mapn,'map.json').read_text());mp=val(j['id']);o=j['object_events'][0];r=dict(name=name,g=mp>>8,m=mp&255,x=o['x'],y=o['y']+1,id=1,script=S[label],flag=val(flag),species=val(mon) if mon else 0)
 if v:r.update(var=val('VAR_'+v+'_TRIAL_STATE'),ready=state)
 if name=='Blaine-generator':r.update(x=5,y=3)
 rows.append(r)
Path('/tmp/gym-story-qa/followups.lua').write_text('qa.followups={\n'+',\n'.join('{'+','.join(k+'='+json.dumps(v) for k,v in r.items())+'}' for r in rows)+'\n}\n')
Path('docs/gym-story-playthrough/followup-fixtures.json').write_text(json.dumps(rows,indent=2)+'\n')
