"""Exercise the production revival timeline with host-side graphics/audio stubs."""
from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


class RevivalTimelineTest(unittest.TestCase):
    def test_two_shards_amber_and_sprite_allocation_failure(self):
        source = (ROOT / 'src/fossil_revival_scene.c').read_text()
        task = source[source.index('static void Task_Revival(u8 taskId)\n{'):]
        stub = r'''
#include <assert.h>
#include <string.h>
typedef unsigned char u8;
#define TRUE 1
#define FALSE 0
#define MAX_SPRITES 64
#define ITEM_NONE 0
#define PALETTES_OBJECTS 1
#define PALETTES_ALL 2
#define RGB_WHITE 32767
#define RGB_BLACK 0
struct { int species, items[2], monSprite, itemSprites[2], frame, phase; } sRevival;
struct { int x, invisible; } gSprites[64];
struct { int active; } gPaletteFade;
static int fadeFrames, cryFrames, cries, auras, exited, freed;
void SpawnRevivalAura(void) { auras++; }
void BeginNormalPaletteFade(int mask, int delay, int from, int to, int color) {
    gPaletteFade.active=1; fadeFrames=20;
}
void PlayCry_Normal(int species, int pan) { cries++; cryFrames=110; }
int IsCryPlaying(void) { return cryFrames > 0; }
void SetVBlankCallback(void *p) {}
void FreeAndDestroyMonPicSprite(int id) { assert(id<64); freed++; }
void ResetSpriteData(void) {}
void FreeAllSpritePalettes(void) {}
void DestroyTask(int id) {}
void CB2_ReturnToFieldContinueScript(void) {}
void SetMainCallback2(void (*cb)(void)) { assert(cb==CB2_ReturnToFieldContinueScript);exited++; }
'''
        main = r'''
int main(void) {
    for (int mode=0;mode<3;mode++) {
        memset(&sRevival,0,sizeof(sRevival)); memset(gSprites,0,sizeof(gSprites));
        cries=auras=exited=freed=cryFrames=fadeFrames=0;gPaletteFade.active=0;
        sRevival.items[0]=1;sRevival.items[1]=mode==1?0:2;
        sRevival.monSprite=mode==2?65535:0;
        sRevival.itemSprites[0]=mode==2?64:1;
        sRevival.itemSprites[1]=mode==0?2:64;
        gSprites[0].invisible=1;gSprites[1].x=mode==1?120:80;gSprites[2].x=160;
        int lastLeft=gSprites[1].x,lastRight=gSprites[2].x;
        for (int frame=0;frame<600 && !exited;frame++) {
            int phase=sRevival.phase;
            Task_Revival(0);
            if (mode==0 && phase==1) {
                assert(gSprites[1].x>=lastLeft && gSprites[1].x<=120);
                assert(gSprites[2].x<=lastRight && gSprites[2].x>=120);
                lastLeft=gSprites[1].x;lastRight=gSprites[2].x;
            }
            if (phase<2) assert(gSprites[0].invisible);
            if (phase==4 && cryFrames>0) assert(sRevival.phase==4);
            if (fadeFrames && --fadeFrames==0) gPaletteFade.active=0;
            if (cryFrames) cryFrames--;
        }
        assert(exited==1 && cries==1 && auras==2);
        assert(freed==(mode==2?0:1));
        if (mode==0) assert(lastLeft==120 && lastRight==120);
        if (mode==1) assert(gSprites[1].x==120);
    }
}
'''
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / 'timeline.c'
            exe = Path(tmp) / 'timeline'
            path.write_text(stub + task + main)
            subprocess.run(['cc', '-std=c99', str(path), '-o', str(exe)], check=True)
            subprocess.run([str(exe)], check=True)


if __name__ == '__main__':
    unittest.main()
