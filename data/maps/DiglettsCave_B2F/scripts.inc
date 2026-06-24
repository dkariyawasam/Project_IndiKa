DiglettsCave_B2F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, DiglettsCave_B2F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, DiglettsCave_B2F_OnResume
	.byte 0

DiglettsCave_B2F_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_DIGLETTS_CAVE_B1F
	call_if_unset FLAG_FOUGHT_MIME_SR, DiglettsCave_B2F_EventScript_ShowMimeSr
	call_if_set FLAG_FOUGHT_MIME_SR, DiglettsCave_B2F_EventScript_HideMimeSr
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
	goto_if_questlog EventScript_ReleaseEnd
	special QuestLog_CutRecording
	lock
	faceplayer
	setwildbattle SPECIES_MIME_SR, 50
	waitse
	playmoncry SPECIES_MIME_SR, CRY_MODE_ENCOUNTER
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
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, DiglettsCave_B2F_EventScript_DefeatedMimeSr
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, DiglettsCave_B2F_EventScript_RanFromMimeSr
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, DiglettsCave_B2F_EventScript_RanFromMimeSr
	setflag FLAG_FOUGHT_MIME_SR
	setflag FLAG_HIDE_DIGLETTS_CAVE_MIME_SR
	release
	end

DiglettsCave_B2F_EventScript_DefeatedMimeSr::
	setflag FLAG_FOUGHT_MIME_SR
	setflag FLAG_HIDE_DIGLETTS_CAVE_MIME_SR
	goto EventScript_RemoveStaticMon
	end

DiglettsCave_B2F_EventScript_RanFromMimeSr::
	setflag FLAG_FOUGHT_MIME_SR
	setflag FLAG_HIDE_DIGLETTS_CAVE_MIME_SR
	setvar VAR_0x8004, SPECIES_MIME_SR
	goto EventScript_MonFlewAway
	end
