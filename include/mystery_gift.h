#ifndef GUARD_MYSTERY_GIFT_H
#define GUARD_MYSTERY_GIFT_H

#include "global.h"
#include "constants/mystery_gift.h"

bool32 ValidateSavedWonderNews(void);
bool32 ValidateSavedWonderCard(void);
void ClearMysteryGift(void);
u16 MysteryGift_GetCardStat(u32 stat);
void MysteryGift_TryIncrementStat(u32 stat, u32 trainerId);
u16 *GetQuestionnaireWordsPtr(void);
void MysteryGift_DisableStats(void);
bool32 MysteryGift_TryEnableStatsByFlagId(u16 flagId);
u16 GetWonderCardFlagId(void);

#endif // GUARD_MYSTERY_GIFT_H
