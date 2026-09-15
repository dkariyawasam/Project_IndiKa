if qa and qa.callback then callbacks:remove(qa.callback) end
if qa and qa.poll then callbacks:remove(qa.poll) end
qa={queue={},step=1,timer=0,frame=0}
function qa.state()
 local s=emu:read32(0x03005178)
 return emu:read8(s+4),emu:read8(s+5),emu:read16(s),emu:read16(s+2)
end
function qa.run(commands)
 qa.queue=commands;qa.step=1;qa.timer=0
end
function qa.capture(name)
 local base='/Users/deekariyawasam/pokefirered/docs/palette-recovery/'..name
 emu:screenshot(base..'.png');emu:saveStateFile('/tmp/palette-recovery/state-'..name..'.ss')
 local g,m,x,y=qa.state()
 console:log(string.format('CAPTURE %s map=%d,%d pos=%d,%d',name,g,m,x,y))
end
qa.callback=callbacks:add('frame',function()
 qa.frame=qa.frame+1
 local c=qa.queue[qa.step]
 if not c then return end
 qa.timer=qa.timer+1
 local done=false
 local g,m,x,y=qa.state()
 if c.kind=='walk' then
  if x==c.x and y==c.y then emu:setKeys(0);qa.arrival=(qa.arrival or 0)+1;done=qa.arrival>=20
  elseif x~=c.x and not c.vertical then emu:setKeys(x<c.x and 16 or 32)
  elseif y~=c.y then emu:setKeys(y<c.y and 128 or 64)
  else emu:setKeys(x<c.x and 16 or 32) end
 elseif c.kind=='map' then
  if g==c.group and m==c.map then done=true else emu:setKeys(c.key or 0) end
 elseif c.kind=='press' then
  if qa.timer<= (c.hold or 6) then emu:setKeys(c.key) else emu:setKeys(0) end
  done=qa.timer>=(c.frames or 30)
 elseif c.kind=='wait' then emu:setKeys(0);done=qa.timer>=c.frames or (c.untilFree and qa.timer>30 and emu:read8(0x03000f9c)==0)
 elseif c.kind=='capture' then qa.capture(c.name);done=true
 elseif c.kind=='text' then
  emu:setKeys(qa.timer%30<6 and 1 or 0);done=qa.timer>=c.frames or (c.untilFree and qa.timer>30 and emu:read8(0x03000f9c)==0)
 end
 if c.untilName then
  for i=0,15 do if emu:read8(0x03005204+i*40)~=0 and emu:read32(0x03005200+i*40)==0x0812c4c5 then done=true end end
 end
 if c.untilField and emu:read32(0x03003264)==0x8056535 then done=true end
 if done then
  emu:setKeys(0);qa.step=qa.step+1;qa.timer=0;qa.arrival=0
  if not qa.queue[qa.step] then console:log('QA queue complete') end
 elseif qa.timer>(c.timeout or math.max(c.frames or 0,1200)+120) then
  emu:setKeys(0);qa.capture('stalled-'..qa.step)
  console:error(string.format('QA stalled at %d (%s)',qa.step,c.kind));qa.queue={}
 end
end)
qa.poll=callbacks:add('frame',function()
 if qa.frame%30==0 then
  local ok,err=pcall(dofile,'/tmp/palette-recovery/commands.lua')
  if not ok then console:error(err) end
  local g,m,x,y=qa.state();local f=io.open('/tmp/palette-recovery/status.json','w')
  f:write(string.format('{"group":%d,"map":%d,"x":%d,"y":%d,"step":%d,"timer":%d,"kind":"%s","main":%d}',g,m,x,y,qa.step,qa.timer,qa.queue[qa.step] and qa.queue[qa.step].kind or 'idle',emu:read32(0x03003264)));f:close()
 end
end)

