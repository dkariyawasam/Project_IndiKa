RocketLeague_Lobby_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, RocketLeague_Lobby_OnResume
	map_script MAP_SCRIPT_ON_RETURN_TO_FIELD, RocketLeague_Lobby_OnReturnToField
	map_script MAP_SCRIPT_ON_TRANSITION, RocketLeague_Lobby_OnTransition
	map_script MAP_SCRIPT_ON_LOAD, RocketLeague_Lobby_OnLoad
	map_script MAP_SCRIPT_ON_FRAME_TABLE, RocketLeague_Lobby_OnFrame
	.byte 0

RocketLeague_Lobby_OnResume::
	setvar VAR_TEMP_2, 0
	ttower_resumetimer
	ttower_shouldexit
	goto_if_eq VAR_RESULT, FALSE, RocketLeague_Lobby_OnResumeEnd
	@ Never reached, above always FALSE
	setvar VAR_TEMP_0, 0
	setobjectxy LOCALID_PLAYER, 9, 7
	applymovement LOCALID_PLAYER, RocketLeague_Lobby_Movement_FaceReceptionist
RocketLeague_Lobby_OnResumeEnd::
	end

RocketLeague_Lobby_OnReturnToField::
	addobject LOCALID_TOWER_NURSE
	addobject LOCALID_TOWER_MART_CLERK
	addobject LOCALID_TOWER_RECEPTIONIST
	addobject LOCALID_TOWER_COOLTRAINER_F
	addobject LOCALID_TOWER_BALDING_MAN
	end

RocketLeague_Lobby_OnLoad::
	call_if_eq VAR_TEMP_D, 17, RocketLeague_Lobby_OpenCounterBarrier
	end

RocketLeague_Lobby_OpenCounterBarrier::
	setmetatile 17, 10, METATILE_TrainerTower_Floor_ShadeBottomLeft, 0
	return

RocketLeague_Lobby_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_ROCKET_LEAGUE_LOBBY
	setvar VAR_ROCKET_LEAGUE_PRIZE_TIER, 0
	setvar VAR_TEMP_3, 0
	getplayerxy VAR_TEMP_D, VAR_RESULT
	end

RocketLeague_Lobby_OnFrame::
	map_script_2 VAR_TEMP_0, 0, RocketLeague_Lobby_EventScript_Enter
	map_script_2 VAR_TEMP_D, 17, RocketLeague_Lobby_EventScript_ExitElevator
	.2byte 0

RocketLeague_Lobby_EventScript_ExitElevator::
	lockall
	applymovement LOCALID_PLAYER, RocketLeague_Lobby_Movement_ExitElevator
	waitmovement 0
	setmetatile 17, 10, METATILE_TrainerTower_CounterBarrier, 1
	special DrawWholeMapView
	playse SE_CLICK
	waitse
	setvar VAR_TEMP_D, 0
	releaseall
	end

RocketLeague_Lobby_Movement_ExitElevator::
	walk_down
	walk_down
	step_end

RocketLeague_Lobby_EventScript_Enter::
	setvar VAR_TEMP_0, 1
	ttower_getchallengestatus
	switch VAR_RESULT
	case CHALLENGE_STATUS_LOST, RocketLeague_Lobby_EventScript_LostChallenge
	case CHALLENGE_STATUS_UNK, RocketLeague_Lobby_EventScript_ExitChallengeSpeakToReceptionist
	case CHALLENGE_STATUS_NORMAL, RocketLeague_Lobby_EventScript_EnterEnd

RocketLeague_Lobby_EventScript_LostChallenge::
	lock
	faceplayer
	applymovement LOCALID_PLAYER, RocketLeague_Lobby_Movement_FaceReceptionist
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RocketLeague_Lobby_Text_TooBadComeBackTryAgain
	goto RocketLeague_Lobby_EventScript_ExitChallenge

