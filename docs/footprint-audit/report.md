# Active Pokémon footprint audit

All 443 active species checked. This report reflects the current assets; fixed-footprints.json records the 19 corrected footprints and before-fixes/ retains their prior images.

- 0 missing registrations/source files; all resolved images are 16×16.
- 398 match the pokeemerald-expansion reference footprints.
- 4 additional differences are confirmed original FireRed artwork (Shroomish, Golem, Mawile, Ludicolo); retain them.
- 38 use the question-mark placeholder.
- 0 additional non-placeholder images differ from the reference and need review/replacement.
- 98 images are blank; blankness alone is not an error for species without tracks.

## Placeholder entries

ANNIHILAPE, MIME_SR, OSSCYTHE, CINNABAR_FEEBAS, KABUSTAR, KABUKNIGHT, AMUNYTE, KINKABUTO, AEROPTERYX, FARFETCHD_GALARIAN, RATTATA_ALOLAN, RATICATE_ALOLAN, PICHU_ALOLAN, PIKACHU_ALOLAN, RAICHU_ALOLAN, SANDSHREW_ALOLAN, SANDSLASH_ALOLAN, DIGLETT_ALOLAN, DUGTRIO_ALOLAN, MEOWTH_ALOLAN, PERSIAN_ALOLAN, MEOWTH_GALARIAN, PERRSERKER, GEODUDE_ALOLAN, GRAVELER_ALOLAN, GOLEM_ALOLAN, GRIMER_ALOLAN, MUK_ALOLAN, VULPIX_ALOLAN, NINETALES_ALOLAN, EXEGGCUTE_ALOLAN, EXEGGUTOR_ALOLAN, CUBONE_ALOLAN, MAROWAK_ALOLAN, CORSOLA_GALARIAN, CURSOLA, KOFFING_GALARIAN, WEEZING_GALARIAN.

## Other mismatches

.

## Scope and provenance

Reference PNGs come from https://github.com/rh-hideout/pokeemerald-expansion/tree/master/graphics/pokemon ; per-species download URLs/results are in reference-sources.json. Four older-species alternatives were additionally checked against https://github.com/pret/pokefirered/tree/master/graphics/pokemon and matched the project exactly. The expansion reference is community maintained; a match is not independent proof of official provenance for later-generation footprints.

Custom species/forms do not necessarily have an official footprint to compare against. Their placeholder graphics need deliberate design choices. Failed regional-form reference lookups do not mean that no canonical footprint exists. No speculative custom prints were drawn.

all-species.csv lists every registration and result. contact-sheet-*.png covers every active species; differences.png shows the initial 26 reference differences (including the four subsequently verified FireRed originals). These are static asset checks, not an emulator playtest.
