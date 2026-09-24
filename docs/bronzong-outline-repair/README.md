# Bronzong outline repair

Restored interrupted outer edging using the same boundary-only approach as the Mamoswine repair: 32 front pixels and 79 back pixels remapped to existing dark teal index 3. Existing black/dark outline pixels are retained. The back view's intentional canvas crop is retained.

Verified both 64×64 indexed images preserve the exact transparency mask, palette, and interior pixels. Normal and shiny comparison inspected. Main `pokefirered.gba` build passed; no live battle verification in this pass.

`front-before.png` and `back-before.png` preserve the pre-edit assets; coordinate change lists record every modification. `comparison.png` shows the actual game assets before/after and shiny after. An image-generation repair reference was requested separately; generated pixels were not imported into the game. The ROM uses the precise indexed boundary repair.

The continuous campaign remains saved at Cinnabar after the first Koga attempt. Its loaded test ROM has not been replaced during this asset pass.
