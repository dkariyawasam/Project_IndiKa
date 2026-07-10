CinnabarIsland_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CinnabarIsland_OnTransition
	.byte 0

CinnabarIsland_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_CINNABAR_ISLAND
	goto_if_set FLAG_HIDE_CINNABAR_RIVAL_MANSION, CinnabarIsland_EventScript_RemoveRival
	goto_if_unset FLAG_SYS_POKEDEX_GET, CinnabarIsland_EventScript_RemoveRival
	removeobject LOCALID_CINNABAR_RIVAL
	end

CinnabarIsland_EventScript_RemoveRival::
	removeobject LOCALID_CINNABAR_RIVAL
	end

CinnabarIsland_EventScript_RivalMansionTrigger34::
	setvar VAR_TEMP_1, 0
	goto CinnabarIsland_EventScript_RivalMansionTrigger

CinnabarIsland_EventScript_RivalMansionTrigger35::
	setvar VAR_TEMP_1, 1
	goto CinnabarIsland_EventScript_RivalMansionTrigger

CinnabarIsland_EventScript_RivalMansionTrigger36::
	setvar VAR_TEMP_1, 2
	goto CinnabarIsland_EventScript_RivalMansionTrigger

CinnabarIsland_EventScript_RivalMansionTrigger37::
	setvar VAR_TEMP_1, 3
	goto CinnabarIsland_EventScript_RivalMansionTrigger

CinnabarIsland_EventScript_RivalMansionTrigger38::
	setvar VAR_TEMP_1, 4
	goto CinnabarIsland_EventScript_RivalMansionTrigger

CinnabarIsland_EventScript_RivalMansionTrigger39::
	setvar VAR_TEMP_1, 5
	goto CinnabarIsland_EventScript_RivalMansionTrigger

CinnabarIsland_EventScript_RivalMansionTrigger::
	goto_if_set FLAG_HIDE_CINNABAR_RIVAL_MANSION, EventScript_Return
	goto_if_unset FLAG_SYS_POKEDEX_GET, EventScript_Return
	lockall
	playbgm MUS_ENCOUNTER_RIVAL, 0
	opendoor 44, 4
	waitdooranim
	addobject LOCALID_CINNABAR_RIVAL
	applymovement LOCALID_CINNABAR_RIVAL, CinnabarIsland_Movement_RivalStepOutGymDoor
	waitmovement 0
	closedoor 44, 4
	waitdooranim
	applymovement LOCALID_CINNABAR_RIVAL, CinnabarIsland_Movement_RivalLeaveGymDoor
	waitmovement 0
	applymovement LOCALID_CINNABAR_RIVAL, Common_Movement_ExclamationMark
	waitmovement 0
	waitfieldeffect FLDEFF_EXCLAMATION_MARK_ICON
	delay 12
	call_if_eq VAR_TEMP_1, 0, CinnabarIsland_EventScript_RivalApproach34
	call_if_eq VAR_TEMP_1, 1, CinnabarIsland_EventScript_RivalApproach35
	call_if_eq VAR_TEMP_1, 2, CinnabarIsland_EventScript_RivalApproach36
	call_if_eq VAR_TEMP_1, 3, CinnabarIsland_EventScript_RivalApproach37
	call_if_eq VAR_TEMP_1, 4, CinnabarIsland_EventScript_RivalApproach38
	call_if_eq VAR_TEMP_1, 5, CinnabarIsland_EventScript_RivalApproach39
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	call CinnabarIsland_EventScript_RivalMansionScene
	releaseall
	end

CinnabarIsland_EventScript_RivalMansion::
	goto_if_set FLAG_HIDE_CINNABAR_RIVAL_MANSION, EventScript_Return
	goto_if_unset FLAG_SYS_POKEDEX_GET, EventScript_Return
	setvar VAR_TEMP_1, 6
	lock
	faceplayer
	call CinnabarIsland_EventScript_RivalMansionScene
	release
	end

