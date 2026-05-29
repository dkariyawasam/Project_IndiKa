CinnabarVolcano_RubyPath_B3F_MapScripts::
	.byte 0

@ Odd that this is here and not in CinnabarVolcano_RubyPath_B5F/scripts.inc
CinnabarVolcano_RubyPath_B5F_EventScript_Ruby::
	lock
	faceplayer
	setflag FLAG_GOT_RUBY
	removeobject LOCALID_RUBY
	giveitem_msg CinnabarVolcano_RubyPath_B3F_Text_FoundARuby, ITEM_RUBY, 1, MUS_OBTAIN_KEY_ITEM
	release
	end
