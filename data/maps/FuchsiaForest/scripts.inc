FuchsiaForest_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, FuchsiaForest_OnTransition
	.byte 0

FuchsiaForest_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_FUCHSIA_FOREST
	setescapewarp MAP_THREE_ISLAND_BOND_BRIDGE, 12, 6
	end

FuchsiaForest_EventScript_Lostelle::
	lock
	faceplayer
	msgbox FuchsiaForest_Text_HelpScaryPokemon
	playse SE_PIN
	applymovement LOCALID_FUCHSIA_FOREST_LOSTELLE, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_FUCHSIA_FOREST_LOSTELLE, FuchsiaForest_Movement_LostelleLookAround
	waitmovement 0
	msgbox FuchsiaForest_Text_HereItComesAgain
	goto_if_questlog EventScript_ReleaseEnd
	waitse
	playmoncry SPECIES_HYPNO, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_HYPNO, 30
	dowildbattle
	special QuestLog_CutRecording
	applymovement LOCALID_FUCHSIA_FOREST_LOSTELLE, Common_Movement_FacePlayer
	waitmovement 0
	msgbox FuchsiaForest_Text_ThankYouHaveThis
	giveitem ITEM_IAPAPA_BERRY
	call_if_eq VAR_RESULT, FALSE, FuchsiaForest_EventScript_NoRoomForBerry
	special BufferBigGuyOrBigGirlString
	msgbox FuchsiaForest_Text_LetsGoHome
	closemessage
	setflag FLAG_RESCUED_LOSTELLE
	setflag FLAG_HIDE_LOSTELLE_IN_FUCHSIA_FOREST
	setvar VAR_MAP_SCENE_TWO_ISLAND_JOYFUL_GAME_CORNER, 2
	clearflag FLAG_HIDE_TWO_ISLAND_GAME_CORNER_LOSTELLE
	warp MAP_TWO_ISLAND_JOYFUL_GAME_CORNER, 6, 6
	waitstate
	release
	end

FuchsiaForest_EventScript_NoRoomForBerry::
	msgbox FuchsiaForest_Text_BerryPouchIsFull
	return

FuchsiaForest_Movement_LostelleLookAround::
	walk_in_place_faster_left
	delay_16
	walk_in_place_faster_right
	delay_8
	walk_in_place_faster_left
	step_end

FuchsiaForest_EventScript_WelcomeSign::
	msgbox FuchsiaForest_Text_WelcomeToFuchsiaForest, MSGBOX_SIGN
	end

FuchsiaForest_EventScript_BewareSign::
	msgbox FuchsiaForest_Text_BewareWildBerryLovingMons, MSGBOX_SIGN
	end
