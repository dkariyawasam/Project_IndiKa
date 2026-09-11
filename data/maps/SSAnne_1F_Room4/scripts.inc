SSAnne_1F_Room4_MapScripts::
	.byte 0

SSAnne_1F_Room4_EventScript_Woman::
	lock
	faceplayer
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, SSAnne_1F_Room4_EventScript_WomanPlayerMale
	msgbox SSAnne_1F_Room4_Text_WaitressCherryPiePlease
	release
	end

SSAnne_1F_Room4_EventScript_WomanPlayerMale::
	msgbox SSAnne_1F_Room4_Text_WaiterCherryPiePlease
	release
	end

SSAnne_1F_Room4_EventScript_TrendsettersBriRemi::
	trainerbattle_double TRAINER_TRENDSETTERS_BRI_REMI, SSAnne_1F_Room4_EventScript_TrendsettersBriRemi_Intro, SSAnne_1F_Room4_EventScript_TrendsettersBriRemi_Defeat, SSAnne_1F_Room4_EventScript_TrendsettersBriRemi_NeedTwo
	msgbox SSAnne_1F_Room4_EventScript_TrendsettersBriRemi_Post, MSGBOX_AUTOCLOSE
	end

SSAnne_1F_Room4_EventScript_TrendsettersBriRemi_Intro::
    .string "A grand voyage needs\na grand entrance!$"

SSAnne_1F_Room4_EventScript_TrendsettersBriRemi_Defeat::
    .string "You have a style all your own!$"

SSAnne_1F_Room4_EventScript_TrendsettersBriRemi_Post::
    .string "Our next stop?\nThe shops in Celadon!$"

SSAnne_1F_Room4_EventScript_TrendsettersBriRemi_NeedTwo::
    .string "Bring two POKéMON and show us\nyour best combination!$"

