#ifndef GUARD_HELP_MENU_H
#define GUARD_HELP_MENU_H

enum HelpMenuScript
{
    HELPSCR_BATTLE,
    HELPSCR_STATUS,
    HELPSCR_MATCHUPS,
    HELPSCR_CATCHING,
    HELPSCR_TMS,
    HELPSCR_MATCHUPS_THEORY,
    HELPSCR_BATTLE_THEORY,
    HELPSCR_CATCHING_THEORY,
    HELPSCR_STATUS_THEORY,
    HELPSCR_TRAINING
};

void InitHelpMenuController(u8 mode, void (*cb)());
void CB2_ReturnToHelpMenu(void);
void SetHelpMenuControllerModeToResume(void);

#endif //GUARD_HELP_MENU_H
