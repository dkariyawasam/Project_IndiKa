"""Exercise the actual save migration with encrypted inventory and held items."""
import pathlib, re, subprocess, tempfile, unittest, json
ROOT = pathlib.Path(__file__).resolve().parents[1]

class RetiredItemsTest(unittest.TestCase):
    def test_save_cleanup(self):
        source = (ROOT/'src/item.c').read_text()
        migration = source[source.index('bool8 IsRetiredItem('):source.index('u16 SanitizeItemId(')]
        retired = re.findall(r'RETIRED_ITEM\((ITEM_\w+)\)', (ROOT/'include/constants/retired_items.h').read_text())
        definitions = '\n'.join(f'#define {name} {i+1}' for i,name in enumerate(retired))
        harness = r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
typedef uint8_t u8; typedef uint16_t u16; typedef u8 bool8;
#define TRUE 1
#define FALSE 0
#define ITEM_NONE 0
#define NUM_BAG_POCKETS 5
#define PC_ITEMS_COUNT 4
#define PARTY_SIZE 6
#define TOTAL_BOXES_COUNT 14
#define IN_BOX_COUNT 30
#define DAYCARE_MON_COUNT 2
#define MON_DATA_HELD_ITEM 0
#define MON_DATA_MAIL 1
#define FLAG_SYS_WHITE_FLUTE_ACTIVE 0
#define FLAG_SYS_BLACK_FLUTE_ACTIVE 1
struct ItemSlot {u16 itemId, quantity;};
struct BagPocket {struct ItemSlot *itemSlots; u8 capacity;};
struct Pokemon {u16 item, mail;};
struct BoxPokemon {u16 item;};
struct Mail {u16 item;};
struct DaycareMon {struct BoxPokemon mon; struct {struct Mail message;} mail;};
struct Save {struct ItemSlot pcItems[4]; u16 registeredItem; struct {struct DaycareMon mons[2];} daycare;};
struct Save save, *gSaveBlock1Ptr=&save;
struct ItemSlot bag[5][4]; struct BagPocket gBagPockets[5];
struct Pokemon gPlayerParty[6]; struct BoxPokemon boxes[14][30];
int flags[2]={1,1}, mailCleared;
void SetBagPocketsPointers(void) {int i; for(i=0;i<5;i++){gBagPockets[i].itemSlots=bag[i];gBagPockets[i].capacity=4;}}
void SetBagItemQuantity(u16 *p,u16 q){*p=q^0x9A31;}
void SetPcItemQuantity(u16 *p,u16 q){*p=q;}
void BagPocketCompaction(struct ItemSlot *s,u8 n){int i,j=0;for(i=0;i<n;i++)if(s[i].itemId)s[j++]=s[i];while(j<n){s[j].itemId=0;SetBagItemQuantity(&s[j++].quantity,0);}}
void ItemPcCompaction(void){}
u16 GetMonData(struct Pokemon *p,int f){return f?p->mail:p->item;}
void SetMonData(struct Pokemon *p,int f,const void *v){if(f)p->mail=*(u8*)v;else p->item=*(u16*)v;}
struct BoxPokemon *GetBoxedMonPtr(int b,int n){return &boxes[b][n];}
u16 GetBoxMonData(struct BoxPokemon *p,int f){return p->item;}
void SetBoxMonData(struct BoxPokemon *p,int f,const void *v){p->item=*(u16*)v;}
void ClearMailStruct(struct Mail *m){m->item=0;}
void ClearMailData(void){mailCleared=1;}
void FlagClear(int f){flags[f]=0;}
'''
        check = r'''
int main(void){int i,j;
for(i=0;i<5;i++){
 bag[i][0].itemId=ITEM_SOUL_DEW;SetBagItemQuantity(&bag[i][0].quantity,5);
 bag[i][1].itemId=500;SetBagItemQuantity(&bag[i][1].quantity,23);
}
save.pcItems[0]=(struct ItemSlot){ITEM_SACRED_ASH,3};save.pcItems[1]=(struct ItemSlot){500,12};
save.registeredItem=ITEM_POKE_FLUTE;
gPlayerParty[0].item=ITEM_ORANGE_MAIL;gPlayerParty[1].item=500;
boxes[13][29].item=ITEM_RAZZ_BERRY;boxes[4][5].item=500;
save.daycare.mons[0].mon.item=ITEM_FLUFFY_TAIL;save.daycare.mons[1].mon.item=500;
RemoveRetiredItemsFromSave();RemoveRetiredItemsFromSave();
for(i=0;i<5;i++){assert(bag[i][0].itemId==500);assert((bag[i][0].quantity^0x9A31)==23);for(j=1;j<4;j++){assert(!bag[i][j].itemId);assert((bag[i][j].quantity^0x9A31)==0);}}
assert(!save.pcItems[0].itemId && !save.pcItems[0].quantity);assert(save.pcItems[1].itemId==500 && save.pcItems[1].quantity==12);
assert(!save.registeredItem && !gPlayerParty[0].item && gPlayerParty[0].mail==255 && gPlayerParty[1].item==500);
assert(!boxes[13][29].item && boxes[4][5].item==500);
assert(!save.daycare.mons[0].mon.item && save.daycare.mons[1].mon.item==500);
assert(mailCleared && !flags[0] && !flags[1]);return 0;}
'''
        with tempfile.TemporaryDirectory() as tmp:
            p=pathlib.Path(tmp);(p/'test.c').write_text(definitions+'\n'+harness+migration+check)
            subprocess.run(['cc','-I',str(ROOT/'include'),str(p/'test.c'),'-o',str(p/'test')],check=True)
            subprocess.run([str(p/'test')],check=True)

    def test_no_active_catalog_or_map_sources(self):
        retired=set(re.findall(r'RETIRED_ITEM\((ITEM_\w+)\)',(ROOT/'include/constants/retired_items.h').read_text()))
        items=json.loads((ROOT/'src/data/items.json').read_text())['items']
        self.assertEqual(len(retired),35)
        for item in items:
            if item['itemId'] in retired:
                self.assertEqual(item['pocket'],0)
                self.assertEqual(item['holdEffect'],'HOLD_EFFECT_NONE')
                self.assertEqual(item['battleUsage'],0)
        for path in (ROOT/'data/maps').rglob('*'):
            if path.suffix in ('.json','.inc'):
                self.assertFalse(retired.intersection(re.findall(r'\bITEM_\w+',path.read_text())),str(path))

if __name__=='__main__':unittest.main()
