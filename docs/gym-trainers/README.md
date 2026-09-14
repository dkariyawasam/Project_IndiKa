# Indoor field-leader gym trainers

Implemented 13 September 2026. This pass populates Fuchsia Gym and enables indoor VS Seeker use in Celadon, Fuchsia and Saffron Gyms, whose official leader battles happen in the field. Celadon and Saffron rosters were not populated in this pass; their older trainer slots include reassigned identities and need a separate roster choice.

Fuchsia now has six ninjas: Phil, Edgar, Kirk, Shawn, Kayden and Nate. They use their existing parties and battle dialogue, with two-tile sight ranges and placements clear of the central leader/aide route. Their scripts now check for a VS Seeker rematch and enter the standard rematch battle branch. Nate has an explicit rematch entry; existing stronger rematch parties remain in place for trainers that have them. Leaders remain excluded from VS Seeker rematches.

The item-use exception is restricted to those three map IDs. Charging, range and random willingness rules are unchanged. Phil explains that the gym keeps training while Koga is away and mentions the VS Seeker after battle.

The Ninja horizontal PNG sheet lacked a frame-grouped graphics conversion rule. Added an explicit 2-by-4-tile conversion so each 16-by-32 frame is compiled together. No sprite artwork or palette was edited.

## Verification

- Main ROM builds successfully and scoped `git diff --check` passes.
- mGBA: verified all six objects and visually checked the corrected ninja sprites.
- On the final build, all six ninjas accepted rematches within two charged activations. Declining an individual activation is normal VS Seeker behavior.
- Phil's actual rematch battle started and completed with a win through normal battle input.
- Celadon and Saffron accept the item, reporting no trainers in range with their current empty rosters. Pewter still rejects indoor use.
- Disposable fixtures staged travel, charge, prior trainer victories and a level-100 Pokémon. This is functional verification, not difficulty balancing or six complete first-time battles.
- The broad trainer wiring audit reports ten existing unreferenced party definitions; none of those definitions or their references were changed here. It reports no missing trainer/party references. This unrelated audit debt was not altered.

`runtime-tests.tsv` preserves repeated observations. `runtime-summary.tsv` contains the final 11 assertions. Screenshots and the session-specific Lua harness are included. Preliminary testing caught and resolved the graphics conversion and missing rematch-branch problems before the final build.

Final ROM SHA-256: `f2ec13777c2221fa1f59b58befe69ab696a5951ac74d0c3a7317c8258eb922fb`.
The user's main save and retained customization ROM remain unchanged.
