CeruleanCave_B1F_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, CeruleanCave_B1F_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, CeruleanCave_B1F_OnTransition
	.byte 0

CeruleanCave_B1F_OnResume::
	call_if_set FLAG_SYS_SPECIAL_WILD_BATTLE, CeruleanCave_B1F_EventScript_TryRemoveMewtwo
	end

CeruleanCave_B1F_EventScript_TryRemoveMewtwo::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, EventScript_Return
	removeobject VAR_LAST_TALKED
	return

CeruleanCave_B1F_OnTransition::
	call_if_unset FLAG_SAW_GIOVANNI_ROUTE7_RAMPAGE, CeruleanCave_B1F_EventScript_HideMewtwo
	call_if_unset FLAG_SAW_GIOVANNI_ROUTE7_RAMPAGE, CeruleanCave_B1F_EventScript_HideGiovanni
	goto_if_unset FLAG_SAW_GIOVANNI_ROUTE7_RAMPAGE, CeruleanCave_B1F_EventScript_EndTransition
	call_if_unset FLAG_FOUGHT_MEWTWO, CeruleanCave_B1F_EventScript_ShowMewtwo
	call CeruleanCave_B1F_EventScript_ShowGiovanni
	call_if_set FLAG_GIOVANNI_CERULEAN_MOVED_ASIDE, CeruleanCave_B1F_EventScript_MoveGiovanniAside
	call_if_set FLAG_SAW_GIOVANNI_CERULEAN_CAVE, CeruleanCave_B1F_EventScript_HideGiovanni
	end

CeruleanCave_B1F_EventScript_EndTransition::
	end

CeruleanCave_B1F_EventScript_ShowMewtwo::
	removeobject 3
	clearflag FLAG_HIDE_MEWTWO
	addobject 3
	return

CeruleanCave_B1F_EventScript_HideMewtwo::
	setflag FLAG_HIDE_MEWTWO
	removeobject 3
	return

CeruleanCave_B1F_EventScript_ShowGiovanni::
	goto_if_set FLAG_SAW_GIOVANNI_CERULEAN_CAVE, EventScript_Return
	removeobject LOCALID_CERULEAN_CAVE_GIOVANNI
	clearflag FLAG_HIDE_CERULEAN_CAVE_GIOVANNI
	addobject LOCALID_CERULEAN_CAVE_GIOVANNI
	return

CeruleanCave_B1F_EventScript_HideGiovanni::
	setflag FLAG_HIDE_CERULEAN_CAVE_GIOVANNI
	removeobject LOCALID_CERULEAN_CAVE_GIOVANNI
	return

CeruleanCave_B1F_EventScript_MoveGiovanniAside::
	setobjectxyperm LOCALID_CERULEAN_CAVE_GIOVANNI, 9, 16
	setobjectxy LOCALID_CERULEAN_CAVE_GIOVANNI, 9, 16
	setobjectmovementtype LOCALID_CERULEAN_CAVE_GIOVANNI, MOVEMENT_TYPE_FACE_LEFT
	return

CeruleanCave_B1F_EventScript_Giovanni::
	lock
	faceplayer
	goto_if_set FLAG_FOUGHT_MEWTWO, CeruleanCave_B1F_EventScript_GiovanniAfterMewtwoTalk
	goto_if_set FLAG_GIOVANNI_CERULEAN_MOVED_ASIDE, CeruleanCave_B1F_EventScript_GiovanniWaits
	goto CeruleanCave_B1F_EventScript_GiovanniFirstEncounter

CeruleanCave_B1F_EventScript_GiovanniFirstEncounter::
	apexlog APEX_LOG_PERSON_GIOVANNI, 4
	msgbox CeruleanCave_B1F_Text_GiovanniMewtwoFailure
	closemessage
	applymovement LOCALID_CERULEAN_CAVE_GIOVANNI, CeruleanCave_B1F_Movement_GiovanniStepsAside
	waitmovement 0
	setobjectxyperm LOCALID_CERULEAN_CAVE_GIOVANNI, 9, 16
	setobjectxy LOCALID_CERULEAN_CAVE_GIOVANNI, 9, 16
	setobjectmovementtype LOCALID_CERULEAN_CAVE_GIOVANNI, MOVEMENT_TYPE_FACE_LEFT
	setflag FLAG_GIOVANNI_CERULEAN_MOVED_ASIDE
	release
	end

CeruleanCave_B1F_EventScript_GiovanniWaits::
	msgbox CeruleanCave_B1F_Text_GiovanniWaits
	release
	end

CeruleanCave_B1F_EventScript_GiovanniApproachTrigger::
	goto_if_set FLAG_GIOVANNI_CERULEAN_MOVED_ASIDE, CeruleanCave_B1F_EventScript_GiovanniApproachTriggerEnd
	goto_if_set FLAG_FOUGHT_MEWTWO, CeruleanCave_B1F_EventScript_GiovanniApproachTriggerEnd
	goto_if_set FLAG_SAW_GIOVANNI_CERULEAN_CAVE, CeruleanCave_B1F_EventScript_GiovanniApproachTriggerEnd
	setvar VAR_TEMP_0, 1
	lock
	playse SE_PIN
	applymovement LOCALID_CERULEAN_CAVE_GIOVANNI, Common_Movement_ExclamationMark
	waitmovement 0
	waitfieldeffect FLDEFF_EXCLAMATION_MARK_ICON
	delay 12
	turnobject LOCALID_PLAYER, DIR_EAST
	turnobject LOCALID_CERULEAN_CAVE_GIOVANNI, DIR_WEST
	goto CeruleanCave_B1F_EventScript_GiovanniFirstEncounter

CeruleanCave_B1F_EventScript_GiovanniApproachTriggerEnd::
	end

