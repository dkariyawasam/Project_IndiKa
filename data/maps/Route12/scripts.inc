Route12_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, Route12_OnResume
	.byte 0

Route12_OnResume::
	end

Route12_EventScript_DontUsePokeFlute::
	release
	end

Route12_EventScript_RouteSign::
	msgbox Route12_Text_RouteSign, MSGBOX_SIGN
	end

Route12_EventScript_FishingSign::
	msgbox Route12_Text_SportfishingArea, MSGBOX_SIGN
	end
