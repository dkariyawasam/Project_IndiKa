# Overworld frame-packing audit

Compared 1,257 declared frames across 176 indexed PNG sheets pixel-for-pixel against graphics embedded in the main ROM, using the dimensions and frame numbers in object_event_pic_tables.h.

Found and fixed missing horizontal 16×32 frame packing rules for:

- Gambler: 2,699 incorrect pixels before correction.
- Janine: 2,806 incorrect pixels.
- Pokémaniac: 3,033 incorrect pixels.
- Rocket League attendant: 700 incorrect pixels.

Fisherman passes with its earlier fix. All checked frames now match their source images. Added durable make rules, regenerated the four graphics and rebuilt the main ROM. No palette or map modifications.

Run tools/audit_overworld_frame_packing.py with Pillow installed to repeat the check. audit.json contains per-sheet counts. Three non-character assets with special frame geometry are explicitly excluded: S.S. Anne, Seagallop and the Town Map. This verifies frame packing, not live animation sequencing, palette allocation or collision behavior.
