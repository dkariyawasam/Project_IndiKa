Route7_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, Route7_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, Route7_OnTransition
	.byte 0

Route7_OnLoad::
	goto_if_unset FLAG_GIOVANNI_ACTIVATED_MEWTWO_SIGNAL, Route7_EventScript_OnLoadRestoreBeamTrees
	call Route7_EventScript_ClearBeamTrees
	end

Route7_EventScript_OnLoadRestoreBeamTrees::
	call Route7_EventScript_RestoreBeamTrees
	end

Route7_OnTransition::
	goto_if_unset FLAG_GIOVANNI_ACTIVATED_MEWTWO_SIGNAL, Route7_EventScript_RemoveMewtwoRampageScene
	goto_if_set FLAG_SAW_GIOVANNI_ROUTE7_RAMPAGE, Route7_EventScript_RemoveMewtwoRampageScene
	call Route7_EventScript_ShowEliteFourAftermath
	goto Route7_EventScript_RemoveMewtwoAndGiovanni

Route7_EventScript_RestoreBeamTrees::
	setmetatile 8, 51, 0x014, TRUE
	setmetatile 9, 51, 0x015, TRUE
	setmetatile 10, 51, 0x014, TRUE
	setmetatile 11, 51, 0x015, TRUE
	setmetatile 8, 52, 0x01C, TRUE
	setmetatile 9, 52, 0x01D, TRUE
	setmetatile 10, 52, 0x01C, TRUE
	setmetatile 11, 52, 0x01D, TRUE
	setmetatile 8, 53, 0x014, TRUE
	setmetatile 9, 53, 0x015, TRUE
	setmetatile 10, 53, 0x014, TRUE
	setmetatile 11, 53, 0x015, TRUE
	setmetatile 8, 54, 0x01C, TRUE
	setmetatile 9, 54, 0x01D, TRUE
	setmetatile 10, 54, 0x01C, TRUE
	setmetatile 11, 54, 0x01D, TRUE
	setmetatile 8, 55, 0x024, TRUE
	setmetatile 9, 55, 0x025, TRUE
	setmetatile 10, 55, 0x024, TRUE
	setmetatile 11, 55, 0x025, TRUE
	return

Route7_EventScript_ClearBeamTrees::
	setmetatile 8, 51, 0x008, FALSE
	setmetatile 9, 51, 0x009, FALSE
	setmetatile 10, 51, 0x008, FALSE
	setmetatile 11, 51, 0x008, FALSE
	setmetatile 8, 52, 0x010, FALSE
	setmetatile 9, 52, 0x011, FALSE
	setmetatile 10, 52, 0x010, FALSE
	setmetatile 11, 52, 0x008, FALSE
	setmetatile 8, 53, 0x008, FALSE
	setmetatile 9, 53, 0x009, FALSE
	setmetatile 10, 53, 0x008, FALSE
	setmetatile 11, 53, 0x008, FALSE
	setmetatile 8, 54, 0x008, FALSE
	setmetatile 9, 54, 0x009, FALSE
	setmetatile 10, 54, 0x008, FALSE
	setmetatile 11, 54, 0x008, FALSE
	setmetatile 8, 55, 0x010, FALSE
	setmetatile 9, 55, 0x011, FALSE
	setmetatile 10, 55, 0x010, FALSE
	setmetatile 11, 55, 0x008, FALSE
	return

Route7_EventScript_RemoveMewtwoRampageScene::
	call Route7_EventScript_HideEliteFourAftermath
	removeobject LOCALID_ROUTE7_AGATHA
	removeobject LOCALID_ROUTE7_BRUNO
	removeobject LOCALID_ROUTE7_LANCE
	removeobject LOCALID_ROUTE7_LORELEI
	removeobject LOCALID_ROUTE7_MEWTWO
	removeobject LOCALID_ROUTE7_GIOVANNI
	end

Route7_EventScript_RemoveMewtwoAndGiovanni::
	removeobject LOCALID_ROUTE7_MEWTWO
	removeobject LOCALID_ROUTE7_GIOVANNI
	end

