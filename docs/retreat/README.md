# RETREAT

Starter acquisition now gives RETREAT in move slot 3, preserving the two level-5 starting moves. This is an acquisition-only gift, not added to wild species or trainer learnsets. Existing starters are not modified automatically.

Normal-type status move, 20 PP, +7 priority, user target. Uses the Teleport animation and escape outcome. It bypasses speed-based escape rolls and trapping checks, but fails in trainer battles and Apex battles (LEGENDARY_FRLG). Ordinary Teleport retains its original behavior. Added to the forbidden-copy list used by Mimic, Metronome and Assist. Normal forgetting remains available.

Like other moves, RETREAT requires PP and passes the normal attack-cancellation checks: sleep, freeze, paralysis, confusion, Taunt, etc. can prevent execution. Its escape is guaranteed once executed in an eligible battle, not unconditional while unable to act. Using the normal Run command in Apex encounters is unchanged; their existing no-retry behavior remains.

Oak explains the move when giving a starter. An existing save's starter will not acquire it just by reloading the ROM; no existing move is silently overwritten.

Validation: main ROM build and diff whitespace check passed. `python3 docs/retreat/verify.py` checks the compiled move table, highest priority, PP, animation pointer, starter grant opcode and 12 compiled battle-script branch cases for ordinary/trapped/trainer/Apex encounters and unchanged Teleport. This is not a live emulator battle test.
