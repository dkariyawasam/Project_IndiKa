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
	setmetatile 8, 51, 0x0D8, FALSE
	setmetatile 9, 51, 0x0D9, FALSE
	setmetatile 10, 51, 0x107, FALSE
	setmetatile 11, 51, 0x0E2, FALSE
	setmetatile 8, 52, 0x0D8, FALSE
	setmetatile 9, 52, 0x39E, FALSE
	setmetatile 10, 52, 0x0FF, FALSE
	setmetatile 11, 52, 0x0D2, FALSE
	setmetatile 8, 53, 0x0E0, FALSE
	setmetatile 9, 53, 0x106, FALSE
	setmetatile 10, 53, 0x0D9, FALSE
	setmetatile 11, 53, 0x0DA, FALSE
	setmetatile 8, 54, 0x009, FALSE
	setmetatile 9, 54, 0x0D8, FALSE
	setmetatile 10, 54, 0x107, FALSE
	setmetatile 11, 54, 0x0E2, FALSE
	setmetatile 8, 55, 0x008, FALSE
	setmetatile 9, 55, 0x0E0, FALSE
	setmetatile 10, 55, 0x0E2, FALSE
	setmetatile 11, 55, 0x009, FALSE
	return

Route7_EventScript_RemoveMewtwoRampageScene::
	call Route7_EventScript_HideEliteFourAftermath
	call Route7_EventScript_HideFlashbackActors
	removeobject LOCALID_ROUTE7_AGATHA
	removeobject LOCALID_ROUTE7_BRUNO
	removeobject LOCALID_ROUTE7_LORELEI
	removeobject LOCALID_ROUTE7_FLASHBACK_LORELEI
	removeobject LOCALID_ROUTE7_FLASHBACK_AGATHA
	removeobject LOCALID_ROUTE7_FLASHBACK_BRUNO
	removeobject LOCALID_ROUTE7_FLASHBACK_LANCE
	removeobject LOCALID_ROUTE7_FLASHBACK_MEWTWO
	removeobject LOCALID_ROUTE7_FLASHBACK_GIOVANNI
	end

Route7_EventScript_RemoveMewtwoAndGiovanni::
	call Route7_EventScript_HideFlashbackActors
	removeobject LOCALID_ROUTE7_FLASHBACK_LORELEI
	removeobject LOCALID_ROUTE7_FLASHBACK_AGATHA
	removeobject LOCALID_ROUTE7_FLASHBACK_BRUNO
	removeobject LOCALID_ROUTE7_FLASHBACK_LANCE
	removeobject LOCALID_ROUTE7_FLASHBACK_MEWTWO
	removeobject LOCALID_ROUTE7_FLASHBACK_GIOVANNI
	end

Route7_EventScript_ShowEliteFourAftermath::
	setobjectxyperm LOCALID_ROUTE7_AGATHA, 6, 27
	setobjectxyperm LOCALID_ROUTE7_BRUNO, 14, 45
	setobjectxyperm LOCALID_ROUTE7_LORELEI, 7, 56
	removeobject LOCALID_ROUTE7_AGATHA
	removeobject LOCALID_ROUTE7_BRUNO
	removeobject LOCALID_ROUTE7_LORELEI
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

Route7_EventScript_ShowFlashbackActors::
	clearflag FLAG_HIDE_ROUTE7_FLASHBACK_LORELEI
	clearflag FLAG_HIDE_ROUTE7_FLASHBACK_AGATHA
	clearflag FLAG_HIDE_ROUTE7_FLASHBACK_BRUNO
	clearflag FLAG_HIDE_ROUTE7_FLASHBACK_LANCE
	clearflag FLAG_HIDE_ROUTE7_FLASHBACK_MEWTWO
	clearflag FLAG_HIDE_ROUTE7_FLASHBACK_GIOVANNI
	return

