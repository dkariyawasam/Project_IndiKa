Route19_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Route19_OnFrame
	.byte 0

Route19_OnFrame::
	map_script_2 VAR_MAP_SCENE_ROUTE19, 1, Route19_EventScript_PlayerStepOffFerry
	.2byte 0

Route19_EventScript_PlayerStepOffFerry::
	lockall
	applymovement LOCALID_PLAYER, Route19_Movement_PlayerStepOffFerry
	waitmovement 0
	setvar VAR_MAP_SCENE_ROUTE19, 0
	releaseall
	end

Route19_Movement_PlayerStepOffFerry::
	walk_up
	step_end

Route19_EventScript_RouteSign::
	msgbox Route19_Text_RouteSign, MSGBOX_SIGN
	end

Route19_EventScript_SeagallopSailor::
	lock
	faceplayer
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Route19_Text_SeagallopSailorGreeting, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route19_EventScript_SailToSeafoam
	release
	end

Route19_EventScript_SailToSeafoam::
	msgbox Route19_Text_SeagallopSailorAllAboard
	setflag FLAG_HIDE_ROUTE19_SEAGALLOP
	setflag FLAG_HIDE_ROUTE19_SAILOR
	clearflag FLAG_HIDE_ROUTE20_SEAGALLOP
	clearflag FLAG_HIDE_ROUTE20_SAILOR
	setvar VAR_MAP_SCENE_ROUTE20, 1
	setvar VAR_0x8004, SEAGALLOP_ROUTE19_FUCHSIA_SHORE
	setvar VAR_0x8006, SEAGALLOP_SEAFOAM_ISLANDS
	goto EventScript_SetSail
	end

Route19_EventScript_SlowkingTrade_Saige::
	lock
	faceplayer
	setvar VAR_0x8008, INGAME_TRADE_SLOWPOKE
	call EventScript_GetInGameTradeSpeciesInfo
	goto_if_set FLAG_DID_MS_NIDO_TRADE, Route19_EventScript_SlowkingTrade_AlreadyTraded
	msgbox Route19_Text_SlowkingTradeIntro, MSGBOX_DEFAULT
	msgbox Trade_Text_DoYouHaveMonWantToTradeForMon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route19_EventScript_SlowkingTrade_DeclineTrade
	call EventScript_ChooseMonForInGameTrade
	goto_if_ge VAR_0x8004, PARTY_SIZE, Route19_EventScript_SlowkingTrade_DeclineTrade
	call EventScript_GetInGameTradeSpecies
	goto_if_ne VAR_RESULT, VAR_0x8009, Route19_EventScript_SlowkingTrade_NotRequestedMon
	call EventScript_DoInGameTrade
	msgbox Route19_Text_SlowkingTradeThanks
	setflag FLAG_DID_MS_NIDO_TRADE
	release
	end

Route19_EventScript_SlowkingTrade_DeclineTrade::
	msgbox Trade_Text_ThatsTooBad
	release
	end

Route19_EventScript_SlowkingTrade_NotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox Trade_Text_ThisIsNoMon
	release
	end

Route19_EventScript_SlowkingTrade_AlreadyTraded::
	msgbox Route19_Text_SlowkingTradeDone
	release
	end
