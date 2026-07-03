CinnabarVolcano_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CinnabarVolcano_1F_OnTransition
	.byte 0

CinnabarVolcano_1F_OnTransition::
	goto_if_eq VAR_KOGA_TRIAL_STATE, 0, CinnabarVolcano_1F_EventScript_ShowKoga
	goto_if_eq VAR_KOGA_TRIAL_STATE, 1, CinnabarVolcano_1F_EventScript_ShowKoga
	setflag FLAG_HIDE_CINNABAR_VOLCANO_KOGA
	end

CinnabarVolcano_1F_EventScript_ShowKoga::
	clearflag FLAG_HIDE_CINNABAR_VOLCANO_KOGA
	end

CinnabarVolcano_1F_EventScript_OldMan::
	msgbox CinnabarVolcano_1F_Text_WaterWarmsMeToCore, MSGBOX_NPC
	end

CinnabarVolcano_1F_EventScript_BaldingMan1::
	msgbox CinnabarVolcano_1F_Text_EnjoyBowlOfChowder, MSGBOX_NPC
	end

CinnabarVolcano_1F_EventScript_BaldingMan2::
	msgbox CinnabarVolcano_1F_Text_HotSpringIsTherapeutic, MSGBOX_NPC
	end

CinnabarVolcano_1F_EventScript_OldWoman::
	msgbox CinnabarVolcano_1F_Text_SeeHowSmoothMySkinIs, MSGBOX_NPC
	end

CinnabarVolcano_1F_EventScript_BlackBelt::
	lock
	faceplayer
	famechecker FAMECHECKER_BRUNO, 3
	msgbox CinnabarVolcano_1F_Text_BrunoVisitsSpaOnOccasion
	release
	end

CinnabarVolcano_1F_EventScript_Koga::
	lock
	faceplayer
	logbookmenu LOGBOOK_MENU_UNLOCK_QUEST, QUEST_GYM_LEADER_TRIALS
	logbookmenu LOGBOOK_MENU_SET_ACTIVE, QUEST_GYM_LEADER_TRIALS
	sublogbookmenu LOGBOOK_MENU_UNLOCK_QUEST, QUEST_GYM_LEADER_TRIALS, SUB_QUEST_KOGA
	msgbox CinnabarVolcano_1F_Text_KogaLookingForLavicStone
	closemessage
	playse SE_M_EXPLOSION
	setfieldeffectargument 0, 17
	setfieldeffectargument 1, 12
	setfieldeffectargument 2, 3
	setfieldeffectargument 3, 2
	dofieldeffect FLDEFF_POP_OUT_OF_ASH
	delay 12
	setvar VAR_KOGA_TRIAL_STATE, 2
	setflag FLAG_HIDE_CINNABAR_VOLCANO_KOGA
	removeobject VAR_LAST_TALKED
	waitfieldeffect FLDEFF_POP_OUT_OF_ASH
	release
	end

CinnabarVolcano_1F_EventScript_SpaHeal::
	lockall
	fadescreen FADE_TO_WHITE
	special HealPlayerParty
	fadescreen FADE_FROM_WHITE
	msgbox CinnabarVolcano_1F_Text_WaterExquisiteFullyRefreshed
	setvar VAR_TEMP_1, 1
	releaseall
	end
