import subprocess,re,json
from pathlib import Path
root=Path('/Users/deekariyawasam/pokefirered');out=Path('/tmp/kanto-release-playtest')
syms={p[2]:int(p[0],16) for l in subprocess.check_output(['/opt/devkitpro/devkitARM/bin/arm-none-eabi-nm',str(root/'pokefirered.elf')],text=True).splitlines() if len(p:=l.split())==3}
keys=['gMain','gSaveBlock1Ptr','gPlayerParty','gPlayerPartyCount','gStringVar4','sLockFieldControls','CB2_Overworld','gBattleTypeFlags','gBattleOutcome']
s='qa={frame=0,queue={},step=1,timer=0,lastText="", sym={'+','.join(k+'='+str(syms[k]) for k in keys)+'}}\n'
s+='qa.chars={}\n'
for l in (root/'charmap.txt').read_text().splitlines():
 m=re.match(r"'(.*?)'\s*=\s*([0-9A-F]{2})$",l)
 if m:s+=f'qa.chars[{int(m[2],16)}]={json.dumps(m[1],ensure_ascii=False)}\n'
s+=r'''
function qa.text()
 local t={};for i=0,999 do local b=emu:read8(qa.sym.gStringVar4+i);if b==255 then break end;table.insert(t,qa.chars[b] or (b>=250 and "|" or "?")) end
 return table.concat(t)
end
function qa.state()
 local p=emu:read32(qa.sym.gSaveBlock1Ptr)
 if p<0x02000000 or p>0x02040000 then return 0,0,0,0 end
 return emu:read8(p+4),emu:read8(p+5),emu:read16(p),emu:read16(p+2)
end
function qa.snapshot()
 local g,m,x,y=qa.state();local f=io.open('/tmp/kanto-release-playtest/status.txt','w')
 f:write(string.format('frame=%d map=%d,%d xy=%d,%d step=%d/%d timer=%d main=%x lock=%d party=%d\n',qa.frame,g,m,x,y,qa.step,#qa.queue,qa.timer,emu:read32(qa.sym.gMain+4),emu:read8(qa.sym.sLockFieldControls),emu:read8(qa.sym.gPlayerPartyCount)))
 f:write(qa.text());f:close();emu:screenshot('/tmp/kanto-release-playtest/current.png')
end
function qa.run(q) qa.queue=q;qa.step=1;qa.timer=0 end
qa.cb=callbacks:add('frame',function()
 qa.frame=qa.frame+1
 local c=qa.queue[qa.step]
 if c then
  qa.timer=qa.timer+1
  emu:setKeys((not c.pulse or qa.timer%c.pulse<6) and (c.key or 0) or 0)
  if qa.timer>=c.frames then emu:setKeys(0);qa.step=qa.step+1;qa.timer=0;qa.snapshot() end
 end
 if qa.frame%60==0 then
  local t=qa.text();if t~=qa.lastText then local f=io.open('/tmp/kanto-release-playtest/dialogue.txt','a');f:write(t..'\n');f:close();qa.lastText=t end
  local f=io.open('/tmp/kanto-release-playtest/command.lua','r')
  if f then local s=f:read('*a');f:close();if #s>0 then f=io.open('/tmp/kanto-release-playtest/command.lua','w');f:close();local fn,err=load(s);if fn then local ok,e=pcall(fn);if not ok then console:error(e) end else console:error(err) end end end
  qa.snapshot()
 end
end)
qa.snapshot()
'''
(out/'boot.lua').write_text(s);(out/'command.lua').write_text('')
