qa.testingLoss=true;qa.lossPhase='start'
local b={0x25};local n=qa.sym.SPECIAL_StartIndigoLeagueChallenge;table.insert(b,n%256);table.insert(b,math.floor(n/256));for _,v in ipairs({0x39,1,72,255,6,0,9,0,0x27,0x02}) do table.insert(b,v) end
qa.script(b);qa.run({{kind='wait',frames=300},{kind='press',key=64,hold=12,frames=30},{kind='text',frames=12000}})
qa.lossWatcher=callbacks:add('frame',function()
 local main=emu:read32(0x03003264);local g,m=qa.state()
 if qa.lossPhase=='start' and main==qa.sym.BattleMainCB2+1 and emu:read8(qa.sym.sIndigoPartySaved)==1 then
  qa.lossTimer=(qa.lossTimer or 0)+1
  if qa.lossTimer==650 then qa.capture('loss-battle');emu:write8(qa.sym.gBattleOutcome,2);qa.lossPhase='return' end
 elseif qa.lossPhase=='return' and main==qa.sym.CB2_Overworld+1 and g==13 and m==0 then
  qa.run({});qa.dump('after-loss');qa.capture('loss-return');local sb2=emu:read32(qa.sym.gSaveBlock2Ptr)
  qa.result('loss-resets-run',emu:read8(sb2+0xb14)==0 and emu:read8(qa.sym.sIndigoPartySaved)==0)
  qa.lossPhase='done'
 end
end)
