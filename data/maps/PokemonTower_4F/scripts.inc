PokemonTower_4F_MapScripts::
	.byte 0

PokemonTower_4F_EventScript_MrFuji::
	lock
	faceplayer
	famechecker FAMECHECKER_MRFUJI, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	setflag FLAG_HIDE_TOWER_FUJI
	clearflag FLAG_HIDE_POKEHOUSE_FUJI
	setflag FLAG_RESCUED_MR_FUJI
	msgbox PokemonTower_4F_Text_MrFujiThankYouFollowMe
	closemessage
	warp MAP_LAVENDER_TOWN_VOLUNTEER_POKEMON_HOUSE, 4, 7
	waitstate
	release
	end
