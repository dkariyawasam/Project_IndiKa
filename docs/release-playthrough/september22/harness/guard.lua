if qa.guard then callbacks:remove(qa.guard) end
qa.guard=callbacks:add('frame',function()
 if not qa.autoBattle or emu:read32(qa.sym.gMain+4)~=qa.bsym.main+1 or emu:read32(qa.sym.gBattleTypeFlags)%16<8 then return end
 local action=false
 for _,i in ipairs({0,2}) do if emu:read32(qa.bsym.controllers+i*4)==qa.bsym.action+1 then action=true end end
 if not action then return end
 for _,i in ipairs({0,2}) do
  local p=qa.bsym.mons+i*88;local hp=emu:read16(p+40);local max=emu:read16(p+44)
  if hp>0 and hp*2<=max then qa.autoBattle=false;qa.healActor=i;emu:setKeys(0);qa.snapshot();return end
 end
end)
