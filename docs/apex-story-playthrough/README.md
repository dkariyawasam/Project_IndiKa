# Apex story review — main ROM, 11 September 2026

Follow-up: the Lance restart, objective wording, dossier metadata and escape-policy issues were addressed in [Apex story fixes](../apex-story-fixes/README.md). This review records the earlier build.

The individual Apex stories have a coherent identity: witnesses describe something unusual, the environment provides a more specific lead, and Oak interprets the encounter as research into evolution. Mewtwo supplies a useful counterpoint: manufactured power and Giovanni's attempt to control it. The main weaknesses are directions, stale dossier entries, and inconsistent consequences for retreating.

## Coverage and limitations

Tested a byte-identical temporary copy of `pokefirered.gba` in mGBA 0.10.5, with a new disposable save. Original main-ROM SHA-256: `19f8ba7461cc94156fc5515356cbd460054476984928709c3158d6ad4be91404`.

Normal input-only play covered the introduction, RED/BLUE naming, Charmander selection, first rival battle, Oak's expedition briefing, Route 1, Viridian's first Tangrowth rumour, Blue's Route 2 fieldwork scene, and the second Tangrowth rumour outside the forest. No old save, party modification or warping was used for that opening.

Later checks use explicitly staged conditions and native game dialogue/battle input. They are **not an uninterrupted full-campaign playthrough or a difficulty/balance certification**. They include post-Silph Saffron/Cubone, Nikki's third Tangrowth clue, Tangrowth's reveal/battle/escape, and the later Mewtwo scenes. Nikki's trainer-defeated flag was set to reach her post-battle clue without grinding. Travel was skipped with engine warps. The Silph-completed scene variable and Mewtwo-signal flag were staged rather than earned through those dungeons. A level-100 Charizard and one Master Ball were added to the disposable save for the final encounter check.

All eight Apex rumour chains, reveal conditions, retreat branches, Oak responses and the Mansion → Silph → Lance → Cerulean Cave sequence were also reviewed in source. Source-only observations are labelled below. No game source, main ROM, customisation ROM or existing player save was edited for this review.

`dialogue-log.tsv` records expanded field dialogue as it reached the game's text buffer; it is not a video, battle transcript, or proof that every page was displayed. Screenshots corroborate selected checkpoints. The first few log rows retain a logger character-decoding issue, not a game text defect. `rumour-source-audit.json` indexes source call sites; trainer opening dialogue needs to be read alongside its extracted post-battle messages.

## Changes I would prioritise

1. **Prevent Lance's flashback from being interrupted during restoration.** With ordinary A-button advancement automated at one press per 30 frames, the scene repeatedly returned to Lance's opening instead of recording his clue. After repeated cycles, the signal flag was 1, the aftermath flag was 0, and Mewtwo's third-rumour bit was absent. The dialogue log records repeated control/rejection/chase sequences. A likely cause is `RestoreFlashbackPlayerObjectEvents` releasing field controls while the calling event still has fades, dialogue and progression updates to execute (`src/field_specials.c:564`). A second run paused for 240 frames when the camera returned to the player: the final directions then completed, the rumour recorded, and the aftermath flag became 1. Retain the script lock through the caller's final release, then test both slow and rapid advancement. This is a runtime reproduction under staged post-Silph conditions, not a full naturally earned Silph playthrough.

2. **Repair the Silph → Lance handoff.** The active objective is still `ROUTE 7 RAMPAGE`, with “Witness the path left by GIOVANNI's signal on ROUTE 7.” Lance's actual scene is in Celadon Cave. That cave connects to Route 7, so the direction is nearby rather than wholly unrelated, but it does not tell a player what to look for. Giovanni's Silph departure gives no cave destination. Rename/reword the objective and add one concrete witness lead to the cave. Sources: `src/strings.c:1403`, `src/strings.c:1486`, `data/maps/SilphCo_11F/text.inc:86`, `data/maps/CeladonCave/scripts.inc:83`.

3. **Synchronise dossier witnesses and locations with their actual clues.** Source-confirmed discrepancies:

   | Dossier | Actual clue sources, in slot order | Stale dossier information |
   | --- | --- | --- |
   | Zapdos | Route 10 swimmer; Power Plant engineer; Power Plant engineer | Lists Power Plant / Route 10 / Route 10 and Engineer / Electrician / Picnicker |
   | Articuno | Route 20 scout; Route 20 sailor; Seafoam old man | First source says Swimmer; second location says Seafoam |
   | Moltres | Cinnabar old man; laboratory scientist; volcano kindler | Second location says Volcano, with Scientist / Hiker / Witness source labels |

   Some summaries also describe older clues rather than the conversation just heard. These undermine the logbook as a trustworthy record. Update each entry from its actual NPC text, then verify all three slots in the game. Source: `src/apex_log.c:239–333`.

4. **Introduce the three-witness rule when the first rumour is recorded.** The opening makes the expedition's purpose clear and the first two Tangrowth conversations record properly. The explicit three-rumour explanation currently appears in Blue's Route 11 scene, which a player heading toward the forest may not yet have seen. A short first-rumour explanation and a visible “1 of 3” prompt would make the investigative loop clearer. This is an editorial recommendation, not a progression failure.

5. **Make retreat rules consistent.** Source review: Tangrowth, Zapdos, Articuno and Mewtwo's escape branches do not set their fought flags; Mime Sr., Annihilape, Osscythe and Moltres do. The latter four are then suppressed by their map-transition scripts. Tangrowth's escape was exercised in the emulator: it vanished, with interacted=1 and fought=0. Choose a common retry policy, or explain a deliberate difference before the battle. The source currently treats retreat as a permanent resolution for only half the Apex roster.

