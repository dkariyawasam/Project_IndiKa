qa.transientCount=0
qa.trace=callbacks:add('frame',function()
 if emu:read32(qa.sym.gMain+4)~=qa.sym.CB2_Overworld+1 then return end
 local g,m,x,y=qa.state();if g~=3 or m~=17 then return end
 for i=0,15 do local o=qa.sym.gObjectEvents+i*36
  if emu:read8(o)%2==1 and emu:read8(o+5)==22 then
   local sp=qa.sym.gSprites+emu:read8(o+4)*68;local slot=math.floor(emu:read16(sp+4)/4096)%16
   if emu:read16(qa.sym.sSpritePaletteTags+slot*2)~=0x1139 and math.floor(emu:read8(sp+62)/4)%2==0 then
    qa.transientCount=qa.transientCount+1
    if qa.transientCount<=20 then emu:screenshot('/Users/deekariyawasam/pokefirered/docs/palette-stress/transient-'..qa.transientCount..'.png')end
    local f=io.open('/Users/deekariyawasam/pokefirered/docs/palette-stress/transient-bounds.tsv','a');local function signed(v) return v>=32768 and v-65536 or v end
    local cy=emu:read8(sp+41);if cy>=128 then cy=cy-256 end
    local top=signed(emu:read16(sp+34))+signed(emu:read16(sp+38))+cy+signed(emu:read16(qa.sym.gSpriteCoordOffsetY))
    f:write(qa.frame..'\t'..x..','..y..'\t'..slot..'\t'..emu:read8(o+1)..'\t'..top..'\t'..(top+32)..'\n');f:close()
   end
  end
 end
end)
qa.setVar(0x4029,0);qa.setVar(0x402a,0);qa.warp(3,17,12,60)
qa.run({{kind='wait',frames=1200},{kind='press',key=128,hold=120,frames=125},{kind='press',key=64,hold=120,frames=125},{kind='press',key=128,hold=120,frames=125},{kind='press',key=64,hold=120,frames=125},{kind='press',key=128,hold=120,frames=125},{kind='press',key=64,hold=120,frames=125},{kind='capture',name='route7-transient-recheck-settled'}})
