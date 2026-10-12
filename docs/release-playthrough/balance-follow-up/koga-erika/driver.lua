
function qa.u32(n)return {n%256,math.floor(n/256)%256,math.floor(n/65536)%256,math.floor(n/16777216)%256}end
function qa.setFlag(id,on)
 local p=emu:read32(qa.sym.gSaveBlock1Ptr)+0xeec+math.floor(id/8);local mask=2^(id%8);local v=emu:read8(p);local old=math.floor(v/mask)%2;emu:write8(p,v+(on-old)*mask)
end
function qa.script(bytes)
 local addr=0x0203fc00
 for i,b in ipairs(bytes)do emu:write8(addr+i-1,b)end
 local c=qa.sym.sGlobalScriptContext
 for i=0,115 do emu:write8(c+i,0)end
 emu:write8(c+1,1);emu:write32(c+8,addr);emu:write32(c+92,qa.sym.gScriptCmdTable);emu:write32(c+96,qa.sym.gScriptCmdTableEnd);emu:write8(qa.sym.sGlobalScriptContextStatus,0)
end
qa.balanceText=callbacks:add('frame',function()
 local c=qa.queue[qa.step]
 if c and c.kind=='text' then emu:setKeys(qa.frame%30<6 and 1 or 0)end
end)
function qa.capture(n) emu:screenshot("/Users/deekariyawasam/pokefirered/docs/release-playthrough/balance-follow-up/koga-erika/"..n..".png") end
qa.cases={{name="koga-tier6-11",seed=11,badges=5,trainer=500,team={{75,41},{3,41},{18,40},{130,40},{58,40},{25,40}}},{name="erika-tier7-11",seed=11,badges=6,trainer=764,team={{18,46},{58,46},{3,45},{75,45},{130,45},{25,45}}}}
if qa.pilot then callbacks:remove(qa.pilot) end
function qa.effect(t,d)
 local mult=1;local a=emu:read8(d+33);local b=emu:read8(d+34)
 for k=0,363,3 do local p=qa.sym.gTypeEffectiveness+k;local at=emu:read8(p);local de=emu:read8(p+1);local m=emu:read8(p+2)
  if at==255 then break end
  if at==t and (de==a or de==b) then mult=mult*m/10 end
 end
 local ability=emu:read8(d+32)
 if (ability==26 and t==4)or(ability==10 and t==13)or(ability==11 and t==11)or(ability==18 and t==10)then return 0 end
 if ability==25 and mult<=1 then return 0 end
 return mult
end
function qa.bestMove(i)
 local a=qa.sym.gBattleMons+i*88;local best=-1;local chosen=0;local target=1
 for j=0,3 do
  local m=emu:read16(a+12+j*2);local p=qa.sym.gBattleMoves+m*12
  local power=emu:read8(p+1);local typ=emu:read8(p+2);local accuracy=emu:read8(p+3)
  if emu:read8(a+36+j)>0 then
   for _,k in ipairs({1,3}) do local d=qa.sym.gBattleMons+k*88
    if k<emu:read8(qa.sym.gBattlersCount) and emu:read16(d+40)>0 then
     local score=power*qa.effect(typ,d)*(accuracy==0 and 1 or accuracy/100)
     if typ==emu:read8(a+33) or typ==emu:read8(a+34) then score=score*1.5 end
     local physical=typ<9;score=score*emu:read16(a+(physical and 2 or 8))/math.max(1,emu:read16(d+(physical and 4 or 10)))
     score=score*(2*emu:read8(a+42)/5+2)/50+2
     if m==82 or m==49 or m==69 or m==101 then score=qa.effect(typ,d)==0 and 0 or (m==82 and 40 or (m==49 and 20 or emu:read8(a+42))) end
     if emu:read8(qa.sym.gBattlersCount)==4 and emu:read8(p+6)==32 then
      local ally=qa.sym.gBattleMons+(i==0 and 2 or 0)*88
      if emu:read16(ally+40)>0 then score=score*(qa.effect(typ,ally)==0 and 1 or 0.3)end
     end
     if power==0 then score=0.01 end
     if score>best then best=score;chosen=j;target=k end
    end
   end
  end
 end
 return chosen,target
