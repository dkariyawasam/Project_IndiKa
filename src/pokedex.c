#include "global.h"
#include "pokedex.h"
#include "pokedex_screen.h"

#include "data/pokemon/active_pokedex.h"

bool8 IsActivePokedexSpecies(u16 nationalDexNo)
{
    return nationalDexNo <= NATIONAL_DEX_COUNT && sActivePokedexSpecies[nationalDexNo];
}

u16 GetActivePokedexCount(void)
{
    u16 i, count = 0;
    for (i = 1; i <= NATIONAL_DEX_COUNT; i++)
        if (IsActivePokedexSpecies(i))
            count++;
    return count;
}

// Unused
const u8 *GetPokedexCategoryName(u16 dexNum)
{
    return gPokedexEntries[dexNum].categoryName;
}

u16 GetPokedexHeightWeight(u16 dexNum, u8 data)
{
    switch (data)
    {
    case 0:  // height
        return gPokedexEntries[dexNum].height;
    case 1:  // weight
        return gPokedexEntries[dexNum].weight;
    default:
        return 1;
    }
}

s8 GetSetPokedexFlag(u16 nationalDexNo, u8 caseID)
{
    return DexScreen_GetSetPokedexFlag(nationalDexNo, caseID, 0);
}

u16 GetNationalPokedexCount(u8 caseID)
{
    u16 count = 0;
    u16 i;

    for (i = 0; i < NATIONAL_DEX_COUNT; i++)
    {
        if (!IsActivePokedexSpecies(i + 1))
            continue;
        switch (caseID)
        {
        case FLAG_GET_SEEN:
            if (GetSetPokedexFlag(i + 1, FLAG_GET_SEEN))
                count++;
            break;
        case FLAG_GET_CAUGHT:
            if (GetSetPokedexFlag(i + 1, FLAG_GET_CAUGHT))
                count++;
            break;
        }
    }
    return count;
}

u16 CompletePokedex(void)
{
    u16 i;

    for (i = 1; i <= NATIONAL_DEX_COUNT; i++)
    {
        if (!IsActivePokedexSpecies(i))
            continue;
        GetSetPokedexFlag(i, FLAG_SET_SEEN);
        GetSetPokedexFlag(i, FLAG_SET_CAUGHT);
    }

    return 0;
}

/*
u16 GetHoennPokedexCount(u8 caseID)
{
    u16 count = 0;
    u16 i;

    for (i = 0; i < HOENN_DEX_COUNT; i++)
    {
        if (!IsActivePokedexSpecies(i + 1))
            continue;
        switch (caseID)
        {
        case FLAG_GET_SEEN:
            if (GetSetPokedexFlag(HoennToNationalOrder(i + 1), FLAG_GET_SEEN))
                count++;
            break;
        case FLAG_GET_CAUGHT:
            if (GetSetPokedexFlag(HoennToNationalOrder(i + 1), FLAG_GET_CAUGHT))
                count++;
            break;
        }
    }
    return count;
}
*/

u16 GetKantoPokedexCount(u8 caseID)
{
    u16 count = 0;
    u16 i;

    for (i = 0; i < KANTO_DEX_COUNT; i++)
    {
        if (!IsActivePokedexSpecies(i + 1))
            continue;
        switch (caseID)
        {
        case FLAG_GET_SEEN:
            if (GetSetPokedexFlag(i + 1, FLAG_GET_SEEN))
                count++;
            break;
        case FLAG_GET_CAUGHT:
            if (GetSetPokedexFlag(i + 1, FLAG_GET_CAUGHT))
                count++;
            break;
        }
    }
    return count;
}

bool16 HasAllHoennMons(void)
{
    u16 i;

    // -2 excludes Jirachi and Deoxys
    for (i = 0; i < HOENN_DEX_COUNT - 2; i++)
    {
        if (!GetSetPokedexFlag(HoennToNationalOrder(i + 1), FLAG_GET_CAUGHT))
            return FALSE;
    }
    return TRUE;
}

bool16 HasAllKantoMons(void)
{
    u16 i;

    // Preserve the existing mythical exception using its ID, not its position.
    for (i = 1; i <= NATIONAL_DEX_COUNT; i++)
    {
        if (i == NATIONAL_DEX_MEW || !IsActivePokedexSpecies(i))
            continue;
        if (!GetSetPokedexFlag(i, FLAG_GET_CAUGHT))
            return FALSE;
    }
    return TRUE;
}

bool16 HasAllMons(void)
{
    return HasAllKantoMons();
}
