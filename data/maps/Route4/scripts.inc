Route4_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Route4_OnFrame
	map_script MAP_SCRIPT_ON_TRANSITION, Route4_OnTransition
	.byte 0

Route4_OnFrame::
	map_script_2 VAR_TEMP_0, 1, Route4_EventScript_WalkInFromCeladonCave
	.2byte 0

Route4_OnTransition::
	setvar VAR_TEMP_0, 0
	getplayerxy VAR_0x8004, VAR_0x8005
	compare VAR_0x8004, 93
	goto_if_ne Route4_EventScript_EndTransition
	compare VAR_0x8005, 19
	goto_if_ne Route4_EventScript_EndTransition
	setvar VAR_TEMP_0, 1

Route4_EventScript_EndTransition::
	end

Route4_EventScript_WalkInFromCeladonCave::
	setvar VAR_TEMP_0, 0
	lockall
	applymovement LOCALID_PLAYER, Route4_Movement_PlayerWalkInFromCeladonCave
	waitmovement 0
	releaseall
	end

Route4_EventScript_WarpToCeladonCave::
	warp MAP_CELADON_CAVE, 9, 1
	waitstate
	end

Route4_EventScript_Unused::
	end

Route4_EventScript_Woman::
	msgbox Route4_Text_TrippedOverGeodude, MSGBOX_NPC
	setvar VAR_0x8004, SUB_QUEST_APEX_ANNIHILAPE
	setvar VAR_0x8005, 2
	call EventScript_RecordApexRumorWithFeedback
	end

Route4_EventScript_MtMoonSign::
	msgbox Route4_Text_MtMoonEntrance, MSGBOX_SIGN
	end

Route4_EventScript_RouteSign::
	msgbox Route4_Text_RouteSign, MSGBOX_SIGN
	end

Route4_Movement_PlayerWalkInFromCeladonCave::
	face_up
	walk_up
	face_up
	step_end

Route4_EventScript_YoungsterElla::
	trainerbattle_single TRAINER_YOUNGSTER_ELLA, Route4_Text_YoungsterEllaIntro, Route4_Text_YoungsterEllaDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route4_EventScript_YoungsterEllaRematch
	msgbox Route4_Text_YoungsterEllaPostBattle, MSGBOX_AUTOCLOSE
	end

Route4_EventScript_YoungsterEllaRematch::
	trainerbattle_rematch TRAINER_YOUNGSTER_ELLA, Route4_Text_YoungsterEllaRematchIntro, Route4_Text_YoungsterEllaDefeat
	msgbox Route4_Text_YoungsterEllaPostBattle, MSGBOX_AUTOCLOSE
	end

Route4_Text_YoungsterEllaIntro::
	.string "Timmy spotted you first,\nbut I want a battle too!$"

Route4_Text_YoungsterEllaDefeat::
	.string "You caught me off guard!$"

Route4_Text_YoungsterEllaPostBattle::
	.string "My POKéMON and I are getting\nbraver with every battle.$"

Route4_Text_YoungsterEllaRematchIntro::
	.string "Ready for another battle?$"
