const struct PokedexEntry gPokedexEntries[] =
{
    [NATIONAL_DEX_NONE] =
    {
        .categoryName = _("UNKNOWN"),
        .height = 0,
        .weight = 0,
        .description = gDummyPokedexText,
        .unusedDescription = gDummyPokedexTextUnused,
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 256,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_BULBASAUR] =
    {
        .categoryName = _("SEED"),
        .height = 7,
        .weight = 69,
        .description = gBulbasaurPokedexText,
        .unusedDescription = gBulbasaurPokedexTextUnused,
        .pokemonScale = 439,
        .pokemonOffset = 14,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_IVYSAUR] =
    {
        .categoryName = _("SEED"),
        .height = 10,
        .weight = 130,
        .description = gIvysaurPokedexText,
        .unusedDescription = gIvysaurPokedexTextUnused,
        .pokemonScale = 391,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_VENUSAUR] =
    {
        .categoryName = _("SEED"),
        .height = 20,
        .weight = 1000,
        .description = gVenusaurPokedexText,
        .unusedDescription = gVenusaurPokedexTextUnused,
        .pokemonScale = 328,
        .pokemonOffset = 1,
        .trainerScale = 446,
        .trainerOffset = 5,
    },

    [NATIONAL_DEX_CHARMANDER] =
    {
        .categoryName = _("LIZARD"),
        .height = 6,
        .weight = 85,
        .description = gCharmanderPokedexText,
        .unusedDescription = gCharmanderPokedexTextUnused,
        .pokemonScale = 605,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_CHARMELEON] =
    {
        .categoryName = _("FLAME"),
        .height = 11,
        .weight = 190,
        .description = gCharmeleonPokedexText,
        .unusedDescription = gCharmeleonPokedexTextUnused,
        .pokemonScale = 398,
        .pokemonOffset = 8,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_CHARIZARD] =
    {
        .categoryName = _("FLAME"),
        .height = 17,
        .weight = 905,
        .description = gCharizardPokedexText,
        .unusedDescription = gCharizardPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 1,
        .trainerScale = 358,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_SQUIRTLE] =
    {
        .categoryName = _("TINY TURTLE"),
        .height = 5,
        .weight = 90,
        .description = gSquirtlePokedexText,
        .unusedDescription = gSquirtlePokedexTextUnused,
        .pokemonScale = 670,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_WARTORTLE] =
    {
        .categoryName = _("TURTLE"),
        .height = 10,
        .weight = 225,
        .description = gWartortlePokedexText,
        .unusedDescription = gWartortlePokedexTextUnused,
        .pokemonScale = 428,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_BLASTOISE] =
    {
        .categoryName = _("SHELLFISH"),
        .height = 16,
        .weight = 855,
        .description = gBlastoisePokedexText,
        .unusedDescription = gBlastoisePokedexTextUnused,
        .pokemonScale = 328,
        .pokemonOffset = 2,
        .trainerScale = 363,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_CATERPIE] =
    {
        .categoryName = _("WORM"),
        .height = 3,
        .weight = 29,
        .description = gCaterpiePokedexText,
        .unusedDescription = gCaterpiePokedexTextUnused,
        .pokemonScale = 993,
        .pokemonOffset = 19,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_METAPOD] =
    {
        .categoryName = _("COCOON"),
        .height = 7,
        .weight = 99,
        .description = gMetapodPokedexText,
        .unusedDescription = gMetapodPokedexTextUnused,
        .pokemonScale = 332,
        .pokemonOffset = 14,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_BUTTERFREE] =
    {
        .categoryName = _("BUTTERFLY"),
        .height = 11,
        .weight = 320,
        .description = gButterfreePokedexText,
        .unusedDescription = gButterfreePokedexTextUnused,
        .pokemonScale = 364,
        .pokemonOffset = 7,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_WEEDLE] =
    {
        .categoryName = _("HAIRY BUG"),
        .height = 3,
        .weight = 32,
        .description = gWeedlePokedexText,
        .unusedDescription = gWeedlePokedexTextUnused,
        .pokemonScale = 869,
        .pokemonOffset = 19,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_KAKUNA] =
    {
        .categoryName = _("COCOON"),
        .height = 6,
        .weight = 100,
        .description = gKakunaPokedexText,
        .unusedDescription = gKakunaPokedexTextUnused,
        .pokemonScale = 543,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_BEEDRILL] =
    {
        .categoryName = _("POISON BEE"),
        .height = 10,
        .weight = 295,
        .description = gBeedrillPokedexText,
        .unusedDescription = gBeedrillPokedexTextUnused,
        .pokemonScale = 428,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_PIDGEY] =
    {
        .categoryName = _("TINY BIRD"),
        .height = 3,
        .weight = 18,
        .description = gPidgeyPokedexText,
        .unusedDescription = gPidgeyPokedexTextUnused,
        .pokemonScale = 1148,
        .pokemonOffset = 19,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_PIDGEOTTO] =
    {
        .categoryName = _("BIRD"),
        .height = 11,
        .weight = 300,
        .description = gPidgeottoPokedexText,
        .unusedDescription = gPidgeottoPokedexTextUnused,
        .pokemonScale = 440,
        .pokemonOffset = 8,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_PIDGEOT] =
    {
        .categoryName = _("BIRD"),
        .height = 15,
        .weight = 395,
        .description = gPidgeotPokedexText,
        .unusedDescription = gPidgeotPokedexTextUnused,
        .pokemonScale = 379,
        .pokemonOffset = 2,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_RATTATA] =
    {
        .categoryName = _("MOUSE"),
        .height = 3,
        .weight = 35,
        .description = gRattataPokedexText,
        .unusedDescription = gRattataPokedexTextUnused,
        .pokemonScale = 1086,
        .pokemonOffset = 19,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_RATICATE] =
    {
        .categoryName = _("MOUSE"),
        .height = 7,
        .weight = 185,
        .description = gRaticatePokedexText,
        .unusedDescription = gRaticatePokedexTextUnused,
        .pokemonScale = 465,
        .pokemonOffset = 11,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SPEAROW] =
    {
        .categoryName = _("TINY BIRD"),
        .height = 3,
        .weight = 20,
        .description = gSpearowPokedexText,
        .unusedDescription = gSpearowPokedexTextUnused,
        .pokemonScale = 1148,
        .pokemonOffset = 19,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_FEAROW] =
    {
        .categoryName = _("BEAK"),
        .height = 12,
        .weight = 380,
        .description = gFearowPokedexText,
        .unusedDescription = gFearowPokedexTextUnused,
        .pokemonScale = 496,
        .pokemonOffset = 6,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_EKANS] =
    {
        .categoryName = _("SNAKE"),
        .height = 20,
        .weight = 69,
        .description = gEkansPokedexText,
        .unusedDescription = gEkansPokedexTextUnused,
        .pokemonScale = 256,
        .pokemonOffset = 6,
        .trainerScale = 507,
        .trainerOffset = 7,
    },

    [NATIONAL_DEX_ARBOK] =
    {
        .categoryName = _("COBRA"),
        .height = 35,
        .weight = 650,
        .description = gArbokPokedexText,
        .unusedDescription = gArbokPokedexTextUnused,
        .pokemonScale = 323,
        .pokemonOffset = 0,
        .trainerScale = 714,
        .trainerOffset = 12,
    },

    [NATIONAL_DEX_PIKACHU] =
    {
        .categoryName = _("MOUSE"),
        .height = 4,
        .weight = 60,
        .description = gPikachuPokedexText,
        .unusedDescription = gPikachuPokedexTextUnused,
        .pokemonScale = 1140,
        .pokemonOffset = 18,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_RAICHU] =
    {
        .categoryName = _("MOUSE"),
        .height = 8,
        .weight = 300,
        .description = gRaichuPokedexText,
        .unusedDescription = gRaichuPokedexTextUnused,
        .pokemonScale = 698,
        .pokemonOffset = 11,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SANDSHREW] =
    {
        .categoryName = _("MOUSE"),
        .height = 6,
        .weight = 120,
        .description = gSandshrewPokedexText,
        .unusedDescription = gSandshrewPokedexTextUnused,
        .pokemonScale = 574,
        .pokemonOffset = 14,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SANDSLASH] =
    {
        .categoryName = _("MOUSE"),
        .height = 10,
        .weight = 295,
        .description = gSandslashPokedexText,
        .unusedDescription = gSandslashPokedexTextUnused,
        .pokemonScale = 465,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_NIDORAN_F] =
    {
        .categoryName = _("POISON PIN"),
        .height = 4,
        .weight = 70,
        .description = gNidoranFPokedexText,
        .unusedDescription = gNidoranFPokedexTextUnused,
        .pokemonScale = 745,
        .pokemonOffset = 18,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_NIDORINA] =
    {
        .categoryName = _("POISON PIN"),
        .height = 8,
        .weight = 200,
        .description = gNidorinaPokedexText,
        .unusedDescription = gNidorinaPokedexTextUnused,
        .pokemonScale = 489,
        .pokemonOffset = 12,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_NIDOQUEEN] =
    {
        .categoryName = _("DRILL"),
        .height = 13,
        .weight = 600,
        .description = gNidoqueenPokedexText,
        .unusedDescription = gNidoqueenPokedexTextUnused,
        .pokemonScale = 408,
        .pokemonOffset = 4,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_NIDORAN_M] =
    {
        .categoryName = _("POISON PIN"),
        .height = 5,
        .weight = 90,
        .description = gNidoranMPokedexText,
        .unusedDescription = gNidoranMPokedexTextUnused,
        .pokemonScale = 652,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_NIDORINO] =
    {
        .categoryName = _("POISON PIN"),
        .height = 9,
        .weight = 195,
        .description = gNidorinoPokedexText,
        .unusedDescription = gNidorinoPokedexTextUnused,
        .pokemonScale = 486,
        .pokemonOffset = 10,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_NIDOKING] =
    {
        .categoryName = _("DRILL"),
        .height = 14,
        .weight = 620,
        .description = gNidokingPokedexText,
        .unusedDescription = gNidokingPokedexTextUnused,
        .pokemonScale = 386,
        .pokemonOffset = 3,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_CLEFAIRY] =
    {
        .categoryName = _("FAIRY"),
        .height = 6,
        .weight = 75,
        .description = gClefairyPokedexText,
        .unusedDescription = gClefairyPokedexTextUnused,
        .pokemonScale = 512,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_CLEFABLE] =
    {
        .categoryName = _("FAIRY"),
        .height = 13,
        .weight = 400,
        .description = gClefablePokedexText,
        .unusedDescription = gClefablePokedexTextUnused,
        .pokemonScale = 337,
        .pokemonOffset = 4,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_VULPIX] =
    {
        .categoryName = _("FOX"),
        .height = 6,
        .weight = 99,
        .description = gVulpixPokedexText,
        .unusedDescription = gVulpixPokedexTextUnused,
        .pokemonScale = 605,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_NINETALES] =
    {
        .categoryName = _("FOX"),
        .height = 11,
        .weight = 199,
        .description = gNinetalesPokedexText,
        .unusedDescription = gNinetalesPokedexTextUnused,
        .pokemonScale = 474,
        .pokemonOffset = 7,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_JIGGLYPUFF] =
    {
        .categoryName = _("BALLOON"),
        .height = 5,
        .weight = 55,
        .description = gJigglypuffPokedexText,
        .unusedDescription = gJigglypuffPokedexTextUnused,
        .pokemonScale = 614,
        .pokemonOffset = 16,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_WIGGLYTUFF] =
    {
        .categoryName = _("BALLOON"),
        .height = 10,
        .weight = 120,
        .description = gWigglytuffPokedexText,
        .unusedDescription = gWigglytuffPokedexTextUnused,
        .pokemonScale = 465,
        .pokemonOffset = 10,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_ZUBAT] =
    {
        .categoryName = _("BAT"),
        .height = 8,
        .weight = 75,
        .description = gZubatPokedexText,
        .unusedDescription = gZubatPokedexTextUnused,
        .pokemonScale = 465,
        .pokemonOffset = 13,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_GOLBAT] =
    {
        .categoryName = _("BAT"),
        .height = 16,
        .weight = 550,
        .description = gGolbatPokedexText,
        .unusedDescription = gGolbatPokedexTextUnused,
        .pokemonScale = 345,
        .pokemonOffset = 0,
        .trainerScale = 361,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_ODDISH] =
    {
        .categoryName = _("WEED"),
        .height = 5,
        .weight = 54,
        .description = gOddishPokedexText,
        .unusedDescription = gOddishPokedexTextUnused,
        .pokemonScale = 670,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_GLOOM] =
    {
        .categoryName = _("WEED"),
        .height = 8,
        .weight = 86,
        .description = gGloomPokedexText,
        .unusedDescription = gGloomPokedexTextUnused,
        .pokemonScale = 559,
        .pokemonOffset = 12,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_VILEPLUME] =
    {
        .categoryName = _("FLOWER"),
        .height = 12,
        .weight = 186,
        .description = gVileplumePokedexText,
        .unusedDescription = gVileplumePokedexTextUnused,
        .pokemonScale = 334,
        .pokemonOffset = 6,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_PARAS] =
    {
        .categoryName = _("MUSHROOM"),
        .height = 3,
        .weight = 54,
        .description = gParasPokedexText,
        .unusedDescription = gParasPokedexTextUnused,
        .pokemonScale = 869,
        .pokemonOffset = 19,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_PARASECT] =
    {
        .categoryName = _("MUSHROOM"),
        .height = 10,
        .weight = 295,
        .description = gParasectPokedexText,
        .unusedDescription = gParasectPokedexTextUnused,
        .pokemonScale = 438,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_VENONAT] =
    {
        .categoryName = _("INSECT"),
        .height = 10,
        .weight = 300,
        .description = gVenonatPokedexText,
        .unusedDescription = gVenonatPokedexTextUnused,
        .pokemonScale = 438,
        .pokemonOffset = 11,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_VENOMOTH] =
    {
        .categoryName = _("POISON MOTH"),
        .height = 15,
        .weight = 125,
        .description = gVenomothPokedexText,
        .unusedDescription = gVenomothPokedexTextUnused,
        .pokemonScale = 323,
        .pokemonOffset = 4,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_DIGLETT] =
    {
        .categoryName = _("MOLE"),
        .height = 2,
        .weight = 8,
        .description = gDiglettPokedexText,
        .unusedDescription = gDiglettPokedexTextUnused,
        .pokemonScale = 1210,
        .pokemonOffset = 20,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_DUGTRIO] =
    {
        .categoryName = _("MOLE"),
        .height = 7,
        .weight = 333,
        .description = gDugtrioPokedexText,
        .unusedDescription = gDugtrioPokedexTextUnused,
        .pokemonScale = 492,
        .pokemonOffset = 13,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MEOWTH] =
    {
        .categoryName = _("SCRATCH CAT"),
        .height = 4,
        .weight = 42,
        .description = gMeowthPokedexText,
        .unusedDescription = gMeowthPokedexTextUnused,
        .pokemonScale = 931,
        .pokemonOffset = 18,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_PERSIAN] =
    {
        .categoryName = _("CLASSY CAT"),
        .height = 10,
        .weight = 320,
        .description = gPersianPokedexText,
        .unusedDescription = gPersianPokedexTextUnused,
        .pokemonScale = 419,
        .pokemonOffset = 10,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_PSYDUCK] =
    {
        .categoryName = _("DUCK"),
        .height = 8,
        .weight = 196,
        .description = gPsyduckPokedexText,
        .unusedDescription = gPsyduckPokedexTextUnused,
        .pokemonScale = 524,
        .pokemonOffset = 12,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_GOLDUCK] =
    {
        .categoryName = _("DUCK"),
        .height = 17,
        .weight = 766,
        .description = gGolduckPokedexText,
        .unusedDescription = gGolduckPokedexTextUnused,
        .pokemonScale = 289,
        .pokemonOffset = 0,
        .trainerScale = 347,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MANKEY] =
    {
        .categoryName = _("PIG MONKEY"),
        .height = 5,
        .weight = 280,
        .description = gMankeyPokedexText,
        .unusedDescription = gMankeyPokedexTextUnused,
        .pokemonScale = 726,
        .pokemonOffset = 17,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_PRIMEAPE] =
    {
        .categoryName = _("PIG MONKEY"),
        .height = 10,
        .weight = 320,
        .description = gPrimeapePokedexText,
        .unusedDescription = gPrimeapePokedexTextUnused,
        .pokemonScale = 475,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_GROWLITHE] =
    {
        .categoryName = _("PUPPY"),
        .height = 7,
        .weight = 190,
        .description = gGrowlithePokedexText,
        .unusedDescription = gGrowlithePokedexTextUnused,
        .pokemonScale = 532,
        .pokemonOffset = 13,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_ARCANINE] =
    {
        .categoryName = _("LEGENDARY"),
        .height = 19,
        .weight = 1550,
        .description = gArcaninePokedexText,
        .unusedDescription = gArcaninePokedexTextUnused,
        .pokemonScale = 328,
        .pokemonOffset = 0,
        .trainerScale = 388,
        .trainerOffset = 3,
    },

    [NATIONAL_DEX_POLIWAG] =
    {
        .categoryName = _("TADPOLE"),
        .height = 6,
        .weight = 124,
        .description = gPoliwagPokedexText,
        .unusedDescription = gPoliwagPokedexTextUnused,
        .pokemonScale = 481,
        .pokemonOffset = 14,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_POLIWHIRL] =
    {
        .categoryName = _("TADPOLE"),
        .height = 10,
        .weight = 200,
        .description = gPoliwhirlPokedexText,
        .unusedDescription = gPoliwhirlPokedexTextUnused,
        .pokemonScale = 382,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_POLIWRATH] =
    {
        .categoryName = _("TADPOLE"),
        .height = 13,
        .weight = 540,
        .description = gPoliwrathPokedexText,
        .unusedDescription = gPoliwrathPokedexTextUnused,
        .pokemonScale = 344,
        .pokemonOffset = 4,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_ABRA] =
    {
        .categoryName = _("PSI"),
        .height = 9,
        .weight = 195,
        .description = gAbraPokedexText,
        .unusedDescription = gAbraPokedexTextUnused,
        .pokemonScale = 414,
        .pokemonOffset = 10,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_KADABRA] =
    {
        .categoryName = _("PSI"),
        .height = 13,
        .weight = 565,
        .description = gKadabraPokedexText,
        .unusedDescription = gKadabraPokedexTextUnused,
        .pokemonScale = 387,
        .pokemonOffset = 4,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_ALAKAZAM] =
    {
        .categoryName = _("PSI"),
        .height = 15,
        .weight = 480,
        .description = gAlakazamPokedexText,
        .unusedDescription = gAlakazamPokedexTextUnused,
        .pokemonScale = 379,
        .pokemonOffset = 2,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MACHOP] =
    {
        .categoryName = _("SUPERPOWER"),
        .height = 8,
        .weight = 195,
        .description = gMachopPokedexText,
        .unusedDescription = gMachopPokedexTextUnused,
        .pokemonScale = 489,
        .pokemonOffset = 12,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MACHOKE] =
    {
        .categoryName = _("SUPERPOWER"),
        .height = 15,
        .weight = 705,
        .description = gMachokePokedexText,
        .unusedDescription = gMachokePokedexTextUnused,
        .pokemonScale = 310,
        .pokemonOffset = 2,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MACHAMP] =
    {
        .categoryName = _("SUPERPOWER"),
        .height = 16,
        .weight = 1300,
        .description = gMachampPokedexText,
        .unusedDescription = gMachampPokedexTextUnused,
        .pokemonScale = 351,
        .pokemonOffset = 2,
        .trainerScale = 343,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_BELLSPROUT] =
    {
        .categoryName = _("FLOWER"),
        .height = 7,
        .weight = 40,
        .description = gBellsproutPokedexText,
        .unusedDescription = gBellsproutPokedexTextUnused,
        .pokemonScale = 505,
        .pokemonOffset = 14,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_WEEPINBELL] =
    {
        .categoryName = _("FLYCATCHER"),
        .height = 10,
        .weight = 64,
        .description = gWeepinbellPokedexText,
        .unusedDescription = gWeepinbellPokedexTextUnused,
        .pokemonScale = 391,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_VICTREEBEL] =
    {
        .categoryName = _("FLYCATCHER"),
        .height = 17,
        .weight = 155,
        .description = gVictreebelPokedexText,
        .unusedDescription = gVictreebelPokedexTextUnused,
        .pokemonScale = 306,
        .pokemonOffset = 0,
        .trainerScale = 353,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_TENTACOOL] =
    {
        .categoryName = _("JELLYFISH"),
        .height = 9,
        .weight = 455,
        .description = gTentacoolPokedexText,
        .unusedDescription = gTentacoolPokedexTextUnused,
        .pokemonScale = 465,
        .pokemonOffset = 11,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_TENTACRUEL] =
    {
        .categoryName = _("JELLYFISH"),
        .height = 16,
        .weight = 550,
        .description = gTentacruelPokedexText,
        .unusedDescription = gTentacruelPokedexTextUnused,
        .pokemonScale = 343,
        .pokemonOffset = 0,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_GEODUDE] =
    {
        .categoryName = _("ROCK"),
        .height = 4,
        .weight = 200,
        .description = gGeodudePokedexText,
        .unusedDescription = gGeodudePokedexTextUnused,
        .pokemonScale = 605,
        .pokemonOffset = 17,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_GRAVELER] =
    {
        .categoryName = _("ROCK"),
        .height = 10,
        .weight = 1050,
        .description = gGravelerPokedexText,
        .unusedDescription = gGravelerPokedexTextUnused,
        .pokemonScale = 391,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_GOLEM] =
    {
        .categoryName = _("MEGATON"),
        .height = 14,
        .weight = 3000,
        .description = gGolemPokedexText,
        .unusedDescription = gGolemPokedexTextUnused,
        .pokemonScale = 332,
        .pokemonOffset = 4,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_PONYTA] =
    {
        .categoryName = _("FIRE HORSE"),
        .height = 10,
        .weight = 300,
        .description = gPonytaPokedexText,
        .unusedDescription = gPonytaPokedexTextUnused,
        .pokemonScale = 475,
        .pokemonOffset = 10,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_RAPIDASH] =
    {
        .categoryName = _("FIRE HORSE"),
        .height = 17,
        .weight = 950,
        .description = gRapidashPokedexText,
        .unusedDescription = gRapidashPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 0,
        .trainerScale = 347,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SLOWPOKE] =
    {
        .categoryName = _("DOPEY"),
        .height = 12,
        .weight = 360,
        .description = gSlowpokePokedexText,
        .unusedDescription = gSlowpokePokedexTextUnused,
        .pokemonScale = 303,
        .pokemonOffset = 7,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SLOWBRO] =
    {
        .categoryName = _("HERMIT CRAB"),
        .height = 16,
        .weight = 785,
        .description = gSlowbroPokedexText,
        .unusedDescription = gSlowbroPokedexTextUnused,
        .pokemonScale = 328,
        .pokemonOffset = 0,
        .trainerScale = 370,
        .trainerOffset = 2,
    },

    [NATIONAL_DEX_MAGNEMITE] =
    {
        .categoryName = _("MAGNET"),
        .height = 3,
        .weight = 60,
        .description = gMagnemitePokedexText,
        .unusedDescription = gMagnemitePokedexTextUnused,
        .pokemonScale = 621,
        .pokemonOffset = 19,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MAGNETON] =
    {
        .categoryName = _("MAGNET"),
        .height = 10,
        .weight = 600,
        .description = gMagnetonPokedexText,
        .unusedDescription = gMagnetonPokedexTextUnused,
        .pokemonScale = 335,
        .pokemonOffset = 11,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_FARFETCHD] =
    {
        .categoryName = _("WILD DUCK"),
        .height = 8,
        .weight = 150,
        .description = gFarfetchdPokedexText,
        .unusedDescription = gFarfetchdPokedexTextUnused,
        .pokemonScale = 489,
        .pokemonOffset = 12,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_DODUO] =
    {
        .categoryName = _("TWIN BIRD"),
        .height = 14,
        .weight = 392,
        .description = gDoduoPokedexText,
        .unusedDescription = gDoduoPokedexTextUnused,
        .pokemonScale = 313,
        .pokemonOffset = 3,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_DODRIO] =
    {
        .categoryName = _("TRIPLE BIRD"),
        .height = 18,
        .weight = 852,
        .description = gDodrioPokedexText,
        .unusedDescription = gDodrioPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 1,
        .trainerScale = 385,
        .trainerOffset = 2,
    },

    [NATIONAL_DEX_SEEL] =
    {
        .categoryName = _("SEA LION"),
        .height = 11,
        .weight = 900,
        .description = gSeelPokedexText,
        .unusedDescription = gSeelPokedexTextUnused,
        .pokemonScale = 389,
        .pokemonOffset = 8,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_DEWGONG] =
    {
        .categoryName = _("SEA LION"),
        .height = 17,
        .weight = 1200,
        .description = gDewgongPokedexText,
        .unusedDescription = gDewgongPokedexTextUnused,
        .pokemonScale = 351,
        .pokemonOffset = 5,
        .trainerScale = 437,
        .trainerOffset = 5,
    },

    [NATIONAL_DEX_GRIMER] =
    {
        .categoryName = _("SLUDGE"),
        .height = 9,
        .weight = 300,
        .description = gGrimerPokedexText,
        .unusedDescription = gGrimerPokedexTextUnused,
        .pokemonScale = 424,
        .pokemonOffset = 10,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MUK] =
    {
        .categoryName = _("SLUDGE"),
        .height = 12,
        .weight = 300,
        .description = gMukPokedexText,
        .unusedDescription = gMukPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 7,
        .trainerScale = 373,
        .trainerOffset = 2,
    },

    [NATIONAL_DEX_SHELLDER] =
    {
        .categoryName = _("BIVALVE"),
        .height = 3,
        .weight = 40,
        .description = gShellderPokedexText,
        .unusedDescription = gShellderPokedexTextUnused,
        .pokemonScale = 1024,
        .pokemonOffset = 19,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_CLOYSTER] =
    {
        .categoryName = _("BIVALVE"),
        .height = 15,
        .weight = 1325,
        .description = gCloysterPokedexText,
        .unusedDescription = gCloysterPokedexTextUnused,
        .pokemonScale = 348,
        .pokemonOffset = 2,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_GASTLY] =
    {
        .categoryName = _("GAS"),
        .height = 13,
        .weight = 1,
        .description = gGastlyPokedexText,
        .unusedDescription = gGastlyPokedexTextUnused,
        .pokemonScale = 365,
        .pokemonOffset = 5,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_HAUNTER] =
    {
        .categoryName = _("GAS"),
        .height = 16,
        .weight = 1,
        .description = gHaunterPokedexText,
        .unusedDescription = gHaunterPokedexTextUnused,
        .pokemonScale = 323,
        .pokemonOffset = 3,
        .trainerScale = 386,
        .trainerOffset = 2,
    },

    [NATIONAL_DEX_GENGAR] =
    {
        .categoryName = _("SHADOW"),
        .height = 15,
        .weight = 405,
        .description = gGengarPokedexText,
        .unusedDescription = gGengarPokedexTextUnused,
        .pokemonScale = 284,
        .pokemonOffset = 2,
        .trainerScale = 347,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_ONIX] =
    {
        .categoryName = _("ROCK SNAKE"),
        .height = 88,
        .weight = 2100,
        .description = gOnixPokedexText,
        .unusedDescription = gOnixPokedexTextUnused,
        .pokemonScale = 328,
        .pokemonOffset = 0,
        .trainerScale = 1796,
        .trainerOffset = 19,
    },

    [NATIONAL_DEX_DROWZEE] =
    {
        .categoryName = _("HYPNOSIS"),
        .height = 10,
        .weight = 324,
        .description = gDrowzeePokedexText,
        .unusedDescription = gDrowzeePokedexTextUnused,
        .pokemonScale = 428,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_HYPNO] =
    {
        .categoryName = _("HYPNOSIS"),
        .height = 16,
        .weight = 756,
        .description = gHypnoPokedexText,
        .unusedDescription = gHypnoPokedexTextUnused,
        .pokemonScale = 351,
        .pokemonOffset = 3,
        .trainerScale = 367,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_KRABBY] =
    {
        .categoryName = _("RIVER CRAB"),
        .height = 4,
        .weight = 65,
        .description = gKrabbyPokedexText,
        .unusedDescription = gKrabbyPokedexTextUnused,
        .pokemonScale = 815,
        .pokemonOffset = 17,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_KINGLER] =
    {
        .categoryName = _("PINCER"),
        .height = 13,
        .weight = 600,
        .description = gKinglerPokedexText,
        .unusedDescription = gKinglerPokedexTextUnused,
        .pokemonScale = 380,
        .pokemonOffset = 5,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_VOLTORB] =
    {
        .categoryName = _("BALL"),
        .height = 5,
        .weight = 104,
        .description = gVoltorbPokedexText,
        .unusedDescription = gVoltorbPokedexTextUnused,
        .pokemonScale = 465,
        .pokemonOffset = 16,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_ELECTRODE] =
    {
        .categoryName = _("BALL"),
        .height = 12,
        .weight = 666,
        .description = gElectrodePokedexText,
        .unusedDescription = gElectrodePokedexTextUnused,
        .pokemonScale = 279,
        .pokemonOffset = 6,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_EXEGGCUTE] =
    {
        .categoryName = _("EGG"),
        .height = 4,
        .weight = 25,
        .description = gExeggcutePokedexText,
        .unusedDescription = gExeggcutePokedexTextUnused,
        .pokemonScale = 1047,
        .pokemonOffset = 17,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_EXEGGUTOR] =
    {
        .categoryName = _("COCONUT"),
        .height = 20,
        .weight = 1200,
        .description = gExeggutorPokedexText,
        .unusedDescription = gExeggutorPokedexTextUnused,
        .pokemonScale = 351,
        .pokemonOffset = 0,
        .trainerScale = 408,
        .trainerOffset = 4,
    },

    [NATIONAL_DEX_CUBONE] =
    {
        .categoryName = _("LONELY"),
        .height = 4,
        .weight = 65,
        .description = gCubonePokedexText,
        .unusedDescription = gCubonePokedexTextUnused,
        .pokemonScale = 884,
        .pokemonOffset = 17,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MAROWAK] =
    {
        .categoryName = _("BONE KEEPER"),
        .height = 10,
        .weight = 450,
        .description = gMarowakPokedexText,
        .unusedDescription = gMarowakPokedexTextUnused,
        .pokemonScale = 410,
        .pokemonOffset = 11,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_HITMONLEE] =
    {
        .categoryName = _("KICKING"),
        .height = 15,
        .weight = 498,
        .description = gHitmonleePokedexText,
        .unusedDescription = gHitmonleePokedexTextUnused,
        .pokemonScale = 323,
        .pokemonOffset = 1,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_HITMONCHAN] =
    {
        .categoryName = _("PUNCHING"),
        .height = 14,
        .weight = 502,
        .description = gHitmonchanPokedexText,
        .unusedDescription = gHitmonchanPokedexTextUnused,
        .pokemonScale = 372,
        .pokemonOffset = 4,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_LICKITUNG] =
    {
        .categoryName = _("LICKING"),
        .height = 12,
        .weight = 655,
        .description = gLickitungPokedexText,
        .unusedDescription = gLickitungPokedexTextUnused,
        .pokemonScale = 365,
        .pokemonOffset = 6,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_KOFFING] =
    {
        .categoryName = _("POISON GAS"),
        .height = 6,
        .weight = 10,
        .description = gKoffingPokedexText,
        .unusedDescription = gKoffingPokedexTextUnused,
        .pokemonScale = 621,
        .pokemonOffset = 14,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_WEEZING] =
    {
        .categoryName = _("POISON GAS"),
        .height = 12,
        .weight = 95,
        .description = gWeezingPokedexText,
        .unusedDescription = gWeezingPokedexTextUnused,
        .pokemonScale = 419,
        .pokemonOffset = 8,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_RHYHORN] =
    {
        .categoryName = _("SPIKES"),
        .height = 10,
        .weight = 1150,
        .description = gRhyhornPokedexText,
        .unusedDescription = gRhyhornPokedexTextUnused,
        .pokemonScale = 428,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_RHYDON] =
    {
        .categoryName = _("DRILL"),
        .height = 19,
        .weight = 1200,
        .description = gRhydonPokedexText,
        .unusedDescription = gRhydonPokedexTextUnused,
        .pokemonScale = 334,
        .pokemonOffset = 0,
        .trainerScale = 401,
        .trainerOffset = 3,
    },

    [NATIONAL_DEX_HAPPINY] =
    {
        .categoryName = _("PLAYHOUSE"),
        .height = 6,
        .weight = 244,
        .description = gHappinyPokedexText,
        .unusedDescription = gHappinyPokedexTextUnused,
        .pokemonScale = 590,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_LICKILICKY] =
    {
        .categoryName = _("LICKING"),
        .height = 17,
        .weight = 1400,
        .description = gLickilickyPokedexText,
        .unusedDescription = gLickilickyPokedexTextUnused,
        .pokemonScale = 339,
        .pokemonOffset = 0,
        .trainerScale = 347,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_CHANSEY] =
    {
        .categoryName = _("EGG"),
        .height = 11,
        .weight = 346,
        .description = gChanseyPokedexText,
        .unusedDescription = gChanseyPokedexTextUnused,
        .pokemonScale = 389,
        .pokemonOffset = 8,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_TANGELA] =
    {
        .categoryName = _("VINE"),
        .height = 10,
        .weight = 350,
        .description = gTangelaPokedexText,
        .unusedDescription = gTangelaPokedexTextUnused,
        .pokemonScale = 438,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_TANGROWTH] =
    {
        .categoryName = _("VINE"),
        .height = 20,
        .weight = 1286,
        .description = gTangrowthPokedexText,
        .unusedDescription = gTangrowthPokedexTextUnused,
        .pokemonScale = 351,
        .pokemonOffset = 0,
        .trainerScale = 408,
        .trainerOffset = 4,
    },

    [NATIONAL_DEX_KANGASKHAN] =
    {
        .categoryName = _("PARENT"),
        .height = 22,
        .weight = 800,
        .description = gKangaskhanPokedexText,
        .unusedDescription = gKangaskhanPokedexTextUnused,
        .pokemonScale = 328,
        .pokemonOffset = -2,
        .trainerScale = 449,
        .trainerOffset = 5,
    },

    [NATIONAL_DEX_HORSEA] =
    {
        .categoryName = _("DRAGON"),
        .height = 4,
        .weight = 80,
        .description = gHorseaPokedexText,
        .unusedDescription = gHorseaPokedexTextUnused,
        .pokemonScale = 768,
        .pokemonOffset = 18,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SEADRA] =
    {
        .categoryName = _("DRAGON"),
        .height = 12,
        .weight = 250,
        .description = gSeadraPokedexText,
        .unusedDescription = gSeadraPokedexTextUnused,
        .pokemonScale = 365,
        .pokemonOffset = 5,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_GOLDEEN] =
    {
        .categoryName = _("GOLDFISH"),
        .height = 6,
        .weight = 150,
        .description = gGoldeenPokedexText,
        .unusedDescription = gGoldeenPokedexTextUnused,
        .pokemonScale = 683,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SEAKING] =
    {
        .categoryName = _("GOLDFISH"),
        .height = 13,
        .weight = 390,
        .description = gSeakingPokedexText,
        .unusedDescription = gSeakingPokedexTextUnused,
        .pokemonScale = 394,
        .pokemonOffset = 5,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_STARYU] =
    {
        .categoryName = _("STAR SHAPE"),
        .height = 8,
        .weight = 345,
        .description = gStaryuPokedexText,
        .unusedDescription = gStaryuPokedexTextUnused,
        .pokemonScale = 477,
        .pokemonOffset = 13,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_STARMIE] =
    {
        .categoryName = _("MYSTERIOUS"),
        .height = 11,
        .weight = 800,
        .description = gStarmiePokedexText,
        .unusedDescription = gStarmiePokedexTextUnused,
        .pokemonScale = 398,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MR_MIME] =
    {
        .categoryName = _("BARRIER"),
        .height = 13,
        .weight = 545,
        .description = gMrmimePokedexText,
        .unusedDescription = gMrmimePokedexTextUnused,
        .pokemonScale = 344,
        .pokemonOffset = 5,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SCYTHER] =
    {
        .categoryName = _("MANTIS"),
        .height = 15,
        .weight = 560,
        .description = gScytherPokedexText,
        .unusedDescription = gScytherPokedexTextUnused,
        .pokemonScale = 379,
        .pokemonOffset = 3,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_JYNX] =
    {
        .categoryName = _("HUMAN SHAPE"),
        .height = 14,
        .weight = 406,
        .description = gJynxPokedexText,
        .unusedDescription = gJynxPokedexTextUnused,
        .pokemonScale = 313,
        .pokemonOffset = 3,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_ELECTABUZZ] =
    {
        .categoryName = _("ELECTRIC"),
        .height = 11,
        .weight = 300,
        .description = gElectabuzzPokedexText,
        .unusedDescription = gElectabuzzPokedexTextUnused,
        .pokemonScale = 491,
        .pokemonOffset = 8,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MAGMAR] =
    {
        .categoryName = _("SPITFIRE"),
        .height = 13,
        .weight = 445,
        .description = gMagmarPokedexText,
        .unusedDescription = gMagmarPokedexTextUnused,
        .pokemonScale = 387,
        .pokemonOffset = 4,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_PINSIR] =
    {
        .categoryName = _("STAG BEETLE"),
        .height = 15,
        .weight = 550,
        .description = gPinsirPokedexText,
        .unusedDescription = gPinsirPokedexTextUnused,
        .pokemonScale = 348,
        .pokemonOffset = 2,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_TAUROS] =
    {
        .categoryName = _("WILD BULL"),
        .height = 14,
        .weight = 884,
        .description = gTaurosPokedexText,
        .unusedDescription = gTaurosPokedexTextUnused,
        .pokemonScale = 386,
        .pokemonOffset = 5,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MAGIKARP] =
    {
        .categoryName = _("FISH"),
        .height = 9,
        .weight = 100,
        .description = gMagikarpPokedexText,
        .unusedDescription = gMagikarpPokedexTextUnused,
        .pokemonScale = 476,
        .pokemonOffset = 12,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_GYARADOS] =
    {
        .categoryName = _("ATROCIOUS"),
        .height = 65,
        .weight = 2350,
        .description = gGyaradosPokedexText,
        .unusedDescription = gGyaradosPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 0,
        .trainerScale = 1326,
        .trainerOffset = 17,
    },

    [NATIONAL_DEX_LAPRAS] =
    {
        .categoryName = _("TRANSPORT"),
        .height = 25,
        .weight = 2200,
        .description = gLaprasPokedexText,
        .unusedDescription = gLaprasPokedexTextUnused,
        .pokemonScale = 323,
        .pokemonOffset = 1,
        .trainerScale = 528,
        .trainerOffset = 8,
    },

    [NATIONAL_DEX_DITTO] =
    {
        .categoryName = _("TRANSFORM"),
        .height = 3,
        .weight = 40,
        .description = gDittoPokedexText,
        .unusedDescription = gDittoPokedexTextUnused,
        .pokemonScale = 931,
        .pokemonOffset = 19,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_EEVEE] =
    {
        .categoryName = _("EVOLUTION"),
        .height = 3,
        .weight = 65,
        .description = gEeveePokedexText,
        .unusedDescription = gEeveePokedexTextUnused,
        .pokemonScale = 1241,
        .pokemonOffset = 19,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_VAPOREON] =
    {
        .categoryName = _("BUBBLE JET"),
        .height = 10,
        .weight = 290,
        .description = gVaporeonPokedexText,
        .unusedDescription = gVaporeonPokedexTextUnused,
        .pokemonScale = 493,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_JOLTEON] =
    {
        .categoryName = _("LIGHTNING"),
        .height = 8,
        .weight = 245,
        .description = gJolteonPokedexText,
        .unusedDescription = gJolteonPokedexTextUnused,
        .pokemonScale = 489,
        .pokemonOffset = 11,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_FLAREON] =
    {
        .categoryName = _("FLAME"),
        .height = 9,
        .weight = 250,
        .description = gFlareonPokedexText,
        .unusedDescription = gFlareonPokedexTextUnused,
        .pokemonScale = 455,
        .pokemonOffset = 11,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_PORYGON] =
    {
        .categoryName = _("VIRTUAL"),
        .height = 8,
        .weight = 365,
        .description = gPorygonPokedexText,
        .unusedDescription = gPorygonPokedexTextUnused,
        .pokemonScale = 442,
        .pokemonOffset = 11,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_OMANYTE] =
    {
        .categoryName = _("SPIRAL"),
        .height = 4,
        .weight = 75,
        .description = gOmanytePokedexText,
        .unusedDescription = gOmanytePokedexTextUnused,
        .pokemonScale = 791,
        .pokemonOffset = 17,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_OMASTAR] =
    {
        .categoryName = _("SPIRAL"),
        .height = 10,
        .weight = 350,
        .description = gOmastarPokedexText,
        .unusedDescription = gOmastarPokedexTextUnused,
        .pokemonScale = 465,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_KABUTO] =
    {
        .categoryName = _("SHELLFISH"),
        .height = 5,
        .weight = 115,
        .description = gKabutoPokedexText,
        .unusedDescription = gKabutoPokedexTextUnused,
        .pokemonScale = 465,
        .pokemonOffset = 16,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_KABUTOPS] =
    {
        .categoryName = _("SHELLFISH"),
        .height = 13,
        .weight = 405,
        .description = gKabutopsPokedexText,
        .unusedDescription = gKabutopsPokedexTextUnused,
        .pokemonScale = 408,
        .pokemonOffset = 5,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_AERODACTYL] =
    {
        .categoryName = _("FOSSIL"),
        .height = 18,
        .weight = 590,
        .description = gAerodactylPokedexText,
        .unusedDescription = gAerodactylPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 1,
        .trainerScale = 373,
        .trainerOffset = 2,
    },

    [NATIONAL_DEX_ARTICUNO] =
    {
        .categoryName = _("FREEZE"),
        .height = 17,
        .weight = 554,
        .description = gArticunoPokedexText,
        .unusedDescription = gArticunoPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 0,
        .trainerScale = 347,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_ZAPDOS] =
    {
        .categoryName = _("ELECTRIC"),
        .height = 16,
        .weight = 526,
        .description = gZapdosPokedexText,
        .unusedDescription = gZapdosPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 3,
        .trainerScale = 367,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_MOLTRES] =
    {
        .categoryName = _("FLAME"),
        .height = 20,
        .weight = 600,
        .description = gMoltresPokedexText,
        .unusedDescription = gMoltresPokedexTextUnused,
        .pokemonScale = 351,
        .pokemonOffset = 4,
        .trainerScale = 443,
        .trainerOffset = 5,
    },

    [NATIONAL_DEX_DRATINI] =
    {
        .categoryName = _("DRAGON"),
        .height = 18,
        .weight = 33,
        .description = gDratiniPokedexText,
        .unusedDescription = gDratiniPokedexTextUnused,
        .pokemonScale = 273,
        .pokemonOffset = 7,
        .trainerScale = 514,
        .trainerOffset = 8,
    },

    [NATIONAL_DEX_DRAGONAIR] =
    {
        .categoryName = _("DRAGON"),
        .height = 40,
        .weight = 165,
        .description = gDragonairPokedexText,
        .unusedDescription = gDragonairPokedexTextUnused,
        .pokemonScale = 345,
        .pokemonOffset = 4,
        .trainerScale = 973,
        .trainerOffset = 15,
    },

    [NATIONAL_DEX_DRAGONITE] =
    {
        .categoryName = _("DRAGON"),
        .height = 22,
        .weight = 2100,
        .description = gDragonitePokedexText,
        .unusedDescription = gDragonitePokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 0,
        .trainerScale = 449,
        .trainerOffset = 5,
    },

    [NATIONAL_DEX_MEWTWO] =
    {
        .categoryName = _("GENETIC"),
        .height = 20,
        .weight = 1220,
        .description = gMewtwoPokedexText,
        .unusedDescription = gMewtwoPokedexTextUnused,
        .pokemonScale = 351,
        .pokemonOffset = 0,
        .trainerScale = 408,
        .trainerOffset = 4,
    },

    [NATIONAL_DEX_MEW] =
    {
        .categoryName = _("NEW SPECIES"),
        .height = 4,
        .weight = 40,
        .description = gMewPokedexText,
        .unusedDescription = gMewPokedexTextUnused,
        .pokemonScale = 1024,
        .pokemonOffset = 18,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_CINNABAR_MAGIKARP] =
    {
        .categoryName = _("DRAGON"),
        .height = 18,
        .weight = 79,
        .description = gCinnabarMagikarpPokedexText,
        .unusedDescription = gCinnabarMagikarpPokedexTextUnused,
        .pokemonScale = 345,
        .pokemonOffset = 0,
        .trainerScale = 367,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_CHIKORITA] =
    {
        .categoryName = _("LEAF"),
        .height = 9,
        .weight = 64,
        .description = gChikoritaPokedexText,
        .unusedDescription = gChikoritaPokedexTextUnused,
        .pokemonScale = 383,
        .pokemonOffset = 10,
        .trainerScale = 341,
        .trainerOffset = 0,
    },
    [NATIONAL_DEX_BAYLEEF] =
    {
        .categoryName = _("LEAF"),
        .height = 12,
        .weight = 158,
        .description = gBayleefPokedexText,
        .unusedDescription = gBayleefPokedexTextUnused,
        .pokemonScale = 419,
        .pokemonOffset = 6,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MEGANIUM] =
    {
        .categoryName = _("HERB"),
        .height = 18,
        .weight = 1005,
        .description = gMeganiumPokedexText,
        .unusedDescription = gMeganiumPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 0,
        .trainerScale = 367,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_CYNDAQUIL] =
    {
        .categoryName = _("FIRE MOUSE"),
        .height = 5,
        .weight = 79,
        .description = gCyndaquilPokedexText,
        .unusedDescription = gCyndaquilPokedexTextUnused,
        .pokemonScale = 670,
        .pokemonOffset = 16,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_QUILAVA] =
    {
        .categoryName = _("VOLCANO"),
        .height = 9,
        .weight = 190,
        .description = gQuilavaPokedexText,
        .unusedDescription = gQuilavaPokedexTextUnused,
        .pokemonScale = 496,
        .pokemonOffset = 11,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_TYPHLOSION] =
    {
        .categoryName = _("VOLCANO"),
        .height = 17,
        .weight = 795,
        .description = gTyphlosionPokedexText,
        .unusedDescription = gTyphlosionPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 0,
        .trainerScale = 347,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_TOTODILE] =
    {
        .categoryName = _("BIG JAW"),
        .height = 6,
        .weight = 95,
        .description = gTotodilePokedexText,
        .unusedDescription = gTotodilePokedexTextUnused,
        .pokemonScale = 543,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_CROCONAW] =
    {
        .categoryName = _("BIG JAW"),
        .height = 11,
        .weight = 250,
        .description = gCroconawPokedexText,
        .unusedDescription = gCroconawPokedexTextUnused,
        .pokemonScale = 440,
        .pokemonOffset = 8,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_FERALIGATR] =
    {
        .categoryName = _("BIG JAW"),
        .height = 23,
        .weight = 888,
        .description = gFeraligatrPokedexText,
        .unusedDescription = gFeraligatrPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 0,
        .trainerScale = 469,
        .trainerOffset = 6,
    },

    [NATIONAL_DEX_SENTRET] =
    {
        .categoryName = _("SCOUT"),
        .height = 8,
        .weight = 60,
        .description = gSentretPokedexText,
        .unusedDescription = gSentretPokedexTextUnused,
        .pokemonScale = 652,
        .pokemonOffset = 12,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_FURRET] =
    {
        .categoryName = _("LONG BODY"),
        .height = 18,
        .weight = 325,
        .description = gFurretPokedexText,
        .unusedDescription = gFurretPokedexTextUnused,
        .pokemonScale = 278,
        .pokemonOffset = 0,
        .trainerScale = 367,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_HOOTHOOT] =
    {
        .categoryName = _("OWL"),
        .height = 7,
        .weight = 212,
        .description = gHoothootPokedexText,
        .unusedDescription = gHoothootPokedexTextUnused,
        .pokemonScale = 492,
        .pokemonOffset = 13,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_NOCTOWL] =
    {
        .categoryName = _("OWL"),
        .height = 16,
        .weight = 408,
        .description = gNoctowlPokedexText,
        .unusedDescription = gNoctowlPokedexTextUnused,
        .pokemonScale = 332,
        .pokemonOffset = 1,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_LEDYBA] =
    {
        .categoryName = _("FIVE STAR"),
        .height = 10,
        .weight = 108,
        .description = gLedybaPokedexText,
        .unusedDescription = gLedybaPokedexTextUnused,
        .pokemonScale = 382,
        .pokemonOffset = 10,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_LEDIAN] =
    {
        .categoryName = _("FIVE STAR"),
        .height = 14,
        .weight = 356,
        .description = gLedianPokedexText,
        .unusedDescription = gLedianPokedexTextUnused,
        .pokemonScale = 366,
        .pokemonOffset = 3,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SPINARAK] =
    {
        .categoryName = _("STRING SPIT"),
        .height = 5,
        .weight = 85,
        .description = gSpinarakPokedexText,
        .unusedDescription = gSpinarakPokedexTextUnused,
        .pokemonScale = 465,
        .pokemonOffset = 16,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_ARIADOS] =
    {
        .categoryName = _("LONG LEG"),
        .height = 11,
        .weight = 335,
        .description = gAriadosPokedexText,
        .unusedDescription = gAriadosPokedexTextUnused,
        .pokemonScale = 457,
        .pokemonOffset = 8,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_CROBAT] =
    {
        .categoryName = _("BAT"),
        .height = 18,
        .weight = 750,
        .description = gCrobatPokedexText,
        .unusedDescription = gCrobatPokedexTextUnused,
        .pokemonScale = 351,
        .pokemonOffset = 0,
        .trainerScale = 367,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_CHINCHOU] =
    {
        .categoryName = _("ANGLER"),
        .height = 5,
        .weight = 120,
        .description = gChinchouPokedexText,
        .unusedDescription = gChinchouPokedexTextUnused,
        .pokemonScale = 614,
        .pokemonOffset = 16,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_LANTURN] =
    {
        .categoryName = _("LIGHT"),
        .height = 12,
        .weight = 225,
        .description = gLanturnPokedexText,
        .unusedDescription = gLanturnPokedexTextUnused,
        .pokemonScale = 372,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_PICHU] =
    {
        .categoryName = _("TINY MOUSE"),
        .height = 3,
        .weight = 20,
        .description = gPichuPokedexText,
        .unusedDescription = gPichuPokedexTextUnused,
        .pokemonScale = 1179,
        .pokemonOffset = 19,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_CLEFFA] =
    {
        .categoryName = _("STAR SHAPE"),
        .height = 3,
        .weight = 30,
        .description = gCleffaPokedexText,
        .unusedDescription = gCleffaPokedexTextUnused,
        .pokemonScale = 776,
        .pokemonOffset = 19,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_IGGLYBUFF] =
    {
        .categoryName = _("BALLOON"),
        .height = 3,
        .weight = 10,
        .description = gIgglybuffPokedexText,
        .unusedDescription = gIgglybuffPokedexTextUnused,
        .pokemonScale = 900,
        .pokemonOffset = 19,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_TOGEPI] =
    {
        .categoryName = _("SPIKE BALL"),
        .height = 3,
        .weight = 15,
        .description = gTogepiPokedexText,
        .unusedDescription = gTogepiPokedexTextUnused,
        .pokemonScale = 776,
        .pokemonOffset = 19,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_TOGETIC] =
    {
        .categoryName = _("HAPPINESS"),
        .height = 6,
        .weight = 32,
        .description = gTogeticPokedexText,
        .unusedDescription = gTogeticPokedexTextUnused,
        .pokemonScale = 714,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_NATU] =
    {
        .categoryName = _("TINY BIRD"),
        .height = 2,
        .weight = 20,
        .description = gNatuPokedexText,
        .unusedDescription = gNatuPokedexTextUnused,
        .pokemonScale = 1164,
        .pokemonOffset = 20,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_XATU] =
    {
        .categoryName = _("MYSTIC"),
        .height = 15,
        .weight = 150,
        .description = gXatuPokedexText,
        .unusedDescription = gXatuPokedexTextUnused,
        .pokemonScale = 310,
        .pokemonOffset = 2,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MAREEP] =
    {
        .categoryName = _("WOOL"),
        .height = 6,
        .weight = 78,
        .description = gMareepPokedexText,
        .unusedDescription = gMareepPokedexTextUnused,
        .pokemonScale = 512,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_FLAAFFY] =
    {
        .categoryName = _("WOOL"),
        .height = 8,
        .weight = 133,
        .description = gFlaaffyPokedexText,
        .unusedDescription = gFlaaffyPokedexTextUnused,
        .pokemonScale = 512,
        .pokemonOffset = 12,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_AMPHAROS] =
    {
        .categoryName = _("LIGHT"),
        .height = 14,
        .weight = 615,
        .description = gAmpharosPokedexText,
        .unusedDescription = gAmpharosPokedexTextUnused,
        .pokemonScale = 352,
        .pokemonOffset = 3,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_BELLOSSOM] =
    {
        .categoryName = _("FLOWER"),
        .height = 4,
        .weight = 58,
        .description = gBellossomPokedexText,
        .unusedDescription = gBellossomPokedexTextUnused,
        .pokemonScale = 815,
        .pokemonOffset = 17,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MARILL] =
    {
        .categoryName = _("AQUA MOUSE"),
        .height = 4,
        .weight = 85,
        .description = gMarillPokedexText,
        .unusedDescription = gMarillPokedexTextUnused,
        .pokemonScale = 815,
        .pokemonOffset = 17,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_AZUMARILL] =
    {
        .categoryName = _("AQUA RABBIT"),
        .height = 8,
        .weight = 285,
        .description = gAzumarillPokedexText,
        .unusedDescription = gAzumarillPokedexTextUnused,
        .pokemonScale = 524,
        .pokemonOffset = 12,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_POLITOED] =
    {
        .categoryName = _("FROG"),
        .height = 11,
        .weight = 339,
        .description = gPolitoedPokedexText,
        .unusedDescription = gPolitoedPokedexTextUnused,
        .pokemonScale = 440,
        .pokemonOffset = 8,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_HOPPIP] =
    {
        .categoryName = _("COTTONWEED"),
        .height = 4,
        .weight = 5,
        .description = gHoppipPokedexText,
        .unusedDescription = gHoppipPokedexTextUnused,
        .pokemonScale = 1001,
        .pokemonOffset = 17,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SKIPLOOM] =
    {
        .categoryName = _("COTTONWEED"),
        .height = 6,
        .weight = 10,
        .description = gSkiploomPokedexText,
        .unusedDescription = gSkiploomPokedexTextUnused,
        .pokemonScale = 528,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_JUMPLUFF] =
    {
        .categoryName = _("COTTONWEED"),
        .height = 8,
        .weight = 30,
        .description = gJumpluffPokedexText,
        .unusedDescription = gJumpluffPokedexTextUnused,
        .pokemonScale = 582,
        .pokemonOffset = 12,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_AIPOM] =
    {
        .categoryName = _("LONG TAIL"),
        .height = 8,
        .weight = 115,
        .description = gAipomPokedexText,
        .unusedDescription = gAipomPokedexTextUnused,
        .pokemonScale = 652,
        .pokemonOffset = 12,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SUNKERN] =
    {
        .categoryName = _("SEED"),
        .height = 3,
        .weight = 18,
        .description = gSunkernPokedexText,
        .unusedDescription = gSunkernPokedexTextUnused,
        .pokemonScale = 993,
        .pokemonOffset = 19,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SUNFLORA] =
    {
        .categoryName = _("SUN"),
        .height = 8,
        .weight = 85,
        .description = gSunfloraPokedexText,
        .unusedDescription = gSunfloraPokedexTextUnused,
        .pokemonScale = 559,
        .pokemonOffset = 12,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_YANMA] =
    {
        .categoryName = _("CLEAR WING"),
        .height = 12,
        .weight = 380,
        .description = gYanmaPokedexText,
        .unusedDescription = gYanmaPokedexTextUnused,
        .pokemonScale = 334,
        .pokemonOffset = 6,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_WOOPER] =
    {
        .categoryName = _("WATER FISH"),
        .height = 4,
        .weight = 85,
        .description = gWooperPokedexText,
        .unusedDescription = gWooperPokedexTextUnused,
        .pokemonScale = 698,
        .pokemonOffset = 17,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_QUAGSIRE] =
    {
        .categoryName = _("WATER FISH"),
        .height = 14,
        .weight = 750,
        .description = gQuagsirePokedexText,
        .unusedDescription = gQuagsirePokedexTextUnused,
        .pokemonScale = 332,
        .pokemonOffset = 4,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_ESPEON] =
    {
        .categoryName = _("SUN"),
        .height = 9,
        .weight = 265,
        .description = gEspeonPokedexText,
        .unusedDescription = gEspeonPokedexTextUnused,
        .pokemonScale = 496,
        .pokemonOffset = 11,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_UMBREON] =
    {
        .categoryName = _("MOONLIGHT"),
        .height = 10,
        .weight = 270,
        .description = gUmbreonPokedexText,
        .unusedDescription = gUmbreonPokedexTextUnused,
        .pokemonScale = 456,
        .pokemonOffset = 10,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MURKROW] =
    {
        .categoryName = _("DARKNESS"),
        .height = 5,
        .weight = 21,
        .description = gMurkrowPokedexText,
        .unusedDescription = gMurkrowPokedexTextUnused,
        .pokemonScale = 782,
        .pokemonOffset = 16,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SLOWKING] =
    {
        .categoryName = _("ROYAL"),
        .height = 20,
        .weight = 795,
        .description = gSlowkingPokedexText,
        .unusedDescription = gSlowkingPokedexTextUnused,
        .pokemonScale = 334,
        .pokemonOffset = -1,
        .trainerScale = 408,
        .trainerOffset = 4,
    },

    [NATIONAL_DEX_MISDREAVUS] =
    {
        .categoryName = _("SCREECH"),
        .height = 7,
        .weight = 10,
        .description = gMisdreavusPokedexText,
        .unusedDescription = gMisdreavusPokedexTextUnused,
        .pokemonScale = 519,
        .pokemonOffset = 13,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_UNOWN] =
    {
        .categoryName = _("SYMBOL"),
        .height = 5,
        .weight = 50,
        .description = gUnownPokedexText,
        .unusedDescription = gUnownPokedexTextUnused,
        .pokemonScale = 633,
        .pokemonOffset = 16,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_WOBBUFFET] =
    {
        .categoryName = _("PATIENT"),
        .height = 13,
        .weight = 285,
        .description = gWobbuffetPokedexText,
        .unusedDescription = gWobbuffetPokedexTextUnused,
        .pokemonScale = 372,
        .pokemonOffset = 5,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_GIRAFARIG] =
    {
        .categoryName = _("LONG NECK"),
        .height = 15,
        .weight = 415,
        .description = gGirafarigPokedexText,
        .unusedDescription = gGirafarigPokedexTextUnused,
        .pokemonScale = 366,
        .pokemonOffset = 3,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_PINECO] =
    {
        .categoryName = _("BAGWORM"),
        .height = 6,
        .weight = 72,
        .description = gPinecoPokedexText,
        .unusedDescription = gPinecoPokedexTextUnused,
        .pokemonScale = 667,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_FORRETRESS] =
    {
        .categoryName = _("BAGWORM"),
        .height = 12,
        .weight = 1258,
        .description = gForretressPokedexText,
        .unusedDescription = gForretressPokedexTextUnused,
        .pokemonScale = 357,
        .pokemonOffset = 7,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_DUNSPARCE] =
    {
        .categoryName = _("LAND SNAKE"),
        .height = 15,
        .weight = 140,
        .description = gDunsparcePokedexText,
        .unusedDescription = gDunsparcePokedexTextUnused,
        .pokemonScale = 273,
        .pokemonOffset = 9,
        .trainerScale = 500,
        .trainerOffset = 7,
    },

    [NATIONAL_DEX_GLIGAR] =
    {
        .categoryName = _("FLYSCORPION"),
        .height = 11,
        .weight = 648,
        .description = gGligarPokedexText,
        .unusedDescription = gGligarPokedexTextUnused,
        .pokemonScale = 465,
        .pokemonOffset = 7,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_STEELIX] =
    {
        .categoryName = _("IRON SNAKE"),
        .height = 92,
        .weight = 4000,
        .description = gSteelixPokedexText,
        .unusedDescription = gSteelixPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 0,
        .trainerScale = 1907,
        .trainerOffset = 19,
    },

    [NATIONAL_DEX_SNUBBULL] =
    {
        .categoryName = _("FAIRY"),
        .height = 6,
        .weight = 78,
        .description = gSnubbullPokedexText,
        .unusedDescription = gSnubbullPokedexTextUnused,
        .pokemonScale = 590,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_GRANBULL] =
    {
        .categoryName = _("FAIRY"),
        .height = 14,
        .weight = 487,
        .description = gGranbullPokedexText,
        .unusedDescription = gGranbullPokedexTextUnused,
        .pokemonScale = 346,
        .pokemonOffset = 4,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_QWILFISH] =
    {
        .categoryName = _("BALLOON"),
        .height = 5,
        .weight = 39,
        .description = gQwilfishPokedexText,
        .unusedDescription = gQwilfishPokedexTextUnused,
        .pokemonScale = 801,
        .pokemonOffset = 16,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SCIZOR] =
    {
        .categoryName = _("PINCER"),
        .height = 18,
        .weight = 1180,
        .description = gScizorPokedexText,
        .unusedDescription = gScizorPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 0,
        .trainerScale = 367,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_SHUCKLE] =
    {
        .categoryName = _("MOLD"),
        .height = 6,
        .weight = 205,
        .description = gShucklePokedexText,
        .unusedDescription = gShucklePokedexTextUnused,
        .pokemonScale = 698,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_HERACROSS] =
    {
        .categoryName = _("SINGLE HORN"),
        .height = 15,
        .weight = 540,
        .description = gHeracrossPokedexText,
        .unusedDescription = gHeracrossPokedexTextUnused,
        .pokemonScale = 354,
        .pokemonOffset = 2,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SNEASEL] =
    {
        .categoryName = _("SHARP CLAW"),
        .height = 9,
        .weight = 280,
        .description = gSneaselPokedexText,
        .unusedDescription = gSneaselPokedexTextUnused,
        .pokemonScale = 548,
        .pokemonOffset = 10,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_TEDDIURSA] =
    {
        .categoryName = _("LITTLE BEAR"),
        .height = 6,
        .weight = 88,
        .description = gTeddiursaPokedexText,
        .unusedDescription = gTeddiursaPokedexTextUnused,
        .pokemonScale = 667,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_URSARING] =
    {
        .categoryName = _("HIBERNATOR"),
        .height = 18,
        .weight = 1258,
        .description = gUrsaringPokedexText,
        .unusedDescription = gUrsaringPokedexTextUnused,
        .pokemonScale = 345,
        .pokemonOffset = 0,
        .trainerScale = 367,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_SLUGMA] =
    {
        .categoryName = _("LAVA"),
        .height = 7,
        .weight = 350,
        .description = gSlugmaPokedexText,
        .unusedDescription = gSlugmaPokedexTextUnused,
        .pokemonScale = 492,
        .pokemonOffset = 13,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MAGCARGO] =
    {
        .categoryName = _("LAVA"),
        .height = 8,
        .weight = 550,
        .description = gMagcargoPokedexText,
        .unusedDescription = gMagcargoPokedexTextUnused,
        .pokemonScale = 535,
        .pokemonOffset = 14,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SWINUB] =
    {
        .categoryName = _("PIG"),
        .height = 4,
        .weight = 65,
        .description = gSwinubPokedexText,
        .unusedDescription = gSwinubPokedexTextUnused,
        .pokemonScale = 512,
        .pokemonOffset = 17,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_PILOSWINE] =
    {
        .categoryName = _("SWINE"),
        .height = 11,
        .weight = 558,
        .description = gPiloswinePokedexText,
        .unusedDescription = gPiloswinePokedexTextUnused,
        .pokemonScale = 406,
        .pokemonOffset = 8,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_CORSOLA] =
    {
        .categoryName = _("CORAL"),
        .height = 6,
        .weight = 50,
        .description = gCorsolaPokedexText,
        .unusedDescription = gCorsolaPokedexTextUnused,
        .pokemonScale = 605,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_REMORAID] =
    {
        .categoryName = _("JET"),
        .height = 6,
        .weight = 120,
        .description = gRemoraidPokedexText,
        .unusedDescription = gRemoraidPokedexTextUnused,
        .pokemonScale = 543,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_OCTILLERY] =
    {
        .categoryName = _("JET"),
        .height = 9,
        .weight = 285,
        .description = gOctilleryPokedexText,
        .unusedDescription = gOctilleryPokedexTextUnused,
        .pokemonScale = 445,
        .pokemonOffset = 10,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_DELIBIRD] =
    {
        .categoryName = _("DELIVERY"),
        .height = 9,
        .weight = 160,
        .description = gDelibirdPokedexText,
        .unusedDescription = gDelibirdPokedexTextUnused,
        .pokemonScale = 496,
        .pokemonOffset = 11,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MANTINE] =
    {
        .categoryName = _("KITE"),
        .height = 21,
        .weight = 2200,
        .description = gMantinePokedexText,
        .unusedDescription = gMantinePokedexTextUnused,
        .pokemonScale = 351,
        .pokemonOffset = 0,
        .trainerScale = 429,
        .trainerOffset = 5,
    },

    [NATIONAL_DEX_SKARMORY] =
    {
        .categoryName = _("ARMOR BIRD"),
        .height = 17,
        .weight = 505,
        .description = gSkarmoryPokedexText,
        .unusedDescription = gSkarmoryPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 0,
        .trainerScale = 347,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_HOUNDOUR] =
    {
        .categoryName = _("DARK"),
        .height = 6,
        .weight = 108,
        .description = gHoundourPokedexText,
        .unusedDescription = gHoundourPokedexTextUnused,
        .pokemonScale = 636,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_HOUNDOOM] =
    {
        .categoryName = _("DARK"),
        .height = 14,
        .weight = 350,
        .description = gHoundoomPokedexText,
        .unusedDescription = gHoundoomPokedexTextUnused,
        .pokemonScale = 359,
        .pokemonOffset = 4,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_KINGDRA] =
    {
        .categoryName = _("DRAGON"),
        .height = 18,
        .weight = 1520,
        .description = gKingdraPokedexText,
        .unusedDescription = gKingdraPokedexTextUnused,
        .pokemonScale = 328,
        .pokemonOffset = 1,
        .trainerScale = 367,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_PHANPY] =
    {
        .categoryName = _("LONG NOSE"),
        .height = 5,
        .weight = 335,
        .description = gPhanpyPokedexText,
        .unusedDescription = gPhanpyPokedexTextUnused,
        .pokemonScale = 596,
        .pokemonOffset = 16,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_DONPHAN] =
    {
        .categoryName = _("ARMOR"),
        .height = 11,
        .weight = 1200,
        .description = gDonphanPokedexText,
        .unusedDescription = gDonphanPokedexTextUnused,
        .pokemonScale = 406,
        .pokemonOffset = 8,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_PORYGON2] =
    {
        .categoryName = _("VIRTUAL"),
        .height = 6,
        .weight = 325,
        .description = gPorygon2PokedexText,
        .unusedDescription = gPorygon2PokedexTextUnused,
        .pokemonScale = 543,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_PORYGON3] =
    {
        .categoryName = _("VIRTUAL"),
        .height = 6,
        .weight = 325,
        .description = gPorygon2PokedexText,
        .unusedDescription = gPorygon2PokedexTextUnused,
        .pokemonScale = 320,
        .pokemonOffset = 15,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_STANTLER] =
    {
        .categoryName = _("BIG HORN"),
        .height = 14,
        .weight = 712,
        .description = gStantlerPokedexText,
        .unusedDescription = gStantlerPokedexTextUnused,
        .pokemonScale = 419,
        .pokemonOffset = 3,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SMEARGLE] =
    {
        .categoryName = _("PAINTER"),
        .height = 12,
        .weight = 580,
        .description = gSmearglePokedexText,
        .unusedDescription = gSmearglePokedexTextUnused,
        .pokemonScale = 396,
        .pokemonOffset = 6,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_TYROGUE] =
    {
        .categoryName = _("SCUFFLE"),
        .height = 7,
        .weight = 210,
        .description = gTyroguePokedexText,
        .unusedDescription = gTyroguePokedexTextUnused,
        .pokemonScale = 612,
        .pokemonOffset = 13,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_HITMONTOP] =
    {
        .categoryName = _("HANDSTAND"),
        .height = 14,
        .weight = 480,
        .description = gHitmontopPokedexText,
        .unusedDescription = gHitmontopPokedexTextUnused,
        .pokemonScale = 352,
        .pokemonOffset = 3,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SMOOCHUM] =
    {
        .categoryName = _("KISS"),
        .height = 4,
        .weight = 60,
        .description = gSmoochumPokedexText,
        .unusedDescription = gSmoochumPokedexTextUnused,
        .pokemonScale = 838,
        .pokemonOffset = 18,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_ELEKID] =
    {
        .categoryName = _("ELECTRIC"),
        .height = 6,
        .weight = 235,
        .description = gElekidPokedexText,
        .unusedDescription = gElekidPokedexTextUnused,
        .pokemonScale = 683,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MAGBY] =
    {
        .categoryName = _("LIVE COAL"),
        .height = 7,
        .weight = 214,
        .description = gMagbyPokedexText,
        .unusedDescription = gMagbyPokedexTextUnused,
        .pokemonScale = 505,
        .pokemonOffset = 13,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MILTANK] =
    {
        .categoryName = _("MILK COW"),
        .height = 12,
        .weight = 755,
        .description = gMiltankPokedexText,
        .unusedDescription = gMiltankPokedexTextUnused,
        .pokemonScale = 411,
        .pokemonOffset = 6,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_BLISSEY] =
    {
        .categoryName = _("HAPPINESS"),
        .height = 15,
        .weight = 468,
        .description = gBlisseyPokedexText,
        .unusedDescription = gBlisseyPokedexTextUnused,
        .pokemonScale = 317,
        .pokemonOffset = 2,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_RAIKOU] =
    {
        .categoryName = _("THUNDER"),
        .height = 19,
        .weight = 1780,
        .description = gRaikouPokedexText,
        .unusedDescription = gRaikouPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 0,
        .trainerScale = 388,
        .trainerOffset = 3,
    },

    [NATIONAL_DEX_ENTEI] =
    {
        .categoryName = _("VOLCANO"),
        .height = 21,
        .weight = 1980,
        .description = gEnteiPokedexText,
        .unusedDescription = gEnteiPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 0,
        .trainerScale = 429,
        .trainerOffset = 5,
    },

    [NATIONAL_DEX_SUICUNE] =
    {
        .categoryName = _("AURORA"),
        .height = 20,
        .weight = 1870,
        .description = gSuicunePokedexText,
        .unusedDescription = gSuicunePokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 0,
        .trainerScale = 408,
        .trainerOffset = 4,
    },

    [NATIONAL_DEX_LARVITAR] =
    {
        .categoryName = _("ROCK SKIN"),
        .height = 6,
        .weight = 720,
        .description = gLarvitarPokedexText,
        .unusedDescription = gLarvitarPokedexTextUnused,
        .pokemonScale = 683,
        .pokemonOffset = 14,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_PUPITAR] =
    {
        .categoryName = _("HARD SHELL"),
        .height = 12,
        .weight = 1520,
        .description = gPupitarPokedexText,
        .unusedDescription = gPupitarPokedexTextUnused,
        .pokemonScale = 365,
        .pokemonOffset = 7,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_TYRANITAR] =
    {
        .categoryName = _("ARMOR"),
        .height = 20,
        .weight = 2020,
        .description = gTyranitarPokedexText,
        .unusedDescription = gTyranitarPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 0,
        .trainerScale = 408,
        .trainerOffset = 4,
    },

    [NATIONAL_DEX_LUGIA] =
    {
        .categoryName = _("DIVING"),
        .height = 52,
        .weight = 2160,
        .description = gLugiaPokedexText,
        .unusedDescription = gLugiaPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 0,
        .trainerScale = 1061,
        .trainerOffset = 16,
    },

    [NATIONAL_DEX_HO_OH] =
    {
        .categoryName = _("RAINBOW"),
        .height = 38,
        .weight = 1990,
        .description = gHoOhPokedexText,
        .unusedDescription = gHoOhPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 0,
        .trainerScale = 775,
        .trainerOffset = 13,
    },

    [NATIONAL_DEX_CELEBI] =
    {
        .categoryName = _("TIME TRAVEL"),
        .height = 6,
        .weight = 50,
        .description = gCelebiPokedexText,
        .unusedDescription = gCelebiPokedexTextUnused,
        .pokemonScale = 559,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_POOCHYENA] =
    {
        .categoryName = _("BITE"),
        .height = 5,
        .weight = 136,
        .description = gPoochyenaPokedexText,
        .unusedDescription = gPoochyenaPokedexTextUnused,
        .pokemonScale = 745,
        .pokemonOffset = 16,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MIGHTYENA] =
    {
        .categoryName = _("BITE"),
        .height = 10,
        .weight = 370,
        .description = gMightyenaPokedexText,
        .unusedDescription = gMightyenaPokedexTextUnused,
        .pokemonScale = 521,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_ZIGZAGOON] =
    {
        .categoryName = _("TINYRACCOON"),
        .height = 4,
        .weight = 175,
        .description = gZigzagoonPokedexText,
        .unusedDescription = gZigzagoonPokedexTextUnused,
        .pokemonScale = 838,
        .pokemonOffset = 18,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_LINOONE] =
    {
        .categoryName = _("RUSHING"),
        .height = 5,
        .weight = 325,
        .description = gLinoonePokedexText,
        .unusedDescription = gLinoonePokedexTextUnused,
        .pokemonScale = 1117,
        .pokemonOffset = 16,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_WURMPLE] =
    {
        .categoryName = _("WORM"),
        .height = 3,
        .weight = 36,
        .description = gWurmplePokedexText,
        .unusedDescription = gWurmplePokedexTextUnused,
        .pokemonScale = 1117,
        .pokemonOffset = 19,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SILCOON] =
    {
        .categoryName = _("COCOON"),
        .height = 6,
        .weight = 100,
        .description = gSilcoonPokedexText,
        .unusedDescription = gSilcoonPokedexTextUnused,
        .pokemonScale = 512,
        .pokemonOffset = 16,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_BEAUTIFLY] =
    {
        .categoryName = _("BUTTERFLY"),
        .height = 10,
        .weight = 284,
        .description = gBeautiflyPokedexText,
        .unusedDescription = gBeautiflyPokedexTextUnused,
        .pokemonScale = 438,
        .pokemonOffset = 10,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_CASCOON] =
    {
        .categoryName = _("COCOON"),
        .height = 7,
        .weight = 115,
        .description = gCascoonPokedexText,
        .unusedDescription = gCascoonPokedexTextUnused,
        .pokemonScale = 412,
        .pokemonOffset = 13,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_DUSTOX] =
    {
        .categoryName = _("POISON MOTH"),
        .height = 12,
        .weight = 316,
        .description = gDustoxPokedexText,
        .unusedDescription = gDustoxPokedexTextUnused,
        .pokemonScale = 317,
        .pokemonOffset = 9,
        .trainerScale = 349,
        .trainerOffset = 0,
    },




    [NATIONAL_DEX_SEEDOT] =
    {
        .categoryName = _("ACORN"),
        .height = 5,
        .weight = 40,
        .description = gSeedotPokedexText,
        .unusedDescription = gSeedotPokedexTextUnused,
        .pokemonScale = 707,
        .pokemonOffset = 17,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_NUZLEAF] =
    {
        .categoryName = _("WILY"),
        .height = 10,
        .weight = 280,
        .description = gNuzleafPokedexText,
        .unusedDescription = gNuzleafPokedexTextUnused,
        .pokemonScale = 447,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SHIFTRY] =
    {
        .categoryName = _("WICKED"),
        .height = 13,
        .weight = 596,
        .description = gShiftryPokedexText,
        .unusedDescription = gShiftryPokedexTextUnused,
        .pokemonScale = 444,
        .pokemonOffset = 6,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_TAILLOW] =
    {
        .categoryName = _("TINYSWALLOW"),
        .height = 3,
        .weight = 23,
        .description = gTaillowPokedexText,
        .unusedDescription = gTaillowPokedexTextUnused,
        .pokemonScale = 931,
        .pokemonOffset = 19,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SWELLOW] =
    {
        .categoryName = _("SWALLOW"),
        .height = 7,
        .weight = 198,
        .description = gSwellowPokedexText,
        .unusedDescription = gSwellowPokedexTextUnused,
        .pokemonScale = 678,
        .pokemonOffset = 13,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_WINGULL] =
    {
        .categoryName = _("SEAGULL"),
        .height = 6,
        .weight = 95,
        .description = gWingullPokedexText,
        .unusedDescription = gWingullPokedexTextUnused,
        .pokemonScale = 341,
        .pokemonOffset = 17,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_PELIPPER] =
    {
        .categoryName = _("WATER BIRD"),
        .height = 12,
        .weight = 280,
        .description = gPelipperPokedexText,
        .unusedDescription = gPelipperPokedexTextUnused,
        .pokemonScale = 427,
        .pokemonOffset = 6,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_RALTS] =
    {
        .categoryName = _("FEELING"),
        .height = 4,
        .weight = 66,
        .description = gRaltsPokedexText,
        .unusedDescription = gRaltsPokedexTextUnused,
        .pokemonScale = 815,
        .pokemonOffset = 18,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_KIRLIA] =
    {
        .categoryName = _("EMOTION"),
        .height = 8,
        .weight = 202,
        .description = gKirliaPokedexText,
        .unusedDescription = gKirliaPokedexTextUnused,
        .pokemonScale = 582,
        .pokemonOffset = 12,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_GARDEVOIR] =
    {
        .categoryName = _("EMBRACE"),
        .height = 16,
        .weight = 484,
        .description = gGardevoirPokedexText,
        .unusedDescription = gGardevoirPokedexTextUnused,
        .pokemonScale = 367,
        .pokemonOffset = 1,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_GALLADE] =
    {
        .categoryName = _("BLADE"),
        .height = 16,
        .weight = 520,
        .description = gGalladePokedexText,
        .unusedDescription = gGalladePokedexTextUnused,
        .pokemonScale = 372,
        .pokemonOffset = 1,
        .trainerScale = 341,
        .trainerOffset = 0,
    },



    [NATIONAL_DEX_SHROOMISH] =
    {
        .categoryName = _("MUSHROOM"),
        .height = 4,
        .weight = 45,
        .description = gShroomishPokedexText,
        .unusedDescription = gShroomishPokedexTextUnused,
        .pokemonScale = 745,
        .pokemonOffset = 18,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_BRELOOM] =
    {
        .categoryName = _("MUSHROOM"),
        .height = 12,
        .weight = 392,
        .description = gBreloomPokedexText,
        .unusedDescription = gBreloomPokedexTextUnused,
        .pokemonScale = 419,
        .pokemonOffset = 6,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SLAKOTH] =
    {
        .categoryName = _("SLACKER"),
        .height = 8,
        .weight = 240,
        .description = gSlakothPokedexText,
        .unusedDescription = gSlakothPokedexTextUnused,
        .pokemonScale = 349,
        .pokemonOffset = 13,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_VIGOROTH] =
    {
        .categoryName = _("WILD MONKEY"),
        .height = 14,
        .weight = 465,
        .description = gVigorothPokedexText,
        .unusedDescription = gVigorothPokedexTextUnused,
        .pokemonScale = 426,
        .pokemonOffset = 4,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SLAKING] =
    {
        .categoryName = _("LAZY"),
        .height = 20,
        .weight = 1305,
        .description = gSlakingPokedexText,
        .unusedDescription = gSlakingPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 6,
        .trainerScale = 568,
        .trainerOffset = 9,
    },

    [NATIONAL_DEX_NINCADA] =
    {
        .categoryName = _("TRAINEE"),
        .height = 5,
        .weight = 55,
        .description = gNincadaPokedexText,
        .unusedDescription = gNincadaPokedexTextUnused,
        .pokemonScale = 521,
        .pokemonOffset = 16,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_NINJASK] =
    {
        .categoryName = _("NINJA"),
        .height = 8,
        .weight = 120,
        .description = gNinjaskPokedexText,
        .unusedDescription = gNinjaskPokedexTextUnused,
        .pokemonScale = 454,
        .pokemonOffset = 11,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SHEDINJA] =
    {
        .categoryName = _("SHED"),
        .height = 8,
        .weight = 12,
        .description = gShedinjaPokedexText,
        .unusedDescription = gShedinjaPokedexTextUnused,
        .pokemonScale = 512,
        .pokemonOffset = 12,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_WHISMUR] =
    {
        .categoryName = _("WHISPER"),
        .height = 6,
        .weight = 163,
        .description = gWhismurPokedexText,
        .unusedDescription = gWhismurPokedexTextUnused,
        .pokemonScale = 574,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_LOUDRED] =
    {
        .categoryName = _("BIG VOICE"),
        .height = 10,
        .weight = 405,
        .description = gLoudredPokedexText,
        .unusedDescription = gLoudredPokedexTextUnused,
        .pokemonScale = 531,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_EXPLOUD] =
    {
        .categoryName = _("LOUD NOISE"),
        .height = 15,
        .weight = 840,
        .description = gExploudPokedexText,
        .unusedDescription = gExploudPokedexTextUnused,
        .pokemonScale = 385,
        .pokemonOffset = 2,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MAKUHITA] =
    {
        .categoryName = _("GUTS"),
        .height = 10,
        .weight = 864,
        .description = gMakuhitaPokedexText,
        .unusedDescription = gMakuhitaPokedexTextUnused,
        .pokemonScale = 372,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_HARIYAMA] =
    {
        .categoryName = _("ARM THRUST"),
        .height = 23,
        .weight = 2538,
        .description = gHariyamaPokedexText,
        .unusedDescription = gHariyamaPokedexTextUnused,
        .pokemonScale = 351,
        .pokemonOffset = 0,
        .trainerScale = 485,
        .trainerOffset = 7,
    },

    [NATIONAL_DEX_AZURILL] =
    {
        .categoryName = _("POLKA DOT"),
        .height = 2,
        .weight = 20,
        .description = gAzurillPokedexText,
        .unusedDescription = gAzurillPokedexTextUnused,
        .pokemonScale = 1722,
        .pokemonOffset = 20,
        .trainerScale = 341,
        .trainerOffset = 0,
    },




    [NATIONAL_DEX_SABLEYE] =
    {
        .categoryName = _("DARKNESS"),
        .height = 5,
        .weight = 110,
        .description = gSableyePokedexText,
        .unusedDescription = gSableyePokedexTextUnused,
        .pokemonScale = 819,
        .pokemonOffset = 16,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MAWILE] =
    {
        .categoryName = _("DECEIVER"),
        .height = 6,
        .weight = 115,
        .description = gMawilePokedexText,
        .unusedDescription = gMawilePokedexTextUnused,
        .pokemonScale = 729,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MEDITITE] =
    {
        .categoryName = _("MEDITATE"),
        .height = 6,
        .weight = 112,
        .description = gMedititePokedexText,
        .unusedDescription = gMedititePokedexTextUnused,
        .pokemonScale = 621,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MEDICHAM] =
    {
        .categoryName = _("MEDITATE"),
        .height = 13,
        .weight = 315,
        .description = gMedichamPokedexText,
        .unusedDescription = gMedichamPokedexTextUnused,
        .pokemonScale = 437,
        .pokemonOffset = 5,
        .trainerScale = 341,
        .trainerOffset = 0,
    },



    [NATIONAL_DEX_PLUSLE] =
    {
        .categoryName = _("CHEERING"),
        .height = 4,
        .weight = 42,
        .description = gPluslePokedexText,
        .unusedDescription = gPluslePokedexTextUnused,
        .pokemonScale = 931,
        .pokemonOffset = 18,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MINUN] =
    {
        .categoryName = _("CHEERING"),
        .height = 4,
        .weight = 42,
        .description = gMinunPokedexText,
        .unusedDescription = gMinunPokedexTextUnused,
        .pokemonScale = 977,
        .pokemonOffset = 18,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_VOLBEAT] =
    {
        .categoryName = _("FIREFLY"),
        .height = 7,
        .weight = 177,
        .description = gVolbeatPokedexText,
        .unusedDescription = gVolbeatPokedexTextUnused,
        .pokemonScale = 598,
        .pokemonOffset = 13,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_ILLUMISE] =
    {
        .categoryName = _("FIREFLY"),
        .height = 6,
        .weight = 177,
        .description = gIllumisePokedexText,
        .unusedDescription = gIllumisePokedexTextUnused,
        .pokemonScale = 714,
        .pokemonOffset = 14,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_ROSELIA] =
    {
        .categoryName = _("THORN"),
        .height = 3,
        .weight = 20,
        .description = gRoseliaPokedexText,
        .unusedDescription = gRoseliaPokedexTextUnused,
        .pokemonScale = 1489,
        .pokemonOffset = 19,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_GULPIN] =
    {
        .categoryName = _("STOMACH"),
        .height = 4,
        .weight = 103,
        .description = gGulpinPokedexText,
        .unusedDescription = gGulpinPokedexTextUnused,
        .pokemonScale = 791,
        .pokemonOffset = 18,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SWALOT] =
    {
        .categoryName = _("POISON BAG"),
        .height = 17,
        .weight = 800,
        .description = gSwalotPokedexText,
        .unusedDescription = gSwalotPokedexTextUnused,
        .pokemonScale = 267,
        .pokemonOffset = 0,
        .trainerScale = 347,
        .trainerOffset = 0,
    },





    [NATIONAL_DEX_NUMEL] =
    {
        .categoryName = _("NUMB"),
        .height = 7,
        .weight = 240,
        .description = gNumelPokedexText,
        .unusedDescription = gNumelPokedexTextUnused,
        .pokemonScale = 439,
        .pokemonOffset = 13,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_CAMERUPT] =
    {
        .categoryName = _("ERUPTION"),
        .height = 19,
        .weight = 2200,
        .description = gCameruptPokedexText,
        .unusedDescription = gCameruptPokedexTextUnused,
        .pokemonScale = 339,
        .pokemonOffset = 6,
        .trainerScale = 473,
        .trainerOffset = 6,
    },


    [NATIONAL_DEX_SPOINK] =
    {
        .categoryName = _("BOUNCE"),
        .height = 7,
        .weight = 306,
        .description = gSpoinkPokedexText,
        .unusedDescription = gSpoinkPokedexTextUnused,
        .pokemonScale = 625,
        .pokemonOffset = 14,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_GRUMPIG] =
    {
        .categoryName = _("MANIPULATE"),
        .height = 9,
        .weight = 715,
        .description = gGrumpigPokedexText,
        .unusedDescription = gGrumpigPokedexTextUnused,
        .pokemonScale = 569,
        .pokemonOffset = 11,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SPINDA] =
    {
        .categoryName = _("SPOT PANDA"),
        .height = 11,
        .weight = 50,
        .description = gSpindaPokedexText,
        .unusedDescription = gSpindaPokedexTextUnused,
        .pokemonScale = 415,
        .pokemonOffset = 8,
        .trainerScale = 341,
        .trainerOffset = 0,
    },






    [NATIONAL_DEX_SWABLU] =
    {
        .categoryName = _("COTTON BIRD"),
        .height = 4,
        .weight = 12,
        .description = gSwabluPokedexText,
        .unusedDescription = gSwabluPokedexTextUnused,
        .pokemonScale = 838,
        .pokemonOffset = 18,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_ALTARIA] =
    {
        .categoryName = _("HUMMING"),
        .height = 11,
        .weight = 206,
        .description = gAltariaPokedexText,
        .unusedDescription = gAltariaPokedexTextUnused,
        .pokemonScale = 499,
        .pokemonOffset = 8,
        .trainerScale = 341,
        .trainerOffset = 0,
    },



    [NATIONAL_DEX_LUNATONE] =
    {
        .categoryName = _("METEORITE"),
        .height = 10,
        .weight = 1680,
        .description = gLunatonePokedexText,
        .unusedDescription = gLunatonePokedexTextUnused,
        .pokemonScale = 419,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SOLROCK] =
    {
        .categoryName = _("METEORITE"),
        .height = 12,
        .weight = 1540,
        .description = gSolrockPokedexText,
        .unusedDescription = gSolrockPokedexTextUnused,
        .pokemonScale = 481,
        .pokemonOffset = 7,
        .trainerScale = 341,
        .trainerOffset = 0,
    },


    [NATIONAL_DEX_BARBOACH] =
    {
        .categoryName = _("WHISKERS"),
        .height = 4,
        .weight = 19,
        .description = gBarboachPokedexText,
        .unusedDescription = gBarboachPokedexTextUnused,
        .pokemonScale = 977,
        .pokemonOffset = 17,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_WHISCASH] =
    {
        .categoryName = _("WHISKERS"),
        .height = 9,
        .weight = 236,
        .description = gWhiscashPokedexText,
        .unusedDescription = gWhiscashPokedexTextUnused,
        .pokemonScale = 476,
        .pokemonOffset = 11,
        .trainerScale = 341,
        .trainerOffset = 0,
    },



    [NATIONAL_DEX_BALTOY] =
    {
        .categoryName = _("CLAY DOLL"),
        .height = 5,
        .weight = 215,
        .description = gBaltoyPokedexText,
        .unusedDescription = gBaltoyPokedexTextUnused,
        .pokemonScale = 652,
        .pokemonOffset = 17,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_CLAYDOL] =
    {
        .categoryName = _("CLAY DOLL"),
        .height = 15,
        .weight = 1080,
        .description = gClaydolPokedexText,
        .unusedDescription = gClaydolPokedexTextUnused,
        .pokemonScale = 341,
        .pokemonOffset = 3,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_LILEEP] =
    {
        .categoryName = _("SEA LILY"),
        .height = 10,
        .weight = 238,
        .description = gLileepPokedexText,
        .unusedDescription = gLileepPokedexTextUnused,
        .pokemonScale = 465,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_CRADILY] =
    {
        .categoryName = _("BARNACLE"),
        .height = 15,
        .weight = 604,
        .description = gCradilyPokedexText,
        .unusedDescription = gCradilyPokedexTextUnused,
        .pokemonScale = 391,
        .pokemonOffset = 2,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_ANORITH] =
    {
        .categoryName = _("OLD SHRIMP"),
        .height = 7,
        .weight = 125,
        .description = gAnorithPokedexText,
        .unusedDescription = gAnorithPokedexTextUnused,
        .pokemonScale = 638,
        .pokemonOffset = 13,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_ARMALDO] =
    {
        .categoryName = _("PLATE"),
        .height = 15,
        .weight = 682,
        .description = gArmaldoPokedexText,
        .unusedDescription = gArmaldoPokedexTextUnused,
        .pokemonScale = 397,
        .pokemonOffset = 2,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_FEEBAS] =
    {
        .categoryName = _("FISH"),
        .height = 6,
        .weight = 74,
        .description = gFeebasPokedexText,
        .unusedDescription = gFeebasPokedexTextUnused,
        .pokemonScale = 652,
        .pokemonOffset = 16,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MILOTIC] =
    {
        .categoryName = _("TENDER"),
        .height = 62,
        .weight = 1620,
        .description = gMiloticPokedexText,
        .unusedDescription = gMiloticPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 0,
        .trainerScale = 1265,
        .trainerOffset = 17,
    },





    [NATIONAL_DEX_DUSKULL] =
    {
        .categoryName = _("REQUIEM"),
        .height = 8,
        .weight = 150,
        .description = gDuskullPokedexText,
        .unusedDescription = gDuskullPokedexTextUnused,
        .pokemonScale = 535,
        .pokemonOffset = 12,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_DUSCLOPS] =
    {
        .categoryName = _("BECKON"),
        .height = 16,
        .weight = 306,
        .description = gDusclopsPokedexText,
        .unusedDescription = gDusclopsPokedexTextUnused,
        .pokemonScale = 308,
        .pokemonOffset = 1,
        .trainerScale = 341,
        .trainerOffset = 0,
    },


    [NATIONAL_DEX_CHIMECHO] =
    {
        .categoryName = _("WIND CHIME"),
        .height = 6,
        .weight = 10,
        .description = gChimechoPokedexText,
        .unusedDescription = gChimechoPokedexTextUnused,
        .pokemonScale = 822,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_ABSOL] =
    {
        .categoryName = _("DISASTER"),
        .height = 12,
        .weight = 470,
        .description = gAbsolPokedexText,
        .unusedDescription = gAbsolPokedexTextUnused,
        .pokemonScale = 489,
        .pokemonOffset = 6,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_WYNAUT] =
    {
        .categoryName = _("BRIGHT"),
        .height = 6,
        .weight = 140,
        .description = gWynautPokedexText,
        .unusedDescription = gWynautPokedexTextUnused,
        .pokemonScale = 621,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },






    [NATIONAL_DEX_CLAMPERL] =
    {
        .categoryName = _("BIVALVE"),
        .height = 4,
        .weight = 525,
        .description = gClamperlPokedexText,
        .unusedDescription = gClamperlPokedexTextUnused,
        .pokemonScale = 815,
        .pokemonOffset = 17,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_HUNTAIL] =
    {
        .categoryName = _("DEEP SEA"),
        .height = 17,
        .weight = 270,
        .description = gHuntailPokedexText,
        .unusedDescription = gHuntailPokedexTextUnused,
        .pokemonScale = 312,
        .pokemonOffset = -1,
        .trainerScale = 347,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_GOREBYSS] =
    {
        .categoryName = _("SOUTH SEA"),
        .height = 18,
        .weight = 226,
        .description = gGorebyssPokedexText,
        .unusedDescription = gGorebyssPokedexTextUnused,
        .pokemonScale = 339,
        .pokemonOffset = 7,
        .trainerScale = 498,
        .trainerOffset = 7,
    },

    [NATIONAL_DEX_RELICANTH] =
    {
        .categoryName = _("LONGEVITY"),
        .height = 10,
        .weight = 234,
        .description = gRelicanthPokedexText,
        .unusedDescription = gRelicanthPokedexTextUnused,
        .pokemonScale = 438,
        .pokemonOffset = 11,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_LUVDISC] =
    {
        .categoryName = _("RENDEZVOUS"),
        .height = 6,
        .weight = 87,
        .description = gLuvdiscPokedexText,
        .unusedDescription = gLuvdiscPokedexTextUnused,
        .pokemonScale = 481,
        .pokemonOffset = 19,
        .trainerScale = 341,
        .trainerOffset = 0,
    },










    [NATIONAL_DEX_BUNEARY] =
    {
        .categoryName = _("RABBIT"),
        .height = 4,
        .weight = 55,
        .description = gBunearyPokedexText,
        .unusedDescription = gBunearyPokedexTextUnused,
        .pokemonScale = 1140,
        .pokemonOffset = 19,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_LOPUNNY] =
    {
        .categoryName = _("RABBIT"),
        .height = 12,
        .weight = 333,
        .description = gLopunnyPokedexText,
        .unusedDescription = gLopunnyPokedexTextUnused,
        .pokemonScale = 442,
        .pokemonOffset = 7,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_STARLY] =
    {
        .categoryName = _("STARLING"),
        .height = 3,
        .weight = 20,
        .description = gStarlyPokedexText,
        .unusedDescription = gStarlyPokedexTextUnused,
        .pokemonScale = 1210,
        .pokemonOffset = 19,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_STARAVIA] =
    {
        .categoryName = _("STARLING"),
        .height = 6,
        .weight = 155,
        .description = gStaraviaPokedexText,
        .unusedDescription = gStaraviaPokedexTextUnused,
        .pokemonScale = 807,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_STARAPTOR] =
    {
        .categoryName = _("PREDATOR"),
        .height = 12,
        .weight = 249,
        .description = gStaraptorPokedexText,
        .unusedDescription = gStaraptorPokedexTextUnused,
        .pokemonScale = 496,
        .pokemonOffset = 6,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_OMATO] =
    {
        .categoryName = _("SPIRAL"),
        .height = 1,
        .weight = 1,
        .description = gOmatoPokedexText,
        .unusedDescription = gOmatoPokedexTextUnused,
        .pokemonScale = 3351,
        .pokemonOffset = 22,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_OMATOPS] =
    {
        .categoryName = _("SPINNING"),
        .height = 1,
        .weight = 1,
        .description = gOmatopsPokedexText,
        .unusedDescription = gOmatopsPokedexTextUnused,
        .pokemonScale = 4468,
        .pokemonOffset = 22,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_COMBEE] =
    {
        .categoryName = _("TINY BEE"),
        .height = 3,
        .weight = 55,
        .description = gCombeePokedexText,
        .unusedDescription = gCombeePokedexTextUnused,
        .pokemonScale = 1024,
        .pokemonOffset = 19,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_VESPIQUEN] =
    {
        .categoryName = _("BEEHIVE"),
        .height = 12,
        .weight = 385,
        .description = gVespiquenPokedexText,
        .unusedDescription = gVespiquenPokedexTextUnused,
        .pokemonScale = 434,
        .pokemonOffset = 7,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_BUDEW] =
    {
        .categoryName = _("BUD"),
        .height = 2,
        .weight = 12,
        .description = gBudewPokedexText,
        .unusedDescription = gBudewPokedexTextUnused,
        .pokemonScale = 1583,
        .pokemonOffset = 20,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_ROSERADE] =
    {
        .categoryName = _("BOUQUET"),
        .height = 9,
        .weight = 145,
        .description = gRoseradePokedexText,
        .unusedDescription = gRoseradePokedexTextUnused,
        .pokemonScale = 528,
        .pokemonOffset = 11,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MAGNEZONE] =
    {
        .categoryName = _("MAGNET AREA"),
        .height = 12,
        .weight = 1800,
        .description = gMagnezonePokedexText,
        .unusedDescription = gMagnezonePokedexTextUnused,
        .pokemonScale = 388,
        .pokemonOffset = 7,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_ELECTIVIRE] =
    {
        .categoryName = _("THUNDERBLT"),
        .height = 18,
        .weight = 1386,
        .description = gElectivirePokedexText,
        .unusedDescription = gElectivirePokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 0,
        .trainerScale = 373,
        .trainerOffset = 2,
    },

    [NATIONAL_DEX_AMBIPOM] =
    {
        .categoryName = _("LONG TAIL"),
        .height = 12,
        .weight = 203,
        .description = gAmbipomPokedexText,
        .unusedDescription = gAmbipomPokedexTextUnused,
        .pokemonScale = 496,
        .pokemonOffset = 6,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_ANNIHILAPE] =
    {
        .categoryName = _("RAGE MONKEY"),
        .height = 12,
        .weight = 560,
        .description = gAnnihilapePokedexText,
        .unusedDescription = gAnnihilapePokedexTextUnused,
        .pokemonScale = 489,
        .pokemonOffset = 7,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_HONCHKROW] =
    {
        .categoryName = _("BIG BOSS"),
        .height = 9,
        .weight = 273,
        .description = gHonchkrowPokedexText,
        .unusedDescription = gHonchkrowPokedexTextUnused,
        .pokemonScale = 548,
        .pokemonOffset = 11,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_CHINGLING] =
    {
        .categoryName = _("BELL"),
        .height = 2,
        .weight = 6,
        .description = gChinglingPokedexText,
        .unusedDescription = gChinglingPokedexTextUnused,
        .pokemonScale = 1722,
        .pokemonOffset = 20,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_BRONZOR] =
    {
        .categoryName = _("BRONZE"),
        .height = 5,
        .weight = 605,
        .description = gBronzorPokedexText,
        .unusedDescription = gBronzorPokedexTextUnused,
        .pokemonScale = 540,
        .pokemonOffset = 16,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_BRONZONG] =
    {
        .categoryName = _("BRONZE BELL"),
        .height = 13,
        .weight = 1870,
        .description = gBronzongPokedexText,
        .unusedDescription = gBronzongPokedexTextUnused,
        .pokemonScale = 351,
        .pokemonOffset = 4,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MIME_JR] =
    {
        .categoryName = _("MIME"),
        .height = 6,
        .weight = 130,
        .description = gMimeJrPokedexText,
        .unusedDescription = gMimeJrPokedexTextUnused,
        .pokemonScale = 745,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MIME_SR] =
    {
        .categoryName = _("MIME"),
        .height = 18,
        .weight = 100,
        .description = gMimeSrPokedexText,
        .unusedDescription = gMimeSrPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 1,
        .trainerScale = 392,
        .trainerOffset = 3,
    },

    [NATIONAL_DEX_MISMAGIUS] =
    {
        .categoryName = _("MAGICAL"),
        .height = 9,
        .weight = 44,
        .description = gMismagiusPokedexText,
        .unusedDescription = gMismagiusPokedexTextUnused,
        .pokemonScale = 621,
        .pokemonOffset = 11,
        .trainerScale = 341,
        .trainerOffset = 0,
    },


    [NATIONAL_DEX_DUSKNOIR] =
    {
        .categoryName = _("GRIPPER"),
        .height = 22,
        .weight = 1066,
        .description = gDusknoirPokedexText,
        .unusedDescription = gDusknoirPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 1,
        .trainerScale = 504,
        .trainerOffset = 7,
    },

    [NATIONAL_DEX_OSSCYTHE] =
    {
        .categoryName = _("UNKNOWN"),
        .height = 16,
        .weight = 100,
        .description = gOsscythePokedexText,
        .unusedDescription = gOsscythePokedexTextUnused,
        .pokemonScale = 372,
        .pokemonOffset = 1,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_RHYPERIOR] =
    {
        .categoryName = _("DRILL"),
        .height = 24,
        .weight = 2828,
        .description = gRhyperiorPokedexText,
        .unusedDescription = gRhyperiorPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 0,
        .trainerScale = 498,
        .trainerOffset = 7,
    },

    [NATIONAL_DEX_CINNABAR_FEEBAS] =
    {
        .categoryName = _("DEEP DRAGON"),
        .height = 16,
        .weight = 100,
        .description = gCinnabarFeebasPokedexText,
        .unusedDescription = gCinnabarFeebasPokedexTextUnused,
        .pokemonScale = 320,
        .pokemonOffset = 1,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_WEAVILE] =
    {
        .categoryName = _("SHARP CLAW"),
        .height = 11,
        .weight = 340,
        .description = gWeavilePokedexText,
        .unusedDescription = gWeavilePokedexTextUnused,
        .pokemonScale = 457,
        .pokemonOffset = 8,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MAMOSWINE] =
    {
        .categoryName = _("TWIN TUSK"),
        .height = 25,
        .weight = 2910,
        .description = gMamoswinePokedexText,
        .unusedDescription = gMamoswinePokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 1,
        .trainerScale = 628,
        .trainerOffset = 10,
    },


    [NATIONAL_DEX_MAGMORTAR] =
    {
        .categoryName = _("BLAST"),
        .height = 16,
        .weight = 680,
        .description = gMagmortarPokedexText,
        .unusedDescription = gMagmortarPokedexTextUnused,
        .pokemonScale = 367,
        .pokemonOffset = 1,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_KABUSTAR] =
    {
        .categoryName = _("CLINGING"),
        .height = 10,
        .weight = 100,
        .description = gKabustarPokedexText,
        .unusedDescription = gKabustarPokedexTextUnused,
        .pokemonScale = 391,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_KABUKNIGHT] =
    {
        .categoryName = _("ARMORED"),
        .height = 10,
        .weight = 100,
        .description = gKabuknightPokedexText,
        .unusedDescription = gKabuknightPokedexTextUnused,
        .pokemonScale = 559,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_AMUNYTE] =
    {
        .categoryName = _("VENOM SHELL"),
        .height = 9,
        .weight = 240,
        .description = gAmunytePokedexText,
        .unusedDescription = gAmunytePokedexTextUnused,
        .pokemonScale = 641,
        .pokemonOffset = 11,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_KINKABUTO] =
    {
        .categoryName = _("BLADE SHELL"),
        .height = 11,
        .weight = 320,
        .description = gKinkabutoPokedexText,
        .unusedDescription = gKinkabutoPokedexTextUnused,
        .pokemonScale = 508,
        .pokemonOffset = 8,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_AEROPTERYX] =
    {
        .categoryName = _("PRIMEVAL"),
        .height = 10,
        .weight = 100,
        .description = gAeropteryxPokedexText,
        .unusedDescription = gAeropteryxPokedexTextUnused,
        .pokemonScale = 568,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_PORYGON_Z] =
    {
        .categoryName = _("VIRTUAL"),
        .height = 9,
        .weight = 340,
        .description = gPorygonZPokedexText,
        .unusedDescription = gPorygonZPokedexTextUnused,
        .pokemonScale = 528,
        .pokemonOffset = 10,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_LEAFEON] =
    {
        .categoryName = _("VERDANT"),
        .height = 10,
        .weight = 255,
        .description = gLeafeonPokedexText,
        .unusedDescription = gLeafeonPokedexTextUnused,
        .pokemonScale = 465,
        .pokemonOffset = 8,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_GLACEON] =
    {
        .categoryName = _("FRESH SNOW"),
        .height = 8,
        .weight = 259,
        .description = gGlaceonPokedexText,
        .unusedDescription = gGlaceonPokedexTextUnused,
        .pokemonScale = 617,
        .pokemonOffset = 11,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SYLVEON] =
    {
        .categoryName = _("INTERTWIN"),
        .height = 10,
        .weight = 235,
        .description = gSylveonPokedexText,
        .unusedDescription = gSylveonPokedexTextUnused,
        .pokemonScale = 503,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_FARFETCHD_GALARIAN] =
    {
        .categoryName = _("WILD DUCK"),
        .height = 8,
        .weight = 420,
        .description = gFarfetchdGalarianPokedexText,
        .unusedDescription = gFarfetchdGalarianPokedexTextUnused,
        .pokemonScale = 547,
        .pokemonOffset = 12,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_TYNAMO] =
    {
        .categoryName = _("ELEFISH"),
        .height = 2,
        .weight = 3,
        .description = gTynamoPokedexText,
        .unusedDescription = gTynamoPokedexTextUnused,
        .pokemonScale = 791,
        .pokemonOffset = 20,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_EELEKTRIK] =
    {
        .categoryName = _("ELEFISH"),
        .height = 12,
        .weight = 220,
        .description = gEelektrikPokedexText,
        .unusedDescription = gEelektrikPokedexTextUnused,
        .pokemonScale = 489,
        .pokemonOffset = 6,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_EELEKTROSS] =
    {
        .categoryName = _("ELEFISH"),
        .height = 21,
        .weight = 805,
        .description = gEelektrossPokedexText,
        .unusedDescription = gEelektrossPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 1,
        .trainerScale = 560,
        .trainerOffset = 9,
    },

    [NATIONAL_DEX_RATTATA_ALOLAN] =
    {
        .categoryName = _("MOUSE"),
        .height = 3,
        .weight = 38,
        .description = gRattataAlolanPokedexText,
        .unusedDescription = gRattataAlolanPokedexTextUnused,
        .pokemonScale = 1210,
        .pokemonOffset = 19,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_RATICATE_ALOLAN] =
    {
        .categoryName = _("MOUSE"),
        .height = 7,
        .weight = 255,
        .description = gRaticateAlolanPokedexText,
        .unusedDescription = gRaticateAlolanPokedexTextUnused,
        .pokemonScale = 598,
        .pokemonOffset = 13,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_PICHU_ALOLAN] =
    {
        .categoryName = _("UNKNOWN"),
        .height = 10,
        .weight = 100,
        .description = gPichuAlolanPokedexText,
        .unusedDescription = gPichuAlolanPokedexTextUnused,
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_PIKACHU_ALOLAN] =
    {
        .categoryName = _("UNKNOWN"),
        .height = 10,
        .weight = 100,
        .description = gPikachuAlolanPokedexText,
        .unusedDescription = gPikachuAlolanPokedexTextUnused,
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_RAICHU_ALOLAN] =
    {
        .categoryName = _("MOUSE"),
        .height = 7,
        .weight = 210,
        .description = gRaichuAlolanPokedexText,
        .unusedDescription = gRaichuAlolanPokedexTextUnused,
        .pokemonScale = 851,
        .pokemonOffset = 13,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SANDSHREW_ALOLAN] =
    {
        .categoryName = _("MOUSE"),
        .height = 7,
        .weight = 400,
        .description = gSandshrewAlolanPokedexText,
        .unusedDescription = gSandshrewAlolanPokedexTextUnused,
        .pokemonScale = 505,
        .pokemonOffset = 13,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SANDSLASH_ALOLAN] =
    {
        .categoryName = _("MOUSE"),
        .height = 12,
        .weight = 550,
        .description = gSandslashAlolanPokedexText,
        .unusedDescription = gSandslashAlolanPokedexTextUnused,
        .pokemonScale = 481,
        .pokemonOffset = 7,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_DIGLETT_ALOLAN] =
    {
        .categoryName = _("MOLE"),
        .height = 2,
        .weight = 10,
        .description = gDiglettAlolanPokedexText,
        .unusedDescription = gDiglettAlolanPokedexTextUnused,
        .pokemonScale = 1489,
        .pokemonOffset = 21,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_DUGTRIO_ALOLAN] =
    {
        .categoryName = _("MOLE"),
        .height = 7,
        .weight = 666,
        .description = gDugtrioAlolanPokedexText,
        .unusedDescription = gDugtrioAlolanPokedexTextUnused,
        .pokemonScale = 585,
        .pokemonOffset = 14,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MEOWTH_ALOLAN] =
    {
        .categoryName = _("SCRATCHCAT"),
        .height = 4,
        .weight = 42,
        .description = gMeowthAlolanPokedexText,
        .unusedDescription = gMeowthAlolanPokedexTextUnused,
        .pokemonScale = 1024,
        .pokemonOffset = 17,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_PERSIAN_ALOLAN] =
    {
        .categoryName = _("CLASSY CAT"),
        .height = 11,
        .weight = 330,
        .description = gPersianAlolanPokedexText,
        .unusedDescription = gPersianAlolanPokedexTextUnused,
        .pokemonScale = 449,
        .pokemonOffset = 5,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MEOWTH_GALARIAN] =
    {
        .categoryName = _("SCRATCHCAT"),
        .height = 4,
        .weight = 75,
        .description = gMeowthGalarianPokedexText,
        .unusedDescription = gMeowthGalarianPokedexTextUnused,
        .pokemonScale = 1071,
        .pokemonOffset = 18,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_PERRSERKER] =
    {
        .categoryName = _("VIKING"),
        .height = 8,
        .weight = 280,
        .description = gPerrserkerPokedexText,
        .unusedDescription = gPerrserkerPokedexTextUnused,
        .pokemonScale = 698,
        .pokemonOffset = 12,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_GEODUDE_ALOLAN] =
    {
        .categoryName = _("ROCK"),
        .height = 4,
        .weight = 203,
        .description = gGeodudeAlolanPokedexText,
        .unusedDescription = gGeodudeAlolanPokedexTextUnused,
        .pokemonScale = 605,
        .pokemonOffset = 17,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_GRAVELER_ALOLAN] =
    {
        .categoryName = _("ROCK"),
        .height = 10,
        .weight = 1100,
        .description = gGravelerAlolanPokedexText,
        .unusedDescription = gGravelerAlolanPokedexTextUnused,
        .pokemonScale = 456,
        .pokemonOffset = 10,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_GOLEM_ALOLAN] =
    {
        .categoryName = _("MEGATON"),
        .height = 17,
        .weight = 3160,
        .description = gGolemAlolanPokedexText,
        .unusedDescription = gGolemAlolanPokedexTextUnused,
        .pokemonScale = 339,
        .pokemonOffset = 0,
        .trainerScale = 347,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_GRIMER_ALOLAN] =
    {
        .categoryName = _("SLUDGE"),
        .height = 7,
        .weight = 420,
        .description = gGrimerAlolanPokedexText,
        .unusedDescription = gGrimerAlolanPokedexTextUnused,
        .pokemonScale = 532,
        .pokemonOffset = 13,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MUK_ALOLAN] =
    {
        .categoryName = _("SLUDGE"),
        .height = 10,
        .weight = 520,
        .description = gMukAlolanPokedexText,
        .unusedDescription = gMukAlolanPokedexTextUnused,
        .pokemonScale = 521,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_VULPIX_ALOLAN] =
    {
        .categoryName = _("FOX"),
        .height = 6,
        .weight = 99,
        .description = gVulpixAlolanPokedexText,
        .unusedDescription = gVulpixAlolanPokedexTextUnused,
        .pokemonScale = 698,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_NINETALES_ALOLAN] =
    {
        .categoryName = _("FOX"),
        .height = 11,
        .weight = 199,
        .description = gNinetalesAlolanPokedexText,
        .unusedDescription = gNinetalesAlolanPokedexTextUnused,
        .pokemonScale = 525,
        .pokemonOffset = 8,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_EXEGGCUTE_ALOLAN] =
    {
        .categoryName = _("UNKNOWN"),
        .height = 10,
        .weight = 100,
        .description = gExeggcuteAlolanPokedexText,
        .unusedDescription = gExeggcuteAlolanPokedexTextUnused,
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_EXEGGUTOR_ALOLAN] =
    {
        .categoryName = _("COCONUT"),
        .height = 109,
        .weight = 4156,
        .description = gExeggutorAlolanPokedexText,
        .unusedDescription = gExeggutorAlolanPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 1,
        .trainerScale = 2296,
        .trainerOffset = 20,
    },

    [NATIONAL_DEX_CUBONE_ALOLAN] =
    {
        .categoryName = _("UNKNOWN"),
        .height = 10,
        .weight = 100,
        .description = gCuboneAlolanPokedexText,
        .unusedDescription = gCuboneAlolanPokedexTextUnused,
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_MAROWAK_ALOLAN] =
    {
        .categoryName = _("BONE KEEPER"),
        .height = 10,
        .weight = 340,
        .description = gMarowakAlolanPokedexText,
        .unusedDescription = gMarowakAlolanPokedexTextUnused,
        .pokemonScale = 531,
        .pokemonOffset = 10,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_CORSOLA_GALARIAN] =
    {
        .categoryName = _("CORAL"),
        .height = 6,
        .weight = 5,
        .description = gCorsolaGalarianPokedexText,
        .unusedDescription = gCorsolaGalarianPokedexTextUnused,
        .pokemonScale = 652,
        .pokemonOffset = 15,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_CURSOLA] =
    {
        .categoryName = _("CORAL"),
        .height = 10,
        .weight = 4,
        .description = gCursolaPokedexText,
        .unusedDescription = gCursolaPokedexTextUnused,
        .pokemonScale = 596,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_KOFFING_GALARIAN] =
    {
        .categoryName = _("UNKNOWN"),
        .height = 10,
        .weight = 100,
        .description = gKoffingGalarianPokedexText,
        .unusedDescription = gKoffingGalarianPokedexTextUnused,
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_WEEZING_GALARIAN] =
    {
        .categoryName = _("POISON GAS"),
        .height = 30,
        .weight = 160,
        .description = gWeezingGalarianPokedexText,
        .unusedDescription = gWeezingGalarianPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 0,
        .trainerScale = 612,
        .trainerOffset = 10,
    },

    [NATIONAL_DEX_KRICKETOT] =
    {
        .categoryName = _("CRICKET"),
        .height = 3,
        .weight = 22,
        .description = gKricketotPokedexText,
        .unusedDescription = gKricketotPokedexTextUnused,
        .pokemonScale = 1334,
        .pokemonOffset = 19,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_KRICKETUNE] =
    {
        .categoryName = _("CRICKET"),
        .height = 10,
        .weight = 255,
        .description = gKricketunePokedexText,
        .unusedDescription = gKricketunePokedexTextUnused,
        .pokemonScale = 512,
        .pokemonOffset = 9,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_PONYTA_GALARIAN] =
    {
        .categoryName = _("UNIQUE HORN"),
        .height = 8,
        .weight = 240,
        .description = gPonytaGalarianPokedexText,
        .unusedDescription = gPonytaGalarianPokedexTextUnused,
        .pokemonScale = 570,
        .pokemonOffset = 12,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_RAPIDASH_GALARIAN] =
    {
        .categoryName = _("UNIQUE HORN"),
        .height = 17,
        .weight = 800,
        .description = gRapidashGalarianPokedexText,
        .unusedDescription = gRapidashGalarianPokedexTextUnused,
        .pokemonScale = 351,
        .pokemonOffset = 0,
        .trainerScale = 347,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SLOWPOKE_GALARIAN] =
    {
        .categoryName = _("DOPEY"),
        .height = 12,
        .weight = 360,
        .description = gSlowpokeGalarianPokedexText,
        .unusedDescription = gSlowpokeGalarianPokedexTextUnused,
        .pokemonScale = 289,
        .pokemonOffset = 11,
        .trainerScale = 472,
        .trainerOffset = 6,
    },

    [NATIONAL_DEX_SLOWBRO_GALARIAN] =
    {
        .categoryName = _("HERMIT CRAB"),
        .height = 16,
        .weight = 705,
        .description = gSlowbroGalarianPokedexText,
        .unusedDescription = gSlowbroGalarianPokedexTextUnused,
        .pokemonScale = 356,
        .pokemonOffset = 3,
        .trainerScale = 387,
        .trainerOffset = 2,
    },

    [NATIONAL_DEX_SLOWKING_GALARIAN] =
    {
        .categoryName = _("HEXPERT"),
        .height = 18,
        .weight = 795,
        .description = gSlowkingGalarianPokedexText,
        .unusedDescription = gSlowkingGalarianPokedexTextUnused,
        .pokemonScale = 345,
        .pokemonOffset = -1,
        .trainerScale = 367,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_MR_MIME_GALARIAN] =
    {
        .categoryName = _("DANCING"),
        .height = 14,
        .weight = 568,
        .description = gMrMimeGalarianPokedexText,
        .unusedDescription = gMrMimeGalarianPokedexTextUnused,
        .pokemonScale = 332,
        .pokemonOffset = 4,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MR_RIME] =
    {
        .categoryName = _("COMEDIAN"),
        .height = 15,
        .weight = 582,
        .description = gMrRimePokedexText,
        .unusedDescription = gMrRimePokedexTextUnused,
        .pokemonScale = 354,
        .pokemonOffset = 3,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SIRFETCHD] =
    {
        .categoryName = _("WILD DUCK"),
        .height = 8,
        .weight = 1170,
        .description = gSirfetchdPokedexText,
        .unusedDescription = gSirfetchdPokedexTextUnused,
        .pokemonScale = 745,
        .pokemonOffset = 12,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_WOOPER_PALDEAN] =
    {
        .categoryName = _("POISON FISH"),
        .height = 4,
        .weight = 110,
        .description = gWooperPaldeanPokedexText,
        .unusedDescription = gWooperPaldeanPokedexTextUnused,
        .pokemonScale = 721,
        .pokemonOffset = 17,
        .trainerScale = 341,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_CLODSIRE] =
    {
        .categoryName = _("SPINY FISH"),
        .height = 18,
        .weight = 2230,
        .description = gClodsirePokedexText,
        .unusedDescription = gClodsirePokedexTextUnused,
        .pokemonScale = 317,
        .pokemonOffset = 4,
        .trainerScale = 582,
        .trainerOffset = 9,
    },

};
