"""Build the dashboard typeface from the existing game font (Pillow + fontTools)."""
from pathlib import Path
import re
from PIL import Image
from fontTools.fontBuilder import FontBuilder
from fontTools.pens.ttGlyphPen import TTGlyphPen

ROOT = Path(__file__).resolve().parents[2]
image = Image.open(ROOT / 'graphics/fonts/latin_normal.png')
widths = [int(n) for n in re.findall(r'\d+', re.search(r'sFontNormalLatinGlyphWidths\[\]\s*=\s*\{(.*?)\};', (ROOT / 'src/text.c').read_text(), re.S)[1])]
mapping = {}
for char, index in re.findall(r"^'(.+)'\s*=\s*([0-9A-F]{2})\s*$", (ROOT / 'charmap.txt').read_text(), re.M):
    if len(char) == 1:
        mapping[ord(char)] = int(index, 16)
mapping[39] = mapping[0x2019]
mapping.update({0x2019: mapping[ord("'")], 0x2018: mapping[ord("'")], 0x2013: mapping[ord('-')], 0x2014: mapping[ord('-')]})
order = ['.notdef'] + [f'u{c:04X}' for c in sorted(mapping)]
font = FontBuilder(1024, isTTF=True)
font.setupGlyphOrder(order)
font.setupCharacterMap({c:f'u{c:04X}' for c in mapping})
glyphs, metrics = {}, {}
for name in order:
    pen = TTGlyphPen(None)
    index = mapping[int(name[1:],16)] if name != '.notdef' else mapping[ord('?')]
    x0, y0 = index % 16 * 16, index // 16 * 16
    for y in range(16):
        for x in range(16):
            if image.getpixel((x0+x,y0+y)) == 1:
                left, bottom = x*64, (12-y)*64
                pen.moveTo((left,bottom)); pen.lineTo((left,bottom+64)); pen.lineTo((left+64,bottom+64)); pen.lineTo((left+64,bottom)); pen.closePath()
    glyphs[name] = pen.glyph()
    metrics[name] = (widths[index]*64,0)
font.setupGlyf(glyphs)
font.setupHorizontalMetrics(metrics)
font.setupHorizontalHeader(ascent=832,descent=-192)
font.setupNameTable({'familyName':'Expedition Pixel','styleName':'Regular','uniqueFontIdentifier':'ExpeditionPixel-Regular','fullName':'Expedition Pixel','psName':'ExpeditionPixel-Regular'})
font.setupOS2(sTypoAscender=832,sTypoDescender=-192,usWinAscent=832,usWinDescent=192)
font.setupPost()
font.setupMaxp()
font.save(ROOT / 'tools/debug_dashboard/static/pixel.ttf')
