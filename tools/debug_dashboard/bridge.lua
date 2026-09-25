-- Generated bootstrap sets KANTO_CONFIG before loading this file.
-- Commands are numeric, allowlisted records; no command file is executed as Lua.
if kantoDebug then
    if kantoDebug.callback then callbacks:remove(kantoDebug.callback) end
    if kantoDebug.resetCallback then callbacks:remove(kantoDebug.resetCallback) end
end
local cfg = KANTO_CONFIG
local sym = cfg.symbols
local dir = cfg.directory
local function writeFile(name, data)
    local f = assert(io.open(dir..'/'..name..'.tmp','wb')); f:write(data); f:close()
    assert(os.rename(dir..'/'..name..'.tmp', dir..'/'..name))
end
local function hex(s) return (s:gsub('.',function(c) return string.format('%02x',string.byte(c)) end)) end
local function escape(s) return '"'..s:gsub('\\','\\\\'):gsub('"','\\"'):gsub('\n','\\n'):gsub('[%z\1-\31]',' ')..'"' end
local function encode(v)
    if type(v)=='string' then return escape(v) end
    if type(v)=='boolean' or type(v)=='number' then return tostring(v) end
    local a={}; for k,x in pairs(v) do a[#a+1]=escape(tostring(k))..':'..encode(x) end
    return '{'..table.concat(a,',')..'}'
end
-- Full ROM byte comparison once on attachment, rather than trusting a filename.
local function matches()
    if emu:romSize() ~= cfg.romSize then return false end
    local f=io.open(cfg.rom,'rb'); if not f then return false end
    for offset=0,cfg.romSize-1,32768 do
        local expected=f:read(32768)
        if emu:readRange(0x08000000+offset,#expected)~=expected then f:close();return false end
    end
    f:close();return true
end
local connected=matches()
local d={frame=0,last=0,pending=nil,connected=connected,sequence=0}
kantoDebug=d
local layout={}
if connected then for i=0,16 do layout[i]=emu:read32(sym.gDebugDashboardLayout+i*4) end end
if connected and (layout[0]~=0x4B444247 or layout[1]~=1) then connected=false end
local function valid(p,size) return p>=0x02000000 and p+size<=0x02040000 end
local function ready()
    if not connected then return false end
    local p=emu:read32(sym.gSaveBlock1Ptr)
    return valid(p,layout[2]) and emu:read32(sym.gMain+4)==sym.CB2_Overworld+1
        and emu:read8(sym.sLockFieldControls)==0 and emu:read8(sym.sGlobalScriptContextStatus)==2
end
local function ack(id,ok,message)
    writeFile('ack.json',encode({id=id,ok=ok,message=message,session=cfg.session}))
end
local function checkpoint(id)
    if not emu:saveStateFile(dir..'/checkpoint-'..id..'.ss',31) then error('Could not create restore checkpoint') end
    d.lastCheckpoint=id
end
local function snapshot()
    local s={session=cfg.session,frame=emu:currentFrame(),matched=connected,ready=ready(),party={},flags={},vars={},palettes={},checkpoint=d.lastCheckpoint or 0}
    if connected then
        local p=emu:read32(sym.gSaveBlock1Ptr); local p2=emu:read32(sym.gSaveBlock2Ptr)
        s.callback=emu:read32(sym.gMain+4);s.locked=emu:read8(sym.sLockFieldControls)~=0
        if valid(p,layout[2]) and valid(p2,layout[3]) then
            s.group=emu:read8(p+4);s.map=emu:read8(p+5);s.x=emu:read16(p);s.y=emu:read16(p+2)
            s.style=emu:read8(p2+layout[6]);s.accent=emu:read8(p2+layout[8]);s.secondary=emu:read8(p2+layout[9]);s.theme=math.floor(emu:read8(p2+0x14)/8)
            s.flagsHex=hex(emu:readRange(p+layout[4],math.ceil(layout[15]/8)))
            s.varsHex=hex(emu:readRange(p+layout[5],layout[16]*2))
            s.partyCount=math.min(6,emu:read8(sym.gPlayerPartyCount))
            for i=0,s.partyCount-1 do s.party[tostring(i)]=hex(emu:readRange(sym.gPlayerParty+layout[10]*i,layout[10])) end
            s.dialogue=hex(emu:readRange(sym.gStringVar4,1000))
        end
        s.paletteHex=hex(emu:readRange(0x05000000,1024))
    else s.error='ROM differs from this dashboard session. Load the session ROM and reload the bridge.' end
    writeFile('status.json',encode(s))
    emu:screenshot(dir..'/screen-next.png');os.rename(dir..'/screen-next.png',dir..'/screen.png')
end
local function command()
    if d.pending then return end
    local f=io.open(dir..'/command.tsv','rb');if not f then return end
    local line=f:read('*a');f:close();os.remove(dir..'/command.tsv')
    local fields={};for x in line:gmatch('[^\t\r\n]+') do fields[#fields+1]=x end
    if fields[1]~=cfg.session then return end
    local id=tonumber(fields[2]);local op=tonumber(fields[3]);if not id or id<=d.last then return end
    d.last=id
    if not connected then ack(id,false,'ROM mismatch');return end
    if not tonumber(fields[4]) or os.time()>tonumber(fields[4]) then ack(id,false,'Command expired before delivery');return end
    local a={};for i=5,#fields do a[#a+1]=tonumber(fields[i]) end
    if op==100 then
        emu:setKeys(a[1]);d.keyRelease=d.frame+math.min(120,a[2]);ack(id,true,'Input sent');return
    end
    if op==101 then checkpoint(id);ack(id,true,'Checkpoint saved');return end
    if op==102 then
        if not a[1] or not emu:loadStateFile(dir..'/checkpoint-'..a[1]..'.ss',31) then ack(id,false,'Checkpoint restore failed');return end
        emu:setKeys(0);emu:write32(sym.gDebugDashboardMailbox+4,0);ack(id,true,'Checkpoint restored');return
    end
    if op==103 then snapshot();ack(id,true,'Screenshot captured');return end
    if not ready() then ack(id,false,'Return to the overworld and finish movement, menus or dialogue first.');return end
    checkpoint(id)
    for i=0,5 do emu:write32(sym.gDebugDashboardMailbox+16+i*4,a[i+1] or 0) end
    emu:write32(sym.gDebugDashboardMailbox,0x4B444247)
    emu:write32(sym.gDebugDashboardMailbox+8,0)
    emu:write32(sym.gDebugDashboardMailbox+4,op)
    d.pending={id=id,start=d.frame,op=op}
end
local function frame()
    d.frame=d.frame+1
    if d.keyRelease and d.frame>=d.keyRelease then emu:setKeys(0);d.keyRelease=nil end
    if d.pending then
        local status=emu:read32(sym.gDebugDashboardMailbox+8)
        if status~=0 and emu:read32(sym.gDebugDashboardMailbox+4)==0 then
            snapshot()
            ack(d.pending.id,status==1,({[1]='Applied (scripts and battles may still be running)',[2]='Rejected: invalid value, full party or full bag',[3]='Game busy; wait until standing still'})[status] or 'Unknown result')
            d.pending=nil
        elseif d.frame-d.pending.start>180 then
            emu:write32(sym.gDebugDashboardMailbox+4,0);ack(d.pending.id,false,'Command timed out; cancelled');d.pending=nil
        end
    end
    if d.frame%15==0 then command();snapshot() end
end
d.callback=callbacks:add('frame',function()
    local ok,err=pcall(frame)
    if not ok then
        emu:setKeys(0)
        if d.pending then ack(d.pending.id,false,tostring(err));d.pending=nil end
        console:error(tostring(err))
    end
end)
-- Re-check after loading another game/reset, never use old addresses on a new ROM.
d.resetCallback=callbacks:add('reset',function() connected=matches();d.pending=nil;d.keyRelease=nil;emu:setKeys(0) end)
snapshot()
console:log('Expedition Kanto dashboard bridge: '..(connected and 'connected' or 'ROM mismatch'))
