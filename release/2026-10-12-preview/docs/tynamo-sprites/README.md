# Tynamo sprite import

Battle art: Pokémon Black/White, archived by [PokeAPI/sprites](https://github.com/PokeAPI/sprites/tree/master/sprites/pokemon/versions/generation-v/black-white), files `602.png`, `back/602.png`, `shiny/602.png`, `back/shiny/602.png`. Original Pokémon artwork belongs to its rights holders.

Icon, footprint and overworld source: [pokeemerald-expansion](https://github.com/rh-hideout/pokeemerald-expansion/tree/master/graphics/pokemon/tynamo). Community-maintained GBA assets; precise original overworld game/artist provenance not independently established. Source files retained.

Both views unchanged: front 38×17, back 38×18. All use 64×64 battle canvases, nearest-neighbour scaling where needed and shared 16-colour normal/shiny palettes. Scaled views received material-aware outline repairs. FRLG colours drawn from Glalie, Lapras, Chinchou, Lanturn, Huntail, Venusaur, Bellsprout, Eevee and Chimecho.

Two-frame menu icon mapped to shared palette 0; footprint imported. Overworld source frames adapted to the standard nine-frame layout and registered, with matching normal colours. No new map placements. Original shiny overworld palette retained for future use. Shared scripts live under `docs/tynamo-sprites/`.

Main ROM build and asset checks passed. Live battle/map testing remains outstanding.

## FRLG back-view reframing

Supersedes the full-body back fit above. Tynamo is enlarged to 56×27 as a small-species exception. Eelektrik now shows a 64×55 enlarged head/upper-body crop; Eelektross shows a 64×64 head/upper-body crop, with the lower body continuing beyond the bottom edge. Normal and shiny versions share identical pixel indices. Existing FRLG palettes and front sprites retained. Main ROM rebuilt; dimensions, transparency and palette checks passed. Live battle review remains outstanding. `docs/tynamo-sprites/reframe-backs.cjs` records the crop/scale mapping and uses original source art, not the already downscaled images.


Update: the unused overworld sheet, palette and graphics registration were removed at user request. Archived source references remain in this documentation directory. Battle sprites, palettes, footprints and menu icons remain installed.
