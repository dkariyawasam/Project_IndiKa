RocketLeague_Arena_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, RocketLeague_Arena_OnResume
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, RocketLeague_Arena_OnWarp
	.byte 0

RocketLeague_Arena_OnResume::
	setvar VAR_0x8004, 2
	call PokemonLeague_EventScript_DoLightingEffect
	end

RocketLeague_Arena_OnWarp::
	map_script_2 VAR_TEMP_1, 0, RocketLeague_Arena_EventScript_TurnPlayerNorth
	.2byte 0

RocketLeague_Arena_EventScript_TurnPlayerNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

RocketLeague_Arena_EventScript_Official::
	lock
	faceplayer
	msgbox RocketLeague_Arena_Text_Official
	release
	end

RocketLeague_Arena_EventScript_ReturnToLobby::
	warp MAP_ROCKET_LEAGUE_LOBBY, 9, 2
	waitstate
	end

RocketLeague_Arena_EventScript_ReadyPrompt::
	lockall
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RocketLeague_Arena_Text_AreYouReady, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, RocketLeague_Arena_EventScript_StartRocketBattle
	closemessage
	applymovement LOCALID_PLAYER, RocketLeague_Arena_Movement_StepBackFromArena
	waitmovement 0
	releaseall
	end

RocketLeague_Arena_EventScript_StartRocketBattle::
	closemessage
	playbgm MUS_ENCOUNTER_ROCKET, 0
	cleartrainerflag TRAINER_TEAM_ROCKET_GRUNT_6
	trainerbattle_no_intro TRAINER_TEAM_ROCKET_GRUNT_6, RocketLeague_Arena_Text_RocketDefeat
	checkcoins VAR_TEMP_1
	goto_if_ge VAR_TEMP_1, (MAX_COINS + 1) - 250, RocketLeague_Arena_EventScript_CoinRewardFull
	addcoins 250
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RocketLeague_Arena_Text_ReceivedCoins
	playse SE_SHOP
	waitse
	closemessage
	applymovement LOCALID_PLAYER, RocketLeague_Arena_Movement_StepBackFromArena
	waitmovement 0
	releaseall
	end

RocketLeague_Arena_EventScript_CoinRewardFull::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RocketLeague_Arena_Text_CoinCaseFull
	closemessage
	applymovement LOCALID_PLAYER, RocketLeague_Arena_Movement_StepBackFromArena
	waitmovement 0
	releaseall
	end

RocketLeague_Arena_Movement_StepBackFromArena::
	walk_down
	step_end
