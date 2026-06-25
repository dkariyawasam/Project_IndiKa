CeruleanCity_House2_MapScripts::
	.byte 0

CeruleanCity_House2_EventScript_Hiker::
	lock
	faceplayer
	msgbox CeruleanCity_House2_Text_EnormousPokemonBargedIn
	setvar VAR_0x8004, SUB_QUEST_APEX_ANNIHILAPE
	setvar VAR_0x8005, 1
	call EventScript_RecordApexRumorWithFeedback
	release
	end

CeruleanCity_House2_EventScript_Lass::
	msgbox CeruleanCity_House2_Text_RepairWorkIsExhausting, MSGBOX_NPC
	end

CeruleanCity_House2_EventScript_WallHole::
	msgbox CeruleanCity_House2_Text_WallNeedsRepairs, MSGBOX_SIGN
	end
