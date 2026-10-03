static const struct InGameTrade sInGameTrades[] = {
    [INGAME_TRADE_HAUNTER] =
    {
        .nickname = _("HAUNTER"),
        .species = SPECIES_HAUNTER,
        .ivs = {20, 15, 17, 24, 23, 22},
        .abilityNum = 0,
        .otId = 1985,
        .conditions = {5, 5, 5, 30, 5},
        .personality = 0x00009cae,
        .heldItem = ITEM_NONE,
        .mailNum = 255,
        .otName = _("REYLEY"),
        .otGender = MALE,
        .sheen = 10,
        .requestedSpecies = SPECIES_KADABRA
    },
    [INGAME_TRADE_KADABRA] =
    {
        .nickname = _("KADABRA"),
        .species = SPECIES_KADABRA,
        .ivs = {18, 17, 18, 22, 25, 21},
        .abilityNum = 0,
        .otId = 36728,
        .conditions = {5, 30, 5, 5, 5},
        .personality = 0x498a2e1d,
        .heldItem = ITEM_NONE,
        .mailNum = 255,
        .otName = _("DONTAE"),
        .otGender = MALE,
        .sheen = 10,
        .requestedSpecies = SPECIES_HAUNTER
    },
    [INGAME_TRADE_SLOWPOKE] =
    {
        .nickname = _("SLOWPOKE"),
        .species = SPECIES_SLOWPOKE,
        .ivs = {22, 18, 25, 19, 15, 22},
        .abilityNum = 0,
        .otId = 63184,
        .conditions = {5, 5, 5, 5, 30},
        .personality = 0x4c970b89,
        .heldItem = ITEM_KINGS_ROCK,
        .mailNum = 255,
        .otName = _("SAIGE"),
        .otGender = FEMALE,
        .sheen = 10,
        .requestedSpecies = SPECIES_NIDORAN_M
    },
    [INGAME_TRADE_RHYDON] =
    {
        .nickname = _("RHYDON"),
        .species = SPECIES_RHYDON,
        .ivs = {20, 25, 21, 24, 15, 20},
        .abilityNum = 0,
        .otId = 8810,
        .conditions = {30, 5, 5, 5, 5},
        .personality = 0x151943d7,
        .heldItem = ITEM_PROTECTOR,
        .mailNum = 255,
        .otName = _("GARETT"),
        .otGender = MALE,
        .sheen = 10,
        .requestedSpecies = SPECIES_RHYHORN
    },
    [INGAME_TRADE_MACHOKE] =
    {
        .nickname = _("MACHOKE"),
        .species = SPECIES_MACHOKE,
        .ivs = {22, 25, 18, 19, 22, 15},
        .abilityNum = 0,
        .otId = 13637,
        .conditions = {5, 5, 30, 5, 5},
        .personality = 0x00eeca15,
        .heldItem = ITEM_NONE,
        .mailNum = 255,
        .otName = _("TURNER"),
        .otGender = MALE,
        .sheen = 10,
        .requestedSpecies = SPECIES_NIDORINO
    },
    [INGAME_TRADE_ONIX] =
    {
        .nickname = _("ONIX"),
        .species = SPECIES_ONIX,
        .ivs = {24, 19, 21, 15, 23, 21},
        .abilityNum = 0,
        .otId = 1239,
        .conditions = {5, 5, 5, 5, 30},
        .personality = 0x451308ab,
        .heldItem = ITEM_METAL_COAT,
        .mailNum = 255,
        .otName = _("HADEN"),
        .otGender = MALE,
        .sheen = 10,
        .requestedSpecies = SPECIES_GOLDUCK

    },
    [INGAME_TRADE_ELECTABUZZ] =
    {
        .nickname = _("ELECTABUZZ"),
        .species = SPECIES_ELECTABUZZ,
        .ivs = {19, 16, 18, 25, 25, 19},
        .abilityNum = 1,
        .otId = 50298,
        .conditions = {30, 5, 5, 5, 5},
        .personality = 0x06341016,
        .heldItem = ITEM_ELECTIRIZER,
        .mailNum = 255,
        .otName = _("SURGE"),
        .otGender = MALE,
        .sheen = 10,
        .requestedSpecies = SPECIES_RAICHU
    },
    [INGAME_TRADE_SCYTHER] =
    {
        .nickname = _("SCYTHER"),
        .species = SPECIES_SCYTHER,
        .ivs = {22, 17, 25, 16, 23, 20},
        .abilityNum = 0,
        .otId = 60042,
        .conditions = {5, 5, 30, 5, 5},
        .personality = 0x5c77ecfa,
        .heldItem = ITEM_METAL_COAT,
        .mailNum = 255,
        .otName = _("NORMA"),
        .otGender = FEMALE,
        .sheen = 10,
        .requestedSpecies = SPECIES_VENONAT
    },
    [INGAME_TRADE_SEADRA] =
    {
        .nickname = _("SEADRA"),
        .species = SPECIES_SEADRA,
        .ivs = {24, 15, 22, 16, 23, 22},
        .abilityNum = 0,
        .otId = 9853,
        .conditions = {5, 5, 5, 5, 30},
        .personality = 0x482cac89,
        .heldItem = ITEM_DRAGON_SCALE,
        .mailNum = 255,
        .otName = _("ELYSSA"),
        .otGender = MALE,
        .sheen = 10,
        .requestedSpecies = SPECIES_PONYTA
    },
    [INGAME_TRADE_BROCK_GRAVELER] =
    {
        .nickname = _("GRAVELER"),
        .species = SPECIES_GRAVELER,
        .ivs = {22, 25, 18, 19, 22, 15},
        .abilityNum = 0,
        .otId = 01074,
        .conditions = {5, 5, 5, 5, 5},
        .personality = 0x00001074,
        .heldItem = ITEM_NONE,
        .mailNum = 255,
        .otName = _("BROCK"),
        .otGender = MALE,
        .sheen = 10,
        .requestedSpecies = SPECIES_RELICANTH
    },
    [INGAME_TRADE_MISTY_POLIWHIRL] =
    {
        .nickname = _("POLIWHIRL"),
        .species = SPECIES_POLIWHIRL,
        .ivs = {20, 22, 18, 25, 24, 21},
        .abilityNum = 0,
        .otId = 10013,
        .conditions = {5, 30, 5, 5, 5},
        .personality = 0x0000271d,
        .heldItem = ITEM_KINGS_ROCK,
        .mailNum = 255,
        .otName = _("MISTY"),
        .otGender = FEMALE,
        .sheen = 10,
        .requestedSpecies = SPECIES_GYARADOS
    },
    [INGAME_TRADE_MAGMAR] =
    {
        .nickname = _("MAGMAR"),
        .species = SPECIES_MAGMAR,
        .ivs = {22, 20, 22, 24, 22, 18},
        .abilityNum = 0,
        .otId = 50299,
        .conditions = {5, 5, 5, 5, 5},
        .personality = 0x00001074,
        .heldItem = ITEM_MAGMARIZER,
        .mailNum = 255,
        .otName = _("CLIFTON"),
        .otGender = MALE,
        .sheen = 10,
        .requestedSpecies = SPECIES_VULPIX
    },
    [INGAME_TRADE_DUSCLOPS] =
    {
        .nickname = _("DUSCLOPS"),
        .species = SPECIES_DUSCLOPS,
        .ivs = {22, 20, 22, 24, 22, 18},
        .abilityNum = 0,
        .otId = 50300,
        .conditions = {5, 5, 5, 5, 5},
        .personality = 0x00001074,
        .heldItem = ITEM_REAPER_CLOTH,
        .mailNum = 255,
        .otName = _("ELI"),
        .otGender = MALE,
        .sheen = 10,
        .requestedSpecies = SPECIES_HAUNTER
    },
    [INGAME_TRADE_ALOLAN_GRAVELER] =
    {
        .nickname = _("GRAVELER"),
        .species = SPECIES_GRAVELER_ALOLAN,
        .ivs = {22, 20, 22, 24, 22, 18},
        .abilityNum = 0,
        .otId = 50301,
        .conditions = {5, 5, 5, 5, 5},
        .personality = 0x00001074,
        .heldItem = ITEM_NONE,
        .mailNum = 255,
        .otName = _("FLINT"),
        .otGender = MALE,
        .sheen = 10,
        .requestedSpecies = SPECIES_GRAVELER_ALOLAN
    },
    [INGAME_TRADE_GOREBYSS] =
    {
        .nickname = _("CLAMPERL"),
        .species = SPECIES_CLAMPERL,
        .ivs = {22, 20, 22, 24, 22, 18},
        .abilityNum = 0,
        .otId = 50302,
        .conditions = {5, 5, 5, 5, 5},
        .personality = 0x00001074,
        .heldItem = ITEM_DEEP_SEA_SCALE,
        .mailNum = 255,
        .otName = _("NIA"),
        .otGender = FEMALE,
        .sheen = 10,
        .requestedSpecies = SPECIES_CLAMPERL
    },
    [INGAME_TRADE_HUNTAIL] =
    {
        .nickname = _("CLAMPERL"),
        .species = SPECIES_CLAMPERL,
        .ivs = {22, 20, 22, 24, 22, 18},
        .abilityNum = 0,
        .otId = 50303,
        .conditions = {5, 5, 5, 5, 5},
        .personality = 0x00001074,
        .heldItem = ITEM_DEEP_SEA_TOOTH,
        .mailNum = 255,
        .otName = _("FINN"),
        .otGender = MALE,
        .sheen = 10,
        .requestedSpecies = SPECIES_CLAMPERL
    },

};

static const u16 sInGameTradeMailMessages[][10] = {
    {
        EC_WORD_THAT_S,
        EC_WORD_A,
        EC_WORD_HEALTHY,
        EC_POKEMON(JYNX),
        EC_WORD_EXCL,
        EC_WORD_BE,
        EC_WORD_KIND,
        EC_WORD_TO,
        EC_WORD_IT
    }
};
