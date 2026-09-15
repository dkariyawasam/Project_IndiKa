if qa.driver then callbacks:remove(qa.driver) end
function qa.dump(path,addr,size)
 local f=io.open('/Users/deekariyawasam/pokefirered/docs/champion-movesets/'..path,'wb');for i=0,size-1 do f:write(string.char(emu:read8(addr+i))) end;f:close()
end
function qa.beginCase(index)
 qa.caseIndex=index;qa.case=qa.cases[index];local c=qa.case;if not c then qa.run({});qa.testRunning=false;return end
 qa.testRunning=false;emu:write8(emu:read32(qa.sym.gSaveBlock2Ptr)+2836,0);emu:write32(qa.sym.gRngValue,c.seed)
 for i=0,7 do qa.setFlag(0x831+i,i<c.badges and 1 or 0)end
 if c.trainer then qa.setFlag(0x500+c.trainer,0) end
 emu:write8(qa.sym.gPlayerPartyCount,0);for i=0,599 do emu:write8(qa.sym.gPlayerParty+i,0)end
 local bytes={};for _,mon in ipairs(c.team)do
  local b={0x79,mon.id%256,math.floor(mon.id/256),mon.level,0,0,0,0,0,0,0,0,0,0,0};for _,v in ipairs(b)do table.insert(bytes,v)end
 end;table.insert(bytes,0x6b);table.insert(bytes,2);qa.script(bytes);qa.phase='prepare';qa.phaseFrame=qa.frame;qa.run({{kind='wait',frames=200}})
end
function qa.startBattle()
 local c=qa.case
 if c.mode~='baseline' then
  local baseline=c.baselineName or c.name:gsub('%-improved%-','-baseline-'):gsub('%-refined%-','-baseline-');local f=assert(io.open('/Users/deekariyawasam/pokefirered/docs/champion-movesets/'..baseline..'-before.bin','rb'));local data=f:read('*a');f:close();if c.order then local parts={};for _,slot in ipairs(c.order)do parts[#parts+1]=data:sub(slot*100+1,slot*100+100)end;data=table.concat(parts)end;for i=1,#data do emu:write8(qa.sym.gPlayerParty+i-1,data:byte(i))end
 end
 emu:write32(qa.sym.gRngValue,c.seed)
 for i,m in ipairs(c.team)do if m.moves then qa.setMoves(i-1,m.moves)end end;qa.dump(c.name..'-before.bin',qa.sym.gPlayerParty,600)
 local b={}
 local function append(t)for _,v in ipairs(t)do table.insert(b,v)end end
 local function special(n)append({0x25,n%256,math.floor(n/256)})end
 if c.apex then
  append({0x16,4,128,c.apex%256,math.floor(c.apex/256),0x16,5,128,c.enemyLevel,0,0x16,6,128,0,0})
  special(qa.sym.SPECIAL_CreateEnemyEventMon);qa.setFlag(0x818,1);special(qa.sym.SPECIAL_StartApexBattle);append({0x27})
 else
  if c.league==1 then special(qa.sym.SPECIAL_StartIndigoLeagueChallenge) end
  if c.league==2 then special(qa.sym.SPECIAL_StartRocketLeagueChallenge) end
  local text=qa.sym.PewterCity_Gym_Text_BrockDefeat
  local mode=c.league==2 and 0 or 4
  append({0x5c,mode,c.trainer%256,math.floor(c.trainer/256),0,0})
  for i=1,(mode==4 and 3 or 2)do append(qa.u32(text))end
 end
 append({0x6b,2})
 emu:write8(qa.sym.gBattleOutcome,0);qa.script(b);qa.phase='battle';qa.phaseFrame=qa.frame;qa.testRunning=true;qa.seenBattle=false;qa.enemyDumped=false;qa.run({{kind='text',frames=180000}})
end
qa.driver=callbacks:add('frame',function()
 if qa.phase=='prepare' and qa.frame-qa.phaseFrame>200 then qa.warp(1,72,4,10);qa.phase='warping';qa.phaseFrame=qa.frame;qa.run({{kind='wait',frames=240}})
 elseif qa.phase=='warping' and qa.frame-qa.phaseFrame>240 then qa.startBattle()
 elseif qa.phase=='battle' then
  if emu:read32(qa.sym.gMain+4)==qa.sym.BattleMainCB2+1 then qa.seenBattle=true;if not qa.enemyDumped then qa.dump(qa.case.name..'-enemy.bin',qa.sym.gEnemyParty,600);qa.enemyDumped=true end end
  local out=emu:read8(qa.sym.gBattleOutcome)
  if qa.seenBattle and out~=0 then
   qa.testRunning=false;qa.phase='done';qa.phaseFrame=qa.frame
   qa.dump(qa.case.name..'-after.bin',qa.sym.gPlayerParty,600)
   local f=io.open('/Users/deekariyawasam/pokefirered/docs/champion-movesets/results.tsv','a');local alive=0;local hp=0;local max=0
   for i=0,#qa.case.team-1 do local p=qa.sym.gPlayerParty+i*100;local h=emu:read16(p+86);if h>0 then alive=alive+1 end;hp=hp+h;max=max+emu:read16(p+88)end
   f:write(string.format('%s\t%d\t%d\t%d\t%d\t%d\n',qa.case.name,out,emu:read8(qa.sym.gBattleResults+19),alive,hp,max));f:close()
   qa.run({{kind='text',frames=1200},{kind='wait',frames=120}})
  end
 elseif qa.phase=='done' and qa.frame-qa.phaseFrame>1400 then
  qa.phase='paused';qa.run({});qa.capture(qa.case.name..'-result');if qa.auto then qa.beginCase(qa.caseIndex+1) end
 end
end)
