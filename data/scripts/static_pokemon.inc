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
    goto_if_eq VAR_RESULT, 0, EventScript_RecordApexRumorWithFeedbackDone
    playse SE_PIN
    applymovement LOCALID_PLAYER, Common_Movement_ExclamationMark
    waitmovement 0
    goto_if_eq VAR_RESULT, 1, EventScript_ApexRumorRecorded
    msgbox Text_ApexRumorUpdated
    return

EventScript_ApexRumorRecorded::
    msgbox Text_ApexRumorRecorded
    return

EventScript_RecordApexRumorWithFeedbackDone::
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
    .string "Rumour recorded.$"

Text_ApexRumorUpdated:
    .string "Rumour updated.$"

Text_OakApexTangrowth:
    .string "OAK: The POKéDEX signal was\n"
    .string "almost swallowed by the forest.\p"
    .string "So the old stories were not just\n"
    .string "stories. TANGROWTH may be instinct\n"
    .string "given root and vine.$"

Text_OakApexZapdos:
    .string "OAK: Extraordinary! That power\n"
    .string "reading did not fade after battle.\p"
    .string "ZAPDOS seems less like stored\n"
    .string "electricity and more like a source\n"
    .string "that renews itself.$"

Text_OakApexArticuno:
    .string "OAK: Your readings show a sudden\n"
    .string "collapse in temperature.\p"
    .string "ARTICUNO changes an environment\n"
    .string "all at once. Adaptation under\n"
    .string "pressure...remarkable.$"

Text_OakApexMewtwo:
    .string "OAK: The POKéDEX is struggling\n"
    .string "to classify that presence.\p"
    .string "MEWTWO's power is not natural\n"
    .string "instinct alone. It is a question\n"
    .string "we created but do not understand.$"

Text_OakApexOsscythe:
    .string "OAK: I received the tower record.\p"
    .string "OSSCYTHE's readings are bound to\n"
    .string "grief and memory.\p"
    .string "Loss can shape a life as surely\n"
    .string "as evolution.$"

Text_OakApexMoltres:
    .string "OAK: Those volcanic readings are\n"
    .string "alive with renewal.\p"
    .string "MOLTRES does not only burn.\n"
    .string "It returns spent energy to the\n"
    .string "world.$"

Text_OakApexMimeSr:
    .string "OAK: Fascinating. The barriers\n"
    .string "registered as a social pattern.\p"
    .string "MIME SR. seems to protect its\n"
    .string "group by turning isolation into\n"
    .string "structure.$"

Text_OakApexAnnihilape:
    .string "OAK: The POKéDEX recorded extreme\n"
    .string "emotional force.\p"
    .string "ANNIHILAPE shows what happens\n"
    .string "when feeling outruns control.\p"
    .string "Be careful with such power.$"
