Route17_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route17_OnTransition
	.byte 0

Route17_OnTransition::
	call_if_set FLAG_BIKE_RENTAL_ACTIVE, Route17_EventScript_ForceRentalBike
	end

Route17_EventScript_ForceRentalBike::
	special ForcePlayerOntoBike
	return

Route17_EventScript_RouteSign::
	msgbox Route17_Text_RouteSign, MSGBOX_SIGN
	end

Route17_EventScript_CyclingRoadSign::
	msgbox Route17_Text_CyclingRoadSign, MSGBOX_SIGN
	end
