#include "global.h"
#include "trainer_nicknames.h"
#include "constants/species.h"
#include "constants/trainers.h"

struct TrainerNickname
{
    u16 species;
    u8 nickname[POKEMON_NAME_LENGTH + 1];
};

static const struct TrainerNickname sBrockNicknames[] = {
    {SPECIES_GEODUDE, _("PEBBLE")},
    {SPECIES_GRAVELER, _("PEBBLE")},
    {SPECIES_GOLEM, _("PEBBLE")},
    {SPECIES_LARVITAR, _("CHIP")},
    {SPECIES_PUPITAR, _("CHIP")},
    {SPECIES_SHUCKLE, _("PICKLE")},
    {SPECIES_AERODACTYL, _("FLINT")},
    {SPECIES_OMANYTE, _("SPIRAL")},
    {SPECIES_OMASTAR, _("SPIRAL")},
    {SPECIES_KABUTO, _("CHISEL")},
    {SPECIES_KABUTOPS, _("CHISEL")},
};

static const struct TrainerNickname sMistyNicknames[] = {
    {SPECIES_STARYU, _("TWINKLE")},
    {SPECIES_STARMIE, _("TWINKLE")},
    {SPECIES_CHINCHOU, _("BEACON")},
    {SPECIES_LANTURN, _("BEACON")},
    {SPECIES_PSYDUCK, _("DUCKY")},
    {SPECIES_GOLDUCK, _("DUCKY")},
    {SPECIES_POLIWHIRL, _("PUDDLES")},
    {SPECIES_POLITOED, _("PUDDLES")},
    {SPECIES_HORSEA, _("SKIPPER")},
    {SPECIES_KINGDRA, _("SKIPPER")},
    {SPECIES_TENTACRUEL, _("RIBBONS")},
};

static const struct TrainerNickname sLtSurgeNicknames[] = {
    {SPECIES_TYNAMO, _("TORPEDO")},
    {SPECIES_EELEKTRIK, _("TORPEDO")},
    {SPECIES_EELEKTROSS, _("TORPEDO")},
    {SPECIES_PICHU, _("SPARKY")},
    {SPECIES_PIKACHU, _("SPARKY")},
    {SPECIES_RAICHU, _("SPARKY")},
    {SPECIES_VOLTORB, _("FUSE")},
    {SPECIES_ELECTRODE, _("FUSE")},
    {SPECIES_MAGNEMITE, _("RIVET")},
    {SPECIES_MAGNETON, _("RIVET")},
    {SPECIES_MAGNEZONE, _("RADAR")},
    {SPECIES_ELECTABUZZ, _("SARGE")},
    {SPECIES_ELECTIVIRE, _("MAJOR")},
    {SPECIES_PLUSLE, _("BOOST")},
    {SPECIES_MINUN, _("BUDDY")},
};

static const struct TrainerNickname sErikaNicknames[] = {
    {SPECIES_BELLSPROUT, _("HONEY")},
    {SPECIES_WEEPINBELL, _("HONEY")},
    {SPECIES_VICTREEBEL, _("HONEY")},
    {SPECIES_ODDISH, _("DAHLIA")},
    {SPECIES_GLOOM, _("DAHLIA")},
    {SPECIES_VILEPLUME, _("DAHLIA")},
    {SPECIES_BELLOSSOM, _("MARIGOLD")},
    {SPECIES_TANGELA, _("TANGLE")},
    {SPECIES_EXEGGCUTE, _("BONSAI")},
    {SPECIES_EXEGGUTOR, _("BONSAI")},
    {SPECIES_SEEDOT, _("MAPLE")},
    {SPECIES_SHIFTRY, _("MAPLE")},
};

static const struct TrainerNickname sKogaNicknames[] = {
    {SPECIES_GOLBAT, _("WHISPER")},
    {SPECIES_CROBAT, _("WHISPER")},
    {SPECIES_ARBOK, _("SILK")},
    {SPECIES_GRIMER, _("INK")},
    {SPECIES_MUK, _("INK")},
    {SPECIES_KOFFING, _("HAZE")},
    {SPECIES_WEEZING, _("HAZE")},
    {SPECIES_GULPIN, _("DUMPLING")},
    {SPECIES_SWALOT, _("DUMPLING")},
    {SPECIES_GENGAR, _("SHADE")},
};

