RocketLeague_Lobby_MapScripts::
	map_script MAP_SCRIPT_ON_RETURN_TO_FIELD, RocketLeague_Lobby_OnReturnToField
	map_script MAP_SCRIPT_ON_TRANSITION, RocketLeague_Lobby_OnTransition
	.byte 0

RocketLeague_Lobby_OnReturnToField::
	addobject LOCALID_ROCKET_LEAGUE_NURSE
	addobject LOCALID_ROCKET_LEAGUE_MART_CLERK
	addobject LOCALID_ROCKET_LEAGUE_MON_CLERK
	addobject LOCALID_ROCKET_LEAGUE_RECEPTIONIST
	addobject LOCALID_ROCKET_LEAGUE_ROCKET_F
	addobject LOCALID_ROCKET_LEAGUE_ROCKET_M
	end

RocketLeague_Lobby_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_ROCKET_LEAGUE_LOBBY
	setrespawn HEAL_LOCATION_ROCKET_LEAGUE
	setvar VAR_ROCKET_LEAGUE_PRIZE_TIER, 0
	setvar VAR_TEMP_3, 0
	end

RocketLeague_Lobby_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse
	release
	end

RocketLeague_Lobby_EventScript_Receptionist::
	lockall
	faceplayer
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RocketLeague_Lobby_Text_ExplainEntry, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, RocketLeague_Lobby_EventScript_EnterArenaFromReceptionist
	releaseall
	end

RocketLeague_Lobby_EventScript_EnterArenaFromReceptionist::
	closemessage
	getplayerxy VAR_TEMP_D, VAR_RESULT
	goto_if_eq VAR_TEMP_D, 10, RocketLeague_Lobby_EventScript_EnterArenaFromReceptionistBelow
	applymovement LOCALID_PLAYER, RocketLeague_Lobby_Movement_WalkToArenaDoor
	waitmovement 0
	warp MAP_ROCKET_LEAGUE_ARENA, 6, 11
	waitstate
	releaseall
	end

RocketLeague_Lobby_EventScript_EnterArenaFromReceptionistBelow::
	applymovement LOCALID_PLAYER, RocketLeague_Lobby_Movement_WalkToArenaDoorFromBelow
	waitmovement 0
	warp MAP_ROCKET_LEAGUE_ARENA, 6, 11
	waitstate
	releaseall
	end

RocketLeague_Lobby_EventScript_EntryPrompt::
	lockall
	applymovement LOCALID_ROCKET_LEAGUE_RECEPTIONIST, RocketLeague_Lobby_Movement_ReceptionistFacePlayer
	applymovement LOCALID_PLAYER, RocketLeague_Lobby_Movement_FaceReceptionist
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RocketLeague_Lobby_Text_ExplainEntry, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, RocketLeague_Lobby_EventScript_EnterArenaFromPrompt
	closemessage
	applymovement LOCALID_PLAYER, RocketLeague_Lobby_Movement_WalkDown
	waitmovement 0
	releaseall
	end

RocketLeague_Lobby_EventScript_EnterArenaFromPrompt::
	closemessage
	applymovement LOCALID_PLAYER, RocketLeague_Lobby_Movement_WalkToArenaDoor
	waitmovement 0
	warp MAP_ROCKET_LEAGUE_ARENA, 6, 11
	waitstate
	releaseall
	end

RocketLeague_Lobby_EventScript_MartClerk::
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_PrizeClerkTMs
	end

RocketLeague_Lobby_EventScript_MonClerk::
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_PrizeClerkMons
	end

RocketLeague_Lobby_Movement_FaceReceptionist::
	face_right
	step_end

RocketLeague_Lobby_Movement_ReceptionistFacePlayer::
	face_left
	step_end

RocketLeague_Lobby_Movement_WalkDown::
	walk_down
	step_end

RocketLeague_Lobby_Movement_WalkToArenaDoor::
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

RocketLeague_Lobby_Movement_WalkToArenaDoorFromBelow::
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

RocketLeague_Lobby_EventScript_ShowRecords::
	lock
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RocketLeague_Lobby_Text_PrizeBoardIntro
	goto_if_eq VAR_ROCKET_LEAGUE_PRIZE_TIER, 0, RocketLeague_Lobby_EventScript_OfferPrizeTierOne
	goto_if_eq VAR_ROCKET_LEAGUE_PRIZE_TIER, 1, RocketLeague_Lobby_EventScript_OfferPrizeTierTwo
	goto_if_eq VAR_ROCKET_LEAGUE_PRIZE_TIER, 2, RocketLeague_Lobby_EventScript_OfferPrizeTierThree
	msgbox RocketLeague_Lobby_Text_PrizeBoardMax
	release
	end

RocketLeague_Lobby_EventScript_OfferPrizeTierOne::
	msgbox RocketLeague_Lobby_Text_BetForTierOne, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RocketLeague_Lobby_EventScript_PrizeBoardEnd
	goto RocketLeague_Lobby_EventScript_BetForTierOne

RocketLeague_Lobby_EventScript_OfferPrizeTierTwo::
	msgbox RocketLeague_Lobby_Text_BetForTierTwo, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RocketLeague_Lobby_EventScript_PrizeBoardEnd
	goto RocketLeague_Lobby_EventScript_BetForTierTwo

RocketLeague_Lobby_EventScript_OfferPrizeTierThree::
	msgbox RocketLeague_Lobby_Text_BetForTierThree, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RocketLeague_Lobby_EventScript_PrizeBoardEnd
	goto RocketLeague_Lobby_EventScript_BetForTierThree

RocketLeague_Lobby_EventScript_BetForTierOne::
	checkcoins VAR_TEMP_1
	goto_if_lt VAR_TEMP_1, 250, RocketLeague_Lobby_EventScript_NotEnoughCoinsForBet
	removecoins 250
	setvar VAR_ROCKET_LEAGUE_PRIZE_TIER, 1
	msgbox RocketLeague_Lobby_Text_PrizeTierOneSet
	playse SE_SHOP
	waitse
	release
	end

RocketLeague_Lobby_EventScript_BetForTierTwo::
	checkcoins VAR_TEMP_1
	goto_if_lt VAR_TEMP_1, 250, RocketLeague_Lobby_EventScript_NotEnoughCoinsForBet
	removecoins 250
	setvar VAR_ROCKET_LEAGUE_PRIZE_TIER, 2
	msgbox RocketLeague_Lobby_Text_PrizeTierTwoSet
	playse SE_SHOP
	waitse
	release
	end

RocketLeague_Lobby_EventScript_BetForTierThree::
	checkcoins VAR_TEMP_1
	goto_if_lt VAR_TEMP_1, 250, RocketLeague_Lobby_EventScript_NotEnoughCoinsForBet
	removecoins 250
	setvar VAR_ROCKET_LEAGUE_PRIZE_TIER, 3
	msgbox RocketLeague_Lobby_Text_PrizeTierThreeSet
	playse SE_SHOP
	waitse
	release
	end

RocketLeague_Lobby_EventScript_NotEnoughCoinsForBet::
	msgbox RocketLeague_Lobby_Text_NotEnoughCoinsForBet
	release
	end

RocketLeague_Lobby_EventScript_PrizeBoardEnd::
	release
	end

RocketLeague_Lobby_EventScript_CooltrainerF::
	msgbox RocketLeague_Lobby_Text_WonderWhatKindsOfTrainers, MSGBOX_NPC
	end

RocketLeague_Lobby_EventScript_BaldingMan::
	msgbox RocketLeague_Lobby_Text_StairsTougherThanAnyBattle, MSGBOX_NPC
	end
