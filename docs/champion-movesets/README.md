# Champion movesets and deployment comparison

Twenty native mGBA battles compared the original six Pokémon at level 50, using the same individual Pokémon records within each paired test. No opponent teams were changed.

**The original species can beat both Champions.** Revised moves alone did not consistently rescue the losing cases. Revised moves plus a different lead order beat both Champions in the previously losing seed-37 fixture, with three Pokémon remaining in each battle. Kadabra/Lapras substitutions are therefore not necessary to make these matchups winnable.

## Paired results

Three fixture seeds (11, 37, 83) were tested for each moveset. These small samples are observations, not estimated win rates.

| Setup | Indigo Champion (first Charmander variant) | Rocket Champion Giovanni |
|---|---|---|
| Original retained level-up moves; original order | 1/3 wins | 1/3 wins |
| Initial TM revision; original order | 1/3 wins | 0/3 wins |
| Refined TM/tutor revision; original order | 1/3 wins | 1/3 wins |
| Refined moves, changed order; seed 37 only | Win: 3 remaining, 335 HP, 10 turns | Win: 3 remaining, 413 HP, 19 turns |

Refined moves improved some winning runs: Indigo seed 11 finished in 8 rather than 10 turns; Rocket seed 83 finished with two Pokémon and 158 HP rather than one with 107 HP. They did not change the losing seeds into wins by themselves. Changing order then rescued seed 37 without changing species, levels, stats, individual identity, EVs, friendship or held items.

The original order was Venusaur, Gyarados, Pidgeot, Golem, Raichu, Arcanine. The successful order tests used:

- Indigo doubles: Raichu and Pidgeot lead, followed by Venusaur, Gyarados, Golem, Arcanine.
- Rocket singles: Arcanine leads, followed by Raichu, Pidgeot, Golem, Gyarados, Venusaur.

## Refined test moves

| Pokémon | Moves |
|---|---|
| Venusaur | Giga Drain, Sludge Bomb, Sleep Powder, Protect |
| Gyarados | Hydro Pump, Ice Beam, Hyper Beam, Thunderbolt |
| Pidgeot | Double-Edge, Aerial Ace, Steel Wing, Quick Attack |
| Golem | Earthquake, Rock Slide, Brick Break, Protect |
| Raichu | Thunderbolt, Brick Break, Iron Tail, Quick Attack |
| Arcanine | Flamethrower, Extreme Speed, Iron Tail, Bite |

These are tested diagnostic sets, not an assertion that they are optimal. The TM moves are supported by each species' machine compatibility and available through the expanded Celadon prize-room inventory. Rock Slide is the Rock Tunnel B1F tutor; Double-Edge is the Victory Road 2F tutor. They are distinct tutors, so the tested setup does not require duplicating a one-time tutor.

The engine still chooses physical/special damage by type. For example, Ice Beam uses Gyarados's lower Special Attack, while Hyper Beam uses its higher Attack. Coverage alone does not guarantee more useful damage. Surf also receives the engine's doubles spread reduction when hitting two enemies; the refined set retains Hydro Pump for concentrated Water damage. Hyper Beam's recharge and Double-Edge's recoil still apply normally.

## Method and limits

- Disposable `/tmp/champion-movesets/story.gba` and save; main ROM build includes the Apex readiness prompts, SHA-256 `a7119753dc0f869a1c4dc862cfbecdfb9f972aa48928522c106c1c399a53465b`.
- Original party: Venusaur, Gyarados, Pidgeot, Golem, Raichu, Arcanine; all level 50, zero trained EVs, no held items or player Bag healing.
- Baseline Pokémon were created natively. Revised cases clone the corresponding baseline records before modifying only moves/PP/checksums. Lead-order cases additionally reorder those exact records. `verify.py` validates those invariants.
- Normal native opponent AI, damage, accuracy, status, PP, fainting and outcomes. Indigo's actual normalization/doubles/item rules and Rocket's singles rules were used. No HP, damage, victory or outcome was injected during a battle.
- The test pilot estimates damaging moves, respects common immunity abilities and replaces fainted Pokémon. It does not proactively switch, plan status/Protect combinations, or use Bag items. These are controlled combat tests, not a human strategy ceiling, natural-level playthrough, or consecutive League endurance test.
- Initial battle RNG is reseeded consistently before each comparison; move choices naturally change subsequent RNG consumption. A matched starting seed does not make every later random event identical.

`cases.json` records every move ID and party order. `results.tsv` gives outcome, turns, survivors, remaining HP and maximum HP. Native before/enemy/after party buffers are retained. `python3 docs/champion-movesets/verify.py` validates all 20 completed battles and unchanged paired Pokémon attributes.

The practical next step is to make useful TMs/tutors and lead-pair preparation clear to the player. This test does not support weakening the Champions simply because the first team setup lost. No Champion or player movesets were changed in the main ROM.
