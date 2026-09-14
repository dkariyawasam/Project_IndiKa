# Kept ROM versions

- `pokefirered.gba`: main game, with the original boy/girl selection and original player colours.
- `pokefirered-player-customisation.gba`: retained alternate build with style selection, hair colour and clothing accent customisation restored.

The alternate ROM was built on 11 September 2026 from the current working tree plus `docs/shelves/player-customisation-2026-09-11/restore.patch`. It includes the latest Ace Trainer F overworld sheet and shared Ace palette wiring, along with the other retained fixes. It is a snapshot; rebuilding the main ROM does not automatically update it.

The alternate was built in an isolated copy. Main source files and the main ROM were verified unchanged. Appearance tests (245 combinations), palette lifetime tests, player tile checks and the static overworld palette audit passed. The alternate ELF contained the appearance functions and both Ace palette assignments were verified in its ROM. This build was not separately replayed in the emulator.

Alternate ROM SHA-256: `880e9e2dc73c16084b10dfc3cb13bfa1b3535b637277e02a576fb8aee0194d5b`.
