Route23_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route23_OnTransition
	.byte 0

Route23_OnTransition::
	call Route23_EventScript_UpdateRivalVisibility
	clearflag FLAG_HIDE_VICTORY_ROAD_3F_BOULDER
	setflag FLAG_HIDE_VICTORY_ROAD_2F_BOULDER
	setvar VAR_MAP_SCENE_VICTORY_ROAD_1F, 0
	setvar VAR_MAP_SCENE_VICTORY_ROAD_2F_BOULDER1, 0
	setvar VAR_MAP_SCENE_VICTORY_ROAD_2F_BOULDER2, 0
	setvar VAR_MAP_SCENE_VICTORY_ROAD_3F, 0
	end

Route23_EventScript_Wayne::
	trainerbattle_single TRAINER_ROUTE23_HIKER_WAYNE, Route23_Text_WayneIntro, Route23_Text_WayneDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route23_EventScript_WayneRematch
	msgbox Route23_Text_WaynePostBattle, MSGBOX_AUTOCLOSE
	end

Route23_EventScript_WayneRematch::
	trainerbattle_rematch TRAINER_ROUTE23_HIKER_WAYNE, Route23_Text_WayneRematchIntro, Route23_Text_WayneDefeat
	msgbox Route23_Text_WaynePostBattle, MSGBOX_AUTOCLOSE
	end

Route23_EventScript_Chester::
	trainerbattle_single TRAINER_ROUTE23_BIRD_KEEPER_CHESTER, Route23_Text_ChesterIntro, Route23_Text_ChesterDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route23_EventScript_ChesterRematch
	msgbox Route23_Text_ChesterPostBattle, MSGBOX_AUTOCLOSE
	end

Route23_EventScript_ChesterRematch::
	trainerbattle_rematch TRAINER_ROUTE23_BIRD_KEEPER_CHESTER, Route23_Text_ChesterRematchIntro, Route23_Text_ChesterDefeat
	msgbox Route23_Text_ChesterPostBattle, MSGBOX_AUTOCLOSE
	end

Route23_EventScript_Ernest::
	trainerbattle_single TRAINER_ROUTE23_BIKER_ERNEST, Route23_Text_ErnestIntro, Route23_Text_ErnestDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route23_EventScript_ErnestRematch
	msgbox Route23_Text_ErnestPostBattle, MSGBOX_AUTOCLOSE
	end

Route23_EventScript_ErnestRematch::
	trainerbattle_rematch TRAINER_ROUTE23_BIKER_ERNEST, Route23_Text_ErnestRematchIntro, Route23_Text_ErnestDefeat
	msgbox Route23_Text_ErnestPostBattle, MSGBOX_AUTOCLOSE
	end

Route23_EventScript_Zeek::
	trainerbattle_single TRAINER_ROUTE23_BIKER_ZEEK, Route23_Text_ZeekIntro, Route23_Text_ZeekDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route23_EventScript_ZeekRematch
	msgbox Route23_Text_ZeekPostBattle, MSGBOX_AUTOCLOSE
	end

Route23_EventScript_ZeekRematch::
	trainerbattle_rematch TRAINER_ROUTE23_BIKER_ZEEK, Route23_Text_ZeekRematchIntro, Route23_Text_ZeekDefeat
	msgbox Route23_Text_ZeekPostBattle, MSGBOX_AUTOCLOSE
	end

Route23_EventScript_Corey::
	trainerbattle_single TRAINER_ROUTE23_CUE_BALL_COREY, Route23_Text_CoreyIntro, Route23_Text_CoreyDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route23_EventScript_CoreyRematch
	msgbox Route23_Text_CoreyPostBattle, MSGBOX_AUTOCLOSE
	end

Route23_EventScript_CoreyRematch::
	trainerbattle_rematch TRAINER_ROUTE23_CUE_BALL_COREY, Route23_Text_CoreyRematchIntro, Route23_Text_CoreyDefeat
	msgbox Route23_Text_CoreyPostBattle, MSGBOX_AUTOCLOSE
	end

Route23_EventScript_Malik::
	trainerbattle_single TRAINER_ROUTE23_BIKER_MALIK, Route23_Text_MalikIntro, Route23_Text_MalikDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route23_EventScript_MalikRematch
	msgbox Route23_Text_MalikPostBattle, MSGBOX_AUTOCLOSE
	end

