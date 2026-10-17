let mounted=false,data,filtered=[],selected,revision=0,saveRequest;
const drafts=new Map(),images=new Map(),fields=["frontOffset","backOffset","elevation"];
const $=id=>document.getElementById(id);
async function reload(){try{const r=await fetch('/api/pokedex');if(!r.ok)throw Error('Could not load Pokédex');data=await r.json();drafts.clear();images.clear();filter();}catch(e){$('dex-status').textContent=e.message;}}
function filter(){const q=$('dex-search').value.toLowerCase();filtered=data.species.filter(e=>(e.name+' '+e.key+' '+e.id).toLowerCase().includes(q));const old=selected?.key;$('dex-species').replaceChildren(...filtered.map(e=>{const o=document.createElement('option');o.value=e.key;o.textContent=`${e.name} · ${e.key}`;return o;}));if(filtered.some(e=>e.key===old))$('dex-species').value=old;select();}
function select(){selected=filtered.find(e=>e.key===$('dex-species').value);$('dex-count').textContent=`${filtered.length} of ${data.species.length} species`;$('dex-prev').disabled=!selected||filtered.indexOf(selected)===0;$('dex-next').disabled=!selected||filtered.indexOf(selected)===filtered.length-1;syncOffsets();draw();showDetails();}
function syncOffsets(){const draft=selected&&(drafts.get(selected.key)||selected);for(const field of fields){$("dex-"+field).value=draft?.[field]??0;$("dex-"+field).disabled=!selected;}$("dex-save").disabled=!selected;$("dex-reset").disabled=!selected;}
async function draw(){
 const rev=++revision,canvas=$('dex-battle'),ctx=canvas.getContext('2d');ctx.imageSmoothingEnabled=false;ctx.fillStyle='#e8efe4';ctx.fillRect(0,0,240,160);ctx.fillStyle='#fff';ctx.fillRect(0,112,240,48);ctx.strokeStyle='#71866f';ctx.strokeRect(.5,112.5,239,47);ctx.fillStyle='#35463a';ctx.font='8px monospace';ctx.fillText('BATTLE POSITION PREVIEW',8,129);
 if(!selected){$('dex-status').textContent='No matching species.';$('dex-details').textContent='';$('dex-positions').textContent='';for(const v of ['front','back'])$('dex-'+v).getContext('2d').clearRect(0,0,64,64);return;}
 const mon={...selected,...drafts.get(selected.key)},shiny=$('dex-shiny').checked,mode=$('dex-mode').value,guide=$('dex-guides').checked;
 $('dex-status').textContent='Loading artwork…';$('dex-details').textContent=`Front Y offset: ${mon.frontOffset} · Back Y offset: ${mon.backOffset} · Enemy elevation: ${mon.elevation}px`;
 function load(view){const key=mon.key+view+shiny;if(images.has(key))return images.get(key);const promise=new Promise((resolve,reject)=>{const im=new Image();im.onload=()=>resolve(im);im.onerror=()=>reject(Error('Missing '+view+' artwork'));im.src='/pokemon-preview.png?species='+encodeURIComponent(mon.key)+'&view='+view+'&shiny='+(shiny?1:0)+'&v='+Date.now();});images.set(key,promise);return promise;}
 try{
  const [front,back]=await Promise.all([load('front'),load('back')]);if(rev!==revision)return;
  for(const [view,im] of [['front',front],['back',back]]){const c=$('dex-'+view),g=c.getContext('2d');g.clearRect(0,0,64,64);g.imageSmoothingEnabled=false;g.drawImage(im,0,0,64,64,0,0,64,64);}
  const slots=mode==='double'?[1,3,0,2]:[1,0],positions=[];
  for(const slot of slots){const enemy=slot%2===1,[x,base]=data.anchors[mode][slot],y=(base+(enemy?mon.frontOffset-mon.elevation:mon.backOffset)+256)%256,im=enemy?front:back;
   ctx.save();ctx.beginPath();ctx.rect(0,0,240,112);ctx.clip();ctx.drawImage(im,0,0,64,64,x-32,y-32,64,64);
   if(guide){ctx.strokeStyle=enemy?'#b35249':'#2d69ae';ctx.strokeRect(x-32+.5,y-32+.5,63,63);ctx.beginPath();ctx.moveTo(x-4,y+.5);ctx.lineTo(x+4,y+.5);ctx.moveTo(x+.5,y-4);ctx.lineTo(x+.5,y+4);ctx.stroke();}
   ctx.restore();positions.push(`${enemy?'Front':'Back'}${mode==='double'?' '+(slot<2?'1':'2'):''}: centre (${x}, ${y}), top-left (${x-32}, ${y-32})`);
  }
  $('dex-positions').textContent=positions.join(' · ');$('dex-status').textContent=mon.issues.length?mon.issues.join(' · '):`${mon.name} · ${shiny?'Shiny':'Normal'} palette`;
 }catch(e){if(rev===revision){$('dex-status').textContent=e.message;for(const v of ['front','back'])$('dex-'+v).getContext('2d').clearRect(0,0,64,64);$('dex-positions').textContent='';}}
}
export async function mountPokedex(request){saveRequest=request;if(mounted)return;mounted=true;
 $('panel-pokedex').innerHTML=`<article class="card"><h2>Pokédex sprite positions</h2><p>Current source artwork at the game's single/double-battle coordinates. First sprite frame and registered palettes; a neutral 240 × 160 guide replaces the battle scenery. Transformations, personality patterns and battle effects are not simulated.</p><div class="ow-controls"><label>Search Pokémon<input id="dex-search" type="search" placeholder="Name, species key or ID"></label><label>Species<select id="dex-species"></select></label><button id="dex-prev">Previous</button><button id="dex-next">Next</button><button id="dex-reload">Reload source</button><span id="dex-count"></span></div></article><article class="card dex-preview"><div class="ow-controls"><label>Battle layout<select id="dex-mode"><option value="single">Single battle</option><option value="double">Double battle</option></select></label><label><input id="dex-shiny" type="checkbox"> Shiny palette</label><label><input id="dex-guides" type="checkbox" checked> Sprite bounds and centres</label></div><div class="ow-controls"><label>Front Y offset<input id="dex-frontOffset" type="number" required min="0" max="255" step="1"></label><label>Back Y offset<input id="dex-backOffset" type="number" required min="0" max="255" step="1"></label><label>Enemy elevation<input id="dex-elevation" type="number" required min="0" max="255" step="1"></label><button id="dex-reset">Reset preview</button><button id="dex-save">Save offsets to source</button></div><p>Higher Y offsets move sprites down; elevation lifts the opponent. Values are unsigned bytes (0–255). Castform edits affect its normal form.</p><p id="dex-save-status" role="status"></p><p id="dex-status" role="status">Loading…</p><canvas id="dex-battle" width="240" height="160" aria-label="Battle sprite placement"></canvas><p id="dex-details"></p><p id="dex-positions"></p><div class="dex-art"><div><h3>Front · opponent</h3><canvas id="dex-front" width="64" height="64" aria-label="Full front sprite"></canvas></div><div><h3>Back · player</h3><canvas id="dex-back" width="64" height="64" aria-label="Full back sprite"></canvas></div></div></article><article class="card" id="dex-info"></article>`;
 $('dex-search').addEventListener('input',()=>{if(data)filter();});$('dex-species').addEventListener('change',select);
 for(const id of ['dex-mode','dex-shiny','dex-guides'])$(id).addEventListener('change',draw);
 for(const [id,d] of [['dex-prev',-1],['dex-next',1]])$(id).addEventListener('click',()=>{const next=filtered[filtered.indexOf(selected)+d];if(next){$('dex-species').value=next.key;select();}});
 for(const field of fields)$('dex-'+field).addEventListener('input',()=>{if(!selected)return;const value=Number($('dex-'+field).value);if(!$('dex-'+field).value||!Number.isInteger(value)||value<0||value>255)return;drafts.set(selected.key,{...(drafts.get(selected.key)||Object.fromEntries(fields.map(f=>[f,selected[f]]))),[field]:value});$('dex-save-status').textContent='Unsaved preview';draw();});
 $('dex-reset').onclick=()=>{if(!selected)return;drafts.delete(selected.key);syncOffsets();draw();$('dex-save-status').textContent='Preview reset to source values.';};
 $('dex-save').onclick=async()=>{if(!selected)return;const mon=selected,values={...(drafts.get(mon.key)||Object.fromEntries(fields.map(f=>[f,mon[f]])))};if(fields.some(f=>!$('dex-'+f).checkValidity()))return;$('dex-save').disabled=true;try{const result=await saveRequest('/api/pokedex/offsets',{species:mon.key,values,expected:Object.fromEntries(fields.map(f=>[f,mon[f]]))});Object.assign(mon,values);$('dex-save-status').textContent=mon.name+': '+result.message;}catch(e){$('dex-save-status').textContent=e.message;}finally{$('dex-save').disabled=!selected;}};
 $('dex-reload').addEventListener('click',reload);await reload();
}

