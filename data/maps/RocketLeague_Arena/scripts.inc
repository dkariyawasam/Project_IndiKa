RocketLeague_Arena_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, RocketLeague_Arena_OnResume
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, RocketLeague_Arena_OnWarp
	.byte 0

RocketLeague_Arena_OnResume::
	setvar VAR_0x8004, 2
	call PokemonLeague_EventScript_DoLightingEffect
	end

RocketLeague_Arena_OnWarp::
	map_script_2 VAR_TEMP_1, 0, RocketLeague_Arena_EventScript_TurnPlayerNorth
	.2byte 0

RocketLeague_Arena_EventScript_TurnPlayerNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

RocketLeague_Arena_EventScript_Official::
	lock
	faceplayer
	msgbox RocketLeague_Arena_Text_Official
	release
	end