Route7_EventScript_HideFlashbackActors::
	setflag FLAG_HIDE_ROUTE7_FLASHBACK_LORELEI
	setflag FLAG_HIDE_ROUTE7_FLASHBACK_AGATHA
	setflag FLAG_HIDE_ROUTE7_FLASHBACK_BRUNO
	setflag FLAG_HIDE_ROUTE7_FLASHBACK_LANCE
	setflag FLAG_HIDE_ROUTE7_FLASHBACK_MEWTWO
	setflag FLAG_HIDE_ROUTE7_FLASHBACK_GIOVANNI
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
	call Route7_EventScript_BeginEliteFourFlashbackScene
	special SlowEliteFourFlashbackMusic
	msgbox Route7_Text_LoreleiFlashback, MSGBOX_DEFAULT
	playse SE_PIN
	applymovement LOCALID_ROUTE7_FLASHBACK_MEWTWO, Common_Movement_ExclamationMark
	waitmovement 0
	waitfieldeffect FLDEFF_EXCLAMATION_MARK_ICON
	delay 12
	turnobject LOCALID_ROUTE7_FLASHBACK_MEWTWO, DIR_NORTH
	applymovement LOCALID_ROUTE7_FLASHBACK_MEWTWO, Route7_Movement_FaceUp
	waitmovement LOCALID_ROUTE7_FLASHBACK_MEWTWO
	applymovement LOCALID_ROUTE7_FLASHBACK_MEWTWO, Route7_Movement_MewtwoWalkInPlace
	call Route7_EventScript_ChargeMewtwoEnergy
	applymovement LOCALID_ROUTE7_FLASHBACK_LORELEI, Route7_Movement_LoreleiWalkInPlace
	waitmovement LOCALID_ROUTE7_FLASHBACK_LORELEI
	msgbox Route7_Text_LoreleiFlashbackFire, MSGBOX_DEFAULT
	applymovement LOCALID_ROUTE7_FLASHBACK_GIOVANNI, Route7_Movement_GiovanniWalkInPlace
	waitmovement LOCALID_ROUTE7_FLASHBACK_GIOVANNI
	msgbox Route7_Text_LoreleiFlashbackSignal, MSGBOX_DEFAULT
	special AccelerateMewtwoFlashbackSparkleLoop
	applymovement LOCALID_ROUTE7_FLASHBACK_MEWTWO, Route7_Movement_MewtwoWalkInPlace
	closemessage
	turnobject LOCALID_ROUTE7_FLASHBACK_MEWTWO, DIR_NORTH
	special ShowMewtwoFlashbackChargeCircle
	delay 8
	special ShowMewtwoFlashbackBeam
	playse SE_M_EXPLOSION
	fadescreen FADE_TO_WHITE
	delay 60
	special HideMewtwoFlashbackBeam
	special StopMewtwoFlashbackSparkleLoop
	delay 16
	call Route7_EventScript_RestoreEliteFourFlashbackSceneFromWhite
	return

Route7_EventScript_ShowBrunoFlashback::
	call Route7_EventScript_BeginBrunoFlashbackScene
	special SlowEliteFourFlashbackMusic
	applymovement LOCALID_ROUTE7_FLASHBACK_BRUNO, Route7_Movement_BrunoWalkInPlace
	waitmovement LOCALID_ROUTE7_FLASHBACK_BRUNO
	msgbox Route7_Text_BrunoFlashbackChallenge, MSGBOX_DEFAULT
	applymovement LOCALID_ROUTE7_FLASHBACK_MEWTWO, Route7_Movement_MewtwoWalkInPlaceShort
	waitmovement LOCALID_ROUTE7_FLASHBACK_MEWTWO
	call Route7_EventScript_ShoveBrunoRight
	delay 16
	fadescreen FADE_TO_WHITE
	delay 40
	call Route7_EventScript_RestoreEliteFourFlashbackSceneFromWhite
	return

Route7_EventScript_ShowAgathaFlashback::
	call Route7_EventScript_BeginEliteFourFlashbackScene
	special SlowEliteFourFlashbackMusic
	msgbox Route7_Text_AgathaFlashback, MSGBOX_DEFAULT
	call Route7_EventScript_EndEliteFourFlashbackScene
	return

