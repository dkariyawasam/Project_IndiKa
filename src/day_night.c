#include "global.h"
#include "fieldmap.h"
#include "field_weather.h"
#include "event_data.h"
#include "day_night.h"
#include "constants/vars.h"
#include "constants/time.h"
#include "constants/region_map_sections.h"
#include "gba/gba.h"
#include "palette.h"
#include "sprite.h"
#include "field_effect.h"
#include "constants/maps.h"

static EWRAM_DATA u16 sBaseMapPalettes[NUM_PALS_TOTAL * 16];
static EWRAM_DATA bool8 sBaseMapPalettesValid;
static EWRAM_DATA bool8 sTimeTransitionActive;
static EWRAM_DATA u8 sTimeTransitionTimer;
static EWRAM_DATA u8 sTimeTransitionStage;
static EWRAM_DATA u8 sTimeTransitionTarget;

void InitDayNightCycle(void)
{
    u16 time = VarGet(VAR_TIME_OF_DAY);

    if (time != TIME_DAY && time != TIME_NIGHT)
        VarSet(VAR_TIME_OF_DAY, TIME_DAY);
}

u8 GetTimeOfDay(void)
{
    return VarGet(VAR_TIME_OF_DAY);
}

bool8 IsDayTime(void)
{
    return GetTimeOfDay() == TIME_DAY;
}

bool8 IsNightTime(void)
{
    return GetTimeOfDay() == TIME_NIGHT;
}

void UpdateDayNightCycleStep(void)
{
    u16 steps = VarGet(VAR_DAYNIGHT_STEP_COUNTER);

    if (!DoesCurrentMapUseNightPalette())
        return;

    steps++;

    if (steps >= DAY_NIGHT_STEP_THRESHOLD)
    {
        steps = 0;

        if (!sTimeTransitionActive)
        {
            if (VarGet(VAR_TIME_OF_DAY) == TIME_DAY)
                StartTimeOfDayTransition(TIME_NIGHT);
            else
                StartTimeOfDayTransition(TIME_DAY);
        }
    }

    VarSet(VAR_DAYNIGHT_STEP_COUNTER, steps);
}

static u16 TintColorNight(u16 color)
{
    u8 r = color & 0x1F;
    u8 g = (color >> 5) & 0x1F;
    u8 b = (color >> 10) & 0x1F;

    // Subtle darkening, slight cool bias
    r = (r * 18) / 32;
    g = (g * 20) / 32;
    b = (b * 25) / 32;

    return RGB(r, g, b);
}

void ApplyNightPaletteToCurrentMap(void)
{
    int i;

    if (!sBaseMapPalettesValid)
        return;

    for (i = 0; i < NUM_PALS_TOTAL * 16; i++)
    {
        u16 tinted = TintColorNight(sBaseMapPalettes[i]);
        gPlttBufferFaded[i] = tinted;
    }

    CpuFastCopy(gPlttBufferFaded, (void *)BG_PLTT, NUM_PALS_TOTAL * 16 * sizeof(u16));
}

void RestoreDayPaletteForCurrentMap(void)
{
    if (!sBaseMapPalettesValid)
        return;

    CpuFastCopy(sBaseMapPalettes, gPlttBufferUnfaded, NUM_PALS_TOTAL * 16 * sizeof(u16));
    CpuFastCopy(sBaseMapPalettes, gPlttBufferFaded,   NUM_PALS_TOTAL * 16 * sizeof(u16));
    CpuFastCopy(gPlttBufferFaded, (void *)BG_PLTT, NUM_PALS_TOTAL * 16 * sizeof(u16));
}

void RefreshCurrentMapNightPalette(void)
{
    if (!DoesCurrentMapUseNightPalette())
    {
        RestoreDayPaletteForCurrentMap();
        return;
    }

    if (IsNightTime())
        ApplyNightPaletteToCurrentMap();
    else
        RestoreDayPaletteForCurrentMap();
}

struct DayNightMapEntry
{
    u8 mapGroup;
    u8 mapNum;
};

