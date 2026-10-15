import unittest
from catalog import ROOT
from items import catalog,icon

class ItemTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):cls.items={e['key']:e for e in catalog(ROOT)['items']}
    def test_descriptions(self):
        self.assertGreater(len(self.items),240)
        self.assertFalse([e['key'] for e in self.items.values() if e['description']=='No description found.'])
        self.assertIn('20 points',self.items['ITEM_POTION']['description'])
        self.assertIn('Teaches Focus Punch',self.items['ITEM_TM01']['usage'][0])
    def test_evolution_and_source_evidence(self):
        stone=self.items['ITEM_THUNDER_STONE']
        self.assertTrue(any('PIKACHU → RAICHU' in s for s in stone['usage']))
        self.assertTrue(stone['referenced'])
        for ref in stone['references']:
            self.assertTrue((ROOT/ref['path']).is_file())
            self.assertGreater(ref['line'],0)
        self.assertIn('ITEM_PATCH_DISC',self.items)

    def test_icons_and_acquisition(self):
        for item in self.items.values():
            self.assertTrue(icon(ROOT,item['key']).startswith(b'\x89PNG'))
        for key in ['ITEM_POTION','ITEM_UP_GRADE','ITEM_EXP_CANDY_L','ITEM_TM59']:
            self.assertEqual(self.items[key]['availability'],'source',key)
            self.assertTrue(self.items[key]['acquisition'])
        self.assertEqual(self.items['ITEM_PATCH_DISC']['availability'],'unavailable')
        self.assertFalse(self.items['ITEM_PATCH_DISC']['acquisition'])

    def test_scripted_gifts_and_variable_awards(self):
        for key in ['ITEM_MASTER_BALL','ITEM_EXP_SHARE','ITEM_BOULDER_KEY','ITEM_FRESH_WATER','ITEM_POMEG_BERRY','ITEM_MOOMOO_MILK','ITEM_BLACK_BELT']:
            self.assertEqual(self.items[key]['availability'],'source',key)
        self.assertEqual(self.items['ITEM_RAZOR_CLAW']['availability'],'unconfirmed')

    def test_intentional_distribution_exclusions(self):
        for key in ['ITEM_DAWN_STONE', 'ITEM_PATCH_DISC']:
            self.assertEqual(self.items[key]['availability'], 'unavailable', key)
            self.assertFalse(self.items[key]['acquisition'], key)
            self.assertTrue(self.items[key]['availabilityNote'])
        import re
        retired=re.findall(r'RETIRED_ITEM\((ITEM_\w+)\)', (ROOT/'include/constants/retired_items.h').read_text())
        self.assertFalse(set(retired) & self.items.keys())
        milk=self.items['ITEM_MOOMOO_MILK']['acquisition']
        self.assertTrue(milk)
        self.assertTrue(all('DepartmentStore_Roof' in source['path'] for source in milk))
