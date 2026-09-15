# Apex readiness prompts

All eight Apex encounters now share the same confirmation before their encounter sequence:

> This APEX POKéMON is Lv. 50.
>
> Once battle starts, you have only one chance.
>
> If you lose or flee, this encounter will be gone.
>
> You can prepare and return before starting the battle.
>
> Are you ready to challenge it?

Mewtwo correctly displays level 70. The others display 50. No or B closes the conversation and releases the player; it does not create an enemy, record an Apex interaction, mark the encounter fought, hide the object, or change the retry rules. Yes continues the existing sequence.

The shared script lives in `data/scripts/static_pokemon.inc`. Each map calls it immediately after locking/facing the NPC and before creating the event Pokémon, camera shake, music or interaction flags.

## Validation

- Main ROM build passed.
- `python3 docs/apex-readiness/verify.py`: all eight entry points are gated before side effects; levels match their event Pokémon; the decline handler only releases/exits; dialogue fits the game font.
- Native mGBA checks ran each actual entry script on its own map with the revealed object present: No, B, then Yes. All 24 branches passed (`runtime.tsv`).
- No/B preserved fought/interacted/hide flags and the entire enemy party buffer, released controls, and left the object available for another interaction. Yes set the interaction flag and entered the expected-level battle.
- These used a disposable fixture, with the relevant Apex made available and a same-build snapshot restored between encounters. Accepted battles were stopped after verifying entry; their outcomes were already tested in the difficulty pass. No main save was changed.

Screenshots include each final confirmation, declined overworld state and accepted battle. `cases.json` identifies the map, entry point, object and relevant flags used for each live check.

Validated main ROM SHA-256: `a7119753dc0f869a1c4dc862cfbecdfb9f972aa48928522c106c1c399a53465b`. Protected main save and retained player-customisation ROM hashes matched their pre-test values.
