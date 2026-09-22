# Bronzor and Bronzong asset completion

Existing front sprites and shared normal/shiny palette ramps retained. Added HGSS back sprites from the [PokeAPI archive](https://github.com/PokeAPI/sprites/tree/master/sprites/pokemon/versions/generation-iv/heartgold-soulsilver/back), IDs 436 and 437. Official Pokémon artwork belongs to its original rights holders. Source normal/shiny fronts and backs are archived here for reference. The imported backs are remapped to the existing normal palette; shiny rendering uses the existing corresponding shiny palette.

Bronzor's 45×56 back is retained without resampling. Bronzong's 80×64 visible back is resized with nearest-neighbour to 64×51. Both sit at the bottom of a 64×64 frame, retaining the HGSS upper-body view. Corrected front/back coordinate metadata. No overworld graphics were added.

Party icons come from [pokeemerald-expansion](https://github.com/rh-hideout/pokeemerald-expansion/tree/master/graphics/pokemon), mapped to existing FRLG icon palette 0, with transparency at index 0 and both animation frames retained. Footprints already exist and are retained.

Added the missing Bronzor → Bronzong evolution at level 33 and TM compatibility for supported Gen-IV moves (HM moves use this project's TM equivalents). References: [Bronzor](https://pokemondb.net/pokedex/bronzor/moves/4), [Bronzong](https://pokemondb.net/pokedex/bronzong/moves/4), [evolution](https://pokemondb.net/pokedex/bronzor). Existing level-up learnsets, Levitate-only ability configuration and cry system remain as implemented; this is not a port of every later-generation mechanic or audio asset.

`import.py` reproduces backs/icons and coordinate metadata from the archived assets. `battle-preview.png` shows normal front/back and shiny front/back for both species.

Validation: nonblank 64×64 indexed battle sprites, nonblank 32×64 icons, 16-color limits, shared palette correspondence, graphics registrations, evolution/TM entries, and main ROM build. No live battle playtest performed.
