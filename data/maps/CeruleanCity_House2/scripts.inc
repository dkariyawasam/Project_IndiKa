CeruleanCity_House2_MapScripts::
	.byte 0

CeruleanCity_House2_EventScript_Hiker::
	lock
	faceplayer
	msgbox CeruleanCity_House2_Text_RepairsCostABundle
	release
	end

CeruleanCity_House2_EventScript_Lass::
	msgbox CeruleanCity_House2_Text_RepairWorkIsExhausting, MSGBOX_NPC
	end

CeruleanCity_House2_EventScript_WallHole::
	msgbox CeruleanCity_House2_Text_WallNeedsRepairs, MSGBOX_SIGN
	end
