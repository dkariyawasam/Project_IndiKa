dofile('/tmp/gym-story-qa/cases.lua')
qa.battleIndex=1;qa.battlePhase=0
qa.battleRunner=callbacks:add('frame',function()
 local c=qa.cases[qa.battleIndex]
 if not c then callbacks:remove(qa.battleRunner);qa.battleRunner=nil;qa.capture('eight-gym-battles-complete');return end
 if qa.battlePhase==2 then
  qa.battleTimer=qa.battleTimer+1
  if emu:read32(0x03003264)==0x08010d51 then
   qa.sawBattle=true;qa.inBattleFrames=qa.inBattleFrames+1
   if qa.inBattleFrames==600 then qa.capture(c.name..'-battle') end
  end
  if qa.flag(c.badge)==1 and emu:read8(0x03000f9c)==0 and emu:read32(0x03003264)==0x08056541 then
   qa.run({{kind='wait',frames=60},{kind='capture',name=c.name..'-badge'}});qa.battlePhase=3
  elseif qa.battleTimer>18000 then
   qa.capture(c.name..'-needs-inspection');qa.result(c.name..'-battle',false,'Timed out; no next fixture started');qa.run({});callbacks:remove(qa.battleRunner);qa.battleRunner=nil
  end
  return
 end
 if qa.queue[qa.step] then return end
 if qa.battlePhase==0 then
  if c.var then qa.setVar(c.var,c.ready) end
  if c.hide~=0 then qa.setFlag(c.hide,0) end
  qa.setFlag(c.badge,0);qa.setFlag(c.defeat,0)
  if c.name=='Giovanni' then qa.setVar(0x405a,1) end
  local b={0x25,0,0,0x39,c.g,c.m,255,c.x,0,c.y,0,0x27,0x02}
  qa.script(b);qa.run({{kind='wait',frames=600}});qa.battlePhase=1
 elseif qa.battlePhase==1 then
  qa.result(c.name..'-leader-present',qa.select(c.id))
  local b={0x05};for _,x in ipairs(qa.u32(c.script)) do table.insert(b,x) end
  qa.script(b);qa.run({{kind='text',frames=18100}});qa.battleTimer=0;qa.inBattleFrames=0;qa.sawBattle=false;qa.battlePhase=2
 elseif qa.battlePhase==3 then
  qa.result(c.name..'-battle-and-badge',qa.sawBattle and qa.flag(c.badge)==1 and qa.flag(c.defeat)==1,'Battle outcome '..emu:read8(0x02023f4e)..'; trial state '..(c.var and qa.var(c.var) or -1))
  if c.item then
   qa.script({0x47,c.item%256,math.floor(c.item/256),1,0,0x6c,0x02});qa.run({{kind='wait',frames=30}});qa.battlePhase=4
  else qa.battleIndex=qa.battleIndex+1;qa.battlePhase=0 end
 else
  qa.result(c.name..'-badge-item',emu:read16(0x020371e8)==1,'Item '..c.item)
  qa.battleIndex=qa.battleIndex+1;qa.battlePhase=0
 end
end)
