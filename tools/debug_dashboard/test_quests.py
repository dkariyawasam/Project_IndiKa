import unittest
from pathlib import Path
from quests import catalog

ROOT = Path(__file__).resolve().parents[2]

class QuestCatalogueTest(unittest.TestCase):
    def test_source_catalogue_has_current_quests_and_promises(self):
        data = catalog(ROOT)
        self.assertEqual(data['counts'], dict(main=8, objectives=30, promises=21))
        self.assertTrue(data['sourceOnly'])
        self.assertNotIn('EXPEDITION HOMECOMING', [q['name'] for q in data['quests']])
        self.assertEqual([q['id'] for q in data['quests']], [0, 1, 2, 3, 4, 5, 7, 8])
        self.assertEqual({c['name']: len(c['milestones']) for c in data['research']}, {'Bond': 33, 'Design': 13, 'Instinct': 8})
        self.assertFalse(any(q['name'] == 'PORYGON RESEARCH' for q in data['quests']))
        self.assertIn('EVOLUTION BY DESIGN', [o['name'] for o in data['quests'][0]['objectives']])
        reminders = next(q for q in data['quests'] if q['name'] == 'FIELD PROMISES')['objectives']
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
