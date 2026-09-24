# Amunyte front sprite

Converted supplied artwork directly, without generative redrawing. Cropped the artwork from its white canvas, removed exterior-connected background, sampled without smoothing to 57×62 and centred on a 64×64 canvas. Encoded into fifteen opaque GBA colours plus transparency, retaining separate ramps for shell, plume, body and eye.

Front and normal palette now replace the placeholders. Shiny front uses the normal palette pending separate shiny artwork. Back, icon and footprint remain outside this change. Main ROM build passed; all 4,096 encoded pixel indices match the indexed PNG. No live emulator check performed.

Original user source and enlarged final preview are retained in this directory.


## Updated native front — 24 September

Imported the updated 64×64 user source, archived as `user-native-source-update.png`, preserving its occupied-pixel mask and mapping colours to the current shared Amunyte palette. Previous front retained as `front-before-native-update.png`. Main ROM rebuilt and encoded 4bpp pixels verified. The existing back still includes the former purple plume and needs a matching revision for this new design.
