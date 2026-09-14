dofile("/tmp/gym-trainers/harness.lua")
dofile("/tmp/gym-trainers/helpers.lua")
dofile("/tmp/gym-trainers/common.lua")
dofile("/tmp/apex-story-playthrough/characters.lua")
dofile("/tmp/gym-trainers/logger.lua")
qa.run({{kind='press',key=8,frames=30},{kind='text',frames=24000}})
qa.startWatcher=callbacks:add('frame',function()
 if emu:read32(0x03003264)==0x08056541 and emu:read8(0x03000f9c)==0 then
  callbacks:remove(qa.startWatcher)
  qa.setVar(0x4090,4)
  qa.warp(11,3,7,9);qa.run({{kind='wait',frames=600},{kind='capture',name='fuchsia-trainers-final'}})
 end
end)