CinnabarIsland_EventScript_RivalMansionScene::
	msgbox CinnabarIsland_Text_RivalMansion
	closemessage
	playbgm MUS_RIVAL_EXIT, 0
	call_if_eq VAR_TEMP_1, 0, CinnabarIsland_EventScript_RivalExitMansion34
	call_if_eq VAR_TEMP_1, 1, CinnabarIsland_EventScript_RivalExitMansion35
	call_if_eq VAR_TEMP_1, 2, CinnabarIsland_EventScript_RivalExitMansion36
	call_if_eq VAR_TEMP_1, 3, CinnabarIsland_EventScript_RivalExitMansion37
	call_if_eq VAR_TEMP_1, 4, CinnabarIsland_EventScript_RivalExitMansion38
	call_if_eq VAR_TEMP_1, 5, CinnabarIsland_EventScript_RivalExitMansion39
	call_if_eq VAR_TEMP_1, 6, CinnabarIsland_EventScript_RivalExitMansion37
	applymovement LOCALID_PLAYER, CinnabarIsland_Movement_PlayerWatchRivalExitMansion
	waitmovement 0
	setflag FLAG_HIDE_CINNABAR_RIVAL_MANSION
	setvar VAR_MAP_SCENE_CINNABAR_RIVAL, 1
	removeobject LOCALID_CINNABAR_RIVAL
	fadedefaultbgm
	return

CinnabarIsland_Movement_PlayerWatchRivalExitMansion::
	face_left
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	step_end

CinnabarIsland_EventScript_RivalApproach34::
	applymovement LOCALID_CINNABAR_RIVAL, CinnabarIsland_Movement_RivalApproach34
	waitmovement 0
	return

CinnabarIsland_EventScript_RivalApproach35::
	applymovement LOCALID_CINNABAR_RIVAL, CinnabarIsland_Movement_RivalApproach35
	waitmovement 0
	return

CinnabarIsland_EventScript_RivalApproach36::
	applymovement LOCALID_CINNABAR_RIVAL, CinnabarIsland_Movement_RivalApproach36
	waitmovement 0
	return

CinnabarIsland_EventScript_RivalApproach37::
	applymovement LOCALID_CINNABAR_RIVAL, CinnabarIsland_Movement_RivalApproach37
	waitmovement 0
	return

CinnabarIsland_EventScript_RivalApproach38::
	applymovement LOCALID_CINNABAR_RIVAL, CinnabarIsland_Movement_RivalApproach38
	waitmovement 0
	return

CinnabarIsland_EventScript_RivalApproach39::
	applymovement LOCALID_CINNABAR_RIVAL, CinnabarIsland_Movement_RivalApproach39
	waitmovement 0
	return

CinnabarIsland_EventScript_RivalExitMansion34::
	applymovement LOCALID_CINNABAR_RIVAL, CinnabarIsland_Movement_RivalExitMansion34
	waitmovement 0
	call CinnabarIsland_EventScript_RivalEnterMansion
	return

CinnabarIsland_EventScript_RivalExitMansion35::
	applymovement LOCALID_CINNABAR_RIVAL, CinnabarIsland_Movement_RivalExitMansion35
	waitmovement 0
	call CinnabarIsland_EventScript_RivalEnterMansion
	return

CinnabarIsland_EventScript_RivalExitMansion36::
	applymovement LOCALID_CINNABAR_RIVAL, CinnabarIsland_Movement_RivalExitMansion36
	waitmovement 0
	call CinnabarIsland_EventScript_RivalEnterMansion
	return

CinnabarIsland_EventScript_RivalExitMansion37::
	applymovement LOCALID_CINNABAR_RIVAL, CinnabarIsland_Movement_RivalExitMansion37
	waitmovement 0
	call CinnabarIsland_EventScript_RivalEnterMansion
	return

CinnabarIsland_EventScript_RivalExitMansion38::
	applymovement LOCALID_CINNABAR_RIVAL, CinnabarIsland_Movement_RivalExitMansion38
	waitmovement 0
	call CinnabarIsland_EventScript_RivalEnterMansion
	return

