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

static EWRAM_DATA u16 sBaseMapPalettes[NUM_PALS_TOTAL * 16];
static EWRAM_DATA bool8 sBaseMapPalettesValid;

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
    steps++;

    if (steps >= DAY_NIGHT_STEP_THRESHOLD)
    {
        steps = 0;

        if (VarGet(VAR_TIME_OF_DAY) == TIME_DAY)
            VarSet(VAR_TIME_OF_DAY, TIME_NIGHT);
        else
            VarSet(VAR_TIME_OF_DAY, TIME_DAY);
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
        gPlttBufferUnfaded[i] = tinted;
        gPlttBufferFaded[i] = tinted;
    }
}

void RestoreDayPaletteForCurrentMap(void)
{
    if (!sBaseMapPalettesValid)
        return;

    CpuFastCopy(sBaseMapPalettes, gPlttBufferUnfaded, NUM_PALS_TOTAL * 16 * sizeof(u16));
    CpuFastCopy(sBaseMapPalettes, gPlttBufferFaded,   NUM_PALS_TOTAL * 16 * sizeof(u16));
}

void RefreshCurrentMapNightPalette(void)
{
    if (!DoesCurrentMapUseNightPalette())
        return;

    if (IsNightTime())
        ApplyNightPaletteToCurrentMap();
    else
        RestoreDayPaletteForCurrentMap();
}

bool8 DoesCurrentMapUseNightPalette(void)
{
    switch (gMapHeader.regionMapSectionId)
    {
    case MAPSEC_PALLET_TOWN:
    case MAPSEC_ROUTE_1:
    case MAPSEC_VIRIDIAN_CITY:
        return TRUE;
    default:
        return FALSE;
    }
}

void CacheCurrentMapBasePalettes(void)
{
    CpuFastCopy(gPlttBufferUnfaded, sBaseMapPalettes, NUM_PALS_TOTAL * 16 * sizeof(u16));
    sBaseMapPalettesValid = TRUE;
}