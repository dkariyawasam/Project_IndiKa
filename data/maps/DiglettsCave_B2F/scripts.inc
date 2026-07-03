DiglettsCave_B2F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, DiglettsCave_B2F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, DiglettsCave_B2F_OnResume
	.byte 0

DiglettsCave_B2F_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_DIGLETTS_CAVE_B1F
	call DiglettsCave_B2F_EventScript_HideMimeSr
	goto_if_set FLAG_FOUGHT_MIME_SR, DiglettsCave_B2F_EventScript_OnTransitionEnd
	setvar VAR_0x8004, SUB_QUEST_APEX_MIME_SR
	specialvar VAR_RESULT, IsApexRevealed
	call_if_eq VAR_RESULT, TRUE, DiglettsCave_B2F_EventScript_ShowMimeSr
DiglettsCave_B2F_EventScript_OnTransitionEnd::
	end

DiglettsCave_B2F_OnResume::
	call_if_set FLAG_SYS_SPECIAL_WILD_BATTLE, DiglettsCave_B2F_EventScript_TryRemoveMimeSr
	end

DiglettsCave_B2F_EventScript_TryRemoveMimeSr::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, EventScript_Return
	removeobject VAR_LAST_TALKED
	return

DiglettsCave_B2F_EventScript_ShowMimeSr::
	clearflag FLAG_HIDE_DIGLETTS_CAVE_MIME_SR
	return

DiglettsCave_B2F_EventScript_HideMimeSr::
	setflag FLAG_HIDE_DIGLETTS_CAVE_MIME_SR
	return

DiglettsCave_B2F_EventScript_MimeSr::
	lock
	faceplayer
	seteventmon SPECIES_MIME_SR, 50
	call EventScript_ApexEncounterShake
	playmoncry SPECIES_MIME_SR, CRY_MODE_ENCOUNTER
	message DiglettsCave_B2F_Text_MimeSrApexCry
	waitmessage
	waitmoncry
	delay 10
	playbgm MUS_ENCOUNTER_GYM_LEADER, 0
	waitbuttonpress
	setflag FLAG_INTERACTED_APEX_MIME_SR
	special TryCompleteApexInstinctQuest
	setflag FLAG_SYS_SPECIAL_WILD_BATTLE
	special StartApexBattle
	waitstate
	clearflag FLAG_SYS_SPECIAL_WILD_BATTLE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, DiglettsCave_B2F_EventScript_DefeatedMimeSr
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, DiglettsCave_B2F_EventScript_DefeatedMimeSr
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, DiglettsCave_B2F_EventScript_RanFromMimeSr
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, DiglettsCave_B2F_EventScript_RanFromMimeSr
	setflag FLAG_FOUGHT_MIME_SR
	setflag FLAG_HIDE_DIGLETTS_CAVE_MIME_SR
	pokedexmsg Text_OakApexMimeSr
	release
	end

DiglettsCave_B2F_EventScript_DefeatedMimeSr::
	setflag FLAG_FOUGHT_MIME_SR
	setflag FLAG_HIDE_DIGLETTS_CAVE_MIME_SR
	pokedexmsg Text_OakApexMimeSr
	goto EventScript_RemoveStaticMon
	end

DiglettsCave_B2F_EventScript_RanFromMimeSr::
	setflag FLAG_FOUGHT_MIME_SR
	setflag FLAG_HIDE_DIGLETTS_CAVE_MIME_SR
	setvar VAR_0x8004, SPECIES_MIME_SR
	goto EventScript_MonFlewAway
	end

DiglettsCave_B2F_Text_MimeSrApexCry:
	.string "HOOOHAH!$"