static const struct DayNightMapEntry sDayNightMaps[] =
{
    { MAP_GROUP(MAP_PALLET_TOWN),  MAP_NUM(MAP_PALLET_TOWN)         },
    { MAP_GROUP(MAP_LAVENDER_TOWN),MAP_NUM(MAP_LAVENDER_TOWN)       },
    { MAP_GROUP(MAP_VIRIDIAN_CITY),MAP_NUM(MAP_VIRIDIAN_CITY)       },
    { MAP_GROUP(MAP_PEWTER_CITY),  MAP_NUM(MAP_PEWTER_CITY)         },
    { MAP_GROUP(MAP_CERULEAN_CITY),MAP_NUM(MAP_CERULEAN_CITY)       },
    { MAP_GROUP(MAP_VERMILION_CITY),MAP_NUM(MAP_VERMILION_CITY)     },
    { MAP_GROUP(MAP_CELADON_CITY),  MAP_NUM(MAP_CELADON_CITY)       },
    { MAP_GROUP(MAP_FUCHSIA_CITY),  MAP_NUM(MAP_FUCHSIA_CITY)       },
    { MAP_GROUP(MAP_CINNABAR_ISLAND),MAP_NUM(MAP_CINNABAR_ISLAND)   },
    { MAP_GROUP(MAP_INDIGO_PLATEAU_EXTERIOR),MAP_NUM(MAP_INDIGO_PLATEAU_EXTERIOR)     },
    { MAP_GROUP(MAP_SAFFRON_CITY),   MAP_NUM(MAP_SAFFRON_CITY)      },
    { MAP_GROUP(MAP_SAFFRON_CITY_CONNECTION),MAP_NUM(MAP_SAFFRON_CITY_CONNECTION) },
    { MAP_GROUP(MAP_ROUTE1),       MAP_NUM(MAP_ROUTE1)              },
    { MAP_GROUP(MAP_ROUTE2),       MAP_NUM(MAP_ROUTE2)              },
    { MAP_GROUP(MAP_ROUTE3),       MAP_NUM(MAP_ROUTE3)              },
    { MAP_GROUP(MAP_ROUTE4),       MAP_NUM(MAP_ROUTE4)              },
    { MAP_GROUP(MAP_ROUTE5),       MAP_NUM(MAP_ROUTE5)              },
    { MAP_GROUP(MAP_ROUTE6),       MAP_NUM(MAP_ROUTE6)              },
    { MAP_GROUP(MAP_ROUTE7),       MAP_NUM(MAP_ROUTE7)              },
    { MAP_GROUP(MAP_ROUTE8),       MAP_NUM(MAP_ROUTE8)              },
    { MAP_GROUP(MAP_ROUTE9),       MAP_NUM(MAP_ROUTE9)              },
    { MAP_GROUP(MAP_ROUTE10),      MAP_NUM(MAP_ROUTE10)             },
    { MAP_GROUP(MAP_ROUTE11),      MAP_NUM(MAP_ROUTE11)             },
    { MAP_GROUP(MAP_ROUTE12),      MAP_NUM(MAP_ROUTE12)             },
    { MAP_GROUP(MAP_ROUTE13),      MAP_NUM(MAP_ROUTE13)             },
    { MAP_GROUP(MAP_ROUTE14),      MAP_NUM(MAP_ROUTE14)             },
    { MAP_GROUP(MAP_ROUTE15),      MAP_NUM(MAP_ROUTE15)             },
    { MAP_GROUP(MAP_ROUTE16),      MAP_NUM(MAP_ROUTE16)             },
    { MAP_GROUP(MAP_ROUTE17),      MAP_NUM(MAP_ROUTE17)             },
    { MAP_GROUP(MAP_ROUTE18),      MAP_NUM(MAP_ROUTE18)             },
    { MAP_GROUP(MAP_ROUTE19),      MAP_NUM(MAP_ROUTE19)             },
    { MAP_GROUP(MAP_ROUTE20),      MAP_NUM(MAP_ROUTE20)             },
    { MAP_GROUP(MAP_ROUTE21_NORTH),MAP_NUM(MAP_ROUTE21_NORTH)       }, 
    { MAP_GROUP(MAP_ROUTE21_SOUTH),MAP_NUM(MAP_ROUTE21_SOUTH)       },
    { MAP_GROUP(MAP_ROUTE22),      MAP_NUM(MAP_ROUTE22)             },
    { MAP_GROUP(MAP_ROUTE23),      MAP_NUM(MAP_ROUTE23)             },
    { MAP_GROUP(MAP_ROUTE24),      MAP_NUM(MAP_ROUTE24)             },
    { MAP_GROUP(MAP_ROUTE25),      MAP_NUM(MAP_ROUTE25)             },
    { MAP_GROUP(MAP_SAFARI_ZONE_CENTER),MAP_NUM(MAP_SAFARI_ZONE_CENTER) },
    { MAP_GROUP(MAP_SAFARI_ZONE_EAST),MAP_NUM(MAP_SAFARI_ZONE_EAST) },
    { MAP_GROUP(MAP_SAFARI_ZONE_NORTH),MAP_NUM(MAP_SAFARI_ZONE_NORTH) },
    { MAP_GROUP(MAP_SAFARI_ZONE_WEST),MAP_NUM(MAP_SAFARI_ZONE_WEST) },
};

bool8 DoesCurrentMapUseNightPalette(void)
{
    u32 i;
    u8 group = gSaveBlock1Ptr->location.mapGroup;
    u8 num   = gSaveBlock1Ptr->location.mapNum;

    for (i = 0; i < ARRAY_COUNT(sDayNightMaps); i++)
    {
        if (group == sDayNightMaps[i].mapGroup
         && num   == sDayNightMaps[i].mapNum)
            return TRUE;
    }

    return FALSE;
}

