#!/usr/bin/env python3
"""Check built move data and execute the compiled escape decision branches.
Attack cancellation, rendering and PP consumption are outside this focused test.
"""
from pathlib import Path
import struct, subprocess
root=Path(__file__).resolve().parents[2]
rom=(root/'pokefirered.gba').read_bytes()
out=subprocess.check_output(['/opt/devkitpro/devkitARM/bin/arm-none-eabi-nm',str(root/'pokefirered.elf')],text=True)
sym={p[2]:int(p[0],16) for line in out.splitlines() if len(p:=line.split())==3}
def read(a,n):return int.from_bytes(rom[a-0x8000000:a-0x8000000+n],'little')
base=sym['gBattleMoves']; move=362
assert read(base+move*12+4,1)==20
assert read(base+move*12+7,1)==7
assert all(read(base+i*12+7,1)<7 or read(base+i*12+7,1)>=128 for i in range(move))
assert read(sym['gBattleAnims_Moves']+move*4,4)==sym['Move_TELEPORT']
script=sym['PalletTown_ProfessorOaksLab_EventScript_ChoseStarter']
assert bytes([0x7b,0,2])+struct.pack('<H',move) in rom[script-0x8000000:script-0x8000000+180]
def outcome(move,flags,trapped):
 pc=sym['BattleScript_EffectTeleport']; calls=0
 ram={sym['gCurrentMove']:move,sym['gBattleTypeFlags']:flags,sym['gBattleCommunication']:0}
 ends={sym['BattleScript_TeleportEscape']:'escape',sym['BattleScript_ButItFailed']:'fail',sym['BattleScript_PrintAbilityMadeIneffective']:'ability'}
 for _ in range(30):
  if pc in ends:return ends[pc],calls
  op=read(pc,1)
  if op in (0,2,3):pc+=1
  elif op==0x28:pc=read(pc+1,4)
  elif op in (0x29,0x2a,0x2b):
   n={0x29:1,0x2a:2,0x2b:4}[op];cmp=read(pc+1,1);a=read(pc+2,4);value=read(pc+6,n);target=read(pc+6+n,4)
   match=ram[a]==value if cmp==0 else bool(ram[a]&value)
   pc=target if match else pc+10+n
  elif op==0x76:
   assert read(pc+2,1)==2
   ram[sym['gBattleCommunication']]=trapped;calls+=1;pc+=3
  else:raise AssertionError(hex(op))
 raise AssertionError('unterminated')
for trapped in (0,1,2):
 assert outcome(362,0,trapped)==('escape',0)
 assert outcome(362,8,trapped)==('fail',0) # TRAINER
 assert outcome(362,(1<<13)|(1<<18),trapped)==('fail',0) # APEX
 assert outcome(100,0,trapped)==(('escape','fail','ability')[trapped],1)
print('PASS: compiled priority/PP/animation/starter grant; 12 escape/trainer/Apex/trapping cases; Teleport unchanged.')
