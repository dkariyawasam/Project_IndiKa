# October 9: earned Porygon terminal upgrades

Used the isolated campaign's existing level-26 Porygon from Box 1, plus the Upgrade and Dubious Disc earned in the preceding access checks. Deposited Milotic in Box 2 to make room. Walked from Cinnabar's Pokémon Center to the gym terminal. Normal buttons only; read-only RAM inspection for identity, inventory and Pokédex checks. No injected prerequisites, debug warps or save-state restores. Main user save untouched.

## Live outcomes

- Declining the terminal offer returned control.
- Cancelling the party picker returned control.
- Selecting Porygon with no held software showed the incompatible-partner message, with no changes.
- Gave Upgrade to Porygon normally. Answered an Upgrade check incorrectly: installation cancelled, species remained Porygon and Upgrade stayed held.
- Retried with YES / NO / YES: terminal delivered Porygon2 and consumed Upgrade.
- Porygon2 remained level 26, with the same moves, PP and friendship. Read-only raw records confirmed the original personality and trainer ID were retained.
- Gave the earned Dubious Disc to Porygon2. Answered YES / NO / YES: configuration rejected, Porygon2 and its held disc unchanged.
- Retried with NO / NO / YES (one incorrect answer): configuration accepted, normal machine receive scene showed Porygon-Z, and the disc was consumed. Level, moves, PP, friendship, personality and trainer ID remained intact.
- Read-only Pokédex checks confirm Porygon, Porygon2 and Porygon-Z caught. Save panel increased from 48 to 50 owned.
- Saved normally, soft-reset and loaded through CONTINUE. Level-26 Porygon-Z persisted with no held item.

## Wording correction and scope

The initial prompt said “Three checks must pass first,” which contradicted the Dubious Disc's intended incorrect-answer route. Changed it to “Three checks configure it.” The rebuilt ROM succeeds and the line is shorter than its predecessor.

Oak's Bond counter already includes Porygon2. The existing Oak regression suite passes. This campaign had already acknowledged Oak's research, so no fresh Oak milestone notification is claimed by this pass.

Patch Disc remains undistributed; no give-item source was added. Its future Porygon3 branch was not exercised in this earned run. Earlier controlled terminal coverage remains separate.

Screenshots and decoded records are alongside this note. Live behavior ROM SHA-256: `654422ccaef66c23881deebcded115bf28bcd5fdaf492cb0a6e68724c435a1ca`.

The final wording-only rebuild has SHA-256 `08ad049a0d5192b1f884ae10e7d31db3fe353f66cfb52ad548aa5319e38f985b`; the running campaign still uses the behavior-tested build.

Earned checkpoint: `/tmp/kanto-collection-oct6/earned-50-owned-porygonz-oct9.sav`.
