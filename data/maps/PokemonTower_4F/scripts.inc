PokemonTower_4F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PokemonTower_4F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, PokemonTower_4F_OnResume
	.byte 0

PokemonTower_4F_OnTransition::
	call PokemonTower_4F_EventScript_HideOsscythe
	goto_if_set FLAG_FOUGHT_OSSCYTHE, PokemonTower_4F_EventScript_OsscytheHidden
	setvar VAR_0x8004, SUB_QUEST_APEX_OSSCYTHE
	specialvar VAR_RESULT, IsApexRevealed
	call_if_eq VAR_RESULT, TRUE, PokemonTower_4F_EventScript_ShowOsscythe
PokemonTower_4F_EventScript_OsscytheHidden::
	end

PokemonTower_4F_OnResume::
	call_if_set FLAG_SYS_SPECIAL_WILD_BATTLE, PokemonTower_4F_EventScript_TryRemoveOsscythe
	end

PokemonTower_4F_EventScript_TryRemoveOsscythe::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, EventScript_Return
	removeobject VAR_LAST_TALKED
	return

PokemonTower_4F_EventScript_ShowOsscythe::
	clearflag FLAG_HIDE_POKEMON_TOWER_OSSCYTHE
	return

PokemonTower_4F_EventScript_HideOsscythe::
	setflag FLAG_HIDE_POKEMON_TOWER_OSSCYTHE
	return

PokemonTower_4F_EventScript_MrFuji::
	lock
	faceplayer
	famechecker FAMECHECKER_MRFUJI, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	setflag FLAG_HIDE_TOWER_FUJI
	clearflag FLAG_HIDE_POKEHOUSE_FUJI
	setflag FLAG_RESCUED_MR_FUJI
	msgbox PokemonTower_4F_Text_MrFujiThankYouFollowMe
	closemessage
	warp MAP_LAVENDER_TOWN_VOLUNTEER_POKEMON_HOUSE, 4, 7
	waitstate
	release
	end

PokemonTower_4F_EventScript_Osscythe::
	lock
	faceplayer
	seteventmon SPECIES_OSSCYTHE, 50
	call EventScript_ApexEncounterShake
	playmoncry SPECIES_OSSCYTHE, CRY_MODE_ENCOUNTER
	message PokemonTower_4F_Text_OsscytheApexCry
	waitmessage
	waitmoncry
	delay 10
	playbgm MUS_ENCOUNTER_GYM_LEADER, 0
	waitbuttonpress
	setflag FLAG_INTERACTED_APEX_OSSCYTHE
	special TryCompleteApexInstinctQuest
	setflag FLAG_SYS_SPECIAL_WILD_BATTLE
	special StartApexBattle
	waitstate
	clearflag FLAG_SYS_SPECIAL_WILD_BATTLE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, PokemonTower_4F_EventScript_DefeatedOsscythe
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, PokemonTower_4F_EventScript_DefeatedOsscythe
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, PokemonTower_4F_EventScript_RanFromOsscythe
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, PokemonTower_4F_EventScript_RanFromOsscythe
	setflag FLAG_FOUGHT_OSSCYTHE
	setflag FLAG_HIDE_POKEMON_TOWER_OSSCYTHE
	pokedexmsg Text_OakApexOsscythe
	release
	end

PokemonTower_4F_EventScript_DefeatedOsscythe::
	setflag FLAG_FOUGHT_OSSCYTHE
	setflag FLAG_HIDE_POKEMON_TOWER_OSSCYTHE
	pokedexmsg Text_OakApexOsscythe
	goto EventScript_RemoveStaticMon
	end

PokemonTower_4F_EventScript_RanFromOsscythe::
	setflag FLAG_FOUGHT_OSSCYTHE
	setflag FLAG_HIDE_POKEMON_TOWER_OSSCYTHE
	setvar VAR_0x8004, SPECIES_OSSCYTHE
	goto EventScript_MonFlewAway
	end

PokemonTower_4F_Text_OsscytheApexCry:
	.string "SKREEEAH!$"