void CacheCurrentMapBasePalettes(void)
{
    CpuFastCopy(gPlttBufferUnfaded, sBaseMapPalettes, NUM_PALS_TOTAL * 16 * sizeof(u16));
    sBaseMapPalettesValid = TRUE;
}

void ApplyNightTintToTallGrassEffect(void)
{
    u8 slot = IndexOfSpritePaletteTag(FLDEFF_PAL_TAG_GENERAL_1);
    u16 base;
    int i;

    if (slot == 0xFF)
        return;

    base = OBJ_PLTT_ID(slot);

    for (i = 0; i < 16; i++)
        gPlttBufferFaded[base + i] = TintColorNight(gPlttBufferUnfaded[base + i]);

    CpuFastCopy(&gPlttBufferFaded[base], (void *)(OBJ_PLTT + base * sizeof(u16)), 16 * sizeof(u16));
}

static u16 BlendTowardNight(u16 dayColor, u8 blend, u8 maxBlend)
{
    u16 nightColor = TintColorNight(dayColor);

    u8 dayR = dayColor & 0x1F;
    u8 dayG = (dayColor >> 5) & 0x1F;
    u8 dayB = (dayColor >> 10) & 0x1F;

    u8 nightR = nightColor & 0x1F;
    u8 nightG = (nightColor >> 5) & 0x1F;
    u8 nightB = (nightColor >> 10) & 0x1F;

    u8 r = dayR + (((s8)nightR - (s8)dayR) * blend) / maxBlend;
    u8 g = dayG + (((s8)nightG - (s8)dayG) * blend) / maxBlend;
    u8 b = dayB + (((s8)nightB - (s8)dayB) * blend) / maxBlend;

    return RGB(r, g, b);
}

static void ApplyTimeBlendToCurrentMap(u8 blend, u8 maxBlend)
{
    int i;

    if (!sBaseMapPalettesValid || !DoesCurrentMapUseNightPalette())
        return;

    for (i = 0; i < NUM_PALS_TOTAL * 16; i++)
    {
        u16 color = BlendTowardNight(sBaseMapPalettes[i], blend, maxBlend);
        gPlttBufferFaded[i] = color;
    }

    CpuFastCopy(gPlttBufferFaded, (void *)BG_PLTT, NUM_PALS_TOTAL * 16 * sizeof(u16));
}

static void ApplyTransitionStage(u8 stage, u8 targetTime)
{
    switch (targetTime)
    {
    case TIME_NIGHT:
        switch (stage)
        {
        case 0:
            ApplyTimeBlendToCurrentMap(0, 16);   // Day
            break;
        case 1:
            ApplyTimeBlendToCurrentMap(5, 16);   // Tint 1
            break;
        case 2:
            ApplyTimeBlendToCurrentMap(10, 16);  // Tint 2
            break;
        case 3:
            ApplyTimeBlendToCurrentMap(16, 16);  // Night
            break;
        }
        break;

    case TIME_DAY:
        switch (stage)
        {
        case 0:
            ApplyTimeBlendToCurrentMap(16, 16);  // Night
            break;
        case 1:
            ApplyTimeBlendToCurrentMap(10, 16);  // Tint 2
            break;
        case 2:
            ApplyTimeBlendToCurrentMap(5, 16);   // Tint 1
            break;
        case 3:
            ApplyTimeBlendToCurrentMap(0, 16);   // Day
            break;
        }
        break;
    }
}

void StartTimeOfDayTransition(u8 targetTime)
{
    if (!DoesCurrentMapUseNightPalette())
    {
        VarSet(VAR_TIME_OF_DAY, targetTime);
        RefreshCurrentMapNightPalette();
        return;
    }

    sTimeTransitionActive = TRUE;
    sTimeTransitionTimer = 0;
    sTimeTransitionStage = 0;
    sTimeTransitionTarget = targetTime;

    ApplyTransitionStage(0, targetTime);
}

void UpdateTimeOfDayTransition(void)
{
    if (!sTimeTransitionActive)
        return;

    if (!DoesCurrentMapUseNightPalette())
    {
        sTimeTransitionActive = FALSE;
        VarSet(VAR_TIME_OF_DAY, sTimeTransitionTarget);
        RestoreDayPaletteForCurrentMap();
        return;
    }

    sTimeTransitionTimer++;

    if (sTimeTransitionTimer < 20)
        return;

    sTimeTransitionTimer = 0;
    sTimeTransitionStage++;

    if (sTimeTransitionStage >= 4)
    {
        sTimeTransitionActive = FALSE;
        VarSet(VAR_TIME_OF_DAY, sTimeTransitionTarget);
        RefreshCurrentMapNightPalette();
        return;
    }

    ApplyTransitionStage(sTimeTransitionStage, sTimeTransitionTarget);
}
