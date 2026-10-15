"""Overworld source animation and indexed-transparency regression checks."""
import struct
import unittest
from pathlib import Path
from overworlds import catalog, transparent_png
ROOT=Path(__file__).resolve().parents[2]

class OverworldTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):cls.entries={e['id']:e for e in catalog(ROOT)}
    def test_every_registered_graphic_is_listed(self):
        import re
        text=(ROOT/'src/data/object_events/object_event_graphics_info.h').read_text()
        self.assertEqual(len(self.entries),len(re.findall(r'const struct ObjectEventGraphicsInfo \w+ =',text)))
    def test_real_standard_walk_and_east_flip(self):
        e=self.entries['gObjectEventGraphicsInfo_AromaLady']
        self.assertEqual([f['frame'] for f in e['walks']['south']],[3,0,4,0])
        self.assertTrue(all(f['flip'] for f in e['walks']['east']))
        self.assertFalse(any(f['flip'] for f in e['walks']['west']))
        self.assertEqual(e['frames'][9]['index'],9)
    def test_rechallenge_uses_declared_frame_and_zero_duration(self):
        e=self.entries['gObjectEventGraphicsInfo_AromaLady']
        self.assertEqual(e['rechallenge'][0]['frame'],9)
        self.assertEqual(e['rechallenge'][0]['duration'],0)
        self.assertEqual(self.entries['gObjectEventGraphicsInfo_ItemBall']['rechallenge'],[])

    def test_actual_frame_aliases_are_preserved(self):
        e=self.entries['gObjectEventGraphicsInfo_Bruno']
        self.assertTrue(e['frames'])
        self.assertEqual(len(e['walks']),4)
    def test_transparency_does_not_recolour_palette(self):
        p=ROOT/'graphics/object_events/pics/people/black_belt_f.png'
        def chunks(data):
            result={};i=8
            while i<len(data):
                size=struct.unpack('>I',data[i:i+4])[0];result[data[i+4:i+8]]=data[i+8:i+8+size];i+=size+12
            return result
        before=chunks(p.read_bytes());after=chunks(transparent_png(p))
        self.assertEqual(after[b'tRNS'],b'\0')
        self.assertEqual(before[b'PLTE'],after[b'PLTE'])
        self.assertEqual(before[b'IDAT'],after[b'IDAT'])

if __name__=='__main__':unittest.main()
