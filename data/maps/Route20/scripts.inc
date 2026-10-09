Route20_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Route20_OnFrame
	.byte 0

Route20_OnFrame::
	map_script_2 VAR_MAP_SCENE_ROUTE20, 1, Route20_EventScript_PlayerStepOffFerry
	.2byte 0

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
	goto Route20_EventScript_GorebyssTrade

Route20_EventScript_NiaFinnNiaRematch::
	trainerbattle_rematch_double TRAINER_SWIM_SIBS_NIA_FINN, Route20_Text_NiaFinnIntro, Route20_Text_NiaFinnDefeat, Route20_Text_NiaFinnNotEnoughMons
	goto Route20_EventScript_GorebyssTrade

Route20_EventScript_NiaFinnFinn::
	trainerbattle_double TRAINER_SWIM_SIBS_NIA_FINN, Route20_Text_NiaFinnIntro, Route20_Text_NiaFinnDefeat, Route20_Text_NiaFinnNotEnoughMons
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route20_EventScript_NiaFinnFinnRematch
	goto Route20_EventScript_HuntailTrade

Route20_EventScript_NiaFinnFinnRematch::
	trainerbattle_rematch_double TRAINER_SWIM_SIBS_NIA_FINN, Route20_Text_NiaFinnIntro, Route20_Text_NiaFinnDefeat, Route20_Text_NiaFinnNotEnoughMons
	goto Route20_EventScript_HuntailTrade

Route20_Text_NiaFinnIntro::
	.string "Our CLAMPERL took different paths.\nSee how they battle together!$"

Route20_Text_NiaFinnDefeat::
	.string "Our two paths met their match!$"

Route20_Text_NiaFinnPostBattle::
	.string "Keeping pace is easy in water.\nBattling together takes work!$"

Route20_Text_NiaFinnNotEnoughMons::
	.string "Bring two ready POKéMON\nso we can battle as a pair!$"

Route20_EventScript_Elyssa::
	lock
	faceplayer
	setvar VAR_0x8008, INGAME_TRADE_SEADRA
	call EventScript_GetInGameTradeSpeciesInfo
	goto_if_set FLAG_DID_SEELOR_TRADE, Route20_EventScript_Elyssa_Done
	msgbox Route20_Text_ElyssaIntro, MSGBOX_DEFAULT
	msgbox Trade_Text_DoYouHaveMonWantToTradeForMon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route20_EventScript_Elyssa_Decline
	call EventScript_ChooseMonForInGameTrade
	goto_if_ge VAR_0x8004, PARTY_SIZE, Route20_EventScript_Elyssa_Decline
	call EventScript_GetInGameTradeSpecies
	goto_if_ne VAR_RESULT, VAR_0x8009, Route20_EventScript_Elyssa_WrongMon
	call EventScript_DoInGameTrade
	setflag FLAG_DID_SEELOR_TRADE
	msgbox Route20_Text_ElyssaThanks
	release
	end

Route20_EventScript_Elyssa_Decline::
	msgbox Trade_Text_ThatsTooBad
	release
	end

Route20_EventScript_Elyssa_WrongMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox Trade_Text_ThisIsNoMon
	release
	end

Route20_EventScript_Elyssa_Done::
	msgbox Route20_Text_ElyssaDone
	release
	end

Route20_EventScript_GorebyssTrade::
	goto_if_not_defeated TRAINER_SWIM_SIBS_NIA_FINN, Route20_EventScript_NiaFinnNia
	lock
	faceplayer
	setvar VAR_0x8008, INGAME_TRADE_GOREBYSS
	call EventScript_GetInGameTradeSpeciesInfo
	goto_if_set FLAG_DID_GOREBYSS_TRADE, Route20_EventScript_GorebyssTrade_Done
	msgbox Route20_Text_GorebyssTradeIntro, MSGBOX_DEFAULT
	msgbox Trade_Text_DoYouHaveMonWantToTradeForMon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route20_EventScript_GorebyssTrade_Decline
	call EventScript_ChooseMonForInGameTrade
	goto_if_ge VAR_0x8004, PARTY_SIZE, Route20_EventScript_GorebyssTrade_Decline
	call EventScript_GetInGameTradeSpecies
	goto_if_ne VAR_RESULT, VAR_0x8009, Route20_EventScript_GorebyssTrade_WrongMon
	call EventScript_DoInGameTrade
	setflag FLAG_DID_GOREBYSS_TRADE
	msgbox Route20_Text_GorebyssTradeThanks
	release
	end

Route20_EventScript_GorebyssTrade_Decline::
	msgbox Trade_Text_ThatsTooBad
	release
	end

Route20_EventScript_GorebyssTrade_WrongMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox Trade_Text_ThisIsNoMon
	release
	end

Route20_EventScript_GorebyssTrade_Done::
	msgbox Route20_Text_GorebyssTradeDone
	release
	end

Route20_EventScript_HuntailTrade::
	goto_if_not_defeated TRAINER_SWIM_SIBS_NIA_FINN, Route20_EventScript_NiaFinnFinn
	lock
	faceplayer
	setvar VAR_0x8008, INGAME_TRADE_HUNTAIL
	call EventScript_GetInGameTradeSpeciesInfo
	goto_if_set FLAG_DID_HUNTAIL_TRADE, Route20_EventScript_HuntailTrade_Done
	msgbox Route20_Text_HuntailTradeIntro, MSGBOX_DEFAULT
	msgbox Trade_Text_DoYouHaveMonWantToTradeForMon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route20_EventScript_HuntailTrade_Decline
	call EventScript_ChooseMonForInGameTrade
	goto_if_ge VAR_0x8004, PARTY_SIZE, Route20_EventScript_HuntailTrade_Decline
	call EventScript_GetInGameTradeSpecies
	goto_if_ne VAR_RESULT, VAR_0x8009, Route20_EventScript_HuntailTrade_WrongMon
	call EventScript_DoInGameTrade
	setflag FLAG_DID_HUNTAIL_TRADE
	msgbox Route20_Text_HuntailTradeThanks
	release
	end

Route20_EventScript_HuntailTrade_Decline::
	msgbox Trade_Text_ThatsTooBad
	release
	end

Route20_EventScript_HuntailTrade_WrongMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox Trade_Text_ThisIsNoMon
	release
	end

Route20_EventScript_HuntailTrade_Done::
	msgbox Route20_Text_HuntailTradeDone
	release
	end
