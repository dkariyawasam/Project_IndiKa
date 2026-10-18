import unittest
from palette_style import colors,metrics,compare,gba
class PaletteStyleTests(unittest.TestCase):
    def test_unused_and_background_not_counted(self):
        pal=[[255,0,255],[100,100,100],[255,0,0]]
        cs=colors(pal,{1:10})
        self.assertEqual(len(cs),1)
        self.assertIsNone(metrics(cs)['saturation'])
    def test_muted_hue_flag(self):
        cs=colors([[0,0,0],[160,120,120]],{1:100})
        refs=[dict(c,key='TEST') for c in colors([[0,0,0],[192,112,112]],{1:100})]*5
        self.assertEqual(len(compare(cs,refs)),1)
        self.assertEqual(compare(refs,refs),[])
    def test_gba_equivalent(self):
        self.assertEqual(gba([255,128,7]),gba([248,135,0]))

    def test_approval_backup_and_stale_preview(self):
        import tempfile,json,base64,io
        from pathlib import Path
        from unittest.mock import patch
        from PIL import Image
        import palette_style as mod
        with tempfile.TemporaryDirectory() as tmp:
            root=Path(tmp);path=root/'normal.pal'
            ref=root/'tools/debug_dashboard/firered_palette_reference.json';ref.parent.mkdir(parents=True)
            ref.write_text(json.dumps(dict(species=[dict(key='REF',counts={'1':100},normal=[[0,0,0],[160,112,112]]+[[0,0,0]]*14)])))
            pal=[[0,0,0],[160,120,120]]+[[0,0,0]]*14
            original='JASC-PAL\n0100\n16\n'+'\n'.join(' '.join(map(str,c)) for c in pal)+'\n';path.write_text(original)
            im=Image.new('P',(64,64),1);im.putpalette([c for rgb in pal for c in rgb]+[0]*720);im.save(root/'front.png');im.save(root/'back.png')
            mons=[dict(key=k,name=k,normal='normal.pal',front='front.png',back='back.png') for k in ('TEST','SHARED')]
            scan=dict(rows=[dict(key='TEST',status='Review',muted=[dict(index=1,referenceSaturation=.75)])])
            with patch.object(mod,'check',return_value=scan),patch.object(mod,'catalog',return_value=dict(species=mons)):
                p=mod.proposal(root,[],'TEST','normal');self.assertEqual(len(p['previews']),2)
                shown=Image.open(io.BytesIO(base64.b64decode(p['previews'][0]['views'][0]['after'].split(',')[1])))
                expected=tuple(round(v*255) for v in mod.gba(p['palette'][1]))
                self.assertEqual(shown.getpixel((1,1))[:3],expected)
                result=mod.apply_proposal(root,[],dict(key='TEST',variant='normal',revision=p['revision']))
                self.assertEqual((Path(result['backup'])/'normal.pal').read_text(),original)
                self.assertEqual(list(map(int,path.read_text().splitlines()[4].split())),p['palette'][1])
                with self.assertRaises(ValueError):mod.apply_proposal(root,[],dict(key='TEST',variant='normal',revision=p['revision']))

    def test_pastel_highlights_and_ramp_protected(self):
        from palette_style import recommend,lightness
        pal=[[0,0,0],[248,216,224],[224,144,168],[160,80,104]]
        refs=[dict(c,key='REF') for c in colors([[0,0,0],[248,144,168],[224,136,160],[160,72,96]],{1:10,2:20,3:10})]
        out,changes=recommend(pal,{1,2,3},refs)
        self.assertEqual(out[1],pal[1])
        self.assertGreater(lightness(out[1]),lightness(out[2]))
        self.assertGreater(lightness(out[2]),lightness(out[3]))
        self.assertEqual(len(set(tuple(c) for c in out)),len(out))
