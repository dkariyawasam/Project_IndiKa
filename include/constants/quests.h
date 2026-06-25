#ifndef GUARD_CONSTANTS_QUESTS_H
#define GUARD_CONSTANTS_QUESTS_H

//questmenu scripting command params
#define QUEST_MENU_OPEN                 0   //opens the quest menu (questId = 0)
#define QUEST_MENU_UNLOCK_QUEST         1   //questId = QUEST_X (0-indexed)
#define QUEST_MENU_SET_ACTIVE           2   //questId = QUEST_X (0-indexed)
#define QUEST_MENU_SET_REWARD           3   //questId = QUEST_X (0-indexed)
#define QUEST_MENU_COMPLETE_QUEST       4   //questId = QUEST_X (0-indexed)
#define QUEST_MENU_CHECK_UNLOCKED       5   //checks if questId has been unlocked. Returns result to gSpecialVar_Result
#define QUEST_MENU_CHECK_INACTIVE       6 //check if a questID is inactive. Returns result to gSpecialVar_Result
#define QUEST_MENU_CHECK_ACTIVE         7   //checks if questId has been unlocked. Returns result to gSpecialVar_Result
#define QUEST_MENU_CHECK_REWARD         8  //checks if questId is in Reward state. Returns result to gSpecialVar_Result
#define QUEST_MENU_CHECK_COMPLETE       9   //checks if questId has been completed. Returns result to gSpecialVar_Result
#define QUEST_MENU_BUFFER_QUEST_NAME    10   //buffers a quest name to gStringVar1

// quest number defines
#define QUEST_THE_NATURE_OF_EVOLUTION           0
#define QUEST_GYM_LEADER_TRIALS                 1
#define QUEST_APEX_POKEMON                      2
#define QUEST_THE_ROCKET_LEAGUE                 3
#define QUEST_THE_INDIGO_LEAGUE                 4
#define QUEST_COUNT     (QUEST_THE_INDIGO_LEAGUE + 1)

#define SUB_QUEST_EVOLUTION_THROUGH_BOND        0
#define SUB_QUEST_EVOLUTION_THROUGH_INSTINCT    1
#define SUB_QUEST_EVOLUTION_THROUGH_DESIGN      2
#define SUB_QUEST_BROCK                         0
#define SUB_QUEST_MISTY                         1
#define SUB_QUEST_LTSURGE                       2
#define SUB_QUEST_ERIKA                         3
#define SUB_QUEST_KOGA                          4
#define SUB_QUEST_SABRINA                       5
#define SUB_QUEST_BLAINE                        6
#define SUB_QUEST_GIOVANNI                      7
#define SUB_QUEST_APEX_TANGROWTH                0
#define SUB_QUEST_APEX_ZAPDOS                   1
#define SUB_QUEST_APEX_ARTICUNO                 2
#define SUB_QUEST_APEX_MEWTWO                   3
#define SUB_QUEST_APEX_OSSCYTHE                 4
#define SUB_QUEST_APEX_MOLTRES                  5
#define SUB_QUEST_APEX_MIME_SR                  6
#define SUB_QUEST_APEX_ANNIHILAPE               7
#define QUEST_1_SUB_COUNT 3
#define QUEST_2_SUB_COUNT 8
#define QUEST_3_SUB_COUNT 8
#define SUB_QUEST_COUNT (QUEST_1_SUB_COUNT + QUEST_2_SUB_COUNT + QUEST_3_SUB_COUNT)

#define QUEST_ARRAY_COUNT (SUB_QUEST_COUNT > QUEST_COUNT ? SUB_QUEST_COUNT : QUEST_COUNT)
#endif // GUARD_CONSTANTS_QUESTS_H
