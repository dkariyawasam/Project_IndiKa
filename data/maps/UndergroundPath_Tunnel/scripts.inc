UndergroundPath_Tunnel_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, UndergroundPath_Tunnel_OnTransition
	.byte 0

UndergroundPath_Tunnel_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_UNDERGROUND_PATH_TUNNEL
	end

UndergroundPath_Tunnel_EventScript_PokefanAlton::
	trainerbattle_single TRAINER_UNDERGROUND_POKEFAN_ALTON, UndergroundPath_Tunnel_Text_PokefanIntro, UndergroundPath_Tunnel_Text_PokefanDefeat
	msgbox UndergroundPath_Tunnel_Text_PokefanPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_PokefanMira::
	trainerbattle_single TRAINER_UNDERGROUND_POKEFAN_MIRA, UndergroundPath_Tunnel_Text_PokefanIntro, UndergroundPath_Tunnel_Text_PokefanDefeat
	msgbox UndergroundPath_Tunnel_Text_PokefanPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_CollectorNolan::
	trainerbattle_single TRAINER_UNDERGROUND_COLLECTOR_NOLAN, UndergroundPath_Tunnel_Text_CollectorIntro, UndergroundPath_Tunnel_Text_CollectorDefeat
	msgbox UndergroundPath_Tunnel_Text_CollectorPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_CollectorRicardo::
	trainerbattle_single TRAINER_UNDERGROUND_COLLECTOR_RICARDO, UndergroundPath_Tunnel_Text_CollectorIntro, UndergroundPath_Tunnel_Text_CollectorDefeat
	msgbox UndergroundPath_Tunnel_Text_CollectorPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_SuperNerdVance::
	trainerbattle_single TRAINER_UNDERGROUND_SUPER_NERD_VANCE, UndergroundPath_Tunnel_Text_SuperNerdIntro, UndergroundPath_Tunnel_Text_SuperNerdDefeat
	msgbox UndergroundPath_Tunnel_Text_SuperNerdPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_SuperNerdLeslie::
	trainerbattle_single TRAINER_UNDERGROUND_SUPER_NERD_LESLIE, UndergroundPath_Tunnel_Text_SuperNerdIntro, UndergroundPath_Tunnel_Text_SuperNerdDefeat
	msgbox UndergroundPath_Tunnel_Text_SuperNerdPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_GamerHugo::
	trainerbattle_single TRAINER_UNDERGROUND_GAMER_HUGO, UndergroundPath_Tunnel_Text_GamerIntro, UndergroundPath_Tunnel_Text_GamerDefeat
	msgbox UndergroundPath_Tunnel_Text_GamerPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_GamerElliot::
	trainerbattle_single TRAINER_UNDERGROUND_GAMER_ELLIOT, UndergroundPath_Tunnel_Text_GamerIntro, UndergroundPath_Tunnel_Text_GamerDefeat
	msgbox UndergroundPath_Tunnel_Text_GamerPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_JugglerRich::
	trainerbattle_single TRAINER_UNDERGROUND_JUGGLER_RICH, UndergroundPath_Tunnel_Text_JugglerIntro, UndergroundPath_Tunnel_Text_JugglerDefeat
	msgbox UndergroundPath_Tunnel_Text_JugglerPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_JugglerSol::
	trainerbattle_single TRAINER_UNDERGROUND_JUGGLER_SOL, UndergroundPath_Tunnel_Text_JugglerIntro, UndergroundPath_Tunnel_Text_JugglerDefeat
	msgbox UndergroundPath_Tunnel_Text_JugglerPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_PokemaniacDirk::
	trainerbattle_single TRAINER_UNDERGROUND_POKEMANIAC_DIRK, UndergroundPath_Tunnel_Text_PokemaniacIntro, UndergroundPath_Tunnel_Text_PokemaniacDefeat
	msgbox UndergroundPath_Tunnel_Text_PokemaniacPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_PokemaniacHideo::
	trainerbattle_single TRAINER_UNDERGROUND_POKEMANIAC_HIDEO, UndergroundPath_Tunnel_Text_PokemaniacIntro, UndergroundPath_Tunnel_Text_PokemaniacDefeat
	msgbox UndergroundPath_Tunnel_Text_PokemaniacPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_Text_PokefanIntro:
	.string "The tunnel is calmer than the\n"
	.string "streets above.\p"
	.string "Perfect for a little battle!$"

UndergroundPath_Tunnel_Text_PokefanDefeat:
	.string "Oh, that was lively!$"

UndergroundPath_Tunnel_Text_PokefanPostBattle:
	.string "A good shortcut should have a\n"
	.string "few friendly faces in it.$"

UndergroundPath_Tunnel_Text_CollectorIntro:
	.string "You never know what rare things\n"
	.string "pass through this tunnel.$"

UndergroundPath_Tunnel_Text_CollectorDefeat:
	.string "That's quite a find!$"

UndergroundPath_Tunnel_Text_CollectorPostBattle:
	.string "I keep notes on every unusual\n"
	.string "{PKMN} I see down here.$"

UndergroundPath_Tunnel_Text_SuperNerdIntro:
	.string "The acoustic properties down\n"
	.string "here are fascinating!\p"
	.string "Battle data should echo nicely.$"

UndergroundPath_Tunnel_Text_SuperNerdDefeat:
	.string "My data collapsed!$"

UndergroundPath_Tunnel_Text_SuperNerdPostBattle:
	.string "Maybe I should measure the\n"
	.string "tunnel before battling again.$"

UndergroundPath_Tunnel_Text_GamerIntro:
	.string "Pick a path, place a bet,\n"
	.string "take your chances!$"

UndergroundPath_Tunnel_Text_GamerDefeat:
	.string "Bad odds!$"

UndergroundPath_Tunnel_Text_GamerPostBattle:
	.string "The house always wins, unless\n"
	.string "you battle like that.$"

UndergroundPath_Tunnel_Text_JugglerIntro:
	.string "I can keep three ideas in the\n"
	.string "air at once.\p"
	.string "Can you keep up?$"

UndergroundPath_Tunnel_Text_JugglerDefeat:
	.string "Dropped it!$"

UndergroundPath_Tunnel_Text_JugglerPostBattle:
	.string "A straight tunnel is still full\n"
	.string "of tricky timing.$"

UndergroundPath_Tunnel_Text_PokemaniacIntro:
	.string "People rush through here, but I\n"
	.string "know the real treasure is {PKMN}!$"

UndergroundPath_Tunnel_Text_PokemaniacDefeat:
	.string "My favorites lost?$"

UndergroundPath_Tunnel_Text_PokemaniacPostBattle:
	.string "The tunnel feels like a secret\n"
	.string "club for serious fans.$"
