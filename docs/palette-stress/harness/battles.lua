dofile('/tmp/palette-stress/pilot.lua');dofile('/tmp/palette-stress/replacement-assist.lua')
qa.sym.HandleInputChooseAction=0x0802e06c;qa.sym.HandleInputChooseTarget=0x0802e2a8
qa.battleCases={{'rocket',2,0,'SPECIAL_StartRocketLeagueChallenge','RocketLeague_Arena_EventScript_BattlePetrel'},{'indigo',1,72,'SPECIAL_StartIndigoLeagueChallenge','PokemonLeague_BrunosRoom_EventScript_BattleBruno'}}
qa.battleIndex=1;qa.battleStage='warp'
qa.battles=callbacks:add('frame',function()
 local c=qa.battleCases[qa.battleIndex];if not c then return end
 if qa.battleStage=='warp' and not qa.queue[qa.step] then
  qa.warp(c[2],c[3],6,9);qa.run({{kind='wait',frames=1200}});qa.battleStage='start'
 elseif qa.battleStage=='start' and not qa.queue[qa.step] then
  local s=qa.sym[c[4]];qa.script({0x25,s%256,math.floor(s/256),0x6b,2});qa.run({{kind='wait',frames=120}});qa.battleStage='begin'
 elseif qa.battleStage=='begin' and not qa.queue[qa.step] then
  qa.testRunning=true;qa.seenBattle=false;emu:write8(qa.sym.gBattleOutcome,0);qa.callLabel(c[5]);qa.run({{kind='text',frames=120000}});qa.battleStage='battle'
 elseif qa.battleStage=='battle' then
  if emu:read32(qa.sym.gMain+4)==qa.sym.BattleMainCB2+1 then qa.seenBattle=true end
  if qa.seenBattle and emu:read8(qa.sym.gBattleOutcome)~=0 then
   local f=io.open('/Users/deekariyawasam/pokefirered/docs/palette-stress/battles.tsv','a');f:write(c[1]..'\t'..emu:read8(qa.sym.gBattleOutcome)..'\n');f:close()
   qa.testRunning=false;qa.battleStage='return';qa.run({{kind='text',frames=1600},{kind='wait',frames=180},{kind='capture',name=c[1]..'-after-battle'}})
  end
 elseif qa.battleStage=='return' and not qa.queue[qa.step] then
  qa.battleIndex=qa.battleIndex+1;qa.battleStage='warp'
 end
end)
