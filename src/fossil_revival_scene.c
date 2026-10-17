#include "global.h"
#include "gflib.h"
#include "decompress.h"
#include "event_data.h"
#include "item_menu_icons.h"
#include "overworld.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "scanline_effect.h"
#include "sound.h"
#include "task.h"
#include "trainer_pokemon_sprites.h"
#include "trig.h"
#include "constants/items.h"
#include "constants/species.h"
#include "constants/songs.h"

#define TAG_FOSSIL_LEFT  0x5800
#define TAG_FOSSIL_RIGHT 0x5801
#define TAG_FOSSIL_AURA  0x5802

static EWRAM_DATA struct {
    u16 species;
    u16 items[2];
    u32 personality;
    u32 otId;
    u16 monSprite;
    u8 itemSprites[2];
    u16 frame;
    u8 phase;
} sRevival;

static const u32 sBgTiles[] = INCBIN_U32("graphics/pokemon_special_anim/bg.4bpp.lz");
static const u32 sBgMap[] = INCBIN_U32("graphics/pokemon_special_anim/bg.bin.lz");
static const u16 sBgPal[] = INCBIN_U16("graphics/pokemon_special_anim/bg.gbapal");
static const u32 sAuraTiles[] = INCBIN_U32("graphics/pokemon_special_anim/outward_spiral_dots.4bpp.lz");
static const u16 sAuraPal[] = INCBIN_U16("graphics/pokemon_special_anim/outward_spiral_dots.gbapal");
static const struct BgTemplate sBg = {
    .bg = 0, .charBaseIndex = 0, .mapBaseIndex = 31, .priority = 3,
};
static const struct CompressedSpriteSheet sAuraSheet = {sAuraTiles, 0x60, TAG_FOSSIL_AURA};
static const struct SpritePalette sAuraPalette = {sAuraPal, TAG_FOSSIL_AURA};
static const struct OamData sAuraOam = {
    .shape = SPRITE_SHAPE(8x8), .size = SPRITE_SIZE(8x8), .priority = 0,
};
static void SpriteCB_RevivalAura(struct Sprite *sprite);
static const struct SpriteTemplate sAuraTemplate = {
    .tileTag = TAG_FOSSIL_AURA, .paletteTag = TAG_FOSSIL_AURA,
    .oam = &sAuraOam, .anims = gDummySpriteAnimTable,
    .affineAnims = gDummySpriteAffineAnimTable, .callback = SpriteCB_RevivalAura,
};
static void CB2_InitFossilRevival(void);
static void Task_Revival(u8 taskId);

