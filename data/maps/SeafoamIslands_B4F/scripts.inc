SeafoamIslands_B4F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SeafoamIslands_B4F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, SeafoamIslands_B4F_OnResume
	.byte 0

SeafoamIslands_B4F_OnResume::
	call_if_set FLAG_SYS_SPECIAL_WILD_BATTLE, SeafoamIslands_B4F_EventScript_TryRemoveArticuno
	end

SeafoamIslands_B4F_EventScript_TryRemoveArticuno::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, EventScript_Return
	removeobject VAR_LAST_TALKED
	return

SeafoamIslands_B4F_OnTransition::
	setmaplayoutindex LAYOUT_SEAFOAM_ISLANDS_B4F
	setflag FLAG_HIDE_ARTICUNO
	goto_if_set FLAG_FOUGHT_ARTICUNO, SeafoamIslands_B4F_EndTransition
	setvar VAR_0x8004, SUB_QUEST_APEX_ARTICUNO
	specialvar VAR_RESULT, IsApexRevealed
	call_if_eq VAR_RESULT, TRUE, SeafoamIslands_B4F_EventScript_ShowArticuno
SeafoamIslands_B4F_EndTransition::
	end

SeafoamIslands_B4F_EventScript_ShowArticuno::
	removeobject LOCALID_SEAFOAM_ARTICUNO
	clearflag FLAG_HIDE_ARTICUNO
	addobject LOCALID_SEAFOAM_ARTICUNO
	return

SeafoamIslands_B4F_EventScript_Articuno::
	lock
	faceplayer
	setvar VAR_0x8004, 50
	call EventScript_ConfirmApexEncounter
	goto_if_eq VAR_RESULT, NO, EventScript_DeclineApexEncounter
	seteventmon SPECIES_ARTICUNO, 50
	call EventScript_ApexEncounterShake
	playmoncry SPECIES_ARTICUNO, CRY_MODE_ENCOUNTER
	message SeafoamIslands_B4F_Text_ArticunoApexCry
	waitmessage
	waitmoncry
	delay 10
	playbgm MUS_ENCOUNTER_GYM_LEADER, 0
	waitbuttonpress
	setflag FLAG_INTERACTED_APEX_ARTICUNO
	special TryCompleteApexInstinctQuest
	setflag FLAG_SYS_SPECIAL_WILD_BATTLE
	special StartApexBattle
	waitstate
	clearflag FLAG_SYS_SPECIAL_WILD_BATTLE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, SeafoamIslands_B4F_EventScript_DefeatedArticuno
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, SeafoamIslands_B4F_EventScript_DefeatedArticuno
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, SeafoamIslands_B4F_EventScript_RanFromArticuno
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, SeafoamIslands_B4F_EventScript_RanFromArticuno
	setflag FLAG_FOUGHT_ARTICUNO
	pokedexmsg Text_OakApexArticuno
	release
	end

SeafoamIslands_B4F_EventScript_DefeatedArticuno::
	setflag FLAG_FOUGHT_ARTICUNO
	pokedexmsg Text_OakApexArticuno
	goto EventScript_RemoveStaticMon
	end

SeafoamIslands_B4F_EventScript_RanFromArticuno::
	setflag FLAG_FOUGHT_ARTICUNO
	setflag FLAG_HIDE_ARTICUNO
	setvar VAR_0x8004, SPECIES_ARTICUNO
	goto EventScript_MonFlewAway
	end

SeafoamIslands_B4F_Text_ArticunoApexCry:
	.string "KREEEAAH!$"

SeafoamIslands_B4F_EventScript_LakeSign::
	msgbox SeafoamIslands_B4F_Text_LakeSign, MSGBOX_SIGN
	end
