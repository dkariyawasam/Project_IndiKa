# Regional palette audit — 24 September 2026

Reviewed 29 entries: implemented Alolan/Galarian forms, their regional pre-evolutions, Perrserker and Cursola. Four sheets show normal front, normal back, shiny front, shiny back in that order. Sheets use the actual palette-table mappings.

## Findings

- All front/back embedded palettes match their assigned normal palette at occupied indices.
- All 29 menu-icon palette indices select one of the three shared icon palettes.
- Visually, the palettes broadly fit the project's FRLG art direction. No recolouring is warranted from this pass. Alolan Muk's multicoloured bands and Marowak's vivid flames are intentional design features.
- Normal palettes have isolated shades absent from the current Kanto reference pool in Alolan Rattata, Alolan Graveler, Alolan Marowak and Perrserker. This is a diagnostic, not a style failure.
- Several shinies have colours outside the normal reference pool, including standard-looking regional pre-evolutions that share their base species' palettes. These should not be forcibly remapped merely to satisfy exact matching.

## Method and limits

Compared occupied colour indices at GBA 5-bit precision against normal palettes for the project's first 151 dex entries; these include previous custom edits and are not an untouched retail FRLG reference. Visually inspected all four front/back comparison sheets. Style is a visual judgement, not established by palette membership alone. This is an asset audit, not a live battle/emulator test; sprite poses, geometry and shiny design authenticity are outside this palette check. No game assets or ROM were changed.

Detailed mappings and exceptions: `audit.json`.

## Follow-up correction
The initial visual pass missed colour-role conversion errors in regional Cubone and Koffing despite valid shared palettes. These were corrected in `../regional-colour-role-fix/`; the original audit sheets record the pre-fix state. Palette membership alone did not catch incorrect pixel-to-colour assignments.
