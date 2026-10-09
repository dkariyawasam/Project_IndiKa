# October 9: earned Feebas friendship evolution

Continued the isolated campaign using ordinary game inputs. No injected Pokémon, items, friendship, experience or flags; no debug warps or save-state restores. Updated the campaign ROM and loaded through CONTINUE. The main user save was untouched.

## Live results

- Healed at Cinnabar and deposited Golem to make room. Fished on Route 20 at the unchanged 1% Feebas encounter rate. Caught a level-8 ordinary Feebas in a Great Ball after putting it to sleep.
- Used the two earned Exp. Candy S at low friendship: Feebas reached level 11, remained at 70 friendship, and did not evolve.
- Raised friendship through running in Cinnabar, with read-only observations. Stopped at 224; Feebas was still level 11. Fast-forward changed playback speed only.
- A party-cursor mistake used the earned Exp. Candy L on Chimecho (36 → 39). Kept that outcome, declined Heal Bell, and did not restore a save. Explicitly verified party slot 5 before the next item use.
- Used the earned Exp. Candy XL on Feebas. It reached level 28, learned Tackle and evolved into Milotic at 224 friendship. Captured the normal evolution completion screen.
- Read-only Pokédex checks confirm both Feebas (221) and Milotic (222) caught. The normal save panel shows 48 owned, up from 46 before this pass.
- Saved normally in Cinnabar, performed the game's soft reset, and loaded through CONTINUE. Milotic level 28 and friendship 224 persisted.

## Oak integration fix and tests

The Bond counter enumerated eligible species and omitted Milotic after the friendship-evolution change. Added Milotic to `CountEvolutionThroughBondMilestones`.

`tools/test_oak_expedition.py` now exercises the production Bond counter: Crobat + Feebas alone does not meet the target; adding Milotic supplies the second milestone and enables Oak's Bond reaction; acknowledging it prevents a repeat. Existing expedition prerequisites and reaction ordering also pass.

`tools/test_eevee_evolution.py` now explicitly checks ordinary Feebas at 219/220 friendship, day and night, Beauty independence, Everstone prevention, no trade evolution, and unchanged Deep Form Feebas. All checks pass. The ROM rebuild succeeded.

This campaign had already completed and acknowledged Oak's research before this test. A fresh Oak celebration is therefore **not** claimed as live evidence; its new Milotic eligibility is covered by the production-code test.

Evidence screenshots and read-only JSON observations are alongside this note. The fishing helper briefly encountered a file-completion race while reading an enemy record; the helper was fixed and the run resumed without changing game state.

Tested ROM SHA-256: `654422ccaef66c23881deebcded115bf28bcd5fdaf492cb0a6e68724c435a1ca`.

Earned checkpoint: `/tmp/kanto-collection-oct6/earned-48-owned-milotic-oct9.sav`.
