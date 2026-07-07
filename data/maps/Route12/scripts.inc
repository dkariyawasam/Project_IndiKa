Route12_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route12_OnTransition
	.byte 0

Route12_OnTransition::
	call_if_set FLAG_BIKE_RENTAL_ACTIVE, Route12_EventScript_ForceRentalBike
	end

Route12_EventScript_ForceRentalBike::
	special ForcePlayerOntoBike
	return

Route12_EventScript_RouteSign::
	msgbox Route12_Text_RouteSign, MSGBOX_SIGN
	end

Route12_EventScript_FishingSign::
	msgbox Route12_Text_SportfishingArea, MSGBOX_SIGN
	end
