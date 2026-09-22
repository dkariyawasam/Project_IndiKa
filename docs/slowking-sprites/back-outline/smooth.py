from pathlib import Path
from PIL import Image
root=Path(__file__).resolve().parents[3]
folder=Path(__file__).resolve().parent
im=Image.open(folder/'before.png');out=im.copy()
# Continue the shoulder contour into the flank instead of indenting sharply
# for two rows and then stepping outward at the tail attachment.
for y in range(41,45):
 old_edge=43 if y<43 else 42
 for x in range(old_edge,44):out.putpixel((x,y),6)
 out.putpixel((44,y),1)
assert out.getpalette()==im.getpalette()
assert out.crop((0,0,64,41)).tobytes()==im.crop((0,0,64,41)).tobytes()
assert out.crop((0,45,64,64)).tobytes()==im.crop((0,45,64,64)).tobytes()
out.save(root/'graphics/pokemon/slowking/front.png',transparency=0)
out.convert('RGBA').resize((384,384),Image.Resampling.NEAREST).save(folder/'preview.png')
