#!/usr/bin/env python3
"""Convert the generated footprint atlas to the game's 16x16, 1bpp assets."""
from pathlib import Path
import json
import re
from PIL import Image, ImageDraw

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / 'docs/footprint-audit/custom'
# Atlas order, C symbol suffix, and final contact-stamp bounding box.
PRINTS = [
    ('mime_sr', 'MimeSr', (7, 12)),
    ('osscythe', 'Osscythe', (11, 13)),
    ('omato', 'Omato', (8, 8)),
    ('omatops', 'Omatops', (11, 8)),
    ('cinnabar_magikarp', 'CinnabarMagikarp', (5, 7)),
    ('cinnabar_feebas', 'CinnabarFeebas', (5, 7)),
    ('kabustar', 'Kabustar', (9, 9)),
    ('kabuknight', 'Kabuknight', (6, 12)),
    ('amunyte', 'Amunyte', (9, 6)),
    ('kinkabuto', 'Kinkabuto', (9, 8)),
    ('aeropteryx', 'Aeropteryx', (12, 13)),
]


def main():
    atlas = Image.open(OUT / 'source.png').convert('RGBA')
    background = Image.new('RGBA', atlas.size, 'white')
    background.alpha_composite(atlas)
    atlas = background.convert('L')
    table_path = ROOT / 'src/data/pokemon_graphics/footprint_table.h'
    defs_path = ROOT / 'src/data/graphics/pokemon.h'
    declarations_path = ROOT / 'include/graphics.h'
    declarations = declarations_path.read_text()
    table, defs = table_path.read_text(), defs_path.read_text()
    preview = Image.new('RGB', (600, 360), '#f4eddb')
    draw = ImageDraw.Draw(preview)
    manifest = {}
    for i, (species, suffix, size) in enumerate(PRINTS):
        col, row = i % 3, i // 3
        cell = atlas.crop((round(col * atlas.width / 3), round(row * atlas.height / 4),
                           round((col + 1) * atlas.width / 3), round((row + 1) * atlas.height / 4)))
        mask = cell.point(lambda p: 255 if p < 128 else 0)
        bounds = mask.getbbox()
        assert bounds, species
        # Area sampling preserves narrow contact marks during GBA-size conversion.
        stamp = cell.crop(bounds).resize(size, Image.Resampling.BOX).point(lambda p: 0 if p < 160 else 255, mode='1')
        footprint = Image.new('1', (16, 16), 1)
        footprint.paste(stamp, ((16 - size[0]) // 2, (16 - size[1]) // 2))
        path = ROOT / 'graphics/pokemon' / species / 'footprint.png'
        path.parent.mkdir(parents=True, exist_ok=True)
        footprint.save(path)
        symbol = 'gMonFootprint_' + suffix
        table, count = re.subn(r'(\[SPECIES_' + species.upper() + r'\]\s*=\s*)\w+',
                              lambda m: m[1] + symbol, table)
        assert count == 1, species
        if not re.search(r'const u8 ' + symbol + r'\[\]', defs):
            defs += '\nconst u8 ' + symbol + '[] = INCBIN_U8("graphics/pokemon/' + species + '/footprint.1bpp");\n'
        if not re.search(r'extern const u8 ' + symbol + r'\[\]', declarations):
            declarations += '\nextern const u8 ' + symbol + '[];\n'
        manifest[species.upper()] = {'path': str(path.relative_to(ROOT)), 'symbol': symbol, 'atlas_cell': i}
        x, y = i % 4 * 150, i // 4 * 120
        label = species.replace('cinnabar_', 'deep ').replace('_', ' ').title()
        draw.text((x + 8, y + 8), label, fill='#303030')
        preview.paste(footprint.convert('RGB').resize((64, 64), Image.Resampling.NEAREST), (x + 8, y + 30))
        preview.paste(footprint.convert('RGB'), (x + 90, y + 54))
    table_path.write_text(table)
    defs_path.write_text(defs)
    declarations_path.write_text(declarations)
    (OUT / 'manifest.json').write_text(json.dumps(manifest, indent=2) + '\n')
    preview.save(OUT / 'preview.png')


if __name__ == '__main__':
    main()
