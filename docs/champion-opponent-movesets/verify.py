"""Verify every Champion move slot in the rebuilt ROM against source."""
from pathlib import Path
import re,struct,subprocess,hashlib
root=Path(__file__).resolve().parents[2]
source=(root/'src/data/trainer_parties.h').read_text()
moves={k:int(v) for k,v in re.findall(r'^#define (MOVE_\w+)\s+(\d+)\s*$',(root/'include/constants/moves.h').read_text(),re.M)}
symbols={parts[2]:int(parts[0],16) for line in subprocess.check_output(['/opt/devkitpro/devkitARM/bin/arm-none-eabi-nm',str(root/'pokefirered.elf')],text=True).splitlines() if len(parts:=line.split())==3}
rom=(root/'pokefirered.gba').read_bytes()
parties=re.findall(r'static const struct (TrainerMon\w+) (sParty_(?:Champion(?:First|Rematch)\w+|RocketLeagueChampionGiovanni))\[\] = \{(.*?)\n\};',source,re.S)
assert len(parties)==7
for kind,name,body in parties:
    slots=re.findall(r'\.moves = \{([^}]+)\}',body)
    assert len(slots)==6
    stride,offset=(16,8) if kind=='TrainerMonItemCustomMoves' else (16,6)
    for i,slot in enumerate(slots):
        names=[v.strip() for v in slot.split(',')]
        assert len(names)==len(set(names))==4
        expected=tuple(moves[n] for n in names)
        assert struct.unpack_from('<4H',rom,symbols[name]-0x08000000+i*stride+offset)==expected,(name,i)
for name,expected in {
'pokefirered.sav':'816eb072055dad4bbd8ca2b4d596cf468e58b68384664931a91e814dbb824b08',
'pokefirered-player-customisation.gba':'880e9e2dc73c16084b10dfc3cb13bfa1b3535b637277e02a576fb8aee0194d5b',
}.items():
    assert hashlib.sha256((root/name).read_bytes()).hexdigest()==expected,name
print('PASS: all 168 Champion move slots match rebuilt ROM; main save and customisation ROM unchanged.')
