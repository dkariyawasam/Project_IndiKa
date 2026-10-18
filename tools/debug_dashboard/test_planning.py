import copy
import json
import tempfile
import unittest
from pathlib import Path
from planning import encounters, save_encounters, WILD
ROOT=Path(__file__).resolve().parents[2]
class PlanningTests(unittest.TestCase):
    def test_save_preserves_other_tables_and_rejects_stale_or_invalid(self):
        with tempfile.TemporaryDirectory() as tmp:
            root=Path(tmp);p=root/WILD;p.parent.mkdir(parents=True);original=(ROOT/WILD).read_text();p.write_text(original)
            data=encounters(root);t=data['tables'][0];payload=dict(id=t['id'],revision=data['revision'],rate=t['rate'],slots=copy.deepcopy(t['slots']))
            species=[dict(key=s['species'].removeprefix('SPECIES_')) for s in t['slots']]
            payload['slots'][0]['min_level']=101
            with self.assertRaises(ValueError):save_encounters(root,species,payload)
            self.assertEqual(p.read_text(),original)
            payload['slots'][0]['min_level']=1
            result=save_encounters(root,species,payload)
            self.assertEqual((Path(result['backup'])/'wild_encounters.json').read_text(),original)
            after=encounters(root)
            self.assertEqual(after['tables'][1:],data['tables'][1:])
            self.assertEqual(after['tables'][0]['slots'][0]['min_level'],1)
            with self.assertRaises(ValueError):save_encounters(root,species,payload)
    def test_weights_match_slots(self):
        for t in encounters(ROOT)['tables']:
            self.assertEqual(len(t['weights']),len(t['slots']),t['id'])
            self.assertGreater(sum(t['weights']),0)

    def test_active_pokemon_assets_include_padded_declarations(self):
        import re
        from planning import assets
        active=set(re.findall(r'\[NATIONAL_DEX_(\w+)\] = TRUE',(ROOT/'src/data/pokemon/active_pokedex.h').read_text()))
        species=[dict(key=k,id=int(v),name=k) for k,v in re.findall(r'#define SPECIES_(\w+)\s+(\d+)',(ROOT/'include/constants/species.h').read_text()) if k in active]
        rows=[r for r in assets(ROOT,species)['rows'] if r['group']=='Pokémon']
        self.assertEqual(len(rows),404)
        self.assertTrue(all(all(r['checks'].values()) for r in rows))
        placeholders={r['key'] for r in rows if any('Placeholder back' in issue for issue in r['issues'])}
        self.assertEqual(placeholders,{'CINNABAR_FEEBAS','KABUSTAR','KABUKNIGHT','AEROPTERYX'})
        for key in ('METAPOD','COMBEE','KABUSTAR','AEROPTERYX'):
            self.assertTrue(next(r for r in rows if r['key']==key)['checks']['icon'])
