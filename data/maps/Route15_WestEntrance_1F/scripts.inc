Route15_WestEntrance_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route15_WestEntrance_1F_OnTransition
	.byte 0

Route15_WestEntrance_1F_OnTransition::
	setvar VAR_TEMP_3, 0
	call_if_set FLAG_BIKE_RENTAL_ACTIVE, Route15_WestEntrance_1F_EventScript_EnableTownReturn
	setvar VAR_TEMP_2, 0
	call_if_set FLAG_BIKE_RENTAL_ACTIVE, Route15_WestEntrance_1F_EventScript_EnableRentalReturn
	setvar VAR_TEMP_1, 0
	call Route15_WestEntrance_1F_EventScript_DisableNeedBikeTriggerIfOwnBike
	call_if_set FLAG_BIKE_RENTAL_ACTIVE, Route15_WestEntrance_1F_EventScript_DisableNeedBikeTrigger
	call_if_set FLAG_BIKE_RENTAL_ACTIVE, Route15_WestEntrance_1F_EventScript_PrepareRentalReturn
	end

Route15_WestEntrance_1F_EventScript_DisableNeedBikeTriggerIfOwnBike::
	checkitem ITEM_BICYCLE, 1
	call_if_eq VAR_RESULT, TRUE, Route15_WestEntrance_1F_EventScript_DisableNeedBikeTrigger
	return

Route15_WestEntrance_1F_EventScript_DisableNeedBikeTrigger::
	setvar VAR_TEMP_1, 1
	return

Route15_WestEntrance_1F_EventScript_Guard::
	lock
	faceplayer
	checkitem ITEM_BICYCLE, 1
	goto_if_eq VAR_RESULT, TRUE, Route15_WestEntrance_1F_EventScript_GuardOwnBike
	goto_if_set FLAG_BIKE_RENTAL_ACTIVE, Route15_WestEntrance_1F_EventScript_GuardRentalActive
	showmoneybox 0, 0
	msgbox Route15_WestEntrance_1F_Text_RentBike, MSGBOX_YESNO
	hidemoneybox
	goto_if_eq VAR_RESULT, NO, Route15_WestEntrance_1F_EventScript_GuardDeclineRental
	checkmoney 500
	goto_if_eq VAR_RESULT, FALSE, Route15_WestEntrance_1F_EventScript_GuardNotEnoughMoney
	removemoney 500
	playse SE_SHOP
	setflag FLAG_BIKE_RENTAL_ACTIVE
	setvar VAR_BIKE_RENTAL_STAGE, 1
	setvar VAR_TEMP_1, 1
	msgbox Route15_WestEntrance_1F_Text_RentalStarted
	closemessage
	call Route15_WestEntrance_1F_EventScript_RentalHandoff
	release
	end

Route15_WestEntrance_1F_EventScript_GuardOwnBike::
	msgbox Route15_WestEntrance_1F_Text_OwnBike
	release
	end

Route15_WestEntrance_1F_EventScript_GuardRentalActive::
	msgbox Route15_WestEntrance_1F_Text_RentalActive
	release
	end

Route15_WestEntrance_1F_EventScript_GuardDeclineRental::
	msgbox Route15_WestEntrance_1F_Text_OaksAideCameByHere
	release
	end

Route15_WestEntrance_1F_EventScript_GuardNotEnoughMoney::
	msgbox Route15_WestEntrance_1F_Text_NotEnoughMoney
	release
	end

Route15_WestEntrance_1F_EventScript_NeedBikeTriggerTop::
	lockall
	setvar VAR_0x8008, 0
	goto Route15_WestEntrance_1F_EventScript_NeedBikeTrigger
	end

Route15_WestEntrance_1F_EventScript_NeedBikeTriggerBottom::
	lockall
	setvar VAR_0x8008, 1
	goto Route15_WestEntrance_1F_EventScript_NeedBikeTrigger
	end

Route15_WestEntrance_1F_EventScript_ReturnBikeTriggerTop::
	lockall
	setvar VAR_0x8008, 0
	goto Route15_WestEntrance_1F_EventScript_ReturnBikeTrigger
	end

Route15_WestEntrance_1F_EventScript_ReturnBikeTriggerBottom::
	lockall
	setvar VAR_0x8008, 1
	goto Route15_WestEntrance_1F_EventScript_ReturnBikeTrigger
	end

