EventScript_SetSail::
	closemessage
	delay 20
	goto_if_questlog EventScript_ReleaseEnd
	special QuestLog_CutRecording
	fadescreen FADE_TO_BLACK
	special DoSeagallopFerryScene
	waitstate
	end
