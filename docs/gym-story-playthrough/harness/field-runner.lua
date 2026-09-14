dofile('/tmp/gym-story-qa/fields.lua')
qa.fieldIndex=1;qa.fieldPhase=0
qa.fieldRunner=callbacks:add('frame',function()
 if qa.queue[qa.step] then return end
 local c=qa.fields[qa.fieldIndex]
 if not c then callbacks:remove(qa.fieldRunner);qa.fieldRunner=nil;qa.capture('field-trials-complete');return end
 if qa.fieldPhase==0 then
  if c.ready then qa.setVar(c.var,c.ready) end
  if c.hide and c.hide~=0 then qa.setFlag(c.hide,0) end
  local b={}
  if qa.fieldIndex==1 or c.name=='Surge-tutoring' or c.name=='Sabrina-tutoring' then
   for i=0,599 do emu:write8(0x02024348+i,0) end;emu:write8(0x020240ed,0)
   local species=c.name=='Sabrina-tutoring' and 94 or 26
   b={0x79,species,0,25,0,0,0,0,0,0,0,0,0,0,0}
  end
  if c.item then for _,x in ipairs({0x44,c.item%256,math.floor(c.item/256),1,0}) do table.insert(b,x) end end
  for _,x in ipairs({0x39,c.g,c.m,255,c.x,0,c.y,0,0x27,0x02}) do table.insert(b,x) end
  qa.script(b);qa.run({{kind='wait',frames=600}});qa.fieldPhase=1
 elseif qa.fieldPhase==1 then
  if not c.auto then
   qa.select(c.id)
   local b={0x05};for _,x in ipairs(qa.u32(c.script)) do table.insert(b,x) end;qa.script(b)
  end
  qa.run({{kind='text',frames=3600},{kind='capture',name=c.name}});qa.fieldPhase=2
 else
  local ok=qa.var(c.var)==c.expected and emu:read8(0x03000f9c)==0
  qa.result(c.name,ok,'Expected state '..c.expected..'; actual '..qa.var(c.var)..'; lock '..emu:read8(0x03000f9c))
  if not ok then callbacks:remove(qa.fieldRunner);qa.fieldRunner=nil;return end
  qa.fieldIndex=qa.fieldIndex+1;qa.fieldPhase=0
 end
end)

qa.fieldFinish=callbacks:add('frame',function()
 local c=qa.fields[qa.fieldIndex];local cmd=qa.queue[qa.step]
 if qa.fieldRunner and c and qa.fieldPhase==2 and cmd and cmd.kind=='text' and qa.timer>30 and qa.var(c.var)==c.expected and emu:read8(0x03000f9c)==0 and emu:read32(0x03003264)==0x08056541 then cmd.frames=qa.timer+1 end
end)
