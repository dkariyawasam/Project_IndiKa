Route19_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Route19_OnFrame
	.byte 0

Route19_OnFrame::
	map_script_2 VAR_MAP_SCENE_ROUTE19, 1, Route19_EventScript_PlayerStepOffFerry
	.2byte 0

Route19_EventScript_PlayerStepOffFerry::
	lockall
	applymovement LOCALID_PLAYER, Route19_Movement_PlayerStepOffFerry
	waitmovement 0
	setvar VAR_MAP_SCENE_ROUTE19, 0
	releaseall
	end

Route19_Movement_PlayerStepOffFerry::
	walk_up
	step_end

Route19_EventScript_RouteSign::
	msgbox Route19_Text_RouteSign, MSGBOX_SIGN
	end

Route19_EventScript_SeagallopSailor::
	lock
	faceplayer
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Route19_Text_SeagallopSailorGreeting, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route19_EventScript_SailToSeafoam
	release
	end

Route19_EventScript_SailToSeafoam::
	msgbox Route19_Text_SeagallopSailorAllAboard
	setflag FLAG_HIDE_ROUTE19_SEAGALLOP
	setflag FLAG_HIDE_ROUTE19_SAILOR
	clearflag FLAG_HIDE_ROUTE20_SEAGALLOP
	clearflag FLAG_HIDE_ROUTE20_SAILOR
	setvar VAR_MAP_SCENE_ROUTE20, 1
	setvar VAR_0x8004, SEAGALLOP_ROUTE19_FUCHSIA_SHORE
	setvar VAR_0x8006, SEAGALLOP_SEAFOAM_ISLANDS
	goto EventScript_SetSail
	end
