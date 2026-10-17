"""Read-only official-game reference; never infer availability from dex membership."""
import json
from functools import lru_cache
from pathlib import Path

@lru_cache(maxsize=1)
def snapshot():
    return json.loads((Path(__file__).parent/'official_encounters.json').read_text())

CUSTOM = {'OMATO','OMATOPS','KABUSTAR','KABUKNIGHT','MIME_SR','OSSCYTHE',
          'CINNABAR_FEEBAS','CINNABAR_MAGIKARP','AMUNYTE','KINKABUTO','AEROPTERYX','PORYGON3',
          'PICHU_ALOLAN','PIKACHU_ALOLAN','EXEGGCUTE_ALOLAN','CUBONE_ALOLAN','KOFFING_GALARIAN'}

def for_species(key):
    data = snapshot()
    name = key.lower().replace('_','-').replace('-alolan','-alola').replace('-galarian','-galar').replace('-paldean','-paldea')
    name = {'deoxys':'deoxys-normal'}.get(name,name)
    status = 'custom' if key in CUSTOM else 'known' if name in data['pokemon'] else 'unmapped'
    return dict(status=status, name=name, regions=data['pokemon'].get(name,[]) if status=='known' else [],
                source=data['source'], retrieved=data['retrieved'], coverage=data['coverage'])
