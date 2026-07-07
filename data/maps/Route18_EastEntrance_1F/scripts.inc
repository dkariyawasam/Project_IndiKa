Route18_EastEntrance_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route18_EastEntrance_1F_OnTransition
	.byte 0

Route18_EastEntrance_1F_OnTransition::
	clearflag FLAG_SYS_ON_CYCLING_ROAD
	setvar VAR_TEMP_1, 0
	call Route18_EastEntrance_1F_EventScript_DisableNeedBikeTriggerIfOwnBike
	call_if_set FLAG_BIKE_RENTAL_ACTIVE, Route18_EastEntrance_1F_EventScript_DisableNeedBikeTrigger
	end

Route18_EastEntrance_1F_EventScript_DisableNeedBikeTriggerIfOwnBike::
	checkitem ITEM_BICYCLE, 1
	call_if_eq VAR_RESULT, TRUE, Route18_EastEntrance_1F_EventScript_DisableNeedBikeTrigger
	return

Route18_EastEntrance_1F_EventScript_DisableNeedBikeTrigger::
	setvar VAR_TEMP_1, 1
	return

Route18_EastEntrance_1F_EventScript_Guard::
	lock
	faceplayer
	checkitem ITEM_BICYCLE, 1
	goto_if_eq VAR_RESULT, TRUE, Route18_EastEntrance_1F_EventScript_GuardOwnBike
	checkflag FLAG_BIKE_RENTAL_ACTIVE
	goto_if_eq VAR_RESULT, TRUE, Route18_EastEntrance_1F_EventScript_GuardRentalActive
	msgbox Route18_EastEntrance_1F_Text_RentBike, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route18_EastEntrance_1F_EventScript_GuardDeclineRental
	checkmoney 500
	goto_if_eq VAR_RESULT, FALSE, Route18_EastEntrance_1F_EventScript_GuardNotEnoughMoney
	removemoney 500
	playse SE_SHOP
	setflag FLAG_BIKE_RENTAL_ACTIVE
	setvar VAR_TEMP_1, 1
	msgbox Route18_EastEntrance_1F_Text_RentalStarted
	release
	end

Route18_EastEntrance_1F_EventScript_GuardOwnBike::
	msgbox Route18_EastEntrance_1F_Text_OwnBike
	release
	end

Route18_EastEntrance_1F_EventScript_GuardRentalActive::
	msgbox Route18_EastEntrance_1F_Text_RentalActive
	release
	end

Route18_EastEntrance_1F_EventScript_GuardDeclineRental::
	msgbox Route18_EastEntrance_1F_Text_CyclingRoadAllUphillFromHere
	release
	end

Route18_EastEntrance_1F_EventScript_GuardNotEnoughMoney::
	msgbox Route18_EastEntrance_1F_Text_NotEnoughMoney
	release
	end

Route18_EastEntrance_1F_EventScript_NeedBikeTriggerTop::
	lockall
	setvar VAR_0x8008, 0
	goto Route18_EastEntrance_1F_EventScript_NeedBikeTrigger
	end

Route18_EastEntrance_1F_EventScript_NeedBikeTriggerMidTop::
	lockall
	setvar VAR_0x8008, 1
	goto Route18_EastEntrance_1F_EventScript_NeedBikeTrigger
	end

Route18_EastEntrance_1F_EventScript_NeedBikeTriggerMid::
	lockall
	setvar VAR_0x8008, 2
	goto Route18_EastEntrance_1F_EventScript_NeedBikeTrigger
	end

Route18_EastEntrance_1F_EventScript_NeedBikeTriggerMidBottom::
	lockall
	setvar VAR_0x8008, 3
	goto Route18_EastEntrance_1F_EventScript_NeedBikeTrigger
	end

Route18_EastEntrance_1F_EventScript_NeedBikeTriggerBottom::
	lockall
	setvar VAR_0x8008, 4
	goto Route18_EastEntrance_1F_EventScript_NeedBikeTrigger
	end

Route18_EastEntrance_1F_EventScript_ReturnBikeTriggerTop::
	lockall
	setvar VAR_0x8008, 0
	goto Route18_EastEntrance_1F_EventScript_ReturnBikeTrigger
	end

Route18_EastEntrance_1F_EventScript_ReturnBikeTriggerMid::
	lockall
	setvar VAR_0x8008, 1
	goto Route18_EastEntrance_1F_EventScript_ReturnBikeTrigger
	end

Route18_EastEntrance_1F_EventScript_ReturnBikeTriggerBottom::
	lockall
	setvar VAR_0x8008, 2
	goto Route18_EastEntrance_1F_EventScript_ReturnBikeTrigger
	end

Route18_EastEntrance_1F_EventScript_ReturnBikeTrigger::
	checkitem ITEM_BICYCLE, 1
	goto_if_eq VAR_RESULT, TRUE, EventScript_SetExitingCyclingRoad
	checkflag FLAG_BIKE_RENTAL_ACTIVE
	goto_if_eq VAR_RESULT, FALSE, EventScript_SetExitingCyclingRoad
	textcolor NPC_TEXT_COLOR_NEUTRAL
	call_if_eq VAR_0x8008, 0, Route18_EastEntrance_1F_EventScript_ReturnBikeWalkToGuardTop
	call_if_eq VAR_0x8008, 1, Route18_EastEntrance_1F_EventScript_ReturnBikeWalkToGuardMid
	call_if_eq VAR_0x8008, 2, Route18_EastEntrance_1F_EventScript_ReturnBikeWalkToGuardBottom
	msgbox Route18_EastEntrance_1F_Text_ReturnRentalBike
	closemessage
	clearflag FLAG_SYS_ON_CYCLING_ROAD
	special EndBikeRental
	setvar VAR_MAP_SCENE_ROUTE16, 0
	applymovement LOCALID_PLAYER, Route18_EastEntrance_1F_Movement_ReturnBikeWalkOut
	waitmovement 0
	warp MAP_ROUTE18, 96, 9
	waitstate
	end

