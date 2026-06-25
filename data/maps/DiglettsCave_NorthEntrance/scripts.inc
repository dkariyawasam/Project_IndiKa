DiglettsCave_NorthEntrance_MapScripts::
	.byte 0

DiglettsCave_NorthEntrance_EventScript_Hiker::
	msgbox DiglettsCave_NorthEntrance_RockTunnelPitchBlack, MSGBOX_NPC
	setvar VAR_0x8004, SUB_QUEST_APEX_MIME_SR
	setvar VAR_0x8005, 2
	special RecordApexRumor
	end
