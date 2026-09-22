@ Called before creating an enemy, playing the encounter sequence, or recording interaction.
EventScript_ConfirmApexEncounter::
    buffernumberstring STR_VAR_1, VAR_0x8004
    msgbox Text_ApexEncounterReadiness, MSGBOX_YESNO
    closemessage
    return

EventScript_DeclineApexEncounter::
    release
    end

Text_ApexEncounterReadiness::
    .string "This APEX POKéMON is Lv. {STR_VAR_1}.\p"
    .string "Once battle starts, you have\n"
    .string "only one chance.\p"
    .string "If you lose or flee, this\n"
    .string "encounter will be gone.\p"
    .string "You can prepare and return\n"
    .string "before starting the battle.\p"
    .string "Are you ready to challenge it?$"

EventScript_ApexEncounterShake::
    waitse
    delay 20
    playse SE_THUNDERSTORM_STOP
    setvar VAR_0x8004, 0  @ Vertical pan
    setvar VAR_0x8005, 3  @ Horizontal pan
    setvar VAR_0x8006, 4  @ Num shakes
    setvar VAR_0x8007, 2  @ Shake delay
    special ShakeScreen
    delay 30
    playse SE_THUNDERSTORM_STOP
    setvar VAR_0x8004, 0  @ Vertical pan
    setvar VAR_0x8005, 3  @ Horizontal pan
    setvar VAR_0x8006, 4  @ Num shakes
    setvar VAR_0x8007, 2  @ Shake delay
    special ShakeScreen
    delay 30
    delay 50
    waitse
    return

EventScript_RecordApexRumorWithFeedback::
    special RecordApexRumor
    copyvar VAR_0x8006, VAR_RESULT
    special TryRevealApexObjectOnCurrentMap
    goto_if_eq VAR_0x8006, 0, EventScript_RecordApexRumorWithFeedbackDone
    playse SE_PIN
    applymovement LOCALID_PLAYER, Common_Movement_ExclamationMark
    waitmovement 0
    goto_if_eq VAR_0x8006, 1, EventScript_ApexRumorRecorded
    msgbox Text_ApexRumorUpdated
    closemessage
    delay 40
    return

EventScript_ApexRumorRecorded::
    msgbox Text_ApexRumorRecorded
    closemessage
    delay 40
    return

EventScript_RecordApexRumorWithFeedbackDone::
    return

EventScript_UpdateSaffronRocketTakeoverState::
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, EventScript_SaffronRocketTakeoverCleared
	goto_if_unset FLAG_SAW_GIOVANNI_POKEMON_MANSION, EventScript_SaffronRocketTakeoverNotStarted
	clearflag FLAG_HIDE_SAFFRON_ROCKETS
	clearflag FLAG_HIDE_SILPH_ROCKETS
	setflag FLAG_HIDE_SAFFRON_CIVILIANS
	return

EventScript_SaffronRocketTakeoverNotStarted::
	setflag FLAG_HIDE_SAFFRON_ROCKETS
	setflag FLAG_HIDE_SILPH_ROCKETS
	clearflag FLAG_HIDE_SAFFRON_CIVILIANS
	return

EventScript_SaffronRocketTakeoverCleared::
	setflag FLAG_HIDE_SAFFRON_ROCKETS
	setflag FLAG_HIDE_SILPH_ROCKETS
	clearflag FLAG_HIDE_SAFFRON_CIVILIANS
	return

EventScript_RemoveStaticMon::
    fadescreen FADE_TO_BLACK
    removeobject VAR_LAST_TALKED
    fadescreen FADE_FROM_BLACK
    release
    end

EventScript_MonFlewAway::
    fadescreen FADE_TO_BLACK
    removeobject VAR_LAST_TALKED
    fadescreen FADE_FROM_BLACK
    bufferspeciesname STR_VAR_1, VAR_0x8004
    msgbox Text_MonFlewAway
    release
    end

Text_ApexRumorRecorded:
    .string "Rumour recorded.\p"
    .string "Compare three accounts in your\n"
    .string "LOGBOOK to find an APEX POKéMON.\p"
    .string "If you flee from its battle,\n"
    .string "it will leave for good.$"

Text_ApexRumorUpdated:
    .string "Rumour updated.$"

Text_OakApexTangrowth:
    .string "OAK: The POKéDEX signal was\n"
    .string "almost swallowed by the forest.\p"
    .string "So the old stories were not just\n"
    .string "stories.\p"
    .string "TANGROWTH may be instinct\n"
    .string "given root and vine.$"

