Route24_MapScripts::
	.byte 0

Route24_EventScript_RewardGiver::
	lock
	faceplayer
	goto_if_eq VAR_MAP_SCENE_ROUTE24, 1, Route24_EventScript_RewardGiverPostReward
	msgbox Route24_Text_JustEarnedFabulousPrize
	checkitemspace ITEM_NUGGET
	goto_if_eq VAR_RESULT, FALSE, Route24_EventScript_NoRoomForNugget
	call Route24_EventScript_GiveNuggetReward
	release
	end

Route24_EventScript_NoRoomForNugget::
	msgbox Route24_Text_YouDontHaveAnyRoom
	release
	end

Route24_EventScript_RewardGiverPostReward::
	msgbox Route24_Text_YouAlreadyClaimedPrize
	release
	end

Route24_EventScript_RewardGiverTriggerLeft::
	lockall
	setvar VAR_TEMP_1, 0
	goto Route24_EventScript_RewardGiverTrigger
	end

Route24_EventScript_RewardGiverTriggerRight::
	lockall
	setvar VAR_TEMP_1, 1
	goto Route24_EventScript_RewardGiverTrigger
	end

Route24_EventScript_RewardGiverTrigger::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	call_if_eq VAR_TEMP_1, 0, Route24_EventScript_RewardGiverApproachPlayer
	call_if_eq VAR_TEMP_1, 1, Route24_EventScript_RewardGiverMotionToPlayer
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox Route24_Text_JustEarnedFabulousPrize
	checkitemspace ITEM_NUGGET
	goto_if_eq VAR_RESULT, FALSE, Route24_EventScript_NoRoomForNuggetTrigger
	call Route24_EventScript_GiveNuggetReward
	releaseall
	end

Route24_EventScript_GiveNuggetReward::
	giveitem_msg Route24_Text_ReceivedNuggetFromRewardGiver, ITEM_NUGGET
	msgbox Route24_Text_KeepTraining
	setvar VAR_MAP_SCENE_ROUTE24, 1
	return

Route24_EventScript_RewardGiverApproachPlayer::
	applymovement LOCALID_ROUTE24_REWARD_GIVER, Route24_Movement_RewardGiverApproachPlayer
	waitmovement 0
	return

Route24_EventScript_RewardGiverMotionToPlayer::
	applymovement LOCALID_ROUTE24_REWARD_GIVER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

Route24_EventScript_NoRoomForNuggetTrigger::
	msgbox Route24_Text_YouDontHaveAnyRoom
	closemessage
	applymovement LOCALID_PLAYER, Route24_Movement_WalkDown
	waitmovement 0
	call_if_eq VAR_TEMP_1, 0, Route24_EventScript_RewardGiverWalkBackToPos
	release
	end

Route24_EventScript_RewardGiverWalkBackToPos::
	applymovement LOCALID_ROUTE24_REWARD_GIVER, Route24_Movement_RewardGiverWalkBackToPos
	waitmovement 0
	return

Route24_Movement_RewardGiverApproachPlayer::
	walk_left
	step_end

Route24_Movement_RewardGiverWalkBackToPos::
	walk_right
	walk_in_place_faster_left
	step_end

Route24_Movement_WalkDown::
	walk_down
	step_end
