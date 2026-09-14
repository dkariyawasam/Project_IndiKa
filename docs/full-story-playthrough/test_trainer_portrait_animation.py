"""Exercise the actual portrait animation selector with retired slot zero empty."""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[2]
source=(root/'src/trainer_pokemon_sprites.c').read_text()
anims=source.split('static const union AnimCmd sTrainerPicAnim[]')[1].split('static const struct OamData')[0]
anims='static const union AnimCmd sTrainerPicAnim[]'+anims
function=source.split('void AssignSpriteAnimsTable(bool8 isTrainer)')[1].split('\nu16 CreatePicSprite')[0]
function='void AssignSpriteAnimsTable(bool8 isTrainer)'+function
harness='''
#include <assert.h>
#include <stddef.h>
typedef unsigned char bool8;
union AnimCmd { struct { unsigned short image, duration; } frame; short type; };
#define ANIMCMD_FRAME(i,d) {.frame={i,d}}
#define ANIMCMD_END {.type=-1}
static struct { const union AnimCmd *const *anims; } sCreatingSpriteTemplate;
static const union AnimCmd monAnim[] = {ANIMCMD_FRAME(0,1),ANIMCMD_END};
const union AnimCmd *const gAnims_MonPic[] = {monAnim};
const union AnimCmd *const *const gTrainerFrontAnimsPtrTable[] = {NULL};
'''+anims+function+'''
int main(void) {
 assert(gTrainerFrontAnimsPtrTable[0] == NULL);
 AssignSpriteAnimsTable(1);
 assert(sCreatingSpriteTemplate.anims != NULL);
 assert(sCreatingSpriteTemplate.anims[0][0].frame.image == 0);
 assert(sCreatingSpriteTemplate.anims[0][0].frame.duration == 1);
 assert(sCreatingSpriteTemplate.anims[0][1].type == -1);
 AssignSpriteAnimsTable(0);
 assert(sCreatingSpriteTemplate.anims == gAnims_MonPic);
 return 0;
}
'''
with tempfile.TemporaryDirectory() as tmp:
 p=Path(tmp);(p/'portrait.c').write_text(harness)
 subprocess.run(['cc','-std=c99','-Wall','-Werror',str(p/'portrait.c'),'-o',str(p/'portrait')],check=True)
 subprocess.run([str(p/'portrait')],check=True)
print('PASS: Trainer portraits use frame 0 and terminate with retired slot zero null; Pokemon animations unchanged.')
