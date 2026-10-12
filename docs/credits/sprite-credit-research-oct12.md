# Sprite credit research — 12 October 2026

## Evidence and method

The original [Gen VII+ resource thread](https://www.pokecommunity.com/threads/ds-style-gen-vii-and-beyond-pok%C3%A9mon-sprite-repository-in-64x64.368703/) links individual sprite sheets. Its [artist spreadsheet](https://docs.google.com/spreadsheets/d/1o6-4IIFnyZr2axewnPNGSk3tUEC5bfcb/edit) was recovered through the [September 2024 resource post](https://whackahack.com/foro/threads/9-gen-sprites.67492/page-52). Column C credits battle sprites; column D credits icons. Row numbers below refer to its List tab.

Compared archived imports with downloaded public sheets: crop each 64×64 battle frame or 32×32 icon frame to its non-background bounds, then compare every color region after canonical color renumbering. This ignores palette changes and padding, but preserves exact geometry and shading regions. A match supports the credited drawing, not identical original colors. Front results refer to frame one; unmatched additional animation frames are retained in the JSON and are not attributed here. This pass does not certify every current local edit or all shipped graphics.

[Comparison records](sprite-resource-comparisons-oct12.json) retain URLs, source hashes, local hashes, frame coordinates and unmatched results.

## Confirmed archived drawings

| Species | Matched parts | Resource credits | List row |
| --- | --- | --- | --- |
| Galarian Corsola | back, icon frame 1 | Battle: leParagon; Icons: Percy | 507 |
| Cursola | back, icon frame 1, icon frame 2 | Battle: leParagon; Icons: Percy | 199 |
| Alolan Diglett | front, back | Battle: Chaos Rush, leParagon | 483 |
| Alolan Dugtrio | front, back | Battle: Chaos Rush, leParagon | 484 |
| Galarian Farfetch'd | back, front, icon frame 1, icon frame 2 | Battle: leParagon; Icons: Megax Rocker | 500 |
| Alolan Geodude | front, back | Battle: leParagon | 487 |
| Alolan Golem | front, back | Battle: leParagon | 489 |
| Alolan Graveler | front, back | Battle: Chaos Rush, leParagon | 488 |
| Alolan Grimer | front, back | Battle: leParagon | 490 |
| Alolan Marowak | back | Battle: leParagon | 493 |
| Alolan Meowth | back | Battle: leParagon | 485 |
| Galarian Meowth | back, front, icon frame 1, icon frame 2 | Battle: leParagon; Icons: Megax Rocker | 495 |
| Alolan Muk | front, back | Battle: leParagon | 491 |
| Alolan Ninetales | front, back | Battle: leParagon (front); Z-nogyroP (back) | 482 |
| Perrserker | back, front, icon frame 1, icon frame 2 | Battle: leParagon; Icons: Megax Rocker | 198 |
| Alolan Persian | back | Battle: leParagon, OtaKun, xiechayghe | 486 |
| Alolan Raichu | front, back | Battle: leParagon, MrDollSteak | 478 |
| Alolan Raticate | front, back | Battle: leParagon | 477 |
| Alolan Rattata | front, back | Battle: leParagon | 476 |
| Alolan Sandshrew | front, back | Battle: leParagon | 479 |
| Alolan Vulpix | front, back | Battle: Elazulmax, Versekr Dark, xiechayghe, goranthegreat, leParagon | 481 |
| Galarian Weezing | back, front, icon frame 1, icon frame 2 | Battle: French-Cyndaquil; Icons: Megax Rocker | 501 |

## Still unresolved

These files did not exactly match the retrieved revisions. Species-level resource credits remain leads, not confirmed credits for these files:

- Galarian Corsola: front, icon frame 2.
- Cursola: front.
- Alolan Exeggutor: front, back.
- Alolan Marowak: front.
- Alolan Meowth: front.
- Alolan Persian: front.
- Alolan Sandslash: front, back.

Alolan icons were not compared in this pass. Roserade, Sylveon, separately imported footprints and other icon sources still need individual matching.

The [Gen VI resource](https://www.pokecommunity.com/threads/gen-vi-ds-style-64x64-pokemon-sprite-resource.314422/) credits leParagon for Sylveon front, Superjub for back, and Soloo993 for quality-control work on Sylveon. These are newly recovered resource credits, but the linked image returned a regional-unavailability placeholder, so no exact local match was established. The linked download returned later-generation sheets rather than a Sylveon sheet. Do not treat that download as Sylveon evidence.

The [second-frame resource](https://whackahack.com/foro/threads/gen-vii-sprites-for-emerald-second-frames.68313/) separately attributes animation contributions. Those names are not automatically credits for the single frames used here.

No ROM credits were changed and no maintainers were contacted. Confirmed credits have been added to the project documentation and staged release credits; unmatched files remain explicitly open.
