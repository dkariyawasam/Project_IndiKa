#!/usr/bin/env python3
"""Validate added moves, learnsets, TM mapping, compatibility and distribution."""
import json,re,subprocess,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def read(p):return (ROOT/p).read_text()
r=json.loads(read('tools/move_pool_expansion.json'))
blocks=dict(re.findall(r'\[MOVE_(\w+)\] =\s*\{(.*?)\n    \}',read('src/data/battle_moves.h'),re.S))
const=read('include/constants/moves.h');count=int(re.search(r'#define MOVES_COUNT (\d+)',const)[1]);assert count<512
for n,m in r['moves'].items():
 b=blocks[n]
 for field,value in [('power',m['power']),('accuracy',m['accuracy']),('pp',m['pp']),('priority',m['priority']),('secondaryEffectChance',m['chance'])]:assert re.search(r'\.'+field+r' = '+str(value)+r',',b),(n,field)
 assert '.category = MOVE_CATEGORY_'+m['category'] in b
 assert '.type = TYPE_'+m['type'] in b
 assert '[MOVE_'+n+']' in read('src/data/text/move_names.h')
 assert '[MOVE_'+n+' - 1]' in read('src/move_descriptions.c')
 assert '@ '+n+'\n' in read('data/battle_anim_scripts.s')
ptr=dict(re.findall(r'\[SPECIES_(\w+)\]\s*=\s*(\w+)',read('src/data/pokemon/level_up_learnset_pointers.h')))
sets=dict(re.findall(r'static const u16 (\w+)\[\] = \{(.*?)\n\};',read('src/data/pokemon/level_up_learnsets.h'),re.S))
limit=int(re.search(r'MAX_LEVEL_UP_MOVES\s+(\d+)',read('include/constants/pokemon.h'))[1])
for name,body in sets.items():
 levels=[int(l) for l in re.findall(r'LEVEL_UP_MOVE\((\d+),',body)]
 assert len(levels)<=limit,name
for change in r['levelups']:
 assert f'LEVEL_UP_MOVE({change["level"]}, MOVE_{change["move"]})' in sets[ptr[change['species']]],change
for sp,moves in r['compatibility'].items():
 b=re.search(r'\[SPECIES_'+sp+r'\]\s*=\s*TMHM_LEARNSET\((.*?)\),',read('src/data/pokemon/tmhm_learnsets.h'),re.S)[1]
 for n in moves:assert f'TM{60+r["tms"].index(n)}_{n}' in b,(sp,n)
shop=read('src/shop.c');catalog=re.search(r'sRocketCoinTms\[\] = \{(.*?)\};',shop,re.S)[1];prices=re.search(r'sRocketCoinPrices\[\] = \{(.*?)\};',shop,re.S)[1]
assert len(re.findall(r'ITEM_TM\d+',catalog))==len(re.findall(r'\d+',prices))==64
for i,n in enumerate(r['tms']):assert f'ITEM_TM{60+i}' in catalog
# Test the actual C indexing macros, including bit 63 and inverse item mapping.
code='''#include <assert.h>
#include <stdint.h>
#include "constants/items.h"
int main(void) {
 assert(ITEM_GOLD_TEETH == 339 && ITEM_PATCH_DISC == 366);
 assert(NUM_TECHNICAL_MACHINES == 64);
 for (unsigned i=0;i<64;i++) {
  unsigned item=TM_INDEX_TO_ITEM(i);
  assert(IS_TM_ITEM(item)); assert(ITEM_TO_TM_INDEX(item)==i);
  uint64_t bit=(uint64_t)1 << ITEM_TO_TM_INDEX(item);
  assert(bit && !(bit & (bit-1)));
 }
 assert(!IS_TM_ITEM(ITEM_GALARICA_CUFF));
 assert(!IS_TM_ITEM(ITEM_GOLD_TEETH));
 return 0;
}'''
with tempfile.TemporaryDirectory() as tmp:
 p=Path(tmp);(p/'test.c').write_text(code)
 subprocess.run(['cc','-Wall','-Wextra','-Werror','-I',str(ROOT/'include'),str(p/'test.c'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
print(f'PASS: {len(r["moves"])} moves, {len(r["levelups"])} level-up additions, 64 TM indices and coin-shop prices')