end
qa.pilot=callbacks:add('frame',function()
 if not qa.testRunning then return end
 for _,i in ipairs({0,2}) do
  local fn=emu:read32(qa.sym.gBattlerControllerFuncs+i*4);local key=nil
  if fn==qa.sym.HandleInputChooseMove+1 then
   local wanted,target=qa.bestMove(i);qa.target=qa.target or {};qa.target[i]=target
   local cur=emu:read8(qa.sym.gMoveSelectionCursor+i)
   if cur%2~=wanted%2 then key=wanted%2==0 and 32 or 16 elseif math.floor(cur/2)~=math.floor(wanted/2) then key=wanted<2 and 64 or 128 else key=1 end
  elseif fn==qa.sym.HandleInputChooseTarget+1 then key=emu:read8(qa.sym.gMultiUsePlayerCursor)==(qa.target[i] or 1) and 1 or 16
  elseif fn==qa.sym.HandleInputChooseAction+1 then
   local cur=emu:read8(qa.sym.gActionSelectionCursor+i);key=cur==0 and 1 or (cur%2==1 and 32 or 64)
  end
  if key then emu:setKeys(qa.frame%20<5 and key or 0);return end
 end
end)

if qa.replaceCallback then callbacks:remove(qa.replaceCallback)end
qa.replaceCallback=callbacks:add('frame',function()
 local c=qa.queue[qa.step];if not c or (c.kind~='text' and c.kind~='walk' and c.kind~='map')then return end
 if emu:read32(qa.sym.gMain+4)~=qa.sym.CB2_UpdatePartyMenu+1 then return end
 local active={};for _,b in ipairs({0,2})do if b<emu:read8(qa.sym.gBattlersCount)then active[emu:read16(qa.sym.gBattlerPartyIndexes+b*2)]=true end end
 local target=nil;local best=0
 for i=0,5 do local hp=emu:read16(qa.sym.gPlayerParty+i*100+86);if hp>best and not active[(i%2==0 and math.floor(emu:read8(qa.sym.gBattlePartyCurrentOrder+math.floor(i/2))/16) or emu:read8(qa.sym.gBattlePartyCurrentOrder+math.floor(i/2))%16)]then target=i;best=hp end end
 if qa.partyTarget ~= nil then target=qa.partyTarget end
 if not target then return end
 local slot=emu:read8(qa.sym.gPartyMenu+9);local key=nil;local left=emu:read32(qa.sym.gBattleTypeFlags)%2==1 and 2 or 1
 for i=0,15 do local p=qa.sym.gTasks+i*40;if emu:read8(p+4)~=0 then local fn=emu:read32(p)
  if fn==qa.sym.Task_ReturnToChooseMonAfterText+1 then key=1
  elseif fn==qa.sym.Task_HandleSelectionMenuInput+1 then key=slot==target and 1 or 2
  elseif fn==qa.sym.Task_HandleChooseMonInput+1 then
   if slot==target then key=1
   elseif slot<left and target>=left then key=16
   elseif slot>=left and target<left then key=32
   else key=slot<target and 128 or 64 end
  end
 end end
 if key then emu:setKeys(qa.frame%30<6 and key or 0);qa.timer=0 end
end)

if qa.driver then callbacks:remove(qa.driver) end
function qa.dump(path,addr,size)
 local f=io.open('/Users/deekariyawasam/pokefirered/docs/release-playthrough/balance-follow-up/koga-erika/'..path,'wb');for i=0,size-1 do f:write(string.char(emu:read8(addr+i))) end;f:close()
end
function qa.beginCase(index)
 qa.caseIndex=index;qa.case=qa.cases[index];local c=qa.case;if not c then qa.run({});qa.testRunning=false;return end
 qa.testRunning=false;emu:write32(qa.sym.gRngValue,c.seed)
 for i=0,7 do qa.setFlag(0x831+i,i<c.badges and 1 or 0)end
 qa.setFlag(0x500+c.trainer,0)
 emu:write8(qa.sym.gPlayerPartyCount,0);for i=0,599 do emu:write8(qa.sym.gPlayerParty+i,0)end
 local bytes={0x69};for _,mon in ipairs(c.team)do
  local b={0x79,mon[1]%256,math.floor(mon[1]/256),mon[2],0,0,0,0,0,0,0,0,0,0,0};for _,v in ipairs(b)do table.insert(bytes,v)end
 end;table.insert(bytes,0x6b);table.insert(bytes,2);qa.script(bytes);qa.phase='prepare';qa.phaseFrame=qa.frame;qa.run({{kind='wait',frames=200}})
