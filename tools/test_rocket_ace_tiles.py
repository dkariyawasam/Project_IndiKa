#!/usr/bin/env python3
"""Compare every packed Rocket Ace frame with its source sheet. Requires Pillow."""
from pathlib import Path
from PIL import Image
ROOT = Path(__file__).resolve().parents[1]
sheets = sorted((ROOT/'graphics/object_events/pics/people').glob('rocket_ace_*.png'))
assert len(sheets) == 8
for path in sheets:
    im = Image.open(path)
    assert im.mode == 'P' and im.size == (160, 32), path
    data = path.with_suffix('.4bpp').read_bytes()
    assert len(data) == 10 * 256, path
    for frame in range(10):
        for y in range(32):
            for x in range(16):
                offset = frame*256 + ((y//8)*2+x//8)*32 + (y%8)*4 + (x%8)//2
                actual = (data[offset] >> (4*(x%2))) & 15
                assert actual == im.getpixel((frame*16+x, y)), (path.name, frame, x, y)
print('PASS: all 80 Rocket Ace frames match their source pixels in engine tile order')

# Custom Ace palettes must be loaded by tag, not inherited from a fixed NPC slot.
import re
infos=(ROOT/'src/data/object_events/object_event_graphics_info.h').read_text()
aces=re.findall(r'const struct ObjectEventGraphicsInfo gObjectEventGraphicsInfo_RocketAce\w+ = \{(.*?)\n\};',infos,re.S)
assert len(aces)==8
for ace in aces:
    assert '.paletteSlot = PALSLOT_NPC_DYNAMIC' in ace
    assert '.paletteTag = OBJ_EVENT_PAL_TAG_ROCKET_ACE_' in ace
print('PASS: all eight Aces use their own dynamically loaded palette tags')
