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
        east = blocks('Route18', 124)
        self.assertEqual((len(route), len(east)), (100, 70))
        # Remove the restored western strip to compare with the retained baseline.
        east = [row[:16] + row[32:] for row in east]
        self.assertEqual(sha(route[:50]), '07644be49e2728f6a159fd4305249c52eff588c0ac7840937f793c0fa5505485')
        self.assertEqual(sha([row[:60] + row[76:] for row in east[:20]]), '115f8d0bb1be46bc993175f7beb6c0a8733e273bc2e2b7fe788752b4874a4836')
        # The western sea was shortened by deleting original columns 16..31.
        self.assertEqual(sha([row[:52] for row in east[20:]]), '1bffc36f306eac17333b87fb75fe93918f39cdb4746b3b39ee968b68d5fae94a')
        self.assertTrue(all(v == 0x05d9 for row in east[20:] for v in row[52:]))
    def test_connections_and_events(self):
        load = lambda n: json.loads((ROOT / 'data/maps' / n / 'map.json').read_text())
        route, east = map(load, ['Route21_North', 'Route18'])
        self.assertEqual(len(route['object_events']), 18)
        groups = json.loads((ROOT / 'data/maps/map_groups.json').read_text())
        self.assertEqual(groups['reserved_maps']['Route21_South']['redirect'], 'Route21_North')
        self.assertFalse((ROOT / 'data/maps/Route21_South').exists())
        self.assertIn(dict(map='MAP_ROUTE18', offset=30, direction='right'), route['connections'])
        self.assertIn(dict(map='MAP_ROUTE21_NORTH', offset=-30, direction='left'), east['connections'])
        self.assertIn(dict(map='MAP_ROUTE20', offset=0, direction='down'), east['connections'])
        for name, width, height in [('Route21_North',24,100), ('Route18',124,70)]:
            self.assertLessEqual((width+15)*(height+14),0x2DA0)
            for event in load(name)['object_events']:
                self.assertTrue(0 <= event['x'] < width and 0 <= event['y'] < height)
if __name__ == '__main__':
    unittest.main()
