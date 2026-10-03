# October 3 release QA

Battle-sprite work remains last, as requested. This pass covers collection bookkeeping, progression, the expedition ending, collision/event checks and documentation. It does not certify a complete fresh-save balance playthrough or replace the deliberately deferred live custom-ability battle tests.

## Fixes

- Oak's finale no longer requires `FLAG_DEFEATED_CHAMP`, which the Hall of Fame clears. It requires the persistent `FLAG_SYS_GAME_CLEAR` and all existing expedition prerequisites. A regression test covers each prerequisite and eligibility after the transient Champion flag is cleared.
- The roster generator explicitly excludes unreleased Porygon3, so regeneration cannot accidentally add it to the active Dex. There are 399 active species and 318 currently obtainable species required by the Field Aide completion star.
- Route 2's Viridian Channel sign moved from an out-of-bounds coordinate onto an accessible sign tile. Fuchsia Forest's Tobin moved off a tree. Route 24's Becky now uses the bridge's elevation.
- The collision auditor reads the engine's actual map buffer limit rather than its obsolete hardcoded limit.
- The page-reward host harness now follows the two-column habitat UI and tests directional header boundaries as well as existing grant, rollback and saved-claim behavior.

## Earned playthrough

Continued the separate campaign save using normal movement and dialogue. The user's primary save was not changed. No progress flags, catches or items were injected into this campaign.

1. Confirmed that the bedroom certificate was absent and its wall interaction did nothing before the award.
2. Received Oak's individual Bond, Instinct and Design reactions, then his combined Nature of Evolution reaction.
3. Reproduced the blocked finale despite completed persistent prerequisites. The recorded [gate values](oak-gates-before-fix.txt) explain the failure.
4. Rebuilt with the fix, loaded the same earned save, and completed Oak's finale and certificate award.
5. Returned home, confirmed the wall certificate appeared, and interacted with it to reopen the certificate. A closes it.
6. Saved normally in the bedroom after reviewing the certificate.

Screenshots 01–10 document this sequence. The preserved earned checkpoint is `/tmp/kanto-release-oct3/expedition-certificate-earned-checkpoint.sav`; its matching build is `/tmp/kanto-release-oct3/fixed.gba`. These local test files are not release assets.

The campaign still has 27 owned species. Mountain is 2/15, with Starly, Staravia, Staraptor, Machop, Machoke, Machamp, Igglybuff, Jigglypuff, Wigglytuff, Geodude, Graveler, Golem and Primeape still missing. No natural habitat completion is claimed by this report.

## Controlled reward test

A separate `fixture.gba`/`fixture.sav` copy used the existing `CompletePokedex` special to prepare a full Dex. Normal menu inputs then claimed a habitat reward. Screenshots 11–13 and [runtime counts](habitat-runtime.json) show the claim header, the XL Candy receipt, the completed Poké Ball and the retracted footer. XL inventory rose from zero to one; pressing Start again did not award a second candy. This is runtime feature verification, not earned collection progress.

## Credits playback

In the disposable fixture, invoked the existing `DoCredits` special. SELECT selected the original script (verified its pointer and first attribution page); then restored the choice-screen fixture and pressed A. All fifteen project pages played naturally through the closing screens and reset. The [contact sheet](credits/contact-sheet.png), [per-page frame log](credits/runtime.txt) and screenshot 14 record the result. Text fits without truncation. This tests credits playback, not an additional earned League victory; the full original 42-page sequence was not replayed.

## Automated checks

All sixteen groups in [tests.json](tests.json) pass: candy use, coin shops, region map, Route 21 tilesets, player accents, radial palettes, object palette lifetime/recovery, evolutions, cries, Dex rewards, active roster, Field Aide stars, navigation signs, player sprite palette packing and Oak's ending prerequisites.

The rebuilt-map audit covers 246 maps and reports zero hard event findings or invalid fixed warp targets. Its [remaining heuristic candidates](map-collisions-fixed.log) include surf/land transitions, decorative water and connections entered through warps; these are not proof of collision bugs or proof that every tile has been walked. Navigation validation reports no world-placement overlaps or connection conflicts.

Existing [October 2 feature evidence](../../feature-validation-october2/) covers all sixteen NPC trades, six Porygon terminal cases, Rocket League shop behavior and decoded rival/admin rosters. Those checks were not repeated without a new reason.

## Remaining work

- Finish the naturally earned collection/reward route and a representative ordinary-party balance pass. The late campaign's high-level team cannot establish full progression balance.
- Live custom-ability battle testing remains deferred at the user's request.
- Battle artwork remains last: Deep Feebas' front/back, icon and palettes; Kabustar, Kabuknight and Aeropteryx backs. The [asset audit](../../active-pokemon-sprite-audit/report.md) now has complete footprint coverage for all 399 active species.
- Resolve remaining imported-sprite artist attribution where source records support it; do not substitute importer names for artists.
- Produce the final patch/release package after the artwork and final QA. A verified clean retail base is required; no patch was generated against a modified ROM.
- Patch Disc distribution/Porygon3 and additional ferry destinations remain intentionally held back.
