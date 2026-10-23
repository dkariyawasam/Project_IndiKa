#!/usr/bin/env python3
"""Audit assembly dialogue lines with the game's normal-font pixel widths.
Reports known literal overflow and placeholder-dependent lines separately.
Runtime wrapping handles expanded field/battle dialogue, so source lines need
not be destructively reflowed. This is not an audit of fixed-layout menus.
"""
import json,re
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def audit():
 text=(ROOT/'src/text.c').read_text()
 widths=list(map(int,re.findall(r'\d+',re.search(r'sFontNormalLatinGlyphWidths\[\]\s*=\s*\{(.*?)\};',text,re.S)[1])))
 chars={c:int(n,16) for c,n in re.findall(r"^'(.*)'\s*=\s*([0-9A-F]{2})\s*$",(ROOT/'charmap.txt').read_text(),re.M)}
 seen=set()
 def include(path):
  if path in seen or not path.exists():return
  seen.add(path)
  for name in re.findall(r'\.include\s+"([^"]+)"',path.read_text()):include(ROOT/name)
 include(ROOT/'data/event_scripts.s')
 overflow=[];dynamic=[];count=0
 for p in sorted(seen):
  label=''
  for line,raw in enumerate(p.read_text().splitlines(),1):
   match=re.match(r'^(\w+)::?',raw)
   if match:label=match[1]
   match=re.search(r'\.string\s+"(.*)"',raw)
   if not match:continue
   for fragment in re.split(r'\\[nlp]|\$',match[1]):
    if not fragment:continue
    count+=1
    record=dict(file=str(p.relative_to(ROOT)),line=line,label=label,text=fragment)
    if '{' in fragment or '\\' in fragment or any(c not in chars for c in fragment):
     dynamic.append(record);continue
    width=sum(widths[chars[c]] for c in fragment)
    if width>200:overflow.append(dict(record,pixels=width))
 return dict(lines=count,literal_overflow=overflow,dynamic_or_encoded=dynamic,
  note='200 px limit for standard 26-tile dialogue windows, reserving 8 px for prompt; live window dimensions govern runtime wrapping.')
if __name__=='__main__':
 report=audit();out=ROOT/'.cache/debug-dashboard/reviews/dialogue-widths';out.mkdir(parents=True,exist_ok=True)
 (out/'audit.json').write_text(json.dumps(report,indent=2)+'\n')
 print(f"{report['lines']} lines checked; {len(report['literal_overflow'])} literal overflow candidates; {len(report['dynamic_or_encoded'])} dynamic/encoded lines requiring runtime measurement.")
