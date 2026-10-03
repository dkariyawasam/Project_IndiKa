"""Validate native town-map graphics, cursor coverage, and sprite budgets."""
import json
from pathlib import Path
import re
import struct
import unittest

ROOT = Path(__file__).resolve().parents[1]


class RegionMapTests(unittest.TestCase):
    def setUp(self):
        source = (ROOT / 'src/data/region_map/region_map_layout_kanto.h').read_text()
        self.layers = {}
        for layer in ('LAYER_MAP', 'LAYER_DUNGEON'):
            body = source.split('[' + layer + '] =')[1].split('\n    }')[0]
            self.layers[layer] = [re.findall(r'MAPSEC_\w+', row)
                                  for row in re.findall(r'\{(MAPSEC_[^{}]+)\}', body)]
        self.sections = {s['id']: s for s in json.loads(
            (ROOT / 'src/data/region_map/region_map_sections.json').read_text())['map_sections']}

    def test_cursor_cells_have_valid_bounds(self):
        for grid in self.layers.values():
            self.assertEqual(len(grid), 15)
            for y, row in enumerate(grid):
                self.assertEqual(len(row), 22)
                for x, name in enumerate(row):
                    if name == 'MAPSEC_NONE':
                        continue
                    section = self.sections[name]
                    self.assertLessEqual(section['x'], x)
                    self.assertLess(x, section['x'] + section['width'])
                    self.assertLessEqual(section['y'], y)
                    self.assertLess(y, section['y'] + section['height'])

    def test_harbour_forest_approach_runs_south(self):
        grid = self.layers['LAYER_MAP']
        chain = ('MAPSEC_VERMILION_CITY', 'MAPSEC_VERMILION_HARBOR',
                 'MAPSEC_FUCHSIA_FOREST', 'MAPSEC_ROUTE_15')
        city = next((x, y) for y, row in enumerate(grid)
                    for x, name in enumerate(row) if name == chain[0])
        x, y = city
        self.assertEqual(tuple(grid[y + offset][x] for offset in range(4)), chain)

    def test_native_graphics_fit_character_bank(self):
        gfx = ROOT / 'graphics/region_map'
        routes = (gfx / 'expedition_routes.bin').read_bytes()
        self.assertEqual(len(routes) % 32, 0)
        end = 320 + len(routes) // 32
        self.assertLessEqual(end, 512)
        entries = struct.unpack('<600H', (gfx / 'kanto.bin').read_bytes())
        for entry in entries:
            self.assertLess(entry & 1023, end)
            self.assertLess(entry >> 12, 5)

    def test_header_windows_do_not_overlap_tilemaps(self):
        source = (ROOT / 'src/region_map.c').read_text()
        templates = source.split('sRegionMapWindowTemplates[] =')[1].split('DUMMY_WIN_TEMPLATE')[0]
        occupied = set()
        for entry in re.findall(r'\{([^{}]+)\}', templates):
            fields = dict(re.findall(r'\.(\w+) = (0x[0-9a-f]+|[0-9]+)', entry))
            if 'baseBlock' not in fields:
                continue
            start = int(fields['baseBlock'], 0)
            end = start + int(fields['width']) * int(fields['height'])
            self.assertLessEqual(end, 0x180)
            tiles = set(range(start, end))
            self.assertFalse(occupied & tiles)
            occupied |= tiles
        edge = int(re.search(r'LoadBgTiles\(3, edgeTile, sizeof\(edgeTile\), (0x[0-9a-f]+)', source)[1], 0)
        self.assertNotIn(edge, occupied)
        self.assertLess(edge, 0x180)

    def test_landmarks_fit_sprite_capacity(self):
        icons = sum(name != 'MAPSEC_NONE' for row in self.layers['LAYER_DUNGEON'] for name in row)
        self.assertLessEqual(icons, 25)
        for section in ('MAPSEC_FUCHSIA_FOREST', 'MAPSEC_VIRIDIAN_CHANNEL',
                        'MAPSEC_VERMILION_HARBOR', 'MAPSEC_CINNABAR_VOLCANO',
                        'MAPSEC_CELADON_CAVE', 'MAPSEC_LEAGUE_ROCKET'):
            self.assertTrue(any(section in row for grid in self.layers.values() for row in grid), section)


if __name__ == '__main__':
    unittest.main()
