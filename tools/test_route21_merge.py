"""Regression checks for the Route 21 / Route 18 boundary reorganisation."""
import hashlib
import json
import struct
import unittest
from pathlib import Path
ROOT = Path(__file__).resolve().parents[1]
def blocks(name, width):
    raw = (ROOT / 'data/layouts' / name / 'map.bin').read_bytes()
    words = struct.unpack('<' + 'H' * (len(raw) // 2), raw)
    return [list(words[i:i+width]) for i in range(0, len(words), width)]
def sha(rows):
    words = [v for row in rows for v in row]
    return hashlib.sha256(struct.pack('<' + 'H' * len(words), *words)).hexdigest()
class Route21MergeTests(unittest.TestCase):
    def test_original_terrain_preserved(self):
        route = blocks('Route21_North', 24)
        east = blocks('Route18', 108)
        self.assertEqual((len(route), len(east)), (100, 70))
        self.assertEqual(sha(route[:50]), '07644be49e2728f6a159fd4305249c52eff588c0ac7840937f793c0fa5505485')
        self.assertEqual(sha(east[:20]), 'ae4267bdfb583691e2f853bb4172dee2e5390ce8796cd088e95d43e406fd36ee')
        reconstructed = [route[y+50] + east[y+20][:68] for y in range(50)]
        self.assertEqual(sha(reconstructed), '68ac0a410ea85e8159cddb1f7e8ba1efc60b92dc5c4055e5b9d82ceadb4c3c97')
        self.assertTrue(all(v == 0x05d9 for row in east[20:] for v in row[68:]))
    def test_connections_and_events(self):
        load = lambda n: json.loads((ROOT / 'data/maps' / n / 'map.json').read_text())
        route, east, legacy = map(load, ['Route21_North', 'Route18', 'Route21_South'])
        self.assertEqual(len(route['object_events']), 18)
        self.assertFalse(legacy['connections'])
        self.assertFalse(legacy['object_events'])
        self.assertIn(dict(map='MAP_ROUTE18', offset=30, direction='right'), route['connections'])
        self.assertIn(dict(map='MAP_ROUTE21_NORTH', offset=-30, direction='left'), east['connections'])
        self.assertIn(dict(map='MAP_ROUTE20', offset=0, direction='down'), east['connections'])
        for name, width, height in [('Route21_North',24,100), ('Route18',108,70)]:
            self.assertLessEqual((width+15)*(height+14),0x2880)
            for event in load(name)['object_events']:
                self.assertTrue(0 <= event['x'] < width and 0 <= event['y'] < height)
if __name__ == '__main__':
    unittest.main()
