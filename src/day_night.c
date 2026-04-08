#include "global.h"
#include "event_data.h"
#include "day_night.h"
#include "constants/vars.h"
#include "constants/time.h"

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