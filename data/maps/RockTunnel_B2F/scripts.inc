RockTunnel_B2F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, RockTunnel_B2F_OnTransition
	.byte 0

RockTunnel_B2F_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_ROCK_TUNNEL_1F
	goto_if_ge VAR_MISTY_TRIAL_STATE, 7, RockTunnel_B2F_EventScript_HideMisty
	goto_if_eq VAR_MISTY_TRIAL_STATE, 6, RockTunnel_B2F_EventScript_PlaceMistyAtSecondGenerator
	goto_if_ge VAR_MISTY_TRIAL_STATE, 4, RockTunnel_B2F_EventScript_PlaceMistyAtLowerFalls
	goto_if_eq VAR_MISTY_TRIAL_STATE, 3, RockTunnel_B2F_EventScript_PlaceMistyAtGenerator
	clearflag FLAG_HIDE_POWER_PLANT_MISTY
	setobjectgfx LOCALID_ROCK_TUNNEL_MISTY, OBJ_EVENT_GFX_MISTY_WATER
	end

RockTunnel_B2F_EventScript_HideMisty::
	setflag FLAG_HIDE_POWER_PLANT_MISTY
	end

RockTunnel_B2F_EventScript_PlaceMistyAtGenerator::
	clearflag FLAG_HIDE_POWER_PLANT_MISTY
	setobjectxyperm LOCALID_ROCK_TUNNEL_MISTY, 22, 20
	setobjectxy LOCALID_ROCK_TUNNEL_MISTY, 22, 20
	setobjectgfx LOCALID_ROCK_TUNNEL_MISTY, OBJ_EVENT_GFX_MISTY
	setobjectmovementtype LOCALID_ROCK_TUNNEL_MISTY, MOVEMENT_TYPE_LOOK_AROUND
	addobject LOCALID_ROCK_TUNNEL_MISTY
	end

RockTunnel_B2F_EventScript_PlaceMistyAtLowerFalls::
	clearflag FLAG_HIDE_POWER_PLANT_MISTY
	setobjectxyperm LOCALID_ROCK_TUNNEL_MISTY, 12, 38
	setobjectxy LOCALID_ROCK_TUNNEL_MISTY, 12, 38
	setobjectgfx LOCALID_ROCK_TUNNEL_MISTY, OBJ_EVENT_GFX_MISTY_WATER
	setobjectmovementtype LOCALID_ROCK_TUNNEL_MISTY, MOVEMENT_TYPE_FACE_LEFT
	addobject LOCALID_ROCK_TUNNEL_MISTY
	end

RockTunnel_B2F_EventScript_PlaceMistyAtSecondGenerator::
	clearflag FLAG_HIDE_POWER_PLANT_MISTY
	setobjectxyperm LOCALID_ROCK_TUNNEL_MISTY, 21, 41
	setobjectxy LOCALID_ROCK_TUNNEL_MISTY, 21, 41
	setobjectgfx LOCALID_ROCK_TUNNEL_MISTY, OBJ_EVENT_GFX_MISTY
	setobjectmovementtype LOCALID_ROCK_TUNNEL_MISTY, MOVEMENT_TYPE_LOOK_AROUND
	addobject LOCALID_ROCK_TUNNEL_MISTY
	end

RockTunnel_B2F_EventScript_Misty::
	lock
	faceplayer
	textcolor NPC_TEXT_COLOR_NEUTRAL
	goto_if_eq VAR_MISTY_TRIAL_STATE, 0, RockTunnel_B2F_EventScript_UnlockMistyQuest
	goto_if_eq VAR_MISTY_TRIAL_STATE, 2, RockTunnel_B2F_EventScript_ChooseDirection
	goto_if_eq VAR_MISTY_TRIAL_STATE, 3, RockTunnel_B2F_EventScript_MistyAtGenerator
	goto_if_eq VAR_MISTY_TRIAL_STATE, 4, RockTunnel_B2F_EventScript_MistyLowerScoutRequest
	goto_if_eq VAR_MISTY_TRIAL_STATE, 5, RockTunnel_B2F_EventScript_LowerChooseDirection
	goto_if_eq VAR_MISTY_TRIAL_STATE, 6, RockTunnel_B2F_EventScript_MistyAtSecondGenerator
	goto RockTunnel_B2F_EventScript_MistyScoutRequest

