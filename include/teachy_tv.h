#ifndef GUARD_TEACHY_TV_H
#define GUARD_TEACHY_TV_H

enum TeachyTvScript
{
    TTVSCR_BATTLE,
    TTVSCR_STATUS,
    TTVSCR_MATCHUPS,
    TTVSCR_CATCHING,
    TTVSCR_TMS,
    TTVSCR_MATCHUPS_THEORY,
    TTVSCR_BATTLE_THEORY,
    TTVSCR_CATCHING_THEORY,
    TTVSCR_STATUS_THEORY,
    TTVSCR_TRAINING
};

void InitTeachyTvController(u8 mode, void (*cb)());
void CB2_ReturnToTeachyTV(void);
void SetTeachyTvControllerModeToResume(void);

#endif //GUARD_TEACHY_TV_H
