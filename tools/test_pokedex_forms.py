#!/usr/bin/env python3
"""Execute actual form navigation and arrow lifecycle helpers with host stubs."""
from pathlib import Path
import subprocess,tempfile,re
R=Path(__file__).resolve().parents[1]
s=(R/'src/pokedex_screen.c').read_text()
def section(start,end):return s[s.index(start):s.index(end,s.index(start))]
helpers=section('static const u16 *DexScreen_FormGroup(u16 species)\n{','static const u8 *DexScreen_FormLabel(u16 species)\n{')
arrows=section('static void DexScreen_RemoveFormArrows(void)\n{','static bool8 DexScreen_ChangeForm(s8 direction)\n{')
code=r'''
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "constants/species.h"
typedef unsigned char u8;typedef signed char s8;typedef unsigned short u16;typedef short s16;typedef int bool8;
#define TRUE 1
#define FALSE 0
#define TASK_NONE 255
#define TAG_NONE 65535
#define FLAG_GET_SEEN 0
#define FLAG_GET_CAUGHT 1
#define ARRAY_COUNT(a) (sizeof(a)/sizeof((a)[0]))
u8 seen[NUM_SPECIES],owned[NUM_SPECIES];
int DexScreen_GetSetPokedexFlag(u16 species,int flag,int unused){return flag?owned[species]:seen[species];}
#include "data/pokemon/pokedex_forms.h"
struct {u8 categoryCursorPosInPage,numMonsOnPage,formArrowTaskId;u16 pageSpecies[4],formArrowPosition,formArrowSpecies;} screen,*sPokedexScreenData=&screen;
struct ScrollArrowsTemplate {u8 firstArrowType,firstX,firstY,secondArrowType,secondX,secondY;u16 fullyUpThreshold,fullyDownThreshold,tileTag,palTag;u8 palNum;};
const struct ScrollArrowsTemplate sDexOrderScrollArrowsTemplate={.firstArrowType=2,.secondArrowType=3};
const u8 coords[1][4]={{11,3,11,11}};
const u8 (*sCategoryPageIconCoords[])[4]={coords};
int created,destroyed;
struct ScrollArrowsTemplate last;
u8 AddScrollIndicatorArrowPair(const struct ScrollArrowsTemplate *a,u16 *offset){last=*a;created++;assert(*offset==1);return 10;}
void RemoveScrollIndicatorArrowPair(u8 task){assert(task==10);destroyed++;}
'''+helpers+arrows+r'''
int main(void){
 memset(seen,1,sizeof(seen));
 assert(DexScreen_AdjacentForm(SPECIES_MEOWTH,-1)==SPECIES_NONE);
 assert(DexScreen_AdjacentForm(SPECIES_MEOWTH,1)==SPECIES_MEOWTH_ALOLAN);
 assert(DexScreen_AdjacentForm(SPECIES_MEOWTH_ALOLAN,1)==SPECIES_MEOWTH_GALARIAN);
 assert(DexScreen_AdjacentForm(SPECIES_MEOWTH_GALARIAN,1)==SPECIES_NONE);
 assert(DexScreen_AdjacentForm(SPECIES_MEOWTH_GALARIAN,-1)==SPECIES_MEOWTH_ALOLAN);
 seen[SPECIES_MEOWTH_ALOLAN]=0;
 assert(DexScreen_AdjacentForm(SPECIES_MEOWTH,1)==SPECIES_MEOWTH_GALARIAN);
 assert(DexScreen_FormGroup(SPECIES_MR_RIME)==NULL);
 assert(DexScreen_FormGroup(SPECIES_PERRSERKER)==NULL);
 assert(DexScreen_FormGroup(SPECIES_CINNABAR_FEEBAS)[0]==SPECIES_FEEBAS);
 owned[SPECIES_FARFETCHD]=1;
 assert(DexScreen_FormFamilyOwned(SPECIES_FARFETCHD));
 owned[SPECIES_RAICHU]=1;
 assert(!DexScreen_FormFamilyOwned(SPECIES_RAICHU));
 owned[SPECIES_RAICHU_ALOLAN]=1;
 assert(DexScreen_FormFamilyOwned(SPECIES_RAICHU));
 screen.numMonsOnPage=1;screen.formArrowTaskId=TASK_NONE;
 screen.pageSpecies[0]=SPECIES_RAICHU;
 DexScreen_UpdateFormArrows(FALSE);
 assert(created==1 && last.firstArrowType==2 && last.secondArrowType==3);
 assert(last.fullyUpThreshold==1 && last.fullyDownThreshold==65535);
 assert(last.palNum==7); // Completion ball owns palette 6.
 DexScreen_UpdateFormArrows(FALSE);assert(created==1);
 screen.pageSpecies[0]=SPECIES_RAICHU_ALOLAN;
 DexScreen_UpdateFormArrows(FALSE);
 assert(created==2 && destroyed==1);
 assert(last.fullyUpThreshold==65535 && last.fullyDownThreshold==1);
 DexScreen_UpdateFormArrows(TRUE);assert(destroyed==2 && screen.formArrowTaskId==TASK_NONE);
 DexScreen_RemoveFormArrows();assert(destroyed==2);
 return 0;
}
'''
with tempfile.TemporaryDirectory() as tmp:
 p=Path(tmp);(p/'test.c').write_text(code)
 subprocess.run(['cc','-I'+str(R/'include'),'-I'+str(R/'src'),str(p/'test.c'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
print('PASS: form boundaries, unseen forms, distinct evolutions, independent ownership and arrow lifecycle/palette')

# Regional forms must only appear through their family's selector, not extra pages.
categories=(R/'src/data/pokemon/pokedex_categories.h').read_text()
forms=(R/'src/data/pokemon/pokedex_forms.h').read_text()
slots=re.findall(r'\bSPECIES_(\w+)', categories)
for group in re.findall(r'\{([^{}]+)\}', forms):
 members=re.findall(r'SPECIES_(\w+)',group)
 if not members or any(m.startswith('CINNABAR_') for m in members):
  continue # Primeval deep forms retain their deliberately themed sea page.
 assert sum(slots.count(m) for m in members)==1,members
for species in ('PERRSERKER','CURSOLA','MR_RIME','CLODSIRE'):
 assert slots.count(species)==1,species
for page in re.findall(r'static const u16 \w+\[\] = \{([^}]+)\}', categories):
 assert 1 <= len(re.findall(r'SPECIES_',page)) <= 4
print('PASS: regional habitat families have one slot; distinct evolutions retained within four-slot pages')
