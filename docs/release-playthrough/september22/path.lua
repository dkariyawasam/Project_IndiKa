function qa.path(points)
 qa.run({});if qa.walker then callbacks:remove(qa.walker) end
 local t=0;local index=1;local stable=0;local og,om=qa.state()
 qa.walker=callbacks:add('frame',function()
  local g,m,x,y=qa.state();local p=points[index];qa.pathTarget=p
  if not p or g~=og or m~=om or t>3600 then emu:setKeys(0);callbacks:remove(qa.walker);qa.walker=nil;qa.snapshot();return end
  if emu:read8(qa.sym.sLockFieldControls)~=0 or emu:read32(qa.sym.gMain+4)~=qa.sym.CB2_Overworld+1 then return end
  t=t+1
  if x==p[1] and y==p[2] then emu:setKeys(0);stable=stable+1;if stable>=12 then index=index+1;stable=0 end
  elseif x~=p[1] then emu:setKeys(x<p[1] and 16 or 32)
  else emu:setKeys(y<p[2] and 128 or 64) end
 end)
end
