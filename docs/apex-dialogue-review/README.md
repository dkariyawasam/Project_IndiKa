# Apex dialogue continuity review — 13 September 2026

The eight investigations have a coherent structure: local observations lead to a habitat, the encounter produces a field record, and Oak interprets what makes that Pokémon unusual. Mewtwo connects those investigations to the wider story by contrasting natural instincts with manufactured power and attempted control.

Small wording and dossier-attribution changes were made in the main ROM. No encounter levels, quest conditions, trainer placements or escape mechanics were changed in this pass.

## Connections improved

- **Opening:** Oak defines Apex Pokémon and explains comparing three accounts in the Logbook. Both his briefing and the first-rumour notification explain that fleeing ends the opportunity permanently. The rival's Route 11 explanation now agrees with the automatic recording system.
- **Readable clue pages:** three existing third-line breaks now scroll within the two-line box, preserving the Mt. Moon destination, the Diglett Cave destination and the end of the Tower mourner’s account.
- **Annihilape:** the Cerulean witness explicitly points west toward Mt. Moon.
- **Mime Sr.:** Chaplin's repeatable clue names the Mr. Mime instead of assuming the player remembers his earlier dialogue.
- **Osscythe:** Cubone's fallback interaction conveys its search for someone. The shared dossier records that observation, rather than claiming the player heard the Rocket's confession. Its portrait/source now identify Cubone. The Tower clue says “inside,” avoiding the misleading summit direction for the 4F encounter.
- **Articuno:** the sailor repeats the environmental clue after battle, and the Seafoam witness points toward the increasing cold deeper inside.
- **Mansion → Silph:** Giovanni names Silph in Saffron and its fail-safe Ball as his next destination and motive.
- **Silph → Route 7:** Giovanni explains that Silph's signal research will complete his alternative device, then names Route 7. The dossier no longer claims the signal already succeeded before the player sees the evidence.
- **Master Ball:** the President distinguishes capturing a Pokémon from earning its trust, keeping his gift consistent with the story's criticism of Giovanni's control.
- **Route 7 → Celadon Cave:** the witnesses name Giovanni and Mewtwo explicitly. Agatha directs the player to Lance in the cave created by the blast. Lance's opening also explains this, so his account stands on its own. The quest remains **ROUTE 7 RAMPAGE**; no line implies that Celadon Cave existed before the blast.
- **Cerulean Cave → Viridian Gym:** Giovanni names his next destination in both the resolved-encounter and escaped-Mewtwo conclusions.

Tangrowth, Zapdos and Moltres already had consistent clue-to-Oak connections and did not need dialogue rewrites. Their distinct emphases remain forest instinct, sustained electricity and ecological renewal.

## Validation and coverage

This is a targeted dialogue walkthrough, not an uninterrupted campaign or a battle-balance test. A fresh disposable character was created in a byte-identical QA copy of the rebuilt ROM. Travel and progression were staged; the original player save was not used.

The source review covers all 24 rumour slots, their repeatable/fallback conversations, all eight Oak responses, and the Mansion–Silph–Route 7–Celadon Cave–Cerulean Cave chain. Compiled dialogue is also exercised through the game's native message-box scripts. Those isolated text-render checks do not represent visiting every NPC or fighting every battle.

All 42 distinct isolated dialogue checks eventually passed. Five complete native events also passed: the Mansion encounter, Agatha first, Lorelei second, Bruno third, and Lance in Celadon Cave. Each event set its completion flag and released field controls. This checks a different witness order, but does not certify every possible route through the campaign.

Runtime scene results are recorded in `runtime-tests.tsv`; expanded text in `dialogue-log.tsv`. The itinerary lists the source text used in the isolated dialogue checks. Initial fixed-duration checks for Oak's longer briefing and Viridian's story expired before the boxes closed; these were harness timing failures. Those messages were rerun with a wait for the script to release field controls.

After the main runtime pass, the final build changed only three `CHAR_NEWLINE` bytes to `CHAR_PROMPT_SCROLL`; every compiled symbol address remained unchanged. The three affected messages were rechecked separately on the final ROM, including screenshots of their scrolling pages.

The changed event-text blocks contain 159 checked lines. Using the compiled normal-font widths and seven-wide-letter player/rival names, the maximum is 202 pixels against a 208-pixel dialogue width. `git diff --check` passed and `make -j4` completed successfully.

## Build and preserved files

Main ROM SHA-256: `b331906484feba5141412e44c1910905d67dcc4adb0d53e26be6cbdbf6d634ed`.

The original `pokefirered.sav` remains byte-identical (`816eb072055dad4bbd8ca2b4d596cf468e58b68384664931a91e814dbb824b08`). The retained player-customisation ROM also remains byte-identical (`880e9e2dc73c16084b10dfc3cb13bfa1b3535b637277e02a576fb8aee0194d5b`). Earlier Lance restart and permanent-escape fixes remain in place; their prior mechanical coverage is documented in [Apex story fixes](../apex-story-fixes/README.md).
