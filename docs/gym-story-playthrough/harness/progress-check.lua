qa.progressPhase=0
qa.progressCheck=callbacks:add('frame',function()
 if qa.battleRunner or qa.queue[qa.step] then return end
 if qa.progressPhase==0 then
  qa.script({0xd6,9,1,0,5,0,0x6c,0x02});qa.run({{kind='wait',frames=30}});qa.progressPhase=1
 elseif qa.progressPhase==1 then
  qa.result('Sabrina-research-completion-record',emu:read16(0x020371e8)~=0,'Badge '..qa.flag(2102)..'; completion record '..emu:read16(0x020371e8))
  qa.script({0xd6,9,1,0,7,0,0x6c,0x02});qa.run({{kind='wait',frames=30}});qa.progressPhase=2
 else
  qa.result('Giovanni-research-completion-record',emu:read16(0x020371e8)~=0,'Badge '..qa.flag(2104)..'; completion record '..emu:read16(0x020371e8))
  qa.capture('all-badges-research-audit');callbacks:remove(qa.progressCheck);qa.progressCheck=nil
 end
end)