RockTunnel_B2F_EventScript_MistyFromPlatform::
	lock
	textcolor NPC_TEXT_COLOR_NEUTRAL
	goto_if_ge VAR_MISTY_TRIAL_STATE, 3, RockTunnel_B2F_EventScript_PlatformNoResponse
	goto_if_eq VAR_MISTY_TRIAL_STATE, 0, RockTunnel_B2F_EventScript_UnlockMistyQuest
	goto_if_eq VAR_MISTY_TRIAL_STATE, 2, RockTunnel_B2F_EventScript_ChooseDirection
	goto RockTunnel_B2F_EventScript_MistyScoutRequest

RockTunnel_B2F_EventScript_PlatformNoResponse::
	release
	end

RockTunnel_B2F_EventScript_UnlockMistyQuest::
	questmenu QUEST_MENU_UNLOCK_QUEST, QUEST_GYM_LEADER_TRIALS
	questmenu QUEST_MENU_SET_ACTIVE, QUEST_GYM_LEADER_TRIALS
	subquestmenu QUEST_MENU_UNLOCK_QUEST, QUEST_GYM_LEADER_TRIALS, SUB_QUEST_MISTY
	setvar VAR_MISTY_TRIAL_STATE, 2

RockTunnel_B2F_EventScript_MistyScoutRequest::
	msgbox RockTunnel_B2F_Text_MistyScoutRequest
	setvar VAR_MISTY_TRIAL_STATE, 2
	release
	end

RockTunnel_B2F_EventScript_ChooseDirection::
	msgbox RockTunnel_B2F_Text_ChooseDirection
	multichoice 18, 6, MULTICHOICE_LEFT_RIGHT, FALSE
	goto_if_eq VAR_RESULT, 0, RockTunnel_B2F_EventScript_ChooseLeft
	goto_if_eq VAR_RESULT, 1, RockTunnel_B2F_EventScript_ChooseRight
	release
	end

RockTunnel_B2F_EventScript_ChooseLeft::
	msgbox RockTunnel_B2F_Text_TryLeft
	applymovement LOCALID_ROCK_TUNNEL_MISTY, RockTunnel_B2F_Movement_MistySwimLeft
	waitmovement 0
	delay 30
	applymovement LOCALID_ROCK_TUNNEL_MISTY, RockTunnel_B2F_Movement_MistyReturnFromLeft
	waitmovement 0
	msgbox RockTunnel_B2F_Text_LeftFailed
	goto RockTunnel_B2F_EventScript_ChooseDirection

RockTunnel_B2F_EventScript_ChooseRight::
	msgbox RockTunnel_B2F_Text_TryRight
	applymovement LOCALID_ROCK_TUNNEL_MISTY, RockTunnel_B2F_Movement_MistySwimRight
	waitmovement 0
	clearflag FLAG_HIDE_POWER_PLANT_MISTY
	setobjectxyperm LOCALID_ROCK_TUNNEL_MISTY, 22, 20
	setobjectxy LOCALID_ROCK_TUNNEL_MISTY, 22, 20
	setobjectgfx LOCALID_ROCK_TUNNEL_MISTY, OBJ_EVENT_GFX_MISTY
	setobjectmovementtype LOCALID_ROCK_TUNNEL_MISTY, MOVEMENT_TYPE_LOOK_AROUND
	addobject LOCALID_ROCK_TUNNEL_MISTY
	setvar VAR_MISTY_TRIAL_STATE, 3
	release
	end

RockTunnel_B2F_EventScript_MistyAtGenerator::
	msgbox RockTunnel_B2F_Text_MistyAtGenerator
	release
	end

