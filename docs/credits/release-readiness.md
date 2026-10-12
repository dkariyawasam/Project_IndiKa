# Repository and release preparation — 12 October 2026

## Completed locally

- Removed two full game ROMs and one emulator state from the Git index, preserving local copies.
- Added ignore rules and an index hygiene check for ROMs and private emulator files.
- Updated README navigation, local screenshots, preview status, installation and rights notes.
- Replaced upstream vanilla-comparison/symbol-push CI with a read-only modified FireRed build and hygiene/trainer checks. No ROM artifact upload or release publishing step.
- Added a credit-correction issue template, keeping the open attribution invitation actionable.
- Added a BPS package script: verified base hash, reviewed target hash, real Floating IPS creation/application, SHA-256 round trip, documentation allowlist, checksums and ZIP.

## Verification

The current local ROM build, index hygiene and diff whitespace checks pass. The package process passed a synthetic binary round trip using Floating IPS at commit `ff216a75df0987047a67d7923567dc4482ce07ac`, including rejection of a wrong base and existing output. This is tool verification, not generation of the actual game patch. GitHub-hosted CI has not run for these changes.

## Pending

The actual BPS requires the user's clean English FireRed revision 0 base, SHA-1 `41cb23d8dccc8ebd7c649cd8fbb58eeace6e2fdc`. No installable game patch or release ZIP has been produced yet.

Rights are not certified. Attribution notices are not permissions. Original game-derived assets and the embedded multiboot dependency remain in source. Earlier commits still contain full ROMs and the state; removing their current tracking is not history cleanup. No Git history rewrite, commit, push, visibility change or release publication was performed. See [RIGHTS.md](../../RIGHTS.md).

Playthrough and line-of-sight reviews remain accepted. Sprite polish and unresolved attribution remain documented preview limitations.
