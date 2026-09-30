function qa.finishDialogue()
 qa.run({});if qa.dialogueCb then callbacks:remove(qa.dialogueCb) end
 local t=0
 qa.dialogueCb=callbacks:add('frame',function()
  t=t+1
  if emu:read8(qa.sym.sLockFieldControls)==0 or t>7200 then
   emu:setKeys(0);callbacks:remove(qa.dialogueCb);qa.dialogueCb=nil;return
  end
  if emu:read32(qa.sym.gMain+4)==qa.sym.CB2_Overworld+1 then emu:setKeys(t%60<20 and 1 or 0) end
 end)
end
