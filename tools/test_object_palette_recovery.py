#!/usr/bin/env python3
"""Exercise production recovery, loading and cleanup with a bounded palette pool."""
from pathlib import Path
import subprocess,tempfile
src=(Path(__file__).resolve().parents[1]/'src/event_object_movement.c').read_text()
def function(signature):
 start=src.index(signature+'\n{'); end=start; depth=0
 for i in range(src.index('{',start),len(src)):
  depth += (src[i]=='{')-(src[i]=='}')
  if depth==0:return src[start:i+1]+'\n'
code='''
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
typedef uint8_t u8; typedef uint16_t u16;
#define OBJ_PALSLOT_COUNT 12
#define MAX_SPRITES 64
#define TAG_NONE 0xffff
#define PALSLOT_NPC_1 2
#define PALSLOT_NPC_DYNAMIC 15
#define ARRAY_COUNT(a) (sizeof(a)/sizeof((a)[0]))
struct SpritePalette { const void *data; u16 tag; };
struct Sprite { int inUse; struct { u8 paletteNum; } oam; } gSprites[64];
struct ObjectEvent { u8 graphicsId; };
struct ObjectEventGraphicsInfo { u8 paletteSlot; u16 paletteTag; } info[3]={{15,0x1101},{15,0x1101},{5,0x1102}};
struct SpritePalette sObjectEventSpritePalettes[]={{info,0x1101},{info,0x1102},{0,0}};
u16 tags[16]; int loads,tints;
const struct ObjectEventGraphicsInfo *GetObjectEventGraphicsInfo(u8 id){return &info[id];}
u16 GetSpritePaletteTagByPaletteNum(u8 slot){return tags[slot];}
void FreeSpritePaletteByTag(u16 tag){for(int i=12;i<16;i++)if(tags[i]==tag)tags[i]=TAG_NONE;}
u8 IndexOfSpritePaletteTag(u16 tag){for(int i=12;i<16;i++)if(tags[i]==tag)return i;return 255;}
u8 TryLoadObjectPalette(const struct SpritePalette *p){
 if(IndexOfSpritePaletteTag(p->tag)!=255)return 255;
 for(int i=12;i<16;i++)if(tags[i]==TAG_NONE){tags[i]=p->tag;loads++;return i;}
 return 255;
}
void ApplyGlobalFieldPaletteTint(u8 slot){assert(slot>=12 && slot<16);tints++;}
'''
for sig in ['static u8 FindObjectEventPaletteIndexByTag(u16 tag)','static void ReleaseUnusedDynamicObjectPalettes(void)','static u8 LoadDynamicObjectEventPalette(u16 paletteTag)','static void TryRecoverObjectEventPalette(struct ObjectEvent *objectEvent, struct Sprite *sprite)']:code+=function(sig)
code+='''
int main(void){
 struct ObjectEvent a={0},b={1},fixed={2};
 for(int i=0;i<16;i++)tags[i]=TAG_NONE;
 for(int i=12;i<16;i++)tags[i]=0x1200+i; // Live weather/effect palettes.
 gSprites[0].inUse=1;gSprites[0].oam.paletteNum=2;
 TryRecoverObjectEventPalette(&a,&gSprites[0]);assert(gSprites[0].oam.paletteNum==2 && loads==0);
 tags[13]=TAG_NONE;
 TryRecoverObjectEventPalette(&a,&gSprites[0]);assert(gSprites[0].oam.paletteNum==13 && loads==1 && tints==1);
 for(int i=0;i<200;i++)TryRecoverObjectEventPalette(&a,&gSprites[0]);assert(loads==1 && tints==1);
 gSprites[1].inUse=1;gSprites[1].oam.paletteNum=2;
 TryRecoverObjectEventPalette(&b,&gSprites[1]);assert(gSprites[1].oam.paletteNum==13 && loads==1);
 gSprites[2].oam.paletteNum=5;TryRecoverObjectEventPalette(&fixed,&gSprites[2]);assert(gSprites[2].oam.paletteNum==5);
 assert(tags[12]==0x120c && tags[14]==0x120e && tags[15]==0x120f);
 puts("PASS: full pool fallback, recovery after release, shared palette reuse, no repeated reload, fixed/effect palette protection.");
}
'''
with tempfile.TemporaryDirectory() as tmp:
 p=Path(tmp);(p/'test.c').write_text(code)
 subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror',str(p/'test.c'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
