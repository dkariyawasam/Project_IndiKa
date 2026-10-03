# Feature and design guide

Updated October 3, 2026. This is the editorial index for the local devlog drafts, not a release-completion claim. The roster counts and implementation status below describe this development snapshot.

## Design principles

| Principle | How it appears in the game | Read more |
| --- | --- | --- |
| Make exploration part of research | Open travel, witnesses, field trials and a Logbook that preserves leads | [The pitch](01-why-rebuild-kanto.md), [routes](02-breaking-the-vanilla-route.md), [rumours](04-rumour-system.md) |
| Connect mechanics to the central theme | Bond, Instinct and Design give different explanations of growth | [Gym leaders](05-gym-leaders-as-researchers.md), [Apex](03-apex-pokemon.md), [Giovanni](06-giovanni-and-mewtwo.md) |
| Reward observation at manageable scales | Page rewards, habitat rewards and obtainable-roster completion | [Every catch matters](16-making-every-catch-matter.md) |
| Let the world explain its systems | Laboratory inventions, coastal trades, natural forest entrances and an unsigned blast cave | [Evolution access](18-evolution-without-a-second-console.md), [Route 7](07-route-7-flashback-sequence.md) |
| Make controls predictable | Shared headers, meaningful direction dots, consistent back behaviour and contextual claims | [Shared UX](10-making-menus-feel-like-one-game.md) |
| Use visual relationships as information | Shared family palettes, fossil body/shell colours and distinct map markers | [Art](13-pokemon-art-and-palette-pass.md), [fossils](17-fossils-as-incomplete-reconstructions.md) |
| Show other people travelling through the same world | The rival's evolving team, specialist trainers and two league formats | [Parallel journeys](19-a-rival-who-travels-the-same-region.md) |
| Give the research a lasting conclusion | Oak's reactions, Field Aide milestones and the bedroom certificate | [The ending](21-bringing-the-expedition-home.md) |

## Current feature coverage

### Exploration and geography

Open Kanto travel; early ferry access; purchased and rented bikes; rental-route continuity checks; removed Saffron tea blockers; direct natural Viridian/Fuchsia Forest entrances; revised Route 25 approaches; Vermilion Harbour/coastline work; Viridian Channel; merged Route 21 and adjusted Route 18 connections. The regional map distinguishes water routes, blue dungeon markers and grey minor landmarks, with approach-aware labels and white place names. The map is available from the outset, so Daisy's duplicate gift has been removed.

Primary posts: [2](02-breaking-the-vanilla-route.md), [7](07-route-7-flashback-sequence.md), [20](20-seeing-and-testing-the-whole-region.md).

### Story and encounters

Oak's fieldwork framing; Gym Leader field trials and scalable challenge variants; eight Apex investigations with three rumours each; the witness dossier; Giovanni's Mansion, Silph, aftermath and Mewtwo progression; four Elite Four flashbacks; the rival's parallel evolution journey; distinct Indigo and Rocket competitions; Oak's research reactions and certificate ending.

Primary posts: [1](01-why-rebuild-kanto.md), [Apex](03-apex-pokemon.md), [rumours](04-rumour-system.md), [gyms](05-gym-leaders-as-researchers.md), [Giovanni](06-giovanni-and-mewtwo.md), [flashbacks](07-route-7-flashback-sequence.md), [19](19-a-rival-who-travels-the-same-region.md), [21](21-bringing-the-expedition-home.md).

### Pokémon and acquisition

399 active species in this snapshot; a separate 318-species obtainable completion target; regional and custom forms; rare surviving primeval Pokémon; four fossil-fragment combinations; NPC trade evolutions; held-item evolution trades placed around appropriate characters; the Porygon quiz terminal; Porygon, Dratini and Smeargle at the Rocket prize counter; species ability and stat revisions; expanded menu icons, cries and complete active-roster footprints.

Primary posts: [13](13-pokemon-art-and-palette-pass.md), [collection](16-making-every-catch-matter.md), [fossils](17-fossils-as-incomplete-reconstructions.md), [evolution access](18-evolution-without-a-second-console.md). Active does not mean obtainable, and complete footprint coverage does not mean complete battle artwork.

### Rewards and inventory

