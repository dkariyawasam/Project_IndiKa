# Deep Form Feebas front — October 4, 2026

Imported the supplied 64×64 front sprite without redrawing or recolouring it. The indexed source retains all 14 opaque colours and the original transparency mask; the GBA build performs its normal colour-depth conversion.

Front graphics and normal palette now use dedicated CinnabarFeebas assets. The shiny table temporarily uses the normal palette, avoiding incompatible placeholder colours; a distinct shiny design is still outstanding. Back sprite and menu icon remain separate unfinished assets. Build passed.

## FRLG colour pass

Following the front import, the palette was revised using Deep Magikarp as the colour reference: shared warm outline and brown shadows, related neutral eye colours and warmer lips, plus a darker blue fin ramp with softer highlights. Pixel indices and transparency remain unchanged. The pre-pass sprite and palette are preserved alongside this note.

The follow-up pass lightens the body and blue fin ramp and restores all three original pink lip shades. The darker outline remains; no sprite pixels were redrawn.

The subsequent user-supplied corrected front replaces the colour-pass version. Its 64×64 pixels, 14 opaque colours and transparency are preserved exactly in the indexed source; normal GBA colour-depth conversion applies when building.
