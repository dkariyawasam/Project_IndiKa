# Sirfetch’d and the Paldean Wooper line

Sirfetch’d, Paldean Wooper and Clodsire append species IDs 499–501. Existing playable species and National Dex IDs remain stable, as do the save's 62-byte Dex flag fields.

Galarian Farfetch’d and Sirfetch’d are retained in the engine but have no acquisition source in this Kanto release. Galarian Farfetch’d is excluded from the Safari because its ordinary counterpart is not a Kanto wild encounter. Existing copies can still evolve. Three successful critical-hit messages against opponents during one won/caught battle trigger its evolution into Sirfetch’d, without requiring a level-up. Progress is per party slot, capped at three, and reset at battle initialization; switching does not erase it. Link and Battle Tower battles are excluded. Everstone still blocks evolution, and cancelling does not carry progress into the next battle. The Stick's existing critical-hit boost now supports both Galarian Farfetch’d and Sirfetch’d.

Steadfast is implemented for both: actually flinching raises Speed one stage, up to the normal cap, with a battle message and animation. Scrappy is not implemented.

Paldean Wooper is catchable in Safari North and evolves into Clodsire at level 20. Both have Poison Point/Water Absorb and Poison/Ground typing. Ordinary Wooper still evolves into Quagsire. Paldean Wooper participates in the habitat form selector; Clodsire and Sirfetch’d have distinct species entries. Hidden abilities are not added.

The Safari researcher explains the Paldean Wooper evolution. Its encounter replaces a duplicate slot, retaining every previously present wild species. The obtainable roster now includes 324 Pokémon (404 active Dex entries).

## Asset and data provenance

Community GBA front/back sprites, shiny palettes, menu icons and footprints are imported from [rh-hideout/pokeemerald-expansion](https://github.com/rh-hideout/pokeemerald-expansion/tree/803bcb7c58afe73b0831893f383e6ce07fe0bac2/graphics/pokemon), pinned at `803bcb7c58afe73b0831893f383e6ce07fe0bac2`. Paths: `sirfetchd`, `wooper/paldea`, and `clodsire`. Clodsire uses the first frame of `anim_front.png`; Paldean Wooper reuses the existing Wooper footprint. Menu icons are mapped to the game's existing palettes. See [retained upstream credits](../roserade-alolan-sprites/upstream-credits.md); individual artist attribution remains part of the project's ongoing research.

Reference stats/types and supported level-up moves come from [PokéAPI's static archive](https://github.com/PokeAPI/api-data), using Sword/Shield for Sirfetch’d and Scarlet/Violet for the Paldean line. Unsupported newer moves are omitted. Original game cries come through the community [PokéAPI cries archive](https://github.com/PokeAPI/cries), converted to full-length mono 10512 Hz unsigned 8-bit WAV. Exact cry URLs and checksums are in [cry-imports.json](../cry-imports.json); other source hashes are in [manifest.json](manifest.json).

## Verification

ROM build, cry import checks, active Dex/obtainability, form navigation and page reward checks pass. `tools/test_regional_evolutions.py` executes production critical-hit counting, evolution selection and Steadfast code with host stubs, covering thresholds, party-slot separation, excluded battle types and stat caps. Live emulator battle verification remains outstanding.
