qa.erikaCheck=callbacks:add('frame',function()
 if qa.followRunner or qa.queue[qa.step] then return end
 if not qa.erikaCheckStage then
  qa.erikaCheckStage=1
  qa.setVar(0x408f,3);qa.setFlag(0xb4,0)
  qa.warp(1,0,5,48)
  qa.run({{kind='wait',frames=600}})
 else
  qa.result('Erika-guide-preserved-on-return',qa.flag(0xb4)==0,'Guide hide flag '..qa.flag(0xb4)..'; trial state '..qa.var(0x408f))
  qa.capture('Erika-guide-return-audit')
  callbacks:remove(qa.erikaCheck);qa.erikaCheck=nil
 end
end)
