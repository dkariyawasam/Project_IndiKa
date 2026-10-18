#!/usr/bin/env python3
"""Local, dependency-free dashboard server for Expedition Kanto and mGBA."""
import argparse
import hashlib
import ipaddress
from http.cookies import SimpleCookie
import json
import mimetypes
import os
import secrets
import shutil
import struct
import subprocess
import sys
import threading
import time
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import urlsplit, parse_qs
from catalog import ROOT, TOOLCHAIN, build_catalog, decode_party, load_symbols

HERE = Path(__file__).resolve().parent
AUDITS = {
    'accent': ('Player colours & save compatibility', 'tools/test_player_accent.py', True),
    'player-sheets': ('Player overworld tile indices', 'tools/test_player_sprite_palettes.py', True),
    'palette-life': ('Object palette lifetime', 'tools/test_object_palette_lifetime.py', True),
    'palette-recovery': ('Object palette recovery', 'tools/test_object_palette_recovery.py', True),
    'radial': ('Radial menu palettes', 'tools/test_radial_menu_palettes.py', True),
    'dex': ('Active Pokédex', 'tools/test_active_pokedex.py', True),
    'eevee': ('Eevee evolution rules', 'tools/test_eevee_evolution.py', True),
    'rage-fist': ('Rage Fist', 'tools/test_rage_fist.py', True),
    'wiring': ('Trainer wiring', 'tools/audit_trainer_wiring.py', False),
    'sprites': ('Active Pokémon sprites', 'tools/audit_active_pokemon_sprites.py', False),
    'palettes': ('Overworld palette demand', 'tools/audit_overworld_palettes.py', False),
    'levels': ('Wild evolution levels', 'tools/audit_wild_evolution_levels.py', False),
    'sizes': ('Pokédex size chart', 'tools/audit_pokedex_size_chart.py', False),
    'footprints': ('Pokémon footprints', 'tools/audit_pokemon_footprints.py', False),
    'evolutions': ('Evolution availability', 'tools/audit_evolution_availability.py', False),
    'habitats': ('Trainer habitat rules', 'tools/audit_trainer_habitat_rules.py', False),
    'rosters': ('Trainer rosters', 'tools/audit_trainer_rosters.py', False),
    'heatmap': ('Trainer class heatmap', 'tools/audit_trainer_class_heatmap.py', False),
}
REQUIRED = ['gDebugDashboardMailbox', 'gDebugDashboardLayout', 'gSaveBlock1Ptr', 'gSaveBlock2Ptr',
            'gPlayerParty', 'gPlayerPartyCount', 'gStringVar4', 'gMain', 'CB2_Overworld',
            'sLockFieldControls', 'sGlobalScriptContextStatus']


def atomic(path, text):
    tmp = path.with_suffix(path.suffix+'.tmp')
    tmp.write_text(text)
    tmp.replace(path)


def lua(value):
    if isinstance(value, dict): return '{'+','.join('['+lua(k)+']='+lua(v) for k,v in value.items())+'}'
    if isinstance(value, str): return json.dumps(value,ensure_ascii=False)
    if isinstance(value, bool): return 'true' if value else 'false'
    if isinstance(value, (int,float)): return str(value)
    raise TypeError(type(value))


def prepare(base, copy_save=True):
    rom, elf = ROOT/'pokefirered.gba', ROOT/'pokefirered.elf'
    symbols = load_symbols(elf)
    missing = [s for s in REQUIRED if s not in symbols]
    if missing: raise RuntimeError('Build the main ROM first; missing symbols: '+', '.join(missing))
    session = time.strftime('%Y%m%d-%H%M%S')+'-'+secrets.token_hex(3)
    directory = base/session; directory.mkdir(parents=True,mode=0o700)
    # Prove ELF and ROM agree before trusting any address.
    verify = directory/'verify.bin'
    subprocess.run([str(TOOLCHAIN/'arm-none-eabi-objcopy'),'-O','binary','--gap-fill','0xFF',str(elf),str(verify)],check=True)
    binary, rom_bytes = verify.read_bytes(), rom.read_bytes();verify.unlink()
    if rom_bytes[:len(binary)] != binary: raise RuntimeError('ROM and ELF differ. Rebuild before starting the dashboard.')
    session_rom = directory/'expedition-debug.gba';session_rom.write_bytes(rom_bytes)
    shutil.copy2(elf,directory/'expedition-debug.elf')
    save = ROOT/'pokefirered.sav'
    if copy_save and save.exists():
        shutil.copy2(save,directory/'expedition-debug.sav')
        shutil.copy2(save,directory/'original-save.sav')
    config = dict(directory=str(directory),session=session,rom=str(session_rom),romSize=len(rom_bytes),
                  symbols={s:symbols[s] for s in REQUIRED})
    bootstrap = directory/'connect.lua'
    bootstrap.write_text('KANTO_CONFIG = '+lua(config)+'\ndofile('+lua(str(HERE/'bridge.lua'))+')\n')
    return directory, bootstrap, hashlib.sha256(rom_bytes).hexdigest()


