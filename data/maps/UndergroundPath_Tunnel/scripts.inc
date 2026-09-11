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

UndergroundPath_Tunnel_EventScript_PokemaniacNolan::
	trainerbattle_single TRAINER_UNDERGROUND_POKEMANIAC_NOLAN, UndergroundPath_Tunnel_Text_PokemaniacNolanIntro, UndergroundPath_Tunnel_Text_PokemaniacNolanDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, UndergroundPath_Tunnel_EventScript_PokemaniacNolanRematch
	msgbox UndergroundPath_Tunnel_Text_PokemaniacNolanPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_PokemaniacNolanRematch::
	trainerbattle_rematch TRAINER_UNDERGROUND_POKEMANIAC_NOLAN, UndergroundPath_Tunnel_Text_PokemaniacNolanIntro, UndergroundPath_Tunnel_Text_PokemaniacNolanDefeat
	msgbox UndergroundPath_Tunnel_Text_PokemaniacNolanPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_PokemaniacRicardo::
	trainerbattle_single TRAINER_UNDERGROUND_POKEMANIAC_RICARDO, UndergroundPath_Tunnel_Text_PokemaniacRicardoIntro, UndergroundPath_Tunnel_Text_PokemaniacRicardoDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, UndergroundPath_Tunnel_EventScript_PokemaniacRicardoRematch
	msgbox UndergroundPath_Tunnel_Text_PokemaniacRicardoPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_PokemaniacRicardoRematch::
	trainerbattle_rematch TRAINER_UNDERGROUND_POKEMANIAC_RICARDO, UndergroundPath_Tunnel_Text_PokemaniacRicardoIntro, UndergroundPath_Tunnel_Text_PokemaniacRicardoDefeat
	msgbox UndergroundPath_Tunnel_Text_PokemaniacRicardoPostBattle, MSGBOX_AUTOCLOSE
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

UndergroundPath_Tunnel_EventScript_PokefanHugo::
	trainerbattle_single TRAINER_UNDERGROUND_POKEFAN_HUGO, UndergroundPath_Tunnel_Text_PokefanHugoIntro, UndergroundPath_Tunnel_Text_PokefanHugoDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, UndergroundPath_Tunnel_EventScript_PokefanHugoRematch
	msgbox UndergroundPath_Tunnel_Text_PokefanHugoPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_PokefanHugoRematch::
	trainerbattle_rematch TRAINER_UNDERGROUND_POKEFAN_HUGO, UndergroundPath_Tunnel_Text_PokefanHugoIntro, UndergroundPath_Tunnel_Text_PokefanHugoDefeat
	msgbox UndergroundPath_Tunnel_Text_PokefanHugoPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_PokefanElliot::
	trainerbattle_single TRAINER_UNDERGROUND_POKEFAN_ELLIOT, UndergroundPath_Tunnel_Text_PokefanElliotIntro, UndergroundPath_Tunnel_Text_PokefanElliotDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, UndergroundPath_Tunnel_EventScript_PokefanElliotRematch
	msgbox UndergroundPath_Tunnel_Text_PokefanElliotPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_PokefanElliotRematch::
	trainerbattle_rematch TRAINER_UNDERGROUND_POKEFAN_ELLIOT, UndergroundPath_Tunnel_Text_PokefanElliotIntro, UndergroundPath_Tunnel_Text_PokefanElliotDefeat
	msgbox UndergroundPath_Tunnel_Text_PokefanElliotPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_PokefanRich::
	trainerbattle_single TRAINER_UNDERGROUND_POKEFAN_RICH, UndergroundPath_Tunnel_Text_PokefanRichIntro, UndergroundPath_Tunnel_Text_PokefanRichDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, UndergroundPath_Tunnel_EventScript_PokefanRichRematch
	msgbox UndergroundPath_Tunnel_Text_PokefanRichPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_PokefanRichRematch::
	trainerbattle_rematch TRAINER_UNDERGROUND_POKEFAN_RICH, UndergroundPath_Tunnel_Text_PokefanRichIntro, UndergroundPath_Tunnel_Text_PokefanRichDefeat
	msgbox UndergroundPath_Tunnel_Text_PokefanRichPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_PokefanSol::
	trainerbattle_single TRAINER_UNDERGROUND_POKEFAN_SOL, UndergroundPath_Tunnel_Text_PokefanSolIntro, UndergroundPath_Tunnel_Text_PokefanSolDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, UndergroundPath_Tunnel_EventScript_PokefanSolRematch
	msgbox UndergroundPath_Tunnel_Text_PokefanSolPostBattle, MSGBOX_AUTOCLOSE
	end

