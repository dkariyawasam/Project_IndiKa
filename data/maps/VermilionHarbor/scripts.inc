VermilionHarbor_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, VermilionHarbor_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, VermilionHarbor_OnFrame
	.byte 0

VermilionHarbor_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_VERMILION_HARBOR
	end

VermilionHarbor_EventScript_FerrySailor::
	lock
	faceplayer
	goto_if_eq VAR_MAP_SCENE_VERMILION_CITY, 3, VermilionHarbor_EventScript_CheckShipDeparted
	msgbox VermilionCity_Text_WelcomeToTheSSAnne
	release
	end

VermilionHarbor_EventScript_CheckShipDeparted::
	msgbox VermilionCity_Text_TheShipSetSail
	release
	end

VermilionHarbor_EventScript_CheckTicket::
	lockall
	textcolor NPC_TEXT_COLOR_NEUTRAL
	applymovement LOCALID_VERMILION_FERRY_SAILOR, Common_Movement_WalkInPlaceFasterDown
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	goto_if_eq VAR_MAP_SCENE_VERMILION_CITY, 3, VermilionHarbor_EventScript_CheckShipDepartedTrigger
	msgbox VermilionCity_Text_DoYouHaveATicket
	goto_if_unset FLAG_GOT_SS_TICKET, VermilionHarbor_EventScript_DontHaveSSTicket
	msgbox VermilionCity_Text_FlashedSSTicket
	setvar VAR_VERMILION_CITY_TICKET_CHECK_TRIGGER, 1
	releaseall
	end

VermilionHarbor_EventScript_DontHaveSSTicket::
	msgbox VermilionCity_Text_DontHaveNeededSSTicket
	closemessage
	goto VermilionHarbor_EventScript_WalkAwayFromShip
	end

VermilionHarbor_EventScript_CheckShipDepartedTrigger::
	msgbox VermilionCity_Text_TheShipSetSail
	closemessage
	goto VermilionHarbor_EventScript_WalkAwayFromShip
	end

VermilionHarbor_EventScript_WalkAwayFromShip::
	applymovement LOCALID_VERMILION_FERRY_SAILOR, Common_Movement_FaceOriginalDirection
	applymovement LOCALID_PLAYER, VermilionHarbor_Movement_WalkAwayFromShip
	waitmovement 0
	releaseall
	end

VermilionHarbor_EventScript_ExitedTicketCheck::
	lockall
	setvar VAR_VERMILION_CITY_TICKET_CHECK_TRIGGER, 0
	releaseall
	end

VermilionHarbor_EventScript_MewEncounter::
	goto_if_questlog EventScript_ReleaseEnd
	special QuestLog_CutRecording
	lock
	msgbox VermilionHarbor_Text_SomethingUnderTruck
	waitse
	playmoncry SPECIES_MEW, CRY_MODE_ENCOUNTER
	message VermilionHarbor_Text_MewCry
	waitmessage
	waitmoncry
	delay 20
	playbgm MUS_ENCOUNTER_GYM_LEADER, 0
	waitbuttonpress
	setwildbattle SPECIES_MEW, 50
	special StartWildBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, VermilionHarbor_EventScript_FoughtMew
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, VermilionHarbor_EventScript_MewFled
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, VermilionHarbor_EventScript_MewFled
	goto VermilionHarbor_EventScript_FoughtMew

VermilionHarbor_EventScript_FoughtMew::
	setflag FLAG_FOUGHT_MEW
	goto EventScript_RemoveStaticMon

VermilionHarbor_EventScript_MewFled::
	setvar VAR_0x8004, SPECIES_MEW
	goto EventScript_MonFlewAway
	end

VermilionHarbor_OnFrame::
	map_script_2 VAR_MAP_SCENE_VERMILION_CITY, 1, VermilionHarbor_ExitSSAnne
	.2byte 0

VermilionHarbor_ExitSSAnne::
	lockall
	getplayerxy VAR_0x8004, VAR_0x8005
	call_if_le VAR_0x8005, 6, VermilionHarbor_WalkDown
	call_if_ge VAR_0x8005, 7, VermilionHarbor_WalkInPlaceDown
	fadenewbgm MUS_SURF
	delay 50
	special DoSSAnneDepartureCutscene
	waitstate
	removeobject LOCALID_SS_ANNE
	applymovement LOCALID_PLAYER, VermilionHarbor_Movement_Exit
	waitmovement 0
	setvar VAR_MAP_SCENE_VERMILION_CITY, 2
	setvar VAR_VERMILION_CITY_TICKET_CHECK_TRIGGER, 0
	warp MAP_VERMILION_CITY, 23, 34
	waitstate
	releaseall
	end

VermilionHarbor_WalkDown::
	applymovement LOCALID_PLAYER, VermilionHarbor_Movement_WalkDown
	waitmovement 0
	return

VermilionHarbor_WalkInPlaceDown::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	return

VermilionHarbor_Movement_Exit::
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

VermilionHarbor_Movement_WalkAwayFromShip::
	walk_up
	step_end

VermilionHarbor_Movement_WalkDown::
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

VermilionHarbor_Text_SomethingUnderTruck::
	.string "There's something hiding under\n"
	.string "the truck!$"

VermilionHarbor_Text_MewCry::
	.string "Mew!$"
