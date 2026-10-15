import unittest
from pathlib import Path
from quests import catalog

ROOT = Path(__file__).resolve().parents[2]

class QuestCatalogueTest(unittest.TestCase):
    def test_source_catalogue_has_current_quests_and_promises(self):
        data = catalog(ROOT)
        self.assertEqual(data['counts'], dict(main=9, objectives=30, promises=21))
        self.assertTrue(data['sourceOnly'])
        self.assertEqual({c['name']: len(c['milestones']) for c in data['research']}, {'Bond': 33, 'Design': 13, 'Instinct': 8})
        self.assertFalse(any(q['name'] == 'PORYGON RESEARCH' for q in data['quests']))
        self.assertIn('EVOLUTION BY DESIGN', [o['name'] for o in data['quests'][0]['objectives']])
        reminders = data['quests'][7]['objectives']
        self.assertEqual(len({r['completion'] for r in reminders}), 21)
        self.assertTrue(all(r['completion'].startswith('FLAG_') for r in reminders))

    def test_dynamic_reader_excludes_unrelated_static_dialogue(self):
        data = catalog(ROOT)
        keys = {v['key'] for v in data['variants']}
        self.assertIn('sText_LogbookTestTheDubiousDiscWithPorygon2', keys)
        self.assertIn('gText_BrockTrialPhase1', keys)
        self.assertNotIn('sText_LogbookFieldPromises', keys)
        self.assertNotIn('sText_LogbookHaunterTrade', keys)
        self.assertTrue(all(q['name'] and q['description'] for q in data['quests']))

if __name__ == '__main__':
    unittest.main()
