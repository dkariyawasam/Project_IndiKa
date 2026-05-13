Route21_South_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Route21_South_OnFrame
	.byte 0

Route21_South_OnFrame::
	map_script_2 VAR_MAP_SCENE_ROUTE21_SOUTH, 1, Route21_South_EventScript_PlayerStepOffFerry
	.2byte 0

Route21_South_EventScript_PlayerStepOffFerry::
	lockall
	applymovement LOCALID_PLAYER, Route21_South_Movement_PlayerStepOffFerry
	waitmovement 0
	setvar VAR_MAP_SCENE_ROUTE21_SOUTH, 0
	releaseall
	end

Route21_South_Movement_PlayerStepOffFerry::
	walk_down
	step_end

Route21_South_EventScript_SeagallopSailor::
	lock
	faceplayer
	textcolor NPC_TEXT_COLOR_NEUTRAL
	message Route21_South_Text_SeagallopSailorDestination
	waitmessage
	multichoice 19, 5, MULTICHOICE_ROUTE21_FERRY, FALSE
	switch VAR_RESULT
	case 0, Route21_South_EventScript_SailToPallet
	case 1, Route21_South_EventScript_SailToSeafoam
	case 2, Route21_South_EventScript_CancelSail
	case SCR_MENU_CANCEL, Route21_South_EventScript_CancelSail
	end

Route21_South_EventScript_SailToSeafoam::
	msgbox Route21_South_Text_SeagallopSailorAllAboard
	setflag FLAG_HIDE_ROUTE21_SOUTH_SEAGALLOP
	setflag FLAG_HIDE_ROUTE21_SOUTH_SAILOR
	clearflag FLAG_HIDE_ROUTE20_SEAGALLOP
	clearflag FLAG_HIDE_ROUTE20_SAILOR
	setvar VAR_MAP_SCENE_ROUTE20, 1
	setvar VAR_0x8004, SEAGALLOP_ROUTE21_SOUTH
	setvar VAR_0x8006, SEAGALLOP_SEAFOAM_ISLANDS
	goto EventScript_SetSail
	end

Route21_South_EventScript_SailToPallet::
	msgbox Route21_South_Text_SeagallopSailorAllAboard
	setflag FLAG_HIDE_ROUTE21_SOUTH_SEAGALLOP
	setflag FLAG_HIDE_ROUTE21_SOUTH_SAILOR
	clearflag FLAG_HIDE_ROUTE21_NORTH_SEAGALLOP
	clearflag FLAG_HIDE_ROUTE21_NORTH_SAILOR
	setvar VAR_MAP_SCENE_ROUTE21_NORTH, 1
	setvar VAR_0x8004, SEAGALLOP_ROUTE21_SOUTH
	setvar VAR_0x8006, SEAGALLOP_ROUTE21_NORTH
	goto EventScript_SetSail
	end

Route21_South_EventScript_CancelSail::
	release
	end
