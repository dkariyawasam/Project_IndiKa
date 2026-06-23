LavenderTown_House1_MapScripts::
	.byte 0

LavenderTown_House1_EventScript_AceTrainerF::
	lock
	faceplayer
	goto_if_set FLAG_RESCUED_MR_FUJI, LavenderTown_House1_EventScript_AceTrainerFGhostGone
	msgbox LavenderTown_House1_Text_RocketsKilledCubonesMother
	release
	end

LavenderTown_House1_EventScript_AceTrainerFGhostGone::
	msgbox LavenderTown_House1_Text_GhostOfPokemonTowerIsGone
	release
	end

LavenderTown_House1_EventScript_Cubone::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_CUBONE, CRY_MODE_NORMAL
	msgbox LavenderTown_House1_Text_Cubone
	waitmoncry
	release
	end
