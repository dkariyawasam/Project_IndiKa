"""Reject numbered map exits pointing beyond the destination's warp table."""
from pathlib import Path
import json

root = Path(__file__).resolve().parents[2]
maps = {}
for path in (root / 'data/maps').glob('*/map.json'):
    data = json.loads(path.read_text())
    maps[data['id']] = data

checked = 0
errors = []
for data in maps.values():
    for index, warp in enumerate(data['warp_events']):
        destination = maps.get(warp['dest_map'])
        if destination is None:
            continue  # Dynamic/special map destinations have no fixed warp table.
        try:
            target = int(warp['dest_warp_id'], 0)
        except ValueError:
            continue
        if target < 0 or target >= 0x7f:
            continue  # Dynamic return-warp sentinel.
        checked += 1
        if target >= len(destination['warp_events']):
            errors.append(f"{data['name']} warp {index} -> {destination['name']} warp {target}")

assert not errors, '\n'.join(errors)
print(f'PASS: {checked} fixed warp destinations reference existing entries.')
