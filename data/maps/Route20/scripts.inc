Route20_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route20_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Route20_OnFrame
	.byte 0

Route20_OnFrame::
	map_script_2 VAR_MAP_SCENE_ROUTE20, 1, Route20_EventScript_PlayerStepOffFerry
	.2byte 0

Route20_OnTransition::
	call_if_unset FLAG_STOPPED_SEAFOAM_B3F_CURRENT, Route20_EventScript_ResetSeafoamBouldersForB3F
	call_if_unset FLAG_STOPPED_SEAFOAM_B4F_CURRENT, Route20_EventScript_ResetSeafoamBouldersForB4F
	end

Route20_EventScript_PlayerStepOffFerry::
	lockall
	applymovement LOCALID_PLAYER, Route20_Movement_PlayerStepOffFerry
	waitmovement 0
	setvar VAR_MAP_SCENE_ROUTE20, 0
	releaseall
	end

Route20_Movement_PlayerStepOffFerry::
	walk_up
	step_end

Route20_EventScript_ResetSeafoamBouldersForB3F::
	clearflag FLAG_HIDE_SEAFOAM_1F_BOULDER_1
	clearflag FLAG_HIDE_SEAFOAM_1F_BOULDER_2
	setflag FLAG_HIDE_SEAFOAM_B1F_BOULDER_1
	setflag FLAG_HIDE_SEAFOAM_B1F_BOULDER_2
	setflag FLAG_HIDE_SEAFOAM_B2F_BOULDER_1
	setflag FLAG_HIDE_SEAFOAM_B2F_BOULDER_2
	setflag FLAG_HIDE_SEAFOAM_B3F_BOULDER_1
	setflag FLAG_HIDE_SEAFOAM_B3F_BOULDER_2
	return

Route20_EventScript_ResetSeafoamBouldersForB4F::
	clearflag FLAG_HIDE_SEAFOAM_B3F_BOULDER_3
	clearflag FLAG_HIDE_SEAFOAM_B3F_BOULDER_4
	clearflag FLAG_HIDE_SEAFOAM_B3F_BOULDER_5
	clearflag FLAG_HIDE_SEAFOAM_B3F_BOULDER_6
	setflag FLAG_HIDE_SEAFOAM_B4F_BOULDER_1
	setflag FLAG_HIDE_SEAFOAM_B4F_BOULDER_2
	return

Route20_EventScript_SeafoamIslandsSign::
	msgbox Route20_Text_SeafoamIslands, MSGBOX_SIGN
	end

Route20_EventScript_SeagallopSailor::
	lock
	faceplayer
	textcolor NPC_TEXT_COLOR_NEUTRAL
	message Route20_Text_SeagallopSailorDestination
	waitmessage
	multichoice 17, 5, MULTICHOICE_ROUTE20_FERRY, FALSE
	switch VAR_RESULT
	case 0, Route20_EventScript_SailToCinnabar
	case 1, Route20_EventScript_SailToFuchsiaShore
	case 2, Route20_EventScript_CancelSail
	case SCR_MENU_CANCEL, Route20_EventScript_CancelSail
	end

Route20_EventScript_SailToCinnabar::
	msgbox Route20_Text_SeagallopSailorAllAboard
	setflag FLAG_HIDE_ROUTE20_SEAGALLOP
	setflag FLAG_HIDE_ROUTE20_SAILOR
	clearflag FLAG_HIDE_ROUTE21_SOUTH_SEAGALLOP
	clearflag FLAG_HIDE_ROUTE21_SOUTH_SAILOR
	setvar VAR_MAP_SCENE_ROUTE21_SOUTH, 1
	setvar VAR_0x8004, SEAGALLOP_SEAFOAM_ISLANDS
	setvar VAR_0x8006, SEAGALLOP_ROUTE21_SOUTH
	goto EventScript_SetSail
	end

Route20_EventScript_SailToFuchsiaShore::
	msgbox Route20_Text_SeagallopSailorAllAboard
	setflag FLAG_HIDE_ROUTE20_SEAGALLOP
	setflag FLAG_HIDE_ROUTE20_SAILOR
	clearflag FLAG_HIDE_ROUTE19_SEAGALLOP
	clearflag FLAG_HIDE_ROUTE19_SAILOR
	setvar VAR_MAP_SCENE_ROUTE19, 1
	setvar VAR_0x8004, SEAGALLOP_SEAFOAM_ISLANDS
	setvar VAR_0x8006, SEAGALLOP_ROUTE19_FUCHSIA_SHORE
	goto EventScript_SetSail
	end

Route20_EventScript_CancelSail::
	release
	end

Route20_EventScript_NiaFinnNia::
	trainerbattle_double TRAINER_SWIM_SIBS_NIA_FINN, Route20_Text_NiaFinnIntro, Route20_Text_NiaFinnDefeat, Route20_Text_NiaFinnNotEnoughMons
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route20_EventScript_NiaFinnNiaRematch
	msgbox Route20_Text_NiaFinnPostBattle, MSGBOX_AUTOCLOSE
	end

Route20_EventScript_NiaFinnNiaRematch::
	trainerbattle_rematch_double TRAINER_SWIM_SIBS_NIA_FINN, Route20_Text_NiaFinnIntro, Route20_Text_NiaFinnDefeat, Route20_Text_NiaFinnNotEnoughMons
	msgbox Route20_Text_NiaFinnPostBattle, MSGBOX_AUTOCLOSE
	end

Route20_EventScript_NiaFinnFinn::
	trainerbattle_double TRAINER_SWIM_SIBS_NIA_FINN, Route20_Text_NiaFinnIntro, Route20_Text_NiaFinnDefeat, Route20_Text_NiaFinnNotEnoughMons
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route20_EventScript_NiaFinnFinnRematch
	msgbox Route20_Text_NiaFinnPostBattle, MSGBOX_AUTOCLOSE
	end

Route20_EventScript_NiaFinnFinnRematch::
	trainerbattle_rematch_double TRAINER_SWIM_SIBS_NIA_FINN, Route20_Text_NiaFinnIntro, Route20_Text_NiaFinnDefeat, Route20_Text_NiaFinnNotEnoughMons
	msgbox Route20_Text_NiaFinnPostBattle, MSGBOX_AUTOCLOSE
	end

Route20_Text_NiaFinnIntro::
	.string "We practice swimming together.\nNow we are practicing battles!$"

Route20_Text_NiaFinnDefeat::
	.string "You make a great team!$"

Route20_Text_NiaFinnPostBattle::
	.string "Keeping pace is easy in water.\nBattling together takes work!$"

Route20_Text_NiaFinnNotEnoughMons::
	.string "Bring two ready POKéMON\nso we can battle as a pair!$"