Text_OakApexZapdos:
    .string "OAK: Extraordinary! That power\n"
    .string "reading did not fade after battle.\p"
    .string "ZAPDOS seems less like stored\n"
    .string "electricity.\p"
    .string "It is more like a source\n"
    .string "that renews itself.$"

Text_OakApexArticuno:
    .string "OAK: Your readings show a sudden\n"
    .string "collapse in temperature.\p"
    .string "ARTICUNO changes an environment\n"
    .string "all at once.\p"
    .string "Adaptation under pressure...\n"
    .string "remarkable.$"

Text_OakApexMewtwo:
    .string "OAK: The POKéDEX is struggling\n"
    .string "to classify that presence.\p"
    .string "MEWTWO's power is not natural\n"
    .string "instinct alone.\p"
    .string "It is a question we created\n"
    .string "but do not understand.$"

Text_OakApexOsscythe:
    .string "OAK: I received the tower record.\p"
    .string "OSSCYTHE's readings are bound to\n"
    .string "grief and memory.\p"
    .string "Loss can shape a life as surely\n"
    .string "as evolution.$"

Text_OakApexMoltres:
    .string "OAK: Those volcanic readings are\n"
    .string "alive with renewal.\p"
    .string "MOLTRES does not only burn.\p"
    .string "It returns spent energy\n"
    .string "to the world.$"

Text_OakApexMimeSr:
    .string "OAK: Fascinating. The barriers\n"
    .string "registered as a social pattern.\p"
    .string "MIME SR. seems to protect its\n"
    .string "group.\p"
    .string "It turns isolation into\n"
    .string "structure.$"

Text_OakApexAnnihilape:
    .string "OAK: The POKéDEX recorded extreme\n"
    .string "emotional force.\p"
    .string "ANNIHILAPE shows what happens\n"
    .string "when feeling outruns control.\p"
    .string "Be careful with such power.$"

@ Rival Apex investigations can be visited in any order.
EventScript_RivalApexReflection::
	setvar VAR_0x8008, 0
	call_if_ge VAR_MAP_SCENE_ROUTE11_RIVAL, 1, EventScript_RivalApexRememberMime
	call_if_ge VAR_MAP_SCENE_POKEMON_TOWER_1F, 1, EventScript_RivalApexRememberOsscythe
	call_if_ge VAR_FOREST_RIVAL_APEX, 1, EventScript_RivalApexRememberTangrowth
	goto_if_eq VAR_0x8008, 0, EventScript_RivalApexFirst
	goto_if_eq VAR_0x8008, 1, EventScript_RivalApexSecond
	msgbox Text_RivalApexThird
	return

EventScript_RivalApexFirst::
	msgbox Text_RivalApexFirst
	return

EventScript_RivalApexSecond::
	msgbox Text_RivalApexSecond
	return

EventScript_RivalApexRememberMime::
	addvar VAR_0x8008, 1
	bufferstring STR_VAR_1, Text_RivalApexMimeName
	return

EventScript_RivalApexRememberOsscythe::
	addvar VAR_0x8008, 1
	bufferstring STR_VAR_1, Text_RivalApexOsscytheName
	return

EventScript_RivalApexRememberTangrowth::
	addvar VAR_0x8008, 1
	bufferstring STR_VAR_1, Text_RivalApexTangrowthName
	return

Text_RivalApexMimeName::
	.string "MIME SR.$"
Text_RivalApexOsscytheName::
	.string "OSSCYTHE$"
Text_RivalApexTangrowthName::
	.string "TANGROWTH$"

Text_RivalApexFirst::
	.string "A POKéMON changing out here,\n"
	.string "without a TRAINER... Interesting.\p"
	.string "Don't look so surprised!\n"
	.string "I do more than collect BADGES.\p"
	.string "Let's compare the local accounts.\n"
	.string "There must be a reason it's here.$"

Text_RivalApexSecond::
	.string "Remember {STR_VAR_1}?\n"
	.string "Another unusual wild evolution.\p"
	.string "Different places, different forms.\n"
	.string "Maybe the habitat matters.\p"
	.string "That's a theory, not proof.\n"
	.string "Keep your eyes open, {PLAYER}!$"

Text_RivalApexThird::
	.string "MIME SR., OSSCYTHE, TANGROWTH...\n"
	.string "Three places, three unusual forms.\p"
	.string "These accounts point to habitats\n"
	.string "helping wild POKéMON change.\p"
	.string "My team grows by training with me.\n"
	.string "These POKéMON found another way.\p"
	.string "Gramps will want our notes.\n"
	.string "I'll let you write the neat copy!$"

