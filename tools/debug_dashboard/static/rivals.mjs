let loaded = false;
// Narrative grouping, verified against each map's rival dialogue. Not prerequisite edges.
const storyBranches = [
  ['shared', 'Shared opening', [
    ['PalletTown_ProfessorOaksLab', 'First partner and first battle'],
    ['Route2', 'Introduces the alternative fieldwork routes']]],
  ['league', 'League', [
    ['Route3', 'Sets out to challenge all eight Gyms'],
    ['CeladonCity', 'Erika, evolution by bond and the VS Seeker'],
    ['Route23', 'Eight badges: tests your team before Victory Road'],
    ['PokemonLeague_ChampionsRoom', 'Champion battle'],
    ['IndigoPlateau_Exterior', 'Closing scene / credits appearance']]],
  ['apex', 'Apex', [
    ['ViridianForest', 'Investigates the Tangrowth accounts'],
    ['Route11', 'Investigates Mime Sr. at Diglett’s Cave'],
    ['PokemonTower_1F', 'Investigates the Osscythe accounts']]],
  ['giovanni', 'Giovanni', [
    ['CinnabarIsland', 'Introduces the Mansion and its escaped experiment'],
    ['PokemonMansion_3F', 'Tests your readiness to investigate further'],
    ['SilphCo_7F', 'Connects Team Rocket to capture technology']]]
];
const escape = value => String(value ?? '').replace(/[&<>"']/g,c=>({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[c]));
const pretty = value => value.replaceAll('_',' ').replace(/([a-z])([A-Z])/g,'$1 $2');
export async function mountRivals() {
  if (loaded) return;
  const panel = document.getElementById('panel-rivals');
  panel.textContent = 'Reading rival encounters…';
  let selected = null;
  try {
    const response = await fetch('/api/rivals');
    if (!response.ok) throw Error('Could not load rival encounters');
    const data = await response.json();
    panel.innerHTML = `<article class="card"><h2>Your rival’s story branches</h2><p>Follow the shared opening into League, Apex or Giovanni. Lines group related story beats; they do not impose a play order or indicate live completion. Select an encounter to inspect its scripts below.</p><div id="rival-graph"></div><label>Find a location or branch<input id="rival-search" placeholder="Route 3, Celadon, starter, FLAG…"></label><p id="rival-count"></p></article><div id="rival-encounters"></div>`;
    const entries = new Map(data.encounters.map(e=>[e.map,e]));
    document.getElementById('rival-graph').innerHTML = storyBranches.map(([id,title,nodes])=>`<section class="rival-lane rival-${id}"><h3>${title}</h3><div class="rival-nodes">${nodes.filter(([map])=>entries.has(map)).map(([map,description])=>`<button class="rival-node secondary" data-rival="${map}" aria-pressed="false"><strong>${escape(pretty(map))}</strong><span>${escape(description)}</span><small>${entries.get(map).battles.length?'Battle encounter':'Story encounter'}</small></button>`).join('')}</div></section>`).join('');
    document.getElementById('rival-graph').onclick = event => {
      const button = event.target.closest('[data-rival]');
      if (!button) return;
      selected = selected === button.dataset.rival ? null : button.dataset.rival;
      document.getElementById('rival-search').value = '';
      render();
      if (selected) document.getElementById('rival-encounters').scrollIntoView({behavior:'smooth',block:'start'});
    };
    function render() {
      const query = document.getElementById('rival-search').value.toLowerCase().trim();
      const rows = data.encounters.filter(e=>(!selected || e.map===selected) && (JSON.stringify(e).toLowerCase().includes(query)||pretty(e.map).toLowerCase().includes(query)));
      document.querySelectorAll('[data-rival]').forEach(button=>button.setAttribute('aria-pressed',String(button.dataset.rival===selected)));
      document.getElementById('rival-count').textContent = `${rows.length} of ${data.encounters.length} rival locations`;
      document.getElementById('rival-encounters').innerHTML = rows.map(e=>`<article class="card"><h2>${escape(pretty(e.map))}</h2><p>Rival at <b>(${e.x}, ${e.y})</b> · Object ${escape(e.localId)}</p><p>Hidden when: <code>${escape(e.hiddenBy==='0'?'No visibility flag':e.hiddenBy)}</code></p><p>Interaction: <code>${escape(e.interaction==='0x0'?'Scene-controlled appearance':e.interaction)}</code></p><details><summary>Approach triggers (${e.triggers.length})</summary>${e.triggers.map(t=>`<p><b>(${t.x}, ${t.y})</b> · ${escape(t.var)} = ${escape(t.var_value)}<br><code>${escape(t.script)}</code></p>`).join('')||'<p>No rival coordinate triggers on this map; see the scripted branches below.</p>'}</details><details ${query?'open':''}><summary>Branch conditions (${e.branches.length})</summary><p class="muted">Conditions are listed within their script blocks, in source order. VAR_STARTER_MON branches select battle variants. Movement branches select an approach or exit path.</p>${e.branches.map(b=>`<div class="quest-variant"><small>${escape(b.script)}</small><p><code>${escape(b.condition)}</code></p><small>${escape(b.source)}:${b.line}</small></div>`).join('')||'<p>No conditional branches found in this map’s rival scripts.</p>'}</details><details><summary>Battle variants (${e.battles.length})</summary>${e.battles.map(b=>`<p><code>${escape(b)}</code></p>`).join('')||'<p>No rival battle variants found in these scripts.</p>'}</details><small class="muted">${escape(e.source)}</small></article>`).join('') || '<p class="card">No matching encounters.</p>';
    }
    document.getElementById('rival-search').oninput = () => {selected = null; render();};
    render(); loaded = true;
  } catch (error) {panel.textContent = error.message;}
}
