@ Compatibility redirect for saves made before the three-floor layout.
SeafoamIslands_B3F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SeafoamIslands_B3F_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, SeafoamIslands_B3F_OnFrame
	.byte 0

SeafoamIslands_B3F_OnTransition::
	setvar VAR_TEMP_0, 0
	end

SeafoamIslands_B3F_OnFrame::
	map_script_2 VAR_TEMP_0, 0, SeafoamIslands_B3F_Redirect
	.2byte 0

SeafoamIslands_B3F_Redirect::
	lockall
	setvar VAR_TEMP_0, 1
	warp MAP_SEAFOAM_ISLANDS_1F, 6, 20
	waitstate
	releaseall
	end
