UndergroundPath_Tunnel_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, UndergroundPath_Tunnel_OnTransition
	.byte 0

UndergroundPath_Tunnel_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_UNDERGROUND_PATH_TUNNEL
	end

UndergroundPath_Tunnel_EventScript_PokefanAlton::
	trainerbattle_single TRAINER_UNDERGROUND_POKEFAN_ALTON, UndergroundPath_Tunnel_Text_PokefanAltonIntro, UndergroundPath_Tunnel_Text_PokefanAltonDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, UndergroundPath_Tunnel_EventScript_PokefanAltonRematch
	msgbox UndergroundPath_Tunnel_Text_PokefanAltonPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_PokefanAltonRematch::
	trainerbattle_rematch TRAINER_UNDERGROUND_POKEFAN_ALTON, UndergroundPath_Tunnel_Text_PokefanAltonIntro, UndergroundPath_Tunnel_Text_PokefanAltonDefeat
	msgbox UndergroundPath_Tunnel_Text_PokefanAltonPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_PokefanMira::
	trainerbattle_single TRAINER_UNDERGROUND_POKEFAN_MIRA, UndergroundPath_Tunnel_Text_PokefanMiraIntro, UndergroundPath_Tunnel_Text_PokefanMiraDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, UndergroundPath_Tunnel_EventScript_PokefanMiraRematch
	msgbox UndergroundPath_Tunnel_Text_PokefanMiraPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_PokefanMiraRematch::
	trainerbattle_rematch TRAINER_UNDERGROUND_POKEFAN_MIRA, UndergroundPath_Tunnel_Text_PokefanMiraIntro, UndergroundPath_Tunnel_Text_PokefanMiraDefeat
	msgbox UndergroundPath_Tunnel_Text_PokefanMiraPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_CollectorNolan::
	trainerbattle_single TRAINER_UNDERGROUND_COLLECTOR_NOLAN, UndergroundPath_Tunnel_Text_CollectorNolanIntro, UndergroundPath_Tunnel_Text_CollectorNolanDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, UndergroundPath_Tunnel_EventScript_CollectorNolanRematch
	msgbox UndergroundPath_Tunnel_Text_CollectorNolanPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_CollectorNolanRematch::
	trainerbattle_rematch TRAINER_UNDERGROUND_COLLECTOR_NOLAN, UndergroundPath_Tunnel_Text_CollectorNolanIntro, UndergroundPath_Tunnel_Text_CollectorNolanDefeat
	msgbox UndergroundPath_Tunnel_Text_CollectorNolanPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_CollectorRicardo::
	trainerbattle_single TRAINER_UNDERGROUND_COLLECTOR_RICARDO, UndergroundPath_Tunnel_Text_CollectorRicardoIntro, UndergroundPath_Tunnel_Text_CollectorRicardoDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, UndergroundPath_Tunnel_EventScript_CollectorRicardoRematch
	msgbox UndergroundPath_Tunnel_Text_CollectorRicardoPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_CollectorRicardoRematch::
	trainerbattle_rematch TRAINER_UNDERGROUND_COLLECTOR_RICARDO, UndergroundPath_Tunnel_Text_CollectorRicardoIntro, UndergroundPath_Tunnel_Text_CollectorRicardoDefeat
	msgbox UndergroundPath_Tunnel_Text_CollectorRicardoPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_SuperNerdVance::
	trainerbattle_single TRAINER_UNDERGROUND_SUPER_NERD_VANCE, UndergroundPath_Tunnel_Text_SuperNerdVanceIntro, UndergroundPath_Tunnel_Text_SuperNerdVanceDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, UndergroundPath_Tunnel_EventScript_SuperNerdVanceRematch
	msgbox UndergroundPath_Tunnel_Text_SuperNerdVancePostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_SuperNerdVanceRematch::
	trainerbattle_rematch TRAINER_UNDERGROUND_SUPER_NERD_VANCE, UndergroundPath_Tunnel_Text_SuperNerdVanceIntro, UndergroundPath_Tunnel_Text_SuperNerdVanceDefeat
	msgbox UndergroundPath_Tunnel_Text_SuperNerdVancePostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_SuperNerdLeslie::
	trainerbattle_single TRAINER_UNDERGROUND_SUPER_NERD_LESLIE, UndergroundPath_Tunnel_Text_SuperNerdLeslieIntro, UndergroundPath_Tunnel_Text_SuperNerdLeslieDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, UndergroundPath_Tunnel_EventScript_SuperNerdLeslieRematch
	msgbox UndergroundPath_Tunnel_Text_SuperNerdLesliePostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_SuperNerdLeslieRematch::
	trainerbattle_rematch TRAINER_UNDERGROUND_SUPER_NERD_LESLIE, UndergroundPath_Tunnel_Text_SuperNerdLeslieIntro, UndergroundPath_Tunnel_Text_SuperNerdLeslieDefeat
	msgbox UndergroundPath_Tunnel_Text_SuperNerdLesliePostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_GamerHugo::
	trainerbattle_single TRAINER_UNDERGROUND_GAMER_HUGO, UndergroundPath_Tunnel_Text_GamerHugoIntro, UndergroundPath_Tunnel_Text_GamerHugoDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, UndergroundPath_Tunnel_EventScript_GamerHugoRematch
	msgbox UndergroundPath_Tunnel_Text_GamerHugoPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_GamerHugoRematch::
	trainerbattle_rematch TRAINER_UNDERGROUND_GAMER_HUGO, UndergroundPath_Tunnel_Text_GamerHugoIntro, UndergroundPath_Tunnel_Text_GamerHugoDefeat
	msgbox UndergroundPath_Tunnel_Text_GamerHugoPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_GamerElliot::
	trainerbattle_single TRAINER_UNDERGROUND_GAMER_ELLIOT, UndergroundPath_Tunnel_Text_GamerElliotIntro, UndergroundPath_Tunnel_Text_GamerElliotDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, UndergroundPath_Tunnel_EventScript_GamerElliotRematch
	msgbox UndergroundPath_Tunnel_Text_GamerElliotPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_GamerElliotRematch::
	trainerbattle_rematch TRAINER_UNDERGROUND_GAMER_ELLIOT, UndergroundPath_Tunnel_Text_GamerElliotIntro, UndergroundPath_Tunnel_Text_GamerElliotDefeat
	msgbox UndergroundPath_Tunnel_Text_GamerElliotPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_JugglerRich::
	trainerbattle_single TRAINER_UNDERGROUND_JUGGLER_RICH, UndergroundPath_Tunnel_Text_JugglerRichIntro, UndergroundPath_Tunnel_Text_JugglerRichDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, UndergroundPath_Tunnel_EventScript_JugglerRichRematch
	msgbox UndergroundPath_Tunnel_Text_JugglerRichPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_JugglerRichRematch::
	trainerbattle_rematch TRAINER_UNDERGROUND_JUGGLER_RICH, UndergroundPath_Tunnel_Text_JugglerRichIntro, UndergroundPath_Tunnel_Text_JugglerRichDefeat
	msgbox UndergroundPath_Tunnel_Text_JugglerRichPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_JugglerSol::
	trainerbattle_single TRAINER_UNDERGROUND_JUGGLER_SOL, UndergroundPath_Tunnel_Text_JugglerSolIntro, UndergroundPath_Tunnel_Text_JugglerSolDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, UndergroundPath_Tunnel_EventScript_JugglerSolRematch
	msgbox UndergroundPath_Tunnel_Text_JugglerSolPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_JugglerSolRematch::
	trainerbattle_rematch TRAINER_UNDERGROUND_JUGGLER_SOL, UndergroundPath_Tunnel_Text_JugglerSolIntro, UndergroundPath_Tunnel_Text_JugglerSolDefeat
	msgbox UndergroundPath_Tunnel_Text_JugglerSolPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_PokemaniacDirk::
	trainerbattle_single TRAINER_UNDERGROUND_POKEMANIAC_DIRK, UndergroundPath_Tunnel_Text_PokemaniacDirkIntro, UndergroundPath_Tunnel_Text_PokemaniacDirkDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, UndergroundPath_Tunnel_EventScript_PokemaniacDirkRematch
	msgbox UndergroundPath_Tunnel_Text_PokemaniacDirkPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_PokemaniacDirkRematch::
	trainerbattle_rematch TRAINER_UNDERGROUND_POKEMANIAC_DIRK, UndergroundPath_Tunnel_Text_PokemaniacDirkIntro, UndergroundPath_Tunnel_Text_PokemaniacDirkDefeat
	msgbox UndergroundPath_Tunnel_Text_PokemaniacDirkPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_PokemaniacHideo::
	trainerbattle_single TRAINER_UNDERGROUND_POKEMANIAC_HIDEO, UndergroundPath_Tunnel_Text_PokemaniacHideoIntro, UndergroundPath_Tunnel_Text_PokemaniacHideoDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, UndergroundPath_Tunnel_EventScript_PokemaniacHideoRematch
	msgbox UndergroundPath_Tunnel_Text_PokemaniacHideoPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_Text_PokefanAltonIntro:
	.string "I come down here when SAFFRON's\n"
	.string "crowds get too loud.\p"
	.string "My {PKMN} still like attention!$"

