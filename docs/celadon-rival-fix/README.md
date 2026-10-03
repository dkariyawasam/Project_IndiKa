# Celadon rival approach fix — 3 October 2026

The coordinate encounter now removes/recreates the hidden rival after `lockall`, before any approach movement. It clears his hide flag before spawning so the object is also retained when returning from battle. The dialogue, team, approach paths and completion conditions are unchanged.

Two object-lifetime cases were checked:

- At the eastern trigger (40,35), the rival's starting point (30,36) can be outside the automatic object activation range. On the old build, entering from the east or north produced the opening dialogue without a rival on screen. The script must explicitly spawn him rather than depend on the transition-time `addobject`.
- `lockall` freezes NPCs immediately, including an invisible object's pending single-movement initialization. `ObjectEventSetHeldMovement` refuses to start while `singleMovementActive` is set. A controlled fixture that preserves that pending state at the lock reproduces an indefinite `WaitForMovementFinish` before dialogue. Recreating the object after the lock clears this stale state. This timing case was injected to test the failure path; it was not naturally reproduced during the directional-entry runs.

## Live verification

Used independent `before.gba` and `fixed.gba` copies with copied saves under `/tmp/kanto-celadon-freeze`. The main user save and the earned expedition checkpoint were not modified. Debug commands reset only the fixture's Celadon scene/prerequisite values and test location.

- Old build: ordinary entry from north at x38/x39 reached dialogue; x40 reached dialogue with no visible rival. West/east/south approaches also checked. The interrupted-initialization fixture remained locked in `WaitForMovementFinish`.
- Fixed build: all three trigger columns reach dialogue with the rival visible below the player. The same interrupted-initialization fixture reaches `Std_MsgboxDefault` and waits normally for player input instead of waiting indefinitely for movement.

The JSON snapshots contain script mode, script pointer, native wait pointer and field-lock state. A field lock of 1 during an open message is expected; the important distinction is `IsFieldMessageBoxHidden` versus the stuck `WaitForMovementFinish`. See [results](results.json) and the paired screenshots.

- Continued from the corrected interrupted-initialization fixture, won the rival battle using normal battle inputs, completed the post-battle dialogue and departure, and confirmed the script stopped with field controls unlocked. The Celadon completion variable and rival hide flag are both 1. Walking off and back onto the trigger did not restart the encounter.

Build succeeded. Navigation/sign checks passed; `git diff --check` passed.
