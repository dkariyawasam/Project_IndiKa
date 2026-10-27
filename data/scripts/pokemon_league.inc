PokemonLeague_EventScript_OpenDoor::
	applymovement LOCALID_PLAYER, Common_Movement_Delay32
	waitmovement 0
	playse SE_RS_DOOR
	call PokemonLeague_EventScript_SetDoorOpen
	special DrawWholeMapView
	setflag FLAG_TEMP_4
	return

PokemonLeague_EventScript_EnterRoom::
	applymovement LOCALID_PLAYER, Common_Movement_WalkUp5
	waitmovement 0
	setflag FLAG_TEMP_2
	playse SE_UNLOCK
	call PokemonLeague_EventScript_CloseEntry
	special DrawWholeMapView
	return

PokemonLeague_EventScript_SetDoorOpen::
	setmetatile 6, 1, METATILE_PokemonLeague_Door_Top_Open, 1
	setmetatile 6, 2, METATILE_PokemonLeague_Door_Mid_Open, 0
	return

PokemonLeague_EventScript_PreventExit::
	lockall
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Text_VoiceRangOutDontRunAway
	closemessage
	applymovement LOCALID_PLAYER, PokemonLeague_Movement_ForcePlayerIn
	waitmovement 0
	releaseall
	end

PokemonLeague_EventScript_OpenDoorLance::
	applymovement LOCALID_PLAYER, Common_Movement_Delay32
	waitmovement 0
	playse SE_RS_DOOR
	setmetatile 6, 4, METATILE_PokemonLeague_Door_Top_Open, 1
	setmetatile 6, 5, METATILE_PokemonLeague_Door_Mid_Open, 0
	special DrawWholeMapView
	setflag FLAG_TEMP_4
	return

PokemonLeague_EventScript_SetDoorOpenLance::
	setmetatile 6, 4, METATILE_PokemonLeague_Door_Top_Open, 1
	setmetatile 6, 5, METATILE_PokemonLeague_Door_Mid_Open, 0
	return

PokemonLeague_EventScript_CloseEntry::
	setmetatile 5, 11, METATILE_PokemonLeague_Entry_TopLeft_Closed, 1
	setmetatile 6, 11, METATILE_PokemonLeague_Entry_TopMid_Closed, 1
	setmetatile 7, 11, METATILE_PokemonLeague_Entry_TopRight_Closed, 1
	setmetatile 5, 12, METATILE_PokemonLeague_Entry_BottomLeft_Closed, 1
	setmetatile 6, 12, METATILE_PokemonLeague_Entry_BottomMid_Closed, 1
	setmetatile 7, 12, METATILE_PokemonLeague_Entry_BottomRight_Closed, 1
	return

PokemonLeague_Movement_ForcePlayerIn::
	walk_up
	step_end

PokemonLeague_EventScript_DoLightingEffect::
	special DoPokemonLeagueLightingEffect
	return

LeagueTerminal_EventScript_Indigo::
	lockall
	setvar VAR_TEMP_5, 1
	goto LeagueTerminal_EventScript_Menu

LeagueTerminal_EventScript_Rocket::
	lockall
	setvar VAR_TEMP_5, 2
	goto LeagueTerminal_EventScript_Menu

LeagueTerminal_EventScript_Menu::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	goto_if_eq VAR_TEMP_5, 2, LeagueTerminal_EventScript_RocketHeading
	message LeagueTerminal_Text_Indigo
	goto LeagueTerminal_EventScript_Choose

LeagueTerminal_EventScript_RocketHeading::
	message LeagueTerminal_Text_Rocket
	waitmessage
	multichoice 0, 0, MULTICHOICE_ROCKET_BOARD, FALSE
	switch VAR_RESULT
	case 0, RocketLeague_Lobby_EventScript_ShowRecords
	case 1, LeagueTerminal_EventScript_Records
	case 2, LeagueTerminal_EventScript_RocketRules
	closemessage
	releaseall
	end

LeagueTerminal_EventScript_Choose::
	waitmessage
	multichoice 0, 0, MULTICHOICE_LEAGUE_TERMINAL, FALSE
	switch VAR_RESULT
	case 0, LeagueTerminal_EventScript_Rules
	case 1, LeagueTerminal_EventScript_Status
	case 2, LeagueTerminal_EventScript_Records
	case 3, LeagueTerminal_EventScript_Rewards
	closemessage
	releaseall
	end

LeagueTerminal_EventScript_Rules::
	goto_if_eq VAR_TEMP_5, 2, LeagueTerminal_EventScript_RocketRules
	msgbox LeagueTerminal_Text_IndigoRules
	goto LeagueTerminal_EventScript_Menu
LeagueTerminal_EventScript_RocketRules::
	msgbox LeagueTerminal_Text_RocketRules
	goto LeagueTerminal_EventScript_Menu

LeagueTerminal_EventScript_Status::
	copyvar VAR_0x8004, VAR_TEMP_5
	special BufferLeagueTerminalStatus
	msgbox gStringVar4
	goto LeagueTerminal_EventScript_Menu

LeagueTerminal_EventScript_Records::
	closemessage
	fadescreen FADE_TO_BLACK
	copyvar VAR_0x8004, VAR_TEMP_5
	special ShowLeagueRecords
	waitstate
	goto LeagueTerminal_EventScript_Menu

LeagueTerminal_EventScript_Rewards::
	goto_if_eq VAR_TEMP_5, 2, LeagueTerminal_EventScript_RocketRewards
	msgbox LeagueTerminal_Text_IndigoRewards
	goto LeagueTerminal_EventScript_Menu
LeagueTerminal_EventScript_RocketRewards::
	goto RocketLeague_Lobby_EventScript_ShowRecords

LeagueTerminal_Text_Indigo::
	.string "INDIGO LEAGUE TERMINAL\nSelect a service.$"
LeagueTerminal_Text_Rocket::
	.string "ROCKET LEAGUE BOARD\nB: Back$"
LeagueTerminal_Text_IndigoRules::
	.string "Four different pool opponents,\nthen the CHAMPION.\p"
	.string "DOUBLE BATTLES. Bring your team\nof up to six POKéMON.\p"
	.string "Both teams battle at Level 50.\nOriginal levels return afterward.\p"
	.string "HP, PP and status recover fully\nafter each match.\p"
	.string "Held items are allowed. Neither\nside may use items from the Bag.\p"
	.string "Returning to the lobby ends an\nunfinished run.\pLosing or abandoning a run\nbreaks your title streak.$"
LeagueTerminal_Text_RocketRules::
	.string "Four SINGLE BATTLES, then\nGIOVANNI. Beat his GYM first.\p"
	.string "Normal levels. Items allowed.\nDamage carries between matches.\p"
	.string "Leaving or losing ends your run\nand title streak. Boosts reset.\p"
	.string "Win: 1,000 COINS.\nGIOVANNI: 10,000 COINS.\lMake room in your COIN CASE!$"

LeagueTerminal_Text_IndigoRewards::
	.string "Defeat the CHAMPION to enter\nthe HALL OF FAME.\p"
	.string "Each title adds to your record.\nWin more runs to build a streak!\p"
	.string "Your victories and title streaks\nare kept in the Records menu.$"
