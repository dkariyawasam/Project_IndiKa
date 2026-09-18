function qa.pickMove(target)
 qa.autoBattle=false;qa.run({});if qa.chooser then callbacks:remove(qa.chooser) end
 local t=0;local finishing=0
 qa.chooser=callbacks:add('frame',function()
  t=t+1
  if finishing>0 then finishing=finishing+1;if finishing>=8 then emu:setKeys(0);callbacks:remove(qa.chooser);qa.chooser=nil end;return end
  if t>2400 then emu:setKeys(0);callbacks:remove(qa.chooser);qa.chooser=nil;return end
  local ctrl=emu:read32(qa.bsym.controllers);local key=0
  if ctrl==qa.bsym.action+1 then local c=emu:read8(qa.bsym.actionCursor);key=c%2==1 and 32 or (c>=2 and 64 or 1)
  elseif ctrl==qa.bsym.move+1 then
   local c=emu:read8(qa.bsym.moveCursor)
   key=c%2~=target%2 and (c%2==0 and 16 or 32) or (math.floor(c/2)~=math.floor(target/2) and (c<target and 128 or 64) or 1)
   if c==target and t%30<6 then finishing=1 end
  end
  emu:setKeys(t%30<6 and key or 0)
 end)
end
