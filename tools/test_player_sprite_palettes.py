#!/usr/bin/env python3
"""Check player PNG indices against runtime palettes and built tile data."""
from pathlib import Path
from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
PLAYER = ROOT / 'graphics/object_events/pics/people'


def check(path, palette_path, frame_width):
    palette = [tuple(int(v) >> 3 for v in line.split())
               for line in palette_path.read_text().splitlines()[3:]]
    with Image.open(path) as im:
        assert im.mode == 'P', path
        rgb = im.getpalette()
        pixels = im.tobytes()
        for index in set(pixels) - {0}:
            actual = tuple(v >> 3 for v in rgb[index * 3:index * 3 + 3])
            assert actual == palette[index], (path, index, actual, palette[index])
        # gbagfx emits each animation frame as a column/row grid of 8x8 tiles.
        built = path.with_suffix('.4bpp').read_bytes()
        decoded = bytes(nibble for byte in built for nibble in (byte & 15, byte >> 4))
        expected = bytearray()
        frame_height = 32
        for fy in range(0, im.height, frame_height):
            for fx in range(0, im.width, frame_width):
                for ty in range(fy, fy + frame_height, 8):
                    for tx in range(fx, fx + frame_width, 8):
                        for y in range(ty, ty + 8):
                            expected.extend(pixels[y * im.width + tx:y * im.width + tx + 8])
        assert decoded == expected, f'Stale or incorrectly tiled graphics: {path}'


paths = sorted(p for p in PLAYER.glob('*.png') if p.stem.startswith(('red_', 'green_')))
assert len(paths) == 14
for path in paths:
    action = path.stem.split('_', 1)[1]
    width = 32 if action in ('bike', 'fish', 'surf', 'vs_seeker_bike') else 16
    check(path, ROOT / 'graphics/object_events/palettes/player.pal', width)
print('Passed: all 14 player overworld sheets use the runtime palette and correct built tile indices.')