@ Presumably E-Reader related
RocketLeague_Lobby_EventScript_ExitChallengeSpeakToReceptionist::
	lock
	faceplayer
	applymovement LOCALID_PLAYER, RocketLeague_Lobby_Movement_FaceReceptionist
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RocketLeague_Lobby_Text_MoveCounterHereWhenTrainersSwitch
RocketLeague_Lobby_EventScript_ExitChallenge::
	closemessage
	applymovement LOCALID_PLAYER, RocketLeague_Lobby_Movement_WalkDown
	waitmovement 0
	setvar VAR_MAP_SCENE_TRAINER_TOWER, 0
	release
RocketLeague_Lobby_EventScript_EnterEnd::
	end

RocketLeague_Lobby_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse
	release
	end

RocketLeague_Lobby_EventScript_Receptionist::
	lock
	faceplayer
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RocketLeague_Lobby_Text_ExplainEntry, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, RocketLeague_Lobby_EventScript_EnterArena
	release
	end

RocketLeague_Lobby_EventScript_EnterArena::
	closemessage
	warp MAP_ROCKET_LEAGUE_ARENA, 6, 11
	waitstate
	release
	end

RocketLeague_Lobby_EventScript_EntryPrompt::
	lockall
	applymovement LOCALID_TOWER_RECEPTIONIST, RocketLeague_Lobby_Movement_ReceptionistFacePlayer
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

RocketLeague_Lobby_EventScript_EntryTrigger::
	lockall
	applymovement LOCALID_PLAYER, RocketLeague_Lobby_Movement_FaceReceptionist
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RocketLeague_Lobby_Text_WelcomeToRocketLeague
	ttower_getnumfloors
	goto_if_eq VAR_RESULT, FALSE, RocketLeague_Lobby_EventScript_AllFloorsUsed
	msgbox RocketLeague_Lobby_Text_TrainersUpToFloorNum
	goto RocketLeague_Lobby_EventScript_AskEnterChallenge

RocketLeague_Lobby_EventScript_AllFloorsUsed::
	msgbox RocketLeague_Lobby_Text_TrainersUpEighthFloor
RocketLeague_Lobby_EventScript_AskEnterChallenge::
	message RocketLeague_Lobby_Text_LikeToChallengeTrainers
	waitmessage
	multichoice 18, 6, MULTICHOICE_YES_NO_INFO, FALSE
	switch VAR_RESULT
	case 0, RocketLeague_Lobby_EventScript_ChooseChallenge
	case 1, RocketLeague_Lobby_EventScript_DeclineChallenge
	case 2, RocketLeague_Lobby_EventScript_ChallengeInfo
	case SCR_MENU_CANCEL, RocketLeague_Lobby_EventScript_DeclineChallenge
	end

RocketLeague_Lobby_EventScript_ChallengeInfo::
	msgbox RocketLeague_Lobby_Text_ExplainRocketLeague
	goto RocketLeague_Lobby_EventScript_AskEnterChallenge
	end

RocketLeague_Lobby_EventScript_ChooseChallenge::
	multichoice 13, 3, MULTICHOICE_TRAINER_TOWER_MODE, FALSE
	switch VAR_RESULT
	case 0, RocketLeague_Lobby_EventScript_BeginChallenge
	case 1, RocketLeague_Lobby_EventScript_BeginChallenge
	case 2, RocketLeague_Lobby_EventScript_BeginChallenge
	case 3, RocketLeague_Lobby_EventScript_BeginChallenge
	case 4, RocketLeague_Lobby_EventScript_DeclineChallenge
	case SCR_MENU_CANCEL, RocketLeague_Lobby_EventScript_DeclineChallenge
	end

RocketLeague_Lobby_EventScript_BeginChallenge::
	copyvar VAR_0x8005, VAR_RESULT
	setvar VAR_MAP_SCENE_TRAINER_TOWER, 1
	special HealPlayerParty
	msgbox RocketLeague_Lobby_Text_StartClockGetSetGo
	ttower_startchallenge
	releaseall
	end

RocketLeague_Lobby_EventScript_DeclineChallenge::
	msgbox RocketLeague_Lobby_Text_PleaseVisitUsAgain
	closemessage
	applymovement LOCALID_PLAYER, RocketLeague_Lobby_Movement_WalkDown
	waitmovement 0
	releaseall
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
