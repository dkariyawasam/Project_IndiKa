Route14_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route14_OnTransition
	.byte 0

Route14_OnTransition::
	call_if_set FLAG_BIKE_RENTAL_ACTIVE, Route14_EventScript_ForceRentalBike
	end

Route14_EventScript_ForceRentalBike::
	special ForcePlayerOntoBike
	return

Route14_EventScript_RouteSign::
	msgbox Route14_Text_RouteSign, MSGBOX_SIGN
	end
