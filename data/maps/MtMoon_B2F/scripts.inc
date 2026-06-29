MtMoon_B2F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MtMoon_B2F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, MtMoon_B2F_OnResume
	.byte 0

MtMoon_B2F_OnTransition::
	call MtMoon_B2F_EventScript_HideAnnihilape
	goto_if_set FLAG_FOUGHT_ANNIHILAPE, MtMoon_B2F_EventScript_OnTransitionEnd
	setvar VAR_0x8004, SUB_QUEST_APEX_ANNIHILAPE
	specialvar VAR_RESULT, IsApexRevealed
	call_if_eq VAR_RESULT, TRUE, MtMoon_B2F_EventScript_ShowAnnihilape
MtMoon_B2F_EventScript_OnTransitionEnd::
	end

MtMoon_B2F_OnResume::
	call_if_set FLAG_SYS_SPECIAL_WILD_BATTLE, MtMoon_B2F_EventScript_TryRemoveAnnihilape
	end

MtMoon_B2F_EventScript_TryRemoveAnnihilape::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, EventScript_Return
	removeobject VAR_LAST_TALKED
	return

MtMoon_B2F_EventScript_ShowAnnihilape::
	clearflag FLAG_HIDE_MT_MOON_ANNIHILAPE
	return

MtMoon_B2F_EventScript_HideAnnihilape::
	setflag FLAG_HIDE_MT_MOON_ANNIHILAPE
	return

MtMoon_B2F_EventScript_Annihilape::
	goto_if_questlog EventScript_ReleaseEnd
	special QuestLog_CutRecording
	lock
	faceplayer
	seteventmon SPECIES_ANNIHILAPE, 50
	call EventScript_ApexEncounterShake
	playmoncry SPECIES_ANNIHILAPE, CRY_MODE_ENCOUNTER
	message MtMoon_B2F_Text_AnnihilapeApexCry
	waitmessage
	waitmoncry
	delay 10
	playbgm MUS_ENCOUNTER_GYM_LEADER, 0
	waitbuttonpress
	setflag FLAG_INTERACTED_APEX_ANNIHILAPE
	special TryCompleteApexInstinctQuest
	setflag FLAG_SYS_SPECIAL_WILD_BATTLE
	special StartApexBattle
	waitstate
	clearflag FLAG_SYS_SPECIAL_WILD_BATTLE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, MtMoon_B2F_EventScript_DefeatedAnnihilape
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, MtMoon_B2F_EventScript_DefeatedAnnihilape
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, MtMoon_B2F_EventScript_RanFromAnnihilape
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, MtMoon_B2F_EventScript_RanFromAnnihilape
	setflag FLAG_FOUGHT_ANNIHILAPE
	setflag FLAG_HIDE_MT_MOON_ANNIHILAPE
	pokedexmsg Text_OakApexAnnihilape
	release
	end

MtMoon_B2F_EventScript_DefeatedAnnihilape::
	setflag FLAG_FOUGHT_ANNIHILAPE
	setflag FLAG_HIDE_MT_MOON_ANNIHILAPE
	pokedexmsg Text_OakApexAnnihilape
	goto EventScript_RemoveStaticMon
	end

MtMoon_B2F_EventScript_RanFromAnnihilape::
	setflag FLAG_FOUGHT_ANNIHILAPE
	setflag FLAG_HIDE_MT_MOON_ANNIHILAPE
	setvar VAR_0x8004, SPECIES_ANNIHILAPE
	goto EventScript_MonFlewAway
	end

MtMoon_B2F_Text_AnnihilapeApexCry:
	.string "GRAAAOO!$"
