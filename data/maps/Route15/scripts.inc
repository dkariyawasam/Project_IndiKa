Route15_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route15_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, Route15_OnWarp
	.byte 0

Route15_OnTransition::
	call_if_set FLAG_BIKE_RENTAL_ACTIVE, Route15_EventScript_ForceRentalBike
	end

Route15_OnWarp::
	map_script_2 VAR_MAP_SCENE_ROUTE16, 1, Route15_OnWarpRentalBike
	.2byte 0

Route15_OnWarpRentalBike::
	special ForcePlayerOntoBike
	end

Route15_EventScript_ForceRentalBike::
	special ForcePlayerOntoBike
	return

Route15_EventScript_RouteSign::
	msgbox Route15_Text_RouteSign, MSGBOX_SIGN
	end
