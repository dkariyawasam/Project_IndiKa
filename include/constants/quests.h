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
#define QUEST_GIOVANNIS_AMBITION                2
#define QUEST_THE_ROCKET_LEAGUE                 3
#define QUEST_THE_INDIGO_LEAGUE                 4
#define QUEST_COUNT     (QUEST_THE_INDIGO_LEAGUE + 1)

#define SUB_QUEST_EVOLUTION_THROUGH_BOND        0
#define SUB_QUEST_EVOLUTION_THROUGH_INSTINCT    1
#define SUB_QUEST_EVOLUTION_THROUGH_DESIGN      2
#define SUB_QUEST_BROCK                         3
#define SUB_QUEST_MISTY                         4
#define SUB_QUEST_LT_SURGE                      5
#define SUB_QUEST_ERIKA                         6
#define SUB_QUEST_KOGA                          7
#define SUB_QUEST_SABRINA                       8
#define SUB_QUEST_BLAINE                        9
#define SUB_QUEST_GIOVANNI                      10
#define SUB_QUEST_12        11
#define SUB_QUEST_13        12
#define SUB_QUEST_14        13
#define SUB_QUEST_15        14
#define SUB_QUEST_16        15
#define SUB_QUEST_17        16
#define SUB_QUEST_18        17
#define SUB_QUEST_19        18
#define SUB_QUEST_20        19
#define SUB_QUEST_21        20
#define SUB_QUEST_22        21
#define SUB_QUEST_23        22
#define SUB_QUEST_24        23
#define SUB_QUEST_25        24
#define SUB_QUEST_26        25
#define SUB_QUEST_27        26
#define SUB_QUEST_28        27
#define SUB_QUEST_29        28
#define SUB_QUEST_30        29

#define QUEST_1_SUB_COUNT 3
#define QUEST_2_SUB_COUNT 8
#define SUB_QUEST_COUNT (QUEST_1_SUB_COUNT + QUEST_2_SUB_COUNT)

#define QUEST_ARRAY_COUNT (SUB_QUEST_COUNT > QUEST_COUNT ? SUB_QUEST_COUNT : QUEST_COUNT)
#endif // GUARD_CONSTANTS_QUESTS_H