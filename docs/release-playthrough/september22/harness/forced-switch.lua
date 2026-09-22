if qa.switchCb then callbacks:remove(qa.switchCb) end
qa.partyTarget=nil
qa.switchCb=callbacks:add('frame',function()
 local party=emu:read32(qa.sym.gMain+4)==0x811afbd
 if not party then qa.partyTarget=nil;return end
 if not qa.autoBattle or not qa.inBattle or emu:read8(0x203b630+11)>1 then return end
 if not qa.partyTarget then
  local best=-1
  for i=(emu:read32(qa.sym.gBattleTypeFlags)%2==1 and 2 or 1),emu:read8(qa.sym.gPlayerPartyCount)-1 do
   local p=qa.sym.gPlayerParty+i*100
   if emu:read16(p+86)>0 then
    local score=emu:read8(p+84)*100+emu:read16(p+86)
    if score>best then qa.partyTarget=i;best=score end
   end
  end
 end
 if qa.partyTarget then
  local c=emu:read8(0x203b630+9)
  local key=c==qa.partyTarget and 1 or (c<qa.partyTarget and 128 or 64)
  emu:setKeys(qa.frame%45<8 and key or 0)
 end
end)
