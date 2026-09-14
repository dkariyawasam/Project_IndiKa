qa.roundPhase='prepare';qa.roundIndex=2
qa.roundRunner=callbacks:add('frame',function()
 local g,m,x,y=qa.state();local main=emu:read32(0x03003264)
 if qa.roundPhase=='prepare' then
  if qa.queue[qa.step] or main~=qa.sym.CB2_Overworld+1 then return end
  qa.dump('before-round-'..qa.roundIndex)
  qa.warp(1,72,6,9);qa.run({{kind='wait',frames=300},{kind='press',key=64,hold=12,frames=30},{kind='text',frames=18000}})
  qa.roundPhase='battle';qa.seen=false;qa.battleTimer=0
 elseif qa.roundPhase=='battle' then
  if main==qa.sym.BattleMainCB2+1 and emu:read8(qa.sym.sIndigoPartySaved)==1 then
   qa.battleTimer=qa.battleTimer+1
   if qa.battleTimer==600 then
    qa.capture('round-'..qa.roundIndex..'-battle');qa.dump('round-'..qa.roundIndex..'-scaled')
    for i=0,5 do emu:write16(qa.sym.gEnemyParty+i*100+86,1) end
    for _,i in ipairs({1,3}) do emu:write16(qa.sym.gBattleMons+i*88+40,1) end
    qa.seen=true
   end
  end
  if qa.seen and main==qa.sym.CB2_Overworld+1 and emu:read8(0x03000f9c)==0 then
   qa.run({});qa.dump('after-round-'..qa.roundIndex);qa.capture('round-'..qa.roundIndex..'-complete')
   qa.result('round-'..qa.roundIndex..'-won',emu:read8(qa.sym.gBattleOutcome)==1)
   qa.roundIndex=qa.roundIndex+1
   if qa.roundIndex>4 then qa.roundPhase='done';emu:saveStateFile('/tmp/indigo-live/four-wins.ss') else qa.roundPhase='prepare' end
  end
 end
end)
