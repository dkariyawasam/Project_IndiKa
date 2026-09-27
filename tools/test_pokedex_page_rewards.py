"""Run the actual page reward C helpers with a host-side Bag/save harness."""
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
source = (ROOT / 'src/pokedex_screen.c').read_text()
helpers = source[source.index('#define DEX_PAGE_REWARDS_VERSION'):source.index('static bool8 DexScreen_CreateCategoryListGfx(bool8 justRegistered)\n{')]
harness = r'''
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "constants/species.h"
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef u8 bool8;
#define NELEMS(a) (sizeof(a) / sizeof((a)[0]))
#define TRUE 1
#define FALSE 0
#define _(s) s
#define FLAG_GET_CAUGHT 1
#define ITEM_POKE_BALL 1
#define ITEM_RARE_CANDY 2
#define SE_HELP_ERROR 1
#define SE_SELECT 2
#define WINDOW_TILEMAP_TOP 0
#define WINDOW_PALETTE_NUM 1
#define BG_PLTT_ID(n) ((n)*16)
#define PLTT_SIZE_4BPP 32
u16 gPlttBufferUnfaded[512];
void LoadPalette(const void *p, int offset, int size) {}
#define COPYWIN_MAP 0
struct PokedexCategoryPage { const u16 *species; u8 count; };
#include "data/pokemon/pokedex_categories.h"
struct {u8 category, pageNum, rewardHeaderPulseTimer, rewardFooterRevealWidth, rewardFooterHold, rewardFooterRetracting;} screen, *sPokedexScreenData = &screen;
struct Save {u32 dexPageRewardsVersion; u8 dexPageRewards[64];} save, *gSaveBlock1Ptr = &save;
u8 owned[NUM_SPECIES];
int balls, candy, ballSpace = 1, candySpace = 1, failCandy;
const char *footer;
const char *header;
const u8 gText_DPadAnyPickOKBack[] = "NORMAL CONTROLS";
void DexScreen_PrintHeaderControlInfo(const u8 *s) {header = (const char *)s;}
bool8 DexScreen_GetSetPokedexFlag(u16 s, int f, int species) {return owned[s];}
bool8 CheckBagHasSpace(u16 item, u16 count) {return item == 1 ? ballSpace : candySpace;}
bool8 AddBagItem(u16 item, u16 count) {
 if (item == 2 && failCandy) return FALSE;
 if (item == 1) balls += count; else candy += count;
 return TRUE;
}
bool8 RemoveBagItem(u16 item, u16 count) {balls -= count; return TRUE;}
void DexScreen_UpdateCompletionBall(void) {}
void PlaySE(int sound) {}
void FillBgTilemapBufferRect_Palette0(int bg, int tile, int x, int y, int w, int h) {assert(w >= 0 && w <= 30);}
int footerVisible;
void PutWindowTilemap(int w) {if(w==0) footerVisible=1;}
void CopyWindowToVram(int w, int mode) {}
void ClearWindowTilemap(int w) {if(w==0) footerVisible=0;}
void SetWindowAttribute(int w, int a, int v) {}
void DrawUiHintHeader(int w, const u8 *s, int a, int b, int c, int d) {footer = (const char *)s; if(w==0 && d) footerVisible=1;}
'''
tests = r'''
int main(void) {
 int c, p, i; u8 keys[512] = {0}; struct Save saved;
 for(c=0;c<sizeof(gDexCategories)/sizeof(gDexCategories[0]);c++)
  for(p=0;p<gDexCategories[c].count;p++) {
   const struct PokedexCategoryPage *page=&gDexCategories[c].page[p];
   assert(page->count > 0); assert(page->species[0] < 512);
   assert(!keys[page->species[0]]++); // No page can share another page's claim.
  }
 memset(&save, 0xFF, sizeof(save));
 DexScreen_DrawPageRewardFooter(); assert(!footerVisible);
 owned[DexScreen_RewardPage()->species[0]]=1;
 assert(!DexScreen_PageIsOwned()); // An unseen second species must still count.
 DexScreen_ClaimPageReward(); assert(!balls && !candy);
 for(i=0;i<DexScreen_RewardPage()->count;i++) owned[DexScreen_RewardPage()->species[i]]=1;
 DexScreen_DrawPageRewardFooter(); assert(!strcmp(footer," ")); assert(strstr(header,"{START_BUTTON}CLAIM"));
 candySpace=0; DexScreen_ClaimPageReward(); assert(!balls && !candy && !DexScreen_PageRewardClaimed());
 candySpace=1; ballSpace=0; DexScreen_ClaimPageReward(); assert(!balls && !candy && !DexScreen_PageRewardClaimed());
 ballSpace=1; failCandy=1; DexScreen_ClaimPageReward(); assert(!balls && !candy && !DexScreen_PageRewardClaimed());
 failCandy=0; DexScreen_ClaimPageReward(); assert(balls==3 && candy==1 && DexScreen_PageRewardClaimed());
 assert(screen.rewardFooterRevealWidth == 2); assert(!footerVisible);
 for(i=0;i<15;i++) DexScreen_AnimateRewardFooter();
 assert(screen.rewardFooterHold == 120); assert(footerVisible);
 assert(!strcmp(footer,"3 POKé BALLS + 1 RARE CANDY        ")); assert(!strcmp(header,"NORMAL CONTROLS"));
 for(i=0;i<135;i++) DexScreen_AnimateRewardFooter();
 assert(!screen.rewardFooterRevealWidth);
 DexScreen_DrawPageRewardFooter(); assert(!strcmp(footer," ")); assert(!footerVisible);
 saved=save; memset(&save,0,sizeof(save)); save=saved;
 DexScreen_ClaimPageReward(); assert(balls==3 && candy==1);
 screen.pageNum=1; assert(!DexScreen_PageRewardClaimed());
 puts("PASS: unique page keys, incomplete/claim/complete states, full Bag, rollback, repeat claims and saved claims");
}
'''
with tempfile.TemporaryDirectory() as folder:
    c = Path(folder) / 'rewards.c'
    exe = Path(folder) / 'rewards'
    c.write_text(harness + helpers + tests)
    subprocess.run(['cc', '-w', '-I', str(ROOT / 'include'), '-I', str(ROOT / 'src'), str(c), '-o', str(exe)], check=True)
    subprocess.run([str(exe)], check=True)
