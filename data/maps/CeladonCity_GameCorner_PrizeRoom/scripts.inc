CeladonCity_GameCorner_PrizeRoom_MapScripts::
	.byte 0

CeladonCity_GameCorner_PrizeRoom_EventScript_BaldingMan::
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_FancyThatPorygon, MSGBOX_NPC
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_OldMan::
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_RakedItInToday, MSGBOX_NPC
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_PrizeClerkMons::
	lock
	faceplayer
	goto_if_unset FLAG_GOT_COIN_CASE, CeladonCity_GameCorner_PrizeRoom_EventScript_NeedCoinCase
	goto_if_questlog EventScript_ReleaseEnd
	showcoinsbox 0, 0
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_WeExchangeCoinsForPrizes
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeMon::
	message CeladonCity_GameCorner_PrizeRoom_Text_WhichPrize
	waitmessage
	multichoice 11, 0, MULTICHOICE_GAME_CORNER_POKEMON_PRIZES, FALSE
	switch VAR_RESULT
	case 0, CeladonCity_GameCorner_PrizeRoom_EventScript_Abra
	case 1, CeladonCity_GameCorner_PrizeRoom_EventScript_Clefairy
	case 2, CeladonCity_GameCorner_PrizeRoom_EventScript_DratiniPinsir
	case 3, CeladonCity_GameCorner_PrizeRoom_EventScript_ScytherDratini
	case 4, CeladonCity_GameCorner_PrizeRoom_EventScript_Porygon
	case 5, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	case 127, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange::
	hidecoinsbox 0, 0
	release
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_Abra::
	.ifdef FIRERED
	setvar VAR_TEMP_1, SPECIES_ABRA
	setvar VAR_TEMP_2, 180
	.else
	.ifdef LEAFGREEN
	setvar VAR_TEMP_1, SPECIES_ABRA
	setvar VAR_TEMP_2, 120
	.endif
	.endif
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_Clefairy::
	.ifdef FIRERED
	setvar VAR_TEMP_1, SPECIES_CLEFAIRY
	setvar VAR_TEMP_2, 500
	.else
	.ifdef LEAFGREEN
	setvar VAR_TEMP_1, SPECIES_CLEFAIRY
	setvar VAR_TEMP_2, 750
	.endif
	.endif
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_DratiniPinsir::
	.ifdef FIRERED
	setvar VAR_TEMP_1, SPECIES_DRATINI
	setvar VAR_TEMP_2, 2800
	.else
	.ifdef LEAFGREEN
	setvar VAR_TEMP_1, SPECIES_PINSIR
	setvar VAR_TEMP_2, 2500
	.endif
	.endif
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ScytherDratini::
	.ifdef FIRERED
	setvar VAR_TEMP_1, SPECIES_SCYTHER
	setvar VAR_TEMP_2, 5500
	.else
	.ifdef LEAFGREEN
	setvar VAR_TEMP_1, SPECIES_DRATINI
	setvar VAR_TEMP_2, 4600
	.endif
	.endif
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_Porygon::
	.ifdef FIRERED
	setvar VAR_TEMP_1, SPECIES_PORYGON
	setvar VAR_TEMP_2, 9999
	.else
	.ifdef LEAFGREEN
	setvar VAR_TEMP_1, SPECIES_PORYGON
	setvar VAR_TEMP_2, 6500
	.endif
	.endif
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeMon::
	bufferspeciesname STR_VAR_1, VAR_TEMP_1
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_YouWantPrize, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	checkcoins VAR_RESULT
	goto_if_lt VAR_RESULT, VAR_TEMP_2, CeladonCity_GameCorner_PrizeRoom_EventScript_NotEnoughCoins
	textcolor NPC_TEXT_COLOR_NEUTRAL
	switch VAR_TEMP_1
	case SPECIES_ABRA,     CeladonCity_GameCorner_PrizeRoom_EventScript_GiveAbra
	case SPECIES_CLEFAIRY, CeladonCity_GameCorner_PrizeRoom_EventScript_GiveClefairy
	case SPECIES_DRATINI,  CeladonCity_GameCorner_PrizeRoom_EventScript_GiveDratini
	case SPECIES_SCYTHER,  CeladonCity_GameCorner_PrizeRoom_EventScript_GiveScyther
	case SPECIES_PORYGON,  CeladonCity_GameCorner_PrizeRoom_EventScript_GivePorygon
	case SPECIES_PINSIR,   CeladonCity_GameCorner_PrizeRoom_EventScript_GivePinsir
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_GiveAbra::
	.ifdef FIRERED
	givemon VAR_TEMP_1, 9
	.else
	.ifdef LEAFGREEN
	givemon VAR_TEMP_1, 7
	.endif
	.endif
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_CheckReceivedMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_GiveClefairy::
	.ifdef FIRERED
	givemon VAR_TEMP_1, 8
	.else
	.ifdef LEAFGREEN
	givemon VAR_TEMP_1, 12
	.endif
	.endif
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_CheckReceivedMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_GiveDratini::
	.ifdef FIRERED
	givemon VAR_TEMP_1, 18
	.else
	.ifdef LEAFGREEN
	givemon VAR_TEMP_1, 24
	.endif
	.endif
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_CheckReceivedMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_GiveScyther::
	givemon VAR_TEMP_1, 25
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_CheckReceivedMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_GivePorygon::
	.ifdef FIRERED
	givemon VAR_TEMP_1, 26
	.else
	.ifdef LEAFGREEN
	givemon VAR_TEMP_1, 18
	.endif
	.endif
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_CheckReceivedMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_GivePinsir::
	givemon VAR_TEMP_1, 18
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_CheckReceivedMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_CheckReceivedMon::
	goto_if_eq VAR_RESULT, 0, CeladonCity_GameCorner_PrizeRoom_EventScript_ReceivedMonParty
	goto_if_eq VAR_RESULT, 1, CeladonCity_GameCorner_PrizeRoom_EventScript_ReceivedMonPC
	goto_if_eq VAR_RESULT, 2, CeladonCity_GameCorner_PrizeRoom_EventScript_PartyFull
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_PartyFull::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Text_NoMoreRoomForPokemon
	hidecoinsbox 0, 0
	release
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_NicknamePartyMon::
	getpartysize
	subvar VAR_RESULT, 1
	copyvar VAR_0x8004, VAR_RESULT
	call EventScript_ChangePokemonNickname
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_NeedCoinCase::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_CoinCaseRequired
	release
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_NotEnoughCoins::
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_NeedMoreCoins
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ReceivedMonParty::
	removecoins VAR_TEMP_2
	updatecoinsbox 0, 5
	bufferspeciesname STR_VAR_1, VAR_TEMP_1
	playfanfare MUS_LEVEL_UP
	message Text_PlayerObtainedTheMon
	waitmessage
	waitfanfare
	msgbox Text_GiveNicknameToThisMon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, CeladonCity_GameCorner_PrizeRoom_EventScript_NicknamePartyMon
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ReceivedMonPC::
	removecoins VAR_TEMP_2
	updatecoinsbox 0, 5
	bufferspeciesname STR_VAR_1, VAR_TEMP_1
	playfanfare MUS_LEVEL_UP
	message Text_PlayerObtainedTheMon
	waitmessage
	waitfanfare
	msgbox Text_GiveNicknameToThisMon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, CeladonCity_GameCorner_PrizeRoom_EventScript_TransferredToPC
	call EventScript_NameReceivedBoxMon
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_TransferredToPC
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TransferredToPC::
	call EventScript_TransferredToPC
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_PrizeClerkTMs::
	lock
	faceplayer
	goto_if_unset FLAG_GOT_COIN_CASE, CeladonCity_GameCorner_PrizeRoom_EventScript_NeedCoinCase
	goto_if_questlog EventScript_ReleaseEnd
	showcoinsbox 0, 0
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_WeExchangeCoinsForPrizes
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM::
	message CeladonCity_GameCorner_PrizeRoom_Text_WhichPrize
	waitmessage
	multichoice 11, 0, MULTICHOICE_GAME_CORNER_TMPRIZES, FALSE
	switch VAR_RESULT
	case 0, CeladonCity_GameCorner_PrizeRoom_EventScript_TM01
	case 1, CeladonCity_GameCorner_PrizeRoom_EventScript_TM02
	case 2, CeladonCity_GameCorner_PrizeRoom_EventScript_TM03
	case 3, CeladonCity_GameCorner_PrizeRoom_EventScript_TM04
	case 4, CeladonCity_GameCorner_PrizeRoom_EventScript_TM05
	case 5, CeladonCity_GameCorner_PrizeRoom_EventScript_TM06
	case 6, CeladonCity_GameCorner_PrizeRoom_EventScript_TM07
	case 7, CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM2
	case 127, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM2::
	message CeladonCity_GameCorner_PrizeRoom_Text_WhichPrize
	waitmessage
	multichoice 11, 0, MULTICHOICE_GAME_CORNER_TMPRIZES_2, FALSE
	switch VAR_RESULT
	case 0, CeladonCity_GameCorner_PrizeRoom_EventScript_TM08
	case 1, CeladonCity_GameCorner_PrizeRoom_EventScript_TM09
	case 2, CeladonCity_GameCorner_PrizeRoom_EventScript_TM10
	case 3, CeladonCity_GameCorner_PrizeRoom_EventScript_TM11
	case 4, CeladonCity_GameCorner_PrizeRoom_EventScript_TM12
	case 5, CeladonCity_GameCorner_PrizeRoom_EventScript_TM13
	case 6, CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM3
	case 7, CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM
	case 127, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM3::
	message CeladonCity_GameCorner_PrizeRoom_Text_WhichPrize
	waitmessage
	multichoice 11, 0, MULTICHOICE_GAME_CORNER_TMPRIZES_3, FALSE
	switch VAR_RESULT
	case 0, CeladonCity_GameCorner_PrizeRoom_EventScript_TM14
	case 1, CeladonCity_GameCorner_PrizeRoom_EventScript_TM15
	case 2, CeladonCity_GameCorner_PrizeRoom_EventScript_TM16
	case 3, CeladonCity_GameCorner_PrizeRoom_EventScript_TM17
	case 4, CeladonCity_GameCorner_PrizeRoom_EventScript_TM18
	case 5, CeladonCity_GameCorner_PrizeRoom_EventScript_TM19
	case 6, CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM4
	case 7, CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM2
	case 127, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM4::
	message CeladonCity_GameCorner_PrizeRoom_Text_WhichPrize
	waitmessage
	multichoice 11, 0, MULTICHOICE_GAME_CORNER_TMPRIZES_4, FALSE
	switch VAR_RESULT
	case 0, CeladonCity_GameCorner_PrizeRoom_EventScript_TM20
	case 1, CeladonCity_GameCorner_PrizeRoom_EventScript_TM21
	case 2, CeladonCity_GameCorner_PrizeRoom_EventScript_TM22
	case 3, CeladonCity_GameCorner_PrizeRoom_EventScript_TM23
	case 4, CeladonCity_GameCorner_PrizeRoom_EventScript_TM24
	case 5, CeladonCity_GameCorner_PrizeRoom_EventScript_TM25
	case 6, CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM5
	case 7, CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM3
	case 127, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM5::
	message CeladonCity_GameCorner_PrizeRoom_Text_WhichPrize
	waitmessage
	multichoice 11, 0, MULTICHOICE_GAME_CORNER_TMPRIZES_5, FALSE
	switch VAR_RESULT
	case 0, CeladonCity_GameCorner_PrizeRoom_EventScript_TM26
	case 1, CeladonCity_GameCorner_PrizeRoom_EventScript_TM27
	case 2, CeladonCity_GameCorner_PrizeRoom_EventScript_TM28
	case 3, CeladonCity_GameCorner_PrizeRoom_EventScript_TM29
	case 4, CeladonCity_GameCorner_PrizeRoom_EventScript_TM30
	case 5, CeladonCity_GameCorner_PrizeRoom_EventScript_TM31
	case 6, CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM6
	case 7, CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM4
	case 127, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM6::
	message CeladonCity_GameCorner_PrizeRoom_Text_WhichPrize
	waitmessage
	multichoice 11, 0, MULTICHOICE_GAME_CORNER_TMPRIZES_6, FALSE
	switch VAR_RESULT
	case 0, CeladonCity_GameCorner_PrizeRoom_EventScript_TM32
	case 1, CeladonCity_GameCorner_PrizeRoom_EventScript_TM33
	case 2, CeladonCity_GameCorner_PrizeRoom_EventScript_TM34
	case 3, CeladonCity_GameCorner_PrizeRoom_EventScript_TM35
	case 4, CeladonCity_GameCorner_PrizeRoom_EventScript_TM36
	case 5, CeladonCity_GameCorner_PrizeRoom_EventScript_TM37
	case 6, CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM7
	case 7, CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM5
	case 127, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM7::
	message CeladonCity_GameCorner_PrizeRoom_Text_WhichPrize
	waitmessage
	multichoice 11, 0, MULTICHOICE_GAME_CORNER_TMPRIZES_7, FALSE
	switch VAR_RESULT
	case 0, CeladonCity_GameCorner_PrizeRoom_EventScript_TM38
	case 1, CeladonCity_GameCorner_PrizeRoom_EventScript_TM39
	case 2, CeladonCity_GameCorner_PrizeRoom_EventScript_TM40
	case 3, CeladonCity_GameCorner_PrizeRoom_EventScript_TM41
	case 4, CeladonCity_GameCorner_PrizeRoom_EventScript_TM42
	case 5, CeladonCity_GameCorner_PrizeRoom_EventScript_TM43
	case 6, CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM8
	case 7, CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM6
	case 127, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM8::
	message CeladonCity_GameCorner_PrizeRoom_Text_WhichPrize
	waitmessage
	multichoice 11, 0, MULTICHOICE_GAME_CORNER_TMPRIZES_8, FALSE
	switch VAR_RESULT
	case 0, CeladonCity_GameCorner_PrizeRoom_EventScript_TM44
	case 1, CeladonCity_GameCorner_PrizeRoom_EventScript_TM45
	case 2, CeladonCity_GameCorner_PrizeRoom_EventScript_TM46
	case 3, CeladonCity_GameCorner_PrizeRoom_EventScript_TM47
	case 4, CeladonCity_GameCorner_PrizeRoom_EventScript_TM48
	case 5, CeladonCity_GameCorner_PrizeRoom_EventScript_TM49
	case 6, CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM9
	case 7, CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM7
	case 127, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM9::
	message CeladonCity_GameCorner_PrizeRoom_Text_WhichPrize
	waitmessage
	multichoice 11, 0, MULTICHOICE_GAME_CORNER_TMPRIZES_9, FALSE
	switch VAR_RESULT
	case 0, CeladonCity_GameCorner_PrizeRoom_EventScript_TM50
	case 1, CeladonCity_GameCorner_PrizeRoom_EventScript_TM51
	case 2, CeladonCity_GameCorner_PrizeRoom_EventScript_TM52
	case 3, CeladonCity_GameCorner_PrizeRoom_EventScript_TM53
	case 4, CeladonCity_GameCorner_PrizeRoom_EventScript_TM54
	case 5, CeladonCity_GameCorner_PrizeRoom_EventScript_TM55
	case 6, CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM10
	case 7, CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM8
	case 127, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM10::
	message CeladonCity_GameCorner_PrizeRoom_Text_WhichPrize
	waitmessage
	multichoice 11, 0, MULTICHOICE_GAME_CORNER_TMPRIZES_10, FALSE
	switch VAR_RESULT
	case 0, CeladonCity_GameCorner_PrizeRoom_EventScript_TM56
	case 1, CeladonCity_GameCorner_PrizeRoom_EventScript_TM57
	case 2, CeladonCity_GameCorner_PrizeRoom_EventScript_TM58
	case 3, CeladonCity_GameCorner_PrizeRoom_EventScript_TM59
	case 4, CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM9
	case 127, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM01::
	setvar VAR_TEMP_1, ITEM_TM01
	setvar VAR_TEMP_2, 4500
	buffermovename STR_VAR_2, MOVE_FOCUS_PUNCH
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM02::
	setvar VAR_TEMP_1, ITEM_TM02
	setvar VAR_TEMP_2, 5000
	buffermovename STR_VAR_2, MOVE_DRAGON_CLAW
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM03::
	setvar VAR_TEMP_1, ITEM_TM03
	setvar VAR_TEMP_2, 3000
	buffermovename STR_VAR_2, MOVE_WATER_PULSE
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM04::
	setvar VAR_TEMP_1, ITEM_TM04
	setvar VAR_TEMP_2, 3500
	buffermovename STR_VAR_2, MOVE_CALM_MIND
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM05::
	setvar VAR_TEMP_1, ITEM_TM05
	setvar VAR_TEMP_2, 1000
	buffermovename STR_VAR_2, MOVE_ROAR
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM06::
	setvar VAR_TEMP_1, ITEM_TM06
	setvar VAR_TEMP_2, 4000
	buffermovename STR_VAR_2, MOVE_TOXIC
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM07::
	setvar VAR_TEMP_1, ITEM_TM07
	setvar VAR_TEMP_2, 1500
	buffermovename STR_VAR_2, MOVE_HAIL
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM08::
	setvar VAR_TEMP_1, ITEM_TM08
	setvar VAR_TEMP_2, 3500
	buffermovename STR_VAR_2, MOVE_BULK_UP
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM09::
	setvar VAR_TEMP_1, ITEM_TM09
	setvar VAR_TEMP_2, 1500
	buffermovename STR_VAR_2, MOVE_BULLET_SEED
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM10::
	setvar VAR_TEMP_1, ITEM_TM10
	setvar VAR_TEMP_2, 3000
	buffermovename STR_VAR_2, MOVE_HIDDEN_POWER
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM11::
	setvar VAR_TEMP_1, ITEM_TM11
	setvar VAR_TEMP_2, 2500
	buffermovename STR_VAR_2, MOVE_SUNNY_DAY
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM12::
	setvar VAR_TEMP_1, ITEM_TM12
	setvar VAR_TEMP_2, 2000
	buffermovename STR_VAR_2, MOVE_TAUNT
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM13::
	setvar VAR_TEMP_1, ITEM_TM13
	setvar VAR_TEMP_2, 5000
	buffermovename STR_VAR_2, MOVE_ICE_BEAM
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM14::
	setvar VAR_TEMP_1, ITEM_TM14
	setvar VAR_TEMP_2, 6000
	buffermovename STR_VAR_2, MOVE_BLIZZARD
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM15::
	setvar VAR_TEMP_1, ITEM_TM15
	setvar VAR_TEMP_2, 6500
	buffermovename STR_VAR_2, MOVE_HYPER_BEAM
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM16::
	setvar VAR_TEMP_1, ITEM_TM16
	setvar VAR_TEMP_2, 2500
	buffermovename STR_VAR_2, MOVE_LIGHT_SCREEN
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM17::
	setvar VAR_TEMP_1, ITEM_TM17
	setvar VAR_TEMP_2, 4000
	buffermovename STR_VAR_2, MOVE_PROTECT
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM18::
	setvar VAR_TEMP_1, ITEM_TM18
	setvar VAR_TEMP_2, 2500
	buffermovename STR_VAR_2, MOVE_RAIN_DANCE
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM19::
	setvar VAR_TEMP_1, ITEM_TM19
	setvar VAR_TEMP_2, 3500
	buffermovename STR_VAR_2, MOVE_GIGA_DRAIN
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM20::
	setvar VAR_TEMP_1, ITEM_TM20
	setvar VAR_TEMP_2, 2500
	buffermovename STR_VAR_2, MOVE_SAFEGUARD
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM21::
	setvar VAR_TEMP_1, ITEM_TM21
	setvar VAR_TEMP_2, 1000
	buffermovename STR_VAR_2, MOVE_FRUSTRATION
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM22::
	setvar VAR_TEMP_1, ITEM_TM22
	setvar VAR_TEMP_2, 4500
	buffermovename STR_VAR_2, MOVE_SOLAR_BEAM
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM23::
	setvar VAR_TEMP_1, ITEM_TM23
	setvar VAR_TEMP_2, 3500
	buffermovename STR_VAR_2, MOVE_IRON_TAIL
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM24::
	setvar VAR_TEMP_1, ITEM_TM24
	setvar VAR_TEMP_2, 5000
	buffermovename STR_VAR_2, MOVE_THUNDERBOLT
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM25::
	setvar VAR_TEMP_1, ITEM_TM25
	setvar VAR_TEMP_2, 6000
	buffermovename STR_VAR_2, MOVE_THUNDER
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM26::
	setvar VAR_TEMP_1, ITEM_TM26
	setvar VAR_TEMP_2, 6500
	buffermovename STR_VAR_2, MOVE_EARTHQUAKE
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM27::
	setvar VAR_TEMP_1, ITEM_TM27
	setvar VAR_TEMP_2, 3500
	buffermovename STR_VAR_2, MOVE_RETURN
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM28::
	setvar VAR_TEMP_1, ITEM_TM28
	setvar VAR_TEMP_2, 3000
	buffermovename STR_VAR_2, MOVE_DIG
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM29::
	setvar VAR_TEMP_1, ITEM_TM29
	setvar VAR_TEMP_2, 5000
	buffermovename STR_VAR_2, MOVE_PSYCHIC
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM30::
	setvar VAR_TEMP_1, ITEM_TM30
	setvar VAR_TEMP_2, 4500
	buffermovename STR_VAR_2, MOVE_SHADOW_BALL
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM31::
	setvar VAR_TEMP_1, ITEM_TM31
	setvar VAR_TEMP_2, 4000
	buffermovename STR_VAR_2, MOVE_BRICK_BREAK
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM32::
	setvar VAR_TEMP_1, ITEM_TM32
	setvar VAR_TEMP_2, 1500
	buffermovename STR_VAR_2, MOVE_DOUBLE_TEAM
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM33::
	setvar VAR_TEMP_1, ITEM_TM33
	setvar VAR_TEMP_2, 2500
	buffermovename STR_VAR_2, MOVE_REFLECT
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM34::
	setvar VAR_TEMP_1, ITEM_TM34
	setvar VAR_TEMP_2, 3000
	buffermovename STR_VAR_2, MOVE_SHOCK_WAVE
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM35::
	setvar VAR_TEMP_1, ITEM_TM35
	setvar VAR_TEMP_2, 5000
	buffermovename STR_VAR_2, MOVE_FLAMETHROWER
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM36::
	setvar VAR_TEMP_1, ITEM_TM36
	setvar VAR_TEMP_2, 4500
	buffermovename STR_VAR_2, MOVE_SLUDGE_BOMB
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM37::
	setvar VAR_TEMP_1, ITEM_TM37
	setvar VAR_TEMP_2, 2000
	buffermovename STR_VAR_2, MOVE_SANDSTORM
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM38::
	setvar VAR_TEMP_1, ITEM_TM38
	setvar VAR_TEMP_2, 6000
	buffermovename STR_VAR_2, MOVE_FIRE_BLAST
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM39::
	setvar VAR_TEMP_1, ITEM_TM39
	setvar VAR_TEMP_2, 2500
	buffermovename STR_VAR_2, MOVE_ROCK_TOMB
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM40::
	setvar VAR_TEMP_1, ITEM_TM40
	setvar VAR_TEMP_2, 3000
	buffermovename STR_VAR_2, MOVE_AERIAL_ACE
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM41::
	setvar VAR_TEMP_1, ITEM_TM41
	setvar VAR_TEMP_2, 1500
	buffermovename STR_VAR_2, MOVE_TORMENT
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM42::
	setvar VAR_TEMP_1, ITEM_TM42
	setvar VAR_TEMP_2, 3500
	buffermovename STR_VAR_2, MOVE_FACADE
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM43::
	setvar VAR_TEMP_1, ITEM_TM43
	setvar VAR_TEMP_2, 2000
	buffermovename STR_VAR_2, MOVE_SECRET_POWER
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM44::
	setvar VAR_TEMP_1, ITEM_TM44
	setvar VAR_TEMP_2, 2500
	buffermovename STR_VAR_2, MOVE_REST
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM45::
	setvar VAR_TEMP_1, ITEM_TM45
	setvar VAR_TEMP_2, 1500
	buffermovename STR_VAR_2, MOVE_ATTRACT
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM46::
	setvar VAR_TEMP_1, ITEM_TM46
	setvar VAR_TEMP_2, 2000
	buffermovename STR_VAR_2, MOVE_THIEF
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM47::
	setvar VAR_TEMP_1, ITEM_TM47
	setvar VAR_TEMP_2, 3000
	buffermovename STR_VAR_2, MOVE_STEEL_WING
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM48::
	setvar VAR_TEMP_1, ITEM_TM48
	setvar VAR_TEMP_2, 2000
	buffermovename STR_VAR_2, MOVE_SKILL_SWAP
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM49::
	setvar VAR_TEMP_1, ITEM_TM49
	setvar VAR_TEMP_2, 1500
	buffermovename STR_VAR_2, MOVE_SNATCH
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM50::
	setvar VAR_TEMP_1, ITEM_TM50
	setvar VAR_TEMP_2, 6000
	buffermovename STR_VAR_2, MOVE_OVERHEAT
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM51::
	setvar VAR_TEMP_1, ITEM_TM51
	setvar VAR_TEMP_2, 1000
	buffermovename STR_VAR_2, MOVE_CUT
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM52::
	setvar VAR_TEMP_1, ITEM_TM52
	setvar VAR_TEMP_2, 4500
	buffermovename STR_VAR_2, MOVE_FLY
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM53::
	setvar VAR_TEMP_1, ITEM_TM53
	setvar VAR_TEMP_2, 5500
	buffermovename STR_VAR_2, MOVE_SURF
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM54::
	setvar VAR_TEMP_1, ITEM_TM54
	setvar VAR_TEMP_2, 3500
	buffermovename STR_VAR_2, MOVE_STRENGTH
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM55::
	setvar VAR_TEMP_1, ITEM_TM55
	setvar VAR_TEMP_2, 1000
	buffermovename STR_VAR_2, MOVE_FLASH
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM56::
	setvar VAR_TEMP_1, ITEM_TM56
	setvar VAR_TEMP_2, 1500
	buffermovename STR_VAR_2, MOVE_ROCK_SMASH
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM57::
	setvar VAR_TEMP_1, ITEM_TM57
	setvar VAR_TEMP_2, 4500
	buffermovename STR_VAR_2, MOVE_WATERFALL
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM58::
	setvar VAR_TEMP_1, ITEM_TM58
	setvar VAR_TEMP_2, 3500
	buffermovename STR_VAR_2, MOVE_DIVE
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM59::
	setvar VAR_TEMP_1, ITEM_TM59
	setvar VAR_TEMP_2, 4500
	buffermovename STR_VAR_2, MOVE_DAZZLING_GLEAM
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM::
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_YouWantTM, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_TryGivePrize
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeItem::
	bufferitemname STR_VAR_1, VAR_TEMP_1
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_YouWantPrize, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_TryGivePrize
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TryGivePrize::
	bufferitemname STR_VAR_1, VAR_TEMP_1
	checkcoins VAR_RESULT
	goto_if_lt VAR_RESULT, VAR_TEMP_2, CeladonCity_GameCorner_PrizeRoom_EventScript_NotEnoughCoins
	checkitemspace VAR_TEMP_1
	goto_if_eq VAR_RESULT, FALSE, CeladonCity_GameCorner_PrizeRoom_EventScript_BagFull
	removecoins VAR_TEMP_2
	updatecoinsbox 0, 5
	giveitem VAR_TEMP_1
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_BagFull::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox Text_TooBadBagFull
	hidecoinsbox 0, 0
	release
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_PrizeClerkItems::
	lock
	faceplayer
	goto_if_unset FLAG_GOT_COIN_CASE, CeladonCity_GameCorner_PrizeRoom_EventScript_NeedCoinCase
	goto_if_questlog EventScript_ReleaseEnd
	showcoinsbox 0, 0
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_WeExchangeCoinsForPrizes
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeItem
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeItem::
	message CeladonCity_GameCorner_PrizeRoom_Text_WhichPrize
	waitmessage
	multichoice 10, 0, MULTICHOICE_GAME_CORNER_BATTLE_ITEM_PRIZES, FALSE
	switch VAR_RESULT
	case 0, CeladonCity_GameCorner_PrizeRoom_EventScript_SmokeBall
	case 1, CeladonCity_GameCorner_PrizeRoom_EventScript_MiracleSeed
	case 2, CeladonCity_GameCorner_PrizeRoom_EventScript_Charcoal
	case 3, CeladonCity_GameCorner_PrizeRoom_EventScript_MysticWater
	case 4, CeladonCity_GameCorner_PrizeRoom_EventScript_YellowFlute
	case 5, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	case 127, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_SmokeBall::
	setvar VAR_TEMP_1, ITEM_SMOKE_BALL
	setvar VAR_TEMP_2, 800
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeItem
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_MiracleSeed::
	setvar VAR_TEMP_1, ITEM_MIRACLE_SEED
	setvar VAR_TEMP_2, 1000
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeItem
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_Charcoal::
	setvar VAR_TEMP_1, ITEM_CHARCOAL
	setvar VAR_TEMP_2, 1000
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeItem
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_MysticWater::
	setvar VAR_TEMP_1, ITEM_MYSTIC_WATER
	setvar VAR_TEMP_2, 1000
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeItem
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_YellowFlute::
	setvar VAR_TEMP_1, ITEM_YELLOW_FLUTE
	setvar VAR_TEMP_2, 1600
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeItem
	end
