# Tynamo line shading pass

The six battle views retain their original 64×64 indexed canvases, silhouettes, transparency and facial feature colours. Built-in image generation supplied a shading reference; `convert.py` transfers only material-compatible interior lighting pixels onto the original assets. Generated glow and altered edges are discarded. Eelektrik and Eelektross gain a restrained teal intermediate highlight in unused palette slots, with corresponding shiny highlights. Tynamo retains its existing palette.

Edit prompt: Edit the sprite sheet's internal shading only to match FireRed/LeafGreen GBA pixel art; preserve positions, silhouettes, poses, proportions, eyes, teeth, fins and markings. Use restrained upper-left teal highlights, clearer midtones, smaller deep shadows, and readable pale Tynamo shading. No gradients, antialiasing, new anatomy or added white shiny spots.

`before/` retains originals. `generated-reference.png` is the built-in image-tool result, not a game asset. `comparison.png` shows before/after front and back views. Artwork and shared normal/shiny palettes are installed in `graphics/pokemon/{tynamo,eelektrik,eelektross}`. Icons remain unchanged.

Validation: identical transparency masks, 64×64 dimensions, indexed palette values below 16, main ROM build. No live battle test performed.
