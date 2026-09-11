PowerPlant_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, PowerPlant_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, PowerPlant_OnTransition
	.byte 0

PowerPlant_OnResume::
	call_if_set FLAG_SYS_SPECIAL_WILD_BATTLE, PowerPlant_EventScript_TryRemoveStaticMon
	end

PowerPlant_EventScript_TryRemoveStaticMon::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, EventScript_Return
	removeobject VAR_LAST_TALKED
	return

PowerPlant_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_POWER_PLANT
	call PowerPlant_EventScript_HideZapdos
	goto_if_set FLAG_FOUGHT_ZAPDOS, PowerPlant_EventScript_ZapdosHidden
	setvar VAR_0x8004, SUB_QUEST_APEX_ZAPDOS
	specialvar VAR_RESULT, IsApexRevealed
	call_if_eq VAR_RESULT, TRUE, PowerPlant_EventScript_ShowZapdos
PowerPlant_EventScript_ZapdosHidden::
	end

PowerPlant_EventScript_ShowZapdos::
	removeobject 6
	clearflag FLAG_HIDE_ZAPDOS
	addobject 6
	return

PowerPlant_EventScript_HideZapdos::
	setflag FLAG_HIDE_ZAPDOS
	return

PowerPlant_EventScript_WarpToRockTunnelB2F::
	setvar VAR_TEMP_1, 1
	warp MAP_ROCK_TUNNEL_B2F, 49, 26
	waitstate
	end

PowerPlant_EventScript_Zapdos::
	lock
	faceplayer
	seteventmon SPECIES_ZAPDOS, 50
	call EventScript_ApexEncounterShake
	playmoncry SPECIES_ZAPDOS, CRY_MODE_ENCOUNTER
	message PowerPlant_Text_ZapdosApexCry
	waitmessage
	waitmoncry
	delay 10
	playbgm MUS_ENCOUNTER_GYM_LEADER, 0
	waitbuttonpress
	setflag FLAG_INTERACTED_APEX_ZAPDOS
	special TryCompleteApexInstinctQuest
	setflag FLAG_SYS_SPECIAL_WILD_BATTLE
	special StartApexBattle
	waitstate
	clearflag FLAG_SYS_SPECIAL_WILD_BATTLE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, PowerPlant_EventScript_DefeatedZapdos
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, PowerPlant_EventScript_DefeatedZapdos
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, PowerPlant_EventScript_RanFromZapdos
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, PowerPlant_EventScript_RanFromZapdos
	setflag FLAG_FOUGHT_ZAPDOS
	pokedexmsg Text_OakApexZapdos
	release
	end

PowerPlant_EventScript_DefeatedZapdos::
	setflag FLAG_FOUGHT_ZAPDOS
	pokedexmsg Text_OakApexZapdos
	goto EventScript_RemoveStaticMon
	end

PowerPlant_EventScript_RanFromZapdos::
	setvar VAR_0x8004, SPECIES_ZAPDOS
	goto EventScript_MonFlewAway
	end

PowerPlant_EventScript_ElectricianLuca::
	trainerbattle_single TRAINER_POWER_PLANT_ELECTRICIAN_LUCA, PowerPlant_Text_ElectricianLucaIntro, PowerPlant_Text_ElectricianLucaDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, PowerPlant_EventScript_ElectricianLucaRematch
	msgbox PowerPlant_Text_ElectricianLucaPostBattle, MSGBOX_AUTOCLOSE
	end

PowerPlant_Text_ElectricianLucaIntro:
	.string "I'm tracing a live fault through\n"
	.string "this whole place.\p"
	.string "Keep clear unless you can handle\n"
	.string "a few sparks!$"

PowerPlant_Text_ElectricianLucaDefeat:
	.string "Shorted out!$"

PowerPlant_Text_ElectricianLucaPostBattle:
	.string "The current's unstable, but the\n"
	.string "wiring tells you where it wants\n"
	.string "to go.$"

PowerPlant_EventScript_ElectricianLucaRematch::
	trainerbattle_rematch TRAINER_POWER_PLANT_ELECTRICIAN_LUCA, PowerPlant_Text_ElectricianLucaIntro, PowerPlant_Text_ElectricianLucaDefeat
	msgbox PowerPlant_Text_ElectricianLucaPostBattle, MSGBOX_AUTOCLOSE
	end

PowerPlant_Text_ZapdosApexCry:
	.string "SKREEEAAW!$"

PowerPlant_EventScript_ElectricianOtis::
	trainerbattle_single TRAINER_ELECTRICIAN_OTIS, PowerPlant_Text_ElectricianOtisIntro, PowerPlant_Text_ElectricianOtisDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, PowerPlant_EventScript_ElectricianOtisRematch
	msgbox PowerPlant_Text_ElectricianOtisPostBattle, MSGBOX_AUTOCLOSE
	end

PowerPlant_EventScript_ElectricianOtisRematch::
	trainerbattle_rematch TRAINER_ELECTRICIAN_OTIS, PowerPlant_Text_ElectricianOtisIntro, PowerPlant_Text_ElectricianOtisDefeat
	msgbox PowerPlant_Text_ElectricianOtisPostBattle, MSGBOX_AUTOCLOSE
	end

PowerPlant_Text_ElectricianOtisIntro::
	.string "A good circuit needs balance.\nSo does a good POKéMON team!$"

PowerPlant_Text_ElectricianOtisDefeat::
	.string "My circuit is broken!$"

PowerPlant_Text_ElectricianOtisPostBattle::
	.string "One loose connection is enough\nto bring the whole system down.$"

PowerPlant_EventScript_ElectricianWade::
	trainerbattle_single TRAINER_ELECTRICIAN_WADE, PowerPlant_Text_ElectricianWadeIntro, PowerPlant_Text_ElectricianWadeDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, PowerPlant_EventScript_ElectricianWadeRematch
	msgbox PowerPlant_Text_ElectricianWadePostBattle, MSGBOX_AUTOCLOSE
	end

PowerPlant_EventScript_ElectricianWadeRematch::
	trainerbattle_rematch TRAINER_ELECTRICIAN_WADE, PowerPlant_Text_ElectricianWadeIntro, PowerPlant_Text_ElectricianWadeDefeat
	msgbox PowerPlant_Text_ElectricianWadePostBattle, MSGBOX_AUTOCLOSE
	end

PowerPlant_Text_ElectricianWadeIntro::
	.string "Careful around live wires!\nMy team carries quite a charge.$"

PowerPlant_Text_ElectricianWadeDefeat::
	.string "That was quite a shock!$"

PowerPlant_Text_ElectricianWadePostBattle::
	.string "We keep this place working.\nMy POKéMON help with that too.$"
