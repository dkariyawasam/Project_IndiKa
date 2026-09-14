#!/usr/bin/env python3
"""Run the real dynamic-object palette cleanup against sprite ownership cases."""
from pathlib import Path
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
source = (root / 'src/event_object_movement.c').read_text()
start = source.index('static void ReleaseUnusedDynamicObjectPalettes(void)\n{')
end = source.index('\nstatic u8 LoadDynamicObjectEventPalette', start)
cleanup = source[start:end]
lookup_start = source.index('static u8 FindObjectEventPaletteIndexByTag(u16 tag)\n{')
lookup_end = source.index('\nvoid LoadPlayerObjectReflectionPalette', lookup_start)
lookup = source[lookup_start:lookup_end]
stubs = '''
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
typedef uint8_t u8; typedef uint16_t u16;
#define OBJ_PALSLOT_COUNT 12
#define MAX_SPRITES 64
#define TAG_NONE 0xFFFF
#define ARRAY_COUNT(a) (sizeof(a)/sizeof((a)[0]))
struct { const void *data; u16 tag; } sObjectEventSpritePalettes[201];
struct { int inUse; struct { u8 paletteNum; } oam; } gSprites[MAX_SPRITES];
u16 tags[16];
u16 GetSpritePaletteTagByPaletteNum(u8 slot) { return tags[slot]; }
void FreeSpritePaletteByTag(u16 tag) { for (int i=12;i<16;i++) if(tags[i]==tag) tags[i]=TAG_NONE; }
'''
tests = '''
int main(void) {
    for(int i=0;i<200;i++) { sObjectEventSpritePalettes[i].data=tags; sObjectEventSpritePalettes[i].tag=0x1100+i; }
    assert(FindObjectEventPaletteIndexByTag(0x1200)==0xFF);
    assert(FindObjectEventPaletteIndexByTag(0)==0xFF);
    assert(FindObjectEventPaletteIndexByTag(TAG_NONE)==0xFF);
    for(int i=0;i<16;i++) tags[i]=TAG_NONE;
    tags[10]=0x1101; tags[12]=0x1102; tags[13]=0x1103; tags[14]=0x2000;
    gSprites[0].inUse=1; gSprites[0].oam.paletteNum=12;
    gSprites[1].inUse=1; gSprites[1].oam.paletteNum=12;
    ReleaseUnusedDynamicObjectPalettes();
    assert(tags[10]==0x1101 && tags[12]==0x1102);
    assert(tags[13]==TAG_NONE && tags[14]==0x2000);
    gSprites[0].inUse=0;
    ReleaseUnusedDynamicObjectPalettes();
    assert(tags[12]==0x1102);
    gSprites[1].inUse=0;
    ReleaseUnusedDynamicObjectPalettes();
    assert(tags[12]==TAG_NONE);
    // Repeatedly despawning NPCs must not exhaust the four-slot pool.
    for(int n=0;n<200;n++) {
        tags[12]=0x1100+n;
        gSprites[2].inUse=1; gSprites[2].oam.paletteNum=12;
        ReleaseUnusedDynamicObjectPalettes();
        assert(tags[12]==0x1100+n);
        gSprites[2].inUse=0;
        ReleaseUnusedDynamicObjectPalettes();
        assert(tags[12]==TAG_NONE);
    }
    puts("Passed: shared live palettes retained, unused NPC palettes reclaimed, fixed and field-effect palettes protected, 200 reuse cycles.");
}
'''
with tempfile.TemporaryDirectory() as temp:
    path=Path(temp); (path/'test.c').write_text(stubs+lookup+cleanup+tests)
    subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror',str(path/'test.c'),'-o',str(path/'test')],check=True)
    subprocess.run([str(path/'test')],check=True,timeout=5)
