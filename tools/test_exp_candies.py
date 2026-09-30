"""Compile the actual candy and move-range routines against a small host harness."""
from pathlib import Path
import subprocess,tempfile
ROOT=Path(__file__).resolve().parents[1]
s=(ROOT/'src/party_menu.c').read_text()
code=s[s.index('static const u8 sText_ExpCandyGained'):s.index('void ItemUseCB_RareCandy(u8 taskId, TaskFunc func)\n{')]
p=(ROOT/'src/pokemon.c').read_text()
learn=p[p.index('u16 MonTryLearningMovesInLevelRange('):p.index('void DeleteFirstMoveAndGiveMoveToMon(')]
h=r'''
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
typedef uint8_t u8; typedef uint16_t u16; typedef uint32_t u32; typedef int16_t s16; typedef u8 bool8;
typedef void (*TaskFunc)(u8);
#define TRUE 1
#define FALSE 0
#define _(x) x
#define MAX_LEVEL 100
#define NUM_STATS 6
#define MON_DATA_SPECIES 0
#define MON_DATA_EXP 1
#define MON_DATA_LEVEL 2
#define MON_DATA_IS_EGG 3
#define ITEM_EXP_CANDY_S 362
#define SE_SELECT 0
#define FANFARE_LEVEL_UP 0
#define STR_CONV_MODE_LEFT_ALIGN 0
#define LEVEL_UP_END 0xffff
#define LEVEL_UP_MOVE_ID 511
#define MOVE_NONE 0
struct Pokemon {u32 data[4];} gPlayerParty[1];
struct {u8 slotId;} gPartyMenu;
struct {u8 growthRate;} gSpeciesInfo[1];
u32 gExperienceTables[1][101];
struct {s16 data[14];} internal, *sPartyMenuInternal=&internal;
struct {TaskFunc func;} gTasks[1];
u8 gStringVar1[64],gStringVar2[64],gStringVar4[128];
const u8 gText_PkmnElevatedToLvVar2[]="",gText_WontHaveEffect[]="";
u16 gSpecialVar_ItemId; bool8 gPartyMenuUseExitCallback,sExpCandyLearning; u8 sExpCandyOldLevel;
void (*gItemUseCB)(u8,TaskFunc);
int used,animated;
u32 GetMonData(struct Pokemon *m,int k) {return m->data[k];}
void SetMonData(struct Pokemon *m,int k,const void *v) {m->data[k]=*(const u32*)v;}
void CalculateMonStats(struct Pokemon *m) {int i=1; while(i<100 && m->data[1]>=gExperienceTables[0][i+1]) i++; m->data[2]=i;}
void GetMonLevelUpWindowStats(struct Pokemon *m,s16 *s) {}
void RemoveBagItem(int item,int n) {used+=n;}
void UpdateMonDisplayInfoAfterRareCandy(int slot,struct Pokemon *m) {}
void GetMonNickname(struct Pokemon *m,u8 *s) {}
void PlayFanfareByFanfareNum(int a) {}
void ConvertIntToDecimalStringN(u8 *s,int v,int a,int b) {}
void StringExpandPlaceholders(u8 *s,const u8 *t) {}
void Task_DisplayLevelUpStatsPg1(u8 t) {}
void Task_ClosePartyMenuAfterText(u8 t) {}
void DisplayPartyMenuMessage(const u8 *s,int b) {}
void ScheduleBgCopyTilemapToVram(int a) {}
void PlaySE(int a) {}
void Task_DoUseItemAnim(u8 a) {animated++;}
u16 sLearningMoveTableID,gMoveToLearn;
const u16 moves[]={ (5<<9)|1,(10<<9)|2,(10<<9)|3,(15<<9)|4,(20<<9)|5,LEVEL_UP_END};
const u16 *gLevelUpLearnsets[]={moves};
u16 GiveMoveToMon(struct Pokemon *m,u16 move) {return move;}
'''
tests=r'''
int main(void) {
 int i; const u32 amounts[]={800,3000,10000,30000};
 for(i=0;i<=100;i++)gExperienceTables[0][i]=i*i*i;
 for(i=0;i<4;i++) {
  memset(gPlayerParty,0,sizeof(gPlayerParty)); used=animated=0;
  gPlayerParty[0].data[1]=1000;gPlayerParty[0].data[2]=10;
  gSpecialVar_ItemId=362+i; ItemUseCB_ExpCandy(0,NULL);assert(animated==1);
  gItemUseCB(0,NULL);assert(used==1);assert(gPlayerParty[0].data[1]==1000+amounts[i]);
 }
 gPlayerParty[0].data[1]=999990;gPlayerParty[0].data[2]=99;used=0;
 ItemUseCB_ExpCandyStep(0,NULL);assert(gPlayerParty[0].data[1]==1000000 && used==1);
 animated=used=0;ItemUseCB_ExpCandy(0,NULL);assert(!animated && !used);
 gPlayerParty[0].data[2]=5;gPlayerParty[0].data[3]=1;
 ItemUseCB_ExpCandy(0,NULL);assert(!animated && !used);
 gPlayerParty[0].data[3]=0;gPlayerParty[0].data[1]=970299;gPlayerParty[0].data[2]=99;
 gSpecialVar_ItemId=362;ItemUseCB_ExpCandy(0,NULL);gItemUseCB(0,NULL);
 assert(gPlayerParty[0].data[2]==99 && !sExpCandyLearning);
 gPlayerParty[0].data[2]=15;
 assert(MonTryLearningMovesInLevelRange(gPlayerParty,TRUE,5)==2);
 assert(MonTryLearningMovesInLevelRange(gPlayerParty,FALSE,5)==3);
 assert(MonTryLearningMovesInLevelRange(gPlayerParty,FALSE,5)==4);
 assert(MonTryLearningMovesInLevelRange(gPlayerParty,FALSE,5)==MOVE_NONE);
 assert(MonTryLearningMovesInLevelRange(gPlayerParty,TRUE,15)==MOVE_NONE);
 puts("PASS: all four EXP amounts, cap, level-100/Egg rejection, no-level gain, one-item consumption, all crossed-level moves");
}
'''
with tempfile.TemporaryDirectory() as d:
 c=Path(d)/'test.c';exe=Path(d)/'test';c.write_text(h+code+learn+tests)
 subprocess.run(['cc','-w',str(c),'-o',str(exe)],check=True)
 subprocess.run([str(exe)],check=True)
