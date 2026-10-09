"""Natural forest warps must trigger and have a traversable approach/landing."""
import json
import re
import struct
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
LAYOUTS = {x['id']: x for x in json.loads((ROOT / 'data/layouts/layouts.json').read_text())['layouts']}


def block(name, x, y):
    m = json.loads((ROOT / 'data/maps' / name / 'map.json').read_text())
    layout = LAYOUTS[m['layout']]
    data = (ROOT / layout['blockdata_filepath']).read_bytes()
    value = struct.unpack_from('<H', data, 2 * (y * layout['width'] + x))[0]
    tile = value & 1023
    key = 'primary_tileset' if tile < 640 else 'secondary_tileset'
    tileset = re.sub(r'(?<!^)(?=[A-Z])', '_', layout[key].removeprefix('gTileset_')).lower()
    kind = 'primary' if tile < 640 else 'secondary'
    attrs = (ROOT / 'data/tilesets' / kind / tileset / 'metatile_attributes.bin').read_bytes()
    behavior = struct.unpack_from('<I', attrs, 4 * (tile if tile < 640 else tile - 640))[0] & 0x1FF
    return value, behavior


class NaturalForestExits(unittest.TestCase):
    def test_directional_warps_and_clear_approaches(self):
        # dy points from the warp toward the walkable approach/arrival path.
        cases = [
            ('Route2', 4, 15, -1, 0x65), ('Route2', 5, 15, -1, 0x65),
            ('Route2', 4, 48, 1, 0x64), ('Route2', 5, 48, 1, 0x64),
            ('ViridianForest', 4, 9, 1, 0x64), ('ViridianForest', 5, 9, 1, 0x64),
            ('ViridianForest', 43, 62, -1, 0x65), ('ViridianForest', 44, 62, -1, 0x65),
            ('Route15', 26, 18, 1, 0x64), ('Route15', 27, 18, 1, 0x64),
            ('FuchsiaForest', 5, 9, 1, 0x64), ('FuchsiaForest', 6, 9, 1, 0x64),
            ('FuchsiaForest', 43, 40, -1, 0x65), ('FuchsiaForest', 44, 40, -1, 0x65),
            ('VermilionHarbor', 26, 48, -1, 0x65), ('VermilionHarbor', 27, 48, -1, 0x65),
        ]
        for name, x, y, dy, expected in cases:
            with self.subTest(map=name, x=x, y=y):
                value, behavior = block(name, x, y)
                self.assertEqual((value >> 10) & 3, 0)
                self.assertEqual(behavior, expected)
                for distance in (1, 2):
                    approach, _ = block(name, x, y + dy * distance)
                    self.assertEqual((approach >> 10) & 3, 0)
                events = json.loads((ROOT / 'data/maps' / name / 'map.json').read_text())['warp_events']
                self.assertTrue(any(e['x'] == x and e['y'] == y for e in events))


if __name__ == '__main__':
    unittest.main()
