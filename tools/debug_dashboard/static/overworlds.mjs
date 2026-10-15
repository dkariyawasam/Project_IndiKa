let mounted=false, entries=[], cards=[], images=new Map(), paused=false, speed=1, clock=0, previous=0;
const groups=['Gym Leaders & Elite Four','Civilians','Trainer Classes','Pokémon','Apex','Team Rocket','Players & Named Trainers','Objects'];
const directions=['south','north','west','east','rechallenge'];
const panel=()=>document.getElementById('panel-overworlds');
function picture(path){
 if(!images.has(path)){const im=new Image();im.src='/overworld-sheet.png?path='+encodeURIComponent(path)+'&v='+Date.now();images.set(path,im);}return images.get(path);
}
function render(){
 const query=document.getElementById('ow-search').value.toLowerCase();
 const selected=document.getElementById('ow-group').value;
 const list=entries.filter(e=>(!selected||e.group===selected)&&(e.name+' '+e.id+' '+e.group).toLowerCase().includes(query));
 const grid=document.getElementById('ow-grid');grid.replaceChildren();cards=[];
 document.getElementById('ow-count').textContent=`${list.length} of ${entries.length} overworlds`;
 for(const group of groups){
  const members=list.filter(e=>e.group===group);if(!members.length)continue;
  const section=document.createElement('section');section.className='ow-group';
  const heading=document.createElement('h2');heading.textContent=`${group} (${members.length})`;
  const groupGrid=document.createElement('div');groupGrid.className='ow-grid';section.append(heading,groupGrid);grid.append(section);
 for(const entry of members){
  const card=document.createElement('article');card.className='card ow-card';
  const title=document.createElement('h3');title.textContent=entry.name;card.append(title);
  const row=document.createElement('div');row.className='ow-directions';card.append(row);
  for(const direction of (entry.group==='Objects'?['front']:directions)){const col=document.createElement('div'),label=document.createElement('small'),canvas=document.createElement('canvas');label.textContent=direction;canvas.width=64;canvas.height=64;canvas.setAttribute('aria-label',entry.name+' '+direction+' preview');col.append(canvas,label);row.append(col);cards.push({entry,direction,canvas,card});}
  const detail=document.createElement('small');detail.textContent=entry.issues.length?entry.issues.join(' · '):`${entry.frames.length} frame definitions · ${entry.table.replace('sAnimTable_','')}`;detail.className=entry.issues.length?'ow-warning':'muted';card.append(detail);groupGrid.append(card);
 }
 }
}
function draw(item){
 const {entry,direction,canvas}=item,ctx=canvas.getContext('2d');ctx.clearRect(0,0,64,64);ctx.imageSmoothingEnabled=false;
 const seq=direction==='front'?[{frame:0,duration:1,flip:false,vflip:false}]:direction==='rechallenge'?entry.rechallenge:entry.walks[direction];
 if(!seq?.length){ctx.fillStyle='#d77';ctx.font='10px sans-serif';ctx.fillText(direction==='rechallenge'?'No pose':'No walk',8,33);return;}
 let t=clock%(seq.reduce((s,f)=>s+f.duration,0)||1),step=seq[0];
 for(const f of seq){step=f;if(t<f.duration)break;t-=f.duration;}
 // Inspection loop alternates standing/raised pose; map jump/swim motion is not simulated.
 if(direction==='rechallenge'&&clock%120<60)step={frame:0,flip:false,vflip:false};
 const frame=entry.frames[step.frame];if(!frame){ctx.fillStyle='#d77';ctx.fillText('Missing frame',0,33);return;}
 const im=picture(frame.path);if(!im.complete||!im.naturalWidth)return;
 const cols=Math.floor(im.naturalWidth/frame.width);if(!cols||frame.index>=cols*Math.floor(im.naturalHeight/frame.height)){ctx.fillStyle='#d77';ctx.fillText('Missing frame',0,33);return;}
 const scale=Math.min(2,64/frame.width,64/frame.height),w=frame.width*scale,h=frame.height*scale;
 ctx.save();ctx.translate(32,64-h/2);ctx.scale(step.flip?-1:1,step.vflip?-1:1);
 ctx.drawImage(im,(frame.index%cols)*frame.width,Math.floor(frame.index/cols)*frame.height,frame.width,frame.height,-w/2,-h/2,w,h);ctx.restore();
}
function animate(now){
 if(previous&&!paused&&!panel().hidden&&!document.hidden)clock+=Math.min(now-previous,100)*60/1000*speed;
 previous=now;
 if(!panel().hidden){for(const item of cards){const rect=item.card.getBoundingClientRect();if(rect.bottom>=0&&rect.top<=innerHeight)draw(item);}}
 requestAnimationFrame(animate);
}
export async function mountOverworlds(){
 if(mounted)return;mounted=true;
 panel().innerHTML=`<article class="card"><h2>Overworld animations</h2><p>Source artwork with declared walking frames and timing. Four directions loop together. Objects show only their front sprite. Rechallenge alternates standing and the declared raised-hand pose for comparison; map jumping/swimming is not simulated. Missing or repeated frames remain visible; runtime lighting and player accent colours are not simulated.</p><div class="ow-controls"><label>Find an overworld<input id="ow-search" type="search" placeholder="Name or graphics ID"></label><label>Group<select id="ow-group"><option value="">All groups</option>${groups.map(g=>`<option>${g}</option>`).join('')}</select></label><label>Speed<select id="ow-speed"><option value="0.5">Half speed</option><option value="1" selected>Normal</option><option value="2">Double speed</option></select></label><button id="ow-pause">Pause</button><button id="ow-reload">Reload source sheets</button><span id="ow-count" role="status">Loading…</span></div></article><div id="ow-grid"></div>`;
 async function reload(){const status=document.getElementById('ow-count');status.textContent='Loading…';try{const r=await fetch('/api/overworlds');if(!r.ok)throw Error('Could not load overworlds');entries=await r.json();images.clear();clock=0;render();}catch(e){status.textContent=e.message;}}
 document.getElementById('ow-group').addEventListener('change',render);
 document.getElementById('ow-search').addEventListener('input',render);
 document.getElementById('ow-speed').addEventListener('change',e=>speed=Number(e.target.value));
 document.getElementById('ow-pause').addEventListener('click',e=>{paused=!paused;e.target.textContent=paused?'Play':'Pause';});
 document.getElementById('ow-reload').addEventListener('click',reload);
 await reload();requestAnimationFrame(animate);
}
