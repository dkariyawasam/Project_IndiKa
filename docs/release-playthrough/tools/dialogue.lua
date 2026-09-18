function qa.dialogue()
 qa.run({});if qa.talker then callbacks:remove(qa.talker) end
 if qa.walker then callbacks:remove(qa.walker);qa.walker=nil end
 local t=0
 qa.talker=callbacks:add('frame',function()
  if emu:read32(qa.sym.gMain+4)==qa.bsym.main+1 then return end
  t=t+1
  if t>60 and emu:read8(qa.sym.sLockFieldControls)==0 or t>12000 then emu:setKeys(0);callbacks:remove(qa.talker);qa.talker=nil;qa.snapshot();return end
  emu:setKeys(t%30<6 and 1 or 0)
 end)
end
