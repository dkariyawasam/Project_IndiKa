# Chingling sprite import

## Sources

Battle front/back and their shiny variants: Pokémon HeartGold/SoulSilver, archived by [PokeAPI/sprites](https://github.com/PokeAPI/sprites/tree/master/sprites/pokemon/versions/generation-iv/heartgold-soulsilver). Files: `433.png`, `back/433.png`, `shiny/433.png`, `back/shiny/433.png`. Original artwork belongs to its Pokémon rights holders; PokeAPI is the archive, not the original artist.

Party icon, footprint and overworld sheet/palettes: [pokeemerald-expansion Chingling asset directory](https://github.com/rh-hideout/pokeemerald-expansion/tree/master/graphics/pokemon/chingling). These are community-maintained GBA assets; the overworld port's precise original game/individual author was not independently established. Source files preserved in `source/`.

## Adaptation and validation

- Battle images: 80×80 source canvases cropped to 64×64 at (8,8). Every visible source pixel retained without scaling. Front visible size 32×37; back 50×51.
- Shared normal/shiny palette: 13 entries including transparent index 0, padded to 16. Front/back palette correspondence verified per pixel for both colour variants.
- Coordinate tables adjusted to the visible art and baseline.
- Icon: original 32×64 two-frame sheet, matching existing icon palette 1 exactly.
- Footprint: 16×16, 1bpp.
- Overworld: six 32×32 source frames mapped to standard nine-frame order [0,2,4,1,0,3,2,5,4], with idle fallback for hand-raise frame. Added OBJ_EVENT_GFX_CHINGLING and a dynamic palette. Not placed on any map. Shiny overworld palette retained in source assets for future use; the registered map sprite uses the normal palette.
- Source pixel/transparency and palette tests passed. Main ROM build passed. No live Chingling battle or map playtest performed in this change.

`battle-preview.png`: normal front, normal back, shiny front, shiny back.

## FRLG palette revision

Normal/shiny battle palettes now use existing GBA colours from Chimecho and Bellsprout, with a dark outline. Normal ribbons use Chimecho's pink/red ramp; shiny ribbons use its orange ramp. Overworld normal colours were updated to match. Party icon already uses FRLG shared icon palette 1. Pixel shapes, transparency indices and dimensions are unchanged. This is an FRLG-style adaptation, not an official Chingling FRLG sprite. The earlier source-colour equality check describes the initial import; the current palette intentionally differs. `frlg-palettes.py` records the final colour tables and runs after `import.cjs` when regenerating assets.


Update: the unused overworld sheet, palette and graphics registration were removed at user request. Archived source references remain in this documentation directory. Battle sprites, palettes, footprints and menu icons remain installed.
