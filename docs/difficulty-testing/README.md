# Difficulty baseline — 2026-09-14

This pass completed 53 native battles and audits normal-level combat and progression. It is a controlled emulator benchmark, **not a fresh-save, naturally levelled full playthrough or a difficulty certification**. The main ROM and main save were not modified.

## Main findings

1. **Opening-route wild levels are the largest progression concern.** Route 1's daytime table assigns 40% of encounter probability to slots whose minimum level is at least 18, including level 36–39 Pidgeot. At night the equivalent share is 30%. Route 2, Route 3, Viridian Forest and Mt. Moon also mix early catches with much higher-level Pokémon. `ChooseWildMonLevel` uses the slot's configured range without a badge or party-level adjustment. These encounters can punish a new starter or provide catches that skip the early level curve. Consider progression-based availability or clearly separated dangerous areas while retaining the habitat species.

2. **Friendship gift levels do not follow the open-order gym scaling.** Erika, Sabrina and Koga give level-30 Pokémon regardless of badge count. The additional first-gym Erika test won with four level-14 Pokémon, taking no HP damage; her already-verified reward script then offers level-30 Budew. The next badge tier's leaders peak around 23. Scale the gift's level to progress while retaining the high friendship and evolution lesson.

3. **Early Apex encounters need an informed commitment.** Tangrowth is level 50 even when discovered in Viridian Forest. The level-13/14 test team lost; the level-50 team won. Its interaction immediately starts the encounter, and its result script consumes the encounter after defeat or escape. A readiness/commitment prompt before starting would preserve the one-chance design without forcing an uninformed early attempt. Other non-Mewtwo Apex are also fixed at 50; Mewtwo is 70. No retry rules were changed.

4. **The gym curve is broadly workable with appropriate coverage.** All eight sampled badge-tier configurations were won in three trials after correcting the test selector's handling of ability immunities. Brock and Erika were easy with strong type advantages; Koga, Sabrina and Giovanni took more resources. These results do not justify blanket boss nerfs.

5. **The League Champions reward preparation.** The original fully evolved team lost to Agatha and both Champions. Psychic/Ice coverage and a revised lead order were tested next. Agatha was beaten, and the Indigo Champion was beaten with just one Pokémon at 10 HP. The Rocket Champion also fell after the coverage/lead-order change, with two Pokémon remaining. These are individual full-health battle tests, not five-fight endurance runs.

## Gym results

Each row uses the corresponding badge count in a conventional order. It does not cover every leader at every possible badge count. The static audit records all 49 tier lists in `gym-tiers.json`.

| Leader | Badges before battle | Player levels | Enemy levels | Wins | Turns | Pokémon remaining |
|---|---:|---:|---:|---:|---:|---:|
| Brock | 0 | 13–14 (four Pokémon) | 12–16 | 3/3 | 1 | 4 |
| Misty | 1 | 21–22 (five Pokémon) | 17–23 | 3/3 | 3–4 | 5 |
| Surge | 2 | 25–26 | 23–27 | 3/3 | 5–7 | 3–4 |
| Erika | 3 | 30–31 | 24–32 | 3/3 | 3–5 | 5–6 |
| Koga | 4 | 36–37 | 31–38 | 3/3 | 11–14 | 3–5 |
| Sabrina | 5 | 40–41 | 38–42 | 3/3 | 11–12 | 1–4 |
| Blaine | 6 | 45–46 | 41–47 | 3/3 | 3–6 | 5–6 |
| Giovanni | 7 | 48–49 | 47–50 | 3/3 | 7–9 | 1–5 |

The initial Misty seed-37 draw and three Koga results are superseded by their `-ability-aware` retests. The first controller used type effectiveness but missed immunity abilities such as Volt Absorb and Levitate. All raw runs remain in `results.tsv`; the table uses the corrected results. Three trials are observations, not an estimated player win rate.

## Apex results

| Encounter | Player level | Apex level | Outcome | Turns | Pokémon remaining |
|---|---:|---:|---|---:|---:|
| Tangrowth | 50 | 50 | Won | 5 | 6 |
| Annihilape | 50 | 50 | Won | 13 | 4 |
| Mime Sr. | 50 | 50 | Won | 2 | 6 |
| Osscythe | 50 | 50 | Won | 2 | 6 |
| Articuno | 50 | 50 | Won | 9 | 5 |
| Zapdos | 50 | 50 | Won | 15 | 5 |
| Moltres | 50 | 50 | Won | 9 | 5 |
| Mewtwo | 60 | 70 | Won | 9 | 4 |
| Tangrowth, early-team check | 13–14 | 50 | Lost | 19 | 0 |

