# Full story playthrough — completed

Historical report: Seafoam was subsequently simplified to three floors. See
[the replacement layout and tests](../seafoam-simplification/README.md). The ROM
hash and boulder-puzzle findings below describe the earlier playthrough build.

Completed September 14, 2026. The accelerated campaign reached all eight badges,
all eight Apex encounters, both League championships, and the Indigo ending.
The main ROM has been rebuilt with the fixes below. No progression blocker remained
on the tested route after verification.

## Dialogue assessment

The central sequence now connects: gym aides point toward their leaders; trials
explain what to do; witness dialogue supports the outdoor official battles; and
leaders explain the next reward or destination. The Silph / Route 7 flashback /
Celadon Cave / Cerulean Cave sequence explains the blast and its aftermath without
requiring Celadon Cave to exist before it. Giovanni’s Gym and Rocket League closing
lines now share the same trust-focused resolution.

The weakest guidance was practical rather than a missing central plot: which
plants hold Erika’s root, which rocks Brock means, who sells Strength, and where
to go after the final badge. Those leads are now explicit. One optional editorial
opportunity remains: Oak’s normal lab conversation gives a Dex review after all
Apex encounters; there is no dedicated final “all Apex investigations completed”
conversation. The individual encounters each have their own explanation.

## Changes from this playthrough

- Clarified the initial rival’s Route 1 direction and Oak’s warning about strong
  ordinary wild Pokémon versus the permanent consequences of fleeing an Apex.
- Named wild Oddish/Bellsprout as Energy Root sources and exposed Mt. Moon rocks
  as Brock’s targets; updated the matching journal guidance.
- Clarified Misty’s trade prompt and how to use the Cascade Board from Key Items.
- Added useful tutor eligibility hints for Sabrina and Surge; corrected Sabrina’s
  post-trial journal, aide and Chingling/friendship/night guidance.
- Explained Seafoam’s current/boulder/Strength connection and identified the
  Strength seller beneath Celadon’s Game Corner. Clarified the poster switch.
- Removed the unsupported ferry assumption from the Cinnabar rival conversation.
- Added Giovanni’s Ground-type counters and explicit Route 22 → Route 23 →
  Victory Road directions. Matched his Rocket ending to his story resolution.
- Clarified Rocket Champion eligibility, reward amounts and multiplier duration.
- Sealed premature Celadon Cave access until the blast sequence.
- Fixed completed Giovanni scene triggers that could lock movement in the
  Mansion basement and Cerulean Cave on later visits.
- Corrected Celadon Cave, Route 16 gate/stair and Saffron Fan Club warp targets.
- Moved Wade off the Power Plant’s only upper passage and Ivo off the required
  first-floor Victory Road boulder route, preserving their battles.
- Restored falling-boulder reveal metadata in Seafoam and Victory Road. Actual
  pushes now create the lower-floor boulders needed to finish the puzzles.
- Fixed the Hall of Fame player-portrait crash: trainer portraits now use a
  dedicated static animation instead of a null animation entry for retired
  trainer picture slot zero. Pokémon portrait animations are unchanged.

These changes sit alongside the project’s preexisting uncommitted trainer,
League, gym and sprite work; the complete Git diff is not solely this playthrough.

## Live coverage

| Area | Result |
| --- | --- |
| Opening | Fresh intro, Oak/Route 1, starter/rival, Dex and Apex briefing |
| Returning leaders | Brock’s fossils, Misty’s currents/generators, Surge’s tutor; returned Gym battles and aide advice |
| Field leaders | Erika’s root, Sabrina’s Dream Eater, Koga’s stone trial; witness arrivals, official battles and badges |
| Final Gyms | Blaine, Porygon machine and Giovanni; all eight badges earned |
| Gifts/trades | Budew, Chingling, Golbat and Porygon gift branches; Relicanth/Gyarados/Raichu trade prompts read |
| Apex | Tangrowth, Annihilape, Osscythe, Mime Sr., Moltres, Mewtwo, Zapdos and Articuno defeated after their clues |
| Main investigation | Mansion, Silph, Giovanni, Route 7 accounts, Lance flashback/repeat interaction and post-blast cave sequence |
| Geography/puzzles | Connecting routes, water travel, corrected exits, Seafoam currents and all required Victory Road switches/drop |
| Indigo | Locked door before four wins; Brock, Koga, Lorelei, Agatha; Champion; Hall of Fame save, corrected presentation, credits and Pallet return |
| Rocket | Sable, Ariana, Shion, Viper; Giovanni; 5,000 coins received, pouch 150 → 5,150 |
| League menus | Both records screens show 5 wins, 0 losses, 1 championship and streak 1; Rocket records show 5,000 coins. Reward/prize-board explanation opens and decline returns control |

