Route11_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route11_OnTransition
	.byte 0

Route11_OnTransition::
	special EndBikeRental
	goto_if_set FLAG_HIDE_ROUTE11_RIVAL_RUMORS, Route11_EventScript_RemoveRival
	goto_if_unset FLAG_SYS_POKEDEX_GET, Route11_EventScript_RemoveRival
	end

Route11_EventScript_RemoveRival::
	removeobject LOCALID_ROUTE11_RIVAL
	end

Route11_EventScript_DiglettsCaveSign::
	msgbox Route11_Text_DiglettsCave, MSGBOX_SIGN
	end

Route11_EventScript_RivalRumorsTriggerTop::
	setvar VAR_TEMP_1, 0
	goto Route11_EventScript_RivalRumorsTrigger

Route11_EventScript_RivalRumorsTriggerMiddle::
	setvar VAR_TEMP_1, 1
	goto Route11_EventScript_RivalRumorsTrigger

Route11_EventScript_RivalRumorsTriggerBottom::
	setvar VAR_TEMP_1, 2
	goto Route11_EventScript_RivalRumorsTrigger

Route11_EventScript_RivalRumorsTrigger::
	goto_if_set FLAG_HIDE_ROUTE11_RIVAL_RUMORS, EventScript_Return
	goto_if_unset FLAG_SYS_POKEDEX_GET, EventScript_Return
	lockall
	playbgm MUS_ENCOUNTER_RIVAL, 0
	applymovement LOCALID_ROUTE11_RIVAL, Common_Movement_ExclamationMark
	waitmovement 0
	waitfieldeffect FLDEFF_EXCLAMATION_MARK_ICON
	delay 12
	applymovement LOCALID_ROUTE11_RIVAL, Route11_Movement_RivalFaceDown
	waitmovement 0
	delay 8
	call_if_eq VAR_TEMP_1, 0, Route11_EventScript_RivalApproachTop
	call_if_eq VAR_TEMP_1, 1, Route11_EventScript_RivalApproachMiddle
	call_if_eq VAR_TEMP_1, 2, Route11_EventScript_RivalApproachBottom
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	call Route11_EventScript_RivalRumorsScene
	releaseall
	end

Route11_EventScript_RivalRumors::
	goto_if_set FLAG_HIDE_ROUTE11_RIVAL_RUMORS, EventScript_Return
	goto_if_unset FLAG_SYS_POKEDEX_GET, EventScript_Return
	setvar VAR_TEMP_1, 3
	lock
	faceplayer
	call Route11_EventScript_RivalRumorsScene
	release
	end

Route11_EventScript_RivalRumorsScene::
	msgbox Route11_Text_RivalRumors
	closemessage
	playbgm MUS_RIVAL_EXIT, 0
	call_if_eq VAR_TEMP_1, 0, Route11_EventScript_RivalExitTop
	call_if_eq VAR_TEMP_1, 1, Route11_EventScript_RivalExitMiddle
	call_if_eq VAR_TEMP_1, 2, Route11_EventScript_RivalExitBottom
	setflag FLAG_HIDE_ROUTE11_RIVAL_RUMORS
	setvar VAR_MAP_SCENE_ROUTE11_RIVAL, 1
	removeobject LOCALID_ROUTE11_RIVAL
	fadedefaultbgm
	return

Route11_EventScript_RivalApproachTop::
	applymovement LOCALID_ROUTE11_RIVAL, Route11_Movement_RivalApproachTop
	waitmovement 0
	return

Route11_EventScript_RivalApproachMiddle::
	applymovement LOCALID_ROUTE11_RIVAL, Route11_Movement_RivalApproachMiddle
	waitmovement 0
	return

Route11_EventScript_RivalApproachBottom::
	applymovement LOCALID_ROUTE11_RIVAL, Route11_Movement_RivalApproachBottom
	waitmovement 0
	return

Route11_EventScript_RivalExitTop::
	applymovement LOCALID_ROUTE11_RIVAL, Route11_Movement_RivalExitTop
	applymovement LOCALID_PLAYER, Route11_Movement_PlayerWatchRivalExitDown
	waitmovement LOCALID_ROUTE11_RIVAL
	return

Route11_EventScript_RivalExitMiddle::
	applymovement LOCALID_ROUTE11_RIVAL, Route11_Movement_RivalExitMiddle
	applymovement LOCALID_PLAYER, Route11_Movement_PlayerWatchRivalExitDown
	waitmovement LOCALID_ROUTE11_RIVAL
	return

Route11_EventScript_RivalExitBottom::
	applymovement LOCALID_ROUTE11_RIVAL, Route11_Movement_RivalExitBottom
	applymovement LOCALID_PLAYER, Route11_Movement_PlayerWatchRivalExitUp
	waitmovement LOCALID_ROUTE11_RIVAL
	return

Route11_Movement_RivalFaceDown::
	face_down
	step_end

Route11_Movement_RivalApproachTop::
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_left
	step_end

Route11_Movement_RivalApproachMiddle::
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_left
	step_end

Route11_Movement_RivalApproachBottom::
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_left
	step_end

Route11_Movement_RivalExitTop::
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

Route11_Movement_RivalExitMiddle::
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

Route11_Movement_RivalExitBottom::
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

Route11_Movement_PlayerWatchRivalExitDown::
	face_down
	delay_16
	face_left
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	step_end

Route11_Movement_PlayerWatchRivalExitUp::
	face_up
	delay_16
	face_left
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	step_end
