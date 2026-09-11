VictoryRoad_2F_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, VictoryRoad_2F_OnLoad
	.byte 0

VictoryRoad_2F_OnLoad::
	call_if_ne VAR_MAP_SCENE_VICTORY_ROAD_2F_BOULDER1, 100, VictoryRoad_2F_EventScript_SetRockBarrier1
	call_if_ne VAR_MAP_SCENE_VICTORY_ROAD_2F_BOULDER2, 100, VictoryRoad_2F_EventScript_SetRockBarrier2
	end

VictoryRoad_2F_EventScript_SetRockBarrier1::
	setmetatile 13, 10, METATILE_Cave_RockBarrier_Top, 1
	setmetatile 13, 11, METATILE_Cave_RockBarrier_Bottom, 1
	return

VictoryRoad_2F_EventScript_SetRockBarrier2::
	setmetatile 33, 16, METATILE_Cave_RockBarrier_Top, 1
	setmetatile 33, 17, METATILE_Cave_RockBarrier_Bottom, 1
	return

VictoryRoad_2F_EventScript_FloorSwitch1::
	lockall
	goto_if_eq VAR_MAP_SCENE_VICTORY_ROAD_2F_BOULDER1, 100, VictoryRoad_2F_EventScript_FloorSwitch1AlreadyPressed
	setmetatile 13, 10, METATILE_Cave_Floor_Ledge_Top, 0
	setmetatile 13, 11, METATILE_Cave_Floor_Ledge_Bottom, 0
	playse SE_ICE_BREAK
	special DrawWholeMapView
	waitse
	copyobjectxytoperm LOCALID_VICTORY_ROAD_2F_BOULDER1
	setvar VAR_MAP_SCENE_VICTORY_ROAD_2F_BOULDER1, 100
	releaseall
	end

VictoryRoad_2F_EventScript_FloorSwitch1AlreadyPressed::
	releaseall
	end

VictoryRoad_2F_EventScript_FloorSwitch2::
	lockall
	goto_if_eq VAR_MAP_SCENE_VICTORY_ROAD_2F_BOULDER2, 100, VictoryRoad_2F_EventScript_FloorSwitch2AlreadyPressed
	setmetatile 33, 16, METATILE_Cave_Floor_Ledge_Top, 0
	setmetatile 33, 17, METATILE_Cave_Floor_Ledge_Bottom, 0
	playse SE_ICE_BREAK
	special DrawWholeMapView
	waitse
	copyobjectxytoperm LOCALID_VICTORY_ROAD_2F_BOULDER2
	setvar VAR_MAP_SCENE_VICTORY_ROAD_2F_BOULDER2, 100
	releaseall
	end

VictoryRoad_2F_EventScript_FloorSwitch2AlreadyPressed::
	releaseall
	end

