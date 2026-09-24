if qa.battleTextLog then callbacks:remove(qa.battleTextLog) end
qa.battleTextLog=callbacks:add('frame',function()
 if qa.frame%12~=0 or emu:read32(qa.sym.gMain+4)~=qa.bsym.main+1 then return end
 local t={};for i=0,299 do local c=emu:read8(0x2022a50+i);if c==255 then break end;t[#t+1]=qa.chars[c] or (c>=250 and '|' or '?') end
 local s=table.concat(t);if s~=qa.lastBattleText then local f=io.open('/tmp/kanto-release-september22/battle-text.txt','a');f:write(s..'\n');f:close();qa.lastBattleText=s end
end)
