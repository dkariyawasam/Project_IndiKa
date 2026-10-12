# Balance follow-up — 11 October 2026

Started by comparing the current seven-tier Gym rosters against the September controlled benchmark, which recorded 53 battles. Fourteen of the 49 tier lineups changed; their level arrays are unchanged. The comparison checks species and levels only, not moves, items, stats, abilities or AI, so unchanged rows are not a current-build balance certification.

Fresh Gym battle priorities:
- Surge: all seven tiers changed, including the Tynamo family.
- Sabrina: tiers 2–6 changed, including the previously sampled sixth tier.
- Koga: sixth tier changed.
- Erika: seventh tier changed.

The prior sampled conventional-order Surge and Sabrina fights therefore need current-build repeats. No balancing changes were made during the initial comparison. Fresh controlled battles are recorded below. Custom-ability-specific testing remains deferred. Exact old/new lineups are in gym-roster-changes.json. The September evidence is preserved in docs/difficulty-testing.


## Current-build controlled repeats

ROM SHA-256: `9aaa9802cbd0b212a2d41c3a3ef353dee5c7fe10d0c5ea4633a817268cad1ff8`.

Native mGBA battles on a disposable fixture, using the September party species and levels, three seeds per leader. Surge uses tier 3 (two badges); Sabrina uses tier 6 (five badges). Parties have native random IVs/natures, zero EVs and native level-up moves. No Bag items or proactive switching; the input controller selects attacks and replaces fainted Pokémon. This is a controlled roster check, not a naturally levelled campaign or exhaustive difficulty certification. The same seed does not guarantee the same full RNG stream across harness/build changes.

The `live` directory stores case definitions, binary party snapshots and outcomes. TSV columns: case, native outcome (1 = win, 2 = loss), completed-turn counter, survivors, remaining party HP, total maximum party HP. The counter excludes a final partial turn. Starting party checksums, species, levels and zero EVs are validated by `verify-fixtures.py` from the repository root.

Two early trial attempts had incomplete/invalid starting snapshots and were excluded; their artifacts are retained under `live/superseded`. The final driver waits for script completion and checks party integrity before starting the battle. `driver.lua` requires the existing QA callback harness and current ELF symbols in `qa.sym`; it is not a standalone script.

Surge's three validated trials were wins with six survivors and 89–90% party HP remaining. This fixture starts with Graveler and Ivysaur, so it strongly favours the player. Do not strengthen Surge solely on this sample.

The campaign save remained byte-for-byte unchanged (SHA-256 `001a04fc396009ff0a8fecd353eba53a721b32f6577dc2c19950472aa66b4ac4`). No production balance changes were made.


| Trial | Result | Completed turns | Survivors | Party HP |
|---|---|---:|---:|---:|
| Surge 11 | Win | 3 | 6 | 389/432 |
| Surge 37 | Win | 4 | 6 | 368/413 |
| Surge 83 | Win | 3 | 6 | 376/422 |
| Sabrina 11 | Win | 12 | 1 | 88/648 |
| Sabrina 37 | Win | 10 | 2 | 166/665 |
| Sabrina 83 | Loss | 14 | 0 | 0/669 |

All 36 starting Pokémon passed checksum, species, level and zero-EV validation; all six captured enemy rosters matched the expected species/levels. Sabrina produced two close wins and one loss for the controller. Keep her roster unchanged on this evidence: the controller lacks tactical switching, setup/status strategy and Bag use. These three runs do not establish a population win rate. Surge was comfortable with the deliberately favourable opening pair; a broader party sample would be needed before strengthening him.

The September sample recorded Surge wins with 3–4 survivors and Sabrina wins with 1–4 survivors. Those are historical context only: native IV/nature/RNG streams and other mod mechanics can differ, so this is not an isolated experiment attributing the difference to roster changes.

Koga tier 6 and Erika tier 7 received one targeted live trial each after adding Leafeon to Erika tiers 3–7; see [the follow-up](koga-erika/README.md). Both defeated the limited controller fixture. Broader party sampling remains untested. Natural-progression pacing is separate from these controlled tests. Custom-ability-specific testing and sprite work remain deferred.


Prepared-party follow-up on 12 October: [both Koga and Erika were beaten](prepared/README.md) with evolved parties and deliberate moves. Koga: 28 completed turns, 3 survivors; Erika: 5 completed turns, 6 survivors. No balance changes recommended from these individual samples.
