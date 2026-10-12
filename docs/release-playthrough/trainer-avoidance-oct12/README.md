# Trainer avoidance — 12 October 2026

Design intent: ordinary sight-triggered trainers should allow the player to pass through positioning and timed walking. No warning/opt-in battle prompts were added.

A source collision scan found 25 fixed-facing trainers whose unobstructed sight rays split a traversable component into two areas of at least four tiles. Changed their movement to the existing opposite-direction facing pair, retaining their positions, sight ranges, scripts, teams and rewards. See changes.json for the exact objects. Eli/Elin already uses LOOK_AROUND and was left unchanged.

Affected areas: Viridian Forest, Routes 9 and 14, Rock Tunnel, Pokémon Tower, Pokémon Mansion, S.S. Anne, Silph Co. and Victory Road. This includes optional branches and rooms, not only the main route. Story-triggered encounters were not changed.

Validation:
- ROM build passed.
- Trainer wiring audit passed: 777 trainer entries, 327 active references, no missing references or orphaned parties.
- Re-running tools/audit_trainer_sight_chokepoints.py produced no remaining candidates under its model.
- The existing opposite-direction movement randomly chooses a direction after a medium delay; while walking it does not track the player. Running nearby deliberately attracts attention through ObjectEventIsTrainerAndCloseToPlayer.

Limits: this is a static candidate check, not exhaustive live avoidance certification. It uses collision-zero source tiles and source object positions, without elevation, tile behaviors, ledges, Surf prerequisites, scripts, moving-object phases or combined sight lines. Small isolated branches below four tiles are excluded. Timing past groups of moving trainers, body-blocking in narrow paths and individual progression states still need live review. No emulator save was modified for this change. The built ROM is pokefirered.gba; the currently running separate test-ROM copy was not replaced.

## Review accepted — 12 October 2026

The user has ticked off the playthrough and trainer line-of-sight reviews. Both are closed as accepted for the current scope; further testing is not a release task unless a new issue is reported. The evidence and coverage limits above remain unchanged: acceptance does not imply additional live tests were performed.
