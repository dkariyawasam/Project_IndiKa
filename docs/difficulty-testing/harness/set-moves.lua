function qa.xor(a,b)local n,p=0,1;for i=1,32 do if a%2~=b%2 then n=n+p end;a=math.floor(a/2);b=math.floor(b/2);p=p*2 end;return n end
local attacks={1,1,2,3,2,3,0,0,0,0,0,0,2,3,1,1,3,2,2,3,1,1,3,2}
function qa.setMoves(slot,moves)
 local p=qa.sym.gPlayerParty+slot*100;local per=emu:read32(p);local key=qa.xor(per,emu:read32(p+4));local d={}
 for j=0,11 do local word=qa.xor(emu:read32(p+32+j*4),key);for b=0,3 do d[j*4+b+1]=math.floor(word/256^b)%256 end end
 local a=attacks[per%24+1]*12
 for j,m in ipairs(moves)do d[a+(j-1)*2+1]=m%256;d[a+(j-1)*2+2]=math.floor(m/256);d[a+8+j]=emu:read8(qa.sym.gBattleMoves+m*12+4)end
 local checksum=0;for j=1,48,2 do checksum=checksum+d[j]+256*d[j+1]end;emu:write16(p+28,checksum%65536)
 for j=0,11 do local word=0;for b=0,3 do word=word+d[j*4+b+1]*256^b end;emu:write32(p+32+j*4,qa.xor(word,key))end
end
