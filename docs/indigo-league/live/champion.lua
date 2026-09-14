qa.champPhase='walking'
qa.run({{kind='walk',x=9,y=9},{kind='walk',x=9,y=3},{kind='walk',x=7,y=3},{kind='capture',name='champion-door-open'},{kind='map',group=1,map=74,key=32},{kind='text',frames=18000}})
qa.champWatcher=callbacks:add('frame',function()
 local g,m=qa.state();local main=emu:read32(0x03003264)
 if qa.champPhase=='walking' and main==qa.sym.BattleMainCB2+1 and emu:read8(qa.sym.sIndigoPartySaved)==1 then
  qa.champTimer=(qa.champTimer or 0)+1
  if qa.champTimer==650 then
   qa.capture('champion-battle');qa.dump('champion-scaled');qa.result('champion-doubles',emu:read8(qa.sym.gBattlersCount)==4)
   emu:write8(qa.sym.gBattleOutcome,1);qa.champPhase='return'
  end
 elseif qa.champPhase=='return' and main==qa.sym.CB2_Overworld+1 and emu:read8(qa.sym.sIndigoPartySaved)==0 then
  qa.dump('after-champion');qa.result('champion-restored',true);qa.champPhase='hall'
 elseif qa.champPhase=='hall' and m~=74 and g==1 then
  qa.run({});qa.capture('hall-of-fame-entry');qa.champPhase='done'
 end
end)
