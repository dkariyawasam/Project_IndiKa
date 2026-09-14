"""Verify the data contract used by HandleBoulderFallThroughHole."""
from pathlib import Path
import json,re
root=Path(__file__).resolve().parents[2]
flags={k:int(v,16) for k,v in re.findall(r'#define (FLAG_\w+)\s+(0x[0-9A-Fa-f]+)',(root/'include/constants/flags.h').read_text())}
import subprocess,sys
subprocess.run([sys.executable,str(root/'docs/seafoam-simplification/verify.py')],check=True)

v3=json.loads((root/'data/maps/VictoryRoad_3F/map.json').read_text())
v2=json.loads((root/'data/maps/VictoryRoad_2F/map.json').read_text())
falling=next(o for o in v3['object_events'] if o['flag']=='FLAG_HIDE_VICTORY_ROAD_3F_BOULDER')
assert falling['trainer_type']=='FLAG_HIDE_VICTORY_ROAD_2F_BOULDER'
assert any(o['flag']==falling['trainer_type'] for o in v2['object_events'])
print('PASS: Victory Road falling boulder reveals the lower-floor switch boulder.')