Route7_EventScript_ShowLanceFlashback::
	call Route7_EventScript_BeginEliteFourFlashbackScene
	msgbox Route7_Text_LanceFlashback, MSGBOX_DEFAULT
	call Route7_EventScript_EndEliteFourFlashbackScene
	return

Route7_EventScript_BeginEliteFourFlashbackScene::
	closemessage
	fadescreen FADE_TO_WHITE
	setvar VAR_0x8004, 10
	setvar VAR_0x8005, 61
	special SetFlashbackCameraToCoords
	call Route7_EventScript_SetupEliteFourFlashbackScene
	call Route7_EventScript_RevealEliteFourFlashbackActors
	special ApplyFlashbackGreyscaleTint
	special HoldFlashbackFadeWhite
	delay 16
	fadescreen FADE_FROM_WHITE
	return

Route7_EventScript_BeginBrunoFlashbackScene::
	closemessage
	fadescreen FADE_TO_WHITE
	setvar VAR_0x8004, 10
	setvar VAR_0x8005, 46
	special SetFlashbackCameraToCoords
	call Route7_EventScript_SetupBrunoFlashbackScene
	call Route7_EventScript_RevealBrunoFlashbackActors
	special ApplyFlashbackGreyscaleTint
	special HoldFlashbackFadeWhite
	delay 16
	fadescreen FADE_FROM_WHITE
	return

Route7_EventScript_EndEliteFourFlashbackScene::
	closemessage
	fadescreen FADE_TO_WHITE
	delay 16
	call Route7_EventScript_RestoreEliteFourFlashbackSceneFromWhite
	return

Route7_EventScript_RestoreEliteFourFlashbackSceneFromWhite::
	special HideMewtwoFlashbackBeam
	special StopMewtwoFlashbackSparkleLoop
	special RestoreEliteFourFlashbackMusic
	special RestoreFlashbackGreyscaleTint
	call Route7_EventScript_RestoreAfterEliteFourFlashback
	special HoldFlashbackFadeWhite
	special RestoreFlashbackCamera
	special SuppressFlashbackPlayerGroundEffects
	special HoldFlashbackFadeWhite
	call Route7_EventScript_ShowEliteFourAftermath
	special HoldFlashbackFadeWhite
	special DrawWholeMapView
	special HoldFlashbackFadeWhite
	closemessage
	delay 20
	special HoldFlashbackFadeWhite
	special SuppressFlashbackPlayerGroundEffects
	fadescreen FADE_FROM_WHITE
	special RestoreFlashbackPlayerGroundEffects
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

Route7_EventScript_SetupEliteFourFlashbackScene::
	call Route7_EventScript_HideEliteFourAftermath
	removeobject LOCALID_ROUTE7_AGATHA
	removeobject LOCALID_ROUTE7_BRUNO
	removeobject LOCALID_ROUTE7_LORELEI
	call Route7_EventScript_RestoreBeamTrees
	special DrawWholeMapView
	setobjectxyperm LOCALID_ROUTE7_FLASHBACK_LORELEI, 8, 58
	setobjectxyperm LOCALID_ROUTE7_FLASHBACK_BRUNO, 9, 58
	setobjectxyperm LOCALID_ROUTE7_FLASHBACK_AGATHA, 10, 58
	setobjectxyperm LOCALID_ROUTE7_FLASHBACK_LANCE, 11, 58
	setobjectxyperm LOCALID_ROUTE7_FLASHBACK_MEWTWO, 10, 61
	setobjectxyperm LOCALID_ROUTE7_FLASHBACK_GIOVANNI, 10, 62
	removeobject LOCALID_ROUTE7_FLASHBACK_LORELEI
	removeobject LOCALID_ROUTE7_FLASHBACK_AGATHA
	removeobject LOCALID_ROUTE7_FLASHBACK_BRUNO
	removeobject LOCALID_ROUTE7_FLASHBACK_LANCE
	removeobject LOCALID_ROUTE7_FLASHBACK_MEWTWO
	removeobject LOCALID_ROUTE7_FLASHBACK_GIOVANNI
	return

