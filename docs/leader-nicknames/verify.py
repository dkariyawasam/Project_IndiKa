from pathlib import Path
import re,subprocess,tempfile,json
r=Path(__file__).resolve().parents[2]
s=(r/'src/trainer_nicknames.c').read_text()
t=(r/'src/data/trainers.h').read_text();p=(r/'src/data/trainer_parties.h').read_text()
parties={n:re.findall(r'\.species = (SPECIES_\w+)',b) for n,b in re.findall(r'(sParty_\w+)\[\] = \{(.*?)\n\};',p,re.S)}
cases=[]
for trainer,body in re.findall(r'\[(TRAINER_\w+)\] = \{(.*?)\n    \},',t,re.S):
 party=re.search(r'\.party = \w+\((sParty_\w+)\)',body)
 if party and (party[1].startswith('sParty_Leader') or party[1]=='sParty_RocketLeagueChampionGiovanni' or trainer=='TRAINER_BOSS_GIOVANNI_2'):
  cases.append((trainer,parties[party[1]],trainer!='TRAINER_BOSS_GIOVANNI_2'))
assert len(cases)==52,len(cases)
# Compile the actual lookup with host types and ASCII strings, not a reimplementation.
code='#include <stdint.h>\n#include <stddef.h>\n#include <stdio.h>\n#include <assert.h>\n#include <string.h>\ntypedef uint8_t u8; typedef uint16_t u16; typedef uint32_t u32;\n#define POKEMON_NAME_LENGTH 10\n#define ARRAY_COUNT(x) (sizeof(x)/sizeof((x)[0]))\n#define _(x) x\n'
code+=s.replace('#include "global.h"','')
code+='\nint main(void) { const u8 *name; unsigned i;\n'
for trainer,mons,named in cases:
 for i,mon in enumerate(mons):
  code+=f'name=GetTrainerPokemonNickname({trainer},{mon},{i});\n'
  if named:
   code+='assert(name && strlen((const char*)name)>0 && strlen((const char*)name)<=10); for(i=0;name[i];i++)assert(name[i]>=\'A\' && name[i]<=\'Z\');\n'
   code+=f'printf("{trainer}\\t%d\\t{mon}\\t%s\\n",{trainer},name);\n'
  else:code+='assert(name==NULL);\n'
code+='assert(GetTrainerPokemonNickname(TRAINER_YOUNGSTER_BEN,SPECIES_GEODUDE,0)==NULL);\n'
code+='assert(strcmp((const char*)GetTrainerPokemonNickname(TRAINER_LEADER_LT_SURGE_6,SPECIES_ELECTABUZZ,4),"MAJOR")==0);\n'
code+='assert(strcmp((const char*)GetTrainerPokemonNickname(TRAINER_LEADER_BROCK_1,SPECIES_GEODUDE,0),(const char*)GetTrainerPokemonNickname(TRAINER_LEADER_BROCK_7,SPECIES_GOLEM,5))==0);\n'
code+='return 0;}\n'
with tempfile.TemporaryDirectory() as d:
 f=Path(d);(f/'test.c').write_text(code)
 subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror','-I',str(r/'include'),str(f/'test.c'),'-o',str(f/'test')],check=True)
 output=subprocess.check_output([str(f/'test')],text=True)
rows=[l.split('\t') for l in output.splitlines()];by={}
for trainer,tid,mon,name in rows:by.setdefault(trainer,{'trainer':trainer,'id':int(tid),'species':[],'names':[]});by[trainer]['species'].append(mon);by[trainer]['names'].append(name)
for c in by.values():assert len(c['names'])==len(set(c['names'])),c
(r/'docs/leader-nicknames/cases.json').write_text(json.dumps(list(by.values()),indent=2)+'\n')
print(f'PASS: {len(by)} named parties, {len(rows)} Pokemon; uppercase/length/unique names; evolution continuity; Silph Giovanni and unrelated trainer excluded.')
