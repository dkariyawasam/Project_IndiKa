# VS Seeker earned-run follow-up

Loaded the saved 29-owned campaign through CONTINUE using the new Celadon gift build. Walked through Route 2, Viridian Channel and Route 16 to Celadon; no warps or game-state edits. The input harness session dropped once during callback maintenance; reopened the same save, with no new catches or trainer outcomes lost. Do not treat that harness failure as a confirmed ROM defect.

The already-defeated Celadon rival reappeared, delivered the VS Seeker with the new dialogue, removed himself, and released player control. The Key Items screenshot verifies possession. Healed normally, returned via the Route 16 bike rental gate, and used the item. Lao had not previously been beaten, so completed his original battle normally with Chimecho; Chimecho reached level 36. Walked to recharge (read-only counter confirmed 100), used the item again, and received Lao's distinct rematch introduction.

## Defect found and fixed

The subsequent battle incorrectly selected opponent 354, Heather's rematch, beginning with Oddish 24. Lao lacks a dedicated rematch row. `GetNextAvailableRematchTrainer` reports default rematches as ready with a placeholder row index of zero; `GetRematchTrainerId` incorrectly used that row as a real trainer-table entry.

Fixed selection to return the original eligible trainer when there is no base row, before accessing the dedicated table. `tools/test_vs_seeker_default_rematch.py` compiles the actual three selection functions with test dependencies and checks unlisted defaults, listed defaults, dedicated progression, story gating and excluded trainers. Regression, trainer wiring and trainer identity checks pass; ROM rebuilt successfully.

The incorrect battle was left unsaved. The October 8 pass below supersedes the earlier pending-verification status.

## October 8: live verification passed

Reopened the isolated campaign through CONTINUE with the fixed ROM (SHA-256 `6a0d5aa6138b5067a3827acc84bc868b6441c0928f29f833f9cc617a2565b0a4`). Travelled normally from the 29-owned Viridian Forest checkpoint to Celadon. Collected the rival's VS Seeker and saved immediately, healed at the Pokémon Center, then returned through the rental-bike gate to Route 16.

Defeated Lao's original Grimer 22 / Koffing 24 team using Chimecho, which reached level 36, and saved. Normal riding recharged the item to 100. Activating it made Lao available with his rematch dialogue. Read-only observation confirmed opponent **199 / TRAINER_BIKER_LAO**, and the battle visibly used Biker Lao with Grimer 22 and Koffing 24. Completed the rematch, returned to the field with player control released, and saved normally. The charge counter was zero afterwards.

Evidence: [rematch dialogue](oct8-rematch-dialogue.png), [correct trainer](oct8-correct-rematch.png), [win reward](oct8-rematch-win.png), and [opponent ID bytes](oct8-rematch-opponent.bin). Current isolated checkpoint: `/tmp/kanto-collection-oct6/campaign.sav`; preserved copy: `/tmp/kanto-collection-oct6/earned-vs-seeker-rematch-oct8.sav`. Position is Route 16 (14,18), still 29 owned. The main save was untouched. This validates the gift, ordinary recharge, default-rematch selection, completion and save flow; it is not level-matched balance evidence or a complete collection pass.