Route23_EventScript_MalikRematch::
	trainerbattle_rematch TRAINER_ROUTE23_BIKER_MALIK, Route23_Text_MalikRematchIntro, Route23_Text_MalikDefeat
	msgbox Route23_Text_MalikPostBattle, MSGBOX_AUTOCLOSE
	end

Route23_EventScript_CascadeBadgeGuard::
	lock
	faceplayer
	setvar VAR_TEMP_1, 2
	bufferstdstring STR_VAR_1, STDSTRING_CASCADE_BADGE
	goto Route23_EventScript_BadgeGuard
	end

Route23_EventScript_ThunderBadgeGuard::
	lock
	faceplayer
	setvar VAR_TEMP_1, 3
	bufferstdstring STR_VAR_1, STDSTRING_THUNDER_BADGE
	goto Route23_EventScript_BadgeGuard
	end

Route23_EventScript_RainbowBadgeGuard::
	lock
	faceplayer
	setvar VAR_TEMP_1, 4
	bufferstdstring STR_VAR_1, STDSTRING_RAINBOW_BADGE
	goto Route23_EventScript_BadgeGuard
	end

Route23_EventScript_SoulBadgeGuard::
	lock
	faceplayer
	setvar VAR_TEMP_1, 5
	bufferstdstring STR_VAR_1, STDSTRING_SOUL_BADGE
	goto Route23_EventScript_BadgeGuard
	end

Route23_EventScript_MarshBadgeGuard::
	lock
	faceplayer
	setvar VAR_TEMP_1, 6
	bufferstdstring STR_VAR_1, STDSTRING_MARSH_BADGE
	goto Route23_EventScript_BadgeGuard
	end

Route23_EventScript_VolcanoBadgeGuard::
	lock
	faceplayer
	setvar VAR_TEMP_1, 7
	bufferstdstring STR_VAR_1, STDSTRING_VOLCANO_BADGE
	goto Route23_EventScript_BadgeGuard
	end

Route23_EventScript_EarthBadgeGuard::
	lock
	faceplayer
	setvar VAR_TEMP_1, 8
	bufferstdstring STR_VAR_1, STDSTRING_EARTH_BADGE
	goto Route23_EventScript_BadgeGuard
	end

Route23_EventScript_CascadeBadgeGuardTrigger::
	lockall
	setvar VAR_TEMP_1, 2
	setvar VAR_0x8009, LOCALID_CASCADE_BADGE_GUARD
	bufferstdstring STR_VAR_1, STDSTRING_CASCADE_BADGE
	goto Route23_EventScript_BadgeGuardTrigger
	end

Route23_EventScript_ThunderBadgeGuardTrigger::
	lockall
	setvar VAR_TEMP_1, 3
	setvar VAR_0x8009, LOCALID_THUNDER_BADGE_GUARD
	bufferstdstring STR_VAR_1, STDSTRING_THUNDER_BADGE
	goto Route23_EventScript_BadgeGuardTrigger
	end

Route23_EventScript_RainbowBadgeGuardTrigger::
	lockall
	setvar VAR_TEMP_1, 4
	setvar VAR_0x8009, LOCALID_RAINBOW_BADGE_GUARD
	bufferstdstring STR_VAR_1, STDSTRING_RAINBOW_BADGE
	goto Route23_EventScript_BadgeGuardTrigger
	end

Route23_EventScript_SoulBadgeGuardTrigger::
	lockall
	setvar VAR_TEMP_1, 5
	setvar VAR_0x8009, LOCALID_SOUL_BADGE_GUARD
	bufferstdstring STR_VAR_1, STDSTRING_SOUL_BADGE
	goto Route23_EventScript_BadgeGuardTrigger
	end

Route23_EventScript_MarshBadgeGuardTrigger::
	lockall
	setvar VAR_TEMP_1, 6
	setvar VAR_0x8009, LOCALID_MARSH_BADGE_GUARD
	bufferstdstring STR_VAR_1, STDSTRING_MARSH_BADGE
	goto Route23_EventScript_BadgeGuardTrigger
	end

Route23_EventScript_VolcanoBadgeGuardTrigger::
	lockall
	setvar VAR_TEMP_1, 7
	setvar VAR_0x8009, LOCALID_VOLCANO_BADGE_GUARD
	bufferstdstring STR_VAR_1, STDSTRING_VOLCANO_BADGE
	goto Route23_EventScript_BadgeGuardTrigger
	end

