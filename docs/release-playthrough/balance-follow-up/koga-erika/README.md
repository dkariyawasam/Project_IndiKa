# Koga / Erika targeted checks — 11 October 2026

Erika now uses Leafeon in tiers 3–7, at levels 25, 32, 34, 39 and 41 respectively. It replaces Gloom, Exeggcute, Seedot, Shiftry and Shiftry. Team sizes and level arrays are unchanged. Tiers 1–2 retain their existing rosters; Tangela remains in every team that already included it. There are no repeated evolution families within any of Erika's seven teams.

Middle-tier Leafeon uses Helping Hand alongside Grass STAB and priority or coverage; later tiers use Swords Dance, Body Slam, Razor Leaf and Synthesis. This makes Leafeon a recurring part of Erika's kimono/Eeveelution theme in this mod.

The ROM built successfully and the trainer roster audit passed. The build retains the existing linker RWX-segment warning.

Live checks target Koga tier 6 and Erika tier 7, one seed-11 trial each. Parties use native default moves, random IVs/natures and zero EVs, at levels 40–41 and 45–46 respectively. No Bag items or tactical switching; the controller attacks and replaces fainted Pokémon. This is limited functional and difficulty evidence, not a win-rate measurement. The rebuilt ROM was restarted and loaded from a disposable save before these runs.

`cases.json` specifies the exact parties. `verify-fixtures.py` validates starting Pokémon checksums/species/levels/EVs and captured enemy rosters. TSV fields are case, native outcome (1 win, 2 loss), completed-turn counter, survivors, remaining HP, maximum party HP.

ROM SHA-256: `ad3b40c98111ef824c4348cd446815969df3ab9676cc2fb36b47d0821299a2f1`.

## Results

| Trial | Result for player | Completed turns | Survivors | HP |
|---|---|---:|---:|---:|
| Koga tier 6, seed 11 | Loss | 13 | 0 | 0/663 |
| Erika tier 7, seed 11 | Loss | 15 | 0 | 0/718 |

Both native battles completed, returned to the lobby healer, and released field controls after the healing dialogue. All 12 starting Pokémon passed integrity/species/level/zero-EV checks; enemy snapshots match the expected rosters, including Leafeon in Erika's opening slot. These single trials show that the rosters function and challenge this fixture. They do not establish general difficulty or warrant weakening either leader. Native default moves, unevolved Growlithe/Pikachu and the controller's limited tactics make this a weak proxy for a prepared late-game player.

The earned campaign save remained unchanged. Production changes are limited to five Erika party slots and their movesets. No Koga changes were made.