VictoryRoad_2F_EventScript_Dawson::
	trainerbattle_single TRAINER_POKEMANIAC_DAWSON, VictoryRoad_2F_Text_DawsonIntro, VictoryRoad_2F_Text_DawsonDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, VictoryRoad_2F_EventScript_DawsonRematch
	msgbox VictoryRoad_2F_Text_DawsonPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_2F_EventScript_DawsonRematch::
	trainerbattle_rematch TRAINER_POKEMANIAC_DAWSON, VictoryRoad_2F_Text_DawsonIntro, VictoryRoad_2F_Text_DawsonDefeat
	msgbox VictoryRoad_2F_Text_DawsonPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_2F_EventScript_Daisuke::
	trainerbattle_single TRAINER_BLACK_BELT_DAISUKE, VictoryRoad_2F_Text_DaisukeIntro, VictoryRoad_2F_Text_DaisukeDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, VictoryRoad_2F_EventScript_DaisukeRematch
	msgbox VictoryRoad_2F_Text_DaisukePostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_2F_EventScript_DaisukeRematch::
	trainerbattle_rematch TRAINER_BLACK_BELT_DAISUKE, VictoryRoad_2F_Text_DaisukeIntro, VictoryRoad_2F_Text_DaisukeDefeat
	msgbox VictoryRoad_2F_Text_DaisukePostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_2F_EventScript_Nelson::
	trainerbattle_single TRAINER_ACE_TRAINER_NELSON, VictoryRoad_2F_Text_NelsonIntro, VictoryRoad_2F_Text_NelsonDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, VictoryRoad_2F_EventScript_NelsonRematch
	msgbox VictoryRoad_2F_Text_NelsonPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_2F_EventScript_NelsonRematch::
	trainerbattle_rematch TRAINER_ACE_TRAINER_NELSON, VictoryRoad_2F_Text_NelsonIntro, VictoryRoad_2F_Text_NelsonDefeat
	msgbox VictoryRoad_2F_Text_NelsonPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_2F_EventScript_Gregory::
	trainerbattle_single TRAINER_ACE_TRAINER_GREGORY, VictoryRoad_2F_Text_GregoryIntro, VictoryRoad_2F_Text_GregoryDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, VictoryRoad_2F_EventScript_GregoryRematch
	msgbox VictoryRoad_2F_Text_GregoryPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_2F_EventScript_GregoryRematch::
	trainerbattle_rematch TRAINER_ACE_TRAINER_GREGORY, VictoryRoad_2F_Text_GregoryIntro, VictoryRoad_2F_Text_GregoryDefeat
	msgbox VictoryRoad_2F_Text_GregoryPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_2F_EventScript_Vincent::
	trainerbattle_single TRAINER_RUIN_MANIAC_VINCENT, VictoryRoad_2F_Text_VincentIntro, VictoryRoad_2F_Text_VincentDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, VictoryRoad_2F_EventScript_VincentRematch
	msgbox VictoryRoad_2F_Text_VincentPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_2F_EventScript_VincentRematch::
	trainerbattle_rematch TRAINER_RUIN_MANIAC_VINCENT, VictoryRoad_2F_Text_VincentIntro, VictoryRoad_2F_Text_VincentDefeat
	msgbox VictoryRoad_2F_Text_VincentPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_2F_EventScript_KaiTessKai::
	trainerbattle_double TRAINER_ACES_KAI_TESS, VictoryRoad_2F_Text_KaiTessIntro, VictoryRoad_2F_Text_KaiTessDefeat, VictoryRoad_2F_Text_KaiTessNotEnoughMons
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, VictoryRoad_2F_EventScript_KaiTessKaiRematch
	msgbox VictoryRoad_2F_Text_KaiTessPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_2F_EventScript_KaiTessKaiRematch::
	trainerbattle_rematch_double TRAINER_ACES_KAI_TESS, VictoryRoad_2F_Text_KaiTessIntro, VictoryRoad_2F_Text_KaiTessDefeat, VictoryRoad_2F_Text_KaiTessNotEnoughMons
	msgbox VictoryRoad_2F_Text_KaiTessPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_2F_EventScript_KaiTessTess::
	trainerbattle_double TRAINER_ACES_KAI_TESS, VictoryRoad_2F_Text_KaiTessIntro, VictoryRoad_2F_Text_KaiTessDefeat, VictoryRoad_2F_Text_KaiTessNotEnoughMons
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, VictoryRoad_2F_EventScript_KaiTessTessRematch
	msgbox VictoryRoad_2F_Text_KaiTessPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_2F_EventScript_KaiTessTessRematch::
	trainerbattle_rematch_double TRAINER_ACES_KAI_TESS, VictoryRoad_2F_Text_KaiTessIntro, VictoryRoad_2F_Text_KaiTessDefeat, VictoryRoad_2F_Text_KaiTessNotEnoughMons
	msgbox VictoryRoad_2F_Text_KaiTessPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_2F_Text_KaiTessIntro::
	.string "This cave tests your teamwork.\nShow us how your team works!$"

