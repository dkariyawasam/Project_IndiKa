if qa.textAdvance then callbacks:remove(qa.textAdvance) end
qa.textAdvance=callbacks:add('frame',function()
 if qa.autoBattle or qa.step<=#qa.queue or emu:read32(qa.sym.gMain+4)~=qa.bsym.main+1 then return end
 local waiting=false
 for _,i in ipairs({0,1,2,3}) do local c=emu:read32(qa.bsym.controllers+i*4);if c==0x8030361 or c==0x8035ed1 then waiting=true end end
 emu:setKeys(waiting and qa.frame%60<8 and 1 or 0)
end)
