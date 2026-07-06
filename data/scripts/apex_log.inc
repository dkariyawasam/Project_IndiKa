ViridianCity_School_EventScript_PokemonJournal::
	lock
	faceplayer
	apexlog APEX_LOG_PERSON_OAK, 3
	apexlog APEX_LOG_PERSON_DAISY, 3
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PokemonJournal_Text_SpecialFeatureProfOak
	release
	end

EventScript_PokemonJournalUnused1::
	lock
	faceplayer
	apexlog APEX_LOG_PERSON_DAISY, 1
	apexlog APEX_LOG_PERSON_OAK, 4
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PokemonJournal_Text_SpecialFeatureProfOak
	release
	end

EventScript_PokemonJournalUnused2::
	lock
	faceplayer
	apexlog APEX_LOG_PERSON_DAISY, 4
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PokemonJournal_Text_SpecialFeatureProfOak
	release
	end

Route4_EventScript_Boy::
	lock
	faceplayer
	apexlog APEX_LOG_PERSON_BROCK, 3
	msgbox Route4_Text_PeopleLikeAndRespectBrock
	release
	end

MtMoon_1F_EventScript_BaldingMan::
	lock
	faceplayer
	msgbox MtMoon_1F_Text_AngryPrimalPokemon
	setvar VAR_0x8004, SUB_QUEST_APEX_ANNIHILAPE
	setvar VAR_0x8005, 0
	call EventScript_RecordApexRumorWithFeedback
	release
	end

PewterCity_Museum_1F_EventScript_PokemonJournalBrock::
	lockall
	apexlog APEX_LOG_PERSON_BROCK, 5
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PokemonJournal_Text_SpecialFeatureBrock
	releaseall
	end

Route20_EventScript_Camper::
	lock
	faceplayer
	apexlog APEX_LOG_PERSON_MISTY, 3
	msgbox Route20_Text_SeafoamUsedToBeVolcanic
	setvar VAR_0x8004, SUB_QUEST_APEX_ARTICUNO
	setvar VAR_0x8005, 0
	call EventScript_RecordApexRumorWithFeedback
	release
	end

Route25_EventScript_Beauty::
	lock
	faceplayer
	apexlog APEX_LOG_PERSON_MISTY, 4
	msgbox Route25_Text_MistyHighHopesAboutThisPlace
	release
	end

CeruleanCity_PokemonCenter_1F_EventScript_PokemonJournalMisty::
	lock
	faceplayer
	apexlog APEX_LOG_PERSON_MISTY, 5
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PokemonJournal_Text_SpecialFeatureMisty
	release
	end

VermilionCity_PokemonCenter_1F_EventScript_PokemonJournalLtSurge::
	lockall
	apexlog APEX_LOG_PERSON_LTSURGE, 5
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PokemonJournal_Text_SpecialFeatureLtSurge
	releaseall
	end

CeladonCity_Condominiums_2F_EventScript_PokemonJournalErika::
	lock
	faceplayer
	apexlog APEX_LOG_PERSON_ERIKA, 5
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PokemonJournal_Text_SpecialFeatureErika
	release
	end

FuchsiaCity_WardensHouse_EventScript_PokemonJournalKoga::
	lock
	faceplayer
	apexlog APEX_LOG_PERSON_KOGA, 4
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PokemonJournal_Text_SpecialFeatureKoga
	release
	end

SafariZone_North_RestHouse_EventScript_BaldingMan::
	lock
	faceplayer
	apexlog APEX_LOG_PERSON_KOGA, 5
	msgbox SafariZone_West_Text_KogaPatrolsSafariEverySoOften
	release
	end

SaffronCity_PokemonCenter_1F_EventScript_PokemonJournalSabrina::
	lock
	faceplayer
	apexlog APEX_LOG_PERSON_SABRINA, 4
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PokemonJournal_Text_SpecialFeatureSabrina
	release
	end

CinnabarIsland_Gym_EventScript_BlaineFujiPhoto::
	lockall
	apexlog APEX_LOG_PERSON_BLAINE, 4
	apexlog APEX_LOG_PERSON_MRFUJI, 4
	goto_if_unset FLAG_DEFEATED_BLAINE, CinnabarIsland_Gym_EventScript_PorygonMachineLocked
	goto_if_set FLAG_GOT_PORYGON_FROM_BLAINE, CinnabarIsland_Gym_EventScript_PorygonMachineAlreadyGenerated
	msgbox CinnabarIsland_Gym_Text_PorygonMachineInitialize, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, CinnabarIsland_Gym_EventScript_PorygonMachineCancel
	msgbox CinnabarIsland_Gym_Text_PorygonMachineRules
	msgbox CinnabarIsland_Gym_Text_PorygonMachineQuestionStablePattern, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, CinnabarIsland_Gym_EventScript_PorygonMachineQuestionNoise
	goto CinnabarIsland_Gym_EventScript_PorygonMachineFail