Route15_WestEntrance_1F_EventScript_NeedBikeTrigger::
	@ Only stop pedestrians entering the cycling route; swimmers may leave on foot.
	specialvar VAR_RESULT, GetPlayerFacingDirection
	goto_if_ne VAR_RESULT, DIR_EAST, Route15_WestEntrance_1F_EventScript_PedestrianExit
	checkitem ITEM_BICYCLE, 1
	goto_if_eq VAR_RESULT, TRUE, Route15_WestEntrance_1F_EventScript_NeedBikeTriggerAllow
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Route18_EastEntrance_1F_Text_ExcuseMe
	closemessage
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	delay 20
	call_if_eq VAR_0x8008, 0, Route15_WestEntrance_1F_EventScript_PlayerWalkToCounterTop
	call_if_eq VAR_0x8008, 1, Route15_WestEntrance_1F_EventScript_PlayerWalkToCounterBottom
	goto_if_set FLAG_BIKE_RENTAL_ACTIVE, Route15_WestEntrance_1F_EventScript_NeedBikeTriggerAllow
	showmoneybox 0, 0
	msgbox Route15_WestEntrance_1F_Text_RentBike, MSGBOX_YESNO
	hidemoneybox
	goto_if_eq VAR_RESULT, YES, Route15_WestEntrance_1F_EventScript_NeedBikeTriggerRent
Route15_WestEntrance_1F_EventScript_NeedBikeTriggerBounce::
	msgbox Route18_EastEntrance_1F_Text_NeedBicycleForCyclingRoad
	closemessage
	applymovement LOCALID_PLAYER, Route15_WestEntrance_1F_Movement_WalkLeft
	waitmovement 0
	releaseall
	end

Route15_WestEntrance_1F_EventScript_NeedBikeTriggerRent::
	checkmoney 500
	goto_if_eq VAR_RESULT, FALSE, Route15_WestEntrance_1F_EventScript_NeedBikeTriggerNotEnoughMoney
	removemoney 500
	playse SE_SHOP
	setflag FLAG_BIKE_RENTAL_ACTIVE
	setvar VAR_BIKE_RENTAL_STAGE, 1
	msgbox Route15_WestEntrance_1F_Text_RentalStarted
	closemessage
	setvar VAR_TEMP_1, 1
	call Route15_WestEntrance_1F_EventScript_RentalHandoff
	releaseall
	end

Route15_WestEntrance_1F_EventScript_NeedBikeTriggerNotEnoughMoney::
	msgbox Route15_WestEntrance_1F_Text_NotEnoughMoney
	closemessage
	goto Route15_WestEntrance_1F_EventScript_NeedBikeTriggerBounce

Route15_WestEntrance_1F_EventScript_NeedBikeTriggerAllow::
	setvar VAR_TEMP_1, 1
	releaseall
	end

Route15_WestEntrance_1F_EventScript_ReturnBikeTrigger::
	checkitem ITEM_BICYCLE, 1
	goto_if_eq VAR_RESULT, TRUE, EventScript_SetExitingCyclingRoad
	goto_if_unset FLAG_BIKE_RENTAL_ACTIVE, EventScript_SetExitingCyclingRoad
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Route16_NorthEntrance_1F_Text_RentalWelcomeBack
	closemessage
	clearflag FLAG_SYS_ON_CYCLING_ROAD
	special EndBikeRental
	delay 16
	getplayerxy VAR_0x8004, VAR_0x8005
	call_if_eq VAR_0x8005, 4, Route15_WestEntrance_1F_EventScript_ReturnLane4
	call_if_eq VAR_0x8005, 5, Route15_WestEntrance_1F_EventScript_ReturnLane5
	call_if_eq VAR_0x8005, 6, Route15_WestEntrance_1F_EventScript_ReturnLane6
	call_if_eq VAR_0x8005, 7, Route15_WestEntrance_1F_EventScript_ReturnLane7
	call_if_eq VAR_0x8005, 8, Route15_WestEntrance_1F_EventScript_ReturnLane8
	msgbox Route16_NorthEntrance_1F_Text_ReturnRentalBike
	closemessage
	setvar VAR_MAP_SCENE_ROUTE16, 0
	setvar VAR_TEMP_2, 0
	setvar VAR_TEMP_3, 0
	releaseall
	end

Route15_WestEntrance_1F_EventScript_PlayerWalkToCounterTop::
	applymovement LOCALID_PLAYER, Route15_WestEntrance_1F_Movement_WalkUp3
	waitmovement 0
	return

Route15_WestEntrance_1F_EventScript_PlayerWalkToCounterBottom::
	applymovement LOCALID_PLAYER, Route15_WestEntrance_1F_Movement_WalkUp4
	waitmovement 0
	return

Route15_WestEntrance_1F_EventScript_ReturnBikeWalkToGuardTop::
	applymovement LOCALID_PLAYER, Route15_WestEntrance_1F_Movement_ReturnBikeTopToGuard
	waitmovement 0
	return

