qa.sym.HandleInputChooseAction=0x0802e06c;qa.sym.HandleInputChooseTarget=0x0802e2a8
qa.testRunning=true;dofile('/tmp/overworld-palette-playthrough/pilot.lua')
local b={0x5c,0,70,1,8,0};local text=qa.sym.PewterCity_Gym_Text_BrockDefeat
for i=1,2 do for _,v in ipairs(qa.u32(text))do b[#b+1]=v end end
b[#b+1]=0x6b;b[#b+1]=2
emu:write8(qa.sym.gBattleOutcome,0);qa.script(b);qa.run({{kind='text',frames=90000}})
qa.battleCheck=callbacks:add('frame',function()
 if not qa.testRunning then return end
 if emu:read32(qa.sym.gMain+4)==qa.sym.BattleMainCB2+1 then
  if not qa.battleSeen then emu:screenshot('/Users/deekariyawasam/pokefirered/docs/overworld-palette-playthrough/37-lab-battle.png')end
  qa.battleSeen=true
 end
 if qa.battleSeen and emu:read8(qa.sym.gBattleOutcome)~=0 then
  qa.testRunning=false
  local f=io.open('/tmp/overworld-palette-playthrough/battle-result.txt','w');f:write(tostring(emu:read8(qa.sym.gBattleOutcome)));f:close()
  qa.run({{kind='text',frames=900},{kind='wait',frames=120},{kind='capture',name='38-lab-after-battle'}})
 end
end)
