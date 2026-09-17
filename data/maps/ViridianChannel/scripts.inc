ViridianChannel_MapScripts::
	.byte 0

ViridianChannel_EventScript_Fen::
    trainerbattle_single TRAINER_CHANNEL_FEN, ViridianChannel_Text_FenIntro, ViridianChannel_Text_FenDefeat
    specialvar VAR_RESULT, ShouldTryRematchBattle
    goto_if_eq VAR_RESULT, TRUE, ViridianChannel_EventScript_FenRematch
    msgbox ViridianChannel_Text_FenAfter, MSGBOX_AUTOCLOSE
    end

ViridianChannel_EventScript_FenRematch::
    trainerbattle_rematch TRAINER_CHANNEL_FEN, ViridianChannel_Text_FenRematch, ViridianChannel_Text_FenDefeat
    msgbox ViridianChannel_Text_FenAfter, MSGBOX_AUTOCLOSE
    end

ViridianChannel_EventScript_Harlan::
    trainerbattle_single TRAINER_CHANNEL_HARLAN, ViridianChannel_Text_HarlanIntro, ViridianChannel_Text_HarlanDefeat
    specialvar VAR_RESULT, ShouldTryRematchBattle
    goto_if_eq VAR_RESULT, TRUE, ViridianChannel_EventScript_HarlanRematch
    msgbox ViridianChannel_Text_HarlanAfter, MSGBOX_AUTOCLOSE
    end

ViridianChannel_EventScript_HarlanRematch::
    trainerbattle_rematch TRAINER_CHANNEL_HARLAN, ViridianChannel_Text_HarlanRematch, ViridianChannel_Text_HarlanDefeat
    msgbox ViridianChannel_Text_HarlanAfter, MSGBOX_AUTOCLOSE
    end

ViridianChannel_EventScript_Coral::
    trainerbattle_single TRAINER_CHANNEL_CORAL, ViridianChannel_Text_CoralIntro, ViridianChannel_Text_CoralDefeat
    specialvar VAR_RESULT, ShouldTryRematchBattle
    goto_if_eq VAR_RESULT, TRUE, ViridianChannel_EventScript_CoralRematch
    msgbox ViridianChannel_Text_CoralAfter, MSGBOX_AUTOCLOSE
    end

ViridianChannel_EventScript_CoralRematch::
    trainerbattle_rematch TRAINER_CHANNEL_CORAL, ViridianChannel_Text_CoralRematch, ViridianChannel_Text_CoralDefeat
    msgbox ViridianChannel_Text_CoralAfter, MSGBOX_AUTOCLOSE
    end
