Route3_MapScripts::
	.byte 0

Route3_EventScript_Youngster::
	msgbox Route3_Text_TunnelFromCeruleanTiring, MSGBOX_NPC
	end

Route3_EventScript_RouteSign::
	msgbox Route3_Text_RouteSign, MSGBOX_SIGN
	end

Route3_EventScript_YoungsterMia::
	trainerbattle_single TRAINER_YOUNGSTER_MIA, Route3_Text_YoungsterMiaIntro, Route3_Text_YoungsterMiaDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route3_EventScript_YoungsterMiaRematch
	msgbox Route3_Text_YoungsterMiaPostBattle, MSGBOX_AUTOCLOSE
	end

Route3_EventScript_YoungsterMiaRematch::
	trainerbattle_rematch TRAINER_YOUNGSTER_MIA, Route3_Text_YoungsterMiaRematchIntro, Route3_Text_YoungsterMiaDefeat
	msgbox Route3_Text_YoungsterMiaPostBattle, MSGBOX_AUTOCLOSE
	end

Route3_Text_YoungsterMiaIntro::
	.string "Josh and I came out to train!\nWant to battle me too?$"

Route3_Text_YoungsterMiaDefeat::
	.string "I need more practice!$"

Route3_Text_YoungsterMiaPostBattle::
	.string "We take turns picking\nwhere to explore next.$"

Route3_Text_YoungsterMiaRematchIntro::
	.string "Josh says I've improved my\ntiming. Help me prove it!$"

Route3_EventScript_RivalConversation::
	goto_if_set FLAG_HIDE_ROUTE3_RIVAL_CONVERSATION, EventScript_Return
	lockall
	playbgm MUS_ENCOUNTER_RIVAL, 0
	applymovement 6, Common_Movement_ExclamationMark
	waitmovement 6
	waitfieldeffect FLDEFF_EXCLAMATION_MARK_ICON
	getplayerxy VAR_0x8004, VAR_0x8005
	call Route3_EventScript_RivalApproach
	setvar VAR_LAST_TALKED, 6
	call Route3_EventScript_PlayerFaceRival
	msgbox Route3_Text_RivalConversation
	closemessage
	playbgm MUS_RIVAL_EXIT, 0
	getplayerxy VAR_0x8004, VAR_0x8005
	call Route3_EventScript_RivalReturnToExitLane
	@ The northern trigger can leave the player directly in his exit lane.
	goto_if_ne VAR_0x8004, 70, Route3_EventScript_RivalWalkNorth
	goto_if_ne VAR_0x8005, 5, Route3_EventScript_RivalWalkNorth
	applymovement 6, Route3_Movement_RivalStepAroundPlayer
	waitmovement 6
Route3_EventScript_RivalWalkNorth::
	applymovement 6, Route3_Movement_RivalExitNorth
	waitmovement 6
	setflag FLAG_HIDE_ROUTE3_RIVAL_CONVERSATION
	setvar VAR_ROUTE3_RIVAL_CONVERSATION, 1
	removeobject 6
	fadedefaultbgm
	releaseall
	end

Route3_Text_RivalConversation::
	.string "{RIVAL}: Hey, {PLAYER}!\n"
	.string "Heading for MT. MOON too?\p"
	.string "I'm taking on the GYMS.\n"
	.string "All eight of them!\p"
	.string "Those LEADERS know their teams\n"
	.string "like they've been together forever.\p"
	.string "My partner and I are still\n"
	.string "figuring each other out.\p"
	.string "We'll learn as we go. Next time,\n"
	.string "I'll show you how far we've come!$"

Route3_Movement_RivalStepAroundPlayer::
	walk_right
	step_end

Route3_Movement_RivalExitNorth::
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

