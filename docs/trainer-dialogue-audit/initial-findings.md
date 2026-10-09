# Trainer dialogue and reference audit — 2026-10-05

## Result

Not every trainer currently has distinct, class-appropriate dialogue. There are also trainer identity / defeat-flag wiring problems, even though the inspected text pointers resolve.

This is a **static source review**, not a live conversation or rematch playthrough. No game scripts, trainer tables, dialogue or ROM were changed during this audit.

The generated inventory covers 253 compiled map definitions, 444 script-reachable encounter sites (including conditional teams), 920 distinct battle commands including rematches, and 1,773 reachable dialogue labels. The counts are not unique people: doubles, story branches and challenge rosters need interpretation. Script tracing includes both sides of conditions and follows the event assembly include graph; it does not evaluate game flags or preprocess assembly conditions.

- No missing map-root scripts detected.
- No unresolved battle text operands or missing battle-text `$` terminators detected.
- No unresolved referenced `_Text_` labels detected in the scanned message commands.
- No dynamic battle macro operands left unresolved by the existing placement audit.
- 75 exact-text duplicate candidate groups. These include legitimate system messages, conditional rosters, shared prompts and intentional dialogue; **75 is not a count of defects**.
- The existing trainer-table wiring audit found ten unused party definitions, but no reported active missing trainer constants, entries or parties. Its overall result is failure because of those unused definitions.

## Priority 1: rematches change the trainer's identity

The VS Seeker resolves these rematch IDs to a different class / portrait, and in four cases a different named character. `BattleSetup_ConfigureTrainerBattle` uses `GetRematchTrainerId`, so these are not merely misleading constant names.

| Location / original trainer | Rematch identity |
| --- | --- |
| S.S. Anne — Lady Ann | Youngster F Ann |
| S.S. Anne — Trendsetter Dawn | Youngster F Dawn |
| S.S. Anne — Trendsetter Tyler | Youngster M Tyler |
| S.S. Anne — Trendsetter Dale | Fisherman Dale |
| Pokémon Tower — Channeler Angelica | Aroma Lady Angelica |
| Victory Road — Ace Trainer Gregory | Ruin Maniac Gregory |
| Viridian Forest — Bug Catcher Rick | Roughneck Isaiah |
| Viridian Forest — Bug Catcher Doug | Roughneck Zeek |
| Viridian Forest — Bug Catcher Sammy | Roughneck Jamal |
| Viridian Forest — Bug Catcher Anthony | Roughneck Corey |

Evidence: `src/vs_seeker.c`, `include/constants/opponents.h`, `src/data/trainers.h`. Exact before/after records are in `inventory.json` → `rematch_identity_mismatches`.

Recommended fix: preserve each original trainer's name, class, portrait and appropriate party in a dedicated rematch entry. Do not blindly change the aliased trainer entries: some are used by other trainers.

## Priority 1: five ordinary identities share defeat flags across maps

| Trainer | Simultaneously placed locations | Shared base identity |
| --- | --- | --- |
| Kindra | Route 15 and Route 24 | `TRAINER_PICNICKER_KINDRA` |
| Chester | Route 15 and Route 23 | `TRAINER_BIRD_KEEPER_CHESTER` |
| Rina | Route 22 and Route 25 | `TRAINER_TRIATHLETE_F_LAND` |
| Violet | Route 5 and Viridian Forest | `TRAINER_AROMA_LADY_VIOLET` |
| Nikki | Route 8 and Viridian Forest | `TRAINER_AROMA_LADY_NIKKI` |

Their local dialogue is different and generally fits each location. However, all ten placed objects have visibility flag `0`, and their battle commands resolve to the same numeric trainer IDs within each pair. Trainer defeat state uses `TRAINER_FLAGS_START + trainerId`; defeating one therefore marks the other as fought and can skip that other encounter's introduction / first fight.

These are separate from intentional shared identities for gym leaders appearing at their story locations and in league challenge rosters. Nikki's forest script also carries an Apex rumour; preserve that story path when separating identities.

Recommended fix: assign separate IDs and rematch entries to the ordinary placements, retaining existing IDs where save compatibility matters. Distinct names would make them clearer as different people; a travelling-character interpretation would require explicit movement/visibility logic instead.

## Priority 2: 23 trainers still use placeholder-style full dialogue sets

The repeating pattern is “This [CLASS] knows [AREA] well!”, “[AREA] wins today!”, “[AREA] has more to teach than one battle can show”, and “Back on [AREA]? Then we battle again!”

| Area | Trainers |
| --- | --- |
| Vermilion Harbor (12) | Kai, Mina, Otto, Vale, Dane, Lina, Pax, Morgan, Rafe, Skye, Rory, Elle |
| Viridian Forest (4) | Niles, Cedar, Hazel, Elin |
| Fuchsia Forest (4) | Tobin, Quill, Reed, Wren |
| Cerulean Cave (3) | Stanly, Foster, Larry |

