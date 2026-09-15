dofile('/tmp/champion-movesets/readiness-cases.lua')
local base='/tmp/champion-movesets/readiness-base.ss'
local out='/Users/deekariyawasam/pokefirered/docs/apex-readiness/'
local function bytes(addr,n)local s={};for i=0,n-1 do s[#s+1]=string.char(emu:read8(addr+i))end;return table.concat(s)end
local function record(kind,ok,detail)local f=io.open(out..'runtime.tsv','a');f:write(qa.r.case.name..'\t'..kind..'\t'..(ok and 'PASS' or 'FAIL')..'\t'..(detail or '')..'\n');f:close()end
local function snap(name)emu:screenshot(out..qa.r.case.name..'-'..name..'.png')end
function qa.readinessCase(i)
 local c=qa.readinessCases[i];if not c then qa.r=nil;qa.readinessDone=true;qa.run({});return end
 emu:loadStateFile(base);qa.testRunning=false;qa.run({});qa.r={case=c,index=i,mode='no',phase='warp',frame=qa.frame}
 emu:write8(emu:read32(qa.sym.gSaveBlock2Ptr)+2836,0);qa.setFlag(c.fought,0);qa.setFlag(c.interacted,0);qa.setFlag(c.hide,0);qa.setFlag(0x818,0)
 emu:write8(qa.sym.gBattleOutcome,0);qa.warp(c.group,c.map,c.x,c.y);qa.run({{kind='wait',frames=220}})
end
local function prompt()
 local r=qa.r;qa.select(r.case.localId);r.enemy=bytes(qa.sym.gEnemyParty,600);r.phase='prompt';r.frame=qa.frame
 local b={0x05};for _,v in ipairs(qa.u32(qa.sym[r.case.label]))do b[#b+1]=v end;qa.script(b);qa.run({{kind='text',frames=5000}})
end
qa.readinessCallback=callbacks:add('frame',function()
 if qa.caseIndex==13 and not qa.r and not qa.readinessStarted then
  qa.readinessStarted=true;qa.testRunning=false;qa.phase='paused';qa.run({});emu:saveStateFile(base);qa.readinessCase(1);return
 end
 local r=qa.r;if not r then return end
 local elapsed=qa.frame-r.frame;local c=r.case
 if r.phase=='warp' and elapsed>240 then
  qa.setFlag(c.hide,0);qa.script({0x55,c.localId,0,0x6b,2});r.phase='spawn';r.frame=qa.frame;qa.run({{kind='wait',frames=100}})
 elseif r.phase=='spawn' and elapsed>120 then prompt()
 elseif r.phase=='prompt' then
  for i=0,15 do local p=qa.sym.gTasks+i*40
   if emu:read8(p+4)~=0 and emu:read32(p)==qa.sym.Task_YesNoMenu_HandleInput+1 then
    r.phase='menu';r.frame=qa.frame;qa.run({{kind='wait',frames=70}});return
   end
  end
 elseif r.phase=='menu' and elapsed>80 then
  snap(r.mode..'-prompt')
  if r.mode=='no' then qa.run({{kind='press',key=128,frames=30},{kind='press',key=1,frames=30},{kind='wait',frames=150}})
  elseif r.mode=='back' then qa.run({{kind='press',key=2,frames=30},{kind='wait',frames=150}})
  else qa.run({{kind='text',frames=10000}})end
  r.phase=r.mode=='yes' and 'battle' or 'declined';r.frame=qa.frame
 elseif r.phase=='declined' and elapsed>250 then
  local ok=qa.flag(c.fought)==0 and qa.flag(c.interacted)==0 and qa.flag(c.hide)==0 and bytes(qa.sym.gEnemyParty,600)==r.enemy and emu:read8(0x03000f9c)==0 and qa.select(c.localId)
  record(r.mode,ok,'flags untouched; enemy unchanged; controls released; object available');snap(r.mode..'-declined')
  r.mode=r.mode=='no' and 'back' or 'yes';prompt()
 elseif r.phase=='battle' and emu:read32(qa.sym.gMain+4)==qa.sym.BattleMainCB2+1 then
  record('yes',qa.flag(c.interacted)==1 and emu:read8(qa.sym.gEnemyParty+84)==c.level,'battle entered at level '..emu:read8(qa.sym.gEnemyParty+84));r.phase='battleCapture';r.frame=qa.frame;qa.run({{kind='wait',frames=220}})
 elseif r.phase=='battleCapture' and elapsed>240 then
  snap('accepted-battle');qa.readinessCase(r.index+1)
 end
end)
