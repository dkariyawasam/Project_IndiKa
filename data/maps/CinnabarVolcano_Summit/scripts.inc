CinnabarVolcano_Summit_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CinnabarVolcano_Summit_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CinnabarVolcano_Summit_OnResume
	.byte 0

CinnabarVolcano_Summit_OnResume::
	call_if_set FLAG_SYS_SPECIAL_WILD_BATTLE, CinnabarVolcano_Summit_EventScript_TryRemoveMoltres
	end

CinnabarVolcano_Summit_EventScript_TryRemoveMoltres::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, EventScript_Return
	removeobject VAR_LAST_TALKED
	return

CinnabarVolcano_Summit_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_CINNABAR_VOLCANO
	call_if_unset FLAG_FOUGHT_MOLTRES, CinnabarVolcano_Summit_EventScript_ShowMoltres
	end

CinnabarVolcano_Summit_EventScript_ShowMoltres::
	clearflag FLAG_HIDE_MOLTRES
	return

CinnabarVolcano_Summit_EventScript_Moltres::
	goto_if_questlog EventScript_ReleaseEnd
	special QuestLog_CutRecording
	lock
	faceplayer
	setwildbattle SPECIES_MOLTRES, 50
	waitse
	playmoncry SPECIES_MOLTRES, CRY_MODE_ENCOUNTER
	message Text_Gyaoo
	waitmessage
	waitmoncry
	delay 10
	playbgm MUS_ENCOUNTER_GYM_LEADER, 0
	waitbuttonpress
	setflag FLAG_SYS_SPECIAL_WILD_BATTLE
	special StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_SPECIAL_WILD_BATTLE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, CinnabarVolcano_Summit_EventScript_DefeatedMoltres
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, CinnabarVolcano_Summit_EventScript_RanFromMoltres
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, CinnabarVolcano_Summit_EventScript_RanFromMoltres
	setflag FLAG_FOUGHT_MOLTRES
	release
	end

CinnabarVolcano_Summit_EventScript_DefeatedMoltres::
	setflag FLAG_FOUGHT_MOLTRES
	goto EventScript_RemoveStaticMon
	end

CinnabarVolcano_Summit_EventScript_RanFromMoltres::
	setvar VAR_0x8004, SPECIES_MOLTRES
	goto EventScript_MonFlewAway
	end