These have resolving local references, but the class/place substitution does not give each person a distinct voice. The two forests' recently revised Scouts and Aroma Ladies are better differentiated: tracking, knots, leaf sketches, scent blending and conservation provide individual topics.

Recommended writing directions: individual harbour activities and observations; different forest study / care practices; cave strata, water marks and evidence of disturbance for the three Ruin Maniacs. Give each a distinct introduction, defeat, aftermath and rematch response.

## Priority 2: class and location leftovers

- **Route 9 Brent and Cora:** first-battle dialogue fits martial artists, but rematches retain Bug Catcher lines about raising Pokémon from cocoons and “my super BUG POKéMON”. The `Conner` labels now correctly point to Cora's script; the text content is what is stale. See `data/text/trainers.inc:160` and `:168`.
- **Rock Tunnel Stevie:** Black Belt F still explains Pokémon cosplay and Clefairy. Her Victory Road dialogue already has martial training / perseverance themes. A hobby is possible, but this reads as inherited Pokémaniac dialogue rather than a deliberate connection to her current identity.
- **Rock Tunnel Winston:** a Ruin Maniac describes drawing Pokémon at home and says “I'm an artist, not a fighter.” Could be made coherent through cave drawings / fossil sketches; currently it reads as another old Pokémaniac remnant.
- **Route 11 Chaplin:** class is Bug Maniac, but dialogue and both teams focus entirely on Mr. Mime. The dialogue supplies an Apex clue and should be preserved in substance. Consider a Pokémaniac identity or an explicit explanation of the bug research connection.
- **Route 2 Jonah:** still refers to crossing the forest “gate”, despite the natural entrance replacing the gatehouse. “Forest entrance” would be clearer.
- **Route 17 Nico:** says he is proving runners can keep up on Cycling Road; several other triathletes talk primarily about running/footing despite the cycling sprite work. Review intended activity per placement before rewriting all athletic references.
- **Route 8 Johan:** “PSYCHIC JOHAN knows SAFFRON's streets well” remains formulaic, even though the nearby-city subject is geographically reasonable.
- **Route 17 Nikolas:** calls the Power Plant abandoned. This can describe when he caught Voltorb, but the plant is now restored in the story; “before the POWER PLANT reopened” would remove ambiguity.

## Priority 3: repeated supporting lines and generic challenge voices

- Eight Rocket League Aces share `RocketLeague_Arena_Text_RocketAceIntro` and `RocketLeague_Arena_Text_RocketAceDefeat`. They have individual post-battle lines already; their entry/defeat lines should match those personalities. Admins have individual lines.
- Six Saffron Gym Psychics and two Celadon Gym Breeders share “You gave me something new to learn!”
- Four Experts (Hugo, Mara, Ivo and Victory Road Karina) repeat the same defeat and post-battle lesson.
- Several Trendsetters share “You have a style all your own!”; several doubles pairs share “You make a great team!”
- Mia, Ella and Ruby share the rematch invitation “Ready for another battle?”
- Some short losses recur (“Washed out!”, “Line snapped!”, “Outpaced!” etc.). Lower priority than full repeated dialogue sets.
- The central Indigo League challenge uses generic introduction/defeat strings across the selected leader / Elite Four rosters. Distinct opponent reactions would be needed for strict uniqueness there.

Shared two-Pokémon requirements, prize announcements, rumour-recorded confirmations and the Rocket siblings' deliberately repeated introduction can remain shared. Repeated dialogue for the same person's roster variants is also expected.

## Editorial assessment

Most route dialogue has already been adapted to local geography and trainer activities. The strongest remaining gap is repetition and template phrasing, rather than widespread pointers to another area's text. Some routes overuse the same rhythm (“Back ...?”, “Good. ...”, “Let's test ...”) even when strings are technically unique. A writing pass should give people different motivations and sentence rhythms, without making every line announce their class or route number.

The main story trainers generally follow the expedition, growth and trust themes. Ordinary trainer exceptions and challenge-facility generic lines are listed above. No claim is made here about live text wrapping, animation, runtime flags, or whether every conditional branch is reachable in a particular save.

## Reproduce

Run `tools/audit_trainer_dialogue.py` with Python and Pillow available (the existing heatmap audit imports Pillow). It generates:

- `inventory.json`: map roots, battle commands, text sources, duplicates, rematch identity mismatches and shared trainer IDs.
- `inventory.md`: readable dialogue by encounter site.

The editorial findings in this file are manually reviewed, not a keyword-based semantic guarantee. The next pass should fix identity / flag wiring first, rewrite the 23 template sets and class leftovers, then review the smaller repeated lines in context.
