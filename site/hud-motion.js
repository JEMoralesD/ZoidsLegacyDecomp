// Zentrix: 01_010.3 stutter, 08_101.2 bracket lock, 04_054.42 echoes,
// 06_076.4 scale slam, 01_005.9 slats, and motion library 4G stepped chevrons.
const FRAME_MS = 40;
const MAX_ANIMATIONS = 8;
const pick = items => items[Math.floor(Math.random() * items.length)];
const SVG_NS = 'http://www.w3.org/2000/svg';

// Keep traced lettering static; animate only the symbol's mechanical shapes.
const shapeGroups = {
  'decal-kit-hoist': [['chase', [1, 2]], ['index', [3]]],
  'decal-kit-jettison': [['signal', [0]]],
  'decal-kit-static-vent': [['index', [1]], ['shutter', [2]], ['chase', [3]]],
  'decal-kit-battery-vent': [['stutter', [0]], ['chase', [2]]],
  'decal-kit-caution': [['stutter', [0, 1, 2]], ['chase', [3]]],
  'decal-kit-ejection': [['stutter', [0]]],
  'decal-kit-control-strip': [['stutter', [0, 1]], ['chase', [2, 3]], ['chase', [5, 6, 7, 8, 9, 10, 11, 12]]],
  'decal-kit-control-warning': [['shutter', [0, 1, 2, 3]]],
};

function prepareDecalShapes() {
  for (const use of document.querySelectorAll('.decal-motion > use[href^="#decal-kit-"]')) {
    const symbol = document.querySelector(use.getAttribute('href'));
    const groups = shapeGroups[symbol.id];
    if (!groups) continue;
    const graphic = document.createElementNS(SVG_NS, 'svg');
    graphic.setAttribute('viewBox', symbol.getAttribute('viewBox'));
    graphic.setAttribute('overflow', 'visible');
    for (const name of ['x', 'y', 'width', 'height']) {
      if (use.hasAttribute(name)) graphic.setAttribute(name, use.getAttribute(name));
    }
    for (const child of symbol.children) graphic.append(child.cloneNode(true));
    const shapes = [...graphic.children];
    for (const [motion, indices] of groups) {
      const group = document.createElementNS(SVG_NS, 'g');
      group.classList.add('decal-motion');
      group.dataset.motion = motion;
      graphic.insertBefore(group, shapes[indices[0]]);
      for (const index of indices) group.append(shapes[index]);
    }
    use.parentElement.classList.remove('decal-motion');
    use.parentElement.removeAttribute('data-motion');
    use.replaceWith(graphic);
  }
}

