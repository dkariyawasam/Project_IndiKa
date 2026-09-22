local f=io.open('/tmp/kanto-release-september22/battle-state.txt','w')
f:write('auto='..tostring(qa.autoBattle)..' heal='..tostring(qa.healActor)..'\n')
for i=0,3 do local p=qa.bsym.mons+i*88
f:write(i..' species='..emu:read16(p)..' HP='..emu:read16(p+40)..'/'..emu:read16(p+44)..' ctrl='..string.format('%x',emu:read32(qa.bsym.controllers+i*4))..'\n') end
f:close()
