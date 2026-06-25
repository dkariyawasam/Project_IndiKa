#include "global.h"
#include "event_data.h"
#include "random.h"
#include "constants/maps.h"

#define MAX_HIDDEN_ITEMS_PER_GROUP 8

struct RenewableHiddenItemData
{
    s8 mapGroup;
    s8 mapNum;
    u8 filler[2];
    u8 rare[MAX_HIDDEN_ITEMS_PER_GROUP];     // 10%
    u8 uncommon[MAX_HIDDEN_ITEMS_PER_GROUP]; // 30%
    u8 common[MAX_HIDDEN_ITEMS_PER_GROUP];   // 60%
};

static void SampleRenewableItemFlags(void);

#define HIDDEN_ID(flag)(flag - FLAG_HIDDEN_ITEMS_START)
#define NO_ITEM 0xFF

static const struct RenewableHiddenItemData sRenewableHiddenItems[] = {
    {
        .mapGroup = MAP_GROUP(MAP_ROUTE20),
        .mapNum = MAP_NUM(MAP_ROUTE20),
        .rare = {
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM
        },
        .uncommon = {
            HIDDEN_ID(FLAG_HIDDEN_ITEM_ROUTE20_STARDUST),
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM
        },
        .common = {
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM
        }
    },
    {
        .mapGroup = MAP_GROUP(MAP_ROUTE21_NORTH),
        .mapNum = MAP_NUM(MAP_ROUTE21_NORTH),
        .rare = {
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM
        },
        .uncommon = {
            HIDDEN_ID(FLAG_HIDDEN_ITEM_ROUTE21_NORTH_PEARL),
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM
        },
        .common = {
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM
        }
    },
    {
        .mapGroup = MAP_GROUP(MAP_UNDERGROUND_PATH_TUNNEL),
        .mapNum = MAP_NUM(MAP_UNDERGROUND_PATH_TUNNEL),
        .rare = {
            HIDDEN_ID(FLAG_HIDDEN_ITEM_UNDERGROUND_PATH_TUNNEL_ETHER_1),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_UNDERGROUND_PATH_TUNNEL_ETHER_2),
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM
        },
        .uncommon = {
            HIDDEN_ID(FLAG_HIDDEN_ITEM_UNDERGROUND_PATH_TUNNEL_POTION_1),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_UNDERGROUND_PATH_TUNNEL_ANTIDOTE_1),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_UNDERGROUND_PATH_TUNNEL_PARALYZE_HEAL_1),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_UNDERGROUND_PATH_TUNNEL_AWAKENING_1),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_UNDERGROUND_PATH_TUNNEL_BURN_HEAL_1),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_UNDERGROUND_PATH_TUNNEL_ICE_HEAL_1),
            NO_ITEM,
            NO_ITEM
        },
        .common = {
            HIDDEN_ID(FLAG_HIDDEN_ITEM_UNDERGROUND_PATH_TUNNEL_POTION_2),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_UNDERGROUND_PATH_TUNNEL_ANTIDOTE_2),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_UNDERGROUND_PATH_TUNNEL_PARALYZE_HEAL_2),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_UNDERGROUND_PATH_TUNNEL_AWAKENING_2),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_UNDERGROUND_PATH_TUNNEL_BURN_HEAL_2),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_UNDERGROUND_PATH_TUNNEL_ICE_HEAL_2),
            NO_ITEM,
            NO_ITEM
        }
    },
    {
        .mapGroup = MAP_GROUP(MAP_MT_MOON_B1F),
        .mapNum = MAP_NUM(MAP_MT_MOON_B1F),
        .rare = {
            HIDDEN_ID(FLAG_HIDDEN_ITEM_MT_MOON_B1F_TINY_MUSHROOM_3),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_MT_MOON_B1F_BIG_MUSHROOM_3),
            NO_ITEM,
            NO_ITEM
        },
        .uncommon = {
            HIDDEN_ID(FLAG_HIDDEN_ITEM_MT_MOON_B1F_TINY_MUSHROOM_3),
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM
        },
        .common = {
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM,
            NO_ITEM
        }
    },
    {
        .mapGroup = MAP_GROUP(MAP_FUCHSIA_FOREST),
        .mapNum = MAP_NUM(MAP_FUCHSIA_FOREST),
        .rare = {
            HIDDEN_ID(FLAG_HIDDEN_ITEM_FUCHSIA_FOREST_BLUK_BERRY),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_FUCHSIA_FOREST_WEPEAR_BERRY),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_FUCHSIA_FOREST_ORAN_BERRY),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_FUCHSIA_FOREST_CHERI_BERRY),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_FUCHSIA_FOREST_ASPEAR_BERRY),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_FUCHSIA_FOREST_PERSIM_BERRY),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_FUCHSIA_FOREST_PINAP_BERRY),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_FUCHSIA_FOREST_LUM_BERRY),
        },
        .uncommon = {
            HIDDEN_ID(FLAG_HIDDEN_ITEM_FUCHSIA_FOREST_BLUK_BERRY),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_FUCHSIA_FOREST_WEPEAR_BERRY),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_FUCHSIA_FOREST_ORAN_BERRY),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_FUCHSIA_FOREST_CHERI_BERRY),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_FUCHSIA_FOREST_ASPEAR_BERRY),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_FUCHSIA_FOREST_PERSIM_BERRY),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_FUCHSIA_FOREST_PINAP_BERRY),
            NO_ITEM
        },
        .common = {
            HIDDEN_ID(FLAG_HIDDEN_ITEM_FUCHSIA_FOREST_RAZZ_BERRY),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_FUCHSIA_FOREST_NANAB_BERRY),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_FUCHSIA_FOREST_CHESTO_BERRY),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_FUCHSIA_FOREST_PECHA_BERRY),
            HIDDEN_ID(FLAG_HIDDEN_ITEM_FUCHSIA_FOREST_RAWST_BERRY),
            NO_ITEM,
            NO_ITEM,
            NO_ITEM
        }
    },
};

