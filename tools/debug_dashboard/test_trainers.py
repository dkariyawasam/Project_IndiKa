import copy
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch
import trainers
ROOT=Path(__file__).resolve().parents[2]
class TrainerTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):cls.data=trainers.catalog(ROOT)
    def test_reference(self):
        t=next(t for t in self.data['trainers'] if t['name']=='KARDINAL')
        self.assertEqual(len(t['team']),6)
        self.assertIn('RocketLeague_Arena',t['locations'])
        self.assertEqual(len(t['overworlds']),1)
        self.assertIn('floating lights',t['dialogue'][0]['text'])
        self.assertTrue(trainers.portrait(ROOT,t['key']).startswith(b'\x89PNG'))
    def test_save_backup_validation_and_conflict(self):
        t=next(t for t in self.data['trainers'] if t['name']=='KARDINAL')
        with tempfile.TemporaryDirectory() as tmp:
            root=Path(tmp);path=root/trainers.PARTIES;path.parent.mkdir(parents=True)
            original=(ROOT/trainers.PARTIES).read_text();path.write_text(original)
            payload=dict(trainer=t['key'],revision=t['revision'],team=copy.deepcopy(t['team']))
            with patch.object(trainers,'catalog',return_value=self.data):
                payload['team'][0]['level']=101
                with self.assertRaises(ValueError):trainers.save(root,payload)
                self.assertEqual(path.read_text(),original)
                payload['team'][0]['level']=40
                result=trainers.save(root,payload)
                self.assertEqual((Path(result['backup'])/'trainer_parties.h').read_text(),original)
                edited=trainers.parties(path.read_text())[t['party']]
                self.assertEqual(trainers.team(edited[3])[0]['level'],40)
                # Every other party remains byte-for-byte unchanged.
                for key,m in trainers.parties(original).items():
                    if key!=t['party']:self.assertEqual(m[0],trainers.parties(path.read_text())[key][0])
                with self.assertRaises(ValueError):trainers.save(root,payload)
