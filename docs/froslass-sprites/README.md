# Froslass sprite import

Battle art: HeartGold/SoulSilver archived by [PokeAPI/sprites](https://github.com/PokeAPI/sprites/tree/master/sprites/pokemon/versions/generation-iv/heartgold-soulsilver), files `478.png`, `back/478.png`, `shiny/478.png`, `back/shiny/478.png`. Original Pokémon artwork belongs to its rights holders.

Icon, footprint and overworld assets: [pokeemerald-expansion](https://github.com/rh-hideout/pokeemerald-expansion/tree/master/graphics/pokemon/froslass). Community-maintained GBA assets; precise original overworld game/artist provenance not independently established. Originals retained in `source/`.

Front 38×54 unchanged; back 71×71 reduced to 64×64. Nearest-neighbour scaling, 64×64 battle canvases, indexed 16-colour normal/shiny palettes. Resized views received material-aware one-pixel outline repairs. Colours sourced from existing FRLG Gardevoir, Glalie, Jynx and Chimecho palettes.

Two-frame menu icon and footprint wired. Overworld six source frames converted to the standard nine-frame order and registered, with matching normal colours; no map placement added. Original shiny overworld palette retained for future use. Shared palette/outline scripts are under `docs/froslass-sprites/`.

Main ROM build and asset checks passed. Live battle/map testing not yet performed.


Update: the unused overworld sheet, palette and graphics registration were removed at user request. Archived source references remain in this documentation directory. Battle sprites, palettes, footprints and menu icons remain installed.
