PalletTown_PlayersHouse_2F_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, PalletTown_PlayersHouse_2F_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, PalletTown_PlayersHouse_2F_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, PalletTown_PlayersHouse_2F_OnWarp
	.byte 0

PalletTown_PlayersHouse_2F_OnTransition::
	setrespawn HEAL_LOCATION_PALLET_TOWN
	end

PalletTown_PlayersHouse_2F_OnWarp::
	map_script_2 VAR_MAP_SCENE_PALLET_TOWN_PLAYERS_HOUSE_2F, 0, PalletTown_PlayersHouse_2F_FirstWarpIn
	.2byte 0

PalletTown_PlayersHouse_2F_FirstWarpIn::
	turnobject LOCALID_PLAYER, DIR_NORTH
	setflag FLAG_SYS_B_DASH
	setflag FLAG_SYS_LOGBOOK_MENU_GET
	setflag FLAG_HIDE_PEWTER_CITY_GYM_BROCK
	setvar VAR_MAP_SCENE_PALLET_TOWN_PLAYERS_HOUSE_2F, 1
	end

PalletTown_PlayersHouse_2F_EventScript_NES::
	msgbox PalletTown_PlayersHouse_2F_Text_PlayedWithNES, MSGBOX_SIGN
	end

PalletTown_PlayersHouse_2F_EventScript_PC::
	lockall
	setvar VAR_0x8004, 1
	special AnimatePcTurnOn
	playse SE_PC_ON
	msgbox Text_PlayerBootedUpPC
	special BedroomPC
	waitstate
	releaseall
	end

EventScript_PalletTown_PlayersHouse_2F_ShutDownPC::
	setvar VAR_0x8004, 1
	playse SE_PC_OFF
	special AnimatePcTurnOff
	releaseall
	end

PalletTown_PlayersHouse_2F_OnLoad::
	goto_if_unset FLAG_OAK_EXPEDITION_COMPLETE, PalletTown_PlayersHouse_2F_CertificateLoadDone
	setmetatile 11, 0, METATILE_Building_CertificateTop, 1
	setmetatile 11, 1, METATILE_Building_CertificateBottom, 1
PalletTown_PlayersHouse_2F_CertificateLoadDone::
	end

PalletTown_PlayersHouse_2F_EventScript_Certificate::
	goto_if_unset FLAG_OAK_EXPEDITION_COMPLETE, EventScript_Return
	lockall
	msgbox PalletTown_PlayersHouse_2F_Text_Certificate
	closemessage
	special ShowExpeditionCertificate
	waitstate
	releaseall
	end