Route23_EventScript_EarthBadgeGuardTrigger::
	lockall
	setvar VAR_TEMP_1, 8
	setvar VAR_0x8009, LOCALID_EARTH_BADGE_GUARD
	bufferstdstring STR_VAR_1, STDSTRING_EARTH_BADGE
	goto Route23_EventScript_BadgeGuardTrigger
	end

Route23_EventScript_VictoryRoadGateSign::
	msgbox Route23_Text_VictoryRoadGateSign, MSGBOX_SIGN
	end

Route23_EventScript_EvanZoeEvan::
	trainerbattle_double TRAINER_ACES_EVAN_ZOE, Route23_Text_EvanZoeIntro, Route23_Text_EvanZoeDefeat, Route23_Text_EvanZoeNotEnoughMons
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route23_EventScript_EvanZoeEvanRematch
	msgbox Route23_Text_EvanZoePostBattle, MSGBOX_AUTOCLOSE
	end

Route23_EventScript_EvanZoeEvanRematch::
	trainerbattle_rematch_double TRAINER_ACES_EVAN_ZOE, Route23_Text_EvanZoeIntro, Route23_Text_EvanZoeDefeat, Route23_Text_EvanZoeNotEnoughMons
	msgbox Route23_Text_EvanZoePostBattle, MSGBOX_AUTOCLOSE
	end

Route23_EventScript_EvanZoeZoe::
	trainerbattle_double TRAINER_ACES_EVAN_ZOE, Route23_Text_EvanZoeIntro, Route23_Text_EvanZoeDefeat, Route23_Text_EvanZoeNotEnoughMons
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route23_EventScript_EvanZoeZoeRematch
	msgbox Route23_Text_EvanZoePostBattle, MSGBOX_AUTOCLOSE
	end

Route23_EventScript_EvanZoeZoeRematch::
	trainerbattle_rematch_double TRAINER_ACES_EVAN_ZOE, Route23_Text_EvanZoeIntro, Route23_Text_EvanZoeDefeat, Route23_Text_EvanZoeNotEnoughMons
	msgbox Route23_Text_EvanZoePostBattle, MSGBOX_AUTOCLOSE
	end

Route23_Text_EvanZoeIntro::
	.string "We are training for the LEAGUE.\nCan you handle both of us?$"

Route23_Text_EvanZoeDefeat::
	.string "You make a great team!$"

Route23_Text_EvanZoePostBattle::
	.string "A good partner spots things\nyou might miss on your own.$"

Route23_Text_EvanZoeNotEnoughMons::
	.string "Bring two ready POKéMON\nso we can battle as a pair!$"

Route23_EventScript_AceTrainerFNora::
	trainerbattle_single TRAINER_ACE_TRAINER_NORA, Route23_Text_AceTrainerFNoraIntro, Route23_Text_AceTrainerFNoraDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route23_EventScript_AceTrainerFNoraRematch
	msgbox Route23_Text_AceTrainerFNoraPostBattle, MSGBOX_AUTOCLOSE
	end

Route23_EventScript_AceTrainerFNoraRematch::
	trainerbattle_rematch TRAINER_ACE_TRAINER_NORA, Route23_Text_AceTrainerFNoraIntro, Route23_Text_AceTrainerFNoraDefeat
	msgbox Route23_Text_AceTrainerFNoraPostBattle, MSGBOX_AUTOCLOSE
	end

Route23_Text_AceTrainerFNoraIntro::
	.string "The LEAGUE is our goal.\nEvery battle brings us closer!$"

Route23_Text_AceTrainerFNoraDefeat::
	.string "A lesson worth learning!$"

Route23_Text_AceTrainerFNoraPostBattle::
	.string "I keep track of every loss.\nThere is always more to learn.$"

Route23_EventScript_AceTrainerFVera::
	trainerbattle_single TRAINER_ACE_TRAINER_VERA, Route23_Text_AceTrainerFVeraIntro, Route23_Text_AceTrainerFVeraDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route23_EventScript_AceTrainerFVeraRematch
	msgbox Route23_Text_AceTrainerFVeraPostBattle, MSGBOX_AUTOCLOSE
	end

Route23_EventScript_AceTrainerFVeraRematch::
	trainerbattle_rematch TRAINER_ACE_TRAINER_VERA, Route23_Text_AceTrainerFVeraIntro, Route23_Text_AceTrainerFVeraDefeat
	msgbox Route23_Text_AceTrainerFVeraPostBattle, MSGBOX_AUTOCLOSE
	end

Route23_Text_AceTrainerFVeraIntro::
	.string "A strong team needs variety.\nLet me show you what I mean!$"

