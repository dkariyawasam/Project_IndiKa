IndigoPlateau_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, IndigoPlateau_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

IndigoPlateau_PokemonCenter_1F_OnTransition::
	special GetLeagueChallengeType
	call_if_eq VAR_RESULT, 1, IndigoPlateau_PokemonCenter_1F_EventScript_ResetChallenge
	setrespawn HEAL_LOCATION_INDIGO_PLATEAU
	end

IndigoPlateau_PokemonCenter_1F_EventScript_DoorGuard::
	logbookmenu LOGBOOK_MENU_UNLOCK_QUEST, QUEST_THE_INDIGO_LEAGUE
	lockall
	faceplayer
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox IndigoPlateau_PokemonCenter_1F_Text_FaceEliteFourGoodLuck, MSGBOX_YESNO
	goto_if_ne VAR_RESULT, YES, IndigoPlateau_PokemonCenter_1F_EventScript_EntryEnd
	special HasEnoughMonsForDoubleBattle
	goto_if_ne VAR_RESULT, PLAYER_HAS_TWO_USABLE_MONS, IndigoPlateau_PokemonCenter_1F_EventScript_NeedTwoPokemon
	closemessage
	getplayerxy VAR_TEMP_D, VAR_TEMP_E
	goto_if_eq VAR_TEMP_D, 4, IndigoPlateau_PokemonCenter_1F_EventScript_EnterArena
	goto_if_ne VAR_TEMP_D, 5, IndigoPlateau_PokemonCenter_1F_EventScript_EnterArena
	goto_if_ne VAR_TEMP_E, 3, IndigoPlateau_PokemonCenter_1F_EventScript_EnterArena
	applymovement LOCALID_PLAYER, IndigoPlateau_PokemonCenter_1F_Movement_ToArena
	waitmovement 0
IndigoPlateau_PokemonCenter_1F_EventScript_EnterArena::
	warp MAP_POKEMON_LEAGUE_BRUNOS_ROOM, 6, 11
	waitstate
	releaseall
	end

IndigoPlateau_PokemonCenter_1F_EventScript_NeedTwoPokemon::
	msgbox Common_Text_NeedTwoPokemonForDoubleBattle
IndigoPlateau_PokemonCenter_1F_EventScript_EntryEnd::
	closemessage
	releaseall
	end

IndigoPlateau_PokemonCenter_1F_Movement_ToArena::
	walk_left
	walk_up
	step_end

IndigoPlateau_PokemonCenter_1F_EventScript_GymGuy::
	logbookmenu LOGBOOK_MENU_UNLOCK_QUEST, QUEST_THE_INDIGO_LEAGUE
	msgbox IndigoPlateau_PokemonCenter_1F_Text_GymGuyAdvice, MSGBOX_NPC
	end

IndigoPlateau_PokemonCenter_1F_EventScript_Clerk::
	lock
	faceplayer
	message IndigoPlateau_PokemonCenter_1F_Text_HeldItemShop
	waitmessage
	pokemart IndigoPlateau_PokemonCenter_1F_Items
	msgbox Text_PleaseComeAgain
	release
	end

	.align 2
IndigoPlateau_PokemonCenter_1F_Items::
	.2byte ITEM_LEFTOVERS
	.2byte ITEM_SHELL_BELL
	.2byte ITEM_CHOICE_BAND
	.2byte ITEM_SCOPE_LENS
	.2byte ITEM_QUICK_CLAW
	.2byte ITEM_FOCUS_BAND
	.2byte ITEM_SILK_SCARF
	.2byte ITEM_CHARCOAL
	.2byte ITEM_MYSTIC_WATER
	.2byte ITEM_MAGNET
	.2byte ITEM_NEVER_MELT_ICE
	.2byte ITEM_POISON_BARB
	.2byte ITEM_SOFT_SAND
	.2byte ITEM_SHARP_BEAK
	.2byte ITEM_TWISTED_SPOON
	.2byte ITEM_SILVER_POWDER
	.2byte ITEM_HARD_STONE
	.2byte ITEM_SPELL_TAG
	.2byte ITEM_DRAGON_FANG
	.2byte ITEM_BLACK_GLASSES
	.2byte ITEM_METAL_COAT
	.2byte ITEM_LIGHT_BALL
	.2byte ITEM_THICK_CLUB
	.2byte ITEM_NONE
	release
	end

IndigoPlateau_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse
	release
	end

IndigoPlateau_PokemonCenter_1F_EventScript_ResetChallenge::
	special ResetLeagueChallenge
	return