Page difficulty determines S/M/L Exp. Candy, with a Rare Candy alongside it. Friendship pages use L; Apex pages give two L. Habitat completion gives one XL. Claims are saved and checked against inventory space. Loose Rare Candy pickups have been replaced. The five-pocket Bag separates Items, Berries, Poké Balls, TMs and Key Items. Berry behaviour, fossil quantities and item icons have received dedicated passes. Rocket item/TM and Pokémon clerks use coin shops; the Coin Case requirement is removed. Badge-linked field items include the Thunder and Soul passes, with custom card-like icons.

Primary posts: [9](09-rebuilding-the-bag.md), [16](16-making-every-catch-matter.md), [18](18-evolution-without-a-second-console.md).

### Interface and personalisation

Radial tool menu; Style 1/Style 2 selection; two independent clothing accents; secondary accent linked to blue, rose or green frames; matching menu and dialogue colours; shared header dimensions and background edge; direction-aware PICK hints; two-column habitat index; bounded page navigation; A toggles Dex description/area; B exits either; revised party navigation; removal of redundant cancel rows and map exit controls; reward header pulse and retracting footer; persistent completion balls; Field Aide card naming and three achievement stars.

Primary posts: [8](08-new-start-menu.md), [10](10-making-menus-feel-like-one-game.md), [16](16-making-every-catch-matter.md), [21](21-bringing-the-expedition-home.md).

### Presentation and development

Tangrowth title art, sparkles, idle colour transitions and Start cry; thin mod-attribution strip; original splash credits retained with a separate 2026 project line; project credits and attribution records; trainer overworld identity and dialogue passes; palette and footprint work; removal of unused maps and obsolete systems while retaining Colosseum maps; removal of the debug Gengar grant; emulator-linked dashboard, connected tile view, trainer-density inspection, audits and recorded playthrough evidence.

Primary posts: [trainers](11-trainer-identity.md), [cleanup](12-cleaning-up-firered.md), [art](13-pokemon-art-and-palette-pass.md), [title](14-start-screen-makeover.md), [lessons](15-what-i-learned-hacking-firered.md), [20](20-seeing-and-testing-the-whole-region.md).

## Implemented, unfinished and deferred

| Status | Scope |
| --- | --- |
| Implemented with recorded checks | Core progression, Oak's ending, page/habitat grants, NPC trades, terminal branches, coin shops and the main menu systems. Evidence varies by feature; use the QA records below. |
| Art still unfinished | Deep Feebas assets and custom backs for Kabustar, Kabuknight and Aeropteryx. |
| Further validation required | Naturally earned collection pacing, representative ordinary-team balance and the deferred live custom-ability battle pass. The reported Pallet boundary issue has additional fixes and clean test crossings, but its exact reported failure was not reproduced in the fixture. |
| Intentionally unavailable | Patch Disc distribution and Porygon3 acquisition. Additional ferry destinations remain on hold. |
| Release work still open | Remaining imported-art attribution and final patch/package production after final QA. |

## Editorial evidence

- [October 3 release/playthrough record](../release-playthrough/october3/README.md): earned expedition ending, controlled rewards, credits and remaining work.
- [October 2 coverage](../feature-validation-october2/coverage.json): trades, terminal cases, shops and team inspection.
- [Active roster](../active-pokedex-roster.md): display eligibility and reserved/excluded entries.
- [Pokédex reward code](../../src/pokedex_screen.c): current grants, claim controls and presentation.
- [Fossil reconstruction script](../../data/maps/CinnabarIsland_PokemonLab_ExperimentRoom/scripts.inc): fragment recipes.
- [Field Aide card code](../../src/trainer_card.c): achievement-star criteria.
- [Dashboard guide](../../tools/debug_dashboard/README.md): available inspection features and limits.
- [Sprite audit](../active-pokemon-sprite-audit/report.md): remaining asset work.
- [Credits](../credits/README.md) and [attribution sources](../credits/attribution-sources.md): contribution records.
- [Image manifest](images/manifest.json): source, kind and hash for the illustrations.

These are drafts. Archived images are labelled as earlier builds, and controlled completion fixtures are not presented as naturally earned progress. New concepts in the fossil article describe this mod's fiction rather than official canon.
