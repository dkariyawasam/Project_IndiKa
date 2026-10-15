const escape = value => String(value ?? '').replace(/[&<>"']/g, c => ({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[c]));
let loaded = false;
export async function mountQuests() {
  if (loaded) return;
  loaded = true;
  const panel = document.getElementById('panel-quests');
  panel.innerHTML = '<p role="status">Reading the logbook…</p>';
  try {
    const response = await fetch('/api/quests');
    const data = await response.json();
    if (!response.ok) throw Error(data.error || 'Cannot read quests');
    panel.innerHTML = `<article class="card"><p class="eyebrow">FIELD GUIDE / LOGBOOK</p><h2>Quests & research</h2><p>Every quest, research objective, trade offer and gift reminder defined in the game.</p><p class="quest-summary">${data.counts.main} main quests · ${data.counts.objectives} objectives · ${data.counts.promises} field promises</p><p class="muted">Source catalogue, including undiscovered quests. This is not your save’s live progress. Read from the current repository; rebuild the ROM after changing game source.</p><label>Search quests, locations or objectives<input id="quest-search" type="search" placeholder="Porygon, Design, Route 20…"></label><p id="quest-results" role="status"></p></article><div id="quest-list"></div><article class="card"><h2>Nature of Evolution counters</h2><p>Each distinct milestone counts once. Two complete a category, but the counter keeps increasing afterwards. These are supported milestones, not live save totals.</p>${data.research.map(category=>`<details><summary>${escape(category.name)} · ${category.milestones.length} milestones · target ${category.target}</summary><div class="quest-objectives">${category.milestones.map(m=>`<section class="quest-objective"><h3>${escape(m.name)}</h3><small>${escape(m.kind)}</small><code>${escape(m.flag)}</code></section>`).join('')}</div></details>`).join('')}</article><article class="card"><h2>Progression rules</h2><div id="quest-notes"></div></article><article class="card"><details><summary>Dynamic objective dialogue (${data.variants.length} variants)</summary><p class="muted">Text variants used by the game's objective reader. The game selects these using story flags, trial phases and caught species.</p>${data.variants.map(v=>`<div class="quest-variant"><p>${escape(v.text)}</p><code>${escape(v.key)}</code></div>`).join('')}</details></article>`;
    document.getElementById('quest-notes').innerHTML = data.notes.split(/\n\s*\n/).filter(p=>!p.startsWith('# ')).map(p=>p.startsWith('## ')?`<h3>${escape(p.slice(3))}</h3>`:`<p>${escape(p.replaceAll('\n',' '))}</p>`).join('');
    function render() {
      const query = document.getElementById('quest-search').value.toLowerCase().trim();
      const matches = data.quests.filter(q => JSON.stringify(q).toLowerCase().includes(query) || (q.id === 0 && 'porygon research'.includes(query)));
      document.getElementById('quest-results').textContent = `${matches.length} of ${data.counts.main} quests`;
      document.getElementById('quest-list').innerHTML = matches.map(q=>`<article class="card quest-card"><div class="card-heading"><h2>${escape(q.name)}</h2><span class="pill">${escape(q.location)}</span></div><p>${escape(q.description)}</p><details ${query?'open':''}><summary>${q.objectives.length ? `${q.objectives.length} ${q.id===7?'promises':'objectives'}` : 'Reward'} & completion text</summary><p class="quest-completion"><b>When complete:</b> ${escape(q.completed)}</p>${q.id===0?'<p class="quest-research">Porygon creation and upgrades belong to Evolution by Design. See the progression rules and dynamic objective dialogue below.</p>':''}<div class="quest-objectives">${q.objectives.map(o=>`<section class="quest-objective"><h3>${escape(o.name)}</h3><p>${escape(o.description)}</p><small>${escape(o.location)}</small>${o.discovery?`<details><summary>Discovery & completion conditions</summary><p>Visible once the offer is heard, or already fulfilled.</p><code>${escape(o.discovery)}</code><p>Fulfilled:</p><code>${escape(o.completion)}</code></details>`:''}</section>`).join('')}</div></details><small class="muted">Source: ${escape(q.source)}</small></article>`).join('') || '<p class="card">No matching quests.</p>';
    }
    document.getElementById('quest-search').oninput = render;
    render();
  } catch(error) {
    loaded = false;
    panel.textContent = `Quests unavailable: ${error.message}. Select this section again to retry.`;
  }
}
