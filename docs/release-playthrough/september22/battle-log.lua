qa.bsym={main=0x8010d6c,mons=0x2023ca8,controllers=0x3005150,action=0x802e188,move=0x802e760,actionCursor=0x20240bc,moveCursor=0x20240c0,outcome=0x2023f4e,text=0x2022a50}
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
   if f==qa.bsym.action+1 or f==qa.bsym.move+1 or f==0x802e3c5 then actor=i;break end
  end
  local ctrl=emu:read32(qa.bsym.controllers+actor*4);local mon=qa.bsym.mons+actor*88
  if qa.textOnlyBattle and (ctrl==qa.bsym.action+1 or ctrl==qa.bsym.move+1 or ctrl==0x802e3c5) then if qa.step>#qa.queue then emu:setKeys(0) end;return end
  if ctrl==qa.bsym.action+1 then
   local retreat=false;for i=0,3 do if emu:read16(mon+12+i*2)==362 and emu:read8(mon+36+i)>0 then retreat=true end end
   local dest=qa.skipWild and emu:read32(qa.sym.gBattleTypeFlags)%16<8 and not retreat and 3 or 0
   local enemyAbility=emu:read8(qa.bsym.mons+88+32)
   if not retreat and (enemyAbility==71 or enemyAbility==23) then dest=0 end
   local canAttack=false;local living=0
   for i=0,3 do local move=emu:read16(mon+12+i*2);if move>0 and emu:read8(mon+36+i)>0 and emu:read8(0x823531c+move*12+1)>0 then canAttack=true end end
   for i=0,emu:read8(qa.sym.gPlayerPartyCount)-1 do if emu:read16(qa.sym.gPlayerParty+i*100+86)>0 then living=living+1 end end
   if emu:read32(qa.sym.gBattleTypeFlags)%16>=8 and not canAttack and living>1 then dest=2 end
   -- Switch Ivysaur out of a Grass mirror match using normal party inputs.
   if emu:read16(mon)==2 and emu:read32(qa.sym.gBattleTypeFlags)%2==0 and emu:read32(qa.sym.gBattleTypeFlags)%16>=8 and (emu:read8(qa.bsym.mons+88+33)==12 or emu:read8(qa.bsym.mons+88+34)==12) and living>1 then dest=2 end
   local c=emu:read8(qa.bsym.actionCursor+actor)
   key=c%2~=dest%2 and (c%2==0 and 16 or 32) or (math.floor(c/2)~=math.floor(dest/2) and (c<dest and 128 or 64) or 1)
  elseif ctrl==0x802e3c5 and qa.preferredTarget then
   local target=qa.preferredTarget
   if emu:read16(qa.bsym.mons+target*88+40)==0 then target=target==3 and 1 or 3 end
   key=emu:read8(0x3005164)==target and 1 or 32
  elseif ctrl==qa.bsym.move+1 then
   local target=0;local best=-1;local bestRank=-1
   local enemies=emu:read32(qa.sym.gBattleTypeFlags)%2==1 and {1,3} or {1}
   for _,enemyIndex in ipairs(enemies) do
    local foe=qa.bsym.mons+enemyIndex*88
    if emu:read16(foe+40)>0 then
     for i=0,3 do
      local move=emu:read16(mon+12+i*2);local pp=emu:read8(mon+36+i)
      if move>0 and pp>0 and move~=emu:read16(0x2023ed0+actor*28+4) then
       local p=0x823531c+move*12;local power=emu:read8(p+1);local typ=emu:read8(p+2);local score=power
       if typ==emu:read8(mon+33) or typ==emu:read8(mon+34) then score=score*1.5 end
       local t1=emu:read8(foe+33);local t2=emu:read8(foe+34)
       for j=0,149 do local ep=0x823380c+j*3;local at=emu:read8(ep);if at==255 then break end
        local dt=emu:read8(ep+1);if at==typ and (dt==t1 or dt==t2) then score=score*emu:read8(ep+2)/10 end
       end
       local atk=emu:read16(mon+(typ<10 and 2 or 8));local def=math.max(1,emu:read16(foe+(typ<10 and 4 or 10)))
       local damage=((2*emu:read8(mon+42)/5+2)*score*atk/def/50+2)*0.9
       if move==69 or move==101 then damage=score>0 and emu:read8(mon+42) or 0 end
       if score==0 then damage=0 end
       local hp=emu:read16(foe+40);local rank=100*math.min(damage/hp,1.5)+damage*.1
       if damage>=hp then rank=rank+100 end
       if power>0 and rank>bestRank then target=i;best=score;bestRank=rank;qa.preferredTarget=enemyIndex end
      end
     end
    end
   end
   if best<0 then
    for i=0,3 do if emu:read16(mon+12+i*2)>0 and emu:read8(mon+36+i)>0 then target=i;break end end
    for i=0,3 do if emu:read16(mon+12+i*2)==235 and emu:read8(mon+36+i)>0 then target=i end end
   end
   if emu:read32(qa.sym.gBattleTypeFlags)%16<8 and (qa.skipWild or emu:read8(qa.bsym.mons+0x58+42)>emu:read8(mon+42)+3 or emu:read16(mon+40)*3<=emu:read16(mon+44) or best<0) then
    for i=0,3 do if emu:read16(mon+12+i*2)==362 and emu:read8(mon+36+i)>0 then target=i end end
   end
   -- Use Leech Seed against healthy trainer opponents before attacking.
   local enemy=qa.bsym.mons+(qa.preferredTarget or 1)*88
   if best<120 and emu:read32(qa.sym.gBattleTypeFlags)%16>=8 and emu:read16(enemy+40)*2>emu:read16(enemy+44) and emu:read8(enemy+33)~=12 and emu:read8(enemy+34)~=12 and math.floor(emu:read32(0x2023ec0+(qa.preferredTarget or 1)*4)/4)%2==0 then
    for i=0,3 do if emu:read16(mon+12+i*2)==73 and emu:read8(mon+36+i)>0 and emu:read16(0x2023ed0+actor*28+4)~=73 then target=i end end
   end
   -- Stall Destiny Bond with an available support move; seed can end trapping encounters safely.
   local enemy=qa.bsym.mons+(qa.preferredTarget or 1)*88
   if math.floor(emu:read32(enemy+80)/33554432)%2==1 then
    for i=0,3 do local move=emu:read16(mon+12+i*2);if move>0 and emu:read8(mon+36+i)>0 and move~=emu:read16(0x2023ed0+actor*28+4) and emu:read8(0x823531c+move*12+1)==0 then target=i;break end end
   end
   if emu:read8(enemy+32)==23 and emu:read16(mon+12+target*2)~=362 and math.floor(emu:read32(0x2023ec0+(qa.preferredTarget or 1)*4)/4)%2==0 then
    for i=0,3 do if emu:read16(mon+12+i*2)==73 and emu:read8(mon+36+i)>0 and emu:read16(0x2023ed0+actor*28+4)~=73 then target=i end end
   end
   if qa.useSafeguard and emu:read32(qa.sym.gBattleTypeFlags)%16>=8 and math.floor(emu:read16(0x2023ea2)/32)%2==0 then
    for i=0,3 do if emu:read16(mon+12+i*2)==219 and emu:read8(mon+36+i)>0 then target=i end end
   end
   -- Use the earned recovery move when health is below half.
   if emu:read16(mon+40)*2<emu:read16(mon+44) then
    for i=0,3 do if emu:read16(mon+12+i*2)==105 and emu:read8(mon+36+i)>0 and emu:read16(0x2023ed0+actor*28+4)~=105 then target=i end end
   end
   local c=emu:read8(qa.bsym.moveCursor+actor)
   key=c%2~=target%2 and (c%2==0 and 16 or 32) or (math.floor(c/2)~=math.floor(target/2) and (c<target and 128 or 64) or 1)
  end
  emu:setKeys(qa.frame%30<6 and key or 0)
 end
end)
