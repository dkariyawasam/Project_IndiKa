qa.mapCases={{'Celadon',10,16,6,10,true},{'Saffron',14,3,14,12,true},{'Pewter',6,2,6,10,false}}
qa.mapIdx=1;qa.mapPhase=0
qa.mapTests=callbacks:add('frame',function()
 if qa.queue[qa.step] then return end
 local c=qa.mapCases[qa.mapIdx]
 if not c then callbacks:remove(qa.mapTests);return end
 if qa.mapPhase==0 then
  qa.setVar(0x408c,4);qa.setVar(0x408f,4);qa.setVar(0x4091,4)
  qa.warp(c[2],c[3],c[4],c[5]);qa.run({{kind='wait',frames=600}});qa.mapPhase=1
 elseif qa.mapPhase==1 then
  emu:write16(emu:read32(0x03005178)+0x646,100)
  qa.run({{kind='press',key=4,frames=30},{kind='wait',frames=600},{kind='capture',name=c[1]..'-vs-policy'}});qa.mapPhase=2
 else
  local msg=qa.lastMessage or ''
  local ok=c[6] and msg:find('no TRAINERS within range',1,true)~=nil or not c[6] and msg:find('time to use that',1,true)~=nil
  qa.result(c[1]..'-vs-policy',ok,msg)
  qa.run({{kind='text',frames=120}});qa.mapIdx=qa.mapIdx+1;qa.mapPhase=0
 end
end)
