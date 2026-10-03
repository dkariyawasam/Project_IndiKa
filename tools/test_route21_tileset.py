"""Check the shared boundary graphics without changing either map's terrain."""
import struct
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
TILESETS = ROOT / 'data/tilesets/secondary'

class Route21TilesetTests(unittest.TestCase):
    def test_cinnabar_graphics_and_metatiles_are_preserved(self):
        old = TILESETS / 'cinnabar_island'
        new = TILESETS / 'route21'
        for name in ('tiles.4bpp', 'metatiles.bin', 'metatile_attributes.bin'):
            self.assertTrue((new / name).read_bytes().startswith((old / name).read_bytes()))
        for palette in (8, 9):
            self.assertEqual((old / f'palettes/{palette:02}.gbapal').read_bytes(),
                             (new / f'palettes/{palette:02}.gbapal').read_bytes())

    def test_shared_physical_tiles_across_boundary(self):
        headers = (ROOT / 'src/data/tilesets/headers.h').read_text()
        for name in ('PalletTown', 'Route21'):
            body = headers.split('const struct Tileset gTileset_' + name + ' =')[1].split('};')[0]
            self.assertIn('.tiles = gTilesetTiles_Route21,', body)
        old = (TILESETS / 'pallet_town/metatiles.bin').read_bytes()
        new = (TILESETS / 'pallet_town/connected_metatiles.bin').read_bytes()
        for offset in range(0, len(old), 2):
            before = struct.unpack_from('<H', old, offset)[0]
            after = struct.unpack_from('<H', new, offset)[0]
            self.assertEqual(before & ~1023, after & ~1023)
            self.assertEqual(after & 1023, (before & 1023) + (128 if before & 1023 >= 640 else 0))

    def test_empty_viewport_border_is_not_translated(self):
        # Exercise the actual guard and assignment from the transition loop.
        import re, subprocess, tempfile
        source = (ROOT / 'src/fieldmap.c').read_text()
        body = re.search(r'u16 block = gSaveBlock2Ptr->mapView\[i\];(.*?)\n            }', source, re.S).group(1)
        body = body.replace('gSaveBlock2Ptr->mapView[i]', 'result')
        c = '#include <assert.h>\n#define MAPGRID_UNDEFINED 1023\n#define NUM_METATILES_IN_PRIMARY 640\n'
        c += 'int main(void) { unsigned block, result; int metatileOffset; '
        for offset in (64, -64):
            for value in (1023, 2, 516, 728 if offset == 64 else 792):
                expected = value if value < 640 or value == 1023 else value + offset
                c += f'block = result = {value}; metatileOffset = {offset}; {body} assert(result == {expected});'
        c += 'return 0;}'
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / 'guard.c'; path.write_text(c)
            subprocess.run(['cc', str(path), '-o', str(path.with_suffix(''))], check=True)
            subprocess.run([str(path.with_suffix(''))], check=True)

    def test_pallet_border_keeps_pixels_palettes_attributes_and_collision(self):
        old = TILESETS / 'pallet_town'
        new = TILESETS / 'route21'
        grid = struct.unpack('<480H', (ROOT / 'data/layouts/PalletTown/map.bin').read_bytes())
        old_tiles = (old / 'tiles.4bpp').read_bytes()
        new_tiles = (new / 'tiles.4bpp').read_bytes()
        for block in grid[-7 * 24:]:
            metatile = block & 1023
            if metatile < 640:
                continue
            mapped = (block & ~1023) | (metatile + 64)
            self.assertEqual(mapped & ~1023, block & ~1023)
            restored = (mapped & ~1023) | ((mapped & 1023) - 64)
            self.assertEqual(restored, block)
            i = metatile - 640
            a = struct.unpack_from('<8H', (old / 'metatiles.bin').read_bytes(), i * 16)
            b = struct.unpack_from('<8H', (new / 'metatiles.bin').read_bytes(), (i + 64) * 16)
            for before, after in zip(a, b):
                self.assertEqual(before & 0xC00, after & 0xC00)
                tile = before & 1023
                if tile >= 640:
                    self.assertEqual(after & 1023, tile + 128)
                    self.assertEqual(old_tiles[(tile-640)*32:(tile-639)*32],
                                     new_tiles[(tile+128-640)*32:(tile+129-640)*32])
                else:
                    self.assertEqual(after & 1023, tile)
                pal = before >> 12
                if pal >= 7:
                    self.assertEqual((old / f'palettes/{pal:02}.gbapal').read_bytes(),
                                     (new / f'palettes/{after >> 12:02}.gbapal').read_bytes())
            self.assertEqual((old / 'metatile_attributes.bin').read_bytes()[i*4:(i+1)*4],
                             (new / 'metatile_attributes.bin').read_bytes()[(i+64)*4:(i+65)*4])

if __name__ == '__main__':
    unittest.main()
