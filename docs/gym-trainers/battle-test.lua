qa.ninjaScripts={135611279,135611318,135611357,135611409,135611448,135611487}
qa.finishWatcher=callbacks:add('frame',function()
 if qa.vsRunner or qa.queue[qa.step] then return end
 callbacks:remove(qa.finishWatcher)
 local p=emu:read32(0x03005178)
 for id=3,8 do if emu:read8(p+0x648+id)>0 then
  qa.select(id);local b={0x7b,0,0,53,0,0x05};for _,x in ipairs(qa.u32(qa.ninjaScripts[id-2])) do table.insert(b,x) end
  qa.script(b);qa.run({{kind='text',frames=18000}});break
 end end
 qa.battleSeen=false
 qa.battleCheck=callbacks:add('frame',function()
  local main=emu:read32(0x03003264)
  if main==0x08010d51 and not qa.battleSeen then
   qa.battleSeen=true;qa.result('rematch-starts',true,'Opponent '..emu:read16(0x0203890e));qa.capture('rematch-battle')
  elseif qa.battleSeen and main==0x08056541 and emu:read8(0x03000f9c)==0 then
   qa.run({});qa.result('rematch-completes',emu:read8(0x02023f4e)==1,'Battle outcome '..emu:read8(0x02023f4e));qa.capture('rematch-complete');callbacks:remove(qa.battleCheck)
  end
 end)
end)
