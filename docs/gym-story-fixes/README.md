# Gym progression fixes

Implemented after the gym playthrough review, 13 September 2026.

- Both Sabrina victory paths now unlock and complete her Gym Leader Trials subquest.
- Giovanni's victory now unlocks and completes his Gym Leader Trials subquest, alongside his existing story quest completion.
- Entering Saffron Gym with the Marsh Badge or Viridian Gym with the Earth Badge repairs missing completion records in existing saves. These writes are idempotent and do not replay battles or rewards.
- Erika's aide arrives after the Energy Root is shown, including when the player has fewer than two usable Pokémon. The party check now happens after her arrival.
- Re-entering Viridian Forest at trial state 3 restores the aide at her witness position (4, 48). Before the root handoff and after victory she remains hidden. The existing post-battle departure remains intact.

Validation: `make -j4` and scoped `git diff --check` passed. Reviewed all three victory call sites, both existing-badge repair paths, the transition state guard and aide arrival/party-check ordering.

Runtime validation completed on retry: all nine focused checks passed in mGBA 0.10.5. `runtime-tests.tsv` records aide absence before the root, arrival with one Pokémon, re-entry at the witness position, absence at the post-victory state, three native victory-script completion paths and two existing-badge repairs. Screenshots confirm the aide beside Erika both after the root and after re-entry. These are staged script/state regression checks; the eight full battles were exercised during the preceding playthrough, not repeated here.

The first retry started in the opening sequence rather than the intended old save state, invalidating its early checks. That startup run was excluded from `runtime-tests.tsv`. After the disposable game reached normal field execution, the complete suite was rerun and all nine checks passed. The dialogue log retains that startup text for transparency. No game code needed further changes.

The archived harness contains session-specific temporary paths and requires a matching ROM and disposable field fixture; it is not a standalone portable test suite. The earlier playthrough report remains a historical record of the bugs before these changes.

Main ROM SHA-256: `5afdab868b6de4571be4ed08880b55d4b75f51e6374beacbc7e286ed954cae9a`.

The main save and retained player-customisation ROM are unchanged from the playthrough baselines. No user save was edited.
