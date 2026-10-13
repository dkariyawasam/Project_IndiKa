# Repository layout

- `src/`, `include/`, `asm/`, `data/`, `constants/`: game code, scripts and data.
- `graphics/`, `sound/`: game assets and generated intermediates.
- `tools/`: build tools, audits and generators. The local compiler and map editor installations are retained.
- `docs/`: development notes, audits, playthrough evidence and sprite sources/backups.
- `docs/reference-previews/`: historical previews formerly loose in the root.
- `docs/sprite-work/`: recent fossil sprite sources, revisions and comparison sheets.
- `build/`: ignored incremental build output; retained to avoid unnecessary rebuilding.
- `output/`: retained artwork drafts, palette backups, generated source images and historical UI captures; see [the archive index](../output/README.md). These are not release artifacts.
- `release/2026-10-12-preview/`: documentation and target manifest for the pending preview patch.
- `dist/`: ignored destination for locally generated patch packages.

## Builds and user files

`pokefirered.gba` is the main ROM, including the integrated [player customisation](player-accent-customisation.md). The obsolete standalone customisation build and shelved restoration files are retired. Keep the main game save and emulator states. `data/mb_colosseum.gba` is a build input, not a temporary ROM.

## Cleanup — 24 September 2026

Moved six root preview images into `reference-previews/`, removed macOS metadata and Python bytecode caches, removed the empty `Animation/` directory, and added Python-cache/emulator-state ignore rules. No game source, sprite revisions, saves or ROMs were removed. Historical test logs remain with their reports.

## Cleanup — 13 October 2026

Removed 1119 files (12.82 MiB): unreferenced intermediate test captures, exact duplicate captures and five early Kinkabuto generated concepts superseded by the user's native sprite. Existing explicit filename references were checked before capture removal. Reports, logs, harnesses, referenced screenshots, selected first/middle/last captures per test folder, original artwork and attribution sources remain.

The [removal manifest](repository-cleanup-oct13.json) records each path, hash, reason and recovery commit. Historical test reports describe the captures available at the time; full image sequences can be recovered from that commit. Game assets and Git history were not changed.
