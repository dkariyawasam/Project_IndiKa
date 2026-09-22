Route3_MapScripts::
	.byte 0

Route3_EventScript_Youngster::
	msgbox Route3_Text_TunnelFromCeruleanTiring, MSGBOX_NPC
	end

Route3_EventScript_RouteSign::
	msgbox Route3_Text_RouteSign, MSGBOX_SIGN
	end

Route3_EventScript_YoungsterMia::
	trainerbattle_single TRAINER_YOUNGSTER_MIA, Route3_Text_YoungsterMiaIntro, Route3_Text_YoungsterMiaDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route3_EventScript_YoungsterMiaRematch
	msgbox Route3_Text_YoungsterMiaPostBattle, MSGBOX_AUTOCLOSE
	end

Route3_EventScript_YoungsterMiaRematch::
	trainerbattle_rematch TRAINER_YOUNGSTER_MIA, Route3_Text_YoungsterMiaRematchIntro, Route3_Text_YoungsterMiaDefeat
	msgbox Route3_Text_YoungsterMiaPostBattle, MSGBOX_AUTOCLOSE
	end

Route3_Text_YoungsterMiaIntro::
	.string "Josh and I came out to train!\nWant to battle me too?$"

Route3_Text_YoungsterMiaDefeat::
	.string "I need more practice!$"

Route3_Text_YoungsterMiaPostBattle::
	.string "We take turns picking\nwhere to explore next.$"

Route3_Text_YoungsterMiaRematchIntro::
	.string "Ready for another battle?$"

Route3_EventScript_RivalConversation::
	goto_if_set FLAG_HIDE_ROUTE3_RIVAL_CONVERSATION, EventScript_Return
	lockall
	setvar VAR_LAST_TALKED, 6
	faceplayer
	msgbox Route3_Text_RivalConversation
	closemessage
	fadescreen FADE_TO_BLACK
	setflag FLAG_HIDE_ROUTE3_RIVAL_CONVERSATION
	setvar VAR_ROUTE3_RIVAL_CONVERSATION, 1
	removeobject 6
	fadescreen FADE_FROM_BLACK
	releaseall
	end

Route3_Text_RivalConversation::
	.string "{RIVAL}: Hey, {PLAYER}!\n"
	.string "Heading for MT. MOON too?\p"
	.string "I'm taking on the GYMS.\n"
	.string "All eight of them!\p"
	.string "Those LEADERS know their teams\n"
	.string "like they've been together forever.\p"
	.string "My partner and I are still\n"
	.string "figuring each other out.\p"
	.string "We'll learn as we go. Next time,\n"
	.string "I'll show you how far we've come!$"
