#ifndef GUARD_DAY_NIGHT_H
#define GUARD_DAY_NIGHT_H

void InitDayNightCycle(void);
void UpdateDayNightCycleStep(void);
u8 GetTimeOfDay(void);
bool8 IsDayTime(void);
bool8 IsNightTime(void);

#endif // GUARD_DAY_NIGHT_H