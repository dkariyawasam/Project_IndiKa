# Title footer cleanup — 3 October 2026

The source copyright map reused a tile containing a black stroke as empty padding. The right-hand side also included old glyph fragments. The FireRed grass/map packer now supplies clean footer padding and masks only the unused portion of the final copyright tile, preserving the full final K.

The existing copyright text and Game Freak splash are unchanged. Both PRESS START blink tile sets were decoded with the generated map: footer pixels match between states, and all pixels outside the text are clean padding. ROM build and whitespace checks pass. Images in this directory are enlarged renders of the built footer tiles, not live emulator screenshots; yellow indicates transparent pixels that reveal the layer beneath.

## Separate mod credit

The footer now occupies the bottom 24 pixels and includes `MOD BY DEE KARIYAWASAM · 2026` below the preserved original copyright lettering. It uses the existing small Latin game font; both lines are centered. The Game Freak splash logo is unchanged.

The packer reads the compiled font and charmap through explicit build dependencies, so the text is reproducible. The generated foreground fits its character block (highest tile 301, below 512), and the footer has identical pixels with PRESS START visible or hidden. [Built footer preview](mod-credit-preview.png).

## Current layout and title cry

At the user's request, the title strip now contains only `MOD BY DEE KARIYAWASAM - 2026`, centered in a 16px footer. Original intro splash credits and the Game Freak logo remain unchanged. The earlier two-line preview above is historical; [current preview](mod-only-preview.png).

The FireRed title START/A transition now calls `PlayCry_Normal` for `SPECIES_TANGROWTH`. Verified compiled argument 421 (`0x1A5`), and all 78 imported-cry checks pass. The ROM builds successfully and both blink states retain identical footer pixels.
