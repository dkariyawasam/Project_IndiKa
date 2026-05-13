FiveIsland_Harbor_MapScripts::
	.byte 0

FiveIsland_Harbor_EventScript_Sailor::
	lock
	faceplayer
	msgbox Text_FerryNotRunning, MSGBOX_NPC
	release
	end
