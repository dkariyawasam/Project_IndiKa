import re,time,sys
from pathlib import Path
base=Path('/tmp/kanto-release-september22')
for attempt in range(100):
 before=base.joinpath('status.txt').read_text();m=re.search(r'frame=(\d+)',before)
 if m:break
 time.sleep(.01)
frame=int(m[1]);cmd=sys.stdin.read();base.joinpath('command.lua').write_text(cmd);start=time.time()
while time.time()-start<40:
 time.sleep(.1)
 try:
  s=base.joinpath('status.txt').read_text();m=re.search(r'frame=(\d+).*step=(\d+)/(\d+)',s)
  if not base.joinpath('command.lua').read_text() and m and int(m[1])>frame+60 and int(m[2])>int(m[3]):
   print(s[:250]);break
 except (OSError,ValueError):pass
else:print('Still running:',s[:200])