def resume_session(base):
    """Reuse the last test save and bridge only while its ROM and ELF still match."""
    try:
        current=json.loads((base/'current.json').read_text())
        directory=Path(current['directory']).resolve()
        if directory.parent != base.resolve(): return None
        for name in ('gba','elf'):
            if hashlib.sha256((directory/('expedition-debug.'+name)).read_bytes()).digest() != hashlib.sha256((ROOT/('pokefirered.'+name)).read_bytes()).digest(): return None
        bootstrap=directory/'connect.lua'
        if not bootstrap.is_file(): return None
        return directory,bootstrap,hashlib.sha256((directory/'expedition-debug.gba').read_bytes()).hexdigest()
    except (OSError,ValueError,KeyError): return None


class Dashboard:
    def __init__(self, directory, bootstrap, digest):
        self.directory, self.bootstrap, self.digest = directory, bootstrap, digest
        self.catalog = build_catalog()
        self.token = secrets.token_urlsafe(32)
        self.lock = threading.Lock(); self.sequence = time.time_ns()//1000000; self.pending = None
        self.atlas_renderer = None
        self.emulator_process = None
        self.events = []; self.jobs = {}; self.port = None
        self.python = sys.executable
        candidate = Path('/tmp/frigibas-imagegen-venv/bin/python')
        if candidate.exists(): self.python = str(candidate)

    def connect_emulator(self):
        from emulator import launch
        with self.lock:
            if self.raw_state().get('connected'):
                return dict(message='Already connected to the matching game.')
            if self.emulator_process and self.emulator_process.poll() is None:
                return dict(message='The test emulator is already running. Unpause it, or load the bridge from the connection guide if required.')
            self.emulator_process,result=launch(self.directory,self.bootstrap)
            self.event('Launched test emulator',automatic=result['automatic'])
            return result

    def atlas(self):
        from atlas import AtlasRenderer
        with self.lock:
            if self.atlas_renderer is None:
                self.atlas_renderer = AtlasRenderer(self.directory,self.catalog)
        return self.atlas_renderer

    def event(self, message, **extra):
        entry=dict(time=time.time(),message=message,**extra)
        self.events.append(entry);self.events=self.events[-100:]
        with (self.directory/'actions.jsonl').open('a') as f: f.write(json.dumps(entry)+'\n')

    def raw_state(self):
        path=self.directory/'status.json'
        try:
            value=json.loads(path.read_text())
            if value.get('session') != self.directory.name: return {}
            value['age']=time.time()-path.stat().st_mtime
            value['connected']=value['age']<3 and value.get('matched',False)
            return value
        except (OSError,ValueError): return {}

    def state(self):
        value=self.raw_state();value.setdefault('connected',False)
        value['mapName']=next((m['name'] for m in self.catalog['maps'] if (m['group'],m['number'])==(value.get('group'),value.get('map'))),'Not in field')
        parties=value.pop('party',{})
        value['party']=[]
        for _,record in sorted(parties.items()):
            try: value['party'].append(decode_party(record,self.catalog))
            except (ValueError,KeyError,struct.error): pass
        text=bytes.fromhex(value.pop('dialogue','')).split(b'\xff')[0]
        value['dialogue']=''.join(self.catalog['chars'].get(c,'\n' if c>=250 else '') for c in text)
        flags=bytes.fromhex(value.pop('flagsHex',''));variables=bytes.fromhex(value.pop('varsHex',''))
        value['flags']={str(f['id']):bool(flags[f['id']//8]&(1<<(f['id']%8))) for f in self.catalog['flags'] if f['id']//8<len(flags)}
        value['vars']={str(v['id']):struct.unpack_from('<H',variables,(v['id']-0x4000)*2)[0] for v in self.catalog['variables'] if (v['id']-0x4000)*2+2<=len(variables)}
        with self.lock:
            if self.pending:
                try: ack=json.loads((self.directory/'ack.json').read_text())
                except (OSError,ValueError): ack={}
                if ack.get('session')==self.directory.name and ack.get('id')==self.pending['id']:
                    self.event(ack['message'],ok=ack['ok'],action=self.pending['action'],id=ack['id']);self.pending=None
                elif time.time()-self.pending['time']>8:
                    # Remove undelivered commands. A running script may already have applied.
                    (self.directory/'command.tsv').unlink(missing_ok=True)
                    self.event('No acknowledgement. Check emulator state before retrying.',ok=False,id=self.pending['id']);self.pending=None
            value['pending']=self.pending
        value['events']=self.events[-20:]
        value['jobs']=self.jobs
        value['checkpoints']=sorted(int(p.stem.split('-')[-1]) for p in self.directory.glob('checkpoint-*.ss'))
        value['evidence']=[p.name for p in sorted((self.directory/'evidence').glob('*/report.md'))] if (self.directory/'evidence').exists() else []
        return value

    @staticmethod
    def integer(data, name, low, high):
        n=data.get(name)
        if type(n) is not int or not low<=n<=high: raise ValueError(f'{name} must be between {low} and {high}')
        return n

    def command(self, data):
        action=data.get('action');a=[];catalog=self.catalog
        n=lambda key,lo,hi:self.integer(data,key,lo,hi)
        ids=lambda key:{x['id'] for x in catalog[key]}
        if action=='warp':
            maps=[m for m in catalog['maps'] if m['name']==data.get('map')]
            if not maps: raise ValueError('Choose a catalog map')
            m=maps[0];a=[m['group'],m['number'],n('x',0,m['width']-1),n('y',0,m['height']-1)];op=1
        elif action in ('pokemon','wild'):
            species=n('species',1,65535)
            if species not in ids('species'): raise ValueError('Choose an active species')
            a=[species,n('level',1,100)];op=2 if action=='pokemon' else 7
        elif action=='heal': op=3
        elif action=='flag':
            ident=n('id',1,65535)
            if ident not in ids('flags'): raise ValueError('Choose a known flag')
            a=[ident,n('value',0,1)];op=4
        elif action=='variable':
            ident=n('id',0x4000,0x4fff)
            if ident not in ids('variables'): raise ValueError('Choose a known variable')
            a=[ident,n('value',0,65535)];op=5
        elif action=='item':
            ident=n('item',1,65535)
            if ident not in ids('items'): raise ValueError('Choose a known item')
            a=[ident,n('quantity',1,99)];op=6
        elif action=='trainer':
            ident=n('trainer',1,65535)
            trainer=next((t for t in catalog['trainers'] if t['id']==ident),None)
            if not trainer: raise ValueError('Choose a defined trainer')
            a=[ident,int(trainer['double'])];op=8
        elif action=='appearance': a=[n('style',0,1),n('accent',0,2),n('secondary',0,2)];op=9
        elif action=='move':
            move=n('move',1,65535)
            if move not in ids('moves'): raise ValueError('Choose a known move')
            a=[n('slot',0,5),n('moveSlot',0,3),move];op=10
        elif action=='friendship': a=[n('slot',0,5),n('value',0,255)];op=11
        elif action=='money': a=[n('value',0,999999)];op=12
        elif action=='input':
            key=data.get('key');keys=dict(A=1,B=2,SELECT=4,START=8,RIGHT=16,LEFT=32,UP=64,DOWN=128,R=256,L=512)
            if key not in keys: raise ValueError('Unknown button')
            a=[keys[key],n('frames',1,120)];op=100
        elif action=='checkpoint': op=101
        elif action=='restore':
            ident=n('checkpoint',1,2**31-1)
            if not (self.directory/f'checkpoint-{ident}.ss').exists(): raise ValueError('Checkpoint not found in this session')
            a=[ident];op=102
        elif action=='capture': op=103
        else: raise ValueError('Unknown action')
        state=self.raw_state()
        if not state.get('connected'): raise ValueError('Connect the matching ROM in mGBA first (and unpause it).')
        if op<100 and not state.get('ready'): raise ValueError('Return to the overworld and stand still before changing game data.')
        with self.lock:
            if self.pending: raise ValueError('Wait for the current command to finish')
            self.sequence+=1
            queued=dict(id=self.sequence,action=action,time=time.time())
            self.pending=queued
            atomic(self.directory/'command.tsv','\t'.join(map(str,[self.directory.name,self.sequence,op,int(time.time())+5]+a)))
            self.event('Queued '+action,id=self.sequence,args=a)
        return queued

    def audit(self, name):
        if name not in AUDITS: raise ValueError('Unknown audit')
        if any(x['status']=='running' for x in self.jobs.values()): raise ValueError('An audit is already running')
        title,path,is_test=AUDITS[name]
        self.jobs[name]=dict(title=title,status='running',output='')
        def work():
            try:
                p=subprocess.run([self.python,str(ROOT/path)],cwd=ROOT,capture_output=True,text=True,timeout=120,
                                 env={**os.environ,'DEVKITARM':str(TOOLCHAIN.parent)})
                output=(p.stdout+'\n'+p.stderr)[-30000:]
                status='passed' if p.returncode==0 and is_test else 'report ready' if p.returncode==0 else 'needs attention'
            except (OSError,subprocess.TimeoutExpired) as e: output=str(e);status='failed'
            (self.directory/f'audit-{name}.txt').write_text(output)
            self.jobs[name]=dict(title=title,status=status,output=output)
        threading.Thread(target=work,daemon=True).start()
        return self.jobs[name]

    def evidence(self, note):
        if not isinstance(note,str) or len(note)>4000: raise ValueError('Keep the note under 4,000 characters')
        state=self.state()
        if not state.get('connected'): raise ValueError('Connect the emulator before capturing evidence')
        folder=self.directory/'evidence'/(time.strftime('%Y%m%d-%H%M%S')+'-'+secrets.token_hex(2));folder.mkdir(parents=True)
        shutil.copy2(self.directory/'screen.png',folder/'screen.png')
        (folder/'state.json').write_text(json.dumps(state,indent=2))
        (folder/'report.md').write_text(f'# Expedition Kanto debug finding\n\n{note}\n\nROM SHA-256: `{self.digest}`\n\nMap: {state["mapName"]} ({state.get("x")}, {state.get("y")})\n\n![Game screen](screen.png)\n')
        self.event('Evidence saved: '+str(folder))
        return dict(path=str(folder))


class Handler(BaseHTTPRequestHandler):
    def log_message(self,*args): pass
    @property
    def app(self): return self.server.app
    def send(self,code,data,mime='application/json'):
        if mime=='application/json': data=json.dumps(data).encode()
        self.send_response(code);self.send_header('Content-Type',mime);self.send_header('Content-Length',str(len(data)))
        self.send_header('Cache-Control','no-store');self.send_header('X-Content-Type-Options','nosniff')
        self.send_header('Content-Security-Policy',"default-src 'self'; img-src 'self' data:; style-src 'self' 'unsafe-inline'; frame-ancestors 'none'")
        self.end_headers();self.wfile.write(data)
    def paired(self):
        if self.client_address[0] in ('127.0.0.1', '::1'):
            return True
        cookie = SimpleCookie()
        try:
            cookie.load(self.headers.get('Cookie', ''))
            value = cookie.get('expedition_phone')
            return bool(value and secrets.compare_digest(value.value, self.server.phone_key))
        except Exception:
            return False

    def local(self):
        allowed={f'127.0.0.1:{self.server.server_port}',f'localhost:{self.server.server_port}'}
        if getattr(self.server, 'lan_host', None):
            allowed.add(f'{self.server.lan_host}:{self.server.server_port}')
        if self.headers.get('Host') not in allowed: return False
        origin=self.headers.get('Origin')
        return not origin or origin in {'http://'+x for x in allowed}
    def do_GET(self):
        if not self.local(): self.send(403,dict(error='Local requests only'));return
        path=urlsplit(self.path).path
        public = {'/', '/manifest.webmanifest', '/app-icon.png'}
        if not self.paired() and path not in public:
            self.send(401, dict(error='Pair this iPhone with the Mac dashboard first.'));return
        if path == '/' and not self.paired():
            self.send(200, (HERE/'static/phone-pair.html').read_bytes(), 'text/html; charset=utf-8');return
        try:
            if path in ('/manifest.webmanifest', '/app-icon.png'):
                mime = {'/manifest.webmanifest':'application/manifest+json', '/app-icon.png':'image/png'}[path]
                self.send(200, (HERE/'static'/path[1:]).read_bytes(), mime)
            elif path=='/api/bootstrap':
                self.send(200,dict(catalog=self.app.catalog,token=self.app.token,rom=str(self.app.directory/'expedition-debug.gba'),
                                  connect='dofile('+lua(str(self.app.bootstrap))+')',session=str(self.app.directory),sha=self.app.digest,
                                  audits=[dict(id=k,title=v[0],test=v[2]) for k,v in AUDITS.items()]))
            elif path in ('/api/pokedex','/pokemon-preview.png'):
                from pokedex import catalog, sprite
                dex=catalog(ROOT,self.app.catalog['species'])
                if path=='/api/pokedex': self.send(200,dex)
                else:
                    query=parse_qs(urlsplit(self.path).query)
                    key=query.get('species',[''])[0];view=query.get('view',[''])[0]
                    entry=next((e for e in dex['species'] if e['key']==key),None)
                    if entry is None or view not in ('front','back'):
                        self.send(404,dict(error='Unknown species or view'));return
                    self.send(200,sprite(ROOT,entry,view,query.get('shiny',['0'])[0]=='1'),'image/png')
            elif path in ('/api/pokemon-details','/pokemon-cry.wav','/pokemon-footprint.png'):
                from pokedex import catalog, details
                key=parse_qs(urlsplit(self.path).query).get('species',[''])[0]
                entry=next((e for e in catalog(ROOT,self.app.catalog['species'])['species'] if e['key']==key),None)
                if entry is None: raise ValueError('Unknown species')
                info=details(ROOT,entry)
                if path=='/api/pokemon-details': self.send(200,info)
                else:
                    asset=info['cry' if path.endswith('.wav') else 'footprint']
                    if not asset or not (ROOT/asset).is_file():
                        self.send(404,dict(error='Asset unavailable'));return
                    self.send(200,(ROOT/asset).read_bytes(),'audio/wav' if path.endswith('.wav') else 'image/png')
            elif path=='/item-icon.png':
                from items import icon
                key=parse_qs(urlsplit(self.path).query).get('item',[''])[0]
                self.send(200,icon(ROOT,key),'image/png')
            elif path=='/api/encounters':
                from planning import encounters
                self.send(200,encounters(ROOT))
            elif path=='/api/palette-style':
                variant=parse_qs(urlsplit(self.path).query).get('variant',['normal'])[0]
                if variant not in ('normal','shiny'): raise ValueError('Unknown palette variant')
                result=subprocess.run([self.app.python,str(ROOT/'tools/debug_dashboard/palette_style.py')],input=json.dumps(dict(species=self.app.catalog['species'],variant=variant)),capture_output=True,text=True,timeout=60,cwd=ROOT)
                if result.returncode: raise ValueError('Palette checker failed: '+result.stderr[-1000:])
                self.send(200,json.loads(result.stdout))
            elif path=='/api/assets':
                from planning import assets
                self.send(200,assets(ROOT,self.app.catalog['species']))
            elif path=='/api/trainers':
                from trainers import catalog
                self.send(200,catalog(ROOT))
            elif path=='/trainer-portrait.png':
                from trainers import portrait
                self.send(200,portrait(ROOT,parse_qs(urlsplit(self.path).query).get('trainer',[''])[0]),'image/png')
            elif path=='/api/rivals':
                from rivals import catalog
                self.send(200,catalog(ROOT))
            elif path=='/api/quests':
                from quests import catalog
                self.send(200,catalog(ROOT))
            elif path=='/api/items':
                from items import catalog
                self.send(200,catalog(ROOT))
            elif path=='/api/overworlds':
                from overworlds import catalog
                self.send(200, catalog(ROOT))
            elif path=='/overworld-sheet.png':
                from overworlds import transparent_png
                name=parse_qs(urlsplit(self.path).query).get('path',[''])[0]
                target=(ROOT/name).resolve()
                allowed=(ROOT/'graphics/object_events/pics').resolve()
                if not target.is_relative_to(allowed) or target.suffix!='.png':
                    self.send(404,dict(error='Unknown sheet'));return
                self.send(200,transparent_png(target),'image/png')
            elif path=='/api/atlas': self.send(200,self.app.atlas().manifest)
            elif path=='/map-art.png':
                name=parse_qs(urlsplit(self.path).query).get('name',[''])[0]
                if name not in {m['name'] for m in self.app.catalog['maps']}:
                    self.send(404,dict(error='Unknown map'));return
                self.send(200,self.app.atlas().render(name),'image/png')
            elif path=='/api/state': self.send(200,self.app.state())
            elif path=='/screen.png': self.send(200,(self.app.directory/'screen.png').read_bytes(),'image/png')
            elif path=='/pixel.ttf':
                self.send(200,(ROOT/'tools/debug_dashboard/static/pixel.ttf').read_bytes(),'font/ttf')
            elif path in ('/','/app.mjs','/trainers.mjs','/planning.mjs','/palette-style.mjs','/rivals.mjs','/quests.mjs','/atlas.mjs','/overworlds.mjs','/pokedex.mjs','/items.mjs','/style.css'):
                file=HERE/'static'/('index.html' if path=='/' else path[1:])
                self.send(200,file.read_bytes(),{'/':'text/html; charset=utf-8','/app.mjs':'text/javascript; charset=utf-8','/trainers.mjs':'text/javascript; charset=utf-8','/planning.mjs':'text/javascript; charset=utf-8','/palette-style.mjs':'text/javascript; charset=utf-8','/rivals.mjs':'text/javascript; charset=utf-8','/quests.mjs':'text/javascript; charset=utf-8','/atlas.mjs':'text/javascript; charset=utf-8','/overworlds.mjs':'text/javascript; charset=utf-8','/items.mjs':'text/javascript; charset=utf-8','/pokedex.mjs':'text/javascript; charset=utf-8','/style.css':'text/css; charset=utf-8'}[path])
            else: self.send(404,dict(error='Not found'))
        except FileNotFoundError: self.send(404,dict(error='Not available yet'))
        except Exception as e: self.send(500,dict(error=str(e)))
    def do_POST(self):
        if urlsplit(self.path).path == '/pair':
            if not self.local():
                self.send(403, dict(error='Invalid dashboard origin'));return
            try:
                length = int(self.headers.get('Content-Length', '0'))
                if not 0 < length <= 256: raise ValueError()
                code = ''.join(parse_qs(self.rfile.read(length).decode()).get('code', [''])[0].split()).lower()
                if not secrets.compare_digest(code, self.server.phone_key):
                    self.send(403, b'Pairing code incorrect. Go back and use the code shown on your Mac.', 'text/plain');return
                self.send_response(303)
                self.send_header('Location', '/')
                self.send_header('Set-Cookie', 'expedition_phone='+self.server.phone_key+'; HttpOnly; SameSite=Strict; Path=/; Max-Age=2592000')
                self.send_header('Cache-Control', 'no-store')
                self.send_header('Content-Length', '0')
                self.end_headers()
            except (ValueError, UnicodeError):
                self.send(400, dict(error='Invalid pairing request'))
            return
        if not self.local() or not self.paired() or self.headers.get('X-Debug-Token') != self.app.token:
            self.send(403,dict(error='Invalid local session'));return
        try:
            length=int(self.headers.get('Content-Length','0'))
            if not 0<length<=8192: raise ValueError('Invalid request size')
            data=json.loads(self.rfile.read(length))
            if not isinstance(data,dict): raise ValueError('Expected an object')
            path=urlsplit(self.path).path
            if path=='/api/command': result=self.app.command(data)
            elif path in ('/api/palette-style/proposal','/api/palette-style/apply'):
                payload=dict(data,species=self.app.catalog['species'],action='apply' if path.endswith('/apply') else 'proposal')
                with self.app.lock:
                    process=subprocess.run([self.app.python,str(ROOT/'tools/debug_dashboard/palette_style.py')],input=json.dumps(payload),capture_output=True,text=True,timeout=60,cwd=ROOT)
                    if process.returncode: raise ValueError('Palette operation failed: '+process.stderr[-1000:])
                    result=json.loads(process.stdout)
                    if 'error' in result: raise ValueError(result['error'])
                if path.endswith('/apply'): self.app.event('Approved Pokémon palette',species=data.get('key'))
            elif path=='/api/encounters/save':
                from planning import save_encounters
                with self.app.lock:
                    result=save_encounters(ROOT,self.app.catalog['species'],data)
                self.app.event('Saved encounter table',table=data.get('id'))
            elif path=='/api/trainers/team':
                from trainers import save
                with self.app.lock:
                    result=save(ROOT,data)
                self.app.event('Saved trainer team',trainer=data.get('trainer'))
            elif path=='/api/pokedex/offsets':
                from pokedex import save_offsets
                with self.app.lock:
                    result=save_offsets(ROOT,self.app.catalog['species'],data)
                self.app.event('Saved Pokémon sprite offsets',species=result['species'],values=result['values'])
            elif path=='/api/connect': result=self.app.connect_emulator()
            elif path=='/api/audit': result=self.app.audit(data.get('id'))
            elif path=='/api/evidence': result=self.app.evidence(data.get('note',''))
            else: raise ValueError('Unknown endpoint')
            self.send(200,result)
        except (ValueError,KeyError,TypeError) as e: self.send(400,dict(error=str(e)))
        except Exception as e: self.send(500,dict(error=str(e)))


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--port',type=int,default=8765)
    parser.add_argument('--fresh',action='store_true',help='Start without copying the main save')
    parser.add_argument('--no-open',action='store_true')
    parser.add_argument('--new-session',action='store_true',help='Copy a new test session instead of resuming')
    parser.add_argument('--launch-game',action='store_true',help='Launch the test game and automatically load the bridge when supported')
    parser.add_argument('--lan-host', help='Mac private IPv4 address for paired iPhone access on trusted Wi-Fi')
    args=parser.parse_args()
    if args.lan_host:
        try:
            address = ipaddress.IPv4Address(args.lan_host)
            if not address.is_private or address.is_loopback or address.is_unspecified or address.is_link_local:
                raise ValueError()
        except ValueError:
            parser.error('--lan-host must be your Mac’s private Wi-Fi IPv4 address')
    base=ROOT/'.cache/debug-dashboard'
    previous=None if args.fresh or args.new_session else resume_session(base)
    directory,bootstrap,digest=previous or prepare(base,not args.fresh)
    app=Dashboard(directory,bootstrap,digest)
    server=ThreadingHTTPServer(('0.0.0.0' if args.lan_host else '127.0.0.1',args.port),Handler);server.app=app
    server.lan_host=args.lan_host
    pairing_file=directory/'phone-pairing.key'
    if not pairing_file.exists():
        fd=os.open(pairing_file, os.O_WRONLY | os.O_CREAT | os.O_EXCL, 0o600)
        with os.fdopen(fd, 'w') as stream:
            stream.write(secrets.token_hex(8))
    server.phone_key=pairing_file.read_text().strip()
    url=f'http://127.0.0.1:{server.server_port}'
    atomic(ROOT/'.cache/debug-dashboard/current.json',json.dumps(dict(url=url,directory=str(directory),connect=str(bootstrap))))
    print(f'Dashboard: {url}\nTest ROM: {directory / "expedition-debug.gba"}\nIn mGBA Tools → Scripting, run: dofile({lua(str(bootstrap))})',flush=True)
    if args.lan_host:
        print(f'iPhone: http://{args.lan_host}:{server.server_port}\nPairing code: {server.phone_key}', flush=True)
    if args.launch_game:
        try: print(app.connect_emulator()['message'],flush=True)
        except (OSError,ValueError,subprocess.SubprocessError) as e: print('Could not launch mGBA: '+str(e),flush=True)
    if not args.no_open:
        import webbrowser
        webbrowser.open(url)
    try: server.serve_forever()
    except KeyboardInterrupt: pass
    finally:
        (directory/'command.tsv').unlink(missing_ok=True)
        server.server_close()

if __name__=='__main__': main()
