# Trainer dialogue and identity fixes — 2026-10-05

All actionable findings from the [initial audit](initial-findings.md) have been addressed. The ROM has been rebuilt.

## Changes

- Split five ordinary trainer identities that shared defeat flags across maps. Route 24 has Scout Inez, Route 23 has Bird Keeper Orville, Route 22 has Triathlete Freya, Route 5 has Aroma Lady Irma, and Route 8 has Aroma Lady Heather. Kindra, Chester and Rina retain their other placements; Nikki and Violet retain their forest identities and story references.
- Added dedicated matching rematches for those five trainers and the ten broken rematch identities. New rematch parties retain their original species/moves/items, with levels raised by six (capped at 100). No unrelated trainer party was repurposed.
- Allocated 20 vacant IDs, keeping `NUM_TRAINERS`, `MAX_TRAINERS_COUNT` and system flag offsets at their existing values. Existing saves cannot distinguish which member of an old shared pair was beaten; the newly separated encounter uses its own previously vacant flag rather than migrating ambiguous progress.
- Rewrote all 23 template dialogue sets across Vermilion Harbor, Viridian Forest, Fuchsia Forest and Cerulean Cave.
- Corrected the class/location leftovers, including the Route 9 Black Belts, Rock Tunnel Stevie and Winston, Jonah's natural forest entrance, Johan's Psychic dialogue and cycling-related lines.
- Made Chaplin a Pokémaniac, with the matching portrait and overworld, while preserving his Mr. Mime team and Apex clue.
- Gave all eight Rocket Aces separate introductions and defeat lines that fit their current names and teams. Their individual post-battle lines remain.
- Added individual introductions and defeat responses for all twelve Indigo challenge opponents.
- Distinguished the repeated Gym trainee, Expert, Trendsetter, doubles-pair, Youngster rematch and short defeat lines.
- Registered the existing Archer and Proton overworld sheets, palettes and ten-frame tables. The arena now selects those graphics for their battles, Ariana for hers, and the matching male/female Rocket graphics for the Aces. Fixed Ariana's female trainer flag.
- Corrected Paxton's Scout overworld to Roughneck, Becky's water portrait to the land triathlete portrait, and Finn's overworld to the Tuber half of the Swim Sibs portrait.
- Removed ten confirmed unreferenced party definitions. Updated the roster audit to recognise the explicitly intended rival Apex Ambipom on his Route 23 and Champion teams.

201 dialogue labels were edited or added. Intentional shared two-Pokémon requirements, system messages and the Rocket siblings' introduction remain shared. Roster variants belonging to the same character may share that character's dialogue. Trendsetter pairs retain the project's explicit alias to the existing two-person Aces portrait.

## Verification

- Main `make -j4` build: passed.
- `tools/test_trainer_identity_dialogue.py`: passed — 446 placed trainer objects, active class/front identities, script-selected league actors, distinct new IDs and all edited dialogue. Widest edited field-text line: 185 pixels against a 208-pixel limit, checked with the actual normal/male/female font widths.
- Updated dialogue inventory: 253 maps, 444 encounter sites, 920 battle commands including rematches, 1,809 reachable dialogue labels. No missing script/text pointers, battle-text terminators, ordinary cross-map shared defeat flags or rematch identity mismatches detected. Remaining cross-map IDs belong to intentional leader/story/challenge appearances.
- Trainer wiring audit: passed — 777 entries and 664 party definitions; no missing or orphan party definitions.
- Trainer roster audit: passed, preserving the intentionally permitted rival Ambipom.
- Battle palette audit: 90 portrait/palette pairs passed.
- Overworld palette audit: 181 sprite/palette pairs passed, with no detected index permutations or fixed-slot conflicts. Existing whole-map dynamic palette demand warnings remain; this audit does not prove simultaneous on-screen palette capacity.
- Archer and Proton: all ten frames each match their compiled 4bpp tile data pixel for pixel.

These are source, compiled-asset and build checks. No new live emulator playthrough was performed; live dialogue timing, map transitions and crowded-map palette behaviour are not claimed as tested.

## Evidence and reproduction

- `inventory.json` and `inventory.md`: regenerated source inventory.
- `identity-fixes.json`: new ID allocations, original identities and party associations.
- `dialogue-fixes.json`: before/after text for the 161 rewritten labels.
- `league-voices.json`: 40 new league dialogue labels.
- `removed-unused-parties.json`: the ten removed definitions.

Run the Python audit tools with Pillow available. `tools/test_trainer_identity_dialogue.py` is the regression check; `tools/audit_trainer_dialogue.py` regenerates the inventory. The original manual findings are preserved separately for comparison.

## October 6 live follow-up

[Runtime checks and evidence](../release-playthrough/october6/README.md) now cover Rocket entrances/portraits, six enabled rematch identities, eight field samples and one complete field-battle return. Three indoor palette-budget failures were found and repaired. The limitations of the controlled fixtures are recorded there.