Route7_EventScript_ShowEliteFourAftermath::
	clearflag FLAG_HIDE_ROUTE7_AGATHA
	clearflag FLAG_HIDE_ROUTE7_BRUNO
	clearflag FLAG_HIDE_ROUTE7_LORELEI
	addobject LOCALID_ROUTE7_AGATHA
	addobject LOCALID_ROUTE7_BRUNO
	addobject LOCALID_ROUTE7_LORELEI
	return

Route7_EventScript_HideEliteFourAftermath::
	setflag FLAG_HIDE_ROUTE7_AGATHA
	setflag FLAG_HIDE_ROUTE7_BRUNO
	setflag FLAG_HIDE_ROUTE7_LORELEI
	return

Route7_EventScript_LoreleiAftermath::
	lock
	faceplayer
	goto_if_set FLAG_SAW_ROUTE7_LORELEI_FLASHBACK, Route7_EventScript_LoreleiRepeat
	msgbox Route7_Text_LoreleiAftermath, MSGBOX_DEFAULT
	call Route7_EventScript_ShowLoreleiFlashback
	setflag FLAG_SAW_ROUTE7_LORELEI_FLASHBACK
	call Route7_EventScript_CheckAllFlashbacks
	release
	end

Route7_EventScript_LoreleiRepeat::
	msgbox Route7_Text_LoreleiRepeat, MSGBOX_DEFAULT
	release
	end

Route7_EventScript_BrunoAftermath::
	lock
	faceplayer
	goto_if_set FLAG_SAW_ROUTE7_BRUNO_FLASHBACK, Route7_EventScript_BrunoRepeat
	msgbox Route7_Text_BrunoAftermath, MSGBOX_DEFAULT
	call Route7_EventScript_ShowBrunoFlashback
	setflag FLAG_SAW_ROUTE7_BRUNO_FLASHBACK
	call Route7_EventScript_CheckAllFlashbacks
	release
	end

Route7_EventScript_BrunoRepeat::
	msgbox Route7_Text_BrunoRepeat, MSGBOX_DEFAULT
	release
	end

Route7_EventScript_AgathaAftermath::
	lock
	faceplayer
	goto_if_set FLAG_SAW_ROUTE7_AGATHA_FLASHBACK, Route7_EventScript_AgathaRepeat
	msgbox Route7_Text_AgathaAftermath, MSGBOX_DEFAULT
	call Route7_EventScript_ShowAgathaFlashback
	setflag FLAG_SAW_ROUTE7_AGATHA_FLASHBACK
	call Route7_EventScript_CheckAllFlashbacks
	release
	end

Route7_EventScript_AgathaRepeat::
	msgbox Route7_Text_AgathaRepeat, MSGBOX_DEFAULT
	release
	end

Route7_EventScript_LanceAftermath::
	lock
	faceplayer
	goto_if_set FLAG_SAW_ROUTE7_LANCE_FLASHBACK, Route7_EventScript_LanceRepeat
	msgbox Route7_Text_LanceAftermath, MSGBOX_DEFAULT
	call Route7_EventScript_ShowLanceFlashback
	setflag FLAG_SAW_ROUTE7_LANCE_FLASHBACK
	call Route7_EventScript_CheckAllFlashbacks
	release
	end

Route7_EventScript_LanceRepeat::
	msgbox Route7_Text_LanceRepeat, MSGBOX_DEFAULT
	release
	end

Route7_EventScript_ShowLoreleiFlashback::
	closemessage
	fadescreen FADE_TO_BLACK
	call Route7_EventScript_SetupLoreleiFlashbackScene
	fadescreen FADE_FROM_BLACK
	special ApplyFlashbackGreyscaleTint
	delay 16
	msgbox Route7_Text_LoreleiFlashback, MSGBOX_DEFAULT
	playse SE_PIN
	applymovement LOCALID_ROUTE7_MEWTWO, Common_Movement_ExclamationMark
	waitmovement 0
	waitfieldeffect FLDEFF_EXCLAMATION_MARK_ICON
	delay 12
	msgbox Route7_Text_LoreleiFlashbackBeam, MSGBOX_DEFAULT
	fadescreen FADE_TO_BLACK
	special RestoreFlashbackGreyscaleTint
	call Route7_EventScript_RestoreAfterLoreleiFlashback
	closemessage
	fadescreen FADE_FROM_BLACK
	return

