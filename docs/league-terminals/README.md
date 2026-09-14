# League terminals

Added an Indigo terminal beside the League entrance (2, 2, face north from 2, 3).
Rocket's existing prize-board console (8, 7, face north from 8, 8) is now its League
terminal. Both provide Rules, Current challenge, Records, Rewards and Exit.

Records reuse the link battle records screen's graphics, text/window setup, fade and
return flow, with separate League data. Link records retain their original behavior.
Indigo records track battle wins/losses, championships, current title streak and best
title streak. Rocket adds gross coins actually received; wagers aren't deducted.
An unfinished withdrawal breaks the title streak without counting as a battle loss.
Finishing a championship and returning to the lobby preserves the title streak.

Current challenge shows live progress when available, or the most recent run's
result. Returning to a lobby still ends an unfinished run under the existing rules.
Rewards explain Indigo's Hall of Fame/title rewards and Rocket's existing payouts.
Rocket's Rewards option offers its original, explicitly confirmed multiplier board.
No new currency, party-size restrictions, prize items or milestone payouts were added.

Records start with this update. A versioned record block consumes 44 bytes of former
SaveBlock2 padding under agbcc, with no change to total size or existing field offsets.
Counters saturate at 65,535; recorded coins saturate at 999,999,999. Legacy padding is
initialized lazily using a magic/version check. Old battles cannot be reconstructed.

Validation completed:

- Main ROM build passed.
- Production record functions passed host tests for initialization, separate leagues,
  four rounds plus Champion, repeated titles, withdrawals, loss de-duplication, coin
  totals, serialized record restoration and overflow saturation.
- Actual agbcc old/new layouts match (save size 3912; league state 2836; record/padding
  start 2856; encryption key 3896; quest data 3900).
- Dialogue widths, both background-event locations and received-coin hooks passed.
- Existing Indigo scaling/restoration tests passed.
- Main save and retained player-customisation ROM hashes remain unchanged.

Live UI verification completed in mGBA on 2026-09-13 using a disposable copy of
main ROM SHA-256 `bbbb4a0a4bdba4f9dcfbc041756977ada15fdf42f99f3958c9bfa108536f5d9b`.
Both physical terminals opened their menus. Rules, status, rewards, zero/populated
records, A/B record-screen returns and menu exits were checked. Rocket Rewards
opened the original multiplier board; declining its wager returned to the field.
Indigo status correctly reported a completed four-round run and Champion victory.

For record fixtures, native challenge specials staged five victories and one title
per league, plus 5,000 received Rocket coins. These are menu/persistence tests, not
additional full battles or balance tests. The actual game save routine wrote the
disposable save, then mGBA was closed and restarted. Both record blocks matched on
reload (see `runtime-tests.tsv`), and both reloaded records screens displayed the
expected values. `indigo-records-reloaded.png` and `rocket-records-reloaded.png`
show the results. No terminal code changes were needed during this live check.
The main save and retained customization ROM were untouched.
