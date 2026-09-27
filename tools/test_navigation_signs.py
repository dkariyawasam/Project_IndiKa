"""Check navigation post wiring, approach space and story-gated cave labels."""
import json
import re
import struct
import sys
from pathlib import Path
ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools/debug_dashboard'))
from catalog import build_catalog
from atlas import arrange
layouts = {v['id']: v for v in json.loads((ROOT / 'data/layouts/layouts.json').read_text())['layouts']}
checked = 0
for path in (ROOT / 'data/maps').glob('*/map.json'):
    data = json.loads(path.read_text())
    layout = layouts[data['layout']]
    raw = (ROOT / layout['blockdata_filepath']).read_bytes()
    tiles = struct.unpack('<' + 'H' * (len(raw) // 2), raw)
    width, height = layout['width'], layout['height']
    for event in data['bg_events']:
        label = event.get('script', '')
        if '_EventScript_Navigation' not in label:
            continue
        x, y = event['x'], event['y']
        assert 0 <= x < width and 0 <= y < height - 1, (path, event)
        assert tiles[y * width + x] & 0xC00, (path, 'post not solid')
        assert not tiles[(y + 1) * width + x] & 0xC00, (path, 'approach blocked')
        assert not any(o['x'] == x and o['y'] == y for o in data['object_events'] + data['warp_events']), path
        script = (path.parent / 'scripts.inc').read_text()
        text = (path.parent / 'text.inc').read_text()
        match = re.search(r'^' + label + r'::\n(.*?)(?=^\w+::|\Z)', script, re.M | re.S)
        assert match, (path, label)
        for target in re.findall(r'msgbox (\w+)', match[1]):
            assert target + '::' in text, (path, target)
        if label.endswith('CaveApproach'):
            assert 'goto_if_unset FLAG_GIOVANNI_ACTIVATED_MEWTWO_SIGNAL' in match[1], path
            before = re.search(r'^' + label + r'BeforeBlast::\n(.*?)(?=^\w+::|\Z)', script, re.M | re.S)
            target = re.search(r'msgbox (\w+)', before[1])[1]
            body = re.search(r'^' + target + r'::\n(.*?)(?=^\w+::|\Z)', text, re.M | re.S)[1]
            assert 'CELADON CAVE' not in body, path
        checked += 1
assert checked > 0
world = arrange(build_catalog()['maps'])
assert not world['conflicts'], world['conflicts']
assert not world['overlaps'], world['overlaps']
print(f'{checked} navigation signs passed; no world connection conflicts or overlaps.')