static void VBlank_Revival(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void CB2_Revival(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void Task_EnterRevival(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        DestroyTask(taskId);
        SetMainCallback2(CB2_InitFossilRevival);
    }
}

// Called after givemon succeeds. TEMP_1 is species; TEMP_2 is its destination.
// Never create a second Pokemon or touch the party/box while displaying it.
void ShowFossilRevival(void)
{
    struct BoxPokemon *mon;
    sRevival.species = VarGet(VAR_TEMP_1);
    if (VarGet(VAR_TEMP_2) == MON_GIVEN_TO_PC)
        mon = GetBoxedMonPtr(gSpecialVar_MonBoxId, gSpecialVar_MonBoxPos);
    else
        mon = &gPlayerParty[gPlayerPartyCount - 1].box;
    sRevival.personality = GetBoxMonData(mon, MON_DATA_PERSONALITY);
    sRevival.otId = GetBoxMonData(mon, MON_DATA_OT_ID);
    sRevival.items[0] = ITEM_OLD_AMBER;
    sRevival.items[1] = ITEM_NONE;
    switch (sRevival.species)
    {
    case SPECIES_OMANYTE:
    case SPECIES_OMATO:
        sRevival.items[0] = ITEM_HELIX_FOSSIL_SHARD;
        sRevival.items[1] = sRevival.species == SPECIES_OMANYTE ? ITEM_HASTE_FOSSIL_SHARD : ITEM_DART_FOSSIL_SHARD;
        break;
    case SPECIES_KABUTO:
    case SPECIES_KABUSTAR:
        sRevival.items[0] = ITEM_DOME_FOSSIL_SHARD;
        sRevival.items[1] = sRevival.species == SPECIES_KABUTO ? ITEM_DART_FOSSIL_SHARD : ITEM_HASTE_FOSSIL_SHARD;
        break;
    }
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    CreateTask(Task_EnterRevival, 0);
}

static void CB2_InitFossilRevival(void)
{
    u8 i;
    SetVBlankCallback(NULL);
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    ScanlineEffect_Stop();
    ResetTasks();
    ResetSpriteData();
    FreeAllSpritePalettes();
    // The Pokemon picture uses fixed OBJ palette 0. Item icons allocate theirs.
    gReservedSpritePaletteCount = 1;
    FreeAllWindowBuffers();
    ResetPaletteFade();
    ResetBgsAndClearDma3BusyFlags(FALSE);
    InitBgsFromTemplates(0, &sBg, 1);
    LZDecompressVram(sBgTiles, (void *)BG_VRAM);
    LZDecompressVram(sBgMap, (void *)(BG_VRAM + 31 * 0x800));
    LoadPalette(sBgPal, 0, sizeof(sBgPal));
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    sRevival.monSprite = CreateMonPicSprite_HandleDeoxys(sRevival.species,
        sRevival.otId, sRevival.personality, TRUE, 120, 72, 0, TAG_NONE);
    if (sRevival.monSprite < MAX_SPRITES)
        gSprites[sRevival.monSprite].invisible = TRUE;
    for (i = 0; i < 2; i++)
    {
        sRevival.itemSprites[i] = MAX_SPRITES;
        if (sRevival.items[i] == ITEM_NONE)
            continue;
        sRevival.itemSprites[i] = AddItemIconObject(TAG_FOSSIL_LEFT + i, TAG_FOSSIL_LEFT + i, sRevival.items[i]);
        if (sRevival.itemSprites[i] < MAX_SPRITES)
        {
            struct Sprite *sprite = &gSprites[sRevival.itemSprites[i]];
            sprite->x = sRevival.items[1] == ITEM_NONE ? 120 : (i == 0 ? 80 : 160);
            sprite->y = 72;
            sprite->callback = SpriteCallbackDummy;
        }
    }
    LoadCompressedSpriteSheet(&sAuraSheet);
    LoadSpritePalette(&sAuraPalette);
    sRevival.frame = 0;
    sRevival.phase = 0;
    BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
    BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
    ShowBg(0);
    SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
    CreateTask(Task_Revival, 0);
    SetVBlankCallback(VBlank_Revival);
    SetMainCallback2(CB2_Revival);
}

static void SpriteCB_RevivalAura(struct Sprite *sprite)
{
    s16 age = ++sprite->data[0];
    u8 angle = sprite->data[1] + age * 2;
    sprite->x2 = Sin(angle, age * 2);
    sprite->y2 = Cos(angle, age);
    if (age >= 32)
        DestroySprite(sprite);
}

static void SpawnRevivalAura(void)
{
    u8 i;
    for (i = 0; i < 8; i++)
    {
        u8 id = CreateSprite(&sAuraTemplate, 120, 72, 0);
        if (id < MAX_SPRITES)
            gSprites[id].data[1] = i * 32;
    }
}

static void Task_Revival(u8 taskId)
{
    u8 i;
    if (gPaletteFade.active)
        return;
    sRevival.frame++;
    switch (sRevival.phase)
    {
    case 0: // Let both shards read clearly before bringing them together.
        if (sRevival.frame < 40)
            return;
        sRevival.phase++;
        sRevival.frame = 0;
        break;
    case 1:
        for (i = 0; i < 2; i++)
            if (sRevival.itemSprites[i] < MAX_SPRITES && sRevival.items[1] != ITEM_NONE)
                gSprites[sRevival.itemSprites[i]].x = 120 + (i == 0 ? -1 : 1) * (40 - sRevival.frame);
        if (sRevival.frame == 40)
        {
            SpawnRevivalAura();
            BeginNormalPaletteFade(PALETTES_OBJECTS, 1, 0, 16, RGB_WHITE);
            sRevival.phase++;
            sRevival.frame = 0;
        }
        break;
    case 2: // Switch sprites at full glow; reveal the actual revived Pokemon.
        for (i = 0; i < 2; i++)
            if (sRevival.itemSprites[i] < MAX_SPRITES)
                gSprites[sRevival.itemSprites[i]].invisible = TRUE;
        if (sRevival.monSprite < MAX_SPRITES)
            gSprites[sRevival.monSprite].invisible = FALSE;
        SpawnRevivalAura();
        BeginNormalPaletteFade(PALETTES_OBJECTS, 1, 16, 0, RGB_WHITE);
        sRevival.phase++;
        sRevival.frame = 0;
        break;
    case 3:
        PlayCry_Normal(sRevival.species, 0);
        sRevival.phase++;
        sRevival.frame = 0;
        break;
    case 4:
        if (sRevival.frame < 90 || IsCryPlaying())
            return;
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        sRevival.phase++;
        break;
    case 5:
        SetVBlankCallback(NULL);
        if (sRevival.monSprite < MAX_SPRITES)
            FreeAndDestroyMonPicSprite(sRevival.monSprite);
        ResetSpriteData();
        FreeAllSpritePalettes();
        DestroyTask(taskId);
        SetMainCallback2(CB2_ReturnToFieldContinueScript);
        break;
    }
}