Route7_EventScript_ShowBrunoFlashback::
	closemessage
	fadescreen FADE_TO_BLACK
	fadescreen FADE_FROM_BLACK
	special ApplyFlashbackGreyscaleTint
	msgbox Route7_Text_BrunoFlashback, MSGBOX_DEFAULT
	fadescreen FADE_TO_BLACK
	special RestoreFlashbackGreyscaleTint
	closemessage
	fadescreen FADE_FROM_BLACK
	return

Route7_EventScript_ShowAgathaFlashback::
	closemessage
	fadescreen FADE_TO_BLACK
	fadescreen FADE_FROM_BLACK
	special ApplyFlashbackGreyscaleTint
	msgbox Route7_Text_AgathaFlashback, MSGBOX_DEFAULT
	fadescreen FADE_TO_BLACK
	special RestoreFlashbackGreyscaleTint
	closemessage
	fadescreen FADE_FROM_BLACK
	return

Route7_EventScript_ShowLanceFlashback::
	closemessage
	fadescreen FADE_TO_BLACK
	fadescreen FADE_FROM_BLACK
	special ApplyFlashbackGreyscaleTint
	msgbox Route7_Text_LanceFlashback, MSGBOX_DEFAULT
	fadescreen FADE_TO_BLACK
	special RestoreFlashbackGreyscaleTint
	closemessage
	fadescreen FADE_FROM_BLACK
	return

Route7_EventScript_CheckAllFlashbacks::
	goto_if_unset FLAG_SAW_ROUTE7_LORELEI_FLASHBACK, Route7_EventScript_ReturnFromFlashbackCheck
	goto_if_unset FLAG_SAW_ROUTE7_BRUNO_FLASHBACK, Route7_EventScript_ReturnFromFlashbackCheck
	goto_if_unset FLAG_SAW_ROUTE7_AGATHA_FLASHBACK, Route7_EventScript_ReturnFromFlashbackCheck
	goto_if_unset FLAG_SAW_ROUTE7_LANCE_FLASHBACK, Route7_EventScript_ReturnFromFlashbackCheck
	goto_if_set FLAG_SAW_GIOVANNI_ROUTE7_RAMPAGE, Route7_EventScript_ReturnFromFlashbackCheck
	msgbox Route7_Text_Route7FindTheCave, MSGBOX_DEFAULT
	return

Route7_EventScript_ReturnFromFlashbackCheck::
	return

Route7_EventScript_SetupLoreleiFlashbackScene::
	call Route7_EventScript_RestoreBeamTrees
	setobjectxyperm LOCALID_ROUTE7_AGATHA, 6, 57
	setobjectxyperm LOCALID_ROUTE7_BRUNO, 8, 57
	setobjectxyperm LOCALID_ROUTE7_LANCE, 7, 55
	setobjectxyperm LOCALID_ROUTE7_MEWTWO, 10, 54
	setobjectxyperm LOCALID_ROUTE7_GIOVANNI, 10, 56
	clearflag FLAG_HIDE_ROUTE7_AGATHA
	clearflag FLAG_HIDE_ROUTE7_BRUNO
	clearflag FLAG_HIDE_ROUTE7_LORELEI
	removeobject LOCALID_ROUTE7_AGATHA
	removeobject LOCALID_ROUTE7_BRUNO
	removeobject LOCALID_ROUTE7_LANCE
	removeobject LOCALID_ROUTE7_MEWTWO
	removeobject LOCALID_ROUTE7_GIOVANNI
	addobject LOCALID_ROUTE7_AGATHA
	addobject LOCALID_ROUTE7_BRUNO
	addobject LOCALID_ROUTE7_LANCE
	addobject LOCALID_ROUTE7_MEWTWO
	addobject LOCALID_ROUTE7_GIOVANNI
	applymovement LOCALID_ROUTE7_AGATHA, Route7_Movement_FaceUp
	applymovement LOCALID_ROUTE7_BRUNO, Route7_Movement_FaceUp
	applymovement LOCALID_ROUTE7_LANCE, Route7_Movement_FaceUp
	applymovement LOCALID_ROUTE7_LORELEI, Route7_Movement_FaceUp
	applymovement LOCALID_ROUTE7_MEWTWO, Route7_Movement_FaceUp
	applymovement LOCALID_ROUTE7_GIOVANNI, Route7_Movement_FaceUp
	waitmovement 0
	return