Route23_Text_AceTrainerFVeraDefeat::
	.string "You found a way through!$"

Route23_Text_AceTrainerFVeraPostBattle::
	.string "Different strengths make\na team worth relying on.$"

Route23_EventScript_AceTrainerFLena::
	trainerbattle_single TRAINER_ACE_TRAINER_LENA, Route23_Text_AceTrainerFLenaIntro, Route23_Text_AceTrainerFLenaDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route23_EventScript_AceTrainerFLenaRematch
	msgbox Route23_Text_AceTrainerFLenaPostBattle, MSGBOX_AUTOCLOSE
	end

Route23_EventScript_AceTrainerFLenaRematch::
	trainerbattle_rematch TRAINER_ACE_TRAINER_LENA, Route23_Text_AceTrainerFLenaIntro, Route23_Text_AceTrainerFLenaDefeat
	msgbox Route23_Text_AceTrainerFLenaPostBattle, MSGBOX_AUTOCLOSE
	end

Route23_Text_AceTrainerFLenaIntro::
	.string "I train for tough opponents.\nYou look like a good test!$"

Route23_Text_AceTrainerFLenaDefeat::
	.string "That was a proper challenge!$"

Route23_Text_AceTrainerFLenaPostBattle::
	.string "A challenge shows you where\nyour training needs more work.$"

Route23_EventScript_UpdateRivalVisibility::
	goto_if_lt VAR_MAP_SCENE_CELADON_RIVAL, 1, Route23_EventScript_HidePendingRival
	goto_if_ge VAR_ROUTE23_RIVAL_CONVERSATION, 1, Route23_EventScript_HidePendingRival
	clearflag FLAG_HIDE_ROUTE23_RIVAL_CONVERSATION
	addobject 19
	return

Route23_EventScript_HidePendingRival::
	removeobject 19
	setflag FLAG_HIDE_ROUTE23_RIVAL_CONVERSATION
	return

Route23_EventScript_RivalConversation::
	goto_if_ge VAR_ROUTE23_RIVAL_CONVERSATION, 1, EventScript_Return
	goto_if_lt VAR_MAP_SCENE_CELADON_RIVAL, 1, EventScript_Return
	lockall
	setvar VAR_LAST_TALKED, 19
	faceplayer
	msgbox Route23_Text_RivalConversation
	call_if_eq VAR_STARTER_MON, 2, Route23_EventScript_RivalSquirtle
	call_if_eq VAR_STARTER_MON, 1, Route23_EventScript_RivalBulbasaur
	call_if_eq VAR_STARTER_MON, 0, Route23_EventScript_RivalCharmander
	msgbox Route23_Text_RivalPostBattle
	closemessage
	fadescreen FADE_TO_BLACK
	setflag FLAG_HIDE_ROUTE23_RIVAL_CONVERSATION
	setvar VAR_ROUTE23_RIVAL_CONVERSATION, 1
	removeobject 19
	fadescreen FADE_FROM_BLACK
	releaseall
	end

Route23_EventScript_RivalSquirtle::
	trainerbattle_no_intro TRAINER_RIVAL_ROUTE23_SQUIRTLE, Route23_Text_RivalDefeat
	return

Route23_EventScript_RivalBulbasaur::
	trainerbattle_no_intro TRAINER_RIVAL_ROUTE23_BULBASAUR, Route23_Text_RivalDefeat
	return

Route23_EventScript_RivalCharmander::
	trainerbattle_no_intro TRAINER_RIVAL_ROUTE23_CHARMANDER, Route23_Text_RivalDefeat
	return

Route23_Text_RivalConversation::
	.string "{RIVAL}: All eight BADGES, huh?\n"
	.string "Let me see those!\p"
	.string "Every GYM taught us something.\n"
	.string "Sometimes after a few losses!\p"
	.string "I used to just shout orders.\n"
	.string "Now I pay attention to my team.\p"
	.string "But you know I'm still aiming\n"
	.string "for the top, {PLAYER}!\p"
	.string "Let's see what you and your\n"
	.string "POKéMON learned. Here we go!$"

Route23_Text_RivalDefeat::
	.string "You've earned those BADGES!$"

Route23_Text_RivalPostBattle::
	.string "{RIVAL}: You trusted your team\n"
	.string "to pull that off. I could tell.\p"
	.string "We've come a long way since\n"
	.string "that first battle at the lab.\p"
	.string "Get through VICTORY ROAD.\n"
	.string "I'll see you at the LEAGUE!$"
