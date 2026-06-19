Route16_House_MapScripts::
	.byte 0

Route16_House_EventScript_Woman::
	lock
	faceplayer
	msgbox Route16_House_Text_FoundMySecretRetreat
	release
	end

Route16_House_EventScript_Fearow::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_FEAROW, CRY_MODE_NORMAL
	msgbox Route16_House_Text_Fearow
	waitmoncry
	release
	end
