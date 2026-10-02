const GLITCH_DURATION = 1.5;

function smoothstep(start, end, value) {
  const t = Math.max(0, Math.min(1, (value - start) / (end - start)));
  return t * t * (3 - 2 * t);
}

export function createAnalogPanels(elements, reducedMotion) {
  const tile = document.createElement('canvas');
  const context = tile.getContext('2d');
  if (!context) return { refresh() {}, async prepare() {} };

  const panels = [...elements].map(element => {
    const texture = document.createElement('div');
    texture.className = 'crt-texture';
    texture.setAttribute('aria-hidden', 'true');
    element.classList.add('crt-panel');
    element.append(texture);
    return { element, texture };
  });
  const visible = new Set();
  const filter = document.querySelector('#panel-crt');
  const redOffset = filter.querySelector('[result="red-signal"]');
  const blueOffset = filter.querySelector('[result="blue-signal"]');
  let image;
  let pixelRatio = 0;
  let enabled = true;
  let activeSince = null;
  let elapsed = 0;
  let timer = 0;
  let frame = 0;
  let glitchStarted = -GLITCH_DURATION;
  let glitchWindow = 30;
  let nextGlitch = glitchWindow + Math.random() * (15 - GLITCH_DURATION);

  function drawTexture() {
    const ratio = Math.min(devicePixelRatio || 1, 2);
    if (pixelRatio === ratio) return;
    pixelRatio = ratio;

    // Cache a repeating grille instead of repainting a full-size canvas for each panel.
    // Beam profile: https://github.com/libretro/glsl-shaders/blob/master/crt/shaders/crt-lottes.glsl
    const lineHeight = Math.max(3, Math.round(3 * ratio));
    tile.width = 96;
    tile.height = lineHeight * 16;
    const grille = context.createImageData(tile.width, tile.height);
    for (let y = 0; y < tile.height; y++) {
      const distance = ((y % lineHeight + .5) / lineHeight - .5) * 2;
      const beam = .9 + .1 * Math.exp(-4 * distance * distance);
      for (let x = 0; x < tile.width; x++) {
        const offset = (y * tile.width + x) * 4;
        const grain = 1 - Math.random() * .025;
        const slot = x % 3 === 2 ? .8 : 1;
        grille.data[offset + 3] = 255 * (1 - beam * grain * slot);
      }
    }
    context.putImageData(grille, 0, 0);
    const textureUrl = tile.toDataURL();
    const textureSize = `${tile.width / ratio}px ${tile.height / ratio}px`;

    image = new Image();
    image.src = textureUrl;
    for (const { texture } of panels) {
      texture.style.setProperty('--crt-mask', `url("${textureUrl}")`);
      texture.style.setProperty('--crt-mask-size', textureSize);
    }
  }

  function updateEffects() {
    clearTimeout(timer);
    cancelAnimationFrame(frame);
    const active = activeSince !== null;
    for (const { element } of panels) {
      element.classList.toggle('crt-enabled', active);
      element.classList.toggle('crt-visible', active && visible.has(element));
    }
    if (!active || !visible.size) {
      panels.forEach(({ element }) => element.classList.remove('crt-burst'));
      return;
    }

    const seconds = elapsed + (performance.now() - activeSince) / 1000;
    while (seconds >= nextGlitch) {
      glitchStarted = nextGlitch;
      // Two random slots per minute, with no burst during the first 30 seconds.
      glitchWindow += glitchWindow < 60 ? 15 : 30;
      const slotDuration = glitchWindow < 60 ? 15 : 30;
      nextGlitch = glitchWindow + Math.random() * (slotDuration - GLITCH_DURATION);
    }
    const progress = smoothstep(0, GLITCH_DURATION, seconds - glitchStarted);
    const strength = 4 * progress * (1 - progress);
    redOffset.setAttribute('dx', 1.5 * strength);
    blueOffset.setAttribute('dx', -1.5 * strength);
    for (const { element } of panels) {
      element.classList.toggle('crt-burst', strength > 0 && visible.has(element));
    }

    if (seconds < glitchStarted + GLITCH_DURATION) {
      timer = setTimeout(() => { frame = requestAnimationFrame(updateEffects); }, 50);
    } else {
      timer = setTimeout(updateEffects, Math.max(0, (nextGlitch - seconds) * 1000));
    }
  }

  function refresh() {
    const active = enabled && !document.hidden && !reducedMotion.matches;
    const now = performance.now();
    if (active && activeSince === null) activeSince = now;
    if (!active && activeSince !== null) {
      elapsed += (now - activeSince) / 1000;
      activeSince = null;
    }
    if (active) drawTexture();
    updateEffects();
  }

  async function prepare() {
    if (document.hidden || reducedMotion.matches) return;
    try {
      await image.decode();
    } catch {
      enabled = false;
      refresh();
    }
    // Decode the shared grille before the intro starts revealing panels.
    await new Promise(resolve => requestAnimationFrame(() => requestAnimationFrame(resolve)));
  }

  const visibilityObserver = new IntersectionObserver(entries => {
    for (const { target, isIntersecting } of entries) {
      if (isIntersecting) visible.add(target);
      else visible.delete(target);
    }
    updateEffects();
  });
  for (const { element } of panels) {
    visibilityObserver.observe(element);
  }
  window.addEventListener('resize', () => {
    if (activeSince !== null) drawTexture();
  });
  return { refresh, prepare };
}
