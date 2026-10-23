#!/usr/bin/env python3
"""Check every active description against the entry page's four-line window."""
from pathlib import Path
import re
import unittest

ROOT = Path(__file__).resolve().parents[1]

class PokedexTextLayoutTest(unittest.TestCase):
    def test_active_descriptions_fit(self):
        font = (ROOT / 'src/text.c').read_text()
        widths = list(map(int, re.findall(r'\d+', re.search(
            r'sFontNormalLatinGlyphWidths\[\]\s*=\s*\{(.*?)\};', font, re.S)[1])))
        chars = {c: int(n, 16) for c, n in re.findall(
            r"^'(.*)'\s*=\s*([0-9A-F]{2})\s*$", (ROOT / 'charmap.txt').read_text(), re.M)}
        chars["'"] = 0xB4
        active = re.findall(r'\[NATIONAL_DEX_(\w+)\]',
                            (ROOT / 'src/data/pokemon/active_pokedex.h').read_text())
        refs = dict(re.findall(r'\[NATIONAL_DEX_(\w+)\].*?\.description = (\w+),',
                              (ROOT / 'src/data/pokemon/pokedex_entries.h').read_text(), re.S))
        texts = dict(re.findall(r'const u8 (g\w+PokedexText)\[\] = _\((.*?)\);',
                               (ROOT / 'src/data/pokemon/pokedex_text_fr.h').read_text(), re.S))
        self.assertGreater(len(active), 400)
        for species in active:
            with self.subTest(species=species):
                body = texts[refs[species]]
                self.assertNotIn('#if', body)
                lines = ''.join(re.findall(r'"([^"\n]*)"', body)).split('\\n')
                self.assertTrue(all(lines))
                self.assertLessEqual(len(lines), 4)
                for line in lines:
                    # One pixel letter spacing; leave four pixels on either side.
                    self.assertLessEqual(sum(widths[chars[c]] + 1 for c in line), 232)

if __name__ == '__main__':
    unittest.main()
