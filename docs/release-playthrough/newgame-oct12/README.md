# New-game smoke test — 12 October 2026

Tested the current ROM on a separate blank-save fixture, using ordinary game inputs. No story flags, party data or position warps were injected. The existing path helper walked the player through the house using directional inputs. Main campaign save remained unchanged.

Verified live:
- Title and introductory dialogue display and advance.
- Combined appearance window: Style 2, primary BLUE, secondary GREEN; preview and dialogue frame update.
- Player and rival naming accept input (test names A and B).
- New game reaches the bedroom with normal movement and the selected style.
- Stairs reach the first floor; the central exit reaches Pallet Town.
- Radial menu uses the selected green theme.
- Normal in-game save completes to the disposable save file.
- Restart exposes CONTINUE; continuing restores Pallet Town and the chosen appearance/theme.

`restored-appearance.txt` records gender=1, version=3, primary=1, secondary=2, frame=2 using offsets verified against the compiled appearance function. The offset comments in `global.h` predate the expanded Pokédex and must not be used directly for this runtime read.

Cosmetic follow-up: naming screens now load the shared theme-aware header palette instead of the fixed blue palette. The rebuilt ROM was tested through a fresh disposable intro with secondary GREEN; player and rival naming headers match the selected theme (theme-player.png and theme-rival.png). The original screenshots 07 and 09 document the pre-fix behaviour. Blue retains its original palette; red uses the existing shared red palette. Red and blue naming were not separately replayed live.

The original smoke ended in Pallet Town. The follow-up below extends the fresh-save test through the opening battle, expedition handoff and Viridian Mart. It does not repeat the full release playthrough, all appearance combinations or every starter/battle outcome.

Fixture ROM: `/tmp/kanto-newgame-oct12/smoke.gba`; disposable battery save: `/tmp/kanto-newgame-oct12/blank.sav`. The fixture was initially loaded temporarily, then attached for disk writes before the save check. The game displayed its different-file overwrite confirmation on the first save; the disposable file was explicitly overwritten. The later save and CONTINUE completed normally.

Campaign SHA-256 remains `001a04fc396009ff0a8fecd353eba53a721b32f6577dc2c19950472aa66b4ac4`.

## Opening progression follow-up

Continued the rebuilt naming-theme fixture (`/tmp/kanto-newgame-oct12/theme.gba`, battery save `theme.sav`) with normal directional and button inputs. No story flags, party data, battle outcomes or positions were injected. Read-only memory checks supplement screenshots in `opening-state.json`.

- Left the bedroom, house and Pallet Town normally. Oak intercepted the player at the north exit, escorted them into the lab, and released control after the briefing.
- Selected Bulbasaur; confirmation preview and RETREAT explanation displayed correctly. Declined a nickname. Rival AA received Charmander (battle nickname ACE).
- First rival battle completed. This run used Tackle and lost; native battle outcome was 2. The loss branch continued correctly, healed Bulbasaur to 19/19 HP and restored its PP.
- Oak immediately delivered the expedition/Pokédex briefing. The old parcel delivery errand is intentionally bypassed in the current scripts, so it was not a test target.
- Opening handoff finished with lab scene 6 and Mart scene 2. Bag contained five Poké Balls, and no parcel. Starter retained Tackle, Growl and RETREAT.
- Walked Route 1 to Viridian City; entered the Mart and opened its normal shop. No parcel prompt or softlock occurred.
- Pokédex opened with 2 seen / 1 owned. Logbook displayed THE NATURE OF EVOLUTION as active and GYM LEADER TRIALS; Bond, Instinct and Design each began at 0/2.
- Saved normally to the disposable theme.sav (explicitly confirmed overwriting only this fixture).

Evidence: `opening-starter-choice.png`, `opening-first-battle.png`, `opening-battle-progress.png`, `opening-battle-progress2.png`, `opening-mart.png`, `opening-pokedex.png`, `opening-logbook.png`, `opening-logbook-details.png`, `opening-saved.png`, and `opening-state.json`.

No production-code changes were needed. The other two starter choices and the opening battle's win branch were not exercised in this run.