UndergroundPath_Tunnel_EventScript_PokefanSolRematch::
	trainerbattle_rematch TRAINER_UNDERGROUND_POKEFAN_SOL, UndergroundPath_Tunnel_Text_PokefanSolIntro, UndergroundPath_Tunnel_Text_PokefanSolDefeat
	msgbox UndergroundPath_Tunnel_Text_PokefanSolPostBattle, MSGBOX_AUTOCLOSE
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

UndergroundPath_Tunnel_Text_PokemaniacNolanIntro:
	.string "Shortcuts collect stories.\n"
	.string "That makes this tunnel valuable.$"

UndergroundPath_Tunnel_Text_PokemaniacNolanDefeat:
	.string "A rare loss for my records!$"

UndergroundPath_Tunnel_Text_PokemaniacNolanPostBattle:
	.string "I catalogue every trainer who\n"
	.string "passes under SAFFRON.$"

UndergroundPath_Tunnel_Text_PokemaniacRicardoIntro:
	.string "People drop odd things when they\n"
	.string "duck underground.\p"
	.string "I collect battles, too!$"

UndergroundPath_Tunnel_Text_PokemaniacRicardoDefeat:
	.string "I'll file that under defeat.$"

UndergroundPath_Tunnel_Text_PokemaniacRicardoPostBattle:
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

UndergroundPath_Tunnel_Text_PokefanHugoIntro:
	.string "Four exits, one tunnel.\n"
	.string "Every trainer brings a\n"
	.string "different partner.\p"
	.string "Show me yours!$"

UndergroundPath_Tunnel_Text_PokefanHugoDefeat:
	.string "That team has spirit!$"

UndergroundPath_Tunnel_Text_PokefanHugoPostBattle:
	.string "I keep notes on the partners\n"
	.string "people choose underground.$"

UndergroundPath_Tunnel_Text_PokefanElliotIntro:
	.string "Rare teams pass under SAFFRON.\n"
	.string "Let me see yours up close!$"

UndergroundPath_Tunnel_Text_PokefanElliotDefeat:
	.string "That was worth the wait!$"

UndergroundPath_Tunnel_Text_PokefanElliotPostBattle:
	.string "Aboveground everyone hurries.\n"
	.string "Down here, I can admire a\n"
	.string "good team.$"

UndergroundPath_Tunnel_Text_PokefanRichIntro:
	.string "I came for the shortcut and\n"
	.string "stayed for the parade of\n"
	.string "POKEMON!$"

UndergroundPath_Tunnel_Text_PokefanRichDefeat:
	.string "Your POKEMON stole the show!$"

UndergroundPath_Tunnel_Text_PokefanRichPostBattle:
	.string "Every entrance brings another\n"
	.string "team to admire.$"

UndergroundPath_Tunnel_Text_PokefanSolIntro:
	.string "Every echo down here sounds\n"
	.string "like another POKEMON cry.\p"
	.string "Battle me before I guess wrong!$"

UndergroundPath_Tunnel_Text_PokefanSolDefeat:
	.string "I heard that loud and clear!$"

UndergroundPath_Tunnel_Text_PokefanSolPostBattle:
	.string "The tunnel makes every team\n"
	.string "feel a little mysterious.$"

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