@ Stay on row 6 and stop adjacent to the player on either trigger strip.
@ Direct interaction from either side already starts next to the player.
Route3_EventScript_RivalApproach::
	goto_if_eq VAR_0x8005, 6, Route3_EventScript_RivalMovementReturn
	goto_if_eq VAR_0x8004, 68, Route3_EventScript_RivalApproachLeftTwo
	goto_if_eq VAR_0x8004, 69, Route3_EventScript_RivalApproachLeft
	goto_if_eq VAR_0x8004, 71, Route3_EventScript_RivalApproachRight
	goto_if_eq VAR_0x8004, 72, Route3_EventScript_RivalApproachRightTwo
	return

Route3_EventScript_RivalReturnToExitLane::
	goto_if_eq VAR_0x8005, 6, Route3_EventScript_RivalMovementReturn
	goto_if_eq VAR_0x8004, 68, Route3_EventScript_RivalReturnRightTwo
	goto_if_eq VAR_0x8004, 69, Route3_EventScript_RivalReturnRight
	goto_if_eq VAR_0x8004, 71, Route3_EventScript_RivalReturnLeft
	goto_if_eq VAR_0x8004, 72, Route3_EventScript_RivalReturnLeftTwo
Route3_EventScript_RivalMovementReturn::
	return

@ Explicitly face both actors; lockall does not select a talking object.
Route3_EventScript_PlayerFaceRival::
	goto_if_eq VAR_0x8005, 5, Route3_EventScript_PlayerFaceDown
	goto_if_eq VAR_0x8005, 6, Route3_EventScript_PlayerFaceSide
	applymovement LOCALID_PLAYER, Route3_Movement_PlayerFaceUp
	applymovement 6, Common_Movement_FaceDown
	goto Route3_EventScript_WaitForFacing

Route3_EventScript_PlayerFaceDown::
	applymovement LOCALID_PLAYER, Common_Movement_FaceDown
	applymovement 6, Route3_Movement_PlayerFaceUp
	goto Route3_EventScript_WaitForFacing

Route3_EventScript_PlayerFaceSide::
	goto_if_eq VAR_0x8004, 69, Route3_EventScript_PlayerFaceRight
	applymovement LOCALID_PLAYER, Route3_Movement_FaceLeft
	applymovement 6, Common_Movement_FaceRight
	goto Route3_EventScript_WaitForFacing

Route3_EventScript_PlayerFaceRight::
	applymovement LOCALID_PLAYER, Common_Movement_FaceRight
	applymovement 6, Route3_Movement_FaceLeft
Route3_EventScript_WaitForFacing::
	waitmovement LOCALID_PLAYER
	waitmovement 6
	return

Route3_EventScript_RivalApproachLeftTwo::
	applymovement 6, Route3_Movement_RivalLeftTwo
	waitmovement 6
	return

Route3_EventScript_RivalReturnLeftTwo::
	applymovement 6, Route3_Movement_RivalLeftTwo
	waitmovement 6
	return

Route3_Movement_RivalLeftTwo::
	walk_left
	walk_left
	step_end

Route3_EventScript_RivalApproachLeft::
	applymovement 6, Route3_Movement_RivalLeft
	waitmovement 6
	return

Route3_EventScript_RivalReturnLeft::
	applymovement 6, Route3_Movement_RivalLeft
	waitmovement 6
	return

Route3_Movement_RivalLeft::
	walk_left
	step_end

Route3_EventScript_RivalApproachRight::
	applymovement 6, Route3_Movement_RivalRight
	waitmovement 6
	return

Route3_EventScript_RivalReturnRight::
	applymovement 6, Route3_Movement_RivalRight
	waitmovement 6
	return

Route3_Movement_RivalRight::
	walk_right
	step_end

Route3_EventScript_RivalApproachRightTwo::
	applymovement 6, Route3_Movement_RivalRightTwo
	waitmovement 6
	return

Route3_EventScript_RivalReturnRightTwo::
	applymovement 6, Route3_Movement_RivalRightTwo
	waitmovement 6
	return

Route3_Movement_RivalRightTwo::
	walk_right
	walk_right
	step_end

Route3_Movement_PlayerFaceUp::
	face_up
	step_end

Route3_Movement_FaceLeft::
	face_left
	step_end
