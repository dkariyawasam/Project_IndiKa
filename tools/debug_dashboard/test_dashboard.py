"""Regression tests for catalogs, command boundaries and encrypted party decoding."""
import json
import struct
import tempfile
import time
import unittest
from pathlib import Path
from unittest.mock import patch
from catalog import build_catalog, constants, decode_party, ORDERS, map_network
from server import Dashboard
from atlas import arrange, lz77, png

class DashboardTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls): cls.catalog=build_catalog()
    def setUp(self):
        self.tmp=tempfile.TemporaryDirectory();self.addCleanup(self.tmp.cleanup)
        self.path=Path(self.tmp.name)
        with patch('server.build_catalog',return_value=self.catalog): self.app=Dashboard(self.path,self.path/'connect.lua','fixture')
        self.state=dict(session=self.path.name,matched=True,ready=True,frame=1,party={},flagsHex='',varsHex='')
        self.write_state()
    def write_state(self): (self.path/'status.json').write_text(json.dumps(self.state))
    def test_catalog(self):
        c=constants();self.assertEqual(c['VARS_START'],0x4000)
        self.assertEqual(self.catalog['chars'][0x1B], 'é')
        self.assertEqual(self.catalog['chars'][0], ' ')
        self.assertFalse(any(x['key']=='FLAG_MAKES_CONTACT' for x in self.catalog['flags']))
        self.assertEqual(next(x['key'] for x in self.catalog['moves'] if x['id']==33), 'MOVE_TACKLE')
        self.assertFalse(any(x['key'].startswith('MOVE_EFFECT_') for x in self.catalog['moves']))
        self.assertGreater(len(self.catalog['species']),200)
        self.assertGreater(len(self.catalog['maps']),200)
        self.assertTrue(any(x['key']=='FLAG_DEFEATED_APEX_MEWTWO' for x in self.catalog['flags']))
        self.assertTrue(any(x['name']=='PalletTown' for x in self.catalog['maps']))
    def test_map_network(self):
        maps=self.catalog['maps']; network=self.catalog['network']
        names={m['name'] for m in maps}
        self.assertEqual(len(network['links'])+len(network['unresolved']),
                         sum(len(m['warps'])+len(m['connections']) for m in maps))
        self.assertTrue(all(e['source'] in names and e['target'] in names for e in network['links']))
        self.assertTrue(any(e['source']=='PalletTown' and e['target']=='Route1' and e['direction']=='up'
                            for e in network['links'] if e['kind']=='connection'))
        self.assertTrue(any(e['source']=='PalletTown' and e['target']=='PalletTown_PlayersHouse_1F'
                            for e in network['links'] if e['kind']=='warp'))
        fixture=[dict(name='Room',id='MAP_ROOM',connections=[],warps=[dict(x=1,y=2,dest_map='MAP_DYNAMIC',dest_warp_id=255)])]
        result=map_network(fixture)
        self.assertFalse(result['links']);self.assertEqual(result['unresolved'][0]['destination'],'MAP_DYNAMIC')

    def test_atlas_edge_offsets(self):
        maps=[dict(name='A',id='MAP_A',width=10,height=8,connections=[dict(map='MAP_B',direction='up',offset=-3)]),
              dict(name='B',id='MAP_B',width=12,height=6,connections=[dict(map='MAP_A',direction='down',offset=3)]),
              dict(name='Room',id='MAP_ROOM',width=4,height=4,connections=[])]
        atlas=arrange(maps);p={m['name']:m for m in atlas['maps']}
        self.assertEqual(p['B']['x']-p['A']['x'],-3)
        self.assertEqual(p['B']['y']+p['B']['height'],p['A']['y'])
        self.assertEqual(len(atlas['components']),2)
        self.assertFalse(atlas['conflicts']);self.assertFalse(atlas['overlaps'])
        maps[1]['connections'][0]['offset']=4
        self.assertTrue(arrange(maps)['conflicts'])
    def test_atlas_compression(self):
        self.assertEqual(lz77(bytes([0x10,6,0,0,0x10])+b'ABC'+bytes([0,2])),b'ABCABC')
        self.assertTrue(png(1,1,b'\xff\x00\x00').startswith(b'\x89PNG\r\n\x1a\n'))

    def test_reject_unknown_and_bounds(self):
        for d in [dict(action='lua',code='os.execute("bad")'),dict(action='warp',map='PalletTown',x=-1,y=1),
                  dict(action='warp',map='../bad',x=0,y=0),dict(action='pokemon',species=0,level=10),
                  dict(action='money',value=True),dict(action='appearance',style=0,accent=5,secondary=0),
                  dict(action='flag',id=65535,value=1),dict(action='input',key='A',frames=121)]:
            with self.subTest(d=d),self.assertRaises(ValueError):self.app.command(d)
        self.assertFalse((self.path/'command.tsv').exists())
    def test_protocol_ack_and_pending(self):
        result=self.app.command(dict(action='heal'))
        fields=(self.path/'command.tsv').read_text().split('\t')
        self.assertEqual(fields[:3],[self.path.name,str(result['id']),'3'])
        self.assertLessEqual(int(fields[3]),time.time()+5)
        with self.assertRaises(ValueError):self.app.command(dict(action='heal'))
        (self.path/'ack.json').write_text(json.dumps(dict(session=self.path.name,id=result['id'],ok=True,message='Applied')))
        self.assertIsNone(self.app.state()['pending'])
        self.assertTrue(self.app.events[-1]['ok'])
    def test_old_ack_ignored(self):
        self.app.command(dict(action='heal'))
        (self.path/'ack.json').write_text(json.dumps(dict(session='old',id=1,ok=True,message='Applied')))
        self.assertIsNotNone(self.app.state()['pending'])
    def test_busy_stale_and_mismatch(self):
        self.state['ready']=False;self.write_state()
        with self.assertRaises(ValueError):self.app.command(dict(action='heal'))
        self.app.command(dict(action='input',key='A',frames=8))
        self.app.pending=None;self.state['matched']=False;self.write_state()
        with self.assertRaises(ValueError):self.app.command(dict(action='input',key='A',frames=8))
        self.state['matched']=True;self.write_state()
        import os
        os.utime(self.path/'status.json',(1,1))
        with self.assertRaises(ValueError):self.app.command(dict(action='input',key='A',frames=8))
    def test_restore_must_exist(self):
        with self.assertRaises(ValueError):self.app.command(dict(action='restore',checkpoint=1))
        (self.path/'checkpoint-1.ss').write_bytes(b'fixture')
        self.assertEqual(self.app.command(dict(action='restore',checkpoint=1))['action'],'restore')
    def test_encrypted_party_all_orders(self):
        for personality in range(24):
            raw=bytearray(100);ot=12345;struct.pack_into('<II',raw,0,personality,ot)
            raw[8:18]=b'\xff'*10;raw[84]=33;struct.pack_into('<HH',raw,86,40,80)
            growth=bytearray(12);struct.pack_into('<HH',growth,0,25,0);growth[9]=220
            attacks=bytearray(12);struct.pack_into('<4H',attacks,0,1,2,3,4);attacks[8:12]=bytes([10,20,30,40])
            parts={'G':growth,'A':attacks,'E':bytes(12),'M':bytes(12)}
            secure=b''.join(parts[x] for x in ORDERS[personality])
            struct.pack_into('<H',raw,28,sum(struct.unpack('<24H',secure))&65535)
            raw[32:80]=b''.join(struct.pack('<I',x^personality^ot) for x in struct.unpack('<12I',secure))
            result=decode_party(raw.hex(),self.catalog)
            self.assertTrue(result['checksumValid']);self.assertEqual(result['species'],25)
            self.assertEqual(result['friendship'],220);self.assertEqual(result['moves'],[1,2,3,4])
            self.assertEqual(result['hp'],40)
    def test_audit_allowlist(self):
        with self.assertRaises(ValueError):self.app.audit('../../bad')

if __name__=='__main__': unittest.main()
