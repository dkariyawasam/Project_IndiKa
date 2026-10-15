# Pokédex size-chart audit

Audited 400 non-retired species in the species-to-National-Dex mapping; excluded legacy OLD_UNOWN placeholders; 65 explicitly zeroed species are absent from the mapped roster. No game data changed.

- 0 missing/blank/invalid entry or front-asset findings.
- 39 species use the identical default calibration (256, 0, 256, -2); 39 have actual species sprites.
- 0 mapped entries use question-mark placeholder sprites. Mapping coverage does not mean all these species are obtainable in-game.
- 361 species have a calculated silhouette crossing the size panel interior (x16–96, y80–128), for at least one player. These are layout review flags, not necessarily visible corruption.

The chart uses hand-authored inverse affine scale/offset values, not the height field. Updating a height or front sprite does not recalibrate the chart. Both RED and LEAF were checked. CASTFORM uses its normal form. UNOWN uses A as the audit representative; the game can show the saved personality form, and SPINDA spots depend on the save. Contact sheets approximate current affine silhouettes with RED in dark gray and LEAF in gray at right; they are static reconstructions, not emulator captures.

## Active Pokédex coverage

All 399 active entries are covered, in the game's Pokédex order, from the explicit active roster (including implemented species beyond MEW). Zero missing active entries. See active-pokedex.csv and active-contact-sheet-*.png.

39 active entries have default calibration; 0 active entries use question-mark sprites. Default values are review candidates, not automatic failures: some original species legitimately use these values.

Active placeholder entries: .

## Default calibration requiring review

TENTACOOL, GASTLY, ELECTRODE, OMATO, OMATOPS, ROSERADE, CINNABAR_FEEBAS, KABUSTAR, KABUKNIGHT, AEROPTERYX, FARFETCHD_GALARIAN, RATTATA_ALOLAN, RATICATE_ALOLAN, PICHU_ALOLAN, PIKACHU_ALOLAN, RAICHU_ALOLAN, SANDSHREW_ALOLAN, SANDSLASH_ALOLAN, DIGLETT_ALOLAN, DUGTRIO_ALOLAN, MEOWTH_ALOLAN, PERSIAN_ALOLAN, MEOWTH_GALARIAN, PERRSERKER, GEODUDE_ALOLAN, GRAVELER_ALOLAN, GOLEM_ALOLAN, GRIMER_ALOLAN, MUK_ALOLAN, VULPIX_ALOLAN, NINETALES_ALOLAN, EXEGGCUTE_ALOLAN, EXEGGUTOR_ALOLAN, CUBONE_ALOLAN, MAROWAK_ALOLAN, CORSOLA_GALARIAN, CURSOLA, KOFFING_GALARIAN, WEEZING_GALARIAN.

## Placeholder sprites

.

## Interpretation

Pixel-height ratios are diagnostics, not proof of biological height: tails, wings, curled bodies, floating poses and species measured by length require individual judgment. Existing vanilla calibrations should not be blindly replaced with bounding-box normalization. This audit does not verify canonical height lore against external sources and is not a live emulator sign-off.

See all-species.csv for every species, both player ratios, baseline differences and layout flags. See contact-sheet-*.png for visual review.
