local f=io.open('/tmp/kanto-release-september22/party.txt','w')
for i=0,emu:read8(qa.sym.gPlayerPartyCount)-1 do
 local p=qa.sym.gPlayerParty+100*i; local name={}
 for j=0,9 do local c=emu:read8(p+8+j);if c==255 then break end;table.insert(name,qa.chars[c] or '?') end
 f:write(table.concat(name)..' level '..emu:read8(p+84)..' HP '..emu:read16(p+86)..'/'..emu:read16(p+88)..'\n')
end
f:close()
