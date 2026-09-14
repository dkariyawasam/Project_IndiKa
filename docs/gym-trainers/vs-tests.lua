qa.vsPhase=0;qa.vsAttempts=0;qa.responses={}
qa.vsRunner=callbacks:add('frame',function()
 if qa.queue[qa.step] then return end
 local p=emu:read32(0x03005178)
 if qa.vsPhase==0 then
  qa.setFlag(0x292,1)
  for _,id in ipairs({288,289,292,293,294,295}) do qa.setFlag(0x500+id,1) end
  qa.script({0x44,94,1,1,0,0x79,6,0,100,0,0,0,0,0,0,0,0,0,0,0,0x6c,0x02})
  qa.run({{kind='wait',frames=60}});qa.vsPhase=1
 elseif qa.vsPhase==1 then
  emu:write16(p+0x296,350);emu:write16(p+0x646,100)
  qa.run({{kind='press',key=4,frames=30},{kind='wait',frames=600}});qa.vsPhase=2
 else
  qa.vsAttempts=qa.vsAttempts+1
  local all=true
  for id=3,8 do if emu:read8(p+0x648+id)>0 then qa.responses[id]=true end;if not qa.responses[id] then all=false end end
  if all or qa.vsAttempts>=12 then
   for id=3,8 do qa.result('ninja-'..id..'-accepts-rematch',qa.responses[id] or false,'Accepted within '..qa.vsAttempts..' charged activations') end
   qa.capture('vs-seeker-final');callbacks:remove(qa.vsRunner);qa.vsRunner=nil
  else qa.vsPhase=1 end
 end
end)