Route15_WestEntrance_1F_EventScript_ReturnBikeWalkToGuardBottom::
	applymovement LOCALID_PLAYER, Route15_WestEntrance_1F_Movement_ReturnBikeBottomToGuard
	waitmovement 0
	return

Route15_WestEntrance_1F_Movement_WalkUp3::
	walk_up
	walk_up
	step_end

Route15_WestEntrance_1F_Movement_WalkUp4::
	walk_up
	walk_up
	walk_up
	step_end

Route15_WestEntrance_1F_Movement_WalkLeft::
	walk_left
	step_end


Route15_WestEntrance_1F_Movement_ReturnBikeTopToGuard::
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

Route15_WestEntrance_1F_Movement_ReturnBikeBottomToGuard::
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

Route15_WestEntrance_1F_Movement_ReturnBikeWalkOut::
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

@ A rental arriving back at either gate can be returned regardless of origin.
Route15_WestEntrance_1F_EventScript_PrepareRentalReturn::
	setvar VAR_MAP_SCENE_ROUTE16, 1
	return

@ Finish the handover on the carpet before restoring player control.
Route15_WestEntrance_1F_EventScript_RentalHandoff::
	setvar VAR_TEMP_3, 1
	setvar VAR_TEMP_2, 0
	applymovement LOCALID_PLAYER, Route15_WestEntrance_1F_Movement_RentalToCarpet
	waitmovement 0
	special ForcePlayerOntoBike
	delay 16
	return

Route15_WestEntrance_1F_Movement_RentalToCarpet::
	walk_down
	walk_down
	walk_in_place_faster_right
	step_end

Route15_WestEntrance_1F_EventScript_ReturnBikeAtCarpet::
	lockall
	goto Route15_WestEntrance_1F_EventScript_ReturnBikeTrigger

Route15_WestEntrance_1F_Movement_ReturnFromCarpet::
	walk_left
	walk_up
	walk_up
	walk_in_place_faster_up
	step_end

@ Only rentals already active when entering this gate can be returned.
Route15_WestEntrance_1F_EventScript_EnableRentalReturn::
	goto_if_eq VAR_BIKE_RENTAL_STAGE, 1, Route15_WestEntrance_1F_EventScript_ReturnNotReady
	setvar VAR_TEMP_2, 1
	return

Route15_WestEntrance_1F_EventScript_ReturnNotReady::
	return

Route15_WestEntrance_1F_EventScript_ReturnLane4::
	applymovement LOCALID_PLAYER, Route15_WestEntrance_1F_EventScript_ReturnLane4_Movement
	waitmovement 0
	return

Route15_WestEntrance_1F_EventScript_ReturnLane4_Movement::
	walk_down
	walk_down
	walk_left
	walk_up
	walk_up
	walk_in_place_faster_up
	step_end

Route15_WestEntrance_1F_EventScript_ReturnLane5::
	applymovement LOCALID_PLAYER, Route15_WestEntrance_1F_EventScript_ReturnLane5_Movement
	waitmovement 0
	return

Route15_WestEntrance_1F_EventScript_ReturnLane5_Movement::
	walk_down
	walk_left
	walk_up
	walk_up
	walk_in_place_faster_up
	step_end

Route15_WestEntrance_1F_EventScript_ReturnLane6::
	applymovement LOCALID_PLAYER, Route15_WestEntrance_1F_EventScript_ReturnLane6_Movement
	waitmovement 0
	return

Route15_WestEntrance_1F_EventScript_ReturnLane6_Movement::
	walk_left
	walk_up
	walk_up
	walk_in_place_faster_up
	step_end

Route15_WestEntrance_1F_EventScript_ReturnLane7::
	applymovement LOCALID_PLAYER, Route15_WestEntrance_1F_EventScript_ReturnLane7_Movement
	waitmovement 0
	return

Route15_WestEntrance_1F_EventScript_ReturnLane7_Movement::
	walk_up
	walk_left
	walk_up
	walk_up
	walk_in_place_faster_up
	step_end

Route15_WestEntrance_1F_EventScript_ReturnLane8::
	applymovement LOCALID_PLAYER, Route15_WestEntrance_1F_EventScript_ReturnLane8_Movement
	waitmovement 0
	return

Route15_WestEntrance_1F_EventScript_ReturnLane8_Movement::
	walk_up
	walk_up
	walk_left
	walk_up
	walk_up
	walk_in_place_faster_up
	step_end

Route15_WestEntrance_1F_EventScript_EnableTownReturn::
	setvar VAR_TEMP_3, 1
	return

