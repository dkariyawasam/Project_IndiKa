DiglettsCave_SouthEntrance_MapScripts::
	.byte 0

DiglettsCave_SouthEntrance_EventScript_OldMan::
	msgbox DiglettsCave_SouthEntrance_Text_DiglettDugThisTunnel, MSGBOX_NPC
	setvar VAR_0x8004, SUB_QUEST_APEX_MIME_SR
	setvar VAR_0x8005, 0
	special RecordApexRumor
	end
