SafariZone_SecretHouse_MapScripts::
	.byte 0

SafariZone_SecretHouse_EventScript_Attendant::
	lock
	faceplayer
	msgbox SafariZone_SecretHouse_Text_CongratsYouveWon
	msgbox SafariZone_SecretHouse_Text_ExplainSurf
	release
	end
