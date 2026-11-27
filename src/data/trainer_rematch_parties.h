// Separate teams for rematches that otherwise reuse the first-encounter trainer ID.
const struct Trainer gTrainerRematchOverrides[NUM_TRAINERS] = {
    [TRAINER_ACE_TRAINER_ALEXA_2] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_RematchVictoryRoadAceAlexa),
    },
    [TRAINER_AROMA_LADY_DAPHNE] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_GymDaphne),
    },
    [TRAINER_AROMA_LADY_FLORA] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_GymFlora),
    },
    [TRAINER_AROMA_LADY_LAUREL] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_GymLaurel),
    },
    [TRAINER_AROMA_LADY_VIOLA] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_GymViola),
    },
    [TRAINER_BIKER_LUKAS_2] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_CueBallZeek),
    },
    [TRAINER_BIRD_KEEPER_BECK] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_RematchFuchsiaNinja),
    },
    [TRAINER_BLACK_BELT_DAISUKE_2] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_RematchVictoryRoadBlackBelt),
    },
    [TRAINER_BUG_CATCHER_KENT_2] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_RematchMtMoonRuinManiacKent),
    },
    [TRAINER_BURGLAR_SIMON] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_BurglarSimon),
    },
    [TRAINER_CAMPER_ETHAN] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_RematchRockTunnelBlackBelt),
    },
    [TRAINER_CHANNELER_KARINA_2] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_RematchMartialArtsExpert),
    },
    [TRAINER_CUE_BALL_COREY_2] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_CueBallCorey),
    },
    [TRAINER_ELECTRICIAN_OTIS] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_ElectricianOtis),
    },
    [TRAINER_ELECTRICIAN_WADE] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_ElectricianWade),
    },
    [TRAINER_ENGINEER_BAILY] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_GymAdjacentEngineerA),
    },
    [TRAINER_EXPERT_IVO] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_ExpertIvo),
    },
    [TRAINER_HIKER_LENNY_2] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_RematchRockTunnelHiker),
    },
    [TRAINER_HIKER_OLIVER_2] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_RematchRockTunnelHiker),
    },
    [TRAINER_LASS_JULIA] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_RematchRockTunnelHiker),
    },
    [TRAINER_LASS_NORA] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_RematchRockTunnelHiker),
    },
    [TRAINER_NINJA_KAYDEN] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_NinjaKayden),
    },
    [TRAINER_POKEMANIAC_ASHTON_2] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_RematchRockTunnelBlackBelt),
    },
    [TRAINER_POWER_PLANT_ELECTRICIAN_LUCA] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_PowerPlantElectricianLuca),
    },
    [TRAINER_PSYCHIC_ALDEN] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_GymAlden),
    },
    [TRAINER_PSYCHIC_LYRA] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_GymLyra),
    },
    [TRAINER_PSYCHIC_MIRA] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_GymMira),
    },
    [TRAINER_PSYCHIC_ORIN] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_GymOrin),
    },
    [TRAINER_PSYCHIC_SELENE] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_GymSelene),
    },
    [TRAINER_PSYCHIC_SILAS] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_GymSilas),
    },
    [TRAINER_ROUTE11_RUIN_MANIAC_MARCOS] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_RematchMtMoonRuinManiac),
    },
    [TRAINER_RUIN_MANIAC_VINCENT_2] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_RematchVictoryRoadRuinManiac),
    },
    [TRAINER_UNDERGROUND_POKEFAN_ELLIOT] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_UndergroundGamblerElliot),
    },
    [TRAINER_YOUNGSTER_EDDIE] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_RematchMtMoonRuinManiac),
    },
    [TRAINER_YOUNGSTER_RUBY] = {
        .party = NO_ITEM_DEFAULT_MOVES(sParty_YoungsterRuby),
    },
};
