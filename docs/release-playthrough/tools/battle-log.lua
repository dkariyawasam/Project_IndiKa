qa.bsym={main=0x08010d54,mons=0x02023ca8,controllers=0x03005150,action=0x0802e090,move=0x0802e668,actionCursor=0x020240bc,moveCursor=0x020240c0,outcome=0x02023f4e,text=0x02022a50}
if qa.battleLog then callbacks:remove(qa.battleLog) end
qa.battleCount=qa.battleCount or 0
qa.battleLog=callbacks:add('frame',function()
 local battle=emu:read32(qa.sym.gMain+4)==qa.bsym.main+1
 if battle and not qa.inBattle then
  qa.battleCount=qa.battleCount+1;local f=io.open('/tmp/kanto-release-playtest/battles.tsv','a');f:write('START\t'..qa.battleCount..'\n');f:close();qa.inBattle=true
 elseif not battle and qa.inBattle and emu:read32(qa.sym.gMain+4)==qa.sym.CB2_Overworld+1 then
  local f=io.open('/tmp/kanto-release-playtest/battles.tsv','a');f:write('END\t'..qa.battleCount..'\t'..emu:read8(qa.bsym.outcome)..'\tHP '..emu:read16(qa.sym.gPlayerParty+86)..'\tLV '..emu:read8(qa.sym.gPlayerParty+84)..'\n');f:close();qa.inBattle=false
 end
 if battle and qa.autoBattle then
  local key=1;local ctrl=emu:read32(qa.bsym.controllers)
  if ctrl==qa.bsym.action+1 then local c=emu:read8(qa.bsym.actionCursor);key=c%2==1 and 32 or (c>=2 and 64 or 1)
  elseif ctrl==qa.bsym.move+1 then
   local target=0;local best=-1
   for i=0,3 do
    local move=emu:read16(qa.bsym.mons+12+i*2);local pp=emu:read8(qa.bsym.mons+36+i)
    if move>0 and pp>0 then
     local p=0x08234178+move*12;local power=emu:read8(p+1);local typ=emu:read8(p+2);local score=power
     if typ==emu:read8(qa.bsym.mons+33) or typ==emu:read8(qa.bsym.mons+34) then score=score*1.5 end
     local t1=emu:read8(qa.bsym.mons+88+33);local t2=emu:read8(qa.bsym.mons+88+34)
     for j=0,149 do local ep=0x08232668+j*3;local at=emu:read8(ep);if at==255 then break end
      local dt=emu:read8(ep+1);if at==typ and (dt==t1 or dt==t2) then score=score*emu:read8(ep+2)/10 end
     end
     if power>0 and score>best then target=i;best=score end
    end
   end
   if best<0 then
    for i=0,3 do if emu:read16(qa.bsym.mons+12+i*2)==235 and emu:read8(qa.bsym.mons+36+i)>0 then target=i end end
   end
   if emu:read32(qa.sym.gBattleTypeFlags)%16<8 and emu:read8(qa.bsym.mons+0x58+42)>emu:read8(qa.bsym.mons+42)+3 then
    for i=0,3 do if emu:read16(qa.bsym.mons+12+i*2)==362 and emu:read8(qa.bsym.mons+36+i)>0 then target=i end end
   end
   local c=emu:read8(qa.bsym.moveCursor)
   key=c%2~=target%2 and (c%2==0 and 16 or 32) or (math.floor(c/2)~=math.floor(target/2) and (c<target and 128 or 64) or 1)
  end
  emu:setKeys(qa.frame%30<6 and key or 0)
 end
end)
