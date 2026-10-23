SafariZone_Center_RestHouse_MapScripts::
	.byte 0

SafariZone_Center_RestHouse_EventScript_Sara::
	msgbox SafariZone_Center_Text_WhereDidErikGo, MSGBOX_NPC
	end

SafariZone_Center_RestHouse_EventScript_Scientist::
    lock
    faceplayer
    msgbox SafariZone_Center_RestHouse_Text_GalarImports, MSGBOX_DEFAULT
    pokemart SafariZone_Center_RestHouse_GalarItems
    msgbox SafariZone_Center_RestHouse_Text_GalarFarewell, MSGBOX_DEFAULT
    release
    end

    .align 2
SafariZone_Center_RestHouse_GalarItems::
    .2byte ITEM_GALARICA_CUFF
    .2byte ITEM_GALARICA_WREATH
    .2byte ITEM_NONE
    release
    end

SafariZone_Center_RestHouse_Text_GalarImports::
    .string "Our GALAR imports have settled in.\n"
    .string "We brought these woven twigs too.\p"
    .string "A CUFF changes GALARIAN SLOWPOKE\n"
    .string "into SLOWBRO; a WREATH, SLOWKING.$"

SafariZone_Center_RestHouse_Text_GalarFarewell::
    .string "Some caught here evolve\n"
    .string "differently, even after leaving our\l"
    .string "habitats.\p"
    .string "PIKACHU becomes ALOLAN RAICHU with\n"
    .string "a THUNDER STONE.\p"
    .string "EXEGGCUTE becomes ALOLAN\n"
    .string "EXEGGUTOR with a LEAF STONE.\p"
    .string "CUBONE becomes ALOLAN MAROWAK\n"
    .string "from level 28, at night.\p"
    .string "KOFFING becomes GALARIAN WEEZING\n"
    .string "from level 35.\p"
    .string "Our PALDEAN WOOPER evolve into\n"
    .string "CLODSIRE from level 20.\p"
    .string "MIME JR. caught here become\n"
    .string "GALARIAN MR. MIME as they grow\l"
    .string "knowing MIMIC.$"
