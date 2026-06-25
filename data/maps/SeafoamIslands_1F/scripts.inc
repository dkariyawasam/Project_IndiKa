SeafoamIslands_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SeafoamIslands_1F_OnTransition
	.byte 0

SeafoamIslands_1F_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_SEAFOAM_ISLANDS_1F
	end

SeafoamIslands_1F_EventScript_LavaRumorNpc::
	lock
	faceplayer
	msgbox SeafoamIslands_1F_Text_UsedToBeFloodedWithLava
	setvar VAR_0x8004, SUB_QUEST_APEX_ARTICUNO
	setvar VAR_0x8005, 2
	special RecordApexRumor
	release
	end

SeafoamIslands_1F_Text_UsedToBeFloodedWithLava::
	.string "Can you believe this whole area\n"
	.string "used to be flooded with lava?\p"
	.string "One sudden change, and the\n"
	.string "whole cave became something else.$"
