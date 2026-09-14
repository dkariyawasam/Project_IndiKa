function qa.script(bytes)
 for i,b in ipairs(bytes) do emu:write8(0x0203fc00+i-1,b) end
 for i=0,115 do emu:write8(0x03000eb0+i,0) end
 emu:write8(0x03000eb1,1);emu:write32(0x03000eb8,0x0203fc00)
 emu:write32(0x03000f0c,0x08141e50);emu:write32(0x03000f10,0x081421b8)
 emu:write8(0x03000ea8,0);emu:write8(0x03000f9c,1)
end
function qa.warp(g,m,x,y)
 qa.script({0x39,g,m,255,x%256,math.floor(x/256),y%256,math.floor(y/256),0x27,0x02})
end
function qa.var(id)
 return emu:read16(emu:read32(0x03005178)+0x1010+2*(id-0x4000))
end
function qa.flag(id)
 return math.floor(emu:read8(emu:read32(0x03005178)+0xeec+math.floor(id/8))/2^(id%8))%2
end
function qa.progress(name)
 local f=io.open('/Users/deekariyawasam/pokefirered/docs/gym-story-fixes/progress.tsv','a')
 local g,m,x,y=qa.state()
 f:write(string.format('%s\t%d,%d\t%d,%d\t%04x\t%04x\tTangrowth:%d/%d\n',name,g,m,x,y,qa.var(0x4098),qa.var(0x4099),qa.flag(0x2e2),qa.flag(0x2d4)));f:close()
end
qa.travelAssist=callbacks:add('frame',function()
 local c=qa.queue[qa.step]
 if c and c.kind=='walk' and (emu:read8(0x03000f9c)~=0 or emu:read32(0x03003264)~=0x08056541) then
  emu:setKeys(qa.frame%30<6 and 1 or 0);qa.timer=0
 end
end)
