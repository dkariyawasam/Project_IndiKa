qa.tests={
{name='Erika-before-root',state=2,g=1,m=0,x=5,y=48,visible=false},
{name='Erika-root-one-mon',state=2,g=1,m=0,x=5,y=48,id=13,script='ViridianForest_EventScript_Erika',root=true,visible=true},
{name='Erika-return-before-battle',state=3,g=1,m=0,x=5,y=48,visible=true},
{name='Erika-after-victory',state=4,g=1,m=0,x=5,y=48,visible=false},
{name='Sabrina-tower-completion',state=3,g=1,m=85,x=11,y=9,id=4,script='PokemonTower_3F_EventScript_DefeatedSabrina',sub=5},
{name='Sabrina-gym-completion',state=4,g=14,m=3,x=14,y=12,id=1,script='SaffronCity_Gym_EventScript_DefeatedSabrina',sub=5},
{name='Giovanni-completion',g=5,m=1,x=2,y=3,id=1,script='ViridianCity_Gym_EventScript_DefeatedGiovanni',sub=7},
{name='Sabrina-existing-badge-repair',state=4,g=14,m=3,x=14,y=12,sub=5,repair=true},
{name='Giovanni-existing-badge-repair',g=5,m=1,x=2,y=3,sub=7,repair=true}
}
qa.testIndex=1;qa.testPhase=0
qa.testsRunner=callbacks:add('frame',function()
 if qa.queue[qa.step] then return end
 local c=qa.tests[qa.testIndex]
 if not c then callbacks:remove(qa.testsRunner);qa.testsRunner=nil;return end
 if qa.testPhase==0 then
  if c.sub then
   qa.setFlag(c.sub==5 and 2102 or 2104,c.repair and 1 or 0)
   local p=emu:read32(0x0300517c)+0xf42
   -- Start with all subquest records cleared in this disposable fixture.
   for i=0,47 do emu:write8(p+i,0) end
   qa.setVar(0x4091,c.state or 4);qa.setFlag(189,0);qa.setFlag(195,0);qa.setFlag(85,0)
  else qa.setVar(0x408f,c.state) end
  local b={}
  if c.root then
   for i=0,599 do emu:write8(0x02024348+i,0) end;emu:write8(0x020240ed,0)
   b={0x79,26,0,25,0,0,0,0,0,0,0,0,0,0,0,0x44,31,0,1,0}
  end
  for _,x in ipairs({0x39,c.g,c.m,255,c.x,0,c.y,0,0x27,0x02}) do table.insert(b,x) end
  qa.script(b);qa.run({{kind='wait',frames=600}});qa.testPhase=1
 elseif qa.testPhase==1 then
  if c.script then
   qa.select(c.id);local b={0x05};for _,x in ipairs(qa.u32(qa.symbols[c.script])) do table.insert(b,x) end
   qa.script(b);qa.run({{kind='text',frames=4000,untilFree=true}})
  end
  qa.testPhase=2
 elseif qa.testPhase==2 then
  if c.sub then qa.script({0xd6,9,1,0,c.sub,0,0x6c,0x02});qa.run({{kind='wait',frames=60}});qa.testPhase=3
  else
   local found=false;local x,y=-1,-1
   for i=0,15 do local p=0x02036f50+i*36
    if emu:read8(p)%2==1 and emu:read8(p+8)==14 then found=true;x=emu:read16(p+16)-7;y=emu:read16(p+18)-7 end
   end
   qa.result(c.name,found==c.visible and emu:read8(0x03000f9c)==0 and (not c.visible or (x==4 and y==48)),'Visible '..tostring(found)..'; position '..x..','..y..'; state '..qa.var(0x408f)..'; lock '..emu:read8(0x03000f9c))
   qa.capture(c.name);qa.testIndex=qa.testIndex+1;qa.testPhase=0
  end
 else
  qa.result(c.name,emu:read16(0x020371e8)==1,'Completion '..emu:read16(0x020371e8))
  qa.capture(c.name);qa.testIndex=qa.testIndex+1;qa.testPhase=0
 end
end)
