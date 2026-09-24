local g,m=qa.state();local a=0x30051b0;local w=emu:read32(a);local h=emu:read32(a+4);local p=emu:read32(a+8)
if w>15 and w<200 and h>14 and h<200 then
 local f=io.open('/tmp/kanto-release-september22/live-map.txt','w');f:write(g..' '..m..' '..(w-15)..' '..(h-14)..'\n')
 for y=7,h-8 do for x=7,w-9 do f:write(emu:read16(p+2*(y*w+x))..' ') end end
 f:close()
end