RockTunnel_B2F_EventScript_MistyAtGeneratorFromEdge::
	lock
	textcolor NPC_TEXT_COLOR_NEUTRAL
	goto_if_ne VAR_MISTY_TRIAL_STATE, 3, RockTunnel_B2F_EventScript_PlatformNoResponse
	msgbox RockTunnel_B2F_Text_MistyGeneratorFixed
	closemessage
	applymovement LOCALID_ROCK_TUNNEL_MISTY, RockTunnel_B2F_Movement_MistyStepOffGenerator
	waitmovement 0
	setobjectgfx LOCALID_ROCK_TUNNEL_MISTY, OBJ_EVENT_GFX_MISTY_WATER
	applymovement LOCALID_ROCK_TUNNEL_MISTY, RockTunnel_B2F_Movement_MistySwimFromGenerator
	waitmovement 0
	setvar VAR_MISTY_TRIAL_STATE, 4
	clearflag FLAG_HIDE_POWER_PLANT_MISTY
	setobjectxyperm LOCALID_ROCK_TUNNEL_MISTY, 12, 38
	setobjectxy LOCALID_ROCK_TUNNEL_MISTY, 12, 38
	setobjectmovementtype LOCALID_ROCK_TUNNEL_MISTY, MOVEMENT_TYPE_FACE_LEFT
	addobject LOCALID_ROCK_TUNNEL_MISTY
	release
	end

RockTunnel_B2F_EventScript_MistyLowerFromWall::
	lock
	textcolor NPC_TEXT_COLOR_NEUTRAL
	goto_if_eq VAR_MISTY_TRIAL_STATE, 4, RockTunnel_B2F_EventScript_MistyLowerScoutRequest
	goto_if_eq VAR_MISTY_TRIAL_STATE, 5, RockTunnel_B2F_EventScript_LowerChooseDirection
	release
	end

RockTunnel_B2F_EventScript_MistyLowerScoutRequest::
	msgbox RockTunnel_B2F_Text_MistyLowerScoutRequest
	setvar VAR_MISTY_TRIAL_STATE, 5
	release
	end

RockTunnel_B2F_EventScript_LowerChooseDirection::
	msgbox RockTunnel_B2F_Text_LowerChooseDirection
	multichoice 18, 6, MULTICHOICE_LEFT_RIGHT, FALSE
	goto_if_eq VAR_RESULT, 0, RockTunnel_B2F_EventScript_LowerChooseLeft
	goto_if_eq VAR_RESULT, 1, RockTunnel_B2F_EventScript_LowerChooseRight
	release
	end

RockTunnel_B2F_EventScript_LowerChooseLeft::
	msgbox RockTunnel_B2F_Text_LowerTryLeft
	closemessage
	applymovement LOCALID_ROCK_TUNNEL_MISTY, RockTunnel_B2F_Movement_MistyLowerWaterfall
	waitmovement 0
	delay 30
	applymovement LOCALID_ROCK_TUNNEL_MISTY, RockTunnel_B2F_Movement_MistyReturnFromLowerWaterfall
	waitmovement 0
	copyobjectxytoperm LOCALID_ROCK_TUNNEL_MISTY
	setobjectmovementtype LOCALID_ROCK_TUNNEL_MISTY, MOVEMENT_TYPE_FACE_LEFT
	msgbox RockTunnel_B2F_Text_LowerLeftFailed
	release
	end

RockTunnel_B2F_EventScript_LowerChooseRight::
	msgbox RockTunnel_B2F_Text_LowerTryRight
	closemessage
	applymovement LOCALID_ROCK_TUNNEL_MISTY, RockTunnel_B2F_Movement_MistyLowerWaterfall
	waitmovement 0
	clearflag FLAG_HIDE_POWER_PLANT_MISTY
	setobjectxyperm LOCALID_ROCK_TUNNEL_MISTY, 21, 41
	setobjectxy LOCALID_ROCK_TUNNEL_MISTY, 21, 41
	setobjectgfx LOCALID_ROCK_TUNNEL_MISTY, OBJ_EVENT_GFX_MISTY
	setobjectmovementtype LOCALID_ROCK_TUNNEL_MISTY, MOVEMENT_TYPE_LOOK_AROUND
	addobject LOCALID_ROCK_TUNNEL_MISTY
	setvar VAR_MISTY_TRIAL_STATE, 6
	release
	end

RockTunnel_B2F_EventScript_MistyAtSecondGenerator::
	msgbox RockTunnel_B2F_Text_MistyAtSecondGenerator
	release
	end

