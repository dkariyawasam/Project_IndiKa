dofile('/tmp/leader-nicknames/cases.lua');dofile('/tmp/leader-nicknames/characters.lua')
if qa.nickDriver then callbacks:remove(qa.nickDriver)end
emu:loadStateFile('/tmp/leader-nicknames/base.ss');qa.run({{kind='wait',frames=60}});qa.nickPhase='start';qa.nickIndex=1
function qa.readName(addr)
 local s='';for i=0,9 do local v=emu:read8(addr+i);if v==255 then break end;s=s..(qa.letters[v] or '?')end;return s
end
qa.nickDriver=callbacks:add('frame',function()
 if qa.queue[qa.step] then return end
 if qa.nickPhase=='base' then emu:saveStateFile('/tmp/leader-nicknames/base.ss');qa.nickPhase='start' end
 local c=qa.cases[qa.nickIndex];if not c then qa.nickPhase='done';return end
 if qa.nickPhase=='start' then
  emu:loadStateFile('/tmp/leader-nicknames/base.ss');qa.run({{kind='wait',frames=60}});qa.nickPhase='inject'
 elseif qa.nickPhase=='inject' then
  qa.setFlag(0x500+c.trainer,0)
  local mode=(c.name=='SILPH_GIOVANNI' or c.name=='TRAINER_LEADER_GIOVANNI' or c.name=='TRAINER_ROCKET_LEAGUE_CHAMPION_GIOVANNI') and 0 or 4
  local bytes={0x5c,mode,c.trainer%256,math.floor(c.trainer/256),0,0};local text=qa.sym.PewterCity_Gym_Text_BrockDefeat
  for j=1,(mode==4 and 3 or 2)do for _,v in ipairs(qa.u32(text))do bytes[#bytes+1]=v end end
  bytes[#bytes+1]=0x6b;bytes[#bytes+1]=2;qa.script(bytes);qa.run({{kind='text',frames=1400},{kind='wait',frames=30}});qa.nickPhase='check'
 elseif qa.nickPhase=='check' then
  if emu:read8(qa.sym.gEnemyParty+8)==0 and (c.retries or 0)<10 then c.retries=(c.retries or 0)+1;qa.run({{kind='text',frames=300}});return end
  local f=io.open('/Users/deekariyawasam/pokefirered/docs/leader-nicknames/runtime.tsv','a')
  for i,want in ipairs(c.names)do local got=qa.readName(qa.sym.gEnemyParty+(i-1)*100+8);f:write(c.name..'\t'..i..'\t'..want..'\t'..got..'\t'..(got==want and 'PASS' or 'FAIL')..'\n')end;f:close()
  emu:screenshot('/Users/deekariyawasam/pokefirered/docs/leader-nicknames/'..c.name..'.png')
  qa.nickIndex=qa.nickIndex+1;qa.nickPhase='start'
 end
end)
