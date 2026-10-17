import unittest
from official_encounters import for_species

class OfficialEncountersTest(unittest.TestCase):
    def test_kanto_location_in_johto_game(self):
        data=for_species('BULBASAUR')
        kanto=next(r for r in data['regions'] if r['region']=='kanto')
        self.assertIn('heartgold',[g['game'] for g in kanto['games']])
        self.assertNotIn('johto',[r['region'] for r in data['regions']])

    def test_forms_are_not_replaced_with_base_species(self):
        data=for_species('RAICHU_ALOLAN')
        self.assertEqual(data['name'],'raichu-alola')
        games={g['game'] for r in data['regions'] for g in r['games']}
        self.assertNotIn('red',games)
        self.assertIn('lets-go-pikachu',games)
        self.assertEqual(for_species('PIKACHU_ALOLAN')['status'],'custom')

    def test_customs_and_missing_records_are_distinct(self):
        self.assertEqual(for_species('KABUKNIGHT')['status'],'custom')
        self.assertEqual(for_species('ANNIHILAPE')['status'],'known')
        self.assertEqual(for_species('ANNIHILAPE')['regions'],[])
        self.assertEqual(for_species('UNKNOWN')['status'],'unmapped')

    def test_location_and_method_evidence(self):
        for region in for_species('DRATINI')['regions']:
            for game in region['games']:
                self.assertTrue(game['encounters'])
                for encounter in game['encounters']:
                    self.assertTrue(encounter['location'])
                    self.assertTrue(encounter['method'])
