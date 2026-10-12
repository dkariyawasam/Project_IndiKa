# Remaining regional sprite completion

Added Galarian Farfetch'd, Meowth, Corsola and Weezing, plus Perrserker and Cursola. Source: [pokeemerald-expansion](https://github.com/rh-hideout/pokeemerald-expansion/tree/803bcb7c58afe73b0831893f383e6ce07fe0bac2/graphics/pokemon), pinned at 803bcb7c58afe73b0831893f383e6ce07fe0bac2. Upstream credits retained in ../roserade-alolan-sprites/upstream-credits.md. Original imported assets archived in source/; these are community GBA adaptations.

Native 64x64 front/back sprites, separate normal/shiny palettes, two-frame menu icons mapped to existing FRLG icon palettes, and source footprints where available (otherwise corresponding standard species footprints). Coordinate metadata updated. Normal/shiny colours subsequently aligned to the project's Kanto colour pool using align_palettes.py; mapping.json records changes and source palettes. Geometry unchanged.

preview-1/2.png show imported normal front/back, shiny front/back and menu icons before the subtle colour alignment. comparison-1.png shows before/after normal and shiny fronts. All29 regional roster entries now have complete asset references. Active Dex checks and main ROM build passed. No live emulator testing performed. Remaining six battle placeholders are unrelated custom species.
