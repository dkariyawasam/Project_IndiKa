qa.oldCapture=qa.capture
function qa.inspect(name)
 local f=io.open('/Users/deekariyawasam/pokefirered/docs/palette-stress/runtime-palettes.tsv','a')
 local g,m,x,y=qa.state()
 for i=0,15 do
  local o=qa.sym.gObjectEvents+i*36
  if emu:read8(o)%2==1 then
   local gfx=emu:read8(o+5);local sid=emu:read8(o+4)
   local info=emu:read32(qa.sym.gObjectEventGraphicsInfoPointers+gfx*4)
   local slot=math.floor(emu:read16(qa.sym.gSprites+sid*68+4)/4096)%16
   local configured=emu:read8(info+12)%16
   local tag=emu:read16(info+2);local actual=emu:read16(qa.sym.sSpritePaletteTags+slot*2)
   local verdict=configured~=15 and 'FIXED' or (tag==actual and 'PASS' or 'FAIL')
   f:write(string.format('%s\t%d,%d\t%d,%d\t%d\t%d\t%d\t%d\t%04x\t%04x\t%s\n',name,g,m,x,y,i,gfx,configured,slot,tag,actual,verdict))
  end
 end
 f:close()
end
function qa.capture(name) qa.inspect(name);qa.oldCapture(name) end
qa.noSight=callbacks:add('frame',function()
 emu:write8(qa.sym.sWildEncountersDisabled,1)
 for i=0,15 do emu:write8(qa.sym.gObjectEvents+i*36+7,0) end
end)
