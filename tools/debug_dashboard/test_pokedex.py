import struct
import tempfile
import shutil
import unittest
from pathlib import Path
from catalog import build_catalog
from pokedex import catalog,sprite,details,save_offsets,OFFSET_FIELDS
ROOT=Path(__file__).resolve().parents[2]
class PokedexTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):cls.dex=catalog(ROOT,build_catalog()['species']);cls.by_key={e['key']:e for e in cls.dex['species']}
    def test_active_sources_resolve(self):
        self.assertEqual(len(self.dex['species']),404)
        self.assertEqual([(e['key'],e['issues']) for e in self.dex['species'] if e['issues']],[])
    def test_single_and_double_anchors(self):
        self.assertEqual(self.dex['anchors']['single'][:2],[[72,80],[176,40]])
        self.assertEqual(self.dex['anchors']['double'],[[32,80],[200,40],[90,88],[152,32]])
    def test_offsets_and_floating_species(self):
        self.assertEqual(self.by_key['BULBASAUR']['frontOffset'],16)
        self.assertEqual(self.by_key['BUTTERFREE']['elevation'],8)
    def test_save_and_conflict(self):
        with tempfile.TemporaryDirectory() as tmp:
            root=Path(tmp)
            for name in ['src/data/pokemon_graphics','src/data/graphics']:
                shutil.copytree(ROOT/name,root/name)
            shutil.copy(ROOT/'src/battle_anim_mons.c',root/'src/battle_anim_mons.c')
            for key in ['BULBASAUR','BUTTERFREE','CASTFORM']:
                mon=self.by_key.get(key) or catalog(ROOT,[dict(key=key,id=385,name='Castform')])['species'][0]
                expected={f:mon[f] for f in OFFSET_FIELDS}
                values={f:mon[f]+1 for f in OFFSET_FIELDS}
                payload=dict(species=key,expected=expected,values=values)
                save_offsets(root,[mon],payload)
                updated=catalog(root,[mon])['species'][0]
                self.assertEqual({f:updated[f] for f in OFFSET_FIELDS},values)
                with self.assertRaises(ValueError):save_offsets(root,[mon],payload)
                save_offsets(root,[mon],dict(species=key,expected=values,values=expected))
                with self.assertRaises(ValueError):save_offsets(root,[mon],dict(species=key,expected=expected,values={**values,'elevation':-1}))
            self.assertEqual((root/'src/battle_anim_mons.c').read_bytes(),(ROOT/'src/battle_anim_mons.c').read_bytes())

    def test_details_assets_and_evolution_family(self):
        for mon in self.dex['species']:
            info=details(ROOT,mon)
            self.assertNotEqual(info['description'],'No entry found.',mon['key'])
            for asset in ('cry','footprint'):self.assertTrue((ROOT/info[asset]).is_file(),mon['key'])
            self.assertEqual(len(info['palettes']['normal']['colors']),16)
        info=details(ROOT,self.by_key['IVYSAUR'])
        self.assertEqual([(e['source'],e['target']) for e in info['evolutions']],[('BULBASAUR','IVYSAUR'),('IVYSAUR','VENUSAUR')])
        self.assertIn('custom_aeropteryx',details(ROOT,self.by_key['AEROPTERYX'])['cry'])

    def test_shiny_preserves_pixels(self):
        def chunks(data):
            result={};i=8
            while i<len(data):
                n=struct.unpack('>I',data[i:i+4])[0];result[data[i+4:i+8]]=data[i+8:i+8+n];i+=12+n
            return result
        mon=self.by_key['BULBASAUR'];a=chunks(sprite(ROOT,mon,'front'));b=chunks(sprite(ROOT,mon,'front',True))
        self.assertEqual(a[b'IDAT'],b[b'IDAT']);self.assertNotEqual(a[b'PLTE'],b[b'PLTE']);self.assertEqual(a[b'tRNS'],b'\0')
if __name__=='__main__':unittest.main()