VictoryRoad_2F_Text_KaiTessDefeat::
	.string "You make a great team!$"

VictoryRoad_2F_Text_KaiTessPostBattle::
	.string "We cover each other when\nthe going gets tough.$"

VictoryRoad_2F_Text_KaiTessNotEnoughMons::
	.string "Bring two ready POKéMON\nso we can battle as a pair!$"

VictoryRoad_2F_EventScript_Stevie::
	trainerbattle_single TRAINER_POKEMANIAC_STEVE_2, VictoryRoad_2F_Text_StevieIntro, VictoryRoad_2F_Text_StevieDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, VictoryRoad_2F_EventScript_StevieRematch
	msgbox VictoryRoad_2F_Text_SteviePostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_2F_EventScript_StevieRematch::
	trainerbattle_rematch TRAINER_POKEMANIAC_STEVE_2, VictoryRoad_2F_Text_StevieIntro, VictoryRoad_2F_Text_StevieDefeat
	msgbox VictoryRoad_2F_Text_SteviePostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_2F_Text_StevieIntro::
	.string "Every cave is a training ground.\nShow me your fighting spirit!$"

VictoryRoad_2F_Text_StevieDefeat::
	.string "My training is far from over!$"

VictoryRoad_2F_Text_SteviePostBattle::
	.string "Rock Tunnel taught me patience.\nThis road will test my resolve.$"

VictoryRoad_2F_EventScript_AceTrainerFIris::
	trainerbattle_single TRAINER_ACE_TRAINER_IRIS, VictoryRoad_2F_Text_AceTrainerFIrisIntro, VictoryRoad_2F_Text_AceTrainerFIrisDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, VictoryRoad_2F_EventScript_AceTrainerFIrisRematch
	msgbox VictoryRoad_2F_Text_AceTrainerFIrisPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_2F_EventScript_AceTrainerFIrisRematch::
	trainerbattle_rematch TRAINER_ACE_TRAINER_IRIS, VictoryRoad_2F_Text_AceTrainerFIrisIntro, VictoryRoad_2F_Text_AceTrainerFIrisDefeat
	msgbox VictoryRoad_2F_Text_AceTrainerFIrisPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_2F_Text_AceTrainerFIrisIntro::
	.string "This road tests every weakness.\nHow well do you know your team?$"

VictoryRoad_2F_Text_AceTrainerFIrisDefeat::
	.string "You know your team well!$"

VictoryRoad_2F_Text_AceTrainerFIrisPostBattle::
	.string "I want to reach the LEAGUE\nwith a team I can trust.$"

VictoryRoad_2F_EventScript_AceTrainerFThea::
	trainerbattle_single TRAINER_ACE_TRAINER_THEA, VictoryRoad_2F_Text_AceTrainerFTheaIntro, VictoryRoad_2F_Text_AceTrainerFTheaDefeat
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, VictoryRoad_2F_EventScript_AceTrainerFTheaRematch
	msgbox VictoryRoad_2F_Text_AceTrainerFTheaPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_2F_EventScript_AceTrainerFTheaRematch::
	trainerbattle_rematch TRAINER_ACE_TRAINER_THEA, VictoryRoad_2F_Text_AceTrainerFTheaIntro, VictoryRoad_2F_Text_AceTrainerFTheaDefeat
	msgbox VictoryRoad_2F_Text_AceTrainerFTheaPostBattle, MSGBOX_AUTOCLOSE
	end

VictoryRoad_2F_Text_AceTrainerFTheaIntro::
	.string "Power alone will not be enough.\nMake every move count!$"

VictoryRoad_2F_Text_AceTrainerFTheaDefeat::
	.string "You made those moves count!$"

VictoryRoad_2F_Text_AceTrainerFTheaPostBattle::
	.string "The right move at the right time\ncan change the whole battle.$"
