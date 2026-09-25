#!/usr/bin/env python3
"""Exercise the production accent transform, save fallback and palette boundaries."""
from pathlib import Path
import subprocess
import tempfile
root = Path(__file__).resolve().parents[1]
source = (root / 'src/player_appearance.c').read_text()
source = '\n'.join(line for line in source.splitlines() if not line.startswith('#include'))
header = (root / 'include/player_appearance.h').read_text()
stubs = '''
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
typedef uint8_t u8; typedef uint16_t u16;
#define RGB(r,g,b) ((r)|((g)<<5)|((b)<<10))
u16 gPlttBufferUnfaded[512], baseline[512];
struct { u8 appearanceVersion, playerAccent, playerSecondaryAccent; } save, *gSaveBlock2Ptr = &save;
void LoadPalette(const void *src, unsigned offset, unsigned bytes) {
    assert(offset*2+bytes <= sizeof(gPlttBufferUnfaded));
    memcpy(gPlttBufferUnfaded+offset,src,bytes);
}
'''
tests = '''
int main(void) {
 for (int i=0;i<512;i++) baseline[i]=RGB(27,12,7);
 for (int version=0;version<=4;version++)
 for (int accent=0;accent<=3;accent++)
 for (int secondary=0;secondary<=3;secondary++)
 for (int kind=0;kind<=4;kind++) {
    for (int i=0;i<512;i++) baseline[i]=RGB(27,12,7);
    for (int i=256+(kind==1?17:5);i<256+(kind==1?22:kind==2?9:8);i++) baseline[i]=RGB(7,12,27);
    memcpy(gPlttBufferUnfaded,baseline,sizeof(baseline));
    save.appearanceVersion=version;save.playerAccent=accent;save.playerSecondaryAccent=secondary;
    ApplyPlayerAppearancePalette(256,kind);
    int first=kind==1?27:kind==2?12:11, count=kind==1?5:2;
    for(int i=0;i<512;i++) {
      if((version==2 || version==3) && accent>0 && accent<3 && i>=256+first && i<256+first+count) {
        u16 c=gPlttBufferUnfaded[i];
        assert(c!=baseline[i]);
        int r=c&31,g=(c>>5)&31,b=(c>>10)&31;
        assert((accent==1?b:g)==27);
        assert(r+g+b==46);
      } else if(version==3 && secondary>0 && secondary<3
        && i>=256+(kind==1?17:5) && i<256+(kind==1?22:kind==2?9:8)) {
        u16 c=gPlttBufferUnfaded[i];
        assert(c!=baseline[i]);
        assert((c&31)+((c>>5)&31)+((c>>10)&31)==46);
        assert((secondary==1 ? c&31 : (c>>5)&31)==27);
      } else assert(gPlttBufferUnfaded[i]==baseline[i]);
    }
 }
 puts("PASS: both independent accents, invalid/legacy saves, version-2 compatibility and palette boundaries across all five layouts.");
}
'''
with tempfile.TemporaryDirectory() as temp:
 p=Path(temp);(p/'test.c').write_text(stubs+header+source+tests)
 subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror',str(p/'test.c'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