CeruleanCave_B1F_EventScript_GiovanniAfterMewtwoTalk::
	goto_if_set FLAG_SAW_GIOVANNI_CERULEAN_CAVE, CeruleanCave_B1F_EventScript_GiovanniAfterMewtwoAlreadyGone
	call CeruleanCave_B1F_EventScript_GiovanniAfterMewtwo
	release
	end

CeruleanCave_B1F_EventScript_GiovanniAfterMewtwoAlreadyGone::
	release
	end

CeruleanCave_B1F_EventScript_GiovanniAfterMewtwo::
	goto_if_set FLAG_SAW_GIOVANNI_CERULEAN_CAVE, EventScript_Return
	applymovement LOCALID_CERULEAN_CAVE_GIOVANNI, CeruleanCave_B1F_Movement_GiovanniApproachesPlayerAfterMewtwo
	waitmovement 0
	turnobject LOCALID_PLAYER, DIR_SOUTH
	msgbox CeruleanCave_B1F_Text_GiovanniAfterMewtwo
	closemessage
	applymovement LOCALID_CERULEAN_CAVE_GIOVANNI, CeruleanCave_B1F_Movement_GiovanniLeaves
	waitmovement 0
	setflag FLAG_SAW_GIOVANNI_CERULEAN_CAVE
	logbookmenu LOGBOOK_MENU_SET_ACTIVE, QUEST_GIOVANNIS_AMBITION
	sublogbookmenu LOGBOOK_MENU_UNLOCK_QUEST, QUEST_GIOVANNIS_AMBITION, SUB_QUEST_GIOVANNI_CERULEAN_CAVE
	sublogbookmenu LOGBOOK_MENU_COMPLETE_QUEST, QUEST_GIOVANNIS_AMBITION, SUB_QUEST_GIOVANNI_CERULEAN_CAVE
	sublogbookmenu LOGBOOK_MENU_UNLOCK_QUEST, QUEST_GIOVANNIS_AMBITION, SUB_QUEST_GIOVANNI_VIRIDIAN_GYM
	setflag FLAG_HIDE_CERULEAN_CAVE_GIOVANNI
	removeobject LOCALID_CERULEAN_CAVE_GIOVANNI
	return

CeruleanCave_B1F_Movement_GiovanniStepsAside::
	walk_in_place_faster_left
	step_end

CeruleanCave_B1F_Movement_GiovanniApproachesPlayerAfterMewtwo::
	walk_left
	walk_left
	walk_up
	walk_up
	walk_in_place_faster_up
	step_end

CeruleanCave_B1F_Movement_GiovanniLeaves::
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

CeruleanCave_B1F_EventScript_Mewtwo::
	lock
	faceplayer
	call EventScript_ApexEncounterShake
	playmoncry SPECIES_MEWTWO, CRY_MODE_ENCOUNTER
	message CeruleanCave_B1F_Text_MewtwoApexCry
	waitmessage
	waitmoncry
	delay 20
	playbgm MUS_ENCOUNTER_GYM_LEADER, 0
	waitbuttonpress
	setflag FLAG_INTERACTED_APEX_MEWTWO
	special TryCompleteApexInstinctQuest
	seteventmon SPECIES_MEWTWO, 70
	setflag FLAG_SYS_SPECIAL_WILD_BATTLE
	special StartApexBattle
	waitstate
	clearflag FLAG_SYS_SPECIAL_WILD_BATTLE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, CeruleanCave_B1F_EventScript_DefeatedMewtwo
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, CeruleanCave_B1F_EventScript_DefeatedMewtwo
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, CeruleanCave_B1F_EventScript_RanFromMewtwo
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, CeruleanCave_B1F_EventScript_RanFromMewtwo
	setflag FLAG_FOUGHT_MEWTWO
	pokedexmsg Text_OakApexMewtwo
	release
	end

CeruleanCave_B1F_EventScript_DefeatedMewtwo::
	setflag FLAG_FOUGHT_MEWTWO
	pokedexmsg Text_OakApexMewtwo
	fadescreen FADE_TO_BLACK
	removeobject VAR_LAST_TALKED
	fadescreen FADE_FROM_BLACK
	call CeruleanCave_B1F_EventScript_GiovanniAfterMewtwo
	release
	end

CeruleanCave_B1F_EventScript_RanFromMewtwo::
	setvar VAR_0x8004, SPECIES_MEWTWO
	goto EventScript_MonFlewAway
	end

CeruleanCave_B1F_Text_MewtwoApexCry:
	.string "GRAAAH!$"

CeruleanCave_B1F_Text_GiovanniMewtwoFailure:
	.string "GIOVANNI: So this is the answer.\p"
	.string "I poured wealth, science, and will\n"
	.string "into creating absolute power.\p"
	.string "Yet when it stood before me, no\n"
	.string "order I gave could bind it.\p"
	.string "Not force. Not fear.\n"
	.string "Not even the strongest BALLS.\p"
	.string "It has judged me unworthy.\p"
	.string "Perhaps you will fare better.\n"
	.string "Go on, then.\p"
	.string "Show me what power answers to\n"
	.string "when it refuses to answer to me.$"

CeruleanCave_B1F_Text_GiovanniWaits:
	.string "GIOVANNI: MEWTWO waits.\p"
	.string "If you believe trust can reach\n"
	.string "what power could not, prove it.$"

CeruleanCave_B1F_Text_GiovanniAfterMewtwo:
	.string "GIOVANNI: So that is the difference.\p"
	.string "I tried to possess MEWTWO.\n"
	.string "You faced it as a TRAINER.\p"
	.string "Perhaps absolute power was never\n"
	.string "something a person could own.\p"
	.string "There is nothing left for me\n"
	.string "to take from this place.$"