CinnabarIsland_Gym_EventScript_PorygonMachineQuestionNoise::
	msgbox CinnabarIsland_Gym_Text_PorygonMachineParameterAccepted
	msgbox CinnabarIsland_Gym_Text_PorygonMachineQuestionRandomNoise, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, CinnabarIsland_Gym_EventScript_PorygonMachineQuestionProjection
	goto CinnabarIsland_Gym_EventScript_PorygonMachineFail

CinnabarIsland_Gym_EventScript_PorygonMachineQuestionProjection::
	msgbox CinnabarIsland_Gym_Text_PorygonMachineParameterAccepted
	msgbox CinnabarIsland_Gym_Text_PorygonMachineQuestionProjection, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, CinnabarIsland_Gym_EventScript_PorygonMachineGenerate
	goto CinnabarIsland_Gym_EventScript_PorygonMachineFail

CinnabarIsland_Gym_EventScript_PorygonMachineGenerate::
	msgbox CinnabarIsland_Gym_Text_PorygonMachineParameterAccepted
	fadescreen FADE_TO_WHITE
	playse SE_UNLOCK
	waitse
	fadescreen FADE_FROM_WHITE
	msgbox CinnabarIsland_Gym_Text_PorygonMachineGenerated
	setvar VAR_0x8004, SPECIES_PORYGON
	setvar VAR_0x8005, 26
	special DoGeneratedPokemonReceiveScene
	waitstate
	setvar VAR_TEMP_1, SPECIES_PORYGON
	givemon SPECIES_PORYGON, 26
	goto_if_eq VAR_RESULT, 0, CinnabarIsland_Gym_EventScript_PorygonMachineReceivedParty
	goto_if_eq VAR_RESULT, 1, CinnabarIsland_Gym_EventScript_PorygonMachineReceivedPC
	goto_if_eq VAR_RESULT, 2, CinnabarIsland_Gym_EventScript_PorygonMachineNoRoom
	releaseall
	end

CinnabarIsland_Gym_EventScript_PorygonMachineReceivedParty::
	setflag FLAG_GOT_PORYGON_FROM_BLAINE
	logbookmenu LOGBOOK_MENU_UNLOCK_QUEST, QUEST_THE_NATURE_OF_EVOLUTION
	logbookmenu LOGBOOK_MENU_SET_ACTIVE, QUEST_THE_NATURE_OF_EVOLUTION
	sublogbookmenu LOGBOOK_MENU_UNLOCK_QUEST, QUEST_THE_NATURE_OF_EVOLUTION, SUB_QUEST_EVOLUTION_THROUGH_DESIGN
	call EventScript_GetGiftMonPartySlot
	bufferspeciesname STR_VAR_1, SPECIES_PORYGON
	playfanfare MUS_LEVEL_UP
	message Text_PlayerObtainedTheMon
	waitmessage
	waitfanfare
	msgbox Text_GiveNicknameToThisMon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, CinnabarIsland_Gym_EventScript_PorygonMachineEnd
	call EventScript_ChangePokemonNickname
	goto CinnabarIsland_Gym_EventScript_PorygonMachineEnd
	end

CinnabarIsland_Gym_EventScript_PorygonMachineReceivedPC::
	setflag FLAG_GOT_PORYGON_FROM_BLAINE
	logbookmenu LOGBOOK_MENU_UNLOCK_QUEST, QUEST_THE_NATURE_OF_EVOLUTION
	logbookmenu LOGBOOK_MENU_SET_ACTIVE, QUEST_THE_NATURE_OF_EVOLUTION
	sublogbookmenu LOGBOOK_MENU_UNLOCK_QUEST, QUEST_THE_NATURE_OF_EVOLUTION, SUB_QUEST_EVOLUTION_THROUGH_DESIGN
	bufferspeciesname STR_VAR_1, SPECIES_PORYGON
	playfanfare MUS_LEVEL_UP
	message Text_PlayerObtainedTheMon
	waitmessage
	waitfanfare
	msgbox Text_GiveNicknameToThisMon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, CinnabarIsland_Gym_EventScript_PorygonMachineTransferToPC
	call EventScript_NameReceivedBoxMon
	goto CinnabarIsland_Gym_EventScript_PorygonMachineTransferToPC
	end

