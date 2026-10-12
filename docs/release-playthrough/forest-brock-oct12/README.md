# Fresh-save forest and Brock progression — 12 October 2026

Tested `/tmp/kanto-newgame-oct12/theme.gba` using the separate `theme.sav` from the opening playthrough. ROM SHA-256: `f2b40f8f51b7ba46d27707fe4eee54212e5e4b590d399e13a7e370b96b2ad24c`.

Movement, conversations, shopping, catching, battles, healing and saves used native game inputs. The path helper consulted collision data and trainer sight lines; battle assistance selected damaging moves using live battle state and pressed controller buttons. No party, levels, items, flags, outcomes or positions were injected. Fast-forward shortened repeated travel and animations. This is a progression smoke test with one small party, not comprehensive balance certification or a frame-by-frame graphics audit.

## Verified

- Route 2 rival navigation dialogue completes and releases control.
- Natural Viridian Forest southern entrance, location splash, northern exit and Route 2/Pewter connections work. Entrance/exit tiles require walking through the threshold; no gatehouse intervenes.
- Rick's dialogue and battle front match his Bug Catcher class. In the first run, level-5 Bulbasaur defeated his two level-6 Pokémon, reached level 7, and finished at 4/23 HP. The nearby Potion restored all 19 missing HP.
- A later replay caught level-8 Bellsprout with a Poké Ball. Registration, catch experience and returning to the field completed normally.
- Sammy's encounter at the northern forest path completed successfully.
- RETREAT escaped ordinary wild encounters, including Mt. Moon's higher-level encounters.
- Defeats recovered normally: home before using a Pokémon Center, then Pewter's Center after registering there. Party HP, PP and status were restored.
- Pewter's Gym guide correctly says Brock is in Mt. Moon and unlocks his trial.
- Route 3's rival scene completes; Mt. Moon entrance and stairs are functional.
- Brock's first meeting explains exposed fossil rocks. Interacting with the rock at B1F (6,12) gave a Helix Fossil.
- Brock recognised the Helix Fossil, awarded a Dart Fossil, walked away, disappeared from Mt. Moon, and appeared in Pewter Gym. Trial state became 3.
- His zero-badge challenge opens as a double battle with Geodude level 12 (PEBBLE) and Larvitar level 16 (CHIP).
- The naturally acquired Bulbasaur level 8 / Bellsprout level 10 party lost that challenge. Recovery returned to Pewter Center, and trial state remained 3 so the challenge is still available. No victory reward was tested in this run.
- Final state saved and reloaded through CONTINUE in Pewter Center: Bulbasaur level 9, Bellsprout level 10, both healthy; three Potions, nine Poké Balls, one Helix Fossil and one Dart Fossil. Brock trial remains 3.

## Balance observations

Breeder Eli can challenge the player near the southern forest corridor with Pichu 16, Budew 18 and Ledyba 20. This is a sharp jump from Rick's two level-6 Pokémon. The small early party lost; waiting for Eli to look away allowed safe passage. Route 1's Camper Milo also defeated this party during the return trip. These observations support reviewing how optional stronger trainers are signposted, rather than concluding that all early trainer levels must be reduced.

Brock was attempted without grinding, further catches or battle-item use. This party's defeat does not establish that Brock needs a nerf; party building and preparation remain untested here. Other starter choices, the alternate Dome Fossil branch, and the victory/badge branch were not exercised.

## Test-session interruption

The first emulator session stopped during a Kakuna catch/control check. The cause was not established; this is not recorded as a reproduced ROM defect. That session's Rick win and Potion use have screenshot/readout evidence, but its unsaved progress was lost. The test resumed from the saved Viridian checkpoint, bought five extra Poké Balls and three Potions, and replayed the route. Subsequent Bellsprout capture, progression, saves and CONTINUE worked. No save state or fabricated progress was used to recover the lost segment.

## Evidence

- `entrance-outside.png`, `entrance-settled.png`, `forest-north-exit.png`, `forest-north-arrival.png`
- `rick-intro.png`, `rick-progress2.png`, `rick-result.png`, `potion-healed.png`
- `bellsprout-catch.png`, `bellsprout-recorded.png`, `breeder-intro.png`, `whiteout-recovery.png`
- `brock-gym.png`, `brock-meeting.png`, `helix-found.png`, `brock-fossil-recognition.png`, `brock-departed.png`, `brock-returned.png`
- `brock-battle-start.png`, `brock-result.png`, `pre-brock.json`, `post-brock.json`, `restored.json`, `final-save.png`, `final-continue.png`

No production changes were needed for the tested transitions and quest. Main campaign save SHA-256 is unchanged: `001a04fc396009ff0a8fecd353eba53a721b32f6577dc2c19950472aa66b4ac4`.

## Review accepted — 12 October 2026

The user has ticked off the playthrough and trainer line-of-sight reviews. Both are closed as accepted for the current scope; further testing is not a release task unless a new issue is reported. The evidence and coverage limits above remain unchanged: acceptance does not imply additional live tests were performed.
