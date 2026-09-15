function qa.setFlag(f,v)
 local p=emu:read32(0x03005178)+0xeec+math.floor(f/8);local b=emu:read8(p);local mask=2^(f%8)
 if math.floor(b/mask)%2~=v then emu:write8(p,b+(v==1 and mask or -mask)) end
end
function qa.setVar(id,v) emu:write16(emu:read32(0x03005178)+0x1010+2*(id-0x4000),v) end
function qa.u32(n) return {n%256,math.floor(n/256)%256,math.floor(n/65536)%256,math.floor(n/16777216)} end
function qa.result(n,ok,detail)
 local f=io.open('/Users/deekariyawasam/pokefirered/docs/palette-stress/runtime-tests.tsv','a');f:write(n..'\t'..(ok and 'PASS' or 'FAIL')..'\t'..(detail or '')..'\n');f:close()
end
function qa.select(id)
 local g,m=qa.state()
 for i=0,15 do local p=0x02036f50+i*36
  if emu:read8(p)%2==1 and emu:read8(p+8)==id and emu:read8(p+9)==m and emu:read8(p+10)==g then emu:write8(0x030051e4,i);emu:write16(0x020371ea,id);return true end
 end;return false
end