Route18_EastEntrance_1F_EventScript_ReturnBikeWalkToGuardTop::
	applymovement LOCALID_PLAYER, Route18_EastEntrance_1F_Movement_ReturnBikeTopToGuard
	waitmovement 0
	return

Route18_EastEntrance_1F_EventScript_ReturnBikeWalkToGuardMid::
	applymovement LOCALID_PLAYER, Route18_EastEntrance_1F_Movement_ReturnBikeMidToGuard
	waitmovement 0
	return

Route18_EastEntrance_1F_EventScript_ReturnBikeWalkToGuardBottom::
	applymovement LOCALID_PLAYER, Route18_EastEntrance_1F_Movement_ReturnBikeBottomToGuard
	waitmovement 0
	return

Route18_EastEntrance_1F_EventScript_NeedBikeTrigger::
	checkitem ITEM_BICYCLE, 1
	goto_if_eq VAR_RESULT, TRUE, Route18_EastEntrance_1F_EventScript_NeedBikeTriggerAllow
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Route18_EastEntrance_1F_Text_ExcuseMe
	closemessage
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	delay 20
	call_if_eq VAR_0x8008, 1, Route18_EastEntrance_1F_EventScript_PlayerWalkToCounterMidTop
	call_if_eq VAR_0x8008, 2, Route18_EastEntrance_1F_EventScript_PlayerWalkToCounterMid
	call_if_eq VAR_0x8008, 3, Route18_EastEntrance_1F_EventScript_PlayerWalkToCounterMidBottom
	call_if_eq VAR_0x8008, 4, Route18_EastEntrance_1F_EventScript_PlayerWalkToCounterBottom
	checkflag FLAG_BIKE_RENTAL_ACTIVE
	goto_if_eq VAR_RESULT, TRUE, Route18_EastEntrance_1F_EventScript_NeedBikeTriggerAllow
	msgbox Route18_EastEntrance_1F_Text_RentBike, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route18_EastEntrance_1F_EventScript_NeedBikeTriggerRent
Route18_EastEntrance_1F_EventScript_NeedBikeTriggerBounce::
	msgbox Route18_EastEntrance_1F_Text_NeedBicycleForCyclingRoad
	closemessage
	applymovement LOCALID_PLAYER, Route18_EastEntrance_1F_Movement_WalkRight
	waitmovement 0
	releaseall
	end

Route18_EastEntrance_1F_EventScript_NeedBikeTriggerRent::
	checkmoney 500
	goto_if_eq VAR_RESULT, FALSE, Route18_EastEntrance_1F_EventScript_NeedBikeTriggerNotEnoughMoney
	removemoney 500
	playse SE_SHOP
	setflag FLAG_BIKE_RENTAL_ACTIVE
	msgbox Route18_EastEntrance_1F_Text_RentalStarted
	closemessage
	setvar VAR_TEMP_1, 1
	applymovement LOCALID_PLAYER, Route18_EastEntrance_1F_Movement_RentalStepOut
	waitmovement 0
	setvar VAR_MAP_SCENE_ROUTE16, 1
	warp MAP_ROUTE18, 89, 9
	waitstate
	end

Route18_EastEntrance_1F_EventScript_NeedBikeTriggerNotEnoughMoney::
	msgbox Route18_EastEntrance_1F_Text_NotEnoughMoney
	closemessage
	goto Route18_EastEntrance_1F_EventScript_NeedBikeTriggerBounce

Route18_EastEntrance_1F_EventScript_NeedBikeTriggerAllow::
	setvar VAR_TEMP_1, 1
	releaseall
	end

Route18_EastEntrance_1F_EventScript_PlayerWalkToCounterMidTop::
	applymovement LOCALID_PLAYER, Route18_EastEntrance_1F_Movement_WalkUp
	waitmovement 0
	return

Route18_EastEntrance_1F_EventScript_PlayerWalkToCounterMid::
	applymovement LOCALID_PLAYER, Route18_EastEntrance_1F_Movement_WalkUp2
	waitmovement 0
	return

Route18_EastEntrance_1F_EventScript_PlayerWalkToCounterMidBottom::
	applymovement LOCALID_PLAYER, Route18_EastEntrance_1F_Movement_WalkUp3
	waitmovement 0
	return

Route18_EastEntrance_1F_EventScript_PlayerWalkToCounterBottom::
	applymovement LOCALID_PLAYER, Route18_EastEntrance_1F_Movement_WalkUp4
	waitmovement 0
	return

Route18_EastEntrance_1F_Movement_WalkUp::
	walk_up
	step_end

Route18_EastEntrance_1F_Movement_WalkUp2::
	walk_up
	walk_up
	step_end

Route18_EastEntrance_1F_Movement_WalkUp3::
	walk_up
	walk_up
	walk_up
	step_end

Route18_EastEntrance_1F_Movement_WalkUp4::
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

Route18_EastEntrance_1F_Movement_WalkRight::
	walk_right
	step_end

Route18_EastEntrance_1F_Movement_RentalStepOut::
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

Route18_EastEntrance_1F_Movement_ReturnBikeTopToGuard::
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

Route18_EastEntrance_1F_Movement_ReturnBikeMidToGuard::
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

Route18_EastEntrance_1F_Movement_ReturnBikeBottomToGuard::
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

Route18_EastEntrance_1F_Movement_ReturnBikeWalkOut::
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	step_end
