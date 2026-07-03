#include "global.h"
#include "help_system.h"

void HelpSystem_Disable(void)
{
    gHelpSystemEnabled = FALSE;
}

void HelpSystem_Enable(void)
{
    gHelpSystemEnabled = TRUE;
    HelpSystem_EnableToggleWithRButton();
}

void HelpSystem_DisableToggleWithRButton(void)
{
    gHelpSystemToggleWithRButtonDisabled = TRUE;
}

void HelpSystem_EnableToggleWithRButton(void)
{
    gHelpSystemToggleWithRButtonDisabled = FALSE;
}
