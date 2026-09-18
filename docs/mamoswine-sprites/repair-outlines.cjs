// Preserve indexed pixels and shape; restore one-pixel material-aware outer edging.
const fs=require('fs');
for(const side of ['front','back']){
 let b=fs.readFileSync('/tmp/mamoswine-frlg-'+side+'.4bpp'),p=Array(4096);
 function offset(x,y){return (Math.floor(y/8)*8+Math.floor(x/8))*64+(y%8)*8+x%8;}
 for(let y=0;y<64;y++)for(let x=0;x<64;x++){let n=offset(x,y);p[y*64+x]=(b[n>>1]>>((n&1)*4))&15;}
 const edge={1:3,2:3,4:4,5:4,7:7,8:7,9:4,10:4,11:3,12:3,13:3,14:15,15:15,3:3,6:6};let count=0;
 for(let y=0;y<64;y++)for(let x=0;x<64;x++){
  const c=p[y*64+x];if(!c)continue;
  const boundary=[[x-1,y],[x+1,y],[x,y-1],[x,y+1]].some(([a,z])=>{if(z===64&&side==='back')return false;return a<0||a>=64||z<0||z>=64||p[z*64+a]===0;});
  if(boundary&&edge[c]!==c){let n=offset(x,y),shift=(n&1)*4;b[n>>1]=(b[n>>1]&~(15<<shift))|(edge[c]<<shift);count++;}
 }
 fs.writeFileSync('/tmp/mamoswine-repaired-'+side+'.4bpp',b);console.log(side+': repaired '+count+' edge pixels');
}
