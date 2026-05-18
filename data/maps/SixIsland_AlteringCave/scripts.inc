SixIsland_AlteringCave_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SixIsland_AlteringCave_OnTransition
	.byte 0

SixIsland_AlteringCave_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_SIX_ISLAND_ALTERING_CAVE
	goto_if_ge VAR_MISTY_TRIAL_STATE, 3, SixIsland_AlteringCave_EventScript_HideMisty
	clearflag FLAG_HIDE_POWER_PLANT_MISTY
	end

SixIsland_AlteringCave_EventScript_HideMisty::
	setflag FLAG_HIDE_POWER_PLANT_MISTY
	end

SixIsland_AlteringCave_EventScript_Misty::
	lock
	faceplayer
	textcolor NPC_TEXT_COLOR_NEUTRAL
	goto_if_eq VAR_MISTY_TRIAL_STATE, 0, SixIsland_AlteringCave_EventScript_UnlockMistyQuest
	goto SixIsland_AlteringCave_EventScript_MistyFound

SixIsland_AlteringCave_EventScript_UnlockMistyQuest::
	questmenu QUEST_MENU_UNLOCK_QUEST, QUEST_GYM_LEADER_TRIALS
	questmenu QUEST_MENU_SET_ACTIVE, QUEST_GYM_LEADER_TRIALS
	subquestmenu QUEST_MENU_UNLOCK_QUEST, QUEST_GYM_LEADER_TRIALS, SUB_QUEST_MISTY
	setvar VAR_MISTY_TRIAL_STATE, 2

SixIsland_AlteringCave_EventScript_MistyFound::
	msgbox SixIsland_AlteringCave_Text_MistyFound
	setvar VAR_MISTY_TRIAL_STATE, 3
	clearflag FLAG_HIDE_CERULEAN_CITY_GYM_MISTY
	setflag FLAG_HIDE_POWER_PLANT_MISTY
	removeobject LOCALID_ROCK_TUNNEL_MISTY
	release
	end

SixIsland_AlteringCave_Text_MistyFound::
	.string "Oh, hey!\n"
	.string "You're all the way out here, too?\p"
	.string "I've been studying how the water\n"
	.string "cuts through stone in this tunnel.\p"
	.string "The currents around CERULEAN are\n"
	.string "stronger than they look.\p"
	.string "I've got what I need.\n"
	.string "I'll head back to the GYM now.$"