Route7_EventScript_SetupBrunoFlashbackScene::
	call Route7_EventScript_HideEliteFourAftermath
	removeobject LOCALID_ROUTE7_AGATHA
	removeobject LOCALID_ROUTE7_BRUNO
	removeobject LOCALID_ROUTE7_LORELEI
	call Route7_EventScript_ClearBeamTrees
	special DrawWholeMapView
	setobjectxyperm LOCALID_ROUTE7_FLASHBACK_BRUNO, 10, 44
	setobjectxyperm LOCALID_ROUTE7_FLASHBACK_MEWTWO, 10, 46
	setobjectxyperm LOCALID_ROUTE7_FLASHBACK_GIOVANNI, 10, 47
	removeobject LOCALID_ROUTE7_FLASHBACK_LORELEI
	removeobject LOCALID_ROUTE7_FLASHBACK_AGATHA
	removeobject LOCALID_ROUTE7_FLASHBACK_BRUNO
	removeobject LOCALID_ROUTE7_FLASHBACK_LANCE
	removeobject LOCALID_ROUTE7_FLASHBACK_MEWTWO
	removeobject LOCALID_ROUTE7_FLASHBACK_GIOVANNI
	return

Route7_EventScript_RevealEliteFourFlashbackActors::
	call Route7_EventScript_ShowFlashbackActors
	addobject LOCALID_ROUTE7_FLASHBACK_LORELEI
	addobject LOCALID_ROUTE7_FLASHBACK_AGATHA
	addobject LOCALID_ROUTE7_FLASHBACK_BRUNO
	addobject LOCALID_ROUTE7_FLASHBACK_LANCE
	addobject LOCALID_ROUTE7_FLASHBACK_MEWTWO
	addobject LOCALID_ROUTE7_FLASHBACK_GIOVANNI
	turnobject LOCALID_ROUTE7_FLASHBACK_LORELEI, DIR_SOUTH
	turnobject LOCALID_ROUTE7_FLASHBACK_AGATHA, DIR_SOUTH
	turnobject LOCALID_ROUTE7_FLASHBACK_BRUNO, DIR_SOUTH
	turnobject LOCALID_ROUTE7_FLASHBACK_LANCE, DIR_SOUTH
	turnobject LOCALID_ROUTE7_FLASHBACK_MEWTWO, DIR_NORTH
	turnobject LOCALID_ROUTE7_FLASHBACK_GIOVANNI, DIR_NORTH
	return

Route7_EventScript_RevealBrunoFlashbackActors::
	clearflag FLAG_HIDE_ROUTE7_FLASHBACK_BRUNO
	clearflag FLAG_HIDE_ROUTE7_FLASHBACK_MEWTWO
	clearflag FLAG_HIDE_ROUTE7_FLASHBACK_GIOVANNI
	addobject LOCALID_ROUTE7_FLASHBACK_BRUNO
	addobject LOCALID_ROUTE7_FLASHBACK_MEWTWO
	addobject LOCALID_ROUTE7_FLASHBACK_GIOVANNI
	turnobject LOCALID_ROUTE7_FLASHBACK_BRUNO, DIR_SOUTH
	turnobject LOCALID_ROUTE7_FLASHBACK_MEWTWO, DIR_NORTH
	turnobject LOCALID_ROUTE7_FLASHBACK_GIOVANNI, DIR_NORTH
	return

Route7_EventScript_RestoreAfterEliteFourFlashback::
	call Route7_EventScript_ClearBeamTrees
	call Route7_EventScript_HideFlashbackActors
	removeobject LOCALID_ROUTE7_FLASHBACK_LORELEI
	removeobject LOCALID_ROUTE7_FLASHBACK_AGATHA
	removeobject LOCALID_ROUTE7_FLASHBACK_BRUNO
	removeobject LOCALID_ROUTE7_FLASHBACK_LANCE
	removeobject LOCALID_ROUTE7_FLASHBACK_MEWTWO
	removeobject LOCALID_ROUTE7_FLASHBACK_GIOVANNI
	return

Route7_EventScript_ChargeMewtwoEnergy::
	special StartMewtwoFlashbackSparkleLoop
	delay 12
	return

