CinnabarIsland_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CinnabarIsland_OnTransition
	.byte 0

CinnabarIsland_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_CINNABAR_ISLAND
	end

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