let detailRevision=0;
async function showDetails(){
 const rev=++detailRevision,host=$('dex-info');const oldAudio=document.querySelector('#dex-audio');if(oldAudio)oldAudio.pause();host.replaceChildren();if(!selected)return;
 const key=selected.key;
 try{
  const r=await fetch('/api/pokemon-details?species='+encodeURIComponent(key));if(!r.ok)throw Error('Could not load species details');const info=await r.json();if(rev!==detailRevision)return;
  const heading=text=>{const h=document.createElement('h3');h.textContent=text;host.append(h);};
  const paragraph=text=>{const p=document.createElement('p');p.textContent=text;host.append(p);return p;};
  heading('Pokédex entry');paragraph(`${info.category} POKéMON · ${Number(info.height)/10} m · ${Number(info.weight)/10} kg`);paragraph(info.description).style.whiteSpace='pre-line';
  heading('Official games · regional encounters');
  const official=info.officialEncounters;
  const readable=value=>value.split('-').map(word=>word.charAt(0).toUpperCase()+word.slice(1)).join(' ');
  const gameNames={firered:'FireRed',leafgreen:'LeafGreen',heartgold:'HeartGold',soulsilver:'SoulSilver',xd:'XD: Gale of Darkness', 'lets-go-pikachu':"Let’s Go, Pikachu!",'lets-go-eevee':"Let’s Go, Eevee!"};
  if(official){
   paragraph('Grouped by the actual encounter location, including Kanto visits in Johto games. Records include wild encounters, gifts and trades where supplied. Evolution, breeding and transfers are not inferred. No record does not mean unobtainable.');
   if(official.status==='custom')paragraph('Custom species or form — no official-game counterpart.');
   else if(official.status==='unmapped')paragraph('This species has not been mapped to the official reference.');
   else {
    const grid=document.createElement('div');grid.className='dex-regions';host.append(grid);
    for(const [title,regions] of [['Kanto',official.regions.filter(r=>r.region==='kanto')],['Johto',official.regions.filter(r=>r.region==='johto')],['Other regions',official.regions.filter(r=>!['kanto','johto'].includes(r.region))]]){
     const section=document.createElement('section'),h=document.createElement('h4');h.textContent=title;section.append(h);grid.append(section);
     if(!regions.length){const p=document.createElement('p');p.textContent='No direct encounter records in this snapshot.';section.append(p);}
     for(const region of regions){
      if(title==='Other regions'){const h=document.createElement('h4');h.textContent=readable(region.region);section.append(h);}
      for(const game of region.games){
       const detail=document.createElement('details'),summary=document.createElement('summary');summary.textContent=gameNames[game.game]||readable(game.game);detail.append(summary);
       const list=document.createElement('ul');
       for(const encounter of game.encounters){const li=document.createElement('li');li.textContent=readable(encounter.location)+' · '+readable(encounter.method);list.append(li);}
       detail.append(list);section.append(detail);
      }
     }
    }
   }
   const coverage=document.createElement('details'),summary=document.createElement('summary');summary.textContent='Source and coverage limitations';coverage.append(summary);
   const note=document.createElement('p');note.textContent='PokéAPI community encounter snapshot, retrieved '+official.retrieved+'. Coverage varies by game and is not exhaustive. Games without encounter data, including Legends: Arceus, Scarlet/Violet and Brilliant Diamond/Shining Pearl, are not marked absent. Included games: '+official.coverage.map(g=>gameNames[g]||readable(g)).join(', ')+'.';coverage.append(note);
   const link=document.createElement('a');link.href=official.source;link.textContent='PokéAPI source tables';link.target='_blank';link.rel='noopener noreferrer';coverage.append(link);host.append(coverage);
  }else paragraph('Official encounter reference unavailable. Restart the dashboard server to load it.');
  heading('Full cry');const audio=document.createElement('audio');audio.id='dex-audio';audio.controls=true;audio.preload='metadata';audio.src='/pokemon-cry.wav?species='+encodeURIComponent(key);host.append(audio);const play=document.createElement('button');play.textContent='Play full cry';play.onclick=()=>{audio.currentTime=0;audio.play().catch(()=>{play.textContent='Audio unavailable';});};host.append(play);paragraph('Full source recording · '+info.cry);
  heading('Footprint');const footprint=document.createElement('img');footprint.src='/pokemon-footprint.png?species='+encodeURIComponent(key);footprint.alt=key+' footprint';footprint.className='dex-footprint';host.append(footprint);
  heading('Sprite palettes');for(const kind of ['normal','shiny']){paragraph(kind+' · '+info.palettes[kind].path);const row=document.createElement('div');row.className='dex-swatches';info.palettes[kind].colors.forEach((color,i)=>{const chip=document.createElement('span');chip.style.backgroundColor=color;chip.title=`Index ${i}: ${color}${i===0?' (transparent)':''}`;chip.textContent=i;row.append(chip);});host.append(row);}
  heading('Evolution tree');const pretty=s=>s.replace(/^ITEM_/,'').replaceAll('_',' ').toLowerCase().replace(/\b\w/g,c=>c.toUpperCase());
  const label=e=>{const m=e.method.replace('EVO_','').replaceAll('_',' ').toLowerCase(),p=e.parameter;return m+(p==='0'?'':' · '+pretty(p));};
  const edges=info.evolutions;const roots=[...new Set(edges.map(e=>e.source))].filter(k=>!edges.some(e=>e.target===k));
  function branch(k,seen=new Set()){const li=document.createElement('li');const b=document.createElement('button');b.textContent=pretty(k)+(k===key?' ◀':'');b.onclick=()=>{if(!data.species.some(e=>e.key===k))return;$('dex-search').value='';filter();$('dex-species').value=k;select();};li.append(b);if(seen.has(k))return li;const next=edges.filter(e=>e.source===k);if(next.length){const ul=document.createElement('ul');for(const e of next){const node=branch(e.target,new Set([...seen,k]));const condition=document.createElement('span');condition.textContent=' → '+label(e)+' → ';node.prepend(condition);ul.append(node);}li.append(ul);}return li;}
  if(!edges.length)paragraph('No evolution-table links for this species.');else{const tree=document.createElement('ul');tree.className='dex-tree';for(const root of roots)tree.append(branch(root));host.append(tree);}
  paragraph('Evolution table conditions shown; scripted transformations and special encounter rewards may use additional rules.');
 }catch(e){if(rev===detailRevision)host.textContent=e.message;}
}
