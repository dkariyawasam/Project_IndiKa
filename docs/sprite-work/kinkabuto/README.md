# Kinkabuto front sprite

Front and normal palette replace the placeholders. The shiny front currently uses the same palette, avoiding an unrelated placeholder palette. Back and menu artwork remain pending.

Revision 3 reverted at the user's request. Restored front-v2.png as the active front sprite, regenerated the preview and rebuilt the main ROM.

Compact user reference replacement: directly converted the new supplied artwork (user-compact-reference.png), without generative redrawing. Cropped its 334×360 artwork bounds, nearest-neighbor sampled to 56×60 and centred on 64×64. Exterior-connected white background becomes transparent; interior highlights remain. Encoded using the current fifteen opaque palette colours. Source/design preserved through format conversion, subject to size and palette constraints. Previous front saved as front-before-new-reference.png. All 4bpp indices verified against PNG; main ROM build passed. No live emulator check.

Sharpness-reference pass: used the supplied 97×101 reference directly (art bounds 82×89), sampled without smoothing to 57×62 on the 64×64 canvas. Reserved distinct red/pink eye and cool grey facial palette entries, with dark outline indices along the silhouette. Earlier active front and palette retained as front-before-sharpness.png and normal-before-sharpness.pal. Main ROM build passed. The larger reference has more pixels than the GBA sprite, so this is a constrained adaptation rather than exact detail preservation.

Sharpness pass reverted following user feedback that the previous version was cleaner. Restored front-before-sharpness.png and normal-before-sharpness.pal byte-for-byte, regenerated preview, and rebuilt main ROM successfully.

User native replacement (kink.png.png): source is already 64×64 with fourteen colours, so imported directly without resizing, redrawing or colour reduction. Only exterior-connected white background became transparent index zero; internal white details retained. Verified all foreground RGB pixels match supplied source exactly in the indexed PNG. Palette conversion to the GBA's 5-bit channels occurs normally during compilation. Prior sprite/palette backed up as front-before-user-native.png and normal-before-user-native.pal. User source archived as user-native-source.png. Main ROM build passed.

Updated native source supplied after Amunyte: imported current kink.png.png directly at 64×64, preserving all foreground source colours and pixels, with exterior white background transparent. Archived source as user-native-source-update.png; previous assets retained as front-before-native-update.png and normal-before-native-update.pal. Main ROM rebuild passed.

FRLG palette / edge cleanup: retained the native user sprite geometry and interior pixel indices. Inspection found a single connected sprite and no exterior white pixels; light outline interruptions were the likely artifacts. Darkened 37 light perimeter pixels into the appropriate dark brown/charcoal ramp, keeping the entire transparency mask unchanged. Palette uses Kabutops golds/greys, Omastar pale ivory, subdued navy and red. Previous source/palette retained as front-before-frlg-cleanup.png and normal-before-frlg-cleanup.pal. Main ROM rebuild passed; live emulator appearance not checked.
