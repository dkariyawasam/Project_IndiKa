Route23_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route23_OnTransition
	.byte 0

Route23_OnTransition::
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
