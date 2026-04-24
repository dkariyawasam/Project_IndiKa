MtMoon_B1F_MapScripts::
	.byte 0

.include "data/maps/MtMoon_B1F/text.inc"


MtMoon_B1F_EventScript_Brock::
	lock
	faceplayer
	goto_if_ge VAR_BROCK_TRIAL_STATE, 3, MtMoon_BrockGone
	goto_if_lt VAR_BROCK_TRIAL_STATE, 2, MtMoon_BrockFirstMeeting
	goto MtMoon_BrockWaitingForShard

MtMoon_BrockFirstMeeting::
	questmenu QUEST_MENU_UNLOCK_QUEST, QUEST_GYM_LEADER_TRIALS
	questmenu QUEST_MENU_SET_ACTIVE, QUEST_GYM_LEADER_TRIALS
	subquestmenu QUEST_MENU_UNLOCK_QUEST, QUEST_GYM_LEADER_TRIALS, SUB_QUEST_BROCK
	setvar VAR_BROCK_TRIAL_STATE, 2
	msgbox MtMoon_Text_BrockFirstMeeting
	release
	end

MtMoon_BrockWaitingForShard::
	checkitem ITEM_HELIX_FOSSIL_SHARD, 1
	goto_if_eq VAR_RESULT, TRUE, MtMoon_BrockSawHelixShard
	checkitem ITEM_DOME_FOSSIL_SHARD, 1
	goto_if_eq VAR_RESULT, TRUE, MtMoon_BrockSawDomeShard
	msgbox MtMoon_Text_BrockStillSearching
	release
	end

MtMoon_BrockSawHelixShard::
	msgbox MtMoon_Text_BrockSawHelixShard
	giveitem_msg MtMoon_Text_BrockGaveDartShard, ITEM_DART_FOSSIL_SHARD, 1
	setvar VAR_BROCK_TRIAL_STATE, 3
	call MtMoon_BrockLeavesForGym
	release
	end

MtMoon_BrockSawDomeShard::
	msgbox MtMoon_Text_BrockSawDomeShard
	giveitem_msg MtMoon_Text_BrockGaveHasteShard, ITEM_HASTE_FOSSIL_SHARD, 1
	setvar VAR_BROCK_TRIAL_STATE, 3
	call MtMoon_BrockLeavesForGym
	release
	end

MtMoon_BrockLeavesForGym::
	msgbox MtMoon_Text_BrockReturnToGym
	closemessage
	applymovement LOCALID_MTMOON_BROCK, MtMoon_Movement_BrockLeave
	waitmovement 0
	setvar VAR_BROCK_TRIAL_STATE, 3
	clearflag FLAG_HIDE_PEWTER_CITY_GYM_BROCK
	setflag FLAG_HIDE_MT_MOON_BROCK
	removeobject LOCALID_MTMOON_BROCK
	return

MtMoon_Movement_BrockLeave:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

MtMoon_BrockGone::
	release
	end