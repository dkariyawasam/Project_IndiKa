"""Exercise the actual shop transaction routines with host-side inventory stubs."""
from pathlib import Path
import re
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
s = (ROOT / 'src/shop.c').read_text()

def function(name):
    start = s.index('static void ' + name + '(u8 taskId)\n{')
    end = s.index('\n}\n', start) + 3
    return s[start:end]

harness = r'''
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
typedef uint8_t u8; typedef uint16_t u16; typedef uint32_t u32; typedef int16_t s16;
#define TRUE 1
#define FALSE 0
#define ITEM_NONE 0
#define ITEM_PREMIER_BALL 12
#define GAME_STAT_SHOPPED 1
#define SE_SHOP 1
#define tItemCount data[1]
#define tPremierBallBonusCount data[2]
#define tItemId data[5]
struct {s16 data[16]; void (*func)(u8);} gTasks[1];
struct {u32 itemPrice;} sShopData;
struct {u32 money;} save, *gSaveBlock1Ptr=&save;
u8 sCoinShopMode, monResult;
u16 coins; int bagFull, bagAdded, monAdded, stats;
const u8 sRocketMonLevels[]={26};
const u8 sText_NotEnoughCoins[]="coins",gText_YouDontHaveMoney[]="money",sText_PrizeFull[]="full",sText_PrizeParty[]="party",sText_PrizePC[]="pc",gText_NoMoreRoomForThis[]="bag",gText_HereYouGoThankYou[]="thanks",gText_ThrowInPremierBall[]="bonus";
const u8 *lastMessage; void (*pending)(u8);
void BuyMenuReturnToItemList(u8 taskId){}
void Task_ReturnToItemListAfterItemPurchase(u8 taskId){}
static void BuyMenuSubtractMoney(u8 taskId);
int IsCoinShop(void){return sCoinShopMode!=0;}
u32 ShopBalance(void){return IsCoinShop()?coins:save.money;}
int CoinCatalogIndex(int item){return 0;}
void PutWindowTilemap(int w){}
u8 ScriptGiveMon(int species,int level,int item,int a,int b,int c){if(monResult!=2)monAdded++;return monResult;}
int GetPremierBallBonusCount(int item,int count){return 0;}
int CheckBagHasSpaceForPremierBallBonus(int item,int count){return !bagFull;}
int AddBagItem(int item,int count){if(bagFull)return 0;bagAdded+=count;return 1;}
void BuyMenuDisplayMessage(u8 id,const u8 *msg,void (*callback)(u8)){lastMessage=msg;pending=callback;}
void DebugFunc_PrintPurchaseDetails(u8 id){}
void IncrementGameStat(int stat){stats++;}
void RemoveCoins(u16 n){assert(coins>=n);coins-=n;}
void RemoveMoney(u32 *money,u32 n){assert(*money>=n);*money-=n;}
void PlaySE(int se){}
void DrawShopCoinBalance(void){}
u32 GetMoney(u32 *m){return *m;}
void PrintMoneyAmountInMoneyBox(int w,u32 n,int speed){}
'''
checks = r'''
void setup(int mode,int price,int balance){
 sCoinShopMode=mode;sShopData.itemPrice=price;coins=balance;save.money=50000;
 bagFull=0;monResult=0;bagAdded=0;monAdded=0;stats=0;pending=0;
 gTasks[0].data[1]=3;gTasks[0].data[5]=289;
}
int main(void){
 setup(1,1500,1500);BuyMenuTryMakePurchase(0);assert(bagAdded==3&&pending==BuyMenuSubtractMoney);pending(0);assert(coins==0&&save.money==50000);
 setup(1,1500,1499);BuyMenuTryMakePurchase(0);assert(!bagAdded&&pending==BuyMenuReturnToItemList&&coins==1499);
 setup(1,1500,2000);bagFull=1;BuyMenuTryMakePurchase(0);assert(!bagAdded&&coins==2000&&pending==BuyMenuReturnToItemList);
 setup(2,9999,9999);BuyMenuTryMakePurchase(0);assert(monAdded==1&&lastMessage==sText_PrizeParty);pending(0);assert(coins==0&&save.money==50000);
 setup(2,500,600);monResult=1;BuyMenuTryMakePurchase(0);assert(monAdded==1&&lastMessage==sText_PrizePC);pending(0);assert(coins==100);
 setup(2,500,600);monResult=2;BuyMenuTryMakePurchase(0);assert(!monAdded&&coins==600&&pending==BuyMenuReturnToItemList);
 setup(0,1500,777);BuyMenuTryMakePurchase(0);pending(0);assert(save.money==48500&&coins==777);
 puts("PASS: coin item and Pokemon purchases, insufficient funds, full Bag/PC, party/PC delivery, ordinary money shops.");
}
'''
with tempfile.TemporaryDirectory() as d:
    path = Path(d)
    (path / 'test.c').write_text(harness + function('BuyMenuTryMakePurchase') + function('BuyMenuSubtractMoney') + checks)
    subprocess.run(['cc', str(path / 'test.c'), '-o', str(path / 'test')], check=True)
    subprocess.run([str(path / 'test')], check=True)

legacy = (ROOT / 'data/maps/CeladonCity_GameCorner_PrizeRoom/scripts.inc').read_text()
expected = re.findall(r'setvar VAR_TEMP_1, (ITEM_TM\d+)\n\tsetvar VAR_TEMP_2, (\d+)', legacy)
items = re.search(r'sRocketCoinTms\[\] = \{(.*?)\n};', s, re.S)[1]
prices = re.search(r'sRocketCoinPrices\[\] = \{(.*?)\n};', s, re.S)[1]
assert list(zip(re.findall(r'ITEM_TM\d+', items), re.findall(r'\d+', prices))) == expected
assert len(expected) == 59
print('PASS: all 59 TM coin prices match the original clerk.')
