dofile('/tmp/gym-story-qa/followups.lua')
qa.followIndex=1;qa.followPhase=0
qa.followRunner=callbacks:add('frame',function()
 if qa.queue[qa.step] then return end
 local c=qa.followups[qa.followIndex]
 if not c then callbacks:remove(qa.followRunner);qa.followRunner=nil;qa.capture('followups-complete');return end
 if qa.followPhase==0 then
  if c.var then qa.setVar(c.var,c.ready) end;qa.setFlag(c.flag,0)
  for i=0,599 do emu:write8(0x02024348+i,0) end;emu:write8(0x020240ed,0)
  local b={}
  if c.species~=0 then b={0x79,c.species%256,math.floor(c.species/256),30,0,0,0,0,0,0,0,0,0,0,0} end
  for _,x in ipairs({0x39,c.g,c.m,255,c.x,0,c.y,0,0x27,0x02}) do table.insert(b,x) end
  qa.script(b);qa.run({{kind='wait',frames=600}});qa.followPhase=1
 elseif qa.followPhase==1 then
  qa.select(c.id);local b={0x05};for _,x in ipairs(qa.u32(c.script)) do table.insert(b,x) end
  qa.script(b);qa.run({{kind='text',frames=12000},{kind='capture',name=c.name}});qa.followPhase=2
 elseif qa.followPhase==2 then
  local ok=qa.flag(c.flag)==1 and emu:read8(0x03000f9c)==0
  qa.result(c.name,ok,'Reward/trade flag '..qa.flag(c.flag)..'; lock '..emu:read8(0x03000f9c)..'; trial '..(c.var and qa.var(c.var) or -1))
  if not ok then callbacks:remove(qa.followRunner);qa.followRunner=nil;return end
  qa.script({0x16,4,128,0,0,0x26,13,128,96,1,0x02});qa.run({{kind="wait",frames=60}});qa.followPhase=3
 else
  local expected=({76,186,427,424,42,137})[qa.followIndex]
  local actual=emu:read16(0x020371e8)
  qa.result(c.name.."-species",actual==expected,"Expected "..expected.."; received "..actual)
  qa.followIndex=qa.followIndex+1;qa.followPhase=0
 end
end)
qa.followInput=callbacks:add('frame',function()
 local c=qa.followups[qa.followIndex];local cmd=qa.queue[qa.step]
 if not qa.followRunner or not c or not cmd or cmd.kind~='text' then return end
 local msg=(qa.lastMessage or ''):lower()
 if msg:find('nickname') or msg:find('random noise') then emu:setKeys(qa.timer%30<6 and 2 or 0) end
 if qa.timer>30 and qa.flag(c.flag)==1 and emu:read8(0x03000f9c)==0 and emu:read32(0x03003264)==0x08056541 then cmd.frames=qa.timer+1 end
end)
