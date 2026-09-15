function qa.callLabel(name,extra)
 local b={4};for _,v in ipairs(qa.u32(qa.sym[name]))do b[#b+1]=v end
 if extra then for _,v in ipairs(extra)do b[#b+1]=v end end;b[#b+1]=0x6b;b[#b+1]=2;qa.script(b)
end
qa.leagueMode='rocket';qa.leagueIndex=1;qa.leaguePhase='entry';qa.warp(2,0,6,9);qa.run({{kind='wait',frames=1200}})
qa.rocketActors={'AdminM','AdminF','Petrel','AdminM','AdminF','Petrel'}
qa.indigoActors={{'Lorelei',77},{'Bruno',79},{'Agatha',75},{'Lance',74},{'Brock',80},{'Misty',81},{'Surge',82},{'Erika',83},{'Koga',84},{'Sabrina',85},{'Blaine',86},{'Giovanni',87}}
qa.leagueTour=callbacks:add('frame',function()
 if qa.queue[qa.step] then return end
 if qa.leagueMode=='done' then return end
 if qa.leagueMode=='rocket' then
  local actor=qa.rocketActors[qa.leagueIndex]
  if not actor then qa.leagueMode='indigo';qa.leagueIndex=1;qa.leaguePhase='entry';qa.warp(1,72,6,9);qa.run({{kind='wait',frames=1200}});return end
  if qa.leaguePhase=='entry' then qa.callLabel('RocketLeague_Arena_EventScript_'..actor..'Enter');qa.run({{kind='wait',frames=240},{kind='capture',name='rocket-'..qa.leagueIndex..'-'..actor..'-in'}});qa.leaguePhase='exit'
  else qa.callLabel('RocketLeague_Arena_EventScript_'..actor..'Exit');qa.run({{kind='wait',frames=240},{kind='capture',name='rocket-'..qa.leagueIndex..'-'..actor..'-out'}});qa.leagueIndex=qa.leagueIndex+1;qa.leaguePhase='entry' end
 else
  local actor=qa.indigoActors[qa.leagueIndex]
  if not actor then qa.leagueMode='done';return end
  if qa.leaguePhase=='entry' then
   qa.setVar(0x4010,actor[2]);qa.callLabel('PokemonLeague_BrunosRoom_EventScript_IndigoIntro');qa.run({{kind='text',frames=500},{kind='wait',frames=60},{kind='capture',name='indigo-'..qa.leagueIndex..'-'..actor[1]..'-in'}});qa.leaguePhase='exit'
  else
   local b={0x4f,1,0};for _,v in ipairs(qa.u32(qa.sym.PokemonLeague_BrunosRoom_Movement_OpponentExit))do b[#b+1]=v end
   for _,v in ipairs({0x51,0,0,0x53,1,0,0x6b,2})do b[#b+1]=v end;qa.script(b)
   qa.run({{kind='wait',frames=240},{kind='capture',name='indigo-'..qa.leagueIndex..'-out'}});qa.leagueIndex=qa.leagueIndex+1;qa.leaguePhase='entry'
  end
 end
end)
