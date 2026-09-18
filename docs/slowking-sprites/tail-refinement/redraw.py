from pathlib import Path
from PIL import Image
root=Path(__file__).resolve().parents[3]
source=Image.open(Path(__file__).with_name('before.png'))
out=source.copy()
# Remove only the previous exposed tail, preserving the body's edge.
starts={43:45,44:44,45:45,46:45,47:45,48:46,49:46,50:47,51:47,52:48,53:48,54:48}
for y,left in starts.items():
 for x in range(left,64):out.putpixel((x,y),0)
# A rounded, upward-curving tip, continuous outline and a lighter pink ramp.
# 7=highlight, 2=main pink, 3=shadow; the old tail used 5/4 as its main/shadow.
rows={42:(49,'11'),43:(48,'1721'),44:(48,'17721'),45:(47,'177221'),
46:(47,'1772221'),47:(46,'17722231'),48:(46,'17222231'),49:(46,'1222231'),
50:(47,'122231'),51:(47,'12231'),52:(48,'1231'),53:(48,'131'),54:(48,'11')}
for y,(left,row) in rows.items():
 for dx,c in enumerate(row):out.putpixel((left+dx,y),int(c,16))
# Merge the tail root into the flank: replace the two touching outlines
# with a shaded attachment, leaving only the outside contour dark.
for y, row in {48: {45: 3, 46: 2}, 49: {45: 3, 46: 2},
               50: {46: 3, 47: 2}, 51: {46: 3, 47: 2},
               52: {47: 3, 48: 2}, 53: {47: 3, 48: 3}}.items():
 for x, colour in row.items():out.putpixel((x,y),colour)
assert out.crop((0,0,64,42)).tobytes()==source.crop((0,0,64,42)).tobytes()
assert out.crop((0,55,64,64)).tobytes()==source.crop((0,55,64,64)).tobytes()
assert out.getpalette()==source.getpalette()
out.save(root/'graphics/pokemon/slowking/front.png',transparency=0)
out.convert('RGBA').resize((512,512),Image.Resampling.NEAREST).save(Path(__file__).with_name('preview.png'))