export function createHudMotion(elements) {
  prepareDecalShapes();
  const scopes = [...elements].map(element => ({
    element,
    parts: [...element.querySelectorAll('.flicker-unit, .decal-motion')],
  }));
  const visible = new Set();
  const animations = new Map();
  let active = false;
  let timer;
  let lastDecal;
  let nextGlyphChain = 0;

  const unavailable = part => part.closest('button:disabled, .menu-control.busy, [aria-busy="true"], [hidden]');
  function animationCount(part) {
    switch (part.dataset.motion) {
      case 'chase':
      case 'slats': return part.children.length;
      case 'echo': return part.querySelectorAll('.decal-echo').length;
      case 'glyph-chain': return part.querySelectorAll('.glyph-cell').length;
      default: return 1;
    }
  }
  // Reserve the whole effect so related shapes animate together within the limit.
  const available = part => !unavailable(part) && ![...animations.values()].includes(part) &&
    animations.size + animationCount(part) <= MAX_ANIMATIONS;

  function animate(part, frames, owner = part, delay = 0) {
    const animation = part.animate(frames.map(frame => ({ ...frame, easing: 'steps(1, end)' })), {
      id: `hud-${owner.dataset.motion || 'stutter'}`,
      duration: (frames.length - 1) * FRAME_MS,
      delay: delay * FRAME_MS,
      fill: 'backwards',
    });
    animations.set(animation, owner);
    animation.addEventListener('finish', () => { animations.delete(animation); animation.cancel(); }, { once: true });
    animation.addEventListener('cancel', () => animations.delete(animation), { once: true });
    return animation;
  }

  function stutter(part) {
    const frames = Array.from({ length: 21 }, () => ({ opacity: 1 }));
    const drops = 3 + Math.floor(Math.random() * 2);
    let frame = 1;
    for (let index = 0; index < drops; index++) {
      frames[frame] = { opacity: .35 + Math.random() * .25 };
      frame += 2 + Math.floor(Math.random() * 2);
    }
    animate(part, frames);
  }

  function chase(part) {
    const cells = [...part.children];
    if (Math.random() < .5) cells.reverse();
    const stride = 2 + Math.floor(Math.random() * 3);
    for (const [index, cell] of cells.entries()) {
      const frames = Array.from({ length: cells.length * stride + 2 }, (_, frame) => ({
        opacity: Math.floor(frame / stride) === index ? 1 : .2,
      }));
      frames[frames.length - 1] = { opacity: 1 };
      animate(cell, frames, part);
    }
  }

  function slam(part) {
    animate(part, [
      { transform: 'scale(1.45)', opacity: .2 },
      { transform: 'scale(1.18)', opacity: .55 },
      { transform: 'scale(1.04)', opacity: 1 },
      { transform: 'scale(1)', opacity: 1 },
      { transform: 'scale(1)', opacity: .5 },
      { transform: 'scale(1)', opacity: 1 },
    ]);
  }

  // Zentrix 4F.7: 01_005.1 and 14_198.6 replace glyphs on hard cuts every 1–5 HUD frames.
  function glyphChain(part) {
    const cells = [...part.querySelectorAll('.glyph-cell')];
    const current = Number(part.dataset.restGlyph || 0);
    const settled = Math.random() < .5 ? 0 : 3;
    const order = [current, 1, 2, 3, 1, settled];
    const states = order.flatMap(index => Array(1 + Math.floor(Math.random() * 5)).fill(index));
    states.push(settled);
    part.dataset.restGlyph = String(settled);
    const start = document.timeline.currentTime;
    for (const [index, cell] of cells.entries()) {
      const animation = animate(cell, states.map(state => ({ opacity: Number(state === index) })), part);
      animation.startTime = start;
    }
  }

  function echo(part) {
    for (const [index, copy] of part.querySelectorAll('.decal-echo').entries()) {
      const direction = index ? 1 : -1;
      animate(copy, [
        { transform: 'translateX(0) scale(1)', opacity: 0 },
        { transform: 'translateX(0) scale(1)', opacity: .65 },
        { transform: `translateX(${direction * 3}px) scale(1.05)`, opacity: .45 },
        { transform: `translateX(${direction * 6}px) scale(1.1)`, opacity: .3 },
        { transform: `translateX(${direction * 9}px) scale(1.15)`, opacity: .15 },
        { transform: `translateX(${direction * 12}px) scale(1.2)`, opacity: 0 },
      ], part);
    }
  }

  function slats(part) {
    const cells = [...part.children];
    if (Math.random() < .5) cells.reverse();
    cells.forEach((cell, index) => animate(cell, [
      { transform: 'translateY(-6px)', opacity: 0 },
      { transform: 'translateY(-3px)', opacity: .4 },
      { transform: 'translateY(0)', opacity: 1 },
      { transform: 'translateY(0)', opacity: .45 },
      { transform: 'translateY(0)', opacity: 1 },
    ], part, index));
  }

  function shutter(part) {
    animate(part, [
      { clipPath: 'inset(0 100% 0 0)' },
      { clipPath: 'inset(0 65% 0 0)' },
      { clipPath: 'inset(0 65% 0 0)' },
      { clipPath: 'inset(0 30% 0 0)' },
      { clipPath: 'inset(0)' },
    ]);
  }

  function index(part) {
    const direction = Math.random() < .5 ? -1 : 1;
    animate(part, [
      { transform: 'rotate(0deg)', opacity: 1 },
      { transform: `rotate(${direction * 30}deg)`, opacity: .45 },
      { transform: `rotate(${direction * 60}deg)`, opacity: 1 },
      { transform: `rotate(${direction * 90}deg)`, opacity: 1 },
      { transform: `rotate(${direction * 90}deg)`, opacity: .5 },
      { transform: 'rotate(0deg)', opacity: 1 },
    ]);
  }

  function signal(part) {
    animate(part, [
      { transform: 'translateX(-8px)', opacity: .2 },
      { transform: 'translateX(-4px)', opacity: .6 },
      { transform: 'translateX(0)', opacity: 1 },
      { transform: 'translateX(0)', opacity: .3 },
      { transform: 'translateX(0)', opacity: 1 },
    ]);
  }

  const effects = { chase, slam, echo, slats, shutter, stutter, index, signal };
  function queue() {
    clearTimeout(timer);
    if (!active || !visible.size) return;
    timer = setTimeout(() => {
      const parts = scopes.filter(scope => visible.has(scope.element)).flatMap(scope => scope.parts).filter(available);
      if (animations.size < MAX_ANIMATIONS) {
        const now = performance.now();
        const glyphPart = parts.find(part => part.dataset.motion === 'glyph-chain');
        if (glyphPart && now >= nextGlyphChain) {
          glyphChain(glyphPart);
          nextGlyphChain = now + 1100 + Math.random() * 500;
        }
        const decalPart = pick(parts.filter(part => available(part) && part.classList.contains('decal-motion') &&
          part.dataset.motion !== 'glyph-chain' && part !== lastDecal));
        if (decalPart) {
          effects[decalPart.dataset.motion](decalPart);
          lastDecal = decalPart;
        }
        const framePart = pick(parts.filter(part => available(part) && !part.classList.contains('decal-motion')));
        if (framePart) stutter(framePart);
      }
      queue();
    }, 200 + Math.random() * 400);
  }

  const visibilityObserver = new IntersectionObserver(entries => {
    for (const { target, isIntersecting } of entries) {
      if (isIntersecting) visible.add(target);
      else {
        visible.delete(target);
        for (const [animation, part] of animations) {
          if (target.contains(part)) animation.cancel();
        }
      }
    }
    queue();
  });
  scopes.forEach(({ element }) => visibilityObserver.observe(element));

  const controlsObserver = new MutationObserver(() => {
    for (const [animation, part] of animations) {
      if (unavailable(part)) animation.cancel();
    }
  });
  document.querySelectorAll('.menu-control').forEach(control => {
    controlsObserver.observe(control, { attributes: true, attributeFilter: ['disabled', 'aria-busy', 'class'] });
  });

  function refresh(enabled) {
    active = enabled;
    for (const animation of animations.keys()) animation.cancel();
    animations.clear();
    queue();
  }

  return { refresh };
}
