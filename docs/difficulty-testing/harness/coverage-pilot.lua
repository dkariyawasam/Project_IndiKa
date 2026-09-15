if qa.pilot then callbacks:remove(qa.pilot) end
function qa.effect(t,d)
 local mult=1;local a=emu:read8(d+33);local b=emu:read8(d+34)
 for k=0,363,3 do local p=qa.sym.gTypeEffectiveness+k;local at=emu:read8(p);local de=emu:read8(p+1);local m=emu:read8(p+2)
  if at==255 then break end
  if at==t and (de==a or de==b) then mult=mult*m/10 end
 end
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
