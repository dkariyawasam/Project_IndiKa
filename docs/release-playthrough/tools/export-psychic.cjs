const sharp=require('/Users/deekariyawasam/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/node_modules/sharp');
const fs=require('fs');
(async()=>{
const input='docs/release-playthrough/psychic-f-candidate.png';
const {data,info}=await sharp(input).ensureAlpha().raw().toBuffer({resolveWithObject:true});
const frames=[];
for(let i=0;i<9;i++){
 let x0=Math.floor(i*info.width/9),x1=Math.floor((i+1)*info.width/9),left=x1,right=x0,top=info.height,bottom=0;
 for(let y=0;y<info.height;y++)for(let x=x0;x<x1;x++)if(data[(y*info.width+x)*4+3]>200){left=Math.min(left,x);right=Math.max(right,x);top=Math.min(top,y);bottom=Math.max(bottom,y)}
 const r=await sharp(input).extract({left,top,width:right-left+1,height:bottom-top+1}).resize(16,28,{fit:'inside',kernel:'nearest'}).ensureAlpha().raw().toBuffer({resolveWithObject:true});
 for(let j=3;j<r.data.length;j+=4)r.data[j]=r.data[j]>200?255:0;
 frames.push({input:await sharp(r.data,{raw:r.info}).png().toBuffer(),left:i*16+Math.floor((16-r.info.width)/2),top:31-r.info.height});
}
await sharp({create:{width:144,height:32,channels:4,background:{r:0,g:0,b:0,alpha:0}}}).composite(frames).png({palette:true,colours:16,dither:0}).toFile('/tmp/kanto-release-playtest/psychic_f.png');
await sharp('/tmp/kanto-release-playtest/psychic_f.png').resize(1152,256,{kernel:'nearest'}).png().toFile('/tmp/kanto-release-playtest/psychic_f-preview.png');
})();
