const fs=require('fs');
for(const name of ['tynamo','eelektrik','eelektross'])for(const side of ['front','back']){
 let b=fs.readFileSync('/tmp/'+name+'-'+side+'.4bpp'),p=Array(4096);const pos=(x,y)=>(Math.floor(y/8)*8+Math.floor(x/8))*64+(y%8)*8+x%8;
 for(let y=0;y<64;y++)for(let x=0;x<64;x++){let n=pos(x,y);p[y*64+x]=(b[n>>1]>>((n&1)*4))&15;}
 let shade=name==='eelektrik'?{1:2,3:2,6:5,8:7,9:7,10:2,11:2,12:5,13:5}:{3:1,4:1,7:6,8:6,9:6,10:6,11:12,13:2,14:2};let count=0;
 if(name==='eelektross'||(name==='eelektrik'&&side==='back'))for(let y=0;y<64;y++)for(let x=0;x<64;x++){
  let c=p[y*64+x];if(!c||shade[c]===undefined)continue;
  let boundary=[[x-1,y],[x+1,y],[x,y-1],[x,y+1]].some(([a,z])=>z===64&&side==='back'?false:a<0||a>=64||z<0||z>=64||p[z*64+a]===0);
  if(boundary){let n=pos(x,y),shift=(n&1)*4;b[n>>1]=(b[n>>1]&~(15<<shift))|(shade[c]<<shift);count++;}
 }
 fs.writeFileSync('/tmp/'+name+'-'+side+'.4bpp',b);console.log(name,side,count,'edge pixels repaired');
}
