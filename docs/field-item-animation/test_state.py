"""Exercise the production animation state machine with field/task fixtures."""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[2]
source='\n'.join(l for l in (root/'src/field_item_animation.c').read_text().splitlines()if not l.startswith('#include'))
fixture=r'''
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
typedef uint8_t u8;typedef uintptr_t u32;typedef int16_t s16;typedef u8 bool8;
#define TRUE 1
#define FALSE 0
#define TASK_NONE 255
#define MOVEMENT_ACTION_START_ANIM_IN_DIRECTION 9
typedef void (*TaskFunc)(u8);
struct Task {TaskFunc func;s16 data[16];}gTasks[16];
struct ObjectEvent {int graphicsId,facingDirection;int held,finished;}gObjectEvents[1];
struct {int objectEventId,preventStep,flags,gender;}gPlayerAvatar;
static int locked,scriptEnabled,poseCount,followCount,active;static u32 words[16];
static void Follow(u8 id){assert(id==2);followCount++;}
static u8 CreateTask(TaskFunc f,int priority){active=1;memset(&gTasks[3],0,sizeof(gTasks[3]));gTasks[3].func=f;return 3;}
static void SetWordTaskArg(u8 id,int data,u32 v){words[id]=v;}
static u32 GetWordTaskArg(u8 id,int data){return words[id];}
static void TaskDummy(u8 id){}
static int ArePlayerFieldControlsLocked(void){return locked;}
static void LockPlayerFieldControls(void){locked=1;}
static void UnlockPlayerFieldControls(void){locked=0;}
static int ObjectEventIsMovementOverridden(struct ObjectEvent*p){return p->held;}
static int ObjectEventClearHeldMovementIfFinished(struct ObjectEvent*p){if(p->held&&p->finished){p->held=0;return 1;}return 0;}
static void StartPlayerAvatarSummonMonForFieldMoveAnim(void){poseCount++;gObjectEvents[0].graphicsId=900+gPlayerAvatar.gender;}
static void ObjectEventSetHeldMovement(struct ObjectEvent*p,int action){p->held=1;p->finished=0;}
static void ObjectEventSetGraphicsId(struct ObjectEvent*p,int gfx){p->graphicsId=gfx;}
static void ObjectEventTurn(struct ObjectEvent*p,int dir){p->facingDirection=dir;}
static void DestroyTask(u8 id){assert(id==3);active=0;}
static void ScriptContext_Enable(void){scriptEnabled=1;}
'''
main=r'''
int main(void){int gender,flags,script,i;for(gender=0;gender<2;gender++)for(flags=1;flags<=8;flags*=2)for(script=0;script<2;script++){
 s16 saved[16];int originalGfx=100+gender*10+flags;
 memset(gTasks,0,sizeof(gTasks));memset(gObjectEvents,0,sizeof(gObjectEvents));
 gObjectEvents[0].graphicsId=originalGfx;gObjectEvents[0].facingDirection=4;gObjectEvents[0].held=1;
 gPlayerAvatar.gender=gender;gPlayerAvatar.flags=flags;gPlayerAvatar.preventStep=0;
 locked=script;scriptEnabled=poseCount=followCount=0;
 for(i=0;i<16;i++)saved[i]=gTasks[2].data[i]=i*37+9;
 if(script)PlayFieldItemAnimation();else StartFieldItemAnimation(2,Follow);
 gTasks[3].func(3);assert(locked&&gPlayerAvatar.preventStep&&poseCount==0);
 gObjectEvents[0].finished=1;gTasks[3].func(3);assert(poseCount==1&&active);
 assert(gObjectEvents[0].graphicsId==900+gender);
 gTasks[3].func(3);assert(active&&followCount==0&&scriptEnabled==0);
 gObjectEvents[0].finished=1;gTasks[3].func(3);
 assert(!active&&gObjectEvents[0].graphicsId==originalGfx&&gObjectEvents[0].facingDirection==4);
 assert(gPlayerAvatar.flags==flags&&!gPlayerAvatar.preventStep&&locked==script);
 assert(scriptEnabled==script&&followCount==!script);
 assert(!memcmp(saved,gTasks[2].data,sizeof(saved)));
 }
 puts("PASS: both sprite sets; walking/bike/surf state and direction restored; pending movement awaited; task data preserved; script and item continuation once, after animation.");
}
'''
with tempfile.TemporaryDirectory()as d:
 p=Path(d);(p/'test.c').write_text(fixture+source+main);subprocess.run(['cc','-std=c99','-Wall','-Wno-unused-function',str(p/'test.c'),'-o',str(p/'test')],check=True);subprocess.run([str(p/'test')],check=True)
# Integration guards: only accepted actions animate; specialized actions retain their own paths.
s=(root/'src/item_use.c').read_text();assert 'sItemUseOnFieldCB = ItemUseOnFieldCB_AnimateItemfinder;'in s
assert 'sItemUseOnFieldCB = ItemUseOnFieldCB_AnimateCascadeBoard;'in s
assert 'CanUseCascadeBoardOnField() == TRUE'in s
for file,label in [('data/maps/CeladonCity/scripts.inc','CeladonCity_EventScript_BerryPatchAccessGranted'),('data/maps/CinnabarIsland/scripts.inc','CinnabarIsland_EventScript_PokemonLabAccessGranted'),('data/scripts/silphco_doors.inc','EventScript_OpenDoor'),('data/scripts/pkmn_center_nurse.inc','EventScript_PkmnCenterNurse_CheckFriendship')]:
 s=(root/file).read_text().split(label+'::')[1].split('\n\n')[0];assert 'special PlayFieldItemAnimation\n\twaitstate'in s
print('PASS: Board/Finder callbacks and successful key/pass script branches wired.')