6. **Keep recorded knowledge grounded in what the player heard.** In staged post-Silph Saffron, Cubone remains and successfully records the Osscythe rumour by saying only “Kyoo!” The dossier then says a Rocket took it from its mother. This avoids a missable-clue blocker, but supplies information that this route through the scene never conveyed. Give the abandoned Cubone a short observational line or a nearby civilian who explains its history. Similarly, the Silph President speaks conditionally about Giovanni calling Mewtwo, while his dossier entry already asserts that the signal forced Mewtwo to respond. Sources: `data/maps/SaffronCity/scripts.inc:32`, `data/maps/SaffronCity/map.json`, `data/maps/SilphCo_11F/text.inc:40`, `src/apex_log.c:249–253`.

## How each Apex story reads

| Apex | Investigation and payoff | Assessment |
| --- | --- | --- |
| Tangrowth | Viridian old story → forest-gate warning → flowers drawn toward the forest centre → instinct given root and vine | Strong first mystery. All three clue dialogues and the encounter were exercised; the third clue's battle was skipped. Nikki's party is level 18–20, then Tangrowth is level 50. Signal that this is a discovery to revisit rather than an immediate beginner objective. |
| Annihilape | Mt. Moon anger → Cerulean witness points west → Route 4's enormous Primeape → emotion outrunning control | Coherent emotional theme and useful westward direction. The revisit loop should be intentional and visible. Source review only. |
| Mime Sr. | Two cave entrances and Route 11 describe a community and invisible barriers → social protection and isolation | Strong fit between habitat, obstacle and species behaviour. Review the permanent retreat consequence. Source review only; cave traversal was not certified. |
| Osscythe | Saffron Cubone/Rocket → Lavender grief story → Tower mourner → loss and memory | Strong theme, weaker delivery through Cubone alone. “Atop” the Tower suggests the summit, but the encounter is on 4F; “within the Tower” would be more accurate. Cubone's fallback was exercised; the Tower encounter was source-reviewed. |
| Zapdos | Route 10 storm rumour → engineers describe power from the sky and persistent lightning → renewing energy | Clear local chain, impaired by the old dossier labels. Source review only. |
| Articuno | Route 20 witnesses and Seafoam old man recall a sudden volcanic-to-ice transformation → adaptation under pressure | Distinct environmental history. Correct the witness/location labels. The sailor's clue is in his battle introduction, so his joking post-battle line is not evidence of a missing clue. Source review only. |
| Moltres | Cinnabar renewal → laboratory explanation → volcanic pressure → energy returned to the world | Coherent renewal theme; it overlaps Zapdos's renewable energy, so emphasise land/ecology for Moltres and electricity for Zapdos. Correct the laboratory clue's location and source labels. Source review only. |
| Mewtwo | Mansion's empty vat → Silph's control device → Lance witnesses rejection → Giovanni confronts the limits of possession | Strongest overarching arc. The missing handoff into Celadon Cave interrupts an otherwise clear causal sequence. Later scenes were exercised under staged conditions; Mansion and Silph were source-reviewed. |

## Research completion and payoff

The quest deliberately uses “Found”/“recorded” terminology. `TryCompleteApexInstinctQuest` marks an Apex found when interaction begins, before the battle result, and completes the instinct study after two Apex interactions. Runtime Tangrowth inspection confirmed interacted=1, fought=0, completed=1 during the encounter. That is reasonable for field research; it should not be presented to players as proof of victory or capture.

The individual Oak responses make the discoveries meaningful, but the all-eight completion branch only changes quest flags. Consider an explicit final Oak conversation comparing instinct, adaptation, grief, social behaviour and manufactured power. This would make completing the collection feel like a conclusion rather than another checked box. This is a source-based editorial suggestion, not a tested missing mandatory cutscene.

A separate source concern remains for follow-up: the common rumour-reveal helper does not check fought flags before spawning an Apex. Repeating a clue on an encounter map could reintroduce an already resolved Pokémon. That candidate was not reproduced here and should be tested before changing it.

## Final runtime outcome

After the paced Lance check, the aftermath flag was 1 and his rumour was recorded. Mewtwo then appeared in Cerulean Cave. Using the staged level-100 Charizard, I defeated level-70 Mewtwo through normal battle input; the Master Ball was not used. Oak's response and Giovanni's concluding conversation ran, Giovanni left, and his cave-completion flag became 1. The Viridian Gym door scene advanced to 1. The main ROM and original save both passed their original SHA-256 checks afterward. The emulator was closed and the temporary ROM copy removed.

There was one discarded setup attempt at Cerulean Cave before Lance's flags had finished recording; it is not counted as valid story progression. The repeated Lance scenes in the later rapid-advance check happened before any subsequent warp and were separately reproduced with the signal flag set. Pausing during camera restoration allowed completion. This distinction matters: a harness transition is not itself evidence of a game bug.

Selected evidence:

- `10-first-tangrowth-rumour.png`: normal opening investigation.
- `13-route2-rumour.png`: second clue outside the forest.
- `22-nikki-dialogue-retry.png`: third clue, trainer battle skipped.
- `26-tangrowth-battle.png`: level-50 reveal.
- `30-tangrowth-escape-result.png`: retreat check.
- `18-cubone-clue.png`: post-Silph fallback clue.
- `44-lance-after-restore.png`: paused restoration, final directions visible.
- `49-mewtwo-mid-battle.png`: staged final encounter.
- `50-giovanni-conclusion.png`: concluding dialogue.
- `runtime-results.txt`: final flag checks.

Suggested next work is to fix the Lance control-release timing, update the objective and dossier text, and agree one retreat policy. Then run a continuous campaign pass to test natural route access, difficulty, and whether players discover the witnesses without prior knowledge.
