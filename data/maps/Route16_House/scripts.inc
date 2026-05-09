Route16_House_MapScripts::
	.byte 0

Route16_House_EventScript_Woman::
	lock
	faceplayer
	goto_if_set FLAG_GOT_TM52, Route16_House_EventScript_AlreadyGotTM52
	msgbox Route16_House_Text_FoundMySecretRetreat
	checkitemspace ITEM_TM52
	goto_if_eq VAR_RESULT, FALSE, Route16_House_EventScript_NoRoomForTM52
	giveitem_msg Route16_House_Text_ReceivedTM52FromGirl, ITEM_TM52
	msgbox Route16_House_Text_ExplainTM52
	setflag FLAG_GOT_TM52
	release
	end

Route16_House_EventScript_NoRoomForTM52::
	msgbox Route16_House_Text_DontHaveAnyRoomForThis
	release
	end

Route16_House_EventScript_AlreadyGotTM52::
	msgbox Route16_House_Text_ExplainTM52
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