Route15_WestEntrance_1F_EventScript_TownReturn0::
	lockall
	goto_if_unset FLAG_BIKE_RENTAL_ACTIVE, Route15_WestEntrance_1F_EventScript_TownReturnDone
	msgbox Route16_NorthEntrance_1F_Text_TownReturn
	closemessage
	clearflag FLAG_SYS_ON_CYCLING_ROAD
	special EndBikeRental
	delay 16
	applymovement LOCALID_PLAYER, Route15_WestEntrance_1F_EventScript_TownReturn0_Movement
	waitmovement 0
	msgbox Route16_NorthEntrance_1F_Text_ReturnRentalBike
	closemessage
	goto Route15_WestEntrance_1F_EventScript_TownReturnDone

Route15_WestEntrance_1F_EventScript_TownReturn0_Movement::
	walk_right
	walk_in_place_faster_up
	step_end

Route15_WestEntrance_1F_EventScript_TownReturn1::
	lockall
	goto_if_unset FLAG_BIKE_RENTAL_ACTIVE, Route15_WestEntrance_1F_EventScript_TownReturnDone
	msgbox Route16_NorthEntrance_1F_Text_TownReturn
	closemessage
	clearflag FLAG_SYS_ON_CYCLING_ROAD
	special EndBikeRental
	delay 16
	applymovement LOCALID_PLAYER, Route15_WestEntrance_1F_EventScript_TownReturn1_Movement
	waitmovement 0
	msgbox Route16_NorthEntrance_1F_Text_ReturnRentalBike
	closemessage
	goto Route15_WestEntrance_1F_EventScript_TownReturnDone

Route15_WestEntrance_1F_EventScript_TownReturn1_Movement::
	walk_right
	walk_up
	walk_in_place_faster_up
	step_end

Route15_WestEntrance_1F_EventScript_TownReturn2::
	lockall
	goto_if_unset FLAG_BIKE_RENTAL_ACTIVE, Route15_WestEntrance_1F_EventScript_TownReturnDone
	msgbox Route16_NorthEntrance_1F_Text_TownReturn
	closemessage
	clearflag FLAG_SYS_ON_CYCLING_ROAD
	special EndBikeRental
	delay 16
	applymovement LOCALID_PLAYER, Route15_WestEntrance_1F_EventScript_TownReturn2_Movement
	waitmovement 0
	msgbox Route16_NorthEntrance_1F_Text_ReturnRentalBike
	closemessage
	goto Route15_WestEntrance_1F_EventScript_TownReturnDone

Route15_WestEntrance_1F_EventScript_TownReturn2_Movement::
	walk_right
	walk_up
	walk_up
	walk_in_place_faster_up
	step_end

Route15_WestEntrance_1F_EventScript_TownReturn3::
	lockall
	goto_if_unset FLAG_BIKE_RENTAL_ACTIVE, Route15_WestEntrance_1F_EventScript_TownReturnDone
	msgbox Route16_NorthEntrance_1F_Text_TownReturn
	closemessage
	clearflag FLAG_SYS_ON_CYCLING_ROAD
	special EndBikeRental
	delay 16
	applymovement LOCALID_PLAYER, Route15_WestEntrance_1F_EventScript_TownReturn3_Movement
	waitmovement 0
	msgbox Route16_NorthEntrance_1F_Text_ReturnRentalBike
	closemessage
	goto Route15_WestEntrance_1F_EventScript_TownReturnDone

Route15_WestEntrance_1F_EventScript_TownReturn3_Movement::
	walk_right
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_up
	step_end

Route15_WestEntrance_1F_EventScript_TownReturn4::
	lockall
	goto_if_unset FLAG_BIKE_RENTAL_ACTIVE, Route15_WestEntrance_1F_EventScript_TownReturnDone
	msgbox Route16_NorthEntrance_1F_Text_TownReturn
	closemessage
	clearflag FLAG_SYS_ON_CYCLING_ROAD
	special EndBikeRental
	delay 16
	applymovement LOCALID_PLAYER, Route15_WestEntrance_1F_EventScript_TownReturn4_Movement
	waitmovement 0
	msgbox Route16_NorthEntrance_1F_Text_ReturnRentalBike
	closemessage
	goto Route15_WestEntrance_1F_EventScript_TownReturnDone

Route15_WestEntrance_1F_EventScript_TownReturn4_Movement::
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_up
	step_end

Route15_WestEntrance_1F_EventScript_TownReturnDone::
	setvar VAR_TEMP_2, 0
	setvar VAR_TEMP_3, 0
	setvar VAR_MAP_SCENE_ROUTE16, 0
	releaseall
	end

@ Do not disable the entry check: turning back must still require a bicycle.
Route15_WestEntrance_1F_EventScript_PedestrianExit::
	releaseall
	end
