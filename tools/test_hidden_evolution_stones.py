#!/usr/bin/env python3
"""Check the habitat stone placements, independent flags and interaction access."""
import json
from pathlib import Path
import struct
import sys
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools/debug_dashboard'))
from catalog import constants

def main():
    plan=json.loads((ROOT/'docs/hidden-evolution-stones.json').read_text())['added']
    layouts={e['id']:e for e in json.loads((ROOT/'data/layouts/layouts.json').read_text())['layouts']}
    flags=constants();all_events=[]
    for path in (ROOT/'data/maps').glob('*/map.json'):
        all_events.extend((path.parent.name,e) for e in json.loads(path.read_text()).get('bg_events',[]) if e['type']=='hidden_item')
    for item in plan:
        name=item['map'];m=json.loads((ROOT/f'data/maps/{name}/map.json').read_text());l=layouts[m['layout']];w,h=l['width'],l['height'];grid=struct.unpack('<'+'H'*(w*h),(ROOT/l['blockdata_filepath']).read_bytes());x,y=item['x'],item['y']
        matching=[e for e in m['bg_events'] if e.get('item')==item['item']]
        assert len(matching)==1,(name,'duplicate stone')
        e=matching[0];assert (e['x'],e['y'])==(x,y)
        tile=grid[y*w+x];assert tile==int(item['clueMetatile'],16)
        assert e['elevation']==tile>>12
        assert e['quantity']==1 and not e['underfoot']
        same_flag=[(n,e) for n,e in all_events if flags[e['flag']]==flags[item['flag']]]
        assert len(same_flag)==1,(name,'shared hidden flag',same_flag)
        assert flags[item['flag']]-flags['FLAG_HIDDEN_ITEMS_START']<256
        blocked={(e['x'],e['y']) for e in m['object_events']+m['warp_events']+m['coord_events']}
        assert any(0<=a<w and 0<=b<h and not((grid[b*w+a]>>10)&3) and (e['elevation']==0 or grid[b*w+a]>>12 in (0,e['elevation'])) and (a,b) not in blocked for a,b in [(x-1,y),(x+1,y),(x,y-1),(x,y+1)]),(name,'cannot interact')
        assert (x,y) not in blocked
    for name,stone in [('Route1','SUN'),('MtMoon_B2F','MOON'),('PowerPlant','THUNDER'),('SafariZone_Center','LEAF')]:
        assert len([e for n,e in all_events if n==name and e['item']==f'ITEM_{stone}_STONE'])==1
    print(f'{len(plan)} new stones: unique flags, matching clues, adjacent interaction tiles and preserved existing stones verified.')
if __name__=='__main__':main()
