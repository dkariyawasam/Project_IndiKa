#!/usr/bin/env python3
"""Audit built overworld palette wiring, PNG index permutations, and map demand.

Requires a built ROM/ELF, the ARM nm tool, and Pillow. Whole-map dynamic
counts are upper bounds, not simultaneous visibility or field-effect budgets.
"""
import json
import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
rom = (ROOT / 'pokefirered.gba').read_bytes()
nm = shutil.which('arm-none-eabi-nm') or str(Path(os.environ.get('DEVKITARM', '/opt/devkitpro/devkitARM')) / 'bin/arm-none-eabi-nm')
symbols = {parts[2]: int(parts[0], 16) - 0x08000000
           for line in subprocess.check_output([nm, str(ROOT / 'pokefirered.elf')], text=True).splitlines()
           if len(parts := line.split()) == 3}
pointers = (ROOT / 'src/data/object_events/object_event_graphics_info_pointers.h').read_text()
entries = re.findall(r'\[(OBJ_EVENT_GFX_\w+)\]\s*=\s*&?(gObjectEventGraphicsInfo_\w+)', pointers)
graphics = (ROOT / 'src/data/object_events/object_event_graphics.h').read_text()
files = {symbols[name]: Path(path).with_suffix('.png') for name, path in
         re.findall(r'(gObjectEventPic_\w+)\[\]\s*=\s*INCBIN_U16\("([^"]+)"', graphics) if name in symbols}
palettes = {}
address = symbols['sObjectEventSpritePalettes']
while True:
    pointer, tag = struct.unpack_from('<IH', rom, address)
    if pointer == 0:
        break
    palettes[tag] = struct.unpack_from('<16H', rom, pointer - 0x08000000)
    address += 8

seen = set()
slots = {}
errors = []
for gfx, symbol in entries:
    if symbol not in symbols:
        continue
    address = symbols[symbol]
    tag = struct.unpack_from('<H', rom, address + 2)[0]
    slots[gfx] = (tag, rom[address + 12] & 15)
    table = struct.unpack_from('<I', rom, address + 28)[0] - 0x08000000
    if not 0 <= table < len(rom):
        continue
    picture = struct.unpack_from('<I', rom, table)[0] - 0x08000000
    path = files.get(picture)
    if path is None or tag not in palettes or (path, tag) in seen or not (ROOT / path).exists():
        continue
    seen.add((path, tag))
    with Image.open(ROOT / path) as image:
        if image.mode != 'P':
            continue
        source = image.getpalette()
        target = [tuple((color >> (5*k)) & 31 for k in range(3)) for color in palettes[tag]]
        used = sorted(set(image.tobytes()) - {0})
        original_error = remapped_error = worst_error = moved = 0
        assigned = {0}
        for index in used:
            color = tuple(v >> 3 for v in source[index*3:index*3+3])
            distances = [sum((color[k]-candidate[k])**2 for k in range(3)) for candidate in target]
            best = min((i for i in range(1, 16) if i not in assigned), key=lambda i: distances[i])
            assigned.add(best)
            original_error += distances[index]
            remapped_error += distances[best]
            worst_error = max(worst_error, distances[best])
            moved += best != index
        # Allow small deliberate colour adjustments in shared palettes. Detect
        # permutations only when reordering is a substantially better match.
        if moved >= 6 and worst_error <= 17 and original_error > max(100, remapped_error * 4):
            errors.append(f'Scrambled index order: {gfx}: {path}')

# Standard NPC slots 2-5 are preloaded; a custom tag does not replace them.
# This catches the water Swimmer F / Electrician fixed-slot mismatch class.
standard_tags = struct.unpack_from('<11H', rom, symbols['sObjectPaletteTags0'])
for gfx, (tag, slot) in slots.items():
    if 2 <= slot <= 5 and tag != standard_tags[slot]:
        errors.append(f'Wrong standard palette slot: {gfx}, slot {slot}, tag {tag:#x}, expected {standard_tags[slot]:#x}')

watch = []
for path in sorted((ROOT / 'data/maps').glob('*/map.json')):
    data = json.loads(path.read_text())
    fixed = {}
    dynamic = set()
    for obj in data.get('object_events', []):
        if obj['graphics_id'] not in slots:
            continue
        tag, slot = slots[obj['graphics_id']]
        if slot == 15:
            dynamic.add(tag)
        elif slot >= 6:
            fixed.setdefault(slot, set()).add(tag)
    for slot, tags in fixed.items():
        if len(tags) > 1:
            errors.append(f'Fixed-slot collision: {path.parent.name}, slot {slot}, tags {sorted(tags)}')
    if len(dynamic) > 4:
        watch.append(f'{path.parent.name}: {len(dynamic)} across the entire map')
print(f'Checked {len(seen)} sprite/palette pairs and all map object lists.')
for entry in watch:
    print('Dynamic palette reuse required: ' + entry)
for error in errors:
    print(error)
if errors:
    raise SystemExit(1)
print('Passed: no detected index permutations or whole-map fixed-slot conflicts.')
