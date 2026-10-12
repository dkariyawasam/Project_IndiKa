# Spiritomb sprite import

Battle art: HeartGold/SoulSilver sprites archived in [PokeAPI/sprites](https://github.com/PokeAPI/sprites/tree/master/sprites/pokemon/versions/generation-iv/heartgold-soulsilver), files `442.png`, `back/442.png`, `shiny/442.png`, `back/shiny/442.png`. Pokémon artwork belongs to its original rights holders.

Icon, footprint and overworld assets: [pokeemerald-expansion](https://github.com/rh-hideout/pokeemerald-expansion/tree/master/graphics/pokemon/spiritomb). These are community-maintained GBA assets; precise original overworld game/artist provenance was not independently established. Originals retained under `source/`.

Front visible art is 57×56, placed unchanged on a 64×64 canvas. Back visible art is 79×68, reduced with nearest-neighbour to 64×55. No visible art cropped away. Shared 16-entry normal/shiny battle palettes; FRLG-style Gastly-inspired purples and brighter greens, with blue/pink shiny colours. Overworld palette adapted to match. This is an FRLG-style adaptation, not official FRLG Spiritomb art.

Party icon mapped to the nearest existing shared FRLG icon palette (2), without changing its pixel layout. Footprint source is blank, appropriate for this species. Six overworld source frames mapped to nine standard animation frames; registered as OBJ_EVENT_GFX_SPIRITOMB but not placed on a map. Shiny overworld source palette preserved for future use; runtime map sprite uses normal colours.

Main ROM build and palette/frame checks passed. Live in-game battle/map testing remains outstanding. Preview order: normal front/back, shiny front/back.


Update: the unused overworld sheet, palette and graphics registration were removed at user request. Archived source references remain in this documentation directory. Battle sprites, palettes, footprints and menu icons remain installed.
