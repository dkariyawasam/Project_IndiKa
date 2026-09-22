# Slowking front refresh

The built-in imagegen tool edited the existing front, with Slowbro as a pink-colour reference. Prompt: widen Slowking’s head and torso modestly, preserve crown, collar, pose and classic GBA pixel-art style; single front sprite on transparent background.

Generated source is retained as generated-front.png. import-front.cjs removes the generated checkerboard background and converts to a 38×60 silhouette on a 64×64 indexed canvas using the existing palette slots. Front coordinates updated. Body palette slots 2–7 now use Slowbro pinks; back uses the same updated normal palette. Shiny palette retained and a shiny preview checked. Main ROM rebuilt; live battle review remains outstanding.

Correction: the generated front lost facial and foot detail during reduction. It has been replaced by the original front geometry, retaining Slowbro's pink palette. The wider generated version is no longer installed. A subsequent generation attempt was blocked by the image tool; no replacement image was produced.

Direct indexed edit approved by user: `direct-edit/widen-belly.py` widens rows 43–58 by up to six pixels, tapering toward the original feet. Rows 0–42 and 59–63 are byte-identical to the prior sprite, preserving face, crown, collar and feet. Palette indices are retained, so normal/shiny shading remains consistent. The new front replaces the restored narrow version.

Refinement: reduced belly expansion to three pixels maximum after the six-pixel version looked too wide. Preserved regions and palette remain unchanged.

Body colour refinement: shifted the normal pink ramp one shade darker in slots 2–7, adding RGB 98,32,32 for its deepest shadow. Other palette slots, sprite indices and shiny palette remain unchanged. Both front and back use the darker ramp.

Shell/tail refinement: normal shell palette slots 11–14 use Slowbro slots 9,8,7,6 respectively. Front tail pixels are extended outward by 2–3 pixels at their existing attachment, preserving rows above 43 and below 54. See tail-edit/update.py and its source backups. The shared neutral-shading slots also colour the small shaded areas on the collar. Shiny palette remains unchanged.

Tail clarity refinement: replaced the exposed front-view tail with a rounded upward curve and continuous outline, using lighter pink indices 7/2/3 instead of the prior 7/5/4 ramp. The palette and body are unchanged. Reproducible edit and preview: tail-refinement/redraw.py.

Latest palette refinement: shell slots 14/13/12 shift to the previous 13/12/11 colours, with a new deepest olive shadow (49,65,24). Snout/belly slots 8/9/10 use Slowpoke normal slots 6/5/4. Both views updated; sprite geometry and shiny palette unchanged.

Front body polish: added 12 small white highlight pixels on forehead, shoulder, belly and flank, inspired by Slowbro/Slowpoke; added 11 sparse dither pixels along body shadow boundaries. Shell, snout, tail, outlines and palettes retained. Reproducible edit and before/after comparison in body-highlights/.

Tail tip refinement: tapered the upper tail diagonally to a single outline pixel, using existing pink indices 7 and 2. No white added; tail root and all rows below 44 unchanged.
