FuchsiaForest_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, FuchsiaForest_OnTransition
	.byte 0

FuchsiaForest_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_FUCHSIA_FOREST
	setescapewarp MAP_ROUTE15, 44, 19
	end

FuchsiaForest_EventScript_WelcomeSign::
	msgbox FuchsiaForest_Text_WelcomeToFuchsiaForest, MSGBOX_SIGN
	end

FuchsiaForest_EventScript_BewareSign::
	msgbox FuchsiaForest_Text_BewareWildBerryLovingMons, MSGBOX_SIGN
	end
