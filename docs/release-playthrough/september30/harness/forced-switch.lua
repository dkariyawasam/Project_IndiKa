if qa.switchCb then callbacks:remove(qa.switchCb) end
qa.partyTarget=nil
qa.switchCb=callbacks:add('frame',function()
 local party=emu:read32(qa.sym.gMain+4)==0x811bc8d
 if not party then qa.partyTarget=nil;return end
 if not qa.autoBattle or not qa.inBattle or emu:read8(0x203c174+11)>1 then return end
 if not qa.partyTarget then
  local best=-1
  for i=0,emu:read8(qa.sym.gPlayerPartyCount)-1 do
   local p=qa.sym.gPlayerParty+i*100
   local order=emu:read8(0x203c1b0+math.floor(i/2))
   local canonical=i%2==0 and math.floor(order/16) or order%16
   local doubles=emu:read32(qa.sym.gBattleTypeFlags)%2==1
   if emu:read16(p+86)>0 and canonical~=emu:read16(0x2023c92) and (not doubles or canonical~=emu:read16(0x2023c96)) then
    local score=emu:read8(p+84)*100+emu:read16(p+86)
    if qa.switchPreference==i then score=score+10000 end
    if score>best then qa.partyTarget=i;best=score end
   end
  end
 end
 if qa.partyTarget then
  local c=emu:read8(0x203c174+9)
  local key=c==qa.partyTarget and 1 or (c<qa.partyTarget and 128 or 64)
  emu:setKeys(qa.frame%45<8 and key or 0)
 end
end)