These are one trial each, using the production event-mon creation and Apex battle functions. They establish feasible ordinary-team wins, not optimal intended first-encounter levels. The pilot attacks and replaces fainted Pokémon; it does not proactively switch to a counter, so turn counts can be inflated by poor lead matchups.

## League sample

All Indigo enemies were observed at level 50. The five sampled Indigo battles use the production normalization, doubles, item restrictions and restoration logic. Rocket uses singles at the trainers' actual levels. Player teams start at level 50. Each match starts healthy; no player Bag healing or Revives were used.

| Opponent | Baseline | Coverage retest |
|---|---|---|
| Indigo Lorelei | Won, 5 remaining | — |
| Indigo Bruno | Won, 6 remaining | — |
| Indigo Agatha | Lost, including immunity-aware retest | Won, 3 remaining |
| Indigo Lance | Won, 1 remaining | — |
| Indigo first Champion (Charmander variant) | Lost, including immunity-aware retest | Won, 1 remaining, 10 HP |
| Rocket Petrel | Won, 4 remaining | — |
| Rocket Archer | Won, 2 remaining | — |
| Rocket Ariana | Won, 4 remaining | — |
| Rocket Proton | Won, 5 remaining | — |
| Rocket Champion Giovanni | Lost | Won, 2 remaining, 177 total HP |

The baseline endgame party is Venusaur, Gyarados, Pidgeot, Golem, Raichu and Arcanine, with retained level-up moves. The coverage party replaces Pidgeot/Golem with Kadabra/Lapras and changes the lead order. Kadabra is available on Routes 7/16; Lapras remains the level-25 Silph Co. 7F gift. Their test moves are learned by level 50. No legendary player Pokémon, trained EVs, rare competitive held items or breeding were used.

## Method and limits

- Disposable ROM/save: `/tmp/difficulty-testing/story.gba` and `story.sav`, staged from the prior disposable campaign save. The main save was never loaded.
- Tested ROM SHA-256: `dbc641b9458f7db36e8e498147671fff674f96919184cbd8243233ec75333967`.
- Gym parties were created with native `givemon`, random IVs/natures, zero EVs, and the game's default moves at each level. The later-game baseline deliberately retains some unevolved members and has no TM optimization, so it is a conservative party rather than a claim about an optimal build.
- Endgame fixtures use specified, legally retained level-up moves; only fixture creation edits these moves. Battle HP, PP, damage, enemy AI and outcomes run natively. No damage, HP, RNG outcome, or win flag is injected during combat.
- The pilot chooses damaging moves using estimated type/STAB effectiveness and sends a surviving replacement after a faint. Later versions account for fixed-damage moves, friendly fire and common immunity abilities. It does not plan status setups, anticipate enemy decisions, use Bag items, or proactively switch. Snapshot seeds fix fixture initialization; these are not exhaustive random-seed statistics.
- Tests are staged direct battles in a neutral arena with the relevant badge flags. They do not measure travel attrition, natural experience gain, catch success, optional grinding, every starter, every gym order, every League pool member, or a complete consecutive League run.
- Enemy trainer items remain enabled in ordinary gym/Rocket rules and disabled under Indigo's actual rule set.
- The earliest Apex tests used the original type-aware pilot; subsequent tests improved fixed-damage/friendly-fire handling, then common immunity handling. Original failed Agatha/Champion runs are preserved alongside the retests. Do not treat this mixed-policy sample as a ranking by win rate.

## Evidence and validation

- `cases.json`: every exact party, level, trainer, seed and prescribed move set.
- `results.tsv`: columns are case name, outcome (1 win / 2 loss / 3 draw), completed turn counter, surviving Pokémon, remaining HP, total maximum HP. Captured when the engine produces its outcome, before post-battle healing.
- `*-before.bin`, `*-enemy.bin`, `*-after.bin`: native party records. The first Brock test predates enemy snapshot capture; its before/after records are retained.
- `harness/`: battle driver and pilot versions for inspection. They require the mGBA `qa` harness and symbol table and are diagnostic fixture tools, not game code.
- `audit_progression.py`: generates `gym-tiers.json` and `early-wild.json` from production data.
- `verify_fixtures.py`: checks exact species/levels, valid encrypted Pokémon checksums, zero EVs, prescribed moves, completed outcomes, and level-50 Indigo enemy normalization.

No balancing changes were made. Recommended first changes are the opening wild encounter curve and progress-scaled friendship gifts, followed by an explicit pre-battle Apex readiness prompt. Retest natural leveling and consecutive League endurance after those choices are settled.

## Follow-ups

Apex readiness confirmations have now been implemented and checked for all eight encounters; see `../apex-readiness/README.md`. Further paired Champion tests retained the original species and isolated movesets/lead order; see `../champion-movesets/README.md`.