CinnabarIsland_EventScript_RivalExitMansion39::
	applymovement LOCALID_CINNABAR_RIVAL, CinnabarIsland_Movement_RivalExitMansion39
	waitmovement 0
	call CinnabarIsland_EventScript_RivalEnterMansion
	return

CinnabarIsland_EventScript_RivalEnterMansion::
	playse SE_EXIT
	applymovement LOCALID_CINNABAR_RIVAL, CinnabarIsland_Movement_RivalEnterMansion
	waitmovement 0
	return

CinnabarIsland_Movement_RivalStepOutGymDoor::
	set_invisible
	set_visible
	walk_down
	step_end

CinnabarIsland_Movement_RivalLeaveGymDoor::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

CinnabarIsland_Movement_RivalApproach34::
	walk_left
	walk_left
	walk_left
	walk_up
	walk_in_place_faster_up
	step_end

CinnabarIsland_Movement_RivalApproach35::
	walk_left
	walk_left
	walk_up
	walk_in_place_faster_up
	step_end

CinnabarIsland_Movement_RivalApproach36::
	walk_left
	walk_up
	walk_in_place_faster_up
	step_end

CinnabarIsland_Movement_RivalApproach37::
	walk_up
	walk_in_place_faster_up
	step_end

CinnabarIsland_Movement_RivalApproach38::
	walk_right
	walk_up
	walk_in_place_faster_up
	step_end

CinnabarIsland_Movement_RivalApproach39::
	walk_right
	walk_right
	walk_up
	walk_in_place_faster_up
	step_end

CinnabarIsland_Movement_RivalExitMansion34::
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

CinnabarIsland_Movement_RivalExitMansion35::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

CinnabarIsland_Movement_RivalExitMansion36::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

CinnabarIsland_Movement_RivalExitMansion37::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

CinnabarIsland_Movement_RivalExitMansion38::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

CinnabarIsland_Movement_RivalExitMansion39::
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

CinnabarIsland_Movement_RivalEnterMansion::
	walk_up
	set_invisible
	step_end

CinnabarIsland_EventScript_PokemonLabDoor::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	checkitem ITEM_BOULDER_KEY
	goto_if_eq VAR_RESULT, TRUE, CinnabarIsland_EventScript_PokemonLabAccessGranted
	msgbox CinnabarIsland_Text_PokemonLabAccessDenied
	closemessage
	applymovement LOCALID_PLAYER, CinnabarIsland_Movement_ForcePlayerFromDoor
	waitmovement 0
	releaseall
	end

CinnabarIsland_EventScript_PokemonLabAccessGranted::
	msgbox CinnabarIsland_Text_PokemonLabAccessGranted
	closemessage
	warp MAP_CINNABAR_ISLAND_POKEMON_LAB_ENTRANCE, 1
	waitstate
	releaseall
	end

CinnabarIsland_Movement_ForcePlayerFromDoor::
	walk_down
	step_end

CinnabarIsland_EventScript_Woman::
	lock
	faceplayer
	apexlog APEX_LOG_PERSON_BLAINE, 3
	msgbox CinnabarIsland_Text_BlaineLivedHereSinceBeforeLab
	release
	end

CinnabarIsland_EventScript_OldMan::
	msgbox CinnabarIsland_Text_ScientistsExperimentInMansion, MSGBOX_NPC
	setvar VAR_0x8004, SUB_QUEST_APEX_MOLTRES
	setvar VAR_0x8005, 0
	call EventScript_RecordApexRumorWithFeedback
	end

CinnabarIsland_EventScript_IslandSign::
	msgbox CinnabarIsland_Text_IslandSign, MSGBOX_SIGN
	end

CinnabarIsland_EventScript_PokemonLabSign::
	msgbox CinnabarIsland_Text_PokemonLab, MSGBOX_SIGN
	end

CinnabarIsland_EventScript_GymSign::
	lockall
	apexlog APEX_LOG_PERSON_BLAINE, 0
	msgbox CinnabarIsland_Text_GymSign
	releaseall
	end
