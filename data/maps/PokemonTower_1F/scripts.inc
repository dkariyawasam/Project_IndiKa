PokemonTower_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PokemonTower_1F_OnTransition
	.byte 0

PokemonTower_1F_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_POKEMON_TOWER_1F
	end

PokemonTower_1F_EventScript_Channeler::
	msgbox PokemonTower_1F_Text_SpiritSearchingForFamily, MSGBOX_NPC
	setvar VAR_0x8004, SUB_QUEST_APEX_OSSCYTHE
	setvar VAR_0x8005, 2
	call EventScript_RecordApexRumorWithFeedback
	end

PokemonTower_1F_EventScript_RivalTriggerRight::
	goto_if_ge VAR_MAP_SCENE_POKEMON_TOWER_1F, 1, EventScript_Return
	lockall
	setvar VAR_TEMP_1, 0
	goto PokemonTower_1F_EventScript_Rival
	end

PokemonTower_1F_EventScript_RivalTriggerDown::
	goto_if_ge VAR_MAP_SCENE_POKEMON_TOWER_1F, 1, EventScript_Return
	lockall
	setvar VAR_TEMP_1, 1
	goto PokemonTower_1F_EventScript_Rival
	end

PokemonTower_1F_EventScript_Rival::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playbgm MUS_ENCOUNTER_RIVAL, 0
	call_if_eq VAR_TEMP_1, 0, PokemonTower_1F_EventScript_RivalFacePlayerRight
	call_if_eq VAR_TEMP_1, 1, PokemonTower_1F_EventScript_RivalFacePlayerDown
	applymovement LOCALID_POKEMON_TOWER_RIVAL, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_POKEMON_TOWER_RIVAL, Common_Movement_Delay48
	waitmovement 0
	msgbox PokemonTower_1F_Text_RivalApex
	call EventScript_RivalApexReflection
	closemessage
	playbgm MUS_RIVAL_EXIT, 0
	call_if_eq VAR_TEMP_1, 0, PokemonTower_1F_EventScript_RivalExitRight
	call_if_eq VAR_TEMP_1, 1, PokemonTower_1F_EventScript_RivalExitDown
	playse SE_EXIT
	delay 25
	fadedefaultbgm
	removeobject LOCALID_POKEMON_TOWER_RIVAL
	setvar VAR_MAP_SCENE_POKEMON_TOWER_1F, 1
	releaseall
	end

PokemonTower_1F_EventScript_RivalFacePlayerRight::
	applymovement LOCALID_POKEMON_TOWER_RIVAL, Common_Movement_WalkInPlaceFasterRight
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

PokemonTower_1F_EventScript_RivalFacePlayerDown::
	applymovement LOCALID_POKEMON_TOWER_RIVAL, Common_Movement_WalkInPlaceFasterDown
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	return

PokemonTower_1F_EventScript_RivalExitRight::
	applymovement LOCALID_POKEMON_TOWER_RIVAL, PokemonTower_1F_Movement_RivalExitRight
	waitmovement 0
	return

PokemonTower_1F_EventScript_RivalExitDown::
	applymovement LOCALID_POKEMON_TOWER_RIVAL, PokemonTower_1F_Movement_RivalExitDown
	waitmovement 0
	return

PokemonTower_1F_Movement_RivalExitRight::
	walk_down
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_in_place_faster_right
	step_end

PokemonTower_1F_Movement_RivalExitDown::
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_in_place_faster_right
	step_end
PokemonTower_1F_Text_RivalApex::
	.string "{RIVAL}: I'm following the\n"
	.string "OSSCYTHE accounts. You too?\p"
	.string "They describe a MAROWAK that\n"
	.string "changed here in POKéMON TOWER.\p"
	.string "Why here? What makes this place\n"
	.string "different from anywhere else?\p"
	.string "Ask around and check your LOGBOOK.\n"
	.string "I'm comparing the stories.$"