Route7_EventScript_RestoreAfterLoreleiFlashback::
	call Route7_EventScript_ClearBeamTrees
	setobjectxyperm LOCALID_ROUTE7_AGATHA, 6, 27
	setobjectxyperm LOCALID_ROUTE7_BRUNO, 14, 45
	setobjectxyperm LOCALID_ROUTE7_LANCE, 9, 15
	setobjectxyperm LOCALID_ROUTE7_MEWTWO, 10, 14
	setobjectxyperm LOCALID_ROUTE7_GIOVANNI, 10, 15
	removeobject LOCALID_ROUTE7_AGATHA
	removeobject LOCALID_ROUTE7_BRUNO
	removeobject LOCALID_ROUTE7_LANCE
	removeobject LOCALID_ROUTE7_MEWTWO
	removeobject LOCALID_ROUTE7_GIOVANNI
	addobject LOCALID_ROUTE7_AGATHA
	addobject LOCALID_ROUTE7_BRUNO
	addobject LOCALID_ROUTE7_LANCE
	removeobject LOCALID_ROUTE7_MEWTWO
	removeobject LOCALID_ROUTE7_GIOVANNI
	return

Route7_EventScript_GiovanniChasesMewtwoTrigger::
	goto_if_unset FLAG_GIOVANNI_ACTIVATED_MEWTWO_SIGNAL, EventScript_Return
	goto_if_set FLAG_SAW_GIOVANNI_ROUTE7_RAMPAGE, EventScript_Return
	goto_if_unset FLAG_SAW_ROUTE7_LORELEI_FLASHBACK, EventScript_Return
	goto_if_unset FLAG_SAW_ROUTE7_BRUNO_FLASHBACK, EventScript_Return
	goto_if_unset FLAG_SAW_ROUTE7_AGATHA_FLASHBACK, EventScript_Return
	goto_if_unset FLAG_SAW_ROUTE7_LANCE_FLASHBACK, EventScript_Return
	lockall
	setobjectxyperm LOCALID_ROUTE7_MEWTWO, 10, 14
	setobjectxyperm LOCALID_ROUTE7_GIOVANNI, 10, 15
	addobject LOCALID_ROUTE7_MEWTWO
	addobject LOCALID_ROUTE7_GIOVANNI
	applymovement LOCALID_ROUTE7_MEWTWO, Route7_Movement_MewtwoFleesNorth
	waitmovement LOCALID_ROUTE7_MEWTWO
	removeobject LOCALID_ROUTE7_MEWTWO
	msgbox Route7_Text_GiovanniChasingMewtwo, MSGBOX_DEFAULT
	applymovement LOCALID_ROUTE7_GIOVANNI, Route7_Movement_GiovanniChasesMewtwo
	waitmovement LOCALID_ROUTE7_GIOVANNI
	removeobject LOCALID_ROUTE7_GIOVANNI
	msgbox Route7_Text_Route7FullPicture, MSGBOX_DEFAULT
	setflag FLAG_SAW_GIOVANNI_ROUTE7_RAMPAGE
	setvar VAR_MAP_SCENE_ROUTE7_RAMPAGE, 1
	logbookmenu LOGBOOK_MENU_SET_ACTIVE, QUEST_GIOVANNIS_AMBITION
	sublogbookmenu LOGBOOK_MENU_UNLOCK_QUEST, QUEST_GIOVANNIS_AMBITION, SUB_QUEST_GIOVANNI_ROUTE7_RAMPAGE
	sublogbookmenu LOGBOOK_MENU_COMPLETE_QUEST, QUEST_GIOVANNIS_AMBITION, SUB_QUEST_GIOVANNI_ROUTE7_RAMPAGE
	sublogbookmenu LOGBOOK_MENU_UNLOCK_QUEST, QUEST_GIOVANNIS_AMBITION, SUB_QUEST_GIOVANNI_CERULEAN_CAVE
	releaseall
	end

Route7_Movement_MewtwoFleesNorth::
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

Route7_Movement_GiovanniChasesMewtwo::
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

Route7_Movement_FaceUp::
	face_up
	step_end

Route7_EventScript_UndergroundPathSign::
	msgbox Route7_Text_UndergroundPathSign, MSGBOX_SIGN
	end
