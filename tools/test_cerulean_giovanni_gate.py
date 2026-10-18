#!/usr/bin/env python3
"""Check cave entry points cannot start effects before the Route 7 scene."""
from pathlib import Path
from itertools import product
import re
s=(Path(__file__).resolve().parents[1]/'data/maps/CeruleanCave_B1F/scripts.inc').read_text()
blocks=dict(re.findall(r'^(\w+)::\n(.*?)(?=^\w+::|\Z)',s,re.M|re.S))
prefix='CeruleanCave_B1F_EventScript_'
prerequisite='FLAG_SAW_GIOVANNI_ROUTE7_RAMPAGE'
def reaches_lock(entry,flags):
    for line in blocks[prefix+entry].strip().splitlines():
        cmd,_,args=line.strip().partition(' ')
        parts=args.split(', ')
        if cmd=='goto_if_unset' and parts[0] not in flags:return False
        if cmd=='goto_if_set' and parts[0] in flags:return False
        if cmd=='lock':return True
    return False
for progressed,moved,fought,finished in product((False,True),repeat=4):
    flags={name for name,on in zip((prerequisite,'FLAG_GIOVANNI_CERULEAN_MOVED_ASIDE','FLAG_FOUGHT_MEWTWO','FLAG_SAW_GIOVANNI_CERULEAN_CAVE'),(progressed,moved,fought,finished)) if on}
    assert reaches_lock('GiovanniApproachTrigger',flags)==(progressed and not (moved or fought or finished))
    assert reaches_lock('Giovanni',flags)==(progressed and not finished)
    assert reaches_lock('Mewtwo',flags)==progressed
for entry in ('GiovanniApproachTrigger','Giovanni','Mewtwo'):
    assert 'goto_if_unset '+prerequisite in blocks[prefix+entry].split('\tlock')[0]
assert blocks[prefix+'GiovanniApproachTriggerEnd'].strip()=='end'
print('PASS: all 16 story-state combinations; early visits cannot lock, speak, move Giovanni or start Mewtwo')
