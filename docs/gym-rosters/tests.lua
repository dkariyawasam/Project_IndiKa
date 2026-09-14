dofile('/tmp/gym-rosters/cases.lua')
qa.index=1;qa.phase=0
qa.runner=callbacks:add('frame',function()
 local c=qa.cases[qa.index]
 if not c then callbacks:remove(qa.runner);qa.runner=nil;return end
 if qa.phase==3 then
  qa.battleFrames=qa.battleFrames+1
  local main=emu:read32(0x03003264)
  if main==0x08010d51 then qa.sawBattle=true end
  if qa.sawBattle and main==0x08056541 and emu:read8(0x03000f9c)==0 then
   qa.result(c.name..'-rematch-win',emu:read8(0x02023f4e)==1,'Opponent '..emu:read16(0x0203890e)..'; outcome '..emu:read8(0x02023f4e))
   qa.run({});qa.capture(c.name..'-complete');qa.index=qa.index+1;qa.phase=0
  elseif qa.battleFrames>18000 then qa.result(c.name..'-rematch-win',false,'Timeout');qa.capture(c.name..'-timeout');callbacks:remove(qa.runner);qa.runner=nil end
  return
 end
 if qa.queue[qa.step] then return end
 local sb=emu:read32(0x03005178)
 if qa.phase==0 then
  qa.setFlag(0x500+c.trainer,1)
  qa.script({0x25,0,0,0x39,c.g,c.m,255,c.x,0,c.y,0,0x27,0x02})
  qa.run({{kind='wait',frames=600}});qa.phase=1;qa.tries=0
 elseif qa.phase==1 then
  if qa.tries==0 then qa.result(c.name..'-present',qa.select(c.id));qa.capture(c.name..'-overworld') end
  emu:write16(sb+0x296,350);emu:write16(sb+0x646,100)
  qa.run({{kind='press',key=4,frames=30},{kind='wait',frames=600}});qa.phase=2
 else
  qa.tries=qa.tries+1
  if emu:read8(sb+0x648+c.id)>0 then
   qa.result(c.name..'-accepts-vs',true,'Within '..qa.tries..' activations')
   qa.select(c.id);local b={0x05};for _,x in ipairs(qa.u32(c.script)) do table.insert(b,x) end
   qa.script(b);qa.run({{kind='text',frames=18100}});qa.phase=3;qa.sawBattle=false;qa.battleFrames=0
  elseif qa.tries<12 then qa.run({{kind='text',frames=90,untilFree=true}});qa.phase=1
  else qa.result(c.name..'-accepts-vs',false,'No response');callbacks:remove(qa.runner);qa.runner=nil end
 end
end)