Route7_EventScript_ShoveBrunoRight::
	playse SE_M_STRENGTH
	setvar VAR_0x8004, 1
	setvar VAR_0x8005, 1
	setvar VAR_0x8006, 8
	setvar VAR_0x8007, 5
	applymovement LOCALID_ROUTE7_FLASHBACK_BRUNO, Route7_Movement_BrunoShovedRight
	waitmovement LOCALID_ROUTE7_FLASHBACK_BRUNO
	setvar VAR_0x8004, 13
	setvar VAR_0x8005, 44
	special SpawnFlashbackDustAtCoords
	delay 8
	setvar VAR_0x8004, 1
	setvar VAR_0x8005, 1
	special ShakeScreen
	waitstate
	return

Route7_EventScript_GiovanniChasesMewtwoTrigger::
	goto_if_unset FLAG_GIOVANNI_ACTIVATED_MEWTWO_SIGNAL, EventScript_Return
	goto_if_set FLAG_SAW_GIOVANNI_ROUTE7_RAMPAGE, EventScript_Return
	goto_if_unset FLAG_SAW_ROUTE7_LORELEI_FLASHBACK, EventScript_Return
	goto_if_unset FLAG_SAW_ROUTE7_BRUNO_FLASHBACK, EventScript_Return
	goto_if_unset FLAG_SAW_ROUTE7_AGATHA_FLASHBACK, EventScript_Return
	goto_if_unset FLAG_SAW_ROUTE7_LANCE_FLASHBACK, EventScript_Return
	lockall
	clearflag FLAG_HIDE_ROUTE7_FLASHBACK_MEWTWO
	clearflag FLAG_HIDE_ROUTE7_FLASHBACK_GIOVANNI
	setobjectxyperm LOCALID_ROUTE7_FLASHBACK_MEWTWO, 10, 14
	setobjectxyperm LOCALID_ROUTE7_FLASHBACK_GIOVANNI, 10, 15
	addobject LOCALID_ROUTE7_FLASHBACK_MEWTWO
	addobject LOCALID_ROUTE7_FLASHBACK_GIOVANNI
	applymovement LOCALID_ROUTE7_FLASHBACK_MEWTWO, Route7_Movement_MewtwoFleesNorth
	waitmovement LOCALID_ROUTE7_FLASHBACK_MEWTWO
	setflag FLAG_HIDE_ROUTE7_FLASHBACK_MEWTWO
	removeobject LOCALID_ROUTE7_FLASHBACK_MEWTWO
	msgbox Route7_Text_GiovanniChasingMewtwo, MSGBOX_DEFAULT
	applymovement LOCALID_ROUTE7_FLASHBACK_GIOVANNI, Route7_Movement_GiovanniChasesMewtwo
	waitmovement LOCALID_ROUTE7_FLASHBACK_GIOVANNI
	setflag FLAG_HIDE_ROUTE7_FLASHBACK_GIOVANNI
	removeobject LOCALID_ROUTE7_FLASHBACK_GIOVANNI
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

Route7_Movement_LoreleiWalkInPlace::
	walk_in_place_down
	walk_in_place_down
	step_end

Route7_Movement_BrunoWalkInPlace::
	walk_in_place_down
	walk_in_place_down
	step_end

Route7_Movement_BrunoShovedRight::
	face_down
	lock_facing_direction
	walk_slower_right
	walk_slower_right
	walk_slower_right
	unlock_facing_direction
	face_down
	step_end

Route7_Movement_GiovanniWalkInPlace::
	walk_in_place_up
	walk_in_place_up
	step_end

Route7_Movement_MewtwoWalkInPlaceShort::
	walk_in_place_up
	walk_in_place_up
	step_end

Route7_Movement_MewtwoWalkInPlace::
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	walk_in_place_up
	step_end

Route7_Movement_FaceUp::
	face_up
	step_end

Route7_Movement_FaceDown::
	face_down
	step_end

Route7_EventScript_UndergroundPathSign::
	msgbox Route7_Text_UndergroundPathSign, MSGBOX_SIGN
	end
