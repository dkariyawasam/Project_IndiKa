# Mamoswine sprite import

Battle art: HeartGold/SoulSilver, archived by [PokeAPI/sprites](https://github.com/PokeAPI/sprites/tree/master/sprites/pokemon/versions/generation-iv/heartgold-soulsilver): `473.png`, `back/473.png`, `shiny/473.png`, `back/shiny/473.png`. Original Pokémon art belongs to its rights holders.

Supporting icon, footprint and overworld assets: [pokeemerald-expansion](https://github.com/rh-hideout/pokeemerald-expansion/tree/master/graphics/pokemon/mamoswine). Community-maintained GBA assets; precise original overworld game/artist provenance not independently established. Originals preserved under `source/`.

Front visible art 80×65 reduced to 64×52; back 79×42 reduced to 64×34. Nearest-neighbour scaling, no extra visible crop. Both use 64×64 canvases. Normal/shiny palettes adapted to FRLG-style shading while retaining brown/blue normal and green/teal shiny identity. Overworld normal palette matched; shiny overworld source palette retained for future use.

Two-frame 32×64 menu icon mapped to existing shared palette 2; footprint imported. Overworld six source frames mapped to standard nine-frame sequence with idle fallback, registered as OBJ_EVENT_GFX_MAMOSWINE; not placed on any map.

Main ROM build, palette/transparency checks and overworld frame-packing audits passed. Live battle/map review not performed. Preview order: normal front/back, shiny front/back.

## Direct FRLG colour revision

All visible battle palette colours now come directly from existing FRLG palettes: Piloswine (normal coat, tusks and neutral shades), Lapras (blue mask and cool shadows), Seel (nose), Victreebel (shiny green coat) and Venusaur (shiny teal mask). The normal overworld palette matches. `frlg-palettes.py` records the exact source indices. Shapes and menu icon remain unchanged.

## Outline repair

Restored material-aware single-pixel boundary shading on front/back sprites after downscaling: 63 front and 32 back boundary pixels. Dark brown fur edges and dark grey tusk edges retain the exact silhouette, palette indices and interior detail. The intended bottom crop of the back sprite stays open. Shared battle image data applies the repair to both normal and shiny colours. Main ROM rebuilt; dimensions, transparency, palette and silhouette preservation verified.


Update: the unused overworld sheet, palette and graphics registration were removed at user request. Archived source references remain in this documentation directory. Battle sprites, palettes, footprints and menu icons remain installed.