UndergroundPath_Tunnel_Text_PokefanAltonDefeat:
	.string "They got plenty of attention!$"

UndergroundPath_Tunnel_Text_PokefanAltonPostBattle:
	.string "A tunnel full of trainers feels\n"
	.string "friendlier than a busy street.$"

UndergroundPath_Tunnel_Text_PokefanMiraIntro:
	.string "Everyone above hurries past each\n"
	.string "other toward SAFFRON.\p"
	.string "Down here, we can actually chat.$"

UndergroundPath_Tunnel_Text_PokefanMiraDefeat:
	.string "That was a warm little battle.$"

UndergroundPath_Tunnel_Text_PokefanMiraPostBattle:
	.string "I like hearing footsteps before\n"
	.string "I see who is coming.$"

UndergroundPath_Tunnel_Text_CollectorNolanIntro:
	.string "Shortcuts collect stories.\n"
	.string "That makes this tunnel valuable.$"

UndergroundPath_Tunnel_Text_CollectorNolanDefeat:
	.string "A rare loss for my records!$"

UndergroundPath_Tunnel_Text_CollectorNolanPostBattle:
	.string "I catalogue every trainer who\n"
	.string "passes under SAFFRON.$"

UndergroundPath_Tunnel_Text_CollectorRicardoIntro:
	.string "People drop odd things when they\n"
	.string "duck underground.\p"
	.string "I collect battles, too!$"

