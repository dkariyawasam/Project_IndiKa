CeruleanCity_House4_MapScripts::
	.byte 0

CeruleanCity_House4_EventScript_BoredMan::
	lock
	goto CeruleanCity_House4_EventScript_NoNews
	end

CeruleanCity_House4_EventScript_NoNews::
	msgbox CeruleanCity_House4_Text_NothingEntertaining
	release
	end
