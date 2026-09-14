qa.entryPhase='start';qa.warp(13,0,4,4)
qa.run({{kind='wait',frames=300},{kind='map',group=1,map=72,key=64},{kind='wait',frames=300},{kind='walk',x=6,y=9},{kind='capture',name='fresh-entry-after-loss'}})
qa.entryWatcher=callbacks:add('frame',function()
 if qa.queue[qa.step] then return end
 local g,m,x,y=qa.state()
 if qa.entryPhase=='start' then
  qa.result('fresh-entry-walkable',g==1 and m==72 and y==9);qa.entryPhase='gate';qa.warp(1,72,7,3)
  qa.run({{kind='wait',frames=300},{kind='press',key=32,hold=12,frames=30},{kind='wait',frames=90},{kind='capture',name='fresh-door-locked'},{kind='text',frames=180}})
 elseif qa.entryPhase=='gate' then
  qa.result('champion-locked-after-reset',g==1 and m==72);qa.entryPhase='exit';qa.warp(1,72,6,10)
  qa.run({{kind='wait',frames=300},{kind='map',group=13,map=0,key=128},{kind='wait',frames=300},{kind='capture',name='safe-lobby-exit'}})
 elseif qa.entryPhase=='exit' then
  qa.result('exit-destination',g==13 and m==0 and x==4 and y==2,string.format('%d,%d at %d,%d',g,m,x,y));qa.entryPhase='done'
 end
end)