Indigo’s settled battle probes confirmed Level 50 teams for matches 2–5. Each
pool match restored all 600 bytes of the prepared party. The Champion comparison
was audited after the ending: original levels, EXP, moves, PP, items, identity,
HP/status and stats were preserved; only Champion ribbons and ordinary walking
friendship changes differed. Rocket retained the prepared team’s actual levels.

## Test method and limits

This was an accelerated story/progression test, not a normal-speed difficulty or
balance certification. It used a disposable save, high-level test Pokémon, native
healing and automated button input. The opening rival was won with the starter;
later wild/trainer losses prompted the accelerated fixture. A prepared six-member
team was used for League verification. That fixture included Mewtwo; the actual
Apex Mewtwo encounter was won by defeating it, not by capturing it.

Story clues, badges, trials and the required puzzle flags were earned through
map interactions. Checkpoints were replayed for the Silph attempt and corrected
Seafoam puzzle. Separate diagnostics tested pre/post-blast gates and completed
scene revisits. After the naturally earned Indigo title exposed the portrait
crash, the corrected Hall of Fame presentation was explicitly replayed through
the native special. The final Indigo records visit used a diagnostic warp after
both League runs; it is not counted as continuous travel. A post-Rocket checkpoint
was restored after automated input accidentally accepted another entry prompt,
then the prompt was declined correctly before checking the terminal.

Not covered exhaustively: every NPC/diary page, every optional quest, friendship
evolutions, completed leader trades, a full Pokédex, all League pool permutations,
all failure/withdrawal paths live, or normal-level combat balance. Automated text
logs include duplicates and capture expanded strings; they do not prove every
page was individually inspected. Failed reproduction/probe rows are retained in
runtime-tests.tsv with subsequent corrections. Some early probes used stale
layout offsets or sampled too early; those are documented as probe errors, not
unresolved game defects.

## Validation and build

All eleven focused checks passed: dialogue widths, 880 fixed warp destinations,
Power Plant reachability, falling-boulder links, Victory Road push-state regression
(including the original blocked fixture), trainer portrait animation, League
records, save layout, Indigo battle rules/maps and Battle Tower retirement.
`make -j4` succeeds; the final invocation reports the build is up to date. The
linker emits its existing RWX-segment warning. Current usage: EWRAM 246,112 bytes,
IWRAM 30,064 bytes, ROM section 15,322,081 bytes.

Main ROM and tested ROM SHA-256:
`78177aa4d1b07b96e5f3db5bcbd05a517239ca90e243acc68925152743772e3e`.

Protected files remained unchanged:

- Main save: `816eb072055dad4bbd8ca2b4d596cf468e58b68384664931a91e814dbb824b08`
- Player-customisation ROM: `880e9e2dc73c16084b10dfc3cb13bfa1b3535b637277e02a576fb8aee0194d5b`

## Evidence

- [Chronological working journal](playthrough-journal.md)
- [Runtime checks, including reproduced failures and corrected probes](runtime-tests.tsv)
- [Expanded dialogue log](dialogue-log.tsv)
- [Corrected Hall of Fame player screen](520-hof-player-clean.png)
- [Rocket records after the full run](537-rocket-records.png)
- [Rocket multiplier offer](540-rocket-multiplier-offer.png)
- [Indigo records after the full run](544-indigo-records.png)