void SetAllRenewableItemFlags(void)
{
    u8 i, j;

    for (i = 0; i < ARRAY_COUNT(sRenewableHiddenItems); i++)
    {
        const u8 * rare = sRenewableHiddenItems[i].rare;
        const u8 * uncommon = sRenewableHiddenItems[i].uncommon;
        const u8 * common = sRenewableHiddenItems[i].common;
        for (j = 0; j < MAX_HIDDEN_ITEMS_PER_GROUP; j++)
        {
            if (rare[j] != NO_ITEM)
                FlagSet(FLAG_HIDDEN_ITEMS_START + rare[j]);
            if (uncommon[j] != NO_ITEM)
                FlagSet(FLAG_HIDDEN_ITEMS_START + uncommon[j]);
            if (common[j] != NO_ITEM)
                FlagSet(FLAG_HIDDEN_ITEMS_START + common[j]);
        }
    }
}

void IncrementRenewableHiddenItemStepCounter(void)
{
    u16 var = VarGet(VAR_RENEWABLE_ITEM_STEP_COUNTER);
    if (var < 1500)
    {
        VarSet(VAR_RENEWABLE_ITEM_STEP_COUNTER, var + 1);
    }
}

void TryRegenerateRenewableHiddenItems(void)
{
    u8 i;
    u8 found_map = 0xFF;
    for (i = 0; i < ARRAY_COUNT(sRenewableHiddenItems); i++)
    {
        if (sRenewableHiddenItems[i].mapGroup == gSaveBlock1Ptr->location.mapGroup
         && sRenewableHiddenItems[i].mapNum == gSaveBlock1Ptr->location.mapNum)
            found_map = i;
    }

    if (found_map == 0xFF)
        return;
    if (VarGet(VAR_RENEWABLE_ITEM_STEP_COUNTER) >= 1500)
    {
        VarSet(VAR_RENEWABLE_ITEM_STEP_COUNTER, 0);
        SetAllRenewableItemFlags();
        SampleRenewableItemFlags();
    }
}

static void SampleRenewableItemFlags(void)
{
    u8 i, j;
    const u8 * flags;
    u16 rval;

    for (i = 0; i < ARRAY_COUNT(sRenewableHiddenItems); i++)
    {
        rval = Random() % 100;
        if (rval >= 90)
            flags = sRenewableHiddenItems[i].rare;
        else if (rval >= 60)
            flags = sRenewableHiddenItems[i].uncommon;
        else
            flags = sRenewableHiddenItems[i].common;
        for (j = 0; j < MAX_HIDDEN_ITEMS_PER_GROUP; j++)
        {
            if (flags[j] != NO_ITEM)
                FlagClear(FLAG_HIDDEN_ITEMS_START + flags[j]);
        }
    }
}