end
function qa.partyValid()
 if emu:read8(qa.sym.gPlayerPartyCount)~=#qa.case.team or emu:read8(qa.sym.sGlobalScriptContextStatus)~=2 then return false end
 local function xor(a,b) local v=0;local bit=1;for j=1,16 do if a%2~=b%2 then v=v+bit end;a=math.floor(a/2);b=math.floor(b/2);bit=bit*2 end;return v end
 for i,m in ipairs(qa.case.team)do
  local p=qa.sym.gPlayerParty+(i-1)*100;local sum=0
  for j=0,23 do local off=(j%2)*2;sum=sum+xor(xor(emu:read16(p+32+j*2),emu:read16(p+off)),emu:read16(p+4+off))end
  if sum%65536~=emu:read16(p+28) or emu:read8(p+84)~=m[2] then return false end
 end
 return true
end
function qa.startBattle()
 local c=qa.case;qa.dump(c.name..'-before.bin',qa.sym.gPlayerParty,600)
 local text=qa.sym.PewterCity_Gym_Text_BrockDefeat;local b={0x5c,4,c.trainer%256,math.floor(c.trainer/256),0,0}
 for i=1,3 do for _,v in ipairs(qa.u32(text))do table.insert(b,v)end end
 table.insert(b,0x6b);table.insert(b,2)
 emu:write8(qa.sym.gBattleOutcome,0);qa.script(b);qa.phase='battle';qa.phaseFrame=qa.frame;qa.testRunning=true;qa.seenBattle=false;qa.enemyDumped=false;qa.run({{kind='text',frames=180000}})
end
qa.driver=callbacks:add('frame',function()
 if qa.phase=='prepare' and qa.frame-qa.phaseFrame>200 and emu:read8(qa.sym.sLockFieldControls)==0 and qa.partyValid() then qa.startBattle()
 elseif qa.phase=='battle' then
  if emu:read32(qa.sym.gMain+4)==qa.sym.BattleMainCB2+1 then qa.seenBattle=true;if not qa.enemyDumped then qa.dump(qa.case.name..'-enemy.bin',qa.sym.gEnemyParty,600);qa.enemyDumped=true end end
  local out=emu:read8(qa.sym.gBattleOutcome)
  if qa.seenBattle and out~=0 then
   qa.testRunning=false;qa.phase='done';qa.phaseFrame=qa.frame
   qa.dump(qa.case.name..'-after.bin',qa.sym.gPlayerParty,600)
   local f=io.open('/Users/deekariyawasam/pokefirered/docs/release-playthrough/balance-follow-up/koga-erika/results.tsv','a');local alive=0;local hp=0;local max=0
   for i=0,#qa.case.team-1 do local p=qa.sym.gPlayerParty+i*100;local h=emu:read16(p+86);if h>0 then alive=alive+1 end;hp=hp+h;max=max+emu:read16(p+88)end
   f:write(string.format('%s\t%d\t%d\t%d\t%d\t%d\n',qa.case.name,out,emu:read8(qa.sym.gBattleResults+19),alive,hp,max));f:close()
   qa.run({{kind='text',frames=1200},{kind='wait',frames=120}})
  end
 elseif qa.phase=='done' and qa.frame-qa.phaseFrame>1400 and emu:read32(qa.sym.gMain+4)==qa.sym.CB2_Overworld+1 and emu:read8(qa.sym.sLockFieldControls)==0 then
  qa.phase='paused';qa.run({});qa.capture(qa.case.name..'-result');if qa.auto then qa.beginCase(qa.caseIndex+1) end
 end
end)

qa.auto=true;qa.beginCase(1)
