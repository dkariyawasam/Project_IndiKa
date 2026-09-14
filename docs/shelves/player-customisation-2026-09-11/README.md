# Shelved player customisation

Shelved on 11 September 2026 at the user's request. The active game again uses the original boy/girl intro selection and original player colours.

`restore.patch` restores only the customisation feature: intro UI, saved hair/accent fields, palette implementation, rendering hooks, its regression test and feature documentation. It is based on the working tree after shelving, with all unrelated repairs retained. From the repository root:

```sh
git apply --check docs/shelves/player-customisation-2026-09-11/restore.patch
git apply docs/shelves/player-customisation-2026-09-11/restore.patch
make -j4
```

The patch passed the applicability check when created. Future edits may require resolving overlapping hunks.

`snapshot.tar.gz` is an additional backup of the original feature files, including its three preview PNGs. It includes complete copies of shared source files for reference; prefer the patch when restoring, because replacing those whole files could overwrite later unrelated work. The previews can be extracted separately to their original `docs/player-customisation-*.png` paths.

Kept active: all trainer-class/map work, 45 NPC sprite-index repairs, the 14 player sprite-index repairs (needed for correct default palettes too), NPC palette cleanup and bounded lookup, Petrel's palette-slot fix, and the blue trainer-card hint strip. The graphical-playthrough report and its screenshots remain as historical evidence. The Route 7 dynamic-palette capacity issue remains open.

No player save files were edited. The saved gender/style still selects the original boy or girl graphics. The customisation fields occupied existing padding, so shelving restores that padding without shifting the save layout; custom colour values are simply ignored.

Validation: ROM rebuilt; palette lifetime test and player sprite-index test passed; sprite/map palette audit passed its static checks; no player-appearance symbols remain in the built ELF. `preserved-files.json` records SHA-256 hashes of other individually listed modified/untracked files taken before shelving and checked unchanged afterward (the rebuilt ROM is excluded).
