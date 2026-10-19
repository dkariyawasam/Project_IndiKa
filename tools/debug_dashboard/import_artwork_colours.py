"""Cache official-artwork references distributed by PokéAPI; extract colour samples."""
import io,json,hashlib,urllib.request,urllib.error
from concurrent.futures import ThreadPoolExecutor
from collections import Counter
from datetime import date
from pathlib import Path
from PIL import Image
from catalog import build_catalog,ROOT
from official_encounters import CUSTOM
CACHE=ROOT/'.cache/debug-dashboard/official-artwork';CACHE.mkdir(parents=True,exist_ok=True)
def fetch(url):
    with urllib.request.urlopen(urllib.request.Request(url,headers={'User-Agent':'ExpeditionDashboard/1.0'}),timeout=30) as r:return r.read()
def main():
    index=json.loads(fetch('https://pokeapi.co/api/v2/pokemon?limit=100000'))
    ids={r['name']:r['url'].rstrip('/').split('/')[-1] for r in index['results']}
    aliases={'mr-mime':'mr-mime','farfetchd':'farfetchd','nidoran-f':'nidoran-f','nidoran-m':'nidoran-m','deoxys':'deoxys-normal','wormadam':'wormadam-plant','giratina':'giratina-altered','shaymin':'shaymin-land'}
    revision=json.loads(fetch('https://api.github.com/repos/PokeAPI/sprites/commits/master'))['sha']
    def load(mon):
        key=mon['key']
        if key in CUSTOM:return key,dict(status='custom')
        name=key.lower().replace('_','-').replace('-alolan','-alola').replace('-galarian','-galar').replace('-paldean','-paldea');name=aliases.get(name,name)
        if name not in ids:return key,dict(status='unmapped',name=name)
        url=f'https://raw.githubusercontent.com/PokeAPI/sprites/{revision}/sprites/pokemon/other/official-artwork/{ids[name]}.png'
        path=CACHE/(key+'.png')
        try:
            raw=fetch(url);path.write_bytes(raw)
            im=Image.open(io.BytesIO(raw)).convert('RGBA');im.thumbnail((128,128))
            pixels=[rgb[:3] for rgb in im.getdata() if rgb[3]>=240]
            sample=Image.new('RGB',(len(pixels),1));sample.putdata(pixels);q=sample.quantize(colors=24,method=Image.Quantize.MEDIANCUT)
            counts=Counter(q.getdata());pal=q.getpalette()
            colours=[dict(rgb=pal[i*3:i*3+3],share=count/len(pixels)) for i,count in counts.most_common() if count/len(pixels)>=.005]
            return key,dict(status='available',name=name,url=url,sha256=hashlib.sha256(raw).hexdigest(),colours=colours)
        except (urllib.error.URLError,OSError,ValueError) as e:return key,dict(status='unavailable',name=name,error=str(e))
    rows=dict(ThreadPoolExecutor(max_workers=8).map(load,build_catalog()['species']))
    output=dict(source='https://github.com/PokeAPI/sprites',revision=revision,retrieved=str(date.today()),note='Official-artwork collection mirrored by PokéAPI, not a Pokémon Company service. Normal artwork only.',species=rows)
    (ROOT/'tools/debug_dashboard/official_artwork_colours.json').write_text(json.dumps(output,indent=2)+'\n')
    print(dict(Counter(r['status'] for r in rows.values())))
    print({k:r for k,r in rows.items() if r['status'] not in ('available','custom')})
if __name__=='__main__':main()