RockTunnel_B2F_EventScript_MistyFound::
	msgbox RockTunnel_B2F_Text_MistyFound
	setvar VAR_MISTY_TRIAL_STATE, 3
	clearflag FLAG_HIDE_CERULEAN_CITY_GYM_MISTY
	setflag FLAG_HIDE_POWER_PLANT_MISTY
	removeobject LOCALID_ROCK_TUNNEL_MISTY
	release
	end

RockTunnel_B2F_EventScript_WarpToLowerPath::
	warp MAP_ROCK_TUNNEL_B2F, 5, 36
	waitstate
	end

RockTunnel_B2F_EventScript_WarpToUpperPath::
	warp MAP_ROCK_TUNNEL_B2F, 15, 24
	waitstate
	end

RockTunnel_B2F_Text_MistyScoutRequest::
	.string "Oh, hey!\n"
	.string "You're all the way out here, too?\p"
	.string "The POWER PLANT's been having\n"
	.string "wild electrical surges lately.\p"
	.string "I think underground currents in\n"
	.string "ROCK TUNNEL are feeding into it.\p"
	.string "There's a POWER STATION up ahead,\n"
	.string "but this tunnel splits around it.\p"
	.string "Could you scout ahead and tell me\n"
	.string "which direction I should take?$"

RockTunnel_B2F_Text_ChooseDirection::
	.string "Which way should I take from here?$"

RockTunnel_B2F_Text_TryLeft::
	.string "LEFT?\n"
	.string "Got it! I'll test that current.$"

RockTunnel_B2F_Text_LeftFailed::
	.string "No good!\p"
	.string "The current that way is too strong.\n"
	.string "It loops back on itself.\p"
	.string "Which way should I try?$"

RockTunnel_B2F_Text_TryRight::
	.string "RIGHT?\n"
	.string "All right, I'll follow that flow.$"

RockTunnel_B2F_Text_MistyAtGenerator::
	.string "This must be the generator.\p"
	.string "The current brought me right to it.\n"
	.string "Now we can work out what's wrong.$"

RockTunnel_B2F_Text_MistyGeneratorFixed::
	.string "Thanks for guiding me here!\p"
	.string "I've got this generator stable\n"
	.string "again.\p"
	.string "But there are more surges ahead.\n"
	.string "I'm going to follow the current.$"

RockTunnel_B2F_Text_MistyLowerScoutRequest::
	.string "I can hear you through the wall!\p"
	.string "There's a waterfall ahead, and I\n"
	.string "can see the current branching.\p"
	.string "Could you scout for me again and\n"
	.string "tell me which way to take?$"

RockTunnel_B2F_Text_LowerChooseDirection::
	.string "Which way should I take from here?$"

RockTunnel_B2F_Text_LowerTryLeft::
	.string "LEFT?\n"
	.string "Got it! I'll check that branch.$"

RockTunnel_B2F_Text_LowerTryRight::
	.string "RIGHT?\n"
	.string "Got it! I'll check that branch.$"

RockTunnel_B2F_Text_LowerLeftFailed::
	.string "No good!\p"
	.string "The current that way is too strong.\n"
	.string "It loops back on itself.$"

RockTunnel_B2F_Text_MistyAtSecondGenerator::
	.string "This is the second generator.\p"
	.string "The current carried me right to it.\n"
	.string "I'll check this one too.$"

RockTunnel_B2F_Text_MistyFound::
	.string "That's the way through?\n"
	.string "Perfect, thanks!\p"
	.string "I'll follow that current and get\n"
	.string "to the bottom of these surges.\p"
	.string "Meet me back at CERULEAN GYM\n"
	.string "when you're ready.$"

RockTunnel_B2F_Movement_MistyReturnFromLeft::
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_down
	step_end

RockTunnel_B2F_Movement_MistySwimLeft::
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

RockTunnel_B2F_Movement_MistyStepOffGenerator::
	walk_down
	walk_down
	step_end

RockTunnel_B2F_Movement_MistySwimFromGenerator::
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

RockTunnel_B2F_Movement_MistyLowerWaterfall::
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

RockTunnel_B2F_Movement_MistyReturnFromLowerWaterfall::
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_in_place_faster_left
	step_end

RockTunnel_B2F_Movement_MistySwimRight::
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

RockTunnel_B2F_Movement_MistyReturnFromRight::
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_in_place_faster_down
	step_end
