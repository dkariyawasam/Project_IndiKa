FuchsiaCity_House2_MapScripts::
	.byte 0

FuchsiaCity_House2_EventScript_Herbalist::
	lock
	faceplayer
	msgbox FuchsiaCity_House2_Text_Herbalist
	pokemart FuchsiaCity_House2_HerbalistItems
	msgbox FuchsiaCity_House2_Text_Farewell
	release
	end

	.align 2
FuchsiaCity_House2_HerbalistItems::
	.2byte ITEM_ENERGY_POWDER
	.2byte ITEM_ENERGY_ROOT
	.2byte ITEM_HEAL_POWDER
	.2byte ITEM_REVIVAL_HERB
	.2byte ITEM_BRIGHT_POWDER
	.2byte ITEM_WHITE_HERB
	.2byte ITEM_MIRACLE_SEED
	.2byte ITEM_MENTAL_HERB
	.2byte ITEM_NONE