static const struct TrainerNickname sSabrinaNicknames[] = {
    {SPECIES_ABRA, _("SAGE")},
    {SPECIES_KADABRA, _("SAGE")},
    {SPECIES_ALAKAZAM, _("SAGE")},
    {SPECIES_DROWZEE, _("REVERIE")},
    {SPECIES_HYPNO, _("REVERIE")},
    {SPECIES_NATU, _("ORACLE")},
    {SPECIES_XATU, _("ORACLE")},
    {SPECIES_CHINGLING, _("ECHO")},
    {SPECIES_CHIMECHO, _("ECHO")},
    {SPECIES_CLAYDOL, _("ORBIT")},
    {SPECIES_BRONZONG, _("SOLACE")},
};

static const struct TrainerNickname sBlaineNicknames[] = {
    {SPECIES_GROWLITHE, _("BUNSEN")},
    {SPECIES_ARCANINE, _("BUNSEN")},
    {SPECIES_VULPIX, _("EMBER")},
    {SPECIES_NINETALES, _("EMBER")},
    {SPECIES_RAPIDASH, _("COMET")},
    {SPECIES_SLUGMA, _("SIMMER")},
    {SPECIES_MAGCARGO, _("SIMMER")},
    {SPECIES_CAMERUPT, _("KETTLE")},
    {SPECIES_MAGMAR, _("FAHRENHEIT")},
    {SPECIES_MAGMORTAR, _("FAHRENHEIT")},
};

static const struct TrainerNickname sGiovanniNicknames[] = {
    {SPECIES_SANDSLASH, _("SPADE")},
    {SPECIES_DUGTRIO, _("TRIO")},
    {SPECIES_MAROWAK, _("KEEPSAKE")},
    {SPECIES_NIDOQUEEN, _("REGINA")},
    {SPECIES_NIDOKING, _("REX")},
    {SPECIES_RHYDON, _("BASTION")},
    {SPECIES_RHYPERIOR, _("BASTION")},
    {SPECIES_PERSIAN, _("VELVET")},
    {SPECIES_CROBAT, _("VESPER")},
    {SPECIES_HONCHKROW, _("CAPO")},
};

static const u8 sNicknameCadet[POKEMON_NAME_LENGTH + 1] = _("CADET");
static const u8 sNicknameMajor[POKEMON_NAME_LENGTH + 1] = _("MAJOR");
static const u8 sNicknamePuff[POKEMON_NAME_LENGTH + 1] = _("PUFF");
static const u8 sNicknameWisp[POKEMON_NAME_LENGTH + 1] = _("WISP");
static const u8 sNicknameLucid[POKEMON_NAME_LENGTH + 1] = _("LUCID");

