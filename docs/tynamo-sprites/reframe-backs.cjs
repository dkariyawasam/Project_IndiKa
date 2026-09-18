const fs=require('fs'),sharp=require('/Users/deekariyawasam/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/node_modules/sharp');
const specs={tynamo:{old:[[29,40,38,17,38,17],[29,39,38,18,38,18]],crop:[29,39,38,18,56,27],top:37},eelektrik:{old:[[23,16,49,63,49,63],[23,15,50,67,48,64]],crop:[23,15,50,43,64,55],top:9},eelektross:{old:[[10,20,77,59,64,49],[7,17,83,62,64,48]],crop:[32,19,52,52,64,64],top:0}};
(async()=>{for(const [name,spec] of Object.entries(specs)){
 let pal=[null];
 async function load(side,rect,shiny){let [left,top,width,height,w,h]=rect;return sharp(`docs/${name}-sprites/source/${side}${shiny?'-shiny':''}.png`).extract({left,top,width,height}).resize(w,h,{kernel:'nearest'}).ensureAlpha().raw().toBuffer();}
 for(const [i,side] of ['front','back'].entries()){
  const a=await load(side,spec.old[i],false),b=await load(side,spec.old[i],true);
  for(let p=0;p<a.length;p+=4){if(!a[p+3])continue;const key=a.subarray(p,p+3).join()+','+b.subarray(p,p+3).join();if(!pal.includes(key))pal.push(key);}
 }
 const a=await load('back',spec.crop,false),b=await load('back',spec.crop,true),w=spec.crop[4],h=spec.crop[5],left=Math.floor((64-w)/2);let pixels=Array(4096).fill(0);
 for(let y=0;y<h;y++)for(let x=0;x<w;x++){let p=(y*w+x)*4;if(!a[p+3])continue;let key=a.subarray(p,p+3).join()+','+b.subarray(p,p+3).join(),i=pal.indexOf(key);if(i<1)throw Error('Unmapped original colour '+name+' '+key);pixels[(spec.top+y)*64+left+x]=i;}
 const shade=name==='tynamo'?{1:4,2:4,3:4,6:4,7:8,9:4,10:4,11:8}:name==='eelektrik'?{1:2,3:2,6:5,8:7,9:7,10:2,11:2,12:5,13:5}:{3:1,4:1,7:6,8:6,9:6,10:6,11:12,13:2,14:2};const original=pixels.slice();
 for(let y=0;y<64;y++)for(let x=0;x<64;x++){let c=original[y*64+x];if(!c||shade[c]===undefined)continue;let boundary=[[x-1,y],[x+1,y],[x,y-1],[x,y+1]].some(([a,z])=>z===64?false:a<0||a>=64||z<0||!original[z*64+a]);if(boundary)pixels[y*64+x]=shade[c];}
 let tiles=[];for(let ty=0;ty<8;ty++)for(let tx=0;tx<8;tx++)for(let y=0;y<8;y++)for(let x=0;x<8;x+=2){let p=(ty*8+y)*64+tx*8+x;tiles.push(pixels[p]|pixels[p+1]<<4);}fs.writeFileSync('/tmp/'+name+'-back-reframed.4bpp',Buffer.from(tiles));console.log(name,'reframed');
}})();