CinnabarIsland_Gym_EventScript_PorygonMachineTransferToPC::
	call EventScript_TransferredToPC
	goto CinnabarIsland_Gym_EventScript_PorygonMachineEnd
	end

CinnabarIsland_Gym_EventScript_PorygonMachineEnd::
	releaseall
	end

CinnabarIsland_Gym_EventScript_PorygonMachineFail::
	playse SE_BOO
	msgbox CinnabarIsland_Gym_Text_PorygonMachineBuildFailed
	releaseall
	end

CinnabarIsland_Gym_EventScript_PorygonMachineNoRoom::
	msgbox Text_NoMoreRoomForPokemon
	releaseall
	end

CinnabarIsland_Gym_EventScript_PorygonMachineCancel::
	msgbox CinnabarIsland_Gym_Text_PorygonMachineCancel
	releaseall
	end

CinnabarIsland_Gym_EventScript_PorygonMachineLocked::
	msgbox CinnabarIsland_Gym_Text_PhotoOfBlaineAndFuji
	releaseall
	end

CinnabarIsland_Gym_EventScript_PorygonMachineAlreadyGenerated::
	msgbox CinnabarIsland_Gym_Text_PorygonMachineAlreadyGenerated
	releaseall
	end

SaffronCity_PokemonTrainerFanClub_EventScript_PokemonJournalBruno::
	lock
	faceplayer
	apexlog APEX_LOG_PERSON_BRUNO, 2
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PokemonJournal_Text_SpecialFeatureBruno
	release
	end

IndigoPlateau_PokemonCenter_1F_EventScript_BlackBelt::
	lock
	faceplayer
	apexlog APEX_LOG_PERSON_AGATHA, 1
	apexlog APEX_LOG_PERSON_AGATHA, 2
	msgbox IndigoPlateau_PokemonCenter_1F_Text_AgathaWhuppedUs
	release
	end

EventScript_LanceUnused::
	lock
	faceplayer
	apexlog APEX_LOG_PERSON_LANCE, 0
	msgbox gApexLogFlavorText_Lance0
	release
	end

SaffronCity_EventScript_CrushGirlUnused::
	lock
	faceplayer
	apexlog APEX_LOG_PERSON_LANCE, 1
	msgbox SaffronCity_Text_HowCanClubNotRecognizeLance
	release
	end

SaffronCity_EventScript_CrushGirl::
	lock
	faceplayer
	apexlog APEX_LOG_PERSON_LANCE, 2
	msgbox SaffronCity_Text_HowCanClubNotRecognizeLance
	release
	end

CeladonCity_DepartmentStore_2F_EventScript_Woman::
	lock
	faceplayer
	apexlog APEX_LOG_PERSON_LANCE, 3
	msgbox CeladonCity_DepartmentStore_2F_Text_LanceComesToBuyCapes
	release
	end

IndigoPlateau_PokemonCenter_1F_EventScript_PokemonJournal::
	lock
	faceplayer
	apexlog APEX_LOG_PERSON_LANCE, 4
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PokemonJournal_Text_SpecialFeatureLance
	release
	end

IndigoPlateau_PokemonCenter_1F_EventScript_AceTrainerM::
	lock
	faceplayer
	apexlog APEX_LOG_PERSON_LANCE, 5
	msgbox IndigoPlateau_PokemonCenter_1F_Text_LancesCousinGymLeaderFarAway
	release
	end

LavenderTown_PokemonCenter_1F_EventScript_BaldingMan::
	lock
	faceplayer
	apexlog APEX_LOG_PERSON_MRFUJI, 3
	msgbox LavenderTown_PokemonCenter_1F_Text_HearMrFujiNotFromAroundHere
	release
	end

CinnabarIsland_PokemonCenter_1F_EventScript_PokemonJournalMrFuji::
	lock
	faceplayer
	apexlog APEX_LOG_PERSON_MRFUJI, 5
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PokemonJournal_Text_SpecialFeatureMrFuji
	release
	end

SilphCo_5F_EventScript_Scientist::
	lock
	faceplayer
	apexlog APEX_LOG_PERSON_GIOVANNI, 1
	msgbox SilphCo_5F_Text_RocketBossLookingForStrongMons
	release
	end

SilphCo_8F_EventScript_Scientist::
	lock
	faceplayer
	apexlog APEX_LOG_PERSON_GIOVANNI, 2
	msgbox SilphCo_8F_Text_ToRocketBossMonsAreTools
	release
	end
