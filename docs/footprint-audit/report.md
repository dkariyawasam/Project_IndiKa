# Active Pokémon footprint audit

All 399 active species checked. This report reflects the current assets; fixed-footprints.json records the 19 corrected footprints and before-fixes/ retains their prior images.

- 0 missing registrations/source files; all resolved images are 16×16.
- 356 match the pokeemerald-expansion reference footprints.
- 3 additional active-species differences are confirmed original FireRed artwork; retain them.
- 11 use purpose-made custom footprints; see custom/manifest.json and custom/preview.png.
- 0 use the question-mark placeholder.
- 1 additional non-placeholder images differ from the reference and need review/replacement.
- 92 images are blank; blankness alone is not an error for species without tracks.

## Placeholder entries

.

## Other mismatches

ANNIHILAPE.

## Scope and provenance

Reference PNGs come from https://github.com/rh-hideout/pokeemerald-expansion/tree/master/graphics/pokemon ; per-species download URLs/results are in reference-sources.json. Four older-species alternatives were additionally checked against https://github.com/pret/pokefirered/tree/master/graphics/pokemon and matched the project exactly. The expansion reference is community maintained; a match is not independent proof of official provenance for later-generation footprints.

Custom species/forms use deliberate contact-stamp designs generated with the built-in image generation tool, then converted to 16×16 monochrome assets by tools/import_custom_footprints.py. The source atlas is preserved in custom/source.png. These are new adaptations, not official footprints. Failed regional-form reference lookups do not mean that no canonical footprint exists.

all-species.csv lists every registration and result. contact-sheet-*.png covers every active species; differences.png shows the initial 26 reference differences (including the four subsequently verified FireRed originals). These are static asset checks, not an emulator playtest.
