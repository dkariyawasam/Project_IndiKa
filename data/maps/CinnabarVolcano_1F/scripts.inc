CinnabarVolcano_1F_MapScripts::
	.byte 0

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

CinnabarVolcano_1F_EventScript_RockSmashMan::
	lock
	faceplayer
	goto_if_set FLAG_GOT_TM56, CinnabarVolcano_1F_EventScript_AlreadyGotTM56
	msgbox CinnabarVolcano_1F_Text_UsedThisToMakeEmberSpa
	setflag FLAG_GOT_TM56
	release
	end

CinnabarVolcano_1F_EventScript_AlreadyGotTM56::
	msgbox CinnabarVolcano_1F_Text_ExplainTM56
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
