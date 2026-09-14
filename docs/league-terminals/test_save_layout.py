"""Compare old and new save layouts using the game's actual agbcc compiler."""
from pathlib import Path
import tempfile,subprocess,re
root=Path(__file__).resolve().parents[2]
with tempfile.TemporaryDirectory(prefix='league-layout-') as d:
 p=Path(d);legacy=p/'legacy';legacy.mkdir()
 header=(root/'include/global.h').read_text().replace('struct LeagueRecordsData leagueRecords;\n    u8 filler_B12[0x40E - sizeof(struct LeagueRecordsData)];','u8 filler_B12[0x40E];')
 (legacy/'global.h').write_text(header)
 results=[]
 for name,field,inc in [('new','leagueRecords',[]),('old','filler_B12',['-I',str(legacy)])]:
  src=p/(name+'.c');src.write_text('#include "global.h"\nconst unsigned offsets[] = {sizeof(struct SaveBlock2), (unsigned)&((struct SaveBlock2*)0)->leagueChallenge, (unsigned)&((struct SaveBlock2*)0)->'+field+', (unsigned)&((struct SaveBlock2*)0)->encryptionKey, (unsigned)&((struct SaveBlock2*)0)->questData};\n')
  args=['/opt/devkitpro/devkitARM/bin/arm-none-eabi-cpp']+inc+['-I',str(root/'include'),'-I',str(root/'tools/agbcc/include'),'-I',str(root/'tools/agbcc'),'-nostdinc','-undef','-std=gnu89','-DFIRERED','-DREVISION=0','-DENGLISH','-DMODERN=0',str(src)]
  pp=p/(name+'.i');pp.write_bytes(subprocess.check_output(args))
  asm=p/(name+'.s');subprocess.run([str(root/'tools/agbcc/bin/agbcc'),'-mthumb-interwork','-O2','-o',str(asm),str(pp)],check=True)
  results.append(re.findall(r'\.word\s+(\d+)',asm.read_text()))
 assert results[0]==results[1],results
 print('PASS: unchanged save size, league state, padding start, encryption key and quest offsets:',', '.join(results[0]))
