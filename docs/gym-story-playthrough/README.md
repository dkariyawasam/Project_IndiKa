# Gym playthrough review

Reviewed 13 September 2026 in mGBA 0.10.5. All eight leader battles, their badges, 22 guide/trial scenes, and six reward/trade sequences completed. Three outstanding progression/continuity findings remain. Two small dialogue fixes were applied to the main ROM.

## Coverage and limits

This was a staged runtime playthrough, supported by script and map review. Travel, prerequisites and party preparation were staged in a disposable emulator save. The eight battles ran through the real battle engine with button input and six level-100 Charizard; wins were not written into memory. This verifies battle completion and story handoffs, not difficulty balance or a continuous new-game campaign. Only the badge-order variants selected by this sequence were fought.

The order was Brock, Misty, Surge, Erika, Koga, Sabrina, Blaine and Giovanni. Erika's battle ran in Viridian Forest; Koga's stone scene and battle ran in Cinnabar Volcano; Sabrina's battle ran in Pokémon Tower. The other battles ran in their gyms. All eight victories set their badge and defeated flags. The six custom badge items were checked in the bag.

Field coverage included six automatic guide introductions; Brock's first meeting and fossil return; Misty's introduction, three current decisions and generator handoffs; Surge's Volt Tackle tutoring; Erika's Energy Root request and return; Koga's volcano request and checkpoint; and Sabrina's Dream Eater tutoring. Misty's correct decision branches were invoked directly: the scouting/travel and wrong-answer branches were not played. Tutor menus and NPC movements ran normally. Every tested field scene reached its expected state and released player control.

Brock's trade produced Golem, Misty's produced Politoed, and Surge's produced Electivire. Erika gave Budew, Koga gave Golbat, and Blaine's generator produced Porygon after the correct answers. Received species were checked with the game's GetPartyMonSpecies special. Friendship was not naturally raised to evolve Budew or Golbat, and their later return checks were not played. Full-party/storage-full branches, battle losses, all gym puzzle traversal and every optional dialogue branch were not exhausted.

## Outstanding findings

1. **Sabrina does not record gym-trial completion.** Her battle awards the Marsh Badge, but the Gym Leader Trials completion record remains false. Neither the tower victory nor her gym return writes it. The journal can infer a finished badge stage from other flags, but Field Aide obedience credit counts the actual completed subquest bits. Sources: `data/maps/PokemonTower_3F/scripts.inc`, `data/maps/SaffronCity_Gym/scripts.inc`, and `src/battle_util.c`.
2. **Giovanni has the same missing gym-trial credit.** His victory correctly finishes Giovanni's Ambition and awards the Earth Badge, but does not complete his entry under Gym Leader Trials. The runtime completion query returned false after victory. Source: `data/maps/ViridianCity_Gym/scripts.inc`.
3. **Erika's witness is not preserved when returning before the battle.** Viridian Forest's transition always hides the gym guide. With the root accepted (trial state 3), Erika jumps straight to her challenge and does not bring the guide back. The runtime return check confirmed the guide's hide flag resets. Showing the root with only one usable Pokémon reaches the same state before the guide arrives. Her battle still completes, but the dialogue describes a witnessed challenge with no witness present. Source: `data/maps/ViridianForest/scripts.inc`.

No progression code was changed during this review.

## Small fixes applied

- Sabrina's tutoring invitation used a second newline that placed “official challenge here” below the two-line box. A scrolling line break now displays it; the rebuilt ROM's tutor sequence was exercised.
- Surge's guide described the removed locked-door puzzle. The current map has a direct path and no switch interactions. Replaced that advice with the requirement to bring two Pokémon for the double battle.

The 352-message source audit found no lines wider than the checked text width. Remaining three-line candidates are two unrelated forest trainer defeat messages, a legacy Koga gym explanation, and two music-control-only endings; these were left unchanged. This is not a claim that every optional message was rendered in the emulator.

## Evidence and fixture corrections

`runtime-tests.tsv` preserves raw checks, including reruns. `runtime-summary.tsv` contains the last result for each unique check: 56 pass and 3 fail. The three failures are the findings above. `dialogue-log.tsv` contains field/tutor/trade text captured from the game; it is not a complete battle dialogue transcript. Screenshots and fixture JSON files are included alongside the Lua harness.

The first Misty and Surge trade species checks failed because the staged fixture had not run Oak's National Pokédex initialization. Their non-Kanto evolutions were consequently cancelled by the evolution engine. Oak enables the National Pokédex when giving the initial Pokédex in this project. After invoking that same native initialization, both trades produced the expected evolved species. These two initial failures are fixture errors, not current-game findings.

Giovanni required adjustment of the QA move-selection input when Rock Slide ran low. The battle subsequently completed. No battle logic was patched for the test.

The battle phase used the pre-review main ROM. The field, tutoring, rewards and Erika return checks used the rebuilt ROM. The only binary edits between them are the two same-length text changes; all compiled symbol addresses remained identical. Save states are address-dependent; do not reuse this harness blindly with a later build. The archived harness retains session-specific `/tmp` paths and is evidence, not a standalone automated test suite.

## Build and preservation

`make -j4` completed successfully. Main ROM SHA-256:
`4c5c29747c9b269598fdaba28576dd3206042299cf258c2b13a6ee28cd986dec`.

The user's main save remained unchanged:
`816eb072055dad4bbd8ca2b4d596cf468e58b68384664931a91e814dbb824b08`.

The retained customization ROM remained unchanged:
`880e9e2dc73c16084b10dfc3cb13bfa1b3535b637277e02a576fb8aee0194d5b`.
