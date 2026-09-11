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
