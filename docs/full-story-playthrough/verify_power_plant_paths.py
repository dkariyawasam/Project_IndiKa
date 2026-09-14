"""Keep Power Plant story targets connected despite trainer placement."""
from collections import deque
import json
from pathlib import Path
import struct

root = Path(__file__).resolve().parents[2]
map_data = json.loads((root / 'data/maps/PowerPlant/map.json').read_text())
layouts = json.loads((root / 'data/layouts/layouts.json').read_text())['layouts']
layout = next(entry for entry in layouts if entry['id'] == map_data['layout'])
width, height = layout['width'], layout['height']
blocks = struct.unpack('<' + 'H' * (width * height),
                       (root / layout['blockdata_filepath']).read_bytes())
occupied = {(obj['x'], obj['y']) for obj in map_data['object_events']}
entrance = map_data['warp_events'][1]
start = (entrance['x'], entrance['y'])
reached = {start}
queue = deque([start])
while queue:
    x, y = queue.popleft()
    for dx, dy in ((0, 1), (0, -1), (1, 0), (-1, 0)):
        neighbor = x + dx, y + dy
        nx, ny = neighbor
        if not (0 <= nx < width and 0 <= ny < height):
            continue
        if blocks[ny * width + nx] >> 10 & 3:
            continue
        if neighbor in occupied or neighbor in reached:
            continue
        reached.add(neighbor)
        queue.append(neighbor)

for script in ('EventScript_GymAdjacent_ENGINEER_BAILY',
               'EventScript_GymAdjacent_ENGINEER_BRAXTON',
               'PowerPlant_EventScript_Zapdos'):
    obj = next(obj for obj in map_data['object_events'] if obj['script'] == script)
    assert any((obj['x'] + dx, obj['y'] + dy) in reached
               for dx, dy in ((0, 1), (0, -1), (1, 0), (-1, 0))), script
print('PASS: both Zapdos witnesses and the encounter have reachable adjacent tiles with all trainers present.')
