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
	special RecordApexRumor
	end

PokemonTower_1F_EventScript_RivalTriggerRight::
	lockall
	setvar VAR_TEMP_1, 0
	goto PokemonTower_1F_EventScript_Rival
	end

PokemonTower_1F_EventScript_RivalTriggerDown::
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
	msgbox PokemonTower_1F_Text_RivalIntro
	setvar VAR_LAST_TALKED, LOCALID_POKEMON_TOWER_RIVAL
	call_if_eq VAR_STARTER_MON, 2, PokemonTower_1F_EventScript_RivalSquirtle
	call_if_eq VAR_STARTER_MON, 1, PokemonTower_1F_EventScript_RivalBulbasaur
	call_if_eq VAR_STARTER_MON, 0, PokemonTower_1F_EventScript_RivalCharmander
	msgbox PokemonTower_1F_Text_RivalPostBattle
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

PokemonTower_1F_EventScript_RivalSquirtle::
	trainerbattle_no_intro TRAINER_RIVAL_POKEMON_TOWER_SQUIRTLE, PokemonTower_1F_Text_RivalDefeat
	return

PokemonTower_1F_EventScript_RivalBulbasaur::
	trainerbattle_no_intro TRAINER_RIVAL_POKEMON_TOWER_BULBASAUR, PokemonTower_1F_Text_RivalDefeat
	return

PokemonTower_1F_EventScript_RivalCharmander::
	trainerbattle_no_intro TRAINER_RIVAL_POKEMON_TOWER_CHARMANDER, PokemonTower_1F_Text_RivalDefeat
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
