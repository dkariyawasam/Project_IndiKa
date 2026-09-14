qa.dialogueCount=0
qa.dialogueLogger=callbacks:add('frame',function()
 if qa.frame%6~=0 then return end
 local out={}
 for i=0,999 do
  local b=emu:read8(0x02021de4+i)
  if b==255 then break end
  if b==254 or b==250 or b==251 then table.insert(out,' / ')
  else table.insert(out,qa.chars[b] or '') end
 end
 local msg=table.concat(out):gsub(' +$','')
 if #msg>8 and msg~=qa.lastMessage then
  qa.lastMessage=msg;qa.dialogueCount=qa.dialogueCount+1
  local g,m,x,y=qa.state()
  local f=io.open('/Users/deekariyawasam/pokefirered/docs/gym-story-fixes/dialogue-log.tsv','a')
  f:write(string.format('%d\t%d\t%d,%d\t%d,%d\t%s\n',qa.dialogueCount,qa.frame,g,m,x,y,msg));f:close()
 end
end)