UndergroundPath_Tunnel_Text_CollectorRicardoDefeat:
	.string "I'll file that under defeat.$"

UndergroundPath_Tunnel_Text_CollectorRicardoPostBattle:
	.string "The path above belongs to traffic.\n"
	.string "This path belongs to finds.$"

UndergroundPath_Tunnel_Text_SuperNerdVanceIntro:
	.string "The air pressure shifts near each\n"
	.string "stairwell.\p"
	.string "Let's test battle acoustics!$"

UndergroundPath_Tunnel_Text_SuperNerdVanceDefeat:
	.string "My figures were off!$"

UndergroundPath_Tunnel_Text_SuperNerdVancePostBattle:
	.string "A cross tunnel changes sound in\n"
	.string "ways a straight one never could.$"

UndergroundPath_Tunnel_Text_SuperNerdLeslieIntro:
	.string "SAFFRON sits above us like a giant\n"
	.string "circuit board.\p"
	.string "Care to complete a battle circuit?$"

UndergroundPath_Tunnel_Text_SuperNerdLeslieDefeat:
	.string "Open circuit!$"

UndergroundPath_Tunnel_Text_SuperNerdLesliePostBattle:
	.string "Every entrance feeds a different\n"
	.string "flow of trainers underground.$"

UndergroundPath_Tunnel_Text_GamerHugoIntro:
	.string "Four exits, one tunnel.\n"
	.string "Pick wrong and you walk farther.\p"
	.string "Pick me and you battle!$"

UndergroundPath_Tunnel_Text_GamerHugoDefeat:
	.string "Bad call!$"

UndergroundPath_Tunnel_Text_GamerHugoPostBattle:
	.string "The best bets are made where\n"
	.string "people are trying to pass quickly.$"

UndergroundPath_Tunnel_Text_GamerElliotIntro:
	.string "I wagered that trainers cutting\n"
	.string "under SAFFRON would be strong.$"

UndergroundPath_Tunnel_Text_GamerElliotDefeat:
	.string "That bet paid out for you!$"

UndergroundPath_Tunnel_Text_GamerElliotPostBattle:
	.string "Routes above change with crowds.\n"
	.string "The tunnel's odds feel steadier.$"

UndergroundPath_Tunnel_Text_JugglerRichIntro:
	.string "Balancing balls is easy.\n"
	.string "Balancing four entrances is art!$"

UndergroundPath_Tunnel_Text_JugglerRichDefeat:
	.string "I lost the rhythm!$"

UndergroundPath_Tunnel_Text_JugglerRichPostBattle:
	.string "Every echo down here arrives a\n"
	.string "moment after the throw.$"

UndergroundPath_Tunnel_Text_JugglerSolIntro:
	.string "The tunnel bends attention.\n"
	.string "Look one way, miss another!\p"
	.string "Can you track my tricks?$"

UndergroundPath_Tunnel_Text_JugglerSolDefeat:
	.string "You tracked them all!$"

UndergroundPath_Tunnel_Text_JugglerSolPostBattle:
	.string "Underground, even a simple toss\n"
	.string "feels like stage magic.$"

UndergroundPath_Tunnel_Text_PokemaniacDirkIntro:
	.string "Rare {PKMN} rumors travel faster\n"
	.string "underground than above.$"

UndergroundPath_Tunnel_Text_PokemaniacDirkDefeat:
	.string "My rumor fizzled out!$"

UndergroundPath_Tunnel_Text_PokemaniacDirkPostBattle:
	.string "SAFFRON's streets talk business.\n"
	.string "Down here, people talk {PKMN}.$"

UndergroundPath_Tunnel_Text_PokemaniacHideoIntro:
	.string "I haunt this tunnel for odd teams\n"
	.string "and stranger battle stories.$"

UndergroundPath_Tunnel_Text_PokemaniacHideoDefeat:
	.string "Your story beats mine!$"

UndergroundPath_Tunnel_Text_PokemaniacHideoPostBattle:
	.string "A real fanatic knows the best\n"
	.string "finds are below the obvious path.$"

UndergroundPath_Tunnel_EventScript_PokemaniacHideoRematch::
	trainerbattle_rematch TRAINER_UNDERGROUND_POKEMANIAC_HIDEO, UndergroundPath_Tunnel_Text_PokemaniacHideoIntro, UndergroundPath_Tunnel_Text_PokemaniacHideoDefeat
	msgbox UndergroundPath_Tunnel_Text_PokemaniacHideoPostBattle, MSGBOX_AUTOCLOSE
	end
