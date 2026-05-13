Route21_North_MapScripts::
	.byte 0

Route21_North_EventScript_SeagallopSailor::
	lock
	faceplayer
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Route21_North_Text_SeagallopSailorGreeting, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route21_North_EventScript_SailToCinnabar
	release
	end

Route21_North_EventScript_SailToCinnabar::
	msgbox Route21_North_Text_SeagallopSailorAllAboard
	setflag FLAG_HIDE_ROUTE21_NORTH_SEAGALLOP
	setflag FLAG_HIDE_ROUTE21_NORTH_SAILOR
	clearflag FLAG_HIDE_ROUTE21_SOUTH_SEAGALLOP
	clearflag FLAG_HIDE_ROUTE21_SOUTH_SAILOR
	setvar VAR_MAP_SCENE_ROUTE21_SOUTH, 1
	setvar VAR_0x8004, SEAGALLOP_ROUTE21_NORTH
	setvar VAR_0x8006, SEAGALLOP_ROUTE21_SOUTH
	goto EventScript_SetSail
	end
