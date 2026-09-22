qa.baseSnapshot=qa.baseSnapshot or qa.snapshot
function qa.snapshot()
 qa.baseSnapshot()
 local g,m=qa.state();local f=io.open('/tmp/kanto-release-september22/live-objects.tsv','w')
 f:write(g..' '..m..'\n')
 for i=0,15 do
  local p=0x2036f50+i*36;local flags=emu:read32(p)
  if flags%2==1 and emu:read8(p+8)~=255 and emu:read8(p+9)==m and emu:read8(p+10)==g then
   f:write(emu:read8(p+8)..' '..(emu:read16(p+16)-7)..' '..(emu:read16(p+18)-7)..'\n')
  end
 end
 f:close()
end
