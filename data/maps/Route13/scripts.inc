Route13_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route13_OnTransition
	.byte 0

Route13_OnTransition::
	call_if_set FLAG_BIKE_RENTAL_ACTIVE, Route13_EventScript_ForceRentalBike
	end

Route13_EventScript_ForceRentalBike::
	special ForcePlayerOntoBike
	return

Route13_EventScript_RouteSign::
	msgbox Route13_Text_RouteSign, MSGBOX_SIGN
	end
