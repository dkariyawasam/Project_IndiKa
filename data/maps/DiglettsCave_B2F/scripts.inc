DiglettsCave_B2F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, DiglettsCave_B2F_OnTransition
	.byte 0

DiglettsCave_B2F_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_DIGLETTS_CAVE_B1F
	end
