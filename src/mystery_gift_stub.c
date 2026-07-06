#include "global.h"
#include "gflib.h"
#include "event_data.h"
#include "main.h"
#include "main_menu.h"
#include "task.h"
#include "constants/mystery_gift.h"

void ClearMysteryGift(void)
{
    CpuFill32(0, &gSaveBlock1Ptr->mysteryGift, sizeof(gSaveBlock1Ptr->mysteryGift));
}

bool32 ValidateSavedWonderCard(void)
{
    return FALSE;
}

bool32 ValidateSavedWonderNews(void)
{
    return FALSE;
}

u16 GetWonderCardFlagId(void)
{
    return 0;
}

u16 MysteryGift_GetCardStat(u32 stat)
{
    return 0;
}

void MysteryGift_DisableStats(void)
{
}

bool32 MysteryGift_TryEnableStatsByFlagId(u16 flagId)
{
    return FALSE;
}

void MysteryGift_TryIncrementStat(u32 stat, u32 trainerId)
{
}

void WonderNews_IncrementStepCounter(void)
{
}

u16 *GetQuestionnaireWordsPtr(void)
{
    return gSaveBlock1Ptr->mysteryGift.questionnaireWords;
}

u32 WonderNews_GetRewardInfo(void)
{
    gSpecialVar_Result = 0;
    return 0;
}

void AddTextPrinterToWindow1(const u8 *str)
{
}

void MG_DrawTextBorder(u8 windowId)
{
}

bool32 PrintMysteryGiftMenuMessage(u8 *state, const u8 *src)
{
    return TRUE;
}

s8 DoMysteryGiftYesNo(u8 *textState, u16 *windowId, bool8 yesNoBoxPlacement, const u8 *str)
{
    return 1;
}

u16 GetMysteryGiftBaseBlock(void)
{
    return 0;
}

void CB2_InitMysteryGift(void)
{
    SetMainCallback2(CB2_InitMainMenu);
}

void CB2_MysteryGiftEReader(void)
{
    SetMainCallback2(CB2_InitMainMenu);
}

void Task_MysteryGift(u8 taskId)
{
    DestroyTask(taskId);
}
