# Pokédex size-chart audit

Audited 410 non-retired species in the species-to-National-Dex mapping; excluded legacy OLD_UNOWN placeholders; 65 explicitly zeroed species are absent from the mapped roster. No game data changed.

- 0 missing/blank/invalid entry or front-asset findings.
- 5 species use the identical default calibration (256, 0, 256, -2); 5 have actual species sprites.
- 0 mapped entries use question-mark placeholder sprites. Mapping coverage does not mean all these species are obtainable in-game.
- 6 species have a calculated silhouette crossing the size panel interior (x16–96, y80–128), for at least one player. These are layout review flags, not necessarily visible corruption.

The chart uses stored inverse affine scale/offset values. Active entries are calibrated from their listed heights and visible front-sprite bounds against a 1.5 m trainer by tools/sync_pokedex_size_chart.py. Run that tool after changing heights or sprites; its --check mode detects stale calibration. Both RED and LEAF were checked. CASTFORM uses its normal form. UNOWN uses A as the audit representative; the game can show the saved personality form, and SPINDA spots depend on the save. Contact sheets approximate current affine silhouettes with RED in dark gray and LEAF in gray at right; they are static reconstructions, not emulator captures.

## Active Pokédex coverage

All 404 active entries are covered, in the game's Pokédex order, from the explicit active roster (including implemented species beyond MEW). Zero missing active entries. See active-pokedex.csv and active-contact-sheet-*.png.

0 active entries have default calibration; 0 active entries use question-mark sprites. Default values are review candidates, not automatic failures: some original species legitimately use these values.

Active placeholder entries: .

## Default calibration requiring review

PICHU_ALOLAN, PIKACHU_ALOLAN, EXEGGCUTE_ALOLAN, CUBONE_ALOLAN, KOFFING_GALARIAN.

## Placeholder sprites

.

## Interpretation

Pixel-height ratios are diagnostics, not proof of biological height: tails, wings, curled bodies, floating poses and species measured by length require individual judgment. The current active-entry calibration treats the whole silhouette as the listed height; curled and length-measured species remain pose approximations. This audit does not verify canonical height lore against external sources and is not a live emulator sign-off.

See all-species.csv for every species, both player ratios, baseline differences and layout flags. See contact-sheet-*.png for visual review.
