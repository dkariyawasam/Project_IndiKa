Route5_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route5_OnTransition
	.byte 0

Route5_OnTransition::
	call Route5_EventScript_TrySetDayCareManPos
	end

Route5_EventScript_TrySetDayCareManPos::
	goto_if_unset FLAG_PENDING_DAYCARE_EGG, Route5_EventScript_EndSetDayCareManPos
	setobjectxyperm LOCALID_ROUTE5_DAYCARE_MAN, 24, 26
	setobjectmovementtype LOCALID_ROUTE5_DAYCARE_MAN, MOVEMENT_TYPE_FACE_DOWN
Route5_EventScript_EndSetDayCareManPos::
	return

Route5_EventScript_UndergroundPathSign::
	msgbox Route5_Text_UndergroundPathSign, MSGBOX_SIGN
	end
