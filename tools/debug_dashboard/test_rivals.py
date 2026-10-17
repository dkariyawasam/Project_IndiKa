import unittest
from pathlib import Path
from rivals import catalog

ROOT = Path(__file__).resolve().parents[2]

class RivalCatalogueTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.data = catalog(ROOT)
        cls.maps = {e['map']:e for e in cls.data['encounters']}

    def test_route_three_both_approaches(self):
        entry = self.maps['Route3']
        self.assertEqual((entry['x'],entry['y']),(70,6))
        self.assertEqual({(e['x'],e['y']) for e in entry['triggers']}, {(x,y) for x in range(68,73) for y in (5,7)})
        self.assertTrue(entry['hiddenBy'].startswith('FLAG_HIDE_'))

    def test_starter_variants_and_shared_reflection(self):
        self.assertEqual(len(self.maps['CeladonCity']['battles']),3)
        self.assertTrue(any('VAR_STARTER_MON' in b['condition'] for b in self.maps['CeladonCity']['branches']))
        self.assertTrue(any('EventScript_Rival' in b['script'] for b in self.maps['ViridianForest']['branches']))
        self.assertTrue(self.maps['PokemonLeague_ChampionsRoom']['battles'])

    def test_sources_and_scene_only_appearances(self):
        self.assertTrue(self.data['sourceOnly'])
        self.assertEqual(len(self.maps),13)
        self.assertEqual(self.maps['IndigoPlateau_Exterior']['interaction'],'0x0')
        for entry in self.maps.values():
            self.assertTrue((ROOT/entry['source']).is_file())
            for branch in entry['branches']:
                self.assertTrue((ROOT/branch['source']).is_file())
                self.assertGreater(branch['line'],0)

if __name__=='__main__':
    unittest.main()
