Normal orange slots 6–9 in both Houndour and Houndoom now use Growlithe FRLG normal slots 11–14: highlight 255,197,82; main 238,139,74; shade 197,82,32; deepest 115,57,16. Other palette slots, indices, geometry and shiny palettes remain unchanged. Both front/back previews refreshed.

Front sprite dithering trial: dither/apply.py lightens 27 Houndour and 30 Houndoom interior shadow pixels along transitions between body shades 3/4/5. Outlines, silhouettes, orange/bone details and palettes are unchanged; backs untouched. Comparison shows original left, edited right.

Dithering refinement: light and dark transitions now use only directly adjacent shade boundaries, removing the second-pixel-wide dither band for a subtler result.
