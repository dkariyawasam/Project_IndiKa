#ifndef GUARD_APEX_LOG_H
#define GUARD_APEX_LOG_H

#include "main.h"
#include "constants/apex_log.h"

enum {
    FCWINDOWID_LIST,
    FCWINDOWID_UIHELP,
    FCWINDOWID_MSGBOX,
    FCWINDOWID_ICONDESC
};

extern struct ListMenuTemplate gApexLog_ListMenuTemplate;
extern u8 gIconDescriptionBoxIsOpen;

void ResetApexLog(void);
void FullyUnlockApexLog(void);
void UseApexLog(MainCallback savedCallback);
void UseApexRumorDossier(MainCallback savedCallback, u8 apexSubquest);
void SetFlavorTextFlagFromSpecialVars(void);
void UpdatePickStateFromSpecialVar8005(void);

#endif //GUARD_APEX_LOG_H
