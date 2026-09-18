# Fresh-save release playthrough

Status: IN PROGRESS. Started 18 September 2026.

The isolated emulator uses `/tmp/kanto-release-playtest/expedition-kanto.gba`, copied from the main ROM. The user’s existing emulator/save is not used. Initial live ROM SHA-256: `ecfe7340f7a236013cecdecb07d117497485b3934a3388cf87a30f3917daac02`.

This run uses normal game inputs with read-only memory logging. No party-level, inventory, story-flag or map-position changes are permitted in the continuous difficulty run. Emulator fast-forward only reduces wall-clock time.

## Completed preparation

- Replaced all three Saffron Gym female Psychic placeholder overworlds with a dedicated sprite based on the existing battle portrait.
- Added nine 16×32 frames, an indexed 16-colour palette and a dynamic palette registration. Explicit 2×4-tile frame packing.
- Trainer overworld audit: 354/354 matched, zero mismatches.
- Frame packing: 177 sheets, 1,266 frames checked.
- Palette audit: 182 sprite/palette pairs passed. Existing Route 9 whole-map dynamic demand warning remains; no new warning.
- Main ROM built; trainer lineup regenerated.

The generated artwork source is `psychic-f-candidate.png`. It was exported to the GBA frame/palette format; in-game appearance still needs live review.

## Live coverage

- Fresh boot, Oak introduction, player house and lab reached through ordinary inputs.
- Mum correctly points to the lab without claiming Oak is presently there.
- The rival says Oak stepped out and asked him to wait. The missing destination was identified live; the main source now adds that Oak went to check ROUTE 1. That wording is built but has not yet been replayed in this continuous run.
- Oak approached the north exit from the north and escorted the player into the lab successfully.
- Test character is RED; automated name entry produced rival name GGGGGGG (cosmetic).
- BULBASAUR obtained at level 5 with RETREAT; rival CHARMANDER battle won with TACKLE, reaching level 6 with 6 HP before scripted recovery. No injected team, items or healing.
- First Route 1 crossing completed: two wild battles won, BULBASAUR reached level 7 with 7/24 HP. Enemy species were not logged during those first two battles.
- Viridian Pokémon Centre restored the party to 24/24 HP through normal interaction.
- A normal save was made. The first persistence check was invalidated by duplicate disposable emulator processes; no game code was changed for this. After removing the duplicate, saving wrote valid cartridge data, and the updated ROM loaded RED in Viridian with the same level-7 party and zero badges.
- An emulator checkpoint recovered the same earned Viridian progress while diagnosing that test setup. No battles were replayed for a better outcome, and no party/story values were injected.
- Level-5 HOPPIP caught on the second Poké Ball after two TACKLEs. Pokédex registration displayed; capture EXP raised BULBASAUR to level 8.
- Level-8 BULBASAUR used RETREAT against level-21 PIDGEOTTO. It escaped before the enemy attacked, with HP unchanged at 26/26. Runtime battle text and outcome 5 confirm the escape.
- Reached Pewter through Route 2 and Viridian Forest. The rest of the campaign and both leagues remain outstanding.

Do not interpret this document as a completed playthrough or release certification.

Updated live ROM: `/tmp/kanto-release-playtest/campaign.gba`, SHA-256 `98a8f061f295a588e62d879b4e117cfb478978bee635370bee2ddd4e0131bfbf`.

## Early balance observation (still in progress)

JANICE intercepted the return through Route 1's northern grass. Her first level-10 PIDGEY used GUST against level-8 BULBASAUR. LEECH SEED followed by GROWL did not prevent BULBASAUR fainting on the next GUST. The level-5 HOPPIP can temporarily survive with SYNTHESIS and the existing LEECH SEED, but has no direct damaging move yet. This is a deliberately recorded weak matchup, not evidence that every starter/team must lose. No trainer levels have been changed.

A source wording correction changes BEN's claim that Route 1 grass is “gentle” to a warning that it hides surprises. The replacement text still needs live playback.

JANICE outcome: loss. The first PIDGEY was defeated by the existing LEECH SEED while HOPPIP used SYNTHESIS; HOPPIP reached level 7. The second PIDGEY defeated HOPPIP, which still lacked a damaging move. Blackout deducted 64 money and returned the party healed to Viridian, preserving earned levels/capture. No retry rollback or trainer nerf was applied.

## Forest and Pewter continuation

- Route 2 rival scene completed and named the alternate exploration routes. The forest entrance rumour NPC supplied the warning about the forest ensnaring people.
- Caught level-8 WEEDLE on the third ball, using two TACKLEs to weaken it. No capture resets.
- Erika’s opening request played fully: wild ODDISH/BELLSPROUT may hold ENERGY ROOTS; catch one, check its held item and bring the root back. The trial has not yet been completed.
- A further wild WEEDLE battle raised BULBASAUR to level 9. BUG CATCHER SAMMY was defeated normally, leaving BULBASAUR at 9/28 HP.
- Level-27 wild PIKACHU on northern Route 2 was escaped using RETREAT before taking damage.
- Reached Pewter and healed all three party members at the Pokémon Centre. The Gym aide intercepted entry and explicitly directed the player to BROCK in MT. MOON.
- Normal in-game saves completed after SAMMY and again inside Pewter Gym after the aide’s introduction. Current saved team: BULBASAUR 9, HOPPIP 7, WEEDLE 8, all healed; zero badges.

## Main ROM consolidation — 18 September 2026

Rebuilt the main `pokefirered.gba` with the current source and assets, including Psychic F and the Oak/Route 1 wording corrections. SHA-256: `c2b5e1b343a31fe731de37a22e160731ee82dd9745c74303a2ee3d851e371849`. Removed the ten temporary playtest ROM copies under `/private/tmp`; saved games, screenshots, logs and test tools were retained. Earlier ROM paths in this journal are historical. The separate player-customisation ROM is preserved unchanged. This build consolidation does not mark the full playthrough complete.
