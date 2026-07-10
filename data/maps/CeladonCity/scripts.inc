CeladonCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CeladonCity_OnTransition
	.byte 0

CeladonCity_OnTransition::
	special EndBikeRental
	setworldmapflag FLAG_WORLD_MAP_CELADON_CITY
	goto_if_set FLAG_HIDE_CELADON_RIVAL_EVOLUTION, CeladonCity_EventScript_RemoveRival
	goto_if_unset FLAG_SYS_POKEDEX_GET, CeladonCity_EventScript_RemoveRival
	end

CeladonCity_EventScript_RemoveRival::
	removeobject LOCALID_CELADON_RIVAL
	end

CeladonCity_EventScript_RivalEvolutionTrigger::
	goto_if_set FLAG_HIDE_CELADON_RIVAL_EVOLUTION, EventScript_Return
	goto_if_unset FLAG_SYS_POKEDEX_GET, EventScript_Return
	lockall
	playbgm MUS_ENCOUNTER_RIVAL, 0
	applymovement LOCALID_CELADON_RIVAL, Common_Movement_ExclamationMark
	waitmovement 0
	waitfieldeffect FLDEFF_EXCLAMATION_MARK_ICON
	applymovement LOCALID_CELADON_RIVAL, Common_Movement_FaceDown
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	call CeladonCity_EventScript_RivalEvolutionScene
	releaseall
	end

CeladonCity_EventScript_RivalEvolution::
	goto_if_set FLAG_HIDE_CELADON_RIVAL_EVOLUTION, EventScript_Return
	goto_if_unset FLAG_SYS_POKEDEX_GET, EventScript_Return
	lock
	faceplayer
	call CeladonCity_EventScript_RivalEvolutionScene
	release
	end

CeladonCity_EventScript_RivalEvolutionScene::
	msgbox CeladonCity_Text_RivalEvolutionIntro
	setvar VAR_LAST_TALKED, LOCALID_CELADON_RIVAL
	call_if_eq VAR_STARTER_MON, 2, CeladonCity_EventScript_RivalSquirtle
	call_if_eq VAR_STARTER_MON, 1, CeladonCity_EventScript_RivalBulbasaur
	call_if_eq VAR_STARTER_MON, 0, CeladonCity_EventScript_RivalCharmander
	msgbox CeladonCity_Text_RivalEvolutionPostBattle
	closemessage
	playbgm MUS_RIVAL_EXIT, 0
	applymovement LOCALID_CELADON_RIVAL, CeladonCity_Movement_RivalExitEvolution
	applymovement LOCALID_PLAYER, CeladonCity_Movement_PlayerWatchRivalExitEvolution
	waitmovement 0
	setflag FLAG_HIDE_CELADON_RIVAL_EVOLUTION
	setvar VAR_MAP_SCENE_CELADON_RIVAL, 1
	setvar VAR_MAP_SCENE_CERULEAN_CITY_RIVAL, 1
	removeobject LOCALID_CELADON_RIVAL
	fadedefaultbgm
	return

CeladonCity_EventScript_RivalSquirtle::
	trainerbattle_no_intro TRAINER_RIVAL_CERULEAN_SQUIRTLE, CeladonCity_Text_RivalDefeat
	return

CeladonCity_EventScript_RivalBulbasaur::
	trainerbattle_no_intro TRAINER_RIVAL_CERULEAN_BULBASAUR, CeladonCity_Text_RivalDefeat
	return

CeladonCity_EventScript_RivalCharmander::
	trainerbattle_no_intro TRAINER_RIVAL_CERULEAN_CHARMANDER, CeladonCity_Text_RivalDefeat
	return

CeladonCity_Movement_RivalExitEvolution::
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

CeladonCity_Movement_PlayerWatchRivalExitEvolution::
	face_left
	delay_16
	delay_16
	delay_16
	delay_16
	step_end

CeladonCity_EventScript_BerryPatchDoor::
	lockall
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	checkitem ITEM_RAINBOW_KEY
	goto_if_eq VAR_RESULT, TRUE, CeladonCity_EventScript_BerryPatchAccessGranted
	msgbox CeladonCity_Text_BerryPatchAccessDenied
	closemessage
	applymovement LOCALID_PLAYER, CeladonCity_Movement_ForcePlayerFromBerryPatchDoor
	waitmovement 0
	releaseall
	end

CeladonCity_EventScript_BerryPatchAccessGranted::
	msgbox CeladonCity_Text_BerryPatchAccessGranted
	closemessage
	warp MAP_CELADON_CITY_BERRY_PATCH, 0
	waitstate
	releaseall
	end

CeladonCity_Movement_ForcePlayerFromBerryPatchDoor::
	walk_down
	step_end

CeladonCity_EventScript_LittleGirl::
	msgbox CeladonCity_Text_GotMyKoffingInCinnabar, MSGBOX_NPC
	end

CeladonCity_EventScript_OldMan1::
	lock
	msgbox CeladonCity_Text_GymIsGreatFullOfWomen
	release
	end

CeladonCity_EventScript_Woman::
	msgbox CeladonCity_Text_GameCornerIsBadForCitysImage, MSGBOX_NPC
	end

CeladonCity_EventScript_OldMan2::
	msgbox CeladonCity_Text_BlewItAllAtSlots, MSGBOX_NPC
	end

CeladonCity_EventScript_SoftboiledTutor::
	goto EventScript_SoftboiledTutor
	end

CeladonCity_EventScript_FatMan::
	lock
	faceplayer
	msgbox CeladonCity_Text_MyTrustedPalPoliwrath
	closemessage
	applymovement LOCALID_CELADON_FAT_MAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

CeladonCity_EventScript_Poliwrath::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_POLIWRATH, CRY_MODE_NORMAL
	msgbox CeladonCity_Text_Poliwrath
	closemessage
	waitmoncry
	applymovement LOCALID_CELADON_POLIWRATH, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

CeladonCity_EventScript_Boy::
	msgbox CeladonCity_Text_ScaldedTongueOnTea, MSGBOX_NPC
	end

CeladonCity_EventScript_SilphCoScientist::
	msgbox CeladonCity_Text_SomeoneStoleSilphScope, MSGBOX_NPC
	end

CeladonCity_EventScript_CitySign::
	msgbox CeladonCity_Text_CitySign, MSGBOX_SIGN
	end

CeladonCity_EventScript_GymSign::
	lockall
	apexlog APEX_LOG_PERSON_ERIKA, 0
	msgbox CeladonCity_Text_GymSign
	releaseall
	end

CeladonCity_EventScript_MansionSign::
	msgbox CeladonCity_Text_MansionSign, MSGBOX_SIGN
	end

CeladonCity_EventScript_DeptStoreSign::
	msgbox CeladonCity_Text_DeptStoreSign, MSGBOX_SIGN
	end

CeladonCity_EventScript_PrizeExchangeSign::
	msgbox CeladonCity_Text_PrizeExchangeSign, MSGBOX_SIGN
	end

CeladonCity_EventScript_GameCornerSign::
	msgbox CeladonCity_Text_GameCornerSign, MSGBOX_SIGN
	end
