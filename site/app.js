import { PATCH_VERSION } from './version.mjs';

let effects;

const themeButton = document.querySelector('#theme-toggle');
function updateThemeButton() {
  const dark = document.documentElement.dataset.theme === 'dark';
  const background = getComputedStyle(document.documentElement).getPropertyValue('--background').trim();
  document.documentElement.style.backgroundColor = background;
  document.body.style.backgroundColor = background;
  themeButton.setAttribute('aria-label', `Switch to ${dark ? 'light' : 'dark'} mode`);
  document.querySelector('#theme-label').textContent = dark ? 'LIGHT MODE' : 'DARK MODE';
  document.querySelector('meta[name="theme-color"]').content = background;
}
updateThemeButton();
themeButton.addEventListener('click', () => {
  const next = document.documentElement.dataset.theme === 'dark' ? 'light' : 'dark';
  document.documentElement.dataset.theme = next;
  try { localStorage.setItem('zoids-theme', next); } catch {}
  updateThemeButton();
});

for (const tabList of document.querySelectorAll('.edition-tabs')) {
  const tabs = [...tabList.querySelectorAll('[role="tab"]')];
  const screen = tabList.closest('.feature').querySelector('.game-shot');
  let transition = 0;
  async function selectTab(selected) {
    const request = ++transition;
    const previous = screen.querySelector('.game-panel:not([hidden])');
    const next = document.getElementById(selected.getAttribute('aria-controls'));
    if (previous !== next) {
      const closing = effects?.powerCrt(previous, false);
      if (closing) await closing.finished.catch(() => {});
      if (request !== transition) return;
    } else {
      if (previous.querySelector('.crt-beam')) effects?.powerCrt(previous, true);
      return;
    }
    for (const tab of tabs) {
      const active = tab === selected;
      tab.setAttribute('aria-selected', String(active));
      tab.tabIndex = active ? 0 : -1;
      document.getElementById(tab.getAttribute('aria-controls')).hidden = !active;
    }
    effects?.powerCrt(next, true);
  }
  for (const tab of tabs) {
    tab.addEventListener('click', () => selectTab(tab));
    tab.addEventListener('keydown', event => {
      let next;
      const index = tabs.indexOf(tab);
      if (event.key === 'ArrowRight') next = (index + 1) % tabs.length;
      else if (event.key === 'ArrowLeft') next = (index + tabs.length - 1) % tabs.length;
      else if (event.key === 'Home') next = 0;
      else if (event.key === 'End') next = tabs.length - 1;
      else return;
      event.preventDefault();
      selectTab(tabs[next]);
      tabs[next].focus();
    });
  }
}

const input = document.querySelector('#rom-input');
const zone = document.querySelector('#drop-zone');
const status = document.querySelector('#status');
const statusDots = document.querySelectorAll('.status-dot');
const patchButton = document.querySelector('#patch-button');
const buttonLabel = document.querySelector('#button-label');
const WORKER_TIMEOUT_MS = 30_000;
let rom = null;
let worker = null;
let workerTimer;
let busy = false;
const supported = Boolean(window.Worker && window.crypto?.subtle);

