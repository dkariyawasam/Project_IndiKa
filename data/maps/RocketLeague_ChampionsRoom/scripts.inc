RocketLeague_ChampionsRoom_MapScripts::
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, RocketLeague_ChampionsRoom_OnWarp
	map_script MAP_SCRIPT_ON_RESUME, RocketLeague_ChampionsRoom_OnResume
	map_script MAP_SCRIPT_ON_FRAME_TABLE, RocketLeague_ChampionsRoom_OnFrame
	.byte 0

RocketLeague_ChampionsRoom_OnResume::
	setvar VAR_0x8004, 4
	call PokemonLeague_EventScript_DoLightingEffect
	end

RocketLeague_ChampionsRoom_OnWarp::
	map_script_2 VAR_TEMP_1, 0, RocketLeague_ChampionsRoom_EventScript_TurnPlayerNorth
	.2byte 0

RocketLeague_ChampionsRoom_EventScript_TurnPlayerNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

RocketLeague_ChampionsRoom_OnFrame::
	map_script_2 VAR_TEMP_1, 0, RocketLeague_ChampionsRoom_EventScript_EnterRoom
	.2byte 0

RocketLeague_ChampionsRoom_EventScript_EnterRoom::
	lockall
	textcolor NPC_TEXT_COLOR_NEUTRAL
	special IsLeagueChallengeActive
	goto_if_eq VAR_RESULT, 0, RocketLeague_ChampionsRoom_EventScript_NotReady
	special GetLeagueChallengeCurrentTrainer
	goto_if_ne VAR_RESULT, TRAINER_ROCKET_LEAGUE_CHAMPION_GIOVANNI, RocketLeague_ChampionsRoom_EventScript_NotReady
	setflag FLAG_TEMP_2
	applymovement LOCALID_PLAYER, RocketLeague_ChampionsRoom_Movement_PlayerEnter
	waitmovement 0
	delay 20
	msgbox RocketLeague_ChampionsRoom_Text_GiovanniIntro
	playbgm MUS_ENCOUNTER_ROCKET, 0
	cleartrainerflag TRAINER_ROCKET_LEAGUE_CHAMPION_GIOVANNI
	trainerbattle_no_intro TRAINER_ROCKET_LEAGUE_CHAMPION_GIOVANNI, RocketLeague_ChampionsRoom_Text_GiovanniDefeat
	special AdvanceLeagueChallengeRound
	msgbox RocketLeague_ChampionsRoom_Text_PostBattle
	goto RocketLeague_ChampionsRoom_EventScript_GiveCoinReward

RocketLeague_ChampionsRoom_EventScript_NotReady::
	setvar VAR_TEMP_1, 1
	msgbox RocketLeague_ChampionsRoom_Text_NotReady
	closemessage
	warp MAP_ROCKET_LEAGUE_ARENA, 6, 2
	waitstate
	releaseall
	end

RocketLeague_ChampionsRoom_EventScript_GiveCoinReward::
	checkcoins VAR_TEMP_1
	goto_if_ge VAR_TEMP_1, (MAX_COINS + 1) - 1000, RocketLeague_ChampionsRoom_EventScript_CoinRewardFull
	addcoins 1000
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RocketLeague_ChampionsRoom_Text_ReceivedCoins
	goto RocketLeague_ChampionsRoom_EventScript_FinishCoinReward

RocketLeague_ChampionsRoom_EventScript_FinishCoinReward::
	setvar VAR_ROCKET_LEAGUE_PRIZE_TIER, 0
	playse SE_SHOP
	waitse
	closemessage
	setvar VAR_TEMP_1, 1
	warp MAP_ROCKET_LEAGUE_LOBBY, 9, 2
	waitstate
	releaseall
	end

RocketLeague_ChampionsRoom_EventScript_CoinRewardFull::
	setvar VAR_ROCKET_LEAGUE_PRIZE_TIER, 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RocketLeague_ChampionsRoom_Text_CoinCaseFull
	closemessage
	setvar VAR_TEMP_1, 1
	warp MAP_ROCKET_LEAGUE_LOBBY, 9, 2
	waitstate
	releaseall
	end

RocketLeague_ChampionsRoom_Movement_PlayerEnter::
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end
