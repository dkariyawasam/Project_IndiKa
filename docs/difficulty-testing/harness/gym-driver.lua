if qa.driver then callbacks:remove(qa.driver) end
function qa.dump(path,addr,size)
 local f=io.open('/Users/deekariyawasam/pokefirered/docs/difficulty-testing/'..path,'wb');for i=0,size-1 do f:write(string.char(emu:read8(addr+i))) end;f:close()
end
function qa.beginCase(index)
 qa.caseIndex=index;qa.case=qa.cases[index];local c=qa.case;if not c then qa.run({});qa.testRunning=false;return end
 qa.testRunning=false;emu:write32(qa.sym.gRngValue,c.seed)
 for i=0,7 do qa.setFlag(0x831+i,i<c.badges and 1 or 0)end
 qa.setFlag(0x500+c.trainer,0)
 emu:write8(qa.sym.gPlayerPartyCount,0);for i=0,599 do emu:write8(qa.sym.gPlayerParty+i,0)end
 local bytes={};for _,mon in ipairs(c.team)do
  local b={0x79,mon[1]%256,math.floor(mon[1]/256),mon[2],0,0,0,0,0,0,0,0,0,0,0};for _,v in ipairs(b)do table.insert(bytes,v)end
 end;table.insert(bytes,0x6b);table.insert(bytes,2);qa.script(bytes);qa.phase='prepare';qa.phaseFrame=qa.frame;qa.run({{kind='wait',frames=200}})
end
function qa.startBattle()
 local c=qa.case;qa.dump(c.name..'-before.bin',qa.sym.gPlayerParty,600)
 local text=qa.sym.PewterCity_Gym_Text_BrockDefeat;local b={0x5c,4,c.trainer%256,math.floor(c.trainer/256),0,0}
 for i=1,3 do for _,v in ipairs(qa.u32(text))do table.insert(b,v)end end
 table.insert(b,0x6b);table.insert(b,2)
 emu:write8(qa.sym.gBattleOutcome,0);qa.script(b);qa.phase='battle';qa.phaseFrame=qa.frame;qa.testRunning=true;qa.seenBattle=false;qa.enemyDumped=false;qa.run({{kind='text',frames=180000}})
end
qa.driver=callbacks:add('frame',function()
 if qa.phase=='prepare' and qa.frame-qa.phaseFrame>200 then qa.startBattle()
 elseif qa.phase=='battle' then
  if emu:read32(qa.sym.gMain+4)==qa.sym.BattleMainCB2+1 then qa.seenBattle=true;if not qa.enemyDumped then qa.dump(qa.case.name..'-enemy.bin',qa.sym.gEnemyParty,600);qa.enemyDumped=true end end
  local out=emu:read8(qa.sym.gBattleOutcome)
  if qa.seenBattle and out~=0 then
   qa.testRunning=false;qa.phase='done';qa.phaseFrame=qa.frame
   qa.dump(qa.case.name..'-after.bin',qa.sym.gPlayerParty,600)
   local f=io.open('/Users/deekariyawasam/pokefirered/docs/difficulty-testing/results.tsv','a');local alive=0;local hp=0;local max=0
   for i=0,#qa.case.team-1 do local p=qa.sym.gPlayerParty+i*100;local h=emu:read16(p+86);if h>0 then alive=alive+1 end;hp=hp+h;max=max+emu:read16(p+88)end
   f:write(string.format('%s\t%d\t%d\t%d\t%d\t%d\n',qa.case.name,out,emu:read8(qa.sym.gBattleResults+19),alive,hp,max));f:close()
   qa.run({{kind='text',frames=1200},{kind='wait',frames=120}})
  end
 elseif qa.phase=='done' and qa.frame-qa.phaseFrame>1400 then
  qa.phase='paused';qa.run({});qa.capture(qa.case.name..'-result');if qa.auto then qa.beginCase(qa.caseIndex+1) end
 end
end)
