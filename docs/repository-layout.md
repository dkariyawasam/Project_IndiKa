# Repository layout

- `src/`, `include/`, `asm/`, `data/`, `constants/`: game code, scripts and data.
- `graphics/`, `sound/`: game assets and generated intermediates.
- `tools/`: build tools, audits and generators. The local compiler and map editor installations are retained.
- `docs/`: development notes, audits, playthrough evidence and sprite sources/backups.
- `docs/reference-previews/`: historical previews formerly loose in the root.
- `docs/sprite-work/`: recent fossil sprite sources, revisions and comparison sheets.
- `build/`: ignored incremental build output; retained to avoid unnecessary rebuilding.
- `output/imagegen/`: saved generation prompts.

## Builds and user files

`pokefirered.gba` is the main ROM. `pokefirered-player-customisation.gba` is the deliberately preserved customisation version. Keep their saves and emulator states. `data/mb_colosseum.gba` is a build input, not a temporary ROM.

`kink.png.png` is the user's current sprite upload (currently Amunyte); its historical filename is retained for ongoing imports.

## Cleanup — 24 September 2026

Moved six root preview images into `reference-previews/`, removed macOS metadata and Python bytecode caches, removed the empty `Animation/` directory, and added Python-cache/emulator-state ignore rules. No game source, sprite revisions, saves or ROMs were removed. Historical test logs remain with their reports.