function setStatus(message, state = 'muted') {
  status.textContent = `Patch v${PATCH_VERSION} · ${message}`;
  status.dataset.error = String(state === 'error');
  for (const dot of statusDots) dot.className = `status-dot ${state}`;
}
function setBusy(value) {
  busy = value;
  input.disabled = value || !supported;
  zone.classList.toggle('busy', value);
  for (const dot of statusDots) dot.dataset.busy = String(value);
  patchButton.disabled = value || !rom || !supported;
  patchButton.setAttribute('aria-busy', String(value && Boolean(rom)));
}
function stopWorker() {
  clearTimeout(workerTimer);
  worker?.terminate();
  worker = null;
}
function failWorker(message) {
  stopWorker();
  setBusy(false);
  buttonLabel.textContent = 'Patch & download';
  setStatus(message, 'error');
}
function startWorker(action, file) {
  stopWorker();
  const task = new Worker(new URL('./worker.js', import.meta.url), { type: 'module' });
  worker = task;
  workerTimer = setTimeout(() => {
    failWorker('The operation timed out. Please try again.');
  }, WORKER_TIMEOUT_MS);
  task.onerror = () => {
    if (worker !== task) return;
    failWorker('The patcher could not start. Reload the page or try another browser.');
  };
  task.onmessage = ({ data }) => {
    if (worker !== task) return;
    stopWorker();
    buttonLabel.textContent = 'Patch & download';
    if (data.error) {
      if (action === 'validate') {
        rom = null;
        zone.classList.remove('selected');
      }
      setBusy(false);
      setStatus(data.error, 'error');
      return;
    }
    if (data.validated) {
      rom = file;
      zone.classList.add('selected');
      setBusy(false);
      setStatus('ROM verified. Ready to patch.', 'ready');
    } else if (data.target) {
      const url = URL.createObjectURL(new Blob([data.target], { type: 'application/octet-stream' }));
      const link = document.createElement('a');
      link.href = url;
      link.download = 'Zoids Legacy (USA) - Retranslation.gba';
      document.body.append(link);
      link.click();
      link.remove();
      setTimeout(() => URL.revokeObjectURL(url), 60000);
      setBusy(false);
      setStatus('Patch complete. Download ready.', 'ready');
      buttonLabel.textContent = 'Download again';
    }
  };
  task.postMessage({ action, rom: file });
}
function selectRom(files) {
  if (busy || !supported || !files.length) return;
  rom = null;
  zone.classList.remove('selected');
  patchButton.disabled = true;
  buttonLabel.textContent = 'Patch & download';
  if (files.length !== 1) {
    setStatus('Choose one ROM file at a time.', 'error');
    return;
  }
  const file = files[0];
  if (!/\.gba$/i.test(file.name)) {
    setStatus('Choose a .gba ROM file. Extract ZIP archives before you continue.', 'error');
    return;
  }
  if (file.size !== 8 * 1024 * 1024) {
    setStatus('Use an unmodified 8 MiB Zoids™ Legacy (USA) ROM.', 'error');
    return;
  }
  setBusy(true);
  setStatus('Checking your ROM…');
  try { startWorker('validate', file); } catch {
    failWorker('The patcher could not start. Reload the page or try another browser.');
  }
}
input.addEventListener('change', () => {
  selectRom(input.files);
  input.value = '';
});
for (const event of ['dragenter', 'dragover']) {
  zone.addEventListener(event, e => {
    e.preventDefault();
    if (!busy && supported) zone.classList.add('drag-over');
  });
}
zone.addEventListener('dragleave', e => {
  if (!zone.contains(e.relatedTarget)) zone.classList.remove('drag-over');
});
zone.addEventListener('drop', e => {
  e.preventDefault();
  zone.classList.remove('drag-over');
  selectRom(e.dataTransfer.files);
});
window.addEventListener('dragover', e => e.preventDefault());
window.addEventListener('drop', e => e.preventDefault());
patchButton.addEventListener('click', () => {
  if (!rom || busy) return;
  setBusy(true);
  buttonLabel.textContent = 'Applying patch…';
  setStatus('Applying the patch and checking the output…');
  try { startWorker('patch', rom); } catch {
    failWorker('The patcher could not start. Reload the page or try another browser.');
  }
});
if (!supported) {
  input.disabled = true;
  setStatus('Open this page over HTTPS or localhost in a modern browser to use the patcher.', 'error');
} else {
  setStatus('Select your USA ROM to begin.');
}
import('./effects.js').then(({ createEffects }) => {
  effects = createEffects();
}).catch(() => {
  clearTimeout(window.introFallback);
  document.documentElement.classList.remove('intro-pending', 'intro-running');
});
