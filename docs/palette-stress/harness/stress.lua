qa.runs={};qa.maximum={};qa.samples=0
qa.scan=callbacks:add('frame',function()
 if emu:read32(qa.sym.gMain+4)~=qa.sym.CB2_Overworld+1 then return end
 local c=qa.queue[qa.step];if not c or (c.kind=='wait' and qa.timer<120)then return end
 local g,m=qa.state();qa.samples=qa.samples+1
 for i=0,15 do
  local o=qa.sym.gObjectEvents+i*36;local sid=emu:read8(o+4);local sp=qa.sym.gSprites+sid*68
  if emu:read8(o)%2==1 and sid<64 and emu:read8(sp+62)%2==1 and math.floor(emu:read8(sp+62)/4)%2==0 then
   local gfx=emu:read8(o+5);local info=emu:read32(qa.sym.gObjectEventGraphicsInfoPointers+gfx*4)
   if emu:read8(info+12)%16==15 then
    local slot=math.floor(emu:read16(sp+4)/4096)%16;local tag=emu:read16(info+2);local actual=emu:read16(qa.sym.sSpritePaletteTags+slot*2)
    local key=g..','..m..':'..gfx..':'..emu:read8(o+8)
    qa.runs[key]=tag~=actual and (qa.runs[key] or 0)+1 or 0
    qa.maximum[key]=math.max(qa.maximum[key] or 0,qa.runs[key])
    if qa.runs[key]==60 then qa.capture('capacity-'..g..'-'..m..'-'..gfx)end
   end
  end
 end
end)
qa.stops={
 {'route9-east',3,19,52,22,16,32},
 {'route9-blackbelt',3,19,49,18,128,64},
 {'route9-west',3,19,25,22,32,16},
 {'route23-aces',3,35,10,53,128,64},
 {'route23-north',3,35,16,48,64,128},
 {'powerplant-electricians',1,87,17,19,16,32},
 {'powerplant-mixed',1,87,32,28,128,64},
 {'powerplant-scientist',1,87,39,17,128,64},
 {'route19-water',3,30,11,42,128,64},
 {'route19-water-south',3,30,13,48,16,32},
 {'route7-regression',3,17,12,60,128,64},
}
qa.visit=1;qa.round=0
qa.tour=callbacks:add('frame',function()
 if qa.queue[qa.step] then return end
 local c=qa.stops[qa.visit];if not c then
  if qa.round==0 then qa.round=1;qa.visit=1;c=qa.stops[1] else return end
 end
 qa.visit=qa.visit+1;qa.setVar(0x4029,qa.round);qa.warp(c[2],c[3],c[4],c[5]);local prefix=(qa.round==0 and 'day-' or 'night-')..c[1]
 qa.run({{kind='wait',frames=1200},{kind='capture',name=prefix..'-entry'},{kind='press',key=c[6],hold=120,frames=125},{kind='capture',name=prefix..'-out'},{kind='press',key=c[7],hold=120,frames=125},{kind='capture',name=prefix..'-back'},{kind='press',key=c[6],hold=120,frames=125},{kind='capture',name=prefix..'-repeat'}})
end)
