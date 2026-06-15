Route1_MapScripts::
	.byte 0

Route1_EventScript_MartClerk::
	lock
	faceplayer
	goto_if_set FLAG_GOT_POTION_ON_ROUTE_1, Route1_EventScript_AlreadyGotPotion
	msgbox Route1_Text_WorkAtPokeMartTakeSample
	textcolor NPC_TEXT_COLOR_NEUTRAL
	checkitemspace ITEM_POTION
	goto_if_eq VAR_RESULT, FALSE, EventScript_BagIsFull
	bufferitemname STR_VAR_2, ITEM_POTION
	playfanfare MUS_LEVEL_UP
	message Text_ObtainedTheX
	waitmessage
	waitfanfare
	additem ITEM_POTION
	msgbox Route1_Text_PutPotionAway
	call EventScript_RestorePrevTextColor
	setflag FLAG_GOT_POTION_ON_ROUTE_1
	release
	end

Route1_EventScript_AlreadyGotPotion::
	msgbox Route1_Text_ComeSeeUsIfYouNeedPokeBalls
	release
	end

Route1_EventScript_Boy::
	msgbox Route1_Text_CanJumpFromLedges, MSGBOX_NPC
	end

Route1_EventScript_Ben::
	trainerbattle_single TRAINER_YOUNGSTER_BEN, Route1_Text_BenIntro, Route1_Text_BenDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route1_EventScript_BenRematch
	msgbox Route1_Text_BenPostBattle, MSGBOX_AUTOCLOSE
	end

Route1_EventScript_BenRematch::
	trainerbattle_rematch TRAINER_YOUNGSTER_BEN, Route1_Text_BenRematchIntro, Route1_Text_BenDefeat
	msgbox Route1_Text_BenPostBattle, MSGBOX_AUTOCLOSE
	end

Route1_EventScript_Janice::
	trainerbattle_single TRAINER_LASS_JANICE, Route1_Text_JaniceIntro, Route1_Text_JaniceDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route1_EventScript_JaniceRematch
	msgbox Route1_Text_JanicePostBattle, MSGBOX_AUTOCLOSE
	end

Route1_EventScript_JaniceRematch::
	trainerbattle_rematch TRAINER_LASS_JANICE, Route1_Text_JaniceRematchIntro, Route1_Text_JaniceDefeat
	msgbox Route1_Text_JanicePostBattle, MSGBOX_AUTOCLOSE
	end

Route1_EventScript_Milo::
	trainerbattle_single TRAINER_CAMPER_MILO, Route1_Text_MiloIntro, Route1_Text_MiloDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route1_EventScript_MiloRematch
	msgbox Route1_Text_MiloPostBattle, MSGBOX_AUTOCLOSE
	end

Route1_EventScript_MiloRematch::
	trainerbattle_rematch TRAINER_CAMPER_MILO, Route1_Text_MiloRematchIntro, Route1_Text_MiloDefeat
	msgbox Route1_Text_MiloPostBattle, MSGBOX_AUTOCLOSE
	end

Route1_EventScript_Liam::
	trainerbattle_single TRAINER_CAMPER_LIAM, Route1_Text_LiamIntro, Route1_Text_LiamDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route1_EventScript_LiamRematch
	msgbox Route1_Text_LiamPostBattle, MSGBOX_AUTOCLOSE
	end

Route1_EventScript_LiamRematch::
	trainerbattle_rematch TRAINER_CAMPER_LIAM, Route1_Text_LiamRematchIntro, Route1_Text_LiamDefeat
	msgbox Route1_Text_LiamPostBattle, MSGBOX_AUTOCLOSE
	end

Route1_EventScript_Diana::
	trainerbattle_single TRAINER_PICNICKER_DIANA, Route1_Text_DianaIntro, Route1_Text_DianaDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route1_EventScript_DianaRematch
	msgbox Route1_Text_DianaPostBattle, MSGBOX_AUTOCLOSE
	end

Route1_EventScript_DianaRematch::
	trainerbattle_rematch TRAINER_PICNICKER_DIANA, Route1_Text_DianaRematchIntro, Route1_Text_DianaDefeat
	msgbox Route1_Text_DianaPostBattle, MSGBOX_AUTOCLOSE
	end

Route1_EventScript_RouteSign::
	msgbox Route1_Text_RouteSign, MSGBOX_SIGN
	end
