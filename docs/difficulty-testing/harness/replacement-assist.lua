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
