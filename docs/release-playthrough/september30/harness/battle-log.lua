qa.bsym={main=0x8010d6c,mons=0x2023ca8,controllers=0x3005160,action=0x802e174,move=0x802e74c,actionCursor=0x20240bc,moveCursor=0x20240c0,outcome=0x2023f4e,text=0x2022a50}
if qa.battleLog then callbacks:remove(qa.battleLog) end
qa.battleCount=qa.battleCount or 0
qa.battleLog=callbacks:add('frame',function()
 local battle=emu:read32(qa.sym.gMain+4)==qa.bsym.main+1
 if battle and not qa.inBattle then
  qa.battleCount=qa.battleCount+1;local f=io.open('/tmp/kanto-release-september22/battles.tsv','a');f:write('START\t'..qa.battleCount..'\n');f:close();qa.inBattle=true
 elseif not battle and qa.inBattle and emu:read32(qa.sym.gMain+4)==qa.sym.CB2_Overworld+1 then
  local f=io.open('/tmp/kanto-release-september22/battles.tsv','a');f:write('END\t'..qa.battleCount..'\t'..emu:read8(qa.bsym.outcome)..'\tHP '..emu:read16(qa.sym.gPlayerParty+86)..'\tLV '..emu:read8(qa.sym.gPlayerParty+84)..'\n');f:close();qa.inBattle=false
 end
 if battle and qa.autoBattle then
  local key=1;local actor=0
  for _,i in ipairs({0,2}) do
   local f=emu:read32(qa.bsym.controllers+i*4)
   if f==qa.bsym.action+1 or f==qa.bsym.move+1 or f==0x802e3b1 then actor=i;break end
  end
  local ctrl=emu:read32(qa.bsym.controllers+actor*4);local mon=qa.bsym.mons+actor*88
  if ctrl==qa.bsym.action+1 then
   local retreat=false;for i=0,3 do if emu:read16(mon+12+i*2)==362 and emu:read8(mon+36+i)>0 then retreat=true end end
   local dest=qa.skipWild and emu:read32(qa.sym.gBattleTypeFlags)%16<8 and not retreat and 3 or 0
   local enemyAbility=emu:read8(qa.bsym.mons+88+32)
   if not retreat and (enemyAbility==71 or enemyAbility==23) then dest=0 end
   local canAttack=false;local living=0
   for i=0,3 do local move=emu:read16(mon+12+i*2);if move>0 and emu:read8(mon+36+i)>0 and emu:read8(0x8235f68+move*12+1)>0 then canAttack=true end end
   for i=0,emu:read8(qa.sym.gPlayerPartyCount)-1 do if emu:read16(qa.sym.gPlayerParty+i*100+86)>0 then living=living+1 end end
   if emu:read32(qa.sym.gBattleTypeFlags)%16>=8 and not canAttack and living>1 then dest=2 end
   -- Switch Ivysaur out of a Grass mirror match using normal party inputs.
   if emu:read16(mon)==2 and emu:read32(qa.sym.gBattleTypeFlags)%2==0 and emu:read32(qa.sym.gBattleTypeFlags)%16>=8 and (emu:read8(qa.bsym.mons+88+33)==12 or emu:read8(qa.bsym.mons+88+34)==12) and living>1 then dest=2 end
   if emu:read16(mon)==3 and emu:read16(qa.sym.gPlayerParty+200+86)>0 and emu:read16(qa.bsym.mons)~=42 and emu:read16(qa.bsym.mons+176)~=42 and (emu:read8(qa.bsym.mons+88+33)==12 or emu:read8(qa.bsym.mons+88+34)==12) then dest=2;qa.switchPreference=2 end
   if math.floor(emu:read32(0x2023ec0+actor*4)/32)%2==1 and emu:read8(0x2023ed0+actor*28+15)%16<=(actor==0 and 2 or 1) and living>2 then dest=2;qa.switchPreference=actor==0 and 4 or 3 end
   local c=emu:read8(qa.bsym.actionCursor+actor)
   key=c%2~=dest%2 and (c%2==0 and 16 or 32) or (math.floor(c/2)~=math.floor(dest/2) and (c<dest and 128 or 64) or 1)
  elseif ctrl==0x802e3b1 and qa.preferredTarget then
   local target=qa.preferredTarget
   if emu:read16(qa.bsym.mons+target*88+40)==0 then target=target==3 and 1 or 3 end
   key=emu:read8(0x3005174)==target and 1 or 32
  elseif ctrl==qa.bsym.move+1 then
   local target=0;local best=-1;local foe=1
   for _, enemyId in ipairs({1,3}) do
    local enemy=qa.bsym.mons+enemyId*88
    if emu:read16(enemy+40)>0 then
     for i=0,3 do
      local move=emu:read16(mon+12+i*2);local pp=emu:read8(mon+36+i)
      if move>0 and pp>0 then
       local p=0x8235f68+move*12;local power=emu:read8(p+1);local typ=emu:read8(p+2);local score=power
       if typ==emu:read8(mon+33) or typ==emu:read8(mon+34) then score=score*1.5 end
       local t1=emu:read8(enemy+33);local t2=emu:read8(enemy+34)
       for j=0,149 do local ep=0x8234458+j*3;local at=emu:read8(ep);if at==255 then break end
        local dt=emu:read8(ep+1);if at==typ and (dt==t1 or dt==t2) then score=score*emu:read8(ep+2)/10 end
       end
       if move==69 or move==101 then score=score>0 and emu:read8(mon+42)*2 or 0 end
       -- Prefer finishing a weakened opponent; healing uses ordinary move inputs.
       score=score*(1+(1-emu:read16(enemy+40)/math.max(1,emu:read16(enemy+44)))*0.3)
       if power>0 and score>best then target=i;best=score;foe=enemyId end
      end
     end
    end
   end
   if best<0 then for i=0,3 do if emu:read16(mon+12+i*2)>0 and emu:read8(mon+36+i)>0 then target=i;break end end end
   for i=0,3 do
    local move=emu:read16(mon+12+i*2)
    if move==105 and emu:read8(mon+36+i)>0 and emu:read16(mon+40)*2<emu:read16(mon+44) then target=i end
   end
   qa.preferredTarget=foe
   local c=emu:read8(qa.bsym.moveCursor+actor)
   key=c%2~=target%2 and (c%2==0 and 16 or 32) or (math.floor(c/2)~=math.floor(target/2) and (c<target and 128 or 64) or 1)
  end
  emu:setKeys(qa.frame%30<6 and key or 0)
 end
end)
