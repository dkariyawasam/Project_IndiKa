// Lossless HGSS canvas/palette conversion. No resampling or generated artwork.
const fs=require('fs');const sharp=require('/Users/deekariyawasam/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/node_modules/sharp');
const root='docs/chingling-sprites/source/',out='graphics/pokemon/chingling/';
(async()=>{
let pal=[[0,0,0]],shiny=[[0,0,0]],frames={};
for(const side of ['front','back']){
 const a=await sharp(root+side+'.png').ensureAlpha().raw().toBuffer(),b=await sharp(root+side+'-shiny.png').ensureAlpha().raw().toBuffer();let pixels=[];
 for(let y=8;y<72;y++)for(let x=8;x<72;x++){
  const p=(y*80+x)*4;if(!a[p+3]){pixels.push(0);continue;}
  const c=Array.from(a.subarray(p,p+3)),s=Array.from(b.subarray(p,p+3));let i=pal.findIndex((v,j)=>j>0&&v.join()==c.join()&&shiny[j].join()==s.join());
  if(i<0){i=pal.length;pal.push(c);shiny.push(s);}pixels.push(i);
 }
 frames[side]=pixels;
}
if(pal.length>16)throw Error('Too many palette pairs: '+pal.length);
console.log('Shared palette entries: '+pal.length);
while(pal.length<16){pal.push([0,0,0]);shiny.push([0,0,0]);}
for(const [name,p] of [['normal',pal],['shiny',shiny]])fs.writeFileSync(out+name+'.pal','JASC-PAL\n0100\n16\n'+p.map(c=>c.join(' ')).join('\n')+'\n');
for(const [side,pixels] of Object.entries(frames)){
 let tiles=[];for(let ty=0;ty<8;ty++)for(let tx=0;tx<8;tx++)for(let y=0;y<8;y++)for(let x=0;x<8;x+=2){const p=(ty*8+y)*64+tx*8+x;tiles.push(pixels[p]|pixels[p+1]<<4);}
 fs.writeFileSync('/tmp/chingling-'+side+'.4bpp',Buffer.from(tiles));
}
})();