const u8 *GetTrainerPokemonNickname(u16 trainerId, u16 species, u8 partySlot)
{
    const struct TrainerNickname *names;
    u32 count;
    u32 i;

    // Separate partners can share an evolutionary family in the same team.
    if (trainerId == TRAINER_LEADER_LT_SURGE_2 && species == SPECIES_PICHU)
        return sNicknameCadet;
    if (trainerId == TRAINER_LEADER_LT_SURGE_6 && partySlot == 4 && species == SPECIES_ELECTABUZZ)
        return sNicknameMajor;
    if (trainerId == TRAINER_LEADER_KOGA_6 && species == SPECIES_KOFFING)
        return sNicknamePuff;
    if ((trainerId == TRAINER_LEADER_SABRINA_2
      || trainerId == TRAINER_LEADER_SABRINA_3
      || trainerId == TRAINER_LEADER_SABRINA_4
      || trainerId == TRAINER_LEADER_SABRINA_5
      || trainerId == TRAINER_LEADER_SABRINA_6) && species == SPECIES_ABRA)
        return sNicknameWisp;
    if (trainerId == TRAINER_LEADER_SABRINA_6 && species == SPECIES_KADABRA)
        return sNicknameLucid;

    switch (trainerId)
    {
    case TRAINER_LEADER_BROCK_1:
    case TRAINER_LEADER_BROCK_2:
    case TRAINER_LEADER_BROCK_3:
    case TRAINER_LEADER_BROCK_4:
    case TRAINER_LEADER_BROCK_5:
    case TRAINER_LEADER_BROCK_6:
    case TRAINER_LEADER_BROCK_7:
        names = sBrockNicknames;
        count = ARRAY_COUNT(sBrockNicknames);
        break;
    case TRAINER_LEADER_MISTY_1:
    case TRAINER_LEADER_MISTY_2:
    case TRAINER_LEADER_MISTY_3:
    case TRAINER_LEADER_MISTY_4:
    case TRAINER_LEADER_MISTY_5:
    case TRAINER_LEADER_MISTY_6:
    case TRAINER_LEADER_MISTY_7:
        names = sMistyNicknames;
        count = ARRAY_COUNT(sMistyNicknames);
        break;
    case TRAINER_LEADER_LT_SURGE_1:
    case TRAINER_LEADER_LT_SURGE_2:
    case TRAINER_LEADER_LT_SURGE_3:
    case TRAINER_LEADER_LT_SURGE_4:
    case TRAINER_LEADER_LT_SURGE_5:
    case TRAINER_LEADER_LT_SURGE_6:
    case TRAINER_LEADER_LT_SURGE_7:
        names = sLtSurgeNicknames;
        count = ARRAY_COUNT(sLtSurgeNicknames);
        break;
    case TRAINER_LEADER_ERIKA_1:
    case TRAINER_LEADER_ERIKA_2:
    case TRAINER_LEADER_ERIKA_3:
    case TRAINER_LEADER_ERIKA_4:
    case TRAINER_LEADER_ERIKA_5:
    case TRAINER_LEADER_ERIKA_6:
    case TRAINER_LEADER_ERIKA_7:
        names = sErikaNicknames;
        count = ARRAY_COUNT(sErikaNicknames);
        break;
    case TRAINER_LEADER_KOGA_1:
    case TRAINER_LEADER_KOGA_2:
    case TRAINER_LEADER_KOGA_3:
    case TRAINER_LEADER_KOGA_4:
    case TRAINER_LEADER_KOGA_5:
    case TRAINER_LEADER_KOGA_6:
    case TRAINER_LEADER_KOGA_7:
        names = sKogaNicknames;
        count = ARRAY_COUNT(sKogaNicknames);
        break;
    case TRAINER_LEADER_SABRINA_1:
    case TRAINER_LEADER_SABRINA_2:
    case TRAINER_LEADER_SABRINA_3:
    case TRAINER_LEADER_SABRINA_4:
    case TRAINER_LEADER_SABRINA_5:
    case TRAINER_LEADER_SABRINA_6:
    case TRAINER_LEADER_SABRINA_7:
        names = sSabrinaNicknames;
        count = ARRAY_COUNT(sSabrinaNicknames);
        break;
    case TRAINER_LEADER_BLAINE_1:
    case TRAINER_LEADER_BLAINE_2:
    case TRAINER_LEADER_BLAINE_3:
    case TRAINER_LEADER_BLAINE_4:
    case TRAINER_LEADER_BLAINE_5:
    case TRAINER_LEADER_BLAINE_6:
    case TRAINER_LEADER_BLAINE_7:
        names = sBlaineNicknames;
        count = ARRAY_COUNT(sBlaineNicknames);
        break;
    // Silph Co. (TRAINER_BOSS_GIOVANNI_2) deliberately keeps species names.
    case TRAINER_LEADER_GIOVANNI:
    case TRAINER_ROCKET_LEAGUE_CHAMPION_GIOVANNI:
        names = sGiovanniNicknames;
        count = ARRAY_COUNT(sGiovanniNicknames);
        break;
    default:
        return NULL;
    }

    for (i = 0; i < count; i++)
        if (names[i].species == species)
            return names[i].nickname;
    return NULL;
}
