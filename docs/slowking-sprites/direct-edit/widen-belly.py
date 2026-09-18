from pathlib import Path
from PIL import Image

root = Path(__file__).resolve().parents[3]
source = Image.open(root / 'docs/slowking-sprites/direct-edit/before.png')
result = source.copy()
# Expand only the torso beneath the collar. Duplicate central belly pixels
# rather than resampling the face, crown, outlines or feet.
# Three pixels at the widest point, tapering to the unchanged feet.
widths = {44: (1, 0), **dict.fromkeys(range(45, 55), (1, 2)),
          55: (1, 1), 56: (1, 1), 57: (1, 0)}
center = 30
for y, (left, right) in widths.items():
    for x in range(64):
        if x < center - left:
            sx = x + left
        elif x >= center + right:
            sx = x - right
        else:
            sx = center - 1 if x < center else center
        result.putpixel((x, y), source.getpixel((sx, y)))
assert result.crop((0, 0, 64, 43)).tobytes() == source.crop((0, 0, 64, 43)).tobytes()
assert result.crop((0, 59, 64, 64)).tobytes() == source.crop((0, 59, 64, 64)).tobytes()
assert result.getpalette() == source.getpalette()
result.save(root / 'graphics/pokemon/slowking/front.png', transparency=0)
result.convert('RGBA').resize((512, 512), Image.Resampling.NEAREST).save(root / 'docs/slowking-sprites/direct-edit/preview.png')
print('Face, crown, collar, feet and palette unchanged; belly up to 3 pixels wider.')
