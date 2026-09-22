#!/usr/bin/env python3
"""Check actual radial palette backup/restore functions with guarded buffers."""
import re, subprocess, tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
s=(ROOT/'src/start_menu.c').read_text()
def extract(name):
    m=re.search(r'static void '+name+r'\(void\)\n\{',s); assert m
    i=m.end();depth=1
    while depth:
        depth+=(s[i]=='{')-(s[i]=='}');i+=1
    return s[m.start():i]
code='''#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
typedef uint16_t u16;
typedef uint8_t bool8;
#define EWRAM_DATA
#define TRUE 1
#define FALSE 0
#define RGB_BLACK 0
#define OBJ_PLTT_ID(n) (256+(n)*16)
#define CpuCopy16(src,dst,len) memcpy(dst,src,len)
struct Guarded { u16 pre[16], colors[512], post[16]; } unfaded, faded;
#define gPlttBufferUnfaded unfaded.colors
#define gPlttBufferFaded faded.colors
static void BlendPalette(u16 start,u16 count,uint8_t coeff,u16 color) {
    assert(start+count<=512); assert(coeff==16); assert(color==0);
    memset(gPlttBufferFaded+start,0,count*sizeof(u16));
}
'''
for line in s.splitlines():
    if re.match(r'#define START_MENU_(ICON_OBJ_PAL_SLOT|OBJ_PAL_BACKUP_START|OBJ_PAL_BACKUP_COUNT)\b',line): code+=line+'\n'
    if re.match(r'static EWRAM_DATA (?:u16|bool8) sRadialStartMenu(?:ObjPalBackup|LeavingOverworld)',line):code+=line+'\n'
code+=extract('BackupRadialStartMenuObjPalettes')+'\n'+extract('RestoreRadialStartMenuObjPalettes')
code+='''
int main(void) {
    memset(&unfaded,0x12,sizeof(unfaded));memset(&faded,0x34,sizeof(faded));
    assert(sizeof(sRadialStartMenuObjPalBackupUnfaded)==32);
    BackupRadialStartMenuObjPalettes();
    memset(gPlttBufferUnfaded+496,0x56,32);memset(gPlttBufferFaded+496,0x78,32);
    // Values outside the borrowed slot change after the backup and must survive.
    gPlttBufferFaded[0]=0x9999;faded.post[0]=0xaaaa;
    RestoreRadialStartMenuObjPalettes();
    assert(gPlttBufferUnfaded[496]==0x1212);assert(gPlttBufferFaded[496]==0x3434);
    assert(gPlttBufferFaded[0]==0x9999);assert(faded.post[0]==0xaaaa);
    BackupRadialStartMenuObjPalettes();
    memset(gPlttBufferFaded,0,sizeof(faded.colors));
    sRadialStartMenuLeavingOverworld=TRUE;
    RestoreRadialStartMenuObjPalettes();
    for(int i=0;i<512;i++)assert(gPlttBufferFaded[i]==0);
    assert(gPlttBufferUnfaded[511]==0x1212);
    assert(!sRadialStartMenuLeavingOverworld && !sRadialStartMenuObjPalBackupValid);
    gPlttBufferFaded[511]=0x7777;RestoreRadialStartMenuObjPalettes();
    assert(gPlttBufferFaded[511]==0x7777);
    assert(unfaded.pre[0]==0x1212 && unfaded.post[0]==0x1212 && faded.pre[0]==0x3434 && faded.post[0]==0xaaaa);
    puts("Radial palette bounds, cancel restoration, black submenu handoff and repeated cleanup passed");
}
'''
with tempfile.TemporaryDirectory(prefix='radial-palette-') as tmp:
    p=Path(tmp);(p/'test.c').write_text(code)
    subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror',str(p/'test.c'),'-o',str(p/'test')],check=True)
    subprocess.run([str(p/'test')],check=True)
