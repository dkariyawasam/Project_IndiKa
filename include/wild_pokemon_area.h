#ifndef GUARD_WILD_POKEMON_AREA_H
#define GUARD_WILD_POKEMON_AREA_H

#define DEX_ENCOUNTER_DAY 1
#define DEX_ENCOUNTER_NIGHT 2
u8 GetSpeciesPokedexEncounterTimes(u16 species);

s32 GetSpeciesPokedexAreaMarkers(u16 species, struct Subsprite * subsprites);

#endif //GUARD_WILD_POKEMON_AREA_H
