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
