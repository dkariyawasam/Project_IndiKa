# Overworld item-action gestures

Implemented September 14, 2026. Main/test ROM SHA-256: `6b542237066e76f82fa81727651ec6c4fd4f2477e0737396ac8927484916c9da`.

The existing 24-frame player field-action pose now precedes:

- Cascade Board boarding, from the Bag or registered item path, after water-use validation.
- Marsh Finder scanning, from the Bag or registered item path.
- Rainbow Key access to Celadon's berry patch.
- Boulder Key access to Cinnabar's laboratory.
- Card Key unlocking a still-locked Silph door.
- Soul Pass friendship checks, after selecting that service at a Pokémon Center.

Denied key access, invalid Board use, already-open Silph doors and cancelled nurse
services do not animate. Ordinary healing does not require a Soul Pass gesture.
Poké Flute is excluded; its existing party/battle effects were not changed.
No active Snorlax objects/scripts were found on Route 12 or Route 16. Fishing and
VS Seeker retain their dedicated animations.

## Implementation

A shared asynchronous task waits for existing player movement, plays the field
pose, then restores the original graphics, facing, prevent-step state and input
lock. It preserves the caller's entire item-task data and script variables.
Script callers wait for completion; item callers resume their existing sequence.
There is no Pokémon cut-in or extra confirmation dialog.

## Verification

- Production state-machine fixture passes both sprite sets, walking/bike/surf
  states, pending movement, original-task preservation and one-time continuations.
- Live disposable-ROM checks: male/female Board gestures followed by successful
  boarding; both key entrances; a reset Silph door unlocking; nurse cancellation
  without a gesture and friendship selection with one; female Finder scanning;
  invalid Board/missing Card Key refusals without animation.
- Finder used while riding on water restored the female riding sprite and Surf
  state; subsequent movement passed. Pose tasks completed in 26 engine frames
  including movement-action bookkeeping.
- Build, dialogue widths and Seafoam layout regression pass.

Live tests used a disposable save and diagnostic warps, a temporary gender change,
and reset door flags. The final ROM matches the live-tested build. Main save and
preserved player-customisation ROM hashes are unchanged.

Evidence: [runtime checks](runtime-tests.tsv), [state test](test_state.py),
[male pose](item-animation-1.png), [female pose](item-animation-6.png),
[Finder result](itemanim-012-finder-scan.png),
[water movement after restoration](itemanim-019-water-controls-restored.png).
