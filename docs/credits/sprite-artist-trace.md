# Imported sprite attribution trace — 24 September 2026

## Scope and evidence

All 80 archived PNGs in the Sylveon, Roserade/Alolan and Galarian import source folders match Git blob hashes in the upstream tree at `803bcb7c58afe73b0831893f383e6ce07fe0bac2`. This establishes the precise upstream source, not the original artist. Some archived files are no longer shipped (for example Sylveon's overworld), and current palettes may differ through local edits.

See `sprite-source-matches.json` for per-file hashes and paths. `sprite-history-evidence.json` records 33 path-history queries covering Sylveon, Roserade, three Alolan examples, the four imported Galarian forms, Perrserker and Cursola. History queries do not follow renames automatically, so this is not a complete historical audit of every file.

## Confirmed contribution roles

| Contributor | Evidence-backed contribution | Limit |
|---|---|---|
| Cafeei / Cafe | Cursola shiny-front correction in [PR 5126](https://github.com/rh-hideout/pokeemerald-expansion/pull/5126) | Correction contributor, not proof of original artwork authorship |
| DizzyEggg | [Gen VI/VII sprite import](https://github.com/rh-hideout/pokeemerald-expansion/commit/431690d1c28f), [Gen IV import](https://github.com/rh-hideout/pokeemerald-expansion/commit/de4722708b99), [icon import](https://github.com/rh-hideout/pokeemerald-expansion/commit/7c6077ef8b98) | Importer, not automatically the artist |
| Blackforest92 | [Gen VI animation integration](https://github.com/rh-hideout/pokeemerald-expansion/commit/86d4658bd375), [Gen VIII sprites/icons integration](https://github.com/rh-hideout/pokeemerald-expansion/commit/badf8945d865) | Integration role; does not establish who drew each frame |
| ultima-soul | [Gen VIII data/assets import](https://github.com/rh-hideout/pokeemerald-expansion/commit/c675c92a1d81) | Importer, not automatically the artist |

## Original-artist attribution still unresolved

- Sylveon front/back and icon.
- Roserade's imported GBA front/back adaptation and icon.
- All 18 Alolan forms' front/back adaptations and icons.
- Galarian Farfetch'd, Meowth, Corsola and Weezing, plus Perrserker and Cursola: original front/back and icon artists. Cursola has the confirmed correction credit above.
- The separately imported icons/footprints need a further per-asset pass; the 80-file match does not cover those other import folders.

Do not relabel the confirmed importers as original sprite artists. Official HGSS/BW battle sprites imported directly via PokeAPI remain credited as original Pokémon game artwork, with PokeAPI as archive.

## Leads, not confirmed attributions

The upstream resource list points to the [Gen VI 64×64 resource](https://www.pokecommunity.com/threads/gen-vi-ds-style-64x64-pokemon-sprite-resource.314422/) and [Gen VII and beyond resource](https://www.pokecommunity.com/threads/ds-style-gen-vii-and-beyond-pok%C3%A9mon-sprite-repository-in-64x64.368703/). Direct retrieval of the old resource URLs returned HTTP 403 during this pass. Indexed posts expose credit lists, but those lists are not enough to identify our particular sprite revision.

A [second-frame resource](https://whackahack.com/foro/threads/gen-vii-sprites-for-emerald-second-frames.68313/) names leParagon for a Cursola front. This is a lead requiring comparison with the archived imported art, not a confirmed match. Likewise the [DPPt backsprite resource](https://www.pokecommunity.com/threads/the-dppt-style-gen-vi-and-beyond-backsprite-resource.448065/) names 44tim44 for a Sylveon back, but that is a different resource and cannot be assumed to be our sprite.

## Credit-screen decision

No speculative artist names added to the ROM. Retain RHH/expansion contributor credit. Confirmed integration/correction names can be listed under upstream sprite adaptation rather than original art. Original artist mapping remains open; finishing it requires matching the archived artwork to the historical resource sheets/author posts, or clarification from their maintainers. No messages have been sent to maintainers.
