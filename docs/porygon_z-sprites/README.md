# Porygon-Z sprite import

Battle source: HeartGold/SoulSilver, archived by [PokeAPI/sprites](https://github.com/PokeAPI/sprites/tree/master/sprites/pokemon/versions/generation-iv/heartgold-soulsilver), files `474.png`, `back/474.png`, `shiny/474.png`, `back/shiny/474.png`. Original Pokémon artwork belongs to its rights holders.

Icon, footprint and overworld source: [pokeemerald-expansion](https://github.com/rh-hideout/pokeemerald-expansion/tree/master/graphics/pokemon/porygon_z). Community-maintained GBA assets; precise original overworld game/artist provenance not independently established. Originals retained under `source/`.

Front 42×51 unchanged; back 75×62 reduced to 64×53. All battle canvases are 64×64. Nearest-neighbour scaling followed by single-pixel material-aware outline repairs on resized views. No silhouette expansion or extra cropping. Shared indexed normal/shiny palettes use FRLG colours from Porygon2, Dusclops, Piloswine, Lapras and Jynx. Source identity and distinct shiny colour schemes retained.

Two-frame 32×64 menu icon mapped to an existing shared palette. Footprint imported. Six overworld source frames adapted to nine animation frames and registered for map use; no new NPC placements. Normal overworld colours matched to battle palette; original shiny overworld palette retained for future use.

Main ROM rebuilt; asset checks passed. Live battle/map testing remains outstanding. Shared palette/outline adaptation scripts are in `docs/porygon_z-sprites/`.


Update: the unused overworld sheet, palette and graphics registration were removed at user request. Archived source references remain in this documentation directory. Battle sprites, palettes, footprints and menu icons remain installed.
